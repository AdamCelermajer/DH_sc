#pragma once
#include "../level-world/character_combat_sound_v1.hpp"
#include <cstring>
namespace dh2::audio {
// Original callsites3c953c/3ca8e0/3d4598/3d23b8/3e4e74/3a0c20:
// copied source XYZ, source bool=false, integer1, and floats -1/-1.
// No visual-to-audio scaling here. Vox/source-group authority owns that.
// Retain negative IDs: original producers query position and call Play3D
// before its own negative-index early return; do not skip their prefix.
inline character::CombatSoundPlayV1 audio_world_request_v38(
 std::uintptr_t actual_vox,std::uintptr_t actual_subject,std::int32_t sound,
 const std::array<float,3>& actual_source_position) noexcept {
 character::CombatSoundPlayV1 out{};
 out.manager=actual_vox;out.target=actual_subject;out.sound_id=sound;
 std::memcpy(out.position.data(),actual_source_position.data(),sizeof(float)*3);
 out.source_bool=false;out.source_integer=1;
 out.source_float0=out.source_float1=-1.f;
 return out;
}
}
