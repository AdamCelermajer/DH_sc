#include "tinyxml-and-c-libs/tixml_dh.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    TiXmlPrinterDH printer;
    TiXmlElement root("root");
    CHECK(printer.VisitEnter(root, nullptr));
    CHECK(printer.buffer == "<root");

    TiXmlPrinterDH nested;
    nested.depth = 2;
    TiXmlElement child("item");
    nested.VisitEnter(child, nullptr);
    CHECK(nested.buffer == "        <item");

    std::printf("%s\n", failures ? "tixml_dh: FAILED" : "tixml_dh: ok");
    return failures != 0;
}
