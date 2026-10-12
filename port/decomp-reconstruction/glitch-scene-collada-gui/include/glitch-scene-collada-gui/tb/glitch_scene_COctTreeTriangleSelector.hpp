// glitch::scene::COctTreeTriangleSelector and the parts of its CTriangleSelector base it uses.
// No earlier header declared this class (slice_05.hpp has only free-function stand-ins), so this
// header is the owner for the Track B batch e1-glitch-scene-collada-gui-glitch_scene_COctTreeTriangleSelector-0.
//
// Offsets in the comments are the original 32-bit ARM layout. The code uses typed members only and
// never does pointer arithmetic on those offsets.
#pragma once

#include <cstdint>
#include <vector>

#include "glitch-shared/glitch_core.hpp"

namespace glitch::scene {

// Stand-in for the CTriangleSelector base. Only the box that the octree selector reads and the
// three methods it calls are named. The rest of the base is an unmapped pad (vtable, owner pointer
// at +0x08, flag byte at +0x18, and so on). The base bodies are reconstructed by their own batches.
class CTriangleSelector {
public:
    std::uint8_t pad_0000[0x44];              // +0x00..+0x43 base state, not mapped
    mutable core::aabbox3d<float> queryBox;   // +0x44..+0x5b, written by Setup(aabbox3d), read by TestWithBox
    std::uint8_t pad_005c[0x44];              // +0x5c..+0x9f base state, not mapped

    // 0x586a34 _ZNK6glitch5scene17CTriangleSelector5SetupEPKNS_4core8CMatrix4IfEE
    void Setup(const core::CMatrix4<float>* transform) const;
    // 0x587604 _ZNK6glitch5scene17CTriangleSelector5SetupERKNS_4core8aabbox3dIfEE
    void Setup(const core::aabbox3d<float>& box) const;
    // 0x586ec8 _ZNK6glitch5scene17CTriangleSelector11TestWithBoxERSt6vectorINS_4core10triangle3dIfEENS3_10SAllocatorIS5_LNS_6memory13E_MEMORY_HINTE0EEEE
    // Tests the triangles of one node against queryBox and appends the hits to the output.
    void TestWithBox(std::vector<core::triangle3d<float>>& triangles) const;
};

class COctTreeTriangleSelector : public CTriangleSelector {
public:
    // One octree node. Size 0x44 in the original: triangle vector, 8 children, box.
    struct SOctTreeNode {
        std::vector<core::triangle3d<float>> triangles;  // +0x00 (begin, end, capacity words in the original)
        SOctTreeNode* children[8] = {};                  // +0x0c..+0x2b, child k at +0x0c + 4k
        core::aabbox3d<float> box{};                     // +0x2c..+0x43 (min +0x2c, max +0x38)

        // 0x5865d0 SOctTreeNode::~SOctTreeNode(). Deletes the children recursively. Defined by its own batch.
        ~SOctTreeNode();
    };

    // Output of the current getTriangles call. The original writes these words in getTriangles
    // (const, so they are mutable here).
    mutable core::triangle3d<float>* outTriangles = nullptr;  // +0xa0
    mutable std::int32_t outCapacity = 0;                     // +0xa4
    mutable std::int32_t outCount = 0;                        // +0xa8

    SOctTreeNode* root = nullptr;             // +0xac, root node of the tree
    std::int32_t nodeCount = 0;               // +0xb0, incremented by constructOctTree
    std::int32_t minimalPolysPerNode = 0;     // +0xb4, a node is split only while its triangle count is above this

    // 0x5870fc _ZNK6glitch5scene24COctTreeTriangleSelector26getTrianglesFromOctTreeBoxEPNS1_12SOctTreeNodeE
    void getTrianglesFromOctTreeBox(SOctTreeNode* node) const;

    // 0x587694 _ZNK6glitch5scene24COctTreeTriangleSelector12getTrianglesEPNS_4core10triangle3dIfEEiRiRKNS2_8aabbox3dIfEEPKNS2_8CMatrix4IfEE
    void getTriangles(core::triangle3d<float>* triangles, std::int32_t capacity, std::int32_t& count,
                      const core::aabbox3d<float>& box, const core::CMatrix4<float>* transform) const;

    // 0x587e60 _ZN6glitch5scene24COctTreeTriangleSelector16constructOctTreeEPNS1_12SOctTreeNodeE
    void constructOctTree(SOctTreeNode* node);
};

}  // namespace glitch::scene
