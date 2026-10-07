#pragma once
#include "particle_resource_init_v2.hpp"
namespace dh2::animation {
// Source init656450: same box/sphere, direction1, axis0 or velocity axis2.
// Axis2 takes the source zero-vector fallback at656868, not axis1 payload.
int initialize_particle_resource_v32(ParticleGenerationOwner&,const ParticleEmitterInput&,
 const ParticleGenerationInitServices&,const ParticleResourceInitServicesV2&);
}
