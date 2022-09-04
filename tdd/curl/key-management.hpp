#include <iostream>
#include <string>
#include <cstring>
#include <curl/curl.h>
#include <openssl/rsa.h>

using namespace std;

static int rsapad = RSA_PKCS1_PADDING;

static RSA *createRSA(unsigned char *, bool);

static int public_decrypt(unsigned char *, int, unsigned char *, unsigned char *);

static void base64_decode(const string &in, char *);

static int private_decrypt(unsigned char *, int, unsigned char *, unsigned char *);


typedef struct MemoryStruct {
    char *memory;
    size_t size;
} MemoryStruct;

static size_t
WriteMemoryCallback(void *, size_t, size_t, void *);

static string sha256(const string);

class KeyManagement {
public:
    KeyManagement(char *);

    ~KeyManagement();

    void KeyGenerate();

    void pubKeyExchange();

    void KeyExchange();

    string getLifecycle();

    string Connect(string);

    string getFPEkey();

    string getFPEtweak();

    string getSessKey();

    string getPubkey();

private:
    CURL *curl;
    CURLcode res;
    struct curl_slist *list;
    MemoryStruct chunk;
    string name;
    string uuid;
    string pubkey;
    string fpekey;
    string fpetweak;
    string sesskey;
};