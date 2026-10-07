#pragma once
#include "particle_emission.hpp"
namespace dh2::animation {
// Source forceBind667f18 scalar slots: type28 BirthRate, 37 SpinPhase,
// 38 SpinPhaseVariation. Owns sampler data; parameters remain in the SAME
// registered generation/model owner and are pinned by the returned lease.
class ParticleScalarAnimationResourceV6 {
 struct Track {std::string uri;assets::Animation animation;std::vector<float> values;std::uint32_t type;};
 std::vector<std::uint8_t> bytes_;std::vector<Track> tracks_;
public:
 static std::shared_ptr<const ParticleScalarAnimationResourceV6> create(const void*,std::size_t,std::string&);
 std::size_t track_count()const{return tracks_.size();}
 const std::string* target(std::size_t)const;
 const char* parameter_name(std::size_t)const;
 int sample(std::size_t,std::int32_t,std::int32_t&,float&)const;
};
struct ParticleScalarBindingV6 {
 std::shared_ptr<const ParticleScalarAnimationResourceV6> resource;
 ParticleParameterLease parameter;
 std::size_t track{};std::int32_t cursor{};
 int sample_apply(std::int32_t milliseconds);
};
}
