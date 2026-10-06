#pragma once
#include <array>
#include <algorithm>
#include <cmath>
#include <limits>
namespace dh2::render {
struct BoundsV35 {
 std::array<float,3> low{},high{};bool valid{},poisoned{};
 void include(const float* p){
  if(poisoned)return;
  for(unsigned i=0;i<3;++i)if(!std::isfinite(p[i])){valid=false;poisoned=true;return;}
  if(!valid){for(unsigned i=0;i<3;++i)low[i]=high[i]=p[i];valid=true;return;}
  for(unsigned i=0;i<3;++i){low[i]=std::min(low[i],p[i]);high[i]=std::max(high[i],p[i]);}
 }
};
// Reject only when all eight bounds corners are strictly beyond one clip
// plane. Handles perspective, mirrored placement and near-plane crossings.
// No actor update, source visibility field or targeting bounds is changed.
inline bool outside_clip_v35(const BoundsV35& b,const std::array<float,16>& m){
 if(!b.valid)return false;
 unsigned shared=63;
 for(unsigned corner=0;corner<8;++corner){
  const double x=(corner&1)?b.high[0]:b.low[0],y=(corner&2)?b.high[1]:b.low[1],z=(corner&4)?b.high[2]:b.low[2];
  double p[4];for(unsigned r=0;r<4;++r){p[r]=m[r]*x+m[4+r]*y+m[8+r]*z+m[12+r];if(!std::isfinite(p[r]))return false;}
  const double epsilon=1e-5*(1.+std::abs(p[3])+std::abs(p[0])+std::abs(p[1])+std::abs(p[2]));
  unsigned mask=0;for(unsigned a=0;a<3;++a){if(p[a]<-p[3]-epsilon)mask|=1u<<(2*a);if(p[a]>p[3]+epsilon)mask|=2u<<(2*a);}
  shared&=mask;if(!shared)return false;
 }
 return shared!=0;
}
}
