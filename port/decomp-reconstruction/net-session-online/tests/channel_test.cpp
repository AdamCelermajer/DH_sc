#include "net-session-online/channel.hpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    slim::XmlNode root("channel", "");
    root.AddChild(slim::XmlNode("title", "News"));
    root.AddChild(slim::XmlNode("link", "http://example/news"));
    root.AddChild(slim::XmlNode("description", "Daily"));
    slim::XmlNode first("item", "");
    first.AddChild(slim::XmlNode("id", "a1"));
    first.AddChild(slim::XmlNode("title", "First"));
    root.AddChild(first);
    slim::XmlNode second("item", "");
    second.AddChild(slim::XmlNode("id", "b2"));
    root.AddChild(second);

    channel c;
    c.read(&root);
    CHECK(c.TitleForTest() == "News");
    CHECK(c.LinkForTest() == "http://example/news");
    CHECK(c.ItemCountForTest() == 2);
    CHECK(c.ItemForTest(0).FieldForTest(0) == "a1");     // id
    CHECK(c.ItemForTest(0).FieldForTest(1) == "First");  // title
    CHECK(c.ItemForTest(1).FieldForTest(0) == "b2");

    channel copy(c);
    CHECK(copy.ItemCountForTest() == 2);
    channel assigned;
    assigned = c;
    CHECK(assigned.TitleForTest() == "News");
    assigned = assigned;  // self-assignment keeps the value
    CHECK(assigned.TitleForTest() == "News");

    if (failures == 0) std::printf("channel_test: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
