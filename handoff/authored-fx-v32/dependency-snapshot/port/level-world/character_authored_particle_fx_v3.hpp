#pragma once
#include "character_blood_fx_resource_v2.hpp"
namespace dh2::fx {
// Whole reached descriptor3/mode0 sphere/box resources with typed authored
// ProfileCOMMON defaults and full uchar4 diffuse-color timeline.
class CharacterAuthoredParticleFxFactoryV3 {
 CharacterBloodFxSceneServicesV2 services_;
 static bool create(void*,std::shared_ptr<const std::vector<std::uint8_t>>,const scene::Scene&,std::shared_ptr<CharacterParticleFxResourceV2>&,std::string&);
public:
 explicit CharacterAuthoredParticleFxFactoryV3(CharacterBloodFxSceneServicesV2 s):services_(s){}
 CharacterParticleFxFactoryV2 factory()noexcept{return {this,create};}
};
}
