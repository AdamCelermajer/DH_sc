// Reference-library probe only: game parser parity is established separately.
#include "tinyxml.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>

static void quoted(const char* input) {
    std::cout << '"';
    for (const unsigned char* p = reinterpret_cast<const unsigned char*>(input); *p; ++p) {
        switch (*p) {
            case '"': std::cout << "\\\""; break;
            case '\\': std::cout << "\\\\"; break;
            case '\n': std::cout << "\\n"; break;
            case '\r': std::cout << "\\r"; break;
            case '\t': std::cout << "\\t"; break;
            default:
                if (*p < 32) {
                    const char* hex = "0123456789abcdef";
                    std::cout << "\\u00" << hex[*p >> 4] << hex[*p & 15];
                } else std::cout << *p;
        }
    }
    std::cout << '"';
}
static void element(const TiXmlElement* e) {
    std::cout << "{\"tag\":"; quoted(e->Value()); std::cout << ",\"attributes\":{";
    bool comma = false;
    for (auto a = e->FirstAttribute(); a; a = a->Next()) {
        if (comma) std::cout << ',';
        quoted(a->Name()); std::cout << ':'; quoted(a->Value()); comma = true;
    }
    std::cout << "},\"children\":["; comma = false;
    for (auto c = e->FirstChildElement(); c; c = c->NextSiblingElement()) {
        if (comma) std::cout << ',';
        element(c); comma = true;
    }
    std::cout << "]}";
}
int main(int argc, char** argv) {
    try {
        if (argc != 2) throw std::runtime_error("Expected one XML input path");
        std::ifstream file(argv[1], std::ios::binary);
        if (!file) throw std::runtime_error("XML input unavailable");
        std::string raw{std::istreambuf_iterator<char>(file), {}};
        if (raw.size() > 16u*1024u*1024u || raw.find('\0') != std::string::npos)
            throw std::runtime_error("XML input outside probe domain");
        TiXmlDocument document;
        document.Parse(raw.c_str(), nullptr, TIXML_ENCODING_UNKNOWN);
        std::cout << "{\"xml_error\":" << document.ErrorId()
                  << ",\"error_row\":" << document.ErrorRow()
                  << ",\"error_column\":" << document.ErrorCol()
                  << ",\"error_description\":";
        quoted(document.ErrorDesc());
        std::cout << ",\"root\":";
        if (document.RootElement()) element(document.RootElement());
        else std::cout << "null";
        std::cout << "}\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
