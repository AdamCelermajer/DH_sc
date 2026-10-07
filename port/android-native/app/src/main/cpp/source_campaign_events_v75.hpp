#pragma once
#include "game_event_runtime_v75.hpp"
namespace dh2::loader {class CanonicalLevelContextV1;}
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
//Despite an older pickup seam named current_game_state, original3ffb64 is
//Application.GetCurrentLevel. Pin that exact Level before constant lookup;
//RaiseAsync uses the captured receiver even after a synchronous global change.
struct SourceCampaignLevelEventsBorrowV88 {
 std::shared_ptr<dh2::loader::CanonicalLevelContextV1> level;
 dh2::events::EventManagerOwnerV12* events{};
 std::uintptr_t identity{};
 explicit operator bool()const noexcept{return bool(level);}
};
bool borrow_source_campaign_level_events_v88(const std::shared_ptr<void>& actual_world,
 SourceCampaignLevelEventsBorrowV88&,std::string&);
bool raise_source_campaign_level_event_v88(const SourceCampaignLevelEventsBorrowV88&,
 const dh2::events::EventBorrowV12&,std::string&);
struct SourceGameEventDependenciesV75 {
 std::shared_ptr<void> provider;
 //Actual Character.s_cachedCharOIDs/count and template-fallback owners.
 //Never substitute immutable Arrays membership or a zero-count heuristic.
 std::function<bool(bool,std::int32_t,std::int32_t&,std::string&)> cached_char_count;
 std::function<bool(std::uintptr_t,std::int32_t&,std::string&)> character_template_id;
 std::function<bool(const dh2::events::EventBorrowV12&,dh2::loader::GameEventQuestBorrowV75&,std::string&)> project_quest_event;
 std::function<bool(const dh2::loader::GameEventQuestBorrowV75&,std::string&)> send_network_event;
};
bool bind_source_campaign_event_dependencies_v75(const SourceCampaignCandidateBorrowV55&,SourceGameEventDependenciesV75,std::string&);
//Shared Objective methods are callable before Stage6 produces194. This creates
//no GameEventManager and does not certify source event Load/Compile readiness.
bool borrow_source_campaign_objectives_v75(const SourceCampaignCandidateBorrowV55&,std::shared_ptr<dh2::loader::GameEventRuntimeV75>&,std::string&);
bool borrow_source_campaign_events_v75(const SourceCampaignCandidateBorrowV55&,std::shared_ptr<dh2::loader::GameEventRuntimeV75>&,std::string&);
}
