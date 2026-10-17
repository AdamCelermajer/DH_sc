#pragma once

#include "frustum.hpp"
#include "frustum_bounds.hpp"

namespace dh2::engine_camera::frustum_runtime {

using Matrix = frustum::Matrix4Words;
using Frustum = frustum_bounds::Frustum;
enum class Status { complete, invalid_argument, dependency_failed };

// Source setFrom plane extraction/normalization followed by the original
// bounds and plane-intersection source bodies. Scalar imported operations use
// IEEE binary32/binary64 host math with no callbacks into game state. Compile
// without fast math or FP contraction; the calling thread uses round-to-nearest.
// The existing frustum position is preserved. Matrix and frustum must be live,
// aligned, disjoint objects. Rejection leaves the frustum unchanged.
// This supplies camera mathematics, not native camera ownership or transforms.
Status set_from(const Matrix*, Frustum*);

} // namespace dh2::engine_camera::frustum_runtime
