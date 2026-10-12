// Stand-in for slim::XmlNode, the XML tree of the slim library (owned by another
// category). Only the lookup and the text read by item::read are declared. The original
// reads a child's text at node+0x2c; this stand-in keeps the text as a std::string.
#pragma once

#include <cstddef>
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

    // slim::XmlNode::findFirstChild(const char*, int&). Stand-in: the cursor is the index of the
    // child returned, so findNextChild resumes after it. The original's cursor type is not
    // recovered, so this is inferred from the channel::read loop.
    const XmlNode* findFirstChild(const char* name, int& cursor) const {
        cursor = 0;
        return findNextChild(name, cursor);
    }

    // slim::XmlNode::findNextChild(const char*, int&): the next direct child with that name
    // after the cursor, or null. Advances the cursor.
    const XmlNode* findNextChild(const char* name, int& cursor) const {
        for (int i = cursor; i < static_cast<int>(children_.size()); ++i) {
            if (children_[static_cast<std::size_t>(i)].name_ == name) {
                cursor = i + 1;
                return &children_[static_cast<std::size_t>(i)];
            }
        }
        cursor = static_cast<int>(children_.size());
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
