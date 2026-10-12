// glitch::scene::CSceneCollisionManager, Track B batch e1-glitch-scene-collada-gui-glitch_scene_CSceneCollisionManager-0.
// No header in this category declared the class with a layout (recon/slice_33.hpp only has the
// CollisionManagerStandIn used by the constructors), so the class lives here. Only the fields and
// methods this batch reads are declared; the constructors and the other collision methods are not done.
//
// Offsets are the original 32-bit offsets, kept as comments.
#pragma once

#include <cstdint>
#include <vector>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/tb/glitch_scene_ITriangleSelector.hpp"

namespace glitch::scene {

class CSceneCollisionManager {
public:
    // 0x6c62e8 getCollisionPoint(line3d const&, ITriangleSelector*, vector3d&, triangle3d&): finds the triangle
    // of the selector that the segment hits nearest to its start. Writes the hit point and the triangle and
    // returns true when a hit exists; otherwise returns false and leaves the outputs untouched.
    bool getCollisionPoint(const core::line3d<float>& ray, ITriangleSelector* selector,
                           core::vector3d<float>& outPoint, core::triangle3d<float>& outTriangle);

    // +0x00..+0x0f: vtable pointer, reference count, scene manager and video driver
    // (see CollisionManagerStandIn in recon/slice_33.hpp). Not read by this batch.
    std::uint8_t pad_0x00[0x10] = {};

    // +0x10: scratch buffer of the selector's triangles (begin at +0x10, end at +0x14, capacity at +0x18 in the
    // original). Grown by getCollisionPoint to the selector's triangle count, never shrunk. Element size is 36 bytes.
    std::vector<core::triangle3d<float>> triangles;
};

}  // namespace glitch::scene
