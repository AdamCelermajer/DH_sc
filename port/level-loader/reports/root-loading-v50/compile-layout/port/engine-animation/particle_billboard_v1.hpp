#pragma once
#include "particle_cloud_models_v1.hpp"
namespace dh2::animation {
struct ParticleBillboardBasisV1 {float first[3],second[3];};
struct ParticleBillboardVertexV1 {float position[3];std::uint32_t color;float uv[2];};
struct ParticleBillboardBoundsV1 {float minimum[3],maximum[3];};
// Static startup 6c0b3c and initParticleSystem657100..657138 both initialize
// this same four-vertex source template, proven by original execution.
extern "C" void dh2_particle_billboard_uv_v1(float* eight);
// Whole source applyPRenderData current blood domain (capacity <=30), source
// same-vector AlphaSort partition/insertion and actual camera-position borrow.
extern "C" int dh2_particle_billboard_apply_v1(ParticleSeed100*,std::uint32_t,const float* actual_camera_position3,const float* actual_world_matrix16,bool local_space,ParticleBillboardBoundsV1*);
// Whole unlocked/unoriented source per-system basis branch. Source locks or
// velocity orientation require their retained context fields and another path.
extern "C" int dh2_particle_billboard_basis_v1(ParticleBillboardBasisV1*,const float* actual_camera_view16);
// Whole source billboard per-particle branch with source contextbyte20==0.
extern "C" int dh2_particle_billboard_corners_v1(float* twelve,const ParticleBillboardBasisV1*,const ParticleSeed100*);
// Actual source template UVs required; outputs four ordered source vertices,
// preserving source SColor word. GPU primitive/index ownership stays renderer.
extern "C" int dh2_particle_billboard_vertices_v1(ParticleBillboardVertexV1* four,const ParticleBillboardBasisV1*,const ParticleSeed100*,const float* actual_template_uv8);
}
