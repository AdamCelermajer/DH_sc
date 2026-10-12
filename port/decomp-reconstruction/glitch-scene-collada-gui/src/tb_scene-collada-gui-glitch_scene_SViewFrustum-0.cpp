// Track B batch e1-glitch-scene-collada-gui-glitch_scene_SViewFrustum-0: glitch::scene::SViewFrustum.
// Layout and declarations are in glitch-scene-collada-gui/tb/glitch_scene_SViewFrustum.hpp.

#include <cmath>

#include "glitch-scene-collada-gui/tb/glitch_scene_SViewFrustum.hpp"

namespace glitch::scene {

namespace {

using glitch::core::aabbox3d;
using glitch::core::plane3d;
using glitch::core::vector3d;

// Grows the box to contain p. Each axis compares strictly against the current edge, max first,
// then min, as the original does (grow for MaxEdge, shrink for MinEdge).
void includeCorner(aabbox3d<float>& box, const vector3d<float>& p) {
    if (p.X > box.MaxEdge.X) box.MaxEdge.X = p.X;
    if (p.Y > box.MaxEdge.Y) box.MaxEdge.Y = p.Y;
    if (p.Z > box.MaxEdge.Z) box.MaxEdge.Z = p.Z;
    if (p.X < box.MinEdge.X) box.MinEdge.X = p.X;
    if (p.Y < box.MinEdge.Y) box.MinEdge.Y = p.Y;
    if (p.Z < box.MinEdge.Z) box.MinEdge.Z = p.Z;
}

// Stores one plane as the four values the original writes into the frustum (normal x, y, z, then D).
void storePlane(plane3d<float>& plane, float nx, float ny, float nz, float d) {
    plane.Normal.X = nx;
    plane.Normal.Y = ny;
    plane.Normal.Z = nz;
    plane.D = d;
}

}  // namespace

// 0x582348 _ZN6glitch5scene12SViewFrustum17setTransformStateENS_5video22E_TRANSFORMATION_STATEE
// The Hex-Rays return value is the r0 left by the last callee (or this); the symbol gives no return
// type, so the function is void here.
void SViewFrustum::setTransformState(glitch::video::E_TRANSFORMATION_STATE state) {
    if (state == 0) {
        // +0x150 = (+0x10c) * (+0x84)
        glitch::core::detail::setbyproduct_nocheck(matrix150, matrix10c, matrix84);
        // memcpy(+0x194, +0x84, 0x41): the 16 values and the identity flag at +0x40 of the matrix
        matrix194 = matrix84;
        matrix194.makeInverse();
    } else if (state == 1) {
        // +0x1d8 = (+0x150) * (+0xc8)
        glitch::core::detail::setbyproduct(matrix1d8, matrix150, matrix0c8);
    }
}

// 0x5823d4 _ZN6glitch5scene12SViewFrustum22recalculateBoundingBoxEv
// Each intersection vector is zeroed before its call and used even when the call returns false,
// as the Hex-Rays body does. The four corners are all where planes[0] (row3 - row2) meets two of
// the side planes: (5, 2), (5, 3), (4, 2), (4, 3).
bool SViewFrustum::recalculateBoundingBox() {
    // Seed the box with the header point (+0x6c..+0x80).
    box.MinEdge.X = header[0];
    box.MinEdge.Y = header[1];
    box.MinEdge.Z = header[2];
    box.MaxEdge.X = header[0];
    box.MaxEdge.Y = header[1];
    box.MaxEdge.Z = header[2];

    vector3d<float> corner{};
    planes[0].getIntersectionWithPlanes(planes[5], planes[2], corner);
    includeCorner(box, corner);

    corner = vector3d<float>{};
    planes[0].getIntersectionWithPlanes(planes[5], planes[3], corner);
    includeCorner(box, corner);

    corner = vector3d<float>{};
    planes[0].getIntersectionWithPlanes(planes[4], planes[2], corner);
    includeCorner(box, corner);

    // The last corner's result is the comparison that lowers MinEdge.Z in the original. The other
    // updates in this step do not touch MinEdge.Z, so the comparison is taken before the update.
    corner = vector3d<float>{};
    planes[0].getIntersectionWithPlanes(planes[4], planes[3], corner);
    const bool lowersMinZ = corner.Z < box.MinEdge.Z;
    includeCorner(box, corner);
    return lowersMinZ;
}

// 0x5826c0 _ZN6glitch5scene12SViewFrustum7setFromERKNS_4core8CMatrix4IfEE
// Named rows here are index patterns m[r + 4j] for j = 0..3: planes[0] = row3 - row2, planes[1] = row2,
// planes[2] = row3 + row0, planes[3] = row3 - row0, planes[4] = row3 + row1, planes[5] = row3 - row1.
// The planes are normalised below.
bool SViewFrustum::setFrom(const glitch::core::CMatrix4<float>& matrix) {
    const float(&m)[16] = matrix.M;

    storePlane(planes[0], m[3] - m[2], m[7] - m[6], m[11] - m[10], m[15] - m[14]);
    storePlane(planes[1], m[2], m[6], m[10], m[14]);
    storePlane(planes[2], m[3] + m[0], m[7] + m[4], m[11] + m[8], m[15] + m[12]);
    storePlane(planes[3], m[3] - m[0], m[7] - m[4], m[11] - m[8], m[15] - m[12]);
    storePlane(planes[4], m[3] + m[1], m[7] + m[5], m[11] + m[9], m[15] + m[13]);
    storePlane(planes[5], m[3] - m[1], m[7] - m[5], m[11] - m[9], m[15] - m[13]);

    for (plane3d<float>& plane : planes) {
        const float lengthSquared =
            ((plane.Normal.X * plane.Normal.X) + (plane.Normal.Y * plane.Normal.Y)) +
            (plane.Normal.Z * plane.Normal.Z);
        // The original computes 1.0f / sqrtf(lengthSquared) and then adds 0x80000000 to its bit
        // pattern (ADD R5, R0, #0x80000000), which flips the sign: the scale is the negated reciprocal.
        const float scale = -(1.0f / std::sqrt(lengthSquared));
        plane.Normal.X *= scale;
        plane.Normal.Y *= scale;
        plane.Normal.Z *= scale;
        plane.D *= scale;
    }

    return recalculateBoundingBox();
}

}  // namespace glitch::scene
