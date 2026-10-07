#pragma once
#include "particle_emission.hpp"
#include "particle_random_v1.hpp"
namespace dh2::animation {
struct ParticleLifeModelV1 {float life,variation;};
struct ParticleSizeModelV1 {float target,variation,growth,fade;};
struct ParticleSpinModelV1 {float time,variation,phase,phase_variation;std::uint32_t axis_type;float axis[3],axis_variation;};
struct ParticleMotionModelV1 {float direction[3],direction_variation,speed,speed_variation;};
struct ParticleSphereV1 {float center[3],radius,inner_radius,radial_range;std::uint32_t fixed_radius;};
struct ParticleGravityV1 {float strength,falloff;std::uint32_t point_mode;};
// Every raw source field not written by these whole methods remains unchanged.
// The same PSRandom word is borrowed; no internal/random default authority.
extern "C" {
int dh2_particle_life_init_v1(ParticleSeed100*,std::uint32_t,const ParticleLifeModelV1*,float delta,std::int32_t* seed);
int dh2_particle_life_apply_v1(ParticleSeed100*,std::uint32_t,float delta);
int dh2_particle_size_init_v1(ParticleSeed100*,std::uint32_t,const ParticleSizeModelV1*,std::int32_t* seed);
int dh2_particle_size_apply_v1(ParticleSeed100*,std::uint32_t,const ParticleSizeModelV1*);
int dh2_particle_motion_apply_v1(ParticleSeed100*,std::uint32_t,float delta);
int dh2_particle_spin_apply_v1(ParticleSeed100*,std::uint32_t,float delta);
// Whole source Spin init for actual axisType0 (RandVec/normalize), or supplied
// axisType1/2 with variation0. Other reached rotation producers fail explicitly.
int dh2_particle_spin_init_v1(ParticleSeed100*,std::uint32_t,const ParticleSpinModelV1*,std::int32_t* seed);
// Whole PMotion init including source three Euler variation draws, actual root
// matrix (null is genuine source identity).
int dh2_particle_motion_init_v1(ParticleSeed100*,std::uint32_t,const ParticleMotionModelV1*,const float* actual_world_matrix16,std::int32_t* seed);
int dh2_particle_sphere_generate_v1(float* xyz,const ParticleSphereV1*,std::int32_t* seed);
int dh2_particle_sphere_construct_v1(ParticleSphereV1*,const float* actual_center3,float first_radius,float second_radius);
// Source PDSphere::transform replaces only center with world translation.
int dh2_particle_emitter_init_v1(ParticleSeed100*,std::uint32_t,const ParticleSphereV1*,const float* actual_world_matrix16,bool local_space,std::int32_t* seed);
// Whole source PColor fallback only when both actual animation pointers NULL.
// Color is a borrowed source SColor word, not an invented lighting default.
int dh2_particle_color_fallback_v1(ParticleSeed100*,std::uint32_t,const std::uint32_t* actual_scene_color);
// Actual force-node world matrix required. Whole directional/zero-falloff
// gravity domain includes both authored blood resources; no fixed axis.
int dh2_particle_gravity_apply_v1(ParticleSeed100*,std::uint32_t,const ParticleGravityV1*,const float* actual_force_world_matrix16,float delta);
}
}
