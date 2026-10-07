#pragma once
#include <character_kill_production_v23.hpp>
#include <character_progression_world_v23.hpp>
#include <canonical_character_death_v84.hpp>
#include <character_player_aggro_owner_v1.hpp>
#include <character_skill_combat_v6.hpp>
#include <memory>
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
class SourceCampaignDeathRewardsV84;
// Positive presentation/audio/group leaves belong to their existing native
// owners. The bridge supplies actual canonical fields, PM, constants, Debug,
// private Save writer, Level events, item pool and reciprocal aggro itself.
struct SourceDeathRewardsLeavesV84 {
 std::shared_ptr<void> owner;
 std::function<bool(std::uintptr_t,std::uintptr_t,std::uintptr_t,std::string&)> group;
 std::function<bool(const dh2::character::PlayerAggroRequestV1&,
  dh2::character::PlayerAggroResponseV1&,std::string&)> player_aggro;
 std::function<bool(dh2::character::ProgressionActorV1&,std::int32_t,std::string&)> level_presentation;
 std::function<bool(dh2::character::ProgressionActorV1&,dh2::character::ProgressionActorV1&,
  std::int32_t,std::string&)> xp_text;
 //Source updateJob_thread.Start2 after level2 clears actual settings byte2e.
 //Not the separate profile Savegame job queue and not an instant-save stub.
 std::function<bool(std::string&)> settings_job_start;
};
// Publish one native transport on the actual campaign façade; no actor,
// SkillOwner, C1 pool, Save, source Level or gameplay flag is constructed here.
bool bind_source_campaign_death_rewards_v84(const SourceCampaignCandidateBorrowV55&,
 SourceDeathRewardsLeavesV84,std::string&);
bool bind_native_death_presentation_v84(const std::shared_ptr<void>& actual_world,
 SourceDeathRewardsLeavesV84&,std::string&);
//Original LevelUp player suffix after status enqueue/notification. Reuses
//actual ScriptManager, FX, settings, PM/Trophy and menu RenderFX receivers.
bool source_campaign_level_up_presentation_v88(const std::shared_ptr<void>& actual_world,
 dh2::character::ProgressionActorV1&,std::int32_t actual_level,std::string&);
// Tri-state: 0 another HitFor service,1 actual synchronous Cmd_Kill delivery,
// -1 reached original failure. The callback scope is the actual nullable VM
// scope from the calling skill, never a scope minted by this transport.
int source_campaign_hit_kill_v84(const std::shared_ptr<void>& actual_world,
 dh2::character::HitActor32*,const dh2::character::HitRequest32*,
 const dh2_script_callback_scope*,std::uintptr_t*,std::string&);
bool source_campaign_cmd_kill_v84(const std::shared_ptr<void>& actual_world,
 std::uintptr_t actual_controller,std::uintptr_t actual_character,std::uintptr_t attacker,
 std::uint32_t force,const dh2_script_callback_scope*,std::string&);
bool source_campaign_character_ai_set_dead_v86(const std::shared_ptr<void>& actual_world,
 std::uintptr_t actual_character,std::string&);
//Set/AddAggro source notifications: TARGET is the CharAI receiver and OWNER
//the argument. Both directions share the SAME selected AISPlayer counters.
bool source_campaign_character_aggro_event_v84(const std::shared_ptr<void>& actual_world,
 std::uint32_t source_bit,std::uintptr_t owner,std::uintptr_t target,
 const dh2_script_callback_scope*,std::string&);
bool source_campaign_character_clear_aggro_v84(const std::shared_ptr<void>& actual_world,
 std::uintptr_t owner,std::uintptr_t actual_other,const dh2_script_callback_scope*,std::string&);
bool source_campaign_forget_kill_actor_v84(const std::shared_ptr<void>& actual_world,
 std::uintptr_t actual_character,std::string&);
bool source_campaign_give_xp_v108(const std::shared_ptr<void>& actual_world,std::uintptr_t character,std::int32_t raw,bool stats,bool& accepted,std::string&);
bool release_source_campaign_death_rewards_v84(const SourceCampaignCandidateBorrowV55&,std::string&);
}
