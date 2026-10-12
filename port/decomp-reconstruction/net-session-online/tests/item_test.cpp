#include "net-session-online/item.hpp"

#include <cstdio>
#include <string>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    slim::XmlNode node;
    node.AddChild(slim::XmlNode("id", "item-7"));
    node.AddChild(slim::XmlNode("title", "Sword"));
    node.AddChild(slim::XmlNode("button_right", "Close"));

    item a;
    a.read(&node);
    CHECK(a.FieldForTest(0) == "item-7");
    CHECK(a.FieldForTest(1) == "Sword");
    CHECK(a.FieldForTest(2).empty());        // no "description" child: field kept
    CHECK(a.FieldForTest(11) == "Close");

    // Copy and assignment carry all twelve fields.
    item b(a);
    CHECK(b.FieldForTest(1) == "Sword");
    CHECK(b.FieldForTest(11) == "Close");
    item c;
    c = b;
    CHECK(c.FieldForTest(0) == "item-7");
    c = c;  // self-assignment keeps the value
    CHECK(c.FieldForTest(0) == "item-7");

    // A second read only replaces the fields whose tags are present.
    slim::XmlNode only_link;
    only_link.AddChild(slim::XmlNode("link", "http://x"));
    c.read(&only_link);
    CHECK(c.FieldForTest(0) == "item-7");
    CHECK(c.FieldForTest(3) == "http://x");

    // write is a no-op in the binary.
    c.write(&node);
    CHECK(c.FieldForTest(1) == "Sword");

    if (failures == 0) std::printf("all checks passed\n");
    return failures == 0 ? 0 : 1;
}
