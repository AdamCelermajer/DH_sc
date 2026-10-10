#pragma once
#include <memory>
#include <cstdint>
#include <string>
namespace dh2::android_ui {struct SourceScriptActorLeavesV96;}
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
bool build_source_campaign_script_actor_leaves_v96(const SourceCampaignCandidateBorrowV55&,
 dh2::android_ui::SourceScriptActorLeavesV96&,std::string&);
//Existing whole Cmd_LookAt over SAME active controller and target loan.
bool source_campaign_character_command_look_v114(const std::shared_ptr<void>& actual_world,
 std::uintptr_t actual_character,std::uintptr_t actual_target,std::string&);
bool source_campaign_character_play_animation_v116(const std::shared_ptr<void>&,
 std::uintptr_t,std::int32_t first,std::int32_t second,std::int32_t base,std::string&);
bool source_campaign_character_animation_blocking_v116(const std::shared_ptr<void>&,
 std::uintptr_t,std::int32_t base,bool&,std::string&);
bool source_campaign_character_move_actor_v116(const std::shared_ptr<void>&,
 std::uintptr_t,std::uintptr_t,bool skip,bool disable_collisions,bool wait,
 std::uint8_t& original_static,bool& retained_wait,std::string&);
bool source_campaign_character_move_blocking_v116(const std::shared_ptr<void>&,
 std::uintptr_t,std::uint8_t original_static,bool&,std::string&);
bool source_campaign_object_set_visible_v116(const std::shared_ptr<void>&,
 std::uintptr_t,bool,std::string&);
// Direct Character.VerifySpecialization3bd000 leaf, without ReloadSkills.
bool source_campaign_character_verify_specialization_v134(const std::shared_ptr<void>&,
 std::uintptr_t,std::string&);
//Whole inherited source virtual40, also required by startup/PM AddCharacter.
bool source_campaign_character_set_visible_v96(const std::shared_ptr<void>& actual_world,
 std::uintptr_t actual_character,bool,std::string&);
//VisualObject.SyncVisibility4713d0 reads the existing source80 without
//executing another SetVisible store; required by base OBJS deserialize.
bool source_campaign_character_sync_visibility_v86(const std::shared_ptr<void>& actual_world,
 std::uintptr_t actual_character,std::string&);
//Implemented by Root's original UI boundary: SAME RenderFX138 invokes
//_root.menu_CharacterMenu.IsSpecTime with one genuine source bool argument.
bool source_native_character_spec_time_v96(const std::shared_ptr<void>& actual_world,bool,std::string&);
}
