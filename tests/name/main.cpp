#include <iostream>

class Name {
public:
    void validate(std::string uri);
};

void Name::validate(std::string uri) {
    if (uri.empty())
        return;

    std::cout << "First :: " << uri << std::endl;

    size_t iColon = uri.find(':');
    if (iColon != std::string::npos) {
        // Make sure the colon came before a '/'.
        size_t iFirstSlash = uri.find('/');
        if (iFirstSlash == std::string::npos || iColon < iFirstSlash) {
            // Omit the leading protocol such as ndn:
            uri.erase(0, iColon + 1);
        }
    }

    // Trim the leading slash and possibly the authority.
    if (uri[0] == '/') {
        if (uri.size() >= 2 && uri[1] == '/') {
            // Strip the authority following "//".
            size_t iAfterAuthority = uri.find('/', 2);
            if (iAfterAuthority == std::string::npos)
                // Unusual case: there was only an authority.
                return;
            else {
                uri.erase(0, iAfterAuthority + 1);
            }
        } else {
            uri.erase(0, 1);
        }
    }

    std::cout << "Final :: " << uri << std::endl;
}


int main() {
    Name T = Name();
    std::string uri = "fpe:///hello/world";

    T.validate(uri);

    std::cout << "\n";

    std::string valid = "/hello/world/seq=1";
    size_t length = valid.find("seq=");

    std::cout << "valid:: " << valid << std::endl;
    std::cout << "length:: " << length << std::endl;
    std::cout << "seq:: " << valid.substr(0, length - 1) << std::endl;

    std::cout << "\n";

    std::string test[] = {"seq="};

    for (auto cmp: test) {
        if (valid.compare(cmp) == 0) {
            std::cout << "check = 1:: " << valid << std::endl;
            break;
        } else {
            std::cout << "check = 0:: " << valid << std::endl;
        }
    }

    return 0;
}
