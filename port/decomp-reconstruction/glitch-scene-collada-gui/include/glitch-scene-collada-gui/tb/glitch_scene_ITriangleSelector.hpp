// glitch::scene::ITriangleSelector, the interface the collision manager calls. Added by Track B batch
// e1-glitch-scene-collada-gui-glitch_scene_CSceneCollisionManager-0, because no header in this category
// defined the interface (scene_node.hpp and others only forward-declare it).
//
// Virtual slots follow the vtable listing (tools/decomp_vtable.py ITriangleSelector, CTriangleSelector):
//   +0x00 / +0x04  destructors (D1 / D0)
//   +0x08          glitch::IReferenceCounted::onDelete (the base is not declared in this category)
//   +0x0c          getTriangleCount() const, pure
//   +0x10          getTriangles(triangle3d*, int, int&, CMatrix4 const*) const, pure
//   +0x14          getTriangles(triangle3d*, int, int&, aabbox3d const&, CMatrix4 const*) const, pure
//   +0x18          getTriangles(triangle3d*, int, int&, line3d const&, CMatrix4 const*) const, pure
// Only the slots from +0x0c on are declared here, in order, with their names from the listing.
#pragma once

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"

namespace glitch::scene {

class ITriangleSelector {
public:
    virtual ~ITriangleSelector() = default;

    // +0x0c: the number of triangles the selector holds.
    virtual int getTriangleCount() const = 0;

    // +0x10: fills up to arraySize triangles and stores the count written in outTriangleCount.
    virtual void getTriangles(core::triangle3d<float>* triangles, int arraySize, int& outTriangleCount,
                              const core::CMatrix4<float>* transform) const = 0;

    // +0x14: as above, keeping only the triangles that overlap box.
    virtual void getTriangles(core::triangle3d<float>* triangles, int arraySize, int& outTriangleCount,
                              const core::aabbox3d<float>& box, const core::CMatrix4<float>* transform) const = 0;

    // +0x18: as above, keeping only the triangles that the line segment passes near.
    virtual void getTriangles(core::triangle3d<float>* triangles, int arraySize, int& outTriangleCount,
                              const core::line3d<float>& line, const core::CMatrix4<float>* transform) const = 0;
};

}  // namespace glitch::scene
