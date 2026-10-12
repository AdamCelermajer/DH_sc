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
    channel copy(c);
    CHECK(copy.read == nullptr);  // read is not a getter; placeholder removed below
    return failures;
}
