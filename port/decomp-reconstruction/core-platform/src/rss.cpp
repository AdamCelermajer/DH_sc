#include "core-platform/rss.hpp"

#include "core-platform/slim_xml.hpp"

#include <cassert>

// 0x842464 rss::write(slim::XmlNode*) const: the original returns at once.
void rss::write(slim::XmlNode*) const {}

// Stand-in for channel::read (another category). Nothing is read into the channel.
void channel::read(const slim::XmlNode*) {}

// 0x844050 rss::read(slim::XmlNode const*)
// Appends one channel per "channel" child of the node, in document order.
// Partial: channel::read is the stand-in above.
void rss::read(const slim::XmlNode* node) {
    assert(node != nullptr);
    slim::XmlNode::ChildIterator it;
    for (const slim::XmlNode* child = node->findFirstChild("channel", it); child != nullptr;
         child = node->findNextChild("channel", it)) {
        emplace_back();
        back().read(child);
    }
}
