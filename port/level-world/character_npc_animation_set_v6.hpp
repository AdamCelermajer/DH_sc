#pragma once
#include "../game-data/animation_tables.hpp"
#include <functional>
namespace dh2::character {
enum class NpcAnimationRegistrationOperationV6 {debug_load,debug_trace,add_template,add_animation,preload_sound,preload_fx};
struct NpcAnimationRegistrationRequestV6 {NpcAnimationRegistrationOperationV6 operation;std::int32_t set_id,resource;};
struct NpcAnimationRegistrationServicesV6 {
 // Actual manager pointer null/non-null controls the original sound branch.
 bool sound_manager_present{};
 std::function<bool(const NpcAnimationRegistrationRequestV6&,std::string&)> invoke;
 // Native production reads the actual nullable global AFTER each AddAnim,
 // immediately before reading/delivering that step's sound. The provider may
 // retain that exact borrow through preload_sound; no early snapshot is used.
 std::function<bool(bool&,std::string&)> current_sound_manager;
};
// Whole non-player SetAnimationSet3c9f4c registration branch. Actual manager
// Exists and source set-id publication precede this entry at the caller.
// Every request is delivered at its source point; failure retains its prefix.
bool character_npc_register_animation_set_v6(const data::AnimationTables&,
 std::int32_t table_index,std::int32_t same_set_id,
 const std::vector<std::int32_t>& actual_skill_animation_ids,
 NpcAnimationRegistrationServicesV6,std::string&);
}
