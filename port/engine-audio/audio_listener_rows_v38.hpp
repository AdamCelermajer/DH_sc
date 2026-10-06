#pragma once
#include "vox_source_fields_v38.hpp"
#include <vector>
namespace dh2::audio {
// Actual Structs::Listener4ed978 serialized fields, runtime stride28:
// vtable, Anchor, MaxDistance, Orientation, RefDistance, RolloffFactor, UpVector.
struct AudioListenerRowV38 {std::int32_t anchor{},maximum_distance{},orientation{},reference_distance{};float rolloff{};std::int32_t up_vector{};};
bool audio_listener_rows_v38(const std::uint8_t* sounds_pyarray,std::size_t,
 std::vector<AudioListenerRowV38>&,std::string&);
// This mirrors only SetListenerPos369f50 reached fields. Actual vectors come
// from Level::UpdateListener3f0ae0 and its current-level row selection.
bool audio_listener_update_v38(const AudioListenerRowV38&,
 const float actual_position[3],const float actual_front[3],const float actual_up[3],
 VoxListenerAuthorityV38&,std::string&);
}
