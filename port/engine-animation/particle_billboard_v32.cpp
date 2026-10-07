#include "particle_billboard_v32.hpp"
#include "../engine-math/math.hpp"
#include <cstring>
#include <algorithm>
#include <limits>
namespace dh2::animation {
namespace {
float cell(const ParticleSeed100& p,unsigned at){float f;std::memcpy(&f,p.data()+at,4);return f;}
float distance(const ParticleSeed100& p){return cell(p,0x60);}
float median(float a,float b,float c){if(a>b){if(b>c)return b;return a>c?c:a;}if(a>c)return a;return b>c?c:b;}
void partition_sort(ParticleSeed100* first,ParticleSeed100* last){
 while(last-first>16){const float pivot=median(distance(*first),distance(first[(last-first)/2]),distance(last[-1]));auto* left=first;auto* right=last;
  for(;;){while(distance(*left)>pivot)++left;--right;while(distance(*right)<pivot)--right;if(left>=right)break;std::swap(*left,*right);++left;}
  partition_sort(left,last);last=left;
 }
}
}
extern "C" int dh2_particle_billboard_apply_v32(ParticleSeed100* p,std::uint32_t n,const float* camera,const float* world,bool local,ParticleBillboardBoundsV1* bounds){
 if(n>16383||(!p&&n)||!camera||!bounds||(local&&!world))return -1;
 for(unsigned k=0;k<3;++k){bounds->minimum[k]=std::numeric_limits<float>::max();bounds->maximum[k]=-std::numeric_limits<float>::max();}
 for(unsigned i=0;i<n;++i){const float x=camera[0]-cell(p[i],0),y=camera[1]-cell(p[i],4),z=camera[2]-cell(p[i],8);const float xy=x*x+y*y,d=xy+z*z;std::memcpy(p[i].data()+0x60,&d,4);for(unsigned k=0;k<3;++k){const float v=cell(p[i],4*k);if(v>bounds->maximum[k])bounds->maximum[k]=v;if(v<bounds->minimum[k])bounds->minimum[k]=v;}}
 if(local)for(unsigned k=0;k<3;++k){bounds->minimum[k]=bounds->minimum[k]+world[12+k];bounds->maximum[k]=bounds->maximum[k]+world[12+k];}
 if(n){partition_sort(p,p+n);for(unsigned i=1;i<n;++i){auto value=p[i];unsigned j=i;while(j&&distance(value)>distance(p[j-1])){p[j]=p[j-1];--j;}p[j]=value;}}return 0;
}
}
