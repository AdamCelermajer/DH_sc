#pragma once
#include "character_world_npc_state_owner_v1.hpp"
#include "character_player_skills_v3.hpp"
#include "character_skills_owner_v6.hpp"
#include "actor_blended_playback.hpp"
#include "navigation_path.hpp"
#include "character_skill_combat_v6.hpp"
#include "character_script_commands.hpp"
namespace dh2::world {struct CanonicalCharacterCandidateRecordV60;}
namespace dh2::world {struct CanonicalCharacterCandidateServicesV60;}
namespace dh2::character {class CharacterCandidateCacheV62;}
namespace dh2::application {class ApplicationServicesOwnerV5;}
namespace dh2::floors {struct World;}
namespace dh2::navigation {struct CollisionWorld;struct ObstacleRegistry;}
namespace model_renderer {
bool source_campaign_character_set_anim_state_v116(const std::shared_ptr<void>&,std::uintptr_t,std::int32_t,bool,bool,std::string&);
bool source_campaign_character_command_move_v116(const std::shared_ptr<void>&,std::uintptr_t,std::uintptr_t,std::string&);
// Direct original Character::Ctrl_Click point continuation. released=false
// dispatches Character::Ctrl_HeadTo; released=true dispatches Ctrl_MoveTo.
// Caller has already executed the source Ctrl_Click controller gates.
bool source_campaign_character_click_point_v120(const std::shared_ptr<void>&,
 std::uintptr_t,const float* point,bool released,std::string&);
bool source_campaign_character_control_stop_v116(const std::shared_ptr<void>&,std::uintptr_t,std::string&);
bool source_campaign_character_set_limbus_v118(const std::shared_ptr<void>&,std::uintptr_t,bool,std::string&);
int source_campaign_character_reaction_v115(const std::shared_ptr<void>&,
 std::uintptr_t,const dh2::character::skills::SkillApplyRequestV6&,
 dh2::character::skills::SkillApplyResponseV6&,std::string&);
bool source_campaign_character_set_interact_v114(const std::shared_ptr<void>&,
 std::uintptr_t,int,bool,std::uintptr_t,bool,std::string&);
bool source_campaign_character_raise_event_v114(const std::shared_ptr<void>&,
 std::uintptr_t,unsigned,std::uintptr_t,std::string&);
struct SourceCampaignCandidateBorrowV55;
struct SourceCharacterPathBorrowV105 {
 // `owner` remains the compatibility pin used by existing path consumers.
 // The typed leases/identity authenticate this SAME retained FSM path.
 std::shared_ptr<void> owner,record_lease,context_lease;
 std::uintptr_t character{};
 dh2::character::NativeFsm24* machine{};
 dh2::navigation::PathObject* path{};
};
// Typed runtime loan of the SAME canonical character's skill graph. The
// context/state are refreshed per borrow because SkillAIContextV3 is a source
// call view; their pointed-to owner, slots, fields and FSM remain actor-owned.
// Keep both leases while using any pointer and reacquire after actor/session
// replacement. Callbacks using these views must remain synchronous.
struct SourceCampaignCharacterSkillContextBorrowV1 {
 std::shared_ptr<void> record_lease,context_lease;
 std::uintptr_t character{};
 dh2::character::NativeFsm24* machine{};
 dh2::character::CharacterScriptSession* session_v1{};
 dh2::character::CharacterScriptSessionV3* session_v3{};
 dh2::data::PropertyView* property_view{};
 dh2::data::SkillTables::Borrow skill_tables;
 dh2::character::skills::CharacterSkillOwner* instances_v1{};
 dh2::character::skills::CharacterSkillOwnerV6* instances_v3{};
 dh2::character::skills::SkillAIContextV3* ai{};
 dh2::character::skills::SkillStateV4* state{};
 dh2::character::skills::SkillAIServices16V3 ai_services{};
 dh2::character::skills::SkillStateServices16V4 state_services{};
};
bool borrow_source_campaign_character_path_v105(const std::shared_ptr<void>&,
 std::uintptr_t,SourceCharacterPathBorrowV105&,std::string&);
// Record-based loan for hosts that already resolved their actor mapping. Pins
// and authenticates the same stored CampaignFsmV101 and its actor path.
bool borrow_source_campaign_character_path_v105(
 const std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>&,
 SourceCharacterPathBorrowV105&,std::string&);
bool borrow_source_campaign_character_skill_context_v1(const std::shared_ptr<void>&,
 std::uintptr_t,SourceCampaignCharacterSkillContextBorrowV1&,std::string&);
// Record-based entry for host bootstraps that already resolved their own
// actor-ID mapping. This authenticates the retained record/FSM graph directly.
bool borrow_source_campaign_character_skill_context_v1(
 const std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>&,
 SourceCampaignCharacterSkillContextBorrowV1&,std::string&);
bool source_campaign_character_drop_path_v105(const std::shared_ptr<void>&,
 std::uintptr_t,std::string&);
bool bind_campaign_character_fsm_v101(dh2::world::CanonicalCharacterCandidateRecordV60&,
 const SourceCampaignCandidateBorrowV55&,dh2::character::WorldNpcStateServicesV1&,std::string&);
// Menu Characters borrow the same constructor-produced process PFWorld used
// by their physical/height providers. No campaign Level or sewn floor exists.
bool bind_process_character_fsm_v121(dh2::world::CanonicalCharacterCandidateRecordV60&,
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 const std::shared_ptr<void>&,const dh2::navigation::CollisionWorld&,
 dh2::navigation::ObstacleRegistry&,dh2::character::WorldNpcStateServicesV1&,std::string&);
// Clean backend entry for a real campaign-world record whose world/floors/PF
// owners are already retained by the host. Reuses the sole CampaignFsmV101;
// does not require the Android renderer's global SourceCampaignCandidateBorrow.
bool bind_backend_character_fsm_v1(dh2::world::CanonicalCharacterCandidateRecordV60&,
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 const std::shared_ptr<void>& same_world_lease,
 const std::shared_ptr<dh2::floors::World>& same_sewn_floors,
 dh2::navigation::ObstacleRegistry& same_obstacle_registry,
 dh2::character::WorldNpcStateServicesV1&,std::string&);
bool bind_campaign_character_player_events_v101(dh2::world::CanonicalCharacterCandidateRecordV60&,
 dh2::character::skills::PlayerSkillGameplayServicesV3&,std::string&);
bool bind_campaign_character_npc_events_v101(dh2::world::CanonicalCharacterCandidateRecordV60&,
 dh2::character::CharacterScriptSessionInput&,std::string&);
void source_campaign_character_animation_event_v101(void*,dh2::actor::BlendedPlayback&,
 const dh2::actor::BlendedPlaybackEvent&);
bool source_campaign_character_fsm_update_v101(const std::shared_ptr<void>&,
 std::uintptr_t,std::string&);
 bool source_campaign_character_target_update_v108(const std::shared_ptr<void>&,
 std::uintptr_t,std::string&);
bool source_campaign_character_master_update_v108(const std::shared_ptr<void>&,
 std::uintptr_t,std::string&);
bool source_campaign_character_ai_on_update_v108(const std::shared_ptr<void>&,
 std::uintptr_t,std::string&);
bool source_campaign_character_ai_update_v108(const std::shared_ptr<void>&,
 std::uintptr_t,std::string&);
bool source_campaign_character_add_aggro_v108(const std::shared_ptr<void>&,
 std::uintptr_t,std::uintptr_t,float,std::string&);
bool source_campaign_character_timers_update_v102(const std::shared_ptr<void>&,
 std::uintptr_t,std::uint32_t actual_dt,std::uint32_t actual_script_blocked,std::string&);
bool source_campaign_character_animator_update_v102(const std::shared_ptr<void>&,
 std::uintptr_t,std::string&);
bool source_campaign_character_enabled_event_v102(const std::shared_ptr<void>&,
 std::uintptr_t,bool,std::string&);
bool source_campaign_character_zone_entered_v102(const std::shared_ptr<void>&,
 std::uintptr_t,std::string&);
bool source_campaign_character_zone_exited_v102(const std::shared_ptr<void>&,
 std::uintptr_t,std::string&);
bool source_campaign_character_is_zonable_v104(const std::shared_ptr<void>&,
 std::uintptr_t,bool&,std::string&);
bool source_campaign_character_command_stop_v101(const std::shared_ptr<void>&,
 std::uintptr_t,std::string&);
bool source_campaign_character_command_heading_v101(const std::shared_ptr<void>&,
 std::uintptr_t,const float*,std::string&);
bool bind_campaign_character_sound_names_v101(const std::shared_ptr<const dh2::character::CharacterCandidateCacheV62>&,
 dh2::world::CanonicalCharacterCandidateServicesV60&,std::string&);
}
