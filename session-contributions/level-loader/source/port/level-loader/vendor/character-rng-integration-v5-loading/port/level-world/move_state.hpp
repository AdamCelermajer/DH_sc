#pragma once
#include <cstdint>
namespace dh2::move {
struct Policy {std::uint32_t position_from_visual,position_from_physics,rotation_from_visual,rotation_from_physics,visual_with_rotation,update_path,validate_floor;};
struct Speed {float walk_multiplier,rotation_multiplier,clip_time_multiplier;};
inline constexpr unsigned walk_property=46,rotation_property=47;
static_assert(sizeof(Policy)==28&&sizeof(Speed)==12);
}
extern "C" {
// Return 0 completed, 1 malformed/overlapping caller storage. The resolved
// sheet contains 224 payload words without the original C++ vtable/header.
int dh2_move_policy(dh2::move::Policy*,const std::uint32_t* character_flags);
int dh2_move_speed(dh2::move::Speed*,const std::int32_t* resolved224,const float* authored_step_speed);
int dh2_move_rotation_speed(float*,const std::uint32_t* character_flags,const std::int32_t* resolved224);
// Only the actual OnFocus prefix: flags=0x23c1, movement_type=0. UpdateType,
// animation/FSM events and subsequent unpin must be performed by the caller.
int dh2_move_focus_begin(std::uint32_t* character_flags,std::uint32_t* movement_type);
}
