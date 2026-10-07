#pragma once
#include "character_world_npc_state_owner_v1.hpp"
#include "character_player_skills_v3.hpp"
#include "actor_blended_playback.hpp"
#include "navigation_path.hpp"
#include "character_skill_combat_v6.hpp"
namespace dh2::world {struct CanonicalCharacterCandidateRecordV60;}
namespace dh2::world {struct CanonicalCharacterCandidateServicesV60;}
namespace dh2::character {class CharacterCandidateCacheV62;}
namespace model_renderer {
bool source_campaign_character_set_anim_state_v116(const std::shared_ptr<void>&,std::uintptr_t,std::int32_t,bool,bool,std::string&);
bool source_campaign_character_command_move_v116(const std::shared_ptr<void>&,std::uintptr_t,std::uintptr_t,std::string&);
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
struct SourceCharacterPathBorrowV105 {std::shared_ptr<void> owner;dh2::navigation::PathObject* path{};};
bool borrow_source_campaign_character_path_v105(const std::shared_ptr<void>&,
 std::uintptr_t,SourceCharacterPathBorrowV105&,std::string&);
bool source_campaign_character_drop_path_v105(const std::shared_ptr<void>&,
 std::uintptr_t,std::string&);
bool bind_campaign_character_fsm_v101(dh2::world::CanonicalCharacterCandidateRecordV60&,
 const SourceCampaignCandidateBorrowV55&,dh2::character::WorldNpcStateServicesV1&,std::string&);
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
