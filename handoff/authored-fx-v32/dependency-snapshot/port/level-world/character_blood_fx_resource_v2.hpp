#pragma once
#include "character_particle_fx_resource_v2.hpp"
namespace dh2::fx {
struct CharacterBloodFxSceneServicesV2 {
 void* context{};
 // Actual camera used by this SAME scene render, never an identity substitute.
 bool (*camera)(void*,float view16[16],float position3[3],std::string&){};
 bool (*driver_type)(void*,std::uint32_t&,std::string&){};
};
class CharacterBloodFxFactoryV2 {
 CharacterBloodFxSceneServicesV2 services_;
 static bool create(void*,std::shared_ptr<const std::vector<std::uint8_t>>,
  const scene::Scene&,std::shared_ptr<CharacterParticleFxResourceV2>&,std::string&);
public:
 explicit CharacterBloodFxFactoryV2(CharacterBloodFxSceneServicesV2 s):services_(s){}
 CharacterParticleFxFactoryV2 factory()noexcept{return {this,create};}
};
}
