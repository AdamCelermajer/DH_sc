#pragma once
#include "retained_light_resources_v92.hpp"
#include "player_light_tweaker_owner_v90.hpp"
namespace model_renderer {
struct SourceWorldBorrowV61;struct SourceCampaignCandidateBorrowV55;
bool borrow_campaign_light_base_v92(const std::shared_ptr<SourceWorldBorrowV61>&,std::uintptr_t,dh2::application::TweakLightV90&,std::string&);
bool borrow_campaign_light_stamp_v92(const SourceCampaignCandidateBorrowV55&,dh2::world::LightRuntimeStampV67&,std::string&);
}
