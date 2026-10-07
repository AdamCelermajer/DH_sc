#include "particle_emission.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <stdexcept>
namespace {
bool aligned(const void* p,std::size_t a){return p&&std::uintptr_t(p)%a==0;}
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){auto x=std::uintptr_t(a),y=std::uintptr_t(b);return x<y?y-x<an:x-y<bn;}
std::uint32_t word(const std::uint8_t* p){std::uint32_t v;std::memcpy(&v,p,4);return v;}
void put(dh2::animation::ParticleSeed100& p,std::size_t o,std::uint32_t v){std::memcpy(p.data()+o,&v,4);}
}
extern "C" int dh2_particle_emission(dh2::animation::ParticleEmission32* s,dh2::animation::ParticleEmissionRange8* out){
 if(!aligned(s,alignof(decltype(*s)))||!aligned(out,alignof(decltype(*out)))||overlap(s,sizeof(*s),out,sizeof(*out)))return -1;
 auto r=*s;if(r.reserved||r.count>65536||r.maximum<0||r.maximum>65536||!std::isfinite(r.birth_rate)||!std::isfinite(r.remainder)||!std::isfinite(r.current_time)||!std::isfinite(r.last_time))return -1;
 r.delta=r.current_time-r.last_time;float product=r.delta*r.birth_rate;float amount=product+r.remainder;
 if(!std::isfinite(r.delta)||!std::isfinite(amount)||amount>=2147483648.0f||amount<-2147483648.0f)return -1;
 auto born=static_cast<std::int32_t>(amount);r.remainder=amount-static_cast<float>(born);
 std::uint32_t desired=r.count;
 if(born>0){auto total=std::uint64_t(r.count)+std::uint32_t(born);if(r.maximum&&total>std::uint32_t(r.maximum))desired=std::uint32_t(r.maximum);else{if(total>65536)return -1;desired=std::uint32_t(total);}}
 *out={r.count,desired};r.count=desired;*s=r;return 0;
}
extern "C" void dh2_particle_emission_template(dh2::animation::ParticleSeed100* p){
 if(!p)return;for(auto o:{0u,4u,8u,12u,16u,20u,32u,36u,40u,48u,52u,56u,84u,88u,92u})put(*p,o,0);
 put(*p,24,0xffffffff);put(*p,28,0x3f800000);put(*p,44,0x3f800000);
}
namespace dh2::animation {
ParticleEmissionOwner::ParticleEmissionOwner(std::shared_ptr<ParticleGenerationOwner> owner):generation_(std::move(owner)){if(!generation_)throw std::invalid_argument("Missing particle generation owner");}
int ParticleEmissionOwner::initialize(){auto& g=const_cast<ParticleGeneration16&>(generation_->generation());if(g.max_particles>65536)return -1;g.field_10=0;particles_.clear();particles_.reserve(g.max_particles);return 0;}
int ParticleEmissionOwner::generate(float now,float last,const ParticleSeed100& seed,ParticleEmissionRange8& out){
 auto& g=const_cast<ParticleGeneration16&>(generation_->generation());if(overlap(&out,sizeof(out),&g,sizeof(g)))return -1;float carry;std::memcpy(&carry,&g.field_10,4);
 ParticleEmission32 s{g.birth_rate,std::int32_t(g.max_particles),0,carry,now,last,std::uint32_t(particles_.size()),0};ParticleEmissionRange8 range;
 auto status=dh2_particle_emission(&s,&range);if(status)return status;
 auto value=seed;dh2_particle_emission_template(&value);particles_.resize(s.count,value);
 std::memcpy(&g.field_c,&s.delta,4);std::memcpy(&g.field_10,&s.remainder,4);out=range;return 0;
}
std::shared_ptr<const ParticleAnimationResource> ParticleAnimationResource::create(const void* data,std::size_t size,std::string& error){
 if(!data||!size){error="Missing particle animation resource";return {};}
 auto result=std::make_shared<ParticleAnimationResource>();const auto* p=static_cast<const std::uint8_t*>(data);result->bytes_.assign(p,p+size);resources::BresView view{};
 if(dh2_bres_open(&view,result->bytes_.data(),size)!=resources::BresError::ok){error="Invalid particle animation BRES";return {};}
 const auto segments=dh2_animation_segments(&view);if(!segments||segments>256){error="Invalid particle authored segment domain";return {};}
 auto n=dh2_bres_library_count(&view,resources::Library::animation);
 for(std::uint32_t i=0;i<n;++i){assets::Animation a{};if(dh2_animation_open(&a,&view,i,0)!=assets::Error::ok){error="Invalid particle animation record";return {};}
  bool particle=false;for(std::uint32_t channel=0;channel<dh2_animation_channels(&a);++channel)particle|=dh2_animation_type(&a,channel)==28;
  if(!particle)continue;if(dh2_animation_channels(&a)!=1){error="Unsupported multichannel particle accessor";return {};}
  Track track;track.uri=dh2_animation_target(&a)?dh2_animation_target(&a):"";
  for(std::uint32_t segment=0;segment<segments;++segment){
  if(dh2_animation_open(&a,&view,i,segment)!=assets::Error::ok||dh2_animation_type(&a,0)!=28||dh2_animation_channels(&a)!=1){error="Invalid particle segment record";return {};}
  if(segment&&a.segment_start!=track.segments.back().animation.segment_end){error="Noncontiguous particle segments";return {};}
  assets::Vector values{},times{};const auto* target=dh2_animation_target(&a);
  if(!target||dh2_animation_samplers(&a)!=1||dh2_animation_offsets(&a)||dh2_animation_scales(&a)||!dh2_animation_vector(&a,0,true,&values)||!dh2_animation_vector(&a,0,false,&times)||values.type!=6||values.components!=1||!values.count||times.count!=values.count||(times.type!=1&&times.type!=3&&times.type!=4)){error="Unsupported particle scalar accessor";return {};}
  if(track.uri!=target||a.segment_end<=a.segment_start){error="Particle segment target/range differs";return {};}
  Segment entry;entry.animation=a;entry.values.resize(values.count);
  for(std::uint32_t k=0;k<values.count;++k){std::uint32_t raw=word(values.data+4*k);std::memcpy(&entry.values[k],&raw,4);if(k&&dh2_animation_key_time(&a,0,k)<dh2_animation_key_time(&a,0,k-1)){error="Unordered particle keys";return {};}}
  track.segments.push_back(std::move(entry));
  }
  result->tracks_.push_back(std::move(track));
 }
 if(result->tracks_.empty()){error="No type28 particle tracks";return {};}
 error.clear();return result;
}
const std::string* ParticleAnimationResource::target(std::size_t i)const{return i<tracks_.size()?&tracks_[i].uri:nullptr;}
std::size_t ParticleAnimationResource::segment_count(std::size_t i)const noexcept{return i<tracks_.size()?tracks_[i].segments.size():0;}
std::size_t ParticleAnimationResource::segment_index(std::size_t i,std::int32_t ms)const noexcept{if(i>=tracks_.size())return 0;const auto& segments=tracks_[i].segments;std::size_t index=0;while(index+1<segments.size()&&ms>=segments[index].animation.segment_end)++index;return index;}
int ParticleAnimationResource::sample(std::size_t i,std::int32_t ms,std::int32_t& cursor,float& out)const{
 if(i>=tracks_.size()||overlap(&cursor,4,&out,4))return -1;const auto& track=tracks_[i].segments[segment_index(i,ms)];if(cursor<0||std::uint32_t(cursor)>=track.values.size())return -1;
 assets::Vector times{};if(!dh2_animation_vector(&track.animation,0,false,&times))return -1;
 const float factor=times.type==4?1.0f:0x1.0aaaaap+5f;float frame=static_cast<float>(ms)/factor;
 auto time=[&](std::int32_t k){float v=0;dh2_vector_read(&times,k,&v);return v;};
 auto key=cursor;auto last=static_cast<std::int32_t>(times.count)-1;
 if(time(key)>frame){if(key>0)--key;}else if(key<last&&time(key+1)<frame){++key;if(key<last&&time(key+1)<frame)++key;}
 if(time(key)>frame||(key<last&&time(key+1)<frame))dh2_animation_find_index(&track.animation,0,ms,&key);
 float first=time(key)*factor;bool between=dh2_animation_interpolation(&track.animation,0)!=0&&static_cast<float>(ms)!=first&&key!=last;
 float fraction=0,value;
 if(between){float next=time(key+1)*factor;if(!(first>=-2147483648.0f&&first<2147483648.0f&&next>=-2147483648.0f&&next<2147483648.0f))return -1;
  auto start=static_cast<std::int32_t>(first),end=static_cast<std::int32_t>(next);std::uint32_t a=std::uint32_t(ms)-std::uint32_t(start),b=std::uint32_t(end)-std::uint32_t(start);std::int32_t sa,sb;std::memcpy(&sa,&a,4);std::memcpy(&sb,&b,4);float ratio=static_cast<float>(sa)/static_cast<float>(sb);fraction=ratio<0?0:ratio<1?ratio:1;}
 if(key<0||std::uint32_t(key)>=track.values.size()||(between&&std::uint32_t(key+1)>=track.values.size()))return -1;
 ParticleAccessor16 accessor{track.values.data(),std::uint32_t(track.values.size()),0};int status=between?dh2_particle_parameter_between(&value,&accessor,key,key+1,fraction):dh2_particle_parameter_key(&value,&accessor,key);
 if(status)return status;cursor=key;out=value;return 0;
}
int ParticleBirthRateBinding::sample_apply(std::int32_t ms){if(!resource||!parameter.owner||parameter.storage!=&parameter.owner->generation().birth_rate||track>=resource->track_count())return -1;const auto count=resource->segment_count(track);if(!count)return -1;if(segment_cursors_v87.empty()){segment_cursors_v87.resize(count,0);segment_cursors_v87[0]=cursor;}if(segment_cursors_v87.size()!=count)return -1;const auto segment=resource->segment_index(track,ms);auto key=count==1?cursor:segment_cursors_v87[segment];float value;auto status=resource->sample(track,ms,key,value);if(status)return status;status=dh2_particle_parameter_apply(static_cast<float*>(parameter.storage),&value);if(!status){cursor=key;segment_cursors_v87[segment]=key;}return status;}
}
