#pragma once
#include "object_save_restore_v3.hpp"
#include "character_buffs.hpp"
#include "character_target_bindings.hpp"
#include "character_state_owner.hpp"
#include "../game-data/combat_application.hpp"
namespace dh2::character {
using level::SavegameStreamV2;
// Legacy ARM32 raw-Structs tag from original CharProperties C2 stores. This
// is opaque FILE data, never a native address/vptr or dispatch authority.
inline constexpr std::uint32_t legacy_character_properties_tag_v3=0x96bbc0u;
struct CharacterLegacyPropertyTagsV3 {
 std::uint32_t saved{},resolved{};bool saved_produced{},resolved_produced{};
 bool construct_fresh(std::string&);
 bool known()const noexcept{return saved_produced&&resolved_produced&&saved==legacy_character_properties_tag_v3&&resolved==legacy_character_properties_tag_v3;}
};
// Own only omitted source metadata: no properties/life/FSM/base copy.
struct CharacterSaveMetadataV3 {
 CharacterLegacyPropertyTagsV3 tags;
 std::array<float,3> respawn_rotation1468{},respawn_position1474{};
 bool pose_produced{};
 bool construct_fresh(std::string&);
 bool adopt_observed(std::uint32_t saved_tag,std::uint32_t resolved_tag,
  const float* actual_rotation1468,const float* actual_position1474,std::string&);
};
struct CharacterSaveGroupBorrowV3 {std::int32_t* field24{};std::uint8_t* byte28{};std::uint8_t* byte29{};};
struct CharacterSaveRestoreBorrowV3 {
 world::ObjectSaveRestoreBorrowV3 base;
 data::PropertyState* properties{};CharacterLegacyPropertyTagsV3* tags{};
 data::CombatActorState* life{};BuffOwner* buffs{};
 StateOwnerMachine40* machine{};
 float* position160{};float* rotation16c{};
 const float* initial_position1450{};const float* initial_rotation145c{};
 float* respawn_rotation1468{};float* respawn_position1474{};
 const std::uintptr_t* group3fc{};
 const std::uintptr_t* physical2dc{};const std::uintptr_t* attached2e0{};
 std::uint32_t* controller_locked8{}; // SAME logical source byte8, no punning
 TargetState48* target{};const TargetServices16* target_services{};
 // Publish direct raw pose reads to existing semantic observations only.
 void* publication_context{};void(*publish_pose)(void*){};
};
struct CharacterSaveRestoreRequestV3 {
 std::uint32_t entry{},argument0{},argument1{};
 std::uintptr_t subject{},payload{};
};
struct CharacterSaveRestoreResponseV3 {
 std::uint32_t word{};const float* point{};CharacterSaveGroupBorrowV3 group;
};
struct CharacterSaveRestoreServicesV3 {
 void* context{};
 bool(*invoke)(void*,const CharacterSaveRestoreRequestV3&,CharacterSaveRestoreResponseV3&,std::string&){};
};
struct CharacterSaveRestoreResultV3 {std::uint32_t calls{},last_entry{},completed{};};
// Full source Character Serialize/Deserialize control/field order. Delegates
// real lifecycle/FSM/scene/Revive helpers only where actually reached.
bool character_serialize_v3(CharacterSaveRestoreResultV3&,SavegameStreamV2&,const CharacterSaveRestoreBorrowV3&,CharacterSaveRestoreServicesV3,std::string&);
bool character_deserialize_v3(CharacterSaveRestoreResultV3&,SavegameStreamV2&,const CharacterSaveRestoreBorrowV3&,CharacterSaveRestoreServicesV3,std::string&);
}
