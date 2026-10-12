// Track B batch e1-glitch-scene-collada-gui-glitch_scene_COctTreeTriangleSelector-0.
#include "glitch-scene-collada-gui/tb/glitch_scene_COctTreeTriangleSelector.hpp"

namespace glitch::scene {

namespace {

// The original's epsilon tests are "v + 0.000001" and "v - 0.000001" computed in double and stored as float.
float widenUp(float v) { return static_cast<float>(v + 0.000001); }
float widenDown(float v) { return static_cast<float>(v - 0.000001); }

// Child-box edge on one axis: the smaller (or larger) of the box centre and the corner coordinate.
float lowerOf(float center, float corner) { return corner < center ? corner : center; }
float upperOf(float center, float corner) { return center < corner ? corner : center; }

// The box a new node is created with: (-1,-1,-1) to (1,1,1), as the original's allocation code sets it.
void setUnitBox(core::aabbox3d<float>& b) {
    b.MinEdge.X = -1.0f;
    b.MinEdge.Y = -1.0f;
    b.MinEdge.Z = -1.0f;
    b.MaxEdge.X = 1.0f;
    b.MaxEdge.Y = 1.0f;
    b.MaxEdge.Z = 1.0f;
}

}  // namespace

// 0x5870fc _ZNK6glitch5scene24COctTreeTriangleSelector26getTrianglesFromOctTreeBoxEPNS1_12SOctTreeNodeE
// The pseudocode returns a float, but that value is the residue of the last compare and TestWithBox,
// not data. It is declared void here.
void COctTreeTriangleSelector::getTrianglesFromOctTreeBox(SOctTreeNode* node) const {
    const core::aabbox3d<float>& query = queryBox;  // +0x44 (min), +0x50 (max)
    const bool overlaps =
        node->box.MinEdge.X <= query.MaxEdge.X &&   // node +0x2c <= query +0x50
        node->box.MinEdge.Y <= query.MaxEdge.Y &&   // node +0x30 <= query +0x54
        node->box.MinEdge.Z <= query.MaxEdge.Z &&   // node +0x34 <= query +0x58
        node->box.MaxEdge.X >= query.MinEdge.X &&   // node +0x38 >= query +0x44
        node->box.MaxEdge.Y >= query.MinEdge.Y &&   // node +0x3c >= query +0x48
        node->box.MaxEdge.Z >= query.MinEdge.Z;     // node +0x40 >= query +0x4c
    if (!overlaps) {
        return;
    }

    TestWithBox(node->triangles);

    if (outCount != outCapacity) {
        for (int k = 0; k < 8; ++k) {  // children at +0x0c + 4k
            SOctTreeNode* child = node->children[k];
            if (child) {
                getTrianglesFromOctTreeBox(child);
            }
        }
    }
}

// 0x587694 _ZNK6glitch5scene24COctTreeTriangleSelector12getTrianglesEPNS_4core10triangle3dIfEEiRiRKNS2_8aabbox3dIfEEPKNS2_8CMatrix4IfEE
void COctTreeTriangleSelector::getTriangles(core::triangle3d<float>* triangles, std::int32_t capacity,
                                            std::int32_t& count, const core::aabbox3d<float>& box,
                                            const core::CMatrix4<float>* transform) const {
    outCapacity = capacity;      // +0xa4
    outCount = 0;                // +0xa8
    outTriangles = triangles;    // +0xa0
    Setup(transform);            // CTriangleSelector::Setup(CMatrix4 const*), called before the box
    Setup(box);                  // CTriangleSelector::Setup(aabbox3d const&), stores the query box at +0x44
    if (root) {                  // +0xac
        getTrianglesFromOctTreeBox(root);
    }
    count = outCount;
}

// 0x587e60 _ZN6glitch5scene24COctTreeTriangleSelector16constructOctTreeEPNS1_12SOctTreeNodeE
// Known deviation: with an empty triangle list the original reads *begin for the box before testing
// the count. This function leaves the box as it is in that case. constructOctTree is only called on
// non-empty nodes (the root from the constructor, children from the loop below), so the path is not used.
void COctTreeTriangleSelector::constructOctTree(SOctTreeNode* node) {
    ++nodeCount;  // +0xb0

    std::vector<core::triangle3d<float>>& tris = node->triangles;
    core::aabbox3d<float>& box = node->box;  // +0x2c..+0x43

    // Node box: min and max over every corner of every triangle (strict compares per axis).
    if (!tris.empty()) {
        box.MinEdge = tris.front().A;
        box.MaxEdge = tris.front().A;
        for (const core::triangle3d<float>& tri : tris) {
            box.addInternalPoint(tri.A);
            box.addInternalPoint(tri.B);
            box.addInternalPoint(tri.C);
        }
    }

    const float centerX = (box.MinEdge.X + box.MaxEdge.X) * 0.5f;
    const float centerY = (box.MinEdge.Y + box.MaxEdge.Y) * 0.5f;
    const float centerZ = (box.MinEdge.Z + box.MaxEdge.Z) * 0.5f;

    // The eight corners of the node box. The original zeroes them before getEdges fills them.
    core::vector3d<float> corners[8] = {};
    box.getEdges(corners);

    // Split only when the box is not degenerate on any axis and the node has more triangles than the limit.
    // The limit is compared as an unsigned value, as in the original.
    const bool notDegenerate =
        box.MaxEdge.X > widenUp(box.MinEdge.X) || box.MaxEdge.X < widenDown(box.MinEdge.X) ||
        box.MaxEdge.Y > widenUp(box.MinEdge.Y) || box.MaxEdge.Y < widenDown(box.MinEdge.Y) ||
        box.MaxEdge.Z > widenUp(box.MinEdge.Z) || box.MaxEdge.Z < widenDown(box.MinEdge.Z);
    if (!(notDegenerate &&
          static_cast<std::uint32_t>(minimalPolysPerNode) < static_cast<std::uint32_t>(tris.size()))) {
        return;
    }

    // Triangles that do not fit wholly inside a child stay in this node. This is the original's temporary "p" vector.
    std::vector<core::triangle3d<float>> remaining;

    for (int k = 0; k < 8; ++k) {  // one child per corner, +0x0c + 4k
        const core::vector3d<float>& corner = corners[k];

        // Child box: the octant spanned by the box centre and this corner.
        core::aabbox3d<float> childBox;
        childBox.MinEdge.X = lowerOf(centerX, corner.X);
        childBox.MinEdge.Y = lowerOf(centerY, corner.Y);
        childBox.MinEdge.Z = lowerOf(centerZ, corner.Z);
        childBox.MaxEdge.X = upperOf(centerX, corner.X);
        childBox.MaxEdge.Y = upperOf(centerY, corner.Y);
        childBox.MaxEdge.Z = upperOf(centerZ, corner.Z);

        SOctTreeNode* child = new SOctTreeNode();
        setUnitBox(child->box);
        node->children[k] = child;

        // Triangles wholly inside the child box move to the child. The rest stay in the node.
        for (const core::triangle3d<float>& tri : tris) {
            if (tri.isTotalInsideBox(childBox)) {
                child->triangles.push_back(tri);
            } else {
                remaining.push_back(tri);
            }
        }
        tris.assign(remaining.begin(), remaining.end());
        remaining.clear();

        // An empty child is freed and its slot cleared. A non-empty child is split in turn.
        if (child->triangles.empty()) {
            delete child;
            node->children[k] = nullptr;
        } else {
            constructOctTree(child);
        }
    }
}

}  // namespace glitch::scene
