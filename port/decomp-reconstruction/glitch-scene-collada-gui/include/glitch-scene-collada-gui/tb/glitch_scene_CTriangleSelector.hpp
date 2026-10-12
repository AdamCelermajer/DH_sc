// glitch::scene::CTriangleSelector, Track B batch e1-glitch-scene-collada-gui-glitch_scene_CTriangleSelector-0.
// No header in this category declared the class with a layout (the slice headers only forward-declare it
// or hold free-function stand-ins), so the class lives here.
//
// Offsets are the original 32-bit offsets, kept as comments. The code uses typed members only.
// The vtable pointer at +0x00 is not declared: the class has no virtual methods yet. The vtable
// slots known from the listing (tools/decomp_vtable.py) are +0x0c getTriangleCount and +0x10 getTriangles.
#pragma once

#include <cstdint>
#include <vector>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/deps/intrusive_ptr.hpp"

namespace glitch::scene {

class ISceneNode;
class IMesh;

class CTriangleSelector {
public:
    // 0x596598 CTriangleSelector(intrusive_ptr<IMesh const> const&, ISceneNode const*, bool). PARTIAL, see the cpp.
    CTriangleSelector(const boost::intrusive_ptr<const IMesh>& mesh, const ISceneNode* node, bool bakeNodeTransform);

    // 0x590f20 getTriangleCount() const: the triangle count (vector byte size / 36).
    int getTriangleCount() const;

    // 0x591464 getTriangles(triangle3d*, int, int&, CMatrix4 const*) const. PARTIAL, see the cpp.
    void getTriangles(core::triangle3d<float>* out, int maxCount, int& outCount,
                      const core::CMatrix4<float>* source) const;

    // 0x586a34 Setup(CMatrix4 const*) const. PARTIAL, see the cpp.
    void Setup(const core::CMatrix4<float>* source) const;

    // 0x587604 Setup(aabbox3d const&) const. PARTIAL, see the cpp.
    void Setup(const core::aabbox3d<float>& box) const;

    // 0x586acc AddResult(triangle3d const&) const: appends to the result buffer; true when it is full.
    bool AddResult(const core::triangle3d<float>& triangle) const;

    // 0x586ec8 TestWithBox(std::vector<triangle3d>&) const: keeps the candidates that overlap the box.
    // Hex-Rays declares a float return; it is the register leftover and both callers discard it.
    void TestWithBox(std::vector<core::triangle3d<float>>& candidates) const;

    // +0x04 IReferenceCounted count, starts at 1.
    std::uint32_t referenceCount = 1;
    // +0x08 ISceneNode const* (the node whose absolute transform the selector uses).
    const ISceneNode* node = nullptr;
    // +0x0c..+0x17 vector of triangles (begin +0x0c, end +0x10, capacity +0x14 in the original).
    // The std::vector is used in place of the glitch SAllocator vector; element size is 36 bytes.
    std::vector<core::triangle3d<float>> triangles;
    // +0x18 bool: the node transform is baked into the triangles at construction (ctor argument).
    // Name inferred from behaviour (ctor bakes the triangles, Setup and getTriangles then skip the node matrix).
    bool bakeNodeTransform = false;
    // +0x1c, +0x20, +0x24: zeroed by the ctor, not read by this batch.
    std::uint32_t unk_0x1c = 0;
    std::uint32_t unk_0x20 = 0;
    std::uint32_t unk_0x24 = 0;
    // +0x28..+0x33: three floats, 1.0 from the ctor.
    float unk_0x28[3] = {1.0f, 1.0f, 1.0f};
    std::uint8_t pad_0x34[4] = {};
    // +0x38, +0x3c, +0x40: zeroed by the ctor, not read by this batch.
    std::uint32_t unk_0x38 = 0;
    std::uint32_t unk_0x3c = 0;
    std::uint32_t unk_0x40 = 0;
    // +0x44..+0x5b: bounding box of the selector (min at +0x44, max at +0x50).
    // Default (-1,-1,-1)..(1,1,1) from the ctor. Written by Setup(aabbox3d) in a const method, hence mutable.
    mutable core::aabbox3d<float> boundingBox;
    // +0x5c..+0x9b: transform matrix (16 floats); its identity flag is at +0x9c (CMatrix4::flag).
    // Written by Setup(CMatrix4) in a const method, hence mutable.
    mutable core::CMatrix4<float> transform;
    // +0xa0 triangle result buffer, +0xa4 capacity, +0xa8 count. AddResult appends in a const method.
    // The ctor does not write these words; they are zeroed here.
    mutable core::triangle3d<float>* resultTriangles = nullptr;
    std::uint32_t resultCapacity = 0;
    mutable std::uint32_t resultCount = 0;
};

}  // namespace glitch::scene
