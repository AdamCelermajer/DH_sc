#pragma once
#include <cstdint>

namespace dh2::animation {

// P17 DOORS: compiled animation channels whose target names no scene node (material/FX-parameter
// channels such as type 86, particle emitter channels such as type 28 "...-emitter") cannot move a
// transform, so the transform compiler skips them instead of rejecting the clip.
// Returns true when the channel must be skipped. Channels this filter does not own (types 1, 5, 10,
// dynamic position/angle channels, and channels without a target) return false and keep their
// existing handling. A skipped channel is logged once per distinct (type, target) for the process.
bool skip_channel_without_node(std::int32_t type,const char* target,bool target_is_node,bool dynamic_channels);

}
