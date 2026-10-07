#pragma once
#include <player_light_tweaker_owner_v90.hpp>
namespace model_renderer {
struct SourceWorldBorrowV61;
bool bind_campaign_player_light_tweak_services_v90(
 const std::shared_ptr<SourceWorldBorrowV61>&,dh2::application::PlayerLightTweakServicesV90&,std::string&);
bool set_campaign_player_light_tweak_value_v90(
 const std::shared_ptr<SourceWorldBorrowV61>&,std::size_t,
 const dh2::application::TweakAttributesV90&,std::int32_t,std::string&);
}
