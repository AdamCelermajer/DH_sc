#pragma once
#include "particle_cloud_runtime_v1.hpp"
#include <functional>
namespace dh2::animation {
struct ParticleCloudEmitterV2 {void* context{};int(*initialize)(void*,ParticleSeed100*,std::uint32_t,const float*,bool,std::int32_t*){};};
class ParticleCloudRuntimeV2 {
 ParticleEmissionOwner& emission_;
 ParticleCloudModelsV1& models_;
 std::int32_t& seed_;
 const std::int32_t initial_seed_;
 ParticleCloudRenderV1 render_;
 ParticleCloudEmitterV2 emitter_;
 float now_=0,last_=0;
 bool initialized_=false;
public:
 // References are sole source storage and must outlive this runtime. Initial
 // seed is the genuine factory constructor value, not emitter authored +24.
 ParticleCloudRuntimeV2(ParticleEmissionOwner&,ParticleCloudModelsV1&,std::int32_t&,
                        std::int32_t constructor_seed,ParticleCloudRenderV1,ParticleCloudEmitterV2);
 int initialize();
 // Current null animation-pointer PColor domain, including actual blood.
 // No scene color/force/world/renderer defaults are manufactured.
 int update(float seconds,const ParticleSeed100& untouched,const ParticleCloudFrameV1&);
 float current_time()const{return now_;}
 float last_time()const{return last_;}
};
}
