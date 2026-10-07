#pragma once
#include "source_campaign_scene_lights_v113.hpp"
#include <catalog_environment_v67.hpp>
namespace dh2::application {struct TweakLightV90;}
namespace model_renderer {
struct SourceWorldBorrowV61;
class SourceCampaignLightEnvironmentV113;
bool prepare_source_campaign_light_environment_v113(const SourceCampaignCandidateBorrowV55&,
 std::shared_ptr<const dh2::loader::LightEnvironmentLeavesV67>&,std::string&);
bool borrow_source_campaign_light_quiescence_v113(const std::shared_ptr<void>&,
 std::shared_ptr<dh2::world::LightQuiescenceLeaseV67>&,std::string&);
bool update_source_campaign_light_set_v113(const std::shared_ptr<void>&,std::string&);
//Only actual SceneManager.clear354468 after its qualified Scene.clear589888,
//not LevelD1's separate bare virtual68 parent drop.
bool clear_source_campaign_light_set_v113(const std::shared_ptr<void>&,std::string&);
bool borrow_source_campaign_tweak_light_v113(const std::shared_ptr<SourceWorldBorrowV61>&,
 std::uintptr_t,dh2::application::TweakLightV90&,std::string&);
}
