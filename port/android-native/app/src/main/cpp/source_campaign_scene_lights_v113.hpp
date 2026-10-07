#pragma once
#include "model_renderer.hpp"
#include <native_scene_lights_v113.hpp>
namespace model_renderer {
//Adopts the SAME Scene/LightSet names C1; no Application/World strong cycle.
bool prepare_source_campaign_light_set_v113(const SourceCampaignCandidateBorrowV55&,
 std::shared_ptr<dh2::world::NativeLightSetV113>&,std::string&);
bool source_campaign_character_init_light_material_v113(const std::shared_ptr<void>&,
 std::uintptr_t character,std::string&);
}
