#pragma once
#include "particle_cloud_models_v1.hpp"
#include <cmath>
#include <cstring>
#include <functional>
#include <limits>

namespace dh2::animation {
// PColorModel 64bf00/64e830: per-particle animation state, sharing PSRandom.
// Track callbacks preserve the source accessor's color/texture interpretation.
struct ParticleColorTracksV1 {
 std::uint32_t mapping{};
 float offset{},offset_variation{},length{},length_variation{},scale{},scale_variation{};
 std::function<int(ParticleSeed100&,std::int32_t)> sample;
 int initialize(ParticleSeed100* p,std::uint32_t count,std::int32_t* seed)const {
  if(!sample)return 0;
  if((count&&!p)||!seed)return -1;
  const float ov=offset*offset_variation,lv=length*length_variation;
  for(unsigned i=0;i<count;++i){
   const float o=offset+(ov==0?0:ov*float(dh2_particle_random_v1(seed))+ov*-.5f);
   const float l=length+(lv==0?0:lv*float(dh2_particle_random_v1(seed))+lv*-.5f);
   std::memcpy(p[i].data()+52,&o,4);std::memcpy(p[i].data()+56,&l,4);
  }return 0;
 }
 int apply(ParticleSeed100* p,std::uint32_t count,const std::uint32_t* fallback)const {
  if(!sample)return dh2_particle_color_fallback_v1(p,count,fallback);
  if(count&&!p)return -1;
  for(unsigned i=0;i<count;++i){
   float o,l,age,life;std::memcpy(&o,p[i].data()+52,4);std::memcpy(&l,p[i].data()+56,4);
   std::memcpy(&age,p[i].data()+60,4);std::memcpy(&life,p[i].data()+64,4);
   const float t=(mapping==1?(age*l/life)*scale+o:age+o)*1000.f;
   if(!std::isfinite(t)||t<float(std::numeric_limits<std::int32_t>::min())||t>=2147483648.f)return -1;
   const int rc=sample(p[i],std::int32_t(t));if(rc)return rc;
  }return 0;
 }
};
}
