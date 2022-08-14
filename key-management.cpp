#include "key-management.hpp"

#include "json/json.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstring>
#include <curl/curl.h>
#include <openssl/sha.h>

using namespace std;

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (struct MemoryStruct *)userp;
    
    mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
    
    if(mem->memory == NULL) {
        fprintf(stderr, "Not enough memory\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

static string sha256(const string str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    stringstream ss;

    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);


    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }

    return ss.str();
}

KeyManagement::KeyManagement(char *name) {
    curl_global_init(CURL_GLOBAL_ALL);
    this->curl = curl_easy_init();
    this->list = nullptr;
    this->chunk.memory = (char *)malloc(1);
    this->chunk.size = 0;
    this->name = string(name);
}

KeyManagement::~KeyManagement() {
    this->list = nullptr;
    curl_easy_cleanup(this->curl);
    curl_global_cleanup();
}

string KeyManagement::KeyGenerate() {
    string role = "generator";
    string result;
    string key;
    Json::Reader reader;
    Json::Value root;

    result = Connect(role);

    if(reader.parse(result, root) == false) {
        std:cerr << "Failed to parse Json : " << reader.getFormattedErrorMessages() << endl;
    }

    key = root["data"]["key"].asString();

    return key;
}

void KeyManagement::KeyExchange() {
    string role = "exchange";
    string result;

    result = Connect(role);
    
    cout << result << endl;
}

string KeyManagement::getLifecycle() {
    string role = "lifecycle";
    string result;
    string lc;
    Json::Reader reader;
    Json::Value root;

    result = Connect(role);

    if(reader.parse(result, root) == false) {
        std:cerr << "Failed to parse Json : " << reader.getFormattedErrorMessages() << endl;
    }

    lc = root["data"]["lifecycle"].asString();
    
    return lc;
}

string KeyManagement::Connect(string role) {
    string baseURL = "14.7.123.170:20010/";
    string result;
    char *URL;
    size_t size = (baseURL.length() + role.length());

    string header_author_tmp = "Authorization: " + sha256(this->name);

    cout << header_author_tmp << endl;

    char *header_author = (char *)malloc(header_author_tmp.length() + 1);
    
    memcpy(header_author, header_author_tmp.c_str(), header_author_tmp.length());
    header_author[header_author_tmp.length()] = 0x00;
    
    cout << header_author << endl;

    URL = (char *)malloc(size + 1);

    strncpy(URL, (baseURL + role).c_str(), size);
    URL[size] = 0x00;

    curl_easy_setopt(this->curl, CURLOPT_URL, URL);
    curl_easy_setopt(this->curl, CURLOPT_NOPROGRESS, 1L);

    this->list = curl_slist_append(this->list, "Content-Type: application/json");
    this->list = curl_slist_append(this->list, header_author);
    curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, list);

    curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, (void *)&chunk);

    this->res = curl_easy_perform(this->curl);

    if (this->res != CURLE_OK) {
        cerr << "Failed to connect KDC Server" << endl;
        exit(1);
    }

    free(URL);

    result = string(this->chunk.memory);

    free(this->chunk.memory);
    this->chunk.size = 0;
    
    free(header_author);

    return result;
}