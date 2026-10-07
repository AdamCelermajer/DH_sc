#include "particle_scalar_animation_v6.hpp"
#include <cstring>
namespace {
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){auto x=std::uintptr_t(a),y=std::uintptr_t(b);return x<y?y-x<an:x-y<bn;}
std::uint32_t word(const std::uint8_t* p){std::uint32_t v;std::memcpy(&v,p,4);return v;}
}
namespace dh2::animation {
std::shared_ptr<const ParticleScalarAnimationResourceV6> ParticleScalarAnimationResourceV6::create(const void* data,std::size_t size,std::string& error){
 if(!data||!size){error="Missing particle animation resource";return {};}
 auto result=std::make_shared<ParticleScalarAnimationResourceV6>();const auto* p=static_cast<const std::uint8_t*>(data);result->bytes_.assign(p,p+size);resources::BresView view{};
 if(dh2_bres_open(&view,result->bytes_.data(),size)!=resources::BresError::ok){error="Invalid particle animation BRES";return {};}
 if(dh2_animation_segments(&view)!=1){error="Particle animation requires one authored segment";return {};}
 auto n=dh2_bres_library_count(&view,resources::Library::animation);
 for(std::uint32_t i=0;i<n;++i){assets::Animation a{};if(dh2_animation_open(&a,&view,i,0)!=assets::Error::ok){error="Invalid particle animation record";return {};}
  bool particle=false;for(std::uint32_t channel=0;channel<dh2_animation_channels(&a);++channel)particle|=(dh2_animation_type(&a,channel)==28||dh2_animation_type(&a,channel)==37||dh2_animation_type(&a,channel)==38);
  if(!particle)continue;if(dh2_animation_channels(&a)!=1){error="Unsupported multichannel particle accessor";return {};}
  assets::Vector values{},times{};const auto* target=dh2_animation_target(&a);
  if(!target||dh2_animation_samplers(&a)!=1||dh2_animation_offsets(&a)||dh2_animation_scales(&a)||!dh2_animation_vector(&a,0,true,&values)||!dh2_animation_vector(&a,0,false,&times)||values.type!=6||values.components!=1||!values.count||times.count!=values.count||(times.type!=1&&times.type!=3&&times.type!=4)){error="Unsupported particle scalar accessor";return {};}
  Track track;track.type=dh2_animation_type(&a,0);track.uri=target;track.animation=a;track.values.resize(values.count);
  for(std::uint32_t k=0;k<values.count;++k){std::uint32_t raw=word(values.data+4*k);std::memcpy(&track.values[k],&raw,4);if(k&&dh2_animation_key_time(&a,0,k)<dh2_animation_key_time(&a,0,k-1)){error="Unordered particle keys";return {};}}
  result->tracks_.push_back(std::move(track));
 }
 if(result->tracks_.empty()){error="No supported source scalar particle tracks";return {};}
 error.clear();return result;
}
const std::string* ParticleScalarAnimationResourceV6::target(std::size_t i)const{return i<tracks_.size()?&tracks_[i].uri:nullptr;}
int ParticleScalarAnimationResourceV6::sample(std::size_t i,std::int32_t ms,std::int32_t& cursor,float& out)const{
 if(i>=tracks_.size()||overlap(&cursor,4,&out,4))return -1;const auto& track=tracks_[i];if(cursor<0||std::uint32_t(cursor)>=track.values.size())return -1;
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
const char* ParticleScalarAnimationResourceV6::parameter_name(std::size_t i)const{if(i>=tracks_.size())return nullptr;switch(tracks_[i].type){case 28:return "BirthRate";case 37:return "SpinPhase";case 38:return "SpinPhaseVariation";default:return nullptr;}}
int ParticleScalarBindingV6::sample_apply(std::int32_t ms){if(!resource||!parameter.owner||!parameter.storage||track>=resource->track_count())return -1;auto* name=resource->parameter_name(track);auto live=parameter.owner->parameter(name);if(live.storage!=parameter.storage)return -1;auto key=cursor;float value;auto status=resource->sample(track,ms,key,value);if(status)return status;status=dh2_particle_parameter_apply(static_cast<float*>(parameter.storage),&value);if(!status)cursor=key;return status;}
}
