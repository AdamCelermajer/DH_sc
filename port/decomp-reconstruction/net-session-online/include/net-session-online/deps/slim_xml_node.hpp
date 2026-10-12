// Stand-in for slim::XmlNode, the XML tree of the slim library (owned by another
// category). Only the lookup and the text read by item::read are declared. The original
// reads a child's text at node+0x2c; this stand-in keeps the text as a std::string.
#pragma once

#include <string>
#include <utility>
#include <vector>

namespace slim {

class XmlNode {
public:
    XmlNode() = default;
    XmlNode(std::string name, std::string text) : name_(std::move(name)), text_(std::move(text)) {}

    // slim::XmlNode::findChild(const char*): the first direct child with that name, or null.
    const XmlNode* findChild(const char* name) const {
        for (const XmlNode& child : children_) {
            if (child.name_ == name) {
                return &child;
            }
        }
        return nullptr;
    }

    // The text of the node (+0x2c in the original).
    const char* Text() const { return text_.c_str(); }

    // Not in the original: test seam for building a tree.
    void AddChild(XmlNode child) { children_.push_back(std::move(child)); }

private:
    std::string name_;
    std::string text_;
    std::vector<XmlNode> children_;
};

}  // namespace slim
