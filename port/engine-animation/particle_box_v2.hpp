#pragma once
#include "particle_cloud_models_v1.hpp"
namespace dh2::animation {
struct ParticleBoxV2 {float dimensions[3],minimum[3],edges[9];};
extern "C" int dh2_particle_box_construct_v2(ParticleBoxV2*,const float dimensions[3]);
// SEmitter shape block [RadiusLength, Width, Height] -> PDBox(Width,Height,RadiusLength).
inline int particle_box_from_shape_v2(ParticleBoxV2* box,const float shape[3]) {
 if(!shape)return -1;const float xyz[]{shape[1],shape[2],shape[0]};return dh2_particle_box_construct_v2(box,xyz);
}
extern "C" int dh2_particle_box_transform_v2(ParticleBoxV2*,const float source_world16[16]);
extern "C" int dh2_particle_box_generate_v2(float out[3],const ParticleBoxV2*,std::int32_t*);
extern "C" int dh2_particle_box_emitter_init_v2(ParticleSeed100*,std::uint32_t,const ParticleBoxV2*,const float* source_world16,bool local,std::int32_t*);
}
