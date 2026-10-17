#pragma once

#include <cstddef>
#include <cstdint>

namespace dh2::engine_camera::frustum_bounds {

using Word = std::uint32_t;
using Address = std::uintptr_t;

struct Frustum {
    Word position[3];
    Word planes[6][4];
    Word box_min[3];
    Word box_max[3];
};

// The recovered bounds body calls this helper four times and ignores its
// boolean return. The provider must preserve plane inputs and write the same
// output words as the source three-plane intersection routine.
using IntersectThreePlanes = Word (*)(void*, const Word*, const Word*,
                                      const Word*, Word output[3]);
using CompareBinary32 = Word (*)(void*, Word left, Word right);

struct Services {
    void* context;
    std::size_t context_extent;
    IntersectThreePlanes intersect_three_planes;
    CompareBinary32 greater;
    CompareBinary32 less;
};

enum class Status : std::int32_t {
    complete,
    invalid_argument,
    reentrant_call
};

// Reconstructs all 748 bytes of SViewFrustum::recalculateBoundingBox.
// The four calls into plane3d::getIntersectionWithPlanes remain an explicit
// source-service boundary; no Irrlicht/upstream intersection behavior is used.
// The Services record is snapshotted before any writes. A callback may not
// reenter this routine for the same Frustum; nested calls for a different
// Frustum remain permitted.
Status recalculate(Frustum*, const Services*);

} // namespace dh2::engine_camera::frustum_bounds
