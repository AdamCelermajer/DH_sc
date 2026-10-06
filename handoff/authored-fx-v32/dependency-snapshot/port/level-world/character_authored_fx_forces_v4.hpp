#pragma once
#include "character_authored_particle_fx_v4.hpp"
#include "../engine-animation/particle_bound_forces_v4.hpp"
#include <map>
namespace dh2::fx {
// Shared force-node world owner per actual retained Scene; each emitter has
// its own genuine source proxy/history. Weak entries do not extend Scene life.
class CharacterAuthoredFxForceFactoryOwnerV4 {
 std::map<const scene::Scene*,std::weak_ptr<animation::ParticleForceWorldOwnerV4>> worlds_;
 static bool create(void*,const resources::BresView&,const scene::Scene&,
  std::shared_ptr<void>,std::uint32_t,CharacterFxBoundForcesV4&,std::string&);
public:
 CharacterFxForceFactoryV4 factory(){return {this,create};}
};
}
