#include "particle_parameter.hpp"
#include <cstddef>
#include <cstring>
#include <limits>
namespace {
bool overlap(const void* a,std::size_t n,const void* b,std::size_t m){
 const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
 return n&&m&&((x<=y&&y-x<n)||(y<x&&x-y<m));
}
bool valid(float* out,const dh2::animation::ParticleAccessor16* a){
 return out&&a&&!a->reserved&&a->values&&a->count&&
  !overlap(out,4,a,sizeof(*a))&&!overlap(out,4,a->values,std::size_t(a->count)*4);
}
float load(const float* p){float v;std::memcpy(&v,p,4);return v;}
void store(float* p,float v){std::memcpy(p,&v,4);}
}
extern "C" int dh2_particle_parameter_key(float* out,const dh2::animation::ParticleAccessor16* a,std::uint32_t key){
 if(!valid(out,a)||key>=a->count)return -1;std::memcpy(out,a->values+key,4);return 0;
}
extern "C" int dh2_particle_parameter_between(float* out,const dh2::animation::ParticleAccessor16* a,std::uint32_t key,std::uint32_t next,float fraction){
 if(!valid(out,a)||key>=a->count||next>=a->count)return -1;
 const float first=load(a->values+key),delta=load(a->values+next)-first,scaled=fraction*delta;
 store(out,first+scaled);return 0;
}
extern "C" int dh2_particle_parameter_delta_key(float* out,const dh2::animation::ParticleAccessor16* a,std::uint32_t key,std::uint32_t next){
 if(!valid(out,a)||key>=a->count||next>=a->count)return -1;store(out,load(a->values+next)-load(a->values+key));return 0;
}
extern "C" int dh2_particle_parameter_delta_between(float* out,const dh2::animation::ParticleAccessor16* a,std::uint32_t reference,std::uint32_t key,std::uint32_t next,float fraction){
 if(!valid(out,a)||reference>=a->count||key>=a->count||next>=a->count)return -1;
 const float first=load(a->values+key),delta=load(a->values+next)-first,scaled=fraction*delta,interpolated=first+scaled;
 store(out,interpolated-load(a->values+reference));return 0;
}
extern "C" int dh2_particle_parameter_blend(float* out,const float* values,const float* weights,std::int32_t count){
 if(!out)return -1;
 if(count>0&&(!values||!weights||overlap(out,4,values,std::size_t(count)*4)||overlap(out,4,weights,std::size_t(count)*4)))return -1;
 float sum=0.0f;
 for(std::int32_t i=0;i<count;++i){const float product=load(values+i)*load(weights+i);sum=sum+product;}
 store(out,sum);return 0;
}
extern "C" int dh2_particle_parameter_apply(float* target,const float* value){
 if(!target||!value||overlap(target,4,value,4))return -1;std::memcpy(target,value,4);return 0;
}
