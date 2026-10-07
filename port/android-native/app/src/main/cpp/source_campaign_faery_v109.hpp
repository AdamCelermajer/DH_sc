#pragma once
#include "level_faery_placement_v8.hpp"
#include "character_menu_faery_connection_v8.hpp"
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
class SourceCampaignFaeryV109;
struct SourceFaeryNativeV109 {
 // Independent actual resource authority. Do not lend a containing World.
 dh2::world::LevelFaeryPlacementServicesV8 placement;
 dh2::character::CharacterMenuFaeryServicesV8 visual;
};
bool enroll_source_campaign_faery_v109(const SourceCampaignCandidateBorrowV55&,SourceFaeryNativeV109,std::string&);
bool enroll_native_source_campaign_faery_v109(const std::shared_ptr<void>&,std::string&);
bool borrow_source_campaign_faery_placement_v109(const std::shared_ptr<void>&,dh2::world::LevelFaeryPlacementServicesV8&,std::string&);
bool borrow_source_campaign_menu_faery_v109(const std::shared_ptr<void>&,std::uintptr_t,dh2::character::CharacterMenuFaeryServicesV8&,std::string&);
bool source_campaign_change_faery_v109(const std::shared_ptr<void>&,std::uintptr_t,std::uint32_t,std::string&);
}
