#pragma once
#include "audio_gameplay_runtime_v40.hpp"
#include "audio_listener_rows_v38.hpp"
#include "integration-v40/runtime/default-source-fields/default_source_fields_v40.hpp"
namespace dh2::audio {
struct AudioSourceCommandAuthorityV40 {
 std::uintptr_t manager{},actual_current_level{};
 std::uint64_t world_epoch{},listener_world_epoch{};
 bool soundpack_initialized{},manager_general_initialized{},listener_initialized{};
 VoxListenerAuthorityV38 listener;
 OriginalVoxGeneralV40 general;
 // Actual current source emitter/group/settings modifiers, after original
 // group gain/RNG setter prefix. No unity settings or private rand defaults.
 float actual_gain{},actual_pitch{};
 int actual_native_initial_state{-1};
};
// Resolve this immutable snapshot synchronously from the same real World/Vox
// owner. Flags mean completed source operations, not a desired-ready setting.
bool audio_source_command_v40(const AudioSourceCommandAuthorityV40&,
 const dh2::character::CombatSoundPlayV1&,const AudioSoundV34&,
 const AudioGroupV34&,AudioCommandV34&,std::string&);
}
