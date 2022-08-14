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
    
static string sha256(const string);

class KeyManagement {
public:
    KeyManagement(char *);
    ~KeyManagement();
    string KeyGenerate();
    void KeyExchange();
    string getLifecycle();
    string Connect(string);

private:
    CURL *curl;
    CURLcode res;
    struct curl_slist *list;
    MemoryStruct chunk;
    string name;
};