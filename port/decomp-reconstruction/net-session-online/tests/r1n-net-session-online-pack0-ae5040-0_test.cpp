// Test for the batch r1n-net-session-online-pack0-ae5040-0: item::read (0x842a88).
// The twelve tags map to the twelve string fields in the order of the ARM listing (the fields
// are 0x18 bytes apart: id at +0x00 ... button_left at +0xf0, button_right at +0x108, the child
// text is read at +0x2c of each XML node).
#include <cstdio>
#include <string>

#include "net-session-online/deps/slim_xml_node.hpp"
#include "net-session-online/item.hpp"

static int failures = 0;
#define CHECK(c)                                                          \
    do {                                                                  \
        if (!(c)) {                                                       \
            std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c);       \
            ++failures;                                                   \
        }                                                                 \
    } while (0)

int main() {
    static const char* const kTags[12] = {
        "id",     "title",       "description", "link",   "attachment_id", "fileType",
        "fileSize", "width",     "height",      "times",  "button_left",   "button_right",
    };

    slim::XmlNode node("item", "");
    for (int i = 0; i < 12; ++i) {
        node.AddChild(slim::XmlNode(kTags[i], "v" + std::to_string(i)));
    }
    item full;
    full.read(&node);
    for (int i = 0; i < 12; ++i) {
        CHECK(full.FieldForTest(i) == "v" + std::to_string(i));
    }

    // A node with no tags leaves every field empty (the original only copies found children).
    slim::XmlNode bare("item", "");
    item empty;
    empty.read(&bare);
    CHECK(empty.FieldForTest(0).empty());
    CHECK(empty.FieldForTest(11).empty());

    std::printf(failures == 0 ? "OK\n" : "FAILED\n");
    return failures == 0 ? 0 : 1;
}
