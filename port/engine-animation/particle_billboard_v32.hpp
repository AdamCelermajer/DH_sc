#pragma once
#include "particle_billboard_v1.hpp"
namespace dh2::animation {
// Original applyPRenderData651eec has no thirty-particle domain limit.
// Same source sort/BBox, extended through retained 16-bit quad capacity.
extern "C" int dh2_particle_billboard_apply_v32(ParticleSeed100*,std::uint32_t,
 const float* camera_position3,const float* world_matrix16,bool local_space,
 ParticleBillboardBoundsV1*);
}
