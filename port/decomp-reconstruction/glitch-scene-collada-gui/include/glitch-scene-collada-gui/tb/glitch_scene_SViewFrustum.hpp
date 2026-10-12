// glitch::scene::SViewFrustum, declared for Track B batch e1-glitch-scene-collada-gui-glitch_scene_SViewFrustum-0.
// No other header in this category declares the class under this name (recon/slice_00.hpp has a stand-in
// named ViewFrustum in glitch::collada::recon_00 with the planes and box only). Offsets are the original
// 32-bit layout, kept as comments and checked with static_assert. Only the members the reconstructed
// functions touch are named; the rest of the object (its size is not confirmed) is not declared.
#pragma once

#include <cstddef>
#include <cstdint>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"

namespace glitch::video {
// Same type as glitch-video/deps/glitch_video_types.hpp (int32_t alias); repeated so this header stands alone.
using E_TRANSFORMATION_STATE = std::int32_t;
}  // namespace glitch::video

namespace glitch::core::detail {
// 0x5822c8 CMatrix4Base<float>::setbyproduct(a, b): self = a * b with identity short-cuts. Defined in
// glitch-core-io/core/matrix4_base.hpp (the same declaration is repeated here for this category).
void setbyproduct(CMatrix4<float>& self, const CMatrix4<float>& a, const CMatrix4<float>& b);
// 0x40ea54 CMatrix4Base<float>::setbyproduct_nocheck(a, b): self = a * b as a full product.
void setbyproduct_nocheck(CMatrix4<float>& self, const CMatrix4<float>& a, const CMatrix4<float>& b);
}  // namespace glitch::core::detail

namespace glitch::scene {

class SViewFrustum {
public:
    // 0x5826c0 _ZN6glitch5scene12SViewFrustum7setFromERKNS_4core8CMatrix4IfEE
    // Builds the six planes from the columns of a column-major matrix, normalises them, then
    // recalculates the bounding box. Returns the result of recalculateBoundingBox().
    bool setFrom(const glitch::core::CMatrix4<float>& matrix);

    // 0x5823d4 _ZN6glitch5scene12SViewFrustum22recalculateBoundingBoxEv
    // Seeds the box from header, then grows it with the four far-plane corners. Returns whether
    // the last corner lowered the box minimum in z.
    bool recalculateBoundingBox();

    // 0x582348 _ZN6glitch5scene12SViewFrustum17setTransformStateENS_5video22E_TRANSFORMATION_STATEE
    // state 0: matrix150 = matrix10c * matrix84; matrix194 = matrix84 (copy); matrix194 inverted.
    // state 1: matrix1d8 = matrix150 * matrix0c8. Any other state does nothing.
    void setTransformState(glitch::video::E_TRANSFORMATION_STATE state);

    // +0x00..+0x0b: three words copied into box min and max by recalculateBoundingBox. The original
    // copies raw 32-bit words; the type is not confirmed, float is used here.
    float header[3];
    // +0x0c: six planes, four floats each (normal x, y, z and D). Plane 0 is row3 - row2 and plane 5
    // is row3 - row1 (see setFrom).
    glitch::core::plane3d<float> planes[6];
    // +0x6c: frustum bounding box; MinEdge at +0x6c, MaxEdge at +0x78.
    glitch::core::aabbox3d<float> box;
    // Matrices are CMatrix4<float>: 16 values at +0x00 and the identity flag at +0x40, a 0x44 stride.
    // +0x84: source of setTransformState (the 0x41-byte copy covers its 16 values and flag).
    glitch::core::CMatrix4<float> matrix84;
    // +0xc8: operand of setTransformState state 1. Written by no function in this batch.
    glitch::core::CMatrix4<float> matrix0c8;
    // +0x10c: operand of setTransformState state 0. Written by no function in this batch.
    glitch::core::CMatrix4<float> matrix10c;
    // +0x150: state 0 product matrix10c * matrix84; operand of state 1.
    glitch::core::CMatrix4<float> matrix150;
    // +0x194: state 0 copy of matrix84, then its inverse.
    glitch::core::CMatrix4<float> matrix194;
    // +0x1d8: state 1 product matrix150 * matrix0c8.
    glitch::core::CMatrix4<float> matrix1d8;
};

static_assert(offsetof(SViewFrustum, header) == 0x00);
static_assert(offsetof(SViewFrustum, planes) == 0x0c);
static_assert(offsetof(SViewFrustum, box) == 0x6c);
static_assert(offsetof(SViewFrustum, matrix84) == 0x84);
static_assert(offsetof(SViewFrustum, matrix0c8) == 0xc8);
static_assert(offsetof(SViewFrustum, matrix10c) == 0x10c);
static_assert(offsetof(SViewFrustum, matrix150) == 0x150);
static_assert(offsetof(SViewFrustum, matrix194) == 0x194);
static_assert(offsetof(SViewFrustum, matrix1d8) == 0x1d8);

}  // namespace glitch::scene
