#pragma once
#include "character_authored_particle_fx_v4.hpp"
namespace dh2::fx {
// General same-scene resource factory: pure meshes and mixed particle meshes
// share the same material/transform/timeline authority.
class CharacterAuthoredResourceFactoryV32 {
 CharacterBloodFxSceneServicesV2 services_;CharacterFxForceFactoryV4 forces_;
 static bool create(void*,std::shared_ptr<const std::vector<std::uint8_t>>,
  const scene::Scene&,std::shared_ptr<CharacterParticleFxResourceV2>&,std::string&);
public:
 CharacterAuthoredResourceFactoryV32(CharacterBloodFxSceneServicesV2 s,CharacterFxForceFactoryV4 f):services_(s),forces_(f){}
 CharacterParticleFxFactoryV2 factory(){return {this,create};}
};
}

