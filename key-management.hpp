#include <iostream>
#include <string>
#include <cstring>
#include <curl/curl.h>

using namespace std;

typedef struct MemoryStruct {
    char *memory;
    size_t size;
} MemoryStruct;

static size_t
WriteMemoryCallback(void *, size_t, size_t, void *);

class KeyManagement {
public:
    KeyManagement();
    ~KeyManagement();
    string KeyGenerate();
    void KeyExchange();
    void getLifecycle();
    string Connect(string);

private:
    CURL *curl;
    CURLcode res;
    struct curl_slist *list;
    MemoryStruct chunk;
};