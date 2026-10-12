// rss: the RSS feed reader. The object is the list of channels read from the
// "channel" children of an XML document. Partial: channel is a stand-in, since
// channel::read belongs to another category.
#pragma once

#include <vector>

namespace slim {
class XmlNode;
}

class channel {
public:
    channel() = default;
    // Stand-in: channel::read is not reconstructed in this category.
    void read(const slim::XmlNode* node);
};

class rss : public std::vector<channel> {
public:
    void write(slim::XmlNode* node) const;       // 0x842464: empty in the original
    void read(const slim::XmlNode* node);        // 0x844050
};
