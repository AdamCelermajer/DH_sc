#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
namespace dh2::character {
using ActorInitializationDigest=std::array<std::uint8_t,32>;
enum ActorInitializationField:std::size_t {actor_ai_state,actor_ai_state_visible,actor_auto_spawn,actor_spawn_delay,actor_spawn_view_radius,actor_char_group,actor_char_group_role,actor_initialization_field_count};
struct AuthoredText {bool present=false;std::string text;};
struct ActorInitializationValues {
 std::string ai_state;
 bool ai_state_visible=true,auto_spawn=true;
 std::array<std::int32_t,2> spawn_delay{};
 float spawn_view_radius=0;
 std::string char_group,char_group_role;
 std::int32_t preset_state=3;
};
struct ActorInitializationKey {std::uint32_t room=0;std::string name,character;};
struct ActorInitializationSource {
 std::uint32_t room=0,bytes=0;std::string cache_entry;
 ActorInitializationDigest sha256{};
};
struct ActorInitializationRecord {
 std::uint32_t room=0,source_index=0;std::string name,character;
 AuthoredText requested_template;
 std::array<AuthoredText,actor_initialization_field_count> authored;
 ActorInitializationValues resolved;
};
struct ActorInitialization {
 ActorInitializationDigest cache_sha256{},descriptor_sha256{},original_sha256{},default_capture_sha256{};
 std::vector<ActorInitializationSource> sources;
 std::vector<ActorInitializationRecord> records;
};
// Bounded CAI1 v1 current Crypt01 eleven-monster sidecar, not a full original
// XML/property factory. The caller supplies the digest of the ACTUAL DACT bytes
// and every monster key decoded from those bytes. Source members/raw authoring
// are checked against this captured inventory. Numeric authoring outside the
// actual absent fields is explicitly unsupported. No descriptor/hash service
// or default is guessed. Failed load preserves out in full.
// This owned snapshot/input keys may die independently after loading. Borrowed
// record/source pointers remain valid until snapshot mutation/move/destruction;
// reload only after all consumers release their borrowed views.
bool load_actor_initialization(const std::uint8_t*,std::size_t,
 const ActorInitializationDigest& actual_descriptor_sha256,
 const std::vector<ActorInitializationKey>& actual_monster_keys,
 ActorInitialization& out,std::string& error);
const ActorInitializationRecord* actor_initialization(const ActorInitialization&,
 std::uint32_t room,const std::string& name)noexcept;
}
