#pragma once
#include <cstdint>

namespace dh2::animation {
// Borrowed two-slot metadata only; animator/timeline/applicator ownership is
// external. Field names map to original Blender +70 through +84 and +34.
struct BlenderState {
 std::uint32_t current, previous;
 std::int32_t duration, remaining;
 float reciprocal;
 std::uint32_t last_time;
 float weights[2];
};
static_assert(sizeof(BlenderState)==32);
}
extern "C" {
// All return0 on success,1 for malformed input without state mutation.
int dh2_blender_begin(dh2::animation::BlenderState*,std::int32_t next_duration);
// Weight phase only: does not advance timelines, normalize or write last_time.
int dh2_blender_update_weights(dh2::animation::BlenderState*,std::uint32_t timestamp);
// Execute after nonzero-weight slot sampling, before target contribution.
int dh2_blender_normalize(dh2::animation::BlenderState*);
}
