// glitch::core::CKdTree<std::pair<unsigned int, aabbox3d<float>>> member bodies, written as free functions
// over the tree. kd_tree.hpp holds the layout only and declares no members, so the tree is passed explicitly.
// Offsets are the ARM layout recorded in kd_tree.hpp; they are comments only.
#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>

#include "glitch-core-io/core/kd_tree.hpp"

namespace glitch::core::detail {

using KdTree = CKdTree<KdElement>;
using KdNode = KdTree::SKdNode;

// Box edges by axis (0 = X, 1 = Y, 2 = Z), the same order as kdCentreComponent.
inline float& kdMinEdge(aabbox3d<float>& box, int axis) {
    return axis == 0 ? box.MinEdge.X : (axis == 1 ? box.MinEdge.Y : box.MinEdge.Z);
}

inline float& kdMaxEdge(aabbox3d<float>& box, int axis) {
    return axis == 0 ? box.MaxEdge.X : (axis == 1 ? box.MaxEdge.Y : box.MaxEdge.Z);
}

// Child node from the tree's pool (boost::object_pool::construct at this+0x1c, 0x5a2dd4). The creator
// writes the split plane (+0x0c) as the box centre on the child's axis, and the axis byte (+0x18).
inline KdNode* kdNewChildNode(KdTree& tree, const aabbox3d<float>& box, std::uint8_t childAxis) {
    tree.nodePool.push_back(std::make_unique<KdNode>());
    KdNode* child = tree.nodePool.back().get();
    child->plane = kdCentreComponent(box, childAxis);
    child->axis = childAxis;
    return child;
}

// Element equality used by the leaf erase. The original SEqPredicate::operator() is not recovered;
// this compares the id and the six box floats. See the ledger note for the row.
inline bool kdSameElement(const KdElement& a, const KdElement& b) {
    return a.first == b.first && a.second.MinEdge.X == b.second.MinEdge.X &&
           a.second.MinEdge.Y == b.second.MinEdge.Y && a.second.MinEdge.Z == b.second.MinEdge.Z &&
           a.second.MaxEdge.X == b.second.MaxEdge.X && a.second.MaxEdge.Y == b.second.MaxEdge.Y &&
           a.second.MaxEdge.Z == b.second.MaxEdge.Z;
}

// 0x5a3120 _ZN6glitch4core7CKdTreeISt4pairIjNS0_8aabbox3dIfEEEE15addElemInternalERKS5_PNS6_7SKdNodeEjRKS4_
// A nonzero depth descends: the side is chosen by lessThanPlane on the node's plane and axis, the child is
// created when missing (plane = box centre on axis + 1 mod 3), and the box passed down is split at the
// node's plane (max edge on the axis for the lower child, min edge for the upper). The recursion uses
// depth - 1. A zero depth appends the element to the node's elements.
inline void addElemInternal(KdTree& tree, const KdElement& elem, KdNode* node, unsigned int depth,
                            const aabbox3d<float>& box) {
    if (depth == 0) {
        node->elements.push_back(elem);
        return;
    }
    const int axis = node->axis;                                    // +0x18
    const auto childAxis = static_cast<std::uint8_t>((axis + 1) % 3);
    aabbox3d<float> childBox = box;
    if (glitch::lessThanPlane(elem, node->plane, static_cast<char>(axis))) {  // +0x0c, +0x18
        if (node->lower == nullptr) {                                // +0x10
            node->lower = kdNewChildNode(tree, box, childAxis);
        }
        kdMaxEdge(childBox, axis) = node->plane;
        addElemInternal(tree, elem, node->lower, depth - 1, childBox);
    } else {
        if (node->upper == nullptr) {                                // +0x14
            node->upper = kdNewChildNode(tree, box, childAxis);
        }
        kdMinEdge(childBox, axis) = node->plane;
        addElemInternal(tree, elem, node->upper, depth - 1, childBox);
    }
}

// 0x5a44b4 _ZN6glitch4core7CKdTreeISt4pairIjNS0_8aabbox3dIfEEEE18removeElemInternalERKS5_PNS6_7SKdNodeE
// Descends by the same lessThanPlane rule until the chosen child is missing, then erases the matching
// elements of that node and subtracts the number removed from the element count (+0x50).
// The listing returns 1227133513 * (remaining size in bytes / 4), i.e. minus the remaining element count;
// no caller is in this category, so the port returns nothing.
inline void removeElemInternal(KdTree& tree, const KdElement& elem, KdNode* node) {
    KdNode* current = node;
    for (;;) {
        KdNode* child = glitch::lessThanPlane(elem, current->plane, static_cast<char>(current->axis))
                            ? current->lower
                            : current->upper;
        if (child == nullptr) break;
        current = child;
    }
    auto& elements = current->elements;
    const std::size_t before = elements.size();
    elements.erase(std::remove_if(elements.begin(), elements.end(),
                                  [&elem](const KdElement& e) { return kdSameElement(e, elem); }),
                   elements.end());
    tree.elementCount -= static_cast<int>(before - elements.size());  // +0x50
}

// 0x5a24f8 _ZNK6glitch4core7CKdTreeISt4pairIjNS0_8aabbox3dIfEEEE24findFarthestElemInternalERPKS5_RfPKNS6_7SKdNodeE
// Visits the subtree: a node with children recurses into the lower then the upper child; a node with only an
// upper child recurses into it and keeps no elements of its own; a childless node is scanned. Each element's
// distance to the query box (tree +0x38) is compared with the best so far; a larger one replaces it.
inline void findFarthestElemInternal(const KdTree& tree, const KdElement*& farthest, float& bestDistance,
                                     const KdNode* node) {
    if (node == nullptr) return;
    if (node->lower != nullptr) {
        findFarthestElemInternal(tree, farthest, bestDistance, node->lower);
        if (node->upper != nullptr) findFarthestElemInternal(tree, farthest, bestDistance, node->upper);
        return;
    }
    if (node->upper != nullptr) {
        findFarthestElemInternal(tree, farthest, bestDistance, node->upper);
        return;
    }
    for (const KdElement& e : node->elements) {
        const float d = glitch::distanceKdTree(e, tree.queryBox);
        if (d > bestDistance) {
            bestDistance = d;
            farthest = &e;
        }
    }
}

}  // namespace glitch::core::detail
