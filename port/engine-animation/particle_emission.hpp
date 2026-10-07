#pragma once
#include "particle_factory.hpp"
#include "particle_parameter.hpp"
#include "../asset-payloads/payloads.hpp"
#include <array>
#include <memory>
#include <string>
#include <vector>
namespace dh2::animation {
// Diagnostic source SParticle bytes. Unwritten source template bytes are
// supplied explicitly; a complete model provider must initialize them before
// consuming the emitted range as simulated/renderable particles.
using ParticleSeed100=std::array<std::uint8_t,100>;
struct ParticleEmission32 {
 float birth_rate;std::int32_t maximum;float delta,remainder,current_time,last_time;
 std::uint32_t count,reserved;
};
static_assert(sizeof(ParticleEmission32)==32);
struct ParticleEmissionRange8 {std::uint32_t first,end;};
// Source maximum==0 permits uncapped growth. A lower maximum may shrink an
// existing vector and return first>end; these indices reproduce the source
// pointer pair and are not a valid range for model initialization.
class ParticleEmissionOwner {
 std::shared_ptr<ParticleGenerationOwner> generation_;
 std::vector<ParticleSeed100> particles_;
public:
 explicit ParticleEmissionOwner(std::shared_ptr<ParticleGenerationOwner>);
 ParticleEmissionOwner(const ParticleEmissionOwner&)=delete;
 ParticleEmissionOwner& operator=(const ParticleEmissionOwner&)=delete;
 ParticleEmissionOwner(ParticleEmissionOwner&&) noexcept=default;
 ParticleEmissionOwner& operator=(ParticleEmissionOwner&&) noexcept=default;
 int initialize(); // source initGenerationModel: carry=0, clear, reserve(max)
 int generate(float current_time,float last_time,const ParticleSeed100& untouched_seed,
              ParticleEmissionRange8&);
 const std::vector<ParticleSeed100>& particles()const{return particles_;}
 // Borrowed address for required model initialization/simulation providers;
 // invalidated by generate/initialize that reallocates or clears storage.
 ParticleSeed100* particle(std::size_t i){return i<particles_.size()?&particles_[i]:nullptr;}
 const std::shared_ptr<ParticleGenerationOwner>& generation_owner()const{return generation_;}
 // Source update's in-place expired-particle swap/resize. Keeps this sole
 // generation vector and its capacity; no parallel simulation storage.
 std::size_t compact_expired_source_v1();
};
// This owns external animator BRES sampling data. It is intentionally distinct
// from the original cloud AnimationDatabase raw streaming-data parameter.
class ParticleAnimationResource {
 struct Segment {assets::Animation animation;std::vector<float> values;};
 struct Track {std::string uri;std::vector<Segment> segments;};
 std::vector<std::uint8_t> bytes_;std::vector<Track> tracks_;
public:
 static std::shared_ptr<const ParticleAnimationResource> create(const void*,std::size_t,std::string&);
 std::size_t track_count()const{return tracks_.size();}
 const std::string* target(std::size_t)const;
 std::size_t segment_count(std::size_t track)const noexcept;
 std::size_t segment_index(std::size_t track,std::int32_t ms)const noexcept;
 // Independent caller cursor; exact-key history follows original accessor.
 // On rejection output and cursor remain unchanged. Source float keys may be
 // nonfinite; arithmetic NaNs retain target arithmetic classification.
 int sample(std::size_t,std::int32_t milliseconds,std::int32_t& cursor,float& out)const;
};
struct ParticleBirthRateBinding {
 std::shared_ptr<const ParticleAnimationResource> resource;
 ParticleParameterLease parameter;
 std::size_t track=0;std::int32_t cursor=0;
 std::vector<std::int32_t> segment_cursors_v87;
 int sample_apply(std::int32_t milliseconds);
};
}
extern "C" {
// Finite arithmetic/conversion and bounded count domain. Native guard rejects
// malformed/overflow (>65536 particles) atomically; original has no such guard.
int dh2_particle_emission(dh2::animation::ParticleEmission32*,dh2::animation::ParticleEmissionRange8*);
void dh2_particle_emission_template(dh2::animation::ParticleSeed100*);
}
