#pragma once
#include "particle_factory.hpp"
namespace dh2::animation {
// Continuation of original initParticleSystem 0x656450 after the existing
// generation prefix. All pointers name actual retained model parameters.
struct ParticleResourceInitServicesV1 {
 void* context{};
 int (*vector_parameter)(void*,ParticleGenerationOwner&,const char*,const std::uint32_t[3]){};
 // Source renderer/material/ViewMatrix/BBox stage, after scalar/model setup.
 int (*render_initialize)(void*,ParticleGenerationOwner&,const ParticleEmitterInput&){};
};
// Authored blood domain: emitter sphere, direction type1, spin axis type0,
// descriptor3/mode0. Unknown domains fail before any continuation mutation.
// Missing scalar parameters follow source setParameter: insert null/no store.
// Vector and render stages require their actual providers. Prefix is retained.
int initialize_particle_resource_v1(ParticleGenerationOwner&,const ParticleEmitterInput&,
 const ParticleGenerationInitServices&,const ParticleResourceInitServicesV1&);
}
