#include "key-management.hpp"

#include "json/json.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <curl/curl.h>
#include <openssl/sha.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/aes.h>

using namespace std;

static RSA *createRSA(unsigned char *key, bool pub)
{
    RSA *rsa = NULL;
    BIO *keybio;
    keybio = BIO_new_mem_buf(key, -1); // 읽기 전용 메모리 만들기 BIO
    if (keybio == NULL)
    {
        printf("Failed to create key BIO");
        return 0;
    }

    /* PEM형식인 키 파일을 읽어와서 RSA 구조체 형식으로 변환 */

    if (pub) // PEM public 키로 RSA 생성
    {
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, NULL, NULL, NULL);
    }
    else // PEM private 키로 RSA 생성
    {
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
    }

    if (rsa == NULL)
    {
        printf("Failed to create RSA");
    }

    return rsa;
}

static int public_decrypt(unsigned char *enc_data, int data_len, unsigned char *key, unsigned char *decrypted)
{
    RSA *rsa = createRSA(key, true);

    int result = RSA_public_decrypt(data_len, enc_data, decrypted, rsa, rsapad);

    return result;
}

static int private_decrypt(unsigned char *enc_data, int data_len, unsigned char *key, unsigned char *decrypted)
{
    RSA *rsa = createRSA(key, 0);
    int result = RSA_private_decrypt(data_len, enc_data, decrypted, rsa, RSA_PKCS1_OAEP_PADDING);
    return result;
}

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);

    if (mem->memory == NULL)
    {
        fprintf(stderr, "Not enough memory\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

static void base64_decode(const string &in, unsigned char *tmp)
{

    typedef unsigned char uchar;
    static const std::string b = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string out;
    std::vector<int> T(256, -1);
    int j = 0;

    for (int i = 0; i < 64; i++)
        T[b[i]] = i;

    int val = 0, valb = -8;

    for (uchar c : in)
    {
        if (T[c] == -1)
            break;

        val = (val << 6) + T[c];
        valb += 6;

        if (valb >= 0)
        {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    for (int i = 0; i < out.length(); i++)
    {
        tmp[i] = (unsigned char)out[i];
    }
}

static string sha256(const string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    stringstream ss;

    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }

    return ss.str();
}

static unsigned char *parseBytes(const char *hex_str)
{
    char tmp[3];
    int i;
    int hex_str_len = strlen(hex_str);
    unsigned char *bytes = NULL;

    if ((hex_str_len % 2) != 0)
        return NULL;
    bytes = (unsigned char *)malloc(hex_str_len / 2);
    if (!bytes)
        return NULL;

    for (i = 0; i < hex_str_len / 2; i++)
    {
        memcpy(tmp, hex_str + (i * 2), 2);
        tmp[2] = 0;
        bytes[i] = (unsigned char)strtoul(tmp, NULL, 16);
    }

    return bytes;
}

static void AES_Decrypt(unsigned char *sKey, unsigned char *FPE_Key, char *out, int byte)
{
    AES_KEY key;

    AES_set_decrypt_key((const unsigned char *)sKey, 256, &key);
    // AES_decrypt(FPE_Key, result, &key);

    for (int i = 0; i < byte / 16; i++)
    {
        AES_decrypt(FPE_Key + (i * 16), (unsigned char *)out + (i * 16), &key);
    }
}

KeyManagement::KeyManagement(char *name)
{
    curl_global_init(CURL_GLOBAL_ALL);
    this->list = nullptr;
    this->chunk.memory = (char *)malloc(1);
    this->chunk.size = 0;
    this->name = string(name);
}

KeyManagement::KeyManagement(string name)
{
    curl_global_init(CURL_GLOBAL_ALL);
    this->list = nullptr;
    this->chunk.memory = (char *)malloc(1);
    this->chunk.size = 0;
    this->name = name;
}

KeyManagement::~KeyManagement()
{
    this->list = nullptr;
    curl_global_cleanup();
}

void KeyManagement::KeyGenerate()
{
    string role = "generator";
    string result;
    string key;
    Json::Reader reader;
    Json::Value root;

    string header_author_tmp = "Authorization: " + sha256(this->name);

    char *header_author = (char *)malloc(header_author_tmp.length() + 1);

    memcpy(header_author, header_author_tmp.c_str(), header_author_tmp.length());
    header_author[header_author_tmp.length()] = 0x00;

    this->list = curl_slist_append(this->list, "Accept: application/json");
    this->list = curl_slist_append(this->list, header_author);

    result = Connect(role);

    free(header_author);
}

void KeyManagement::pubKeyExchange()
{
    string role = "exchange/pkey";

    string result;
    Json::Reader reader;
    Json::Value root;

    string header_author_tmp = "Authorization: " + sha256(this->name);

    char *header_author = (char *)malloc(header_author_tmp.length() + 1);

    memcpy(header_author, header_author_tmp.c_str(), header_author_tmp.length());
    header_author[header_author_tmp.length()] = 0x00;

    this->list = curl_slist_append(this->list, "Accept: application/json");
    this->list = curl_slist_append(this->list, header_author);
    this->list = curl_slist_append(this->list,
                                   "Authorization: LS0tLS1CRUdJTiBQVUJMSUMgS0VZLS0tLS0KTUlJQ0lqQU5CZ2txaGtpRzl3MEJBUUVGQUFPQ0FnOEFNSUlDQ2dLQ0FnRUF5ZmZCWklMRUdtelJGMVdWVXNUKwpGYmo1TExmeVNTR2RPZEVzZktXVElGTllCd3NEZWJpUmxmYkZQTUNrNUdzTkRQaGpuRnN3TGlGU1owZG5XczVYCkc5T09HRzN2bFNGVGxrdkh4SVo2cG9pM3QzQ0E1enlHemtGcHdRYnBBNmtBc3VNY0JLaElTeWRWUjdyWWRSUjkKd3E4U3N0cnpkQ0xqTHhXUFUzcjJlY2JHM0x5dkFRWmszSVAvUldEbldNMlYveWphVkRyN1BGSm5FRTNLdHFFTwpnM05HNjZmSlM1RVdqNGUreGJmUDVhSVlNMWRyRWlyQWVJQXpwWHNXUEtTZ1NodTBQQW1yaWZLMWVZU0ZTT2UzCnZ1NEpUZ3lzSWdvc2piTE1RQ2NXT04zTkhEd0xZcDZ0dzNuQVk3T2p4NkZsY2gzczd6ZHhLRVZsRGoyVEdUcEEKRXMwZFdmbEN0ME1VLzd3dlYxTjJWR2lrUysvbjNJMmJiWE5LbHJHYVE1S083NHZtbmhWTTBwOHY3SUZsdHZCdgpoZWR1VkxXeE9DU25VYjBEd3FuYW42ampSN1F5NVpUQ201QTlxMDJFWkJVNkxicHpwS1FsMVczVmthWFhDQ01uClVXcVl6ZmxYQ2p2VU5lZ0Z5TFRnR1ZSN051RWlEQlpxMzc1M2lKM0UvVStJN0g0OUM2djR4ZGQvTlpvU0UwV1UKZ0ZJdnYxa2JwTEpVTHdacXpwTkVkd3lTK2JyWFYvdmxBbjFUUUNZVDgrM0EvMFJBSlJXOEptTlhHcHpscXNWbApZVWkxL3puYVRwMUVYZWU4MlV2ZkxLSTNnTXArUFZYKzZDL0xFMFJoTVRQV3VjY3BkYzY0cU9wS1NaRW1DRWJOCklPMUxyWmRnUGVVSythVG52Uk1vaTRNQ0F3RUFBUT09Ci0tLS0tRU5EIFBVQkxJQyBLRVktLS0tLQ==");

    result = Connect(role);

    if (reader.parse(result, root) == false)
    {
        cerr << "Failed to parse Json : " << reader.getFormattedErrorMessages() << endl;
    }

    this->uuid = root["data"]["nodeId"].asString();
    this->pubkey = root["data"]["apiPubKey"].asString();

    free(header_author);
}

void KeyManagement::KeyExchange()
{
    string role = "exchange";
    string result;
    Json::Reader reader;
    Json::Value root;

    string header_author_tmp = "Authorization: " + sha256(this->name);
    string header_uuid_tmp = "Authorization: " + this->uuid;

    char *header_author = (char *)malloc(header_author_tmp.length() + 1);
    memcpy(header_author, header_author_tmp.c_str(), header_author_tmp.length());
    header_author[header_author_tmp.length()] = 0x00;

    char *header_uuid = (char *)malloc(header_uuid_tmp.length() + 1);
    memcpy(header_uuid, header_uuid_tmp.c_str(), header_uuid_tmp.length());
    header_uuid[header_uuid_tmp.length()] = 0x00;

    this->list = curl_slist_append(this->list, "Accept: application/json");
    this->list = curl_slist_append(this->list, header_author);
    this->list = curl_slist_append(this->list, header_uuid);

    result = Connect(role);

    if (reader.parse(result, root) == false)
    {
        // cerr << "Failed to parse Json : " << reader.getFormattedErrorMessages() << endl;
    }

    string skey_64 = root["data"]["sKey"].asString();
    string fpekey_64 = root["data"]["fpeKey"].asString();
    string fpetweak_64 = root["data"]["fpeTweak"].asString();

    char *privkey_char;

    unsigned char skey_enc[4096];

    base64_decode(skey_64, skey_enc);

    unsigned char fpekey_enc[4096];

    base64_decode(fpekey_64, fpekey_enc);

    unsigned char fpetweak_enc[4096];

    base64_decode(fpetweak_64, fpetweak_enc);

    ifstream in("/usr/src/app/ndn-cxx/ndn-cxx/security/transform/curl/curl/private_key.pem");
    string privKey;

    if (in.is_open())
    {
        in.seekg(0, ios::end);

        int size = in.tellg();

        privKey.resize(size);

        in.seekg(0, ios::beg);

        in.read(&privKey[0], size);
    }
    else
    {
        cerr << "Private Key Not Found" << endl;
    }

    privkey_char = (char *)malloc(privKey.length() + 1);
    memcpy(privkey_char, privKey.c_str(), privKey.length());
    unsigned char skey[4096] = {
        0,
    };

    private_decrypt(skey_enc, 128, (unsigned char *)privkey_char, (unsigned char *)skey);

    char fpekey[100];
    char fpetweak[100];

    AES_Decrypt(skey, fpekey_enc, fpekey, 64);
    AES_Decrypt(skey, fpetweak_enc, fpetweak, 32);

    this->fpekey = string(fpekey);
    this->fpetweak = string(fpetweak);
    this->sesskey = string((char *)skey);

    free(header_author);
    free(header_uuid);
}

string KeyManagement::getLifecycle()
{
    string role = "lifecycle";
    string result;
    string lc;
    Json::Reader reader;
    Json::Value root;

    result = Connect(role);

    if (reader.parse(result, root) == false)
    {

        cerr << "Failed to parse Json : " << reader.getFormattedErrorMessages() << endl;
    }

    lc = root["data"]["lifecycle"].asString();

    return lc;
}

string KeyManagement::Connect(string role)
{
    string baseURL = "logos.sch.ac.kr:20100/";
    string result;

    char *URL;
    size_t size = (baseURL.length() + role.length());

    URL = (char *)malloc(size + 1);
    strncpy(URL, (baseURL + role).c_str(), size);
    URL[size] = 0x00;

    this->curl = curl_easy_init();

    curl_easy_setopt(this->curl, CURLOPT_URL, URL);
    curl_easy_setopt(this->curl, CURLOPT_NOPROGRESS, 1L);

    curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, this->list);

    curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, (void *)&chunk);

    this->res = curl_easy_perform(this->curl);

    if (this->res != CURLE_OK)
    {
        cerr << "Failed to connect KDC Server" << endl;
        exit(1);
    }

    result = string(this->chunk.memory);

    // free(this->chunk.memory);
    this->chunk.size = 0;

    this->list = nullptr;

    free(URL);

    return result;
}

string KeyManagement::getFPEkey()
{
    return this->fpekey;
}

string KeyManagement::getFPEtweak()
{
    return this->fpetweak;
}

string KeyManagement::getSessKey()
{
    return this->sesskey;
}

string KeyManagement::getPubkey()
{
    return this->pubkey;
}