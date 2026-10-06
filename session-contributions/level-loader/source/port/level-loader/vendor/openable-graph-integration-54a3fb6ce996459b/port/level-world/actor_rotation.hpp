#pragma once
#include <cstdint>
namespace dh2::actor {
// Original GameObject EulerXYZ at+16c/+170/+174; heading angle at+178 and
// last incremental turn direction at+17c. Snap branches retain turn_positive.
struct RotationState {float rotation[3],heading_angle;std::uint32_t turn_positive,reserved;};
struct RotationPolicy {float speed;std::uint32_t dt_ms,visual_present,visual_with_rotation;};
static_assert(sizeof(RotationState)==24&&sizeof(RotationPolicy)==16);
}
extern "C" {
// 0 completed, 1 malformed finite/bool/reserved/overlapping caller storage.
// Updates original current Z angle, then emits whether caller must perform
// genuine VisualObject::SyncRotation. Physical rotation is not changed here.
int dh2_actor_update_rotation(dh2::actor::RotationState*,const dh2::actor::RotationPolicy*,std::uint32_t* sync_visual);
}
