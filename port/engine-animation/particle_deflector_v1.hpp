#pragma once
#include "particle_emission.hpp"
#include "particle_random_v1.hpp"
#include "../engine-math/math.hpp"
namespace dh2::animation {
// SForce type2's seven float words, copied by CDeflectorForceSceneNode631050.
struct ParticleDeflectorParametersV1 {
 float bounce,bounce_variation,chaos,friction,inherit_velocity,width,length;
};
struct ParticleDeflectorModelV1 {
 const ParticleDeflectorParametersV1* parameters{}; // same retained force fields
 math::Matrix4f previous{}; // PDeflector+4..44, source constructor snapshot
};
extern "C" {
int dh2_particle_deflector_construct_v1(ParticleDeflectorModelV1*,
 const ParticleDeflectorParametersV1*,const math::Matrix4f* actual_force_world);
// Whole source SParticle100 apply6338e0. Current and previous identity markers
// are actual mutable source fields; same context PSRandom is required on hit.
// -2 is a reached missing PSRandom borrow after matrix-marker mutation.
int dh2_particle_deflector_apply_v1(ParticleSeed100*,std::uint32_t,
 ParticleDeflectorModelV1*,math::Matrix4f* actual_force_world,float delta,
 std::int32_t* same_source_seed);
float dh2_particle_deflector_friction_v1(float,float,float,float);
}
}
