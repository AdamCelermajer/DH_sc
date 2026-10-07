#pragma once
#include <cstdint>
namespace dh2::animation {
// Borrowed already-decoded float keys; no particle factory or parameter map.
struct ParticleAccessor16 { const float* values; std::uint32_t count, reserved; };
static_assert(sizeof(ParticleAccessor16)==16);
}
extern "C" {
// 0 complete, -1 malformed caller/index/overlap. Rejection is atomic.
// IEEE inputs are accepted. Key copies preserve bits; arithmetic NaNs follow
// the target floating-point implementation. All arithmetic is separate f32.
int dh2_particle_parameter_key(float*,const dh2::animation::ParticleAccessor16*,std::uint32_t key);
int dh2_particle_parameter_between(float*,const dh2::animation::ParticleAccessor16*,std::uint32_t key,std::uint32_t next,float fraction);
int dh2_particle_parameter_delta_key(float*,const dh2::animation::ParticleAccessor16*,std::uint32_t key,std::uint32_t next);
int dh2_particle_parameter_delta_between(float*,const dh2::animation::ParticleAccessor16*,std::uint32_t reference,std::uint32_t key,std::uint32_t next,float fraction);
// CFloatEx blends AND adds using this same sum: starts +0, includes every
// weight, including a single slot and zero weights; count<=0 writes +0.
int dh2_particle_parameter_blend(float*,const float* values,const float* weights,std::int32_t count);
// Already-resolved BirthRate storage must outlive all bindings. Raw memcpy;
// no clamping, invalidation, emission or GPU service is implied.
int dh2_particle_parameter_apply(float* resolved_parameter,const float* value);
}
