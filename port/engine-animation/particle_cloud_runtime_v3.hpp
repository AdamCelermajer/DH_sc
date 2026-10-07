#pragma once
#include "particle_cloud_runtime_v2.hpp"
namespace dh2::animation {
struct ParticleCloudForcesV3 {
 void* context{};
 int(*apply)(void*,ParticleSeed100*,std::uint32_t,float,std::int32_t*){};
};
// Versioned source-order successor. One emitter's exact particles/models/RNG
// remain borrowed. Actual bound force models (including retained Deflector
// history) are supplied in declaration order; no force is silently omitted.
class ParticleCloudRuntimeV3 {
 ParticleEmissionOwner& emission_;ParticleCloudModelsV1& models_;
 std::int32_t& seed_;const std::int32_t initial_seed_;
 ParticleCloudRenderV1 render_;ParticleCloudEmitterV2 emitter_;ParticleCloudForcesV3 forces_;
 float now_=0,last_=0;bool initialized_=false;
public:
 ParticleCloudRuntimeV3(ParticleEmissionOwner&,ParticleCloudModelsV1&,std::int32_t&,
  std::int32_t,ParticleCloudRenderV1,ParticleCloudEmitterV2,ParticleCloudForcesV3);
 int initialize();
 int update(float,const ParticleSeed100&,const ParticleCloudFrameV1&);
};
}
