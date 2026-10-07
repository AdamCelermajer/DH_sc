#pragma once
#include "particle_emission.hpp"
namespace dh2::animation {
// Source forceBind667f18 scalar slots: type28 BirthRate, 37 SpinPhase,
// 38 SpinPhaseVariation. Owns sampler data; parameters remain in the SAME
// registered generation/model owner and are pinned by the returned lease.
class ParticleScalarAnimationResourceV6 {
 struct Segment {assets::Animation animation;std::vector<float> values;};
 struct Track {std::string uri;std::vector<Segment> segments;std::uint32_t type;};
 std::vector<std::uint8_t> bytes_;std::vector<Track> tracks_;
public:
 static std::shared_ptr<const ParticleScalarAnimationResourceV6> create(const void*,std::size_t,std::string&);
 std::size_t track_count()const{return tracks_.size();}
 const std::string* target(std::size_t)const;
 const char* parameter_name(std::size_t)const;
 std::size_t segment_count(std::size_t)const noexcept;
 std::size_t segment_index(std::size_t,std::int32_t)const noexcept;
 int sample(std::size_t,std::int32_t,std::int32_t&,float&)const;
};
struct ParticleScalarBindingV6 {
 std::shared_ptr<const ParticleScalarAnimationResourceV6> resource;
 ParticleParameterLease parameter;
 std::size_t track{};std::int32_t cursor{};
 std::vector<std::int32_t> segment_cursors_v87;
 int sample_apply(std::int32_t milliseconds);
};
}
