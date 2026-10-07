#include "particle_billboard_v1.hpp"
#include "../engine-math/math.hpp"
#include <cstring>
#include <algorithm>
#include <limits>
namespace dh2::animation {
namespace {
float cell(const ParticleSeed100& p,unsigned at){float f;std::memcpy(&f,p.data()+at,4);return f;}
math::Vector3f cross(const math::Vector3f& a,const math::Vector3f& b){return {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};}
float distance(const ParticleSeed100& p){return cell(p,0x60);}
float median(float a,float b,float c){if(a>b){if(b>c)return b;return a>c?c:a;}if(a>c)return a;return b>c?c:b;}
void partition_sort(ParticleSeed100* first,ParticleSeed100* last){
 while(last-first>16){const float pivot=median(distance(*first),distance(first[(last-first)/2]),distance(last[-1]));auto* left=first;auto* right=last;
  for(;;){while(distance(*left)>pivot)++left;--right;while(distance(*right)<pivot)--right;if(left>=right)break;std::swap(*left,*right);++left;}
  partition_sort(left,last);last=left;
 }
}
}
extern "C" void dh2_particle_billboard_uv_v1(float* out){if(out){const float uv[8]{0,0,0,1,1,1,1,0};std::memcpy(out,uv,32);}}
extern "C" int dh2_particle_billboard_apply_v1(ParticleSeed100* p,std::uint32_t n,const float* camera,const float* world,bool local,ParticleBillboardBoundsV1* bounds){
 if(n>30||(!p&&n)||!camera||!bounds||(local&&!world))return -1;
 for(unsigned k=0;k<3;++k){bounds->minimum[k]=std::numeric_limits<float>::max();bounds->maximum[k]=-std::numeric_limits<float>::max();}
 for(unsigned i=0;i<n;++i){const float x=camera[0]-cell(p[i],0),y=camera[1]-cell(p[i],4),z=camera[2]-cell(p[i],8);const float xy=x*x+y*y,d=xy+z*z;std::memcpy(p[i].data()+0x60,&d,4);for(unsigned k=0;k<3;++k){const float v=cell(p[i],4*k);if(v>bounds->maximum[k])bounds->maximum[k]=v;if(v<bounds->minimum[k])bounds->minimum[k]=v;}}
 if(local)for(unsigned k=0;k<3;++k){bounds->minimum[k]=bounds->minimum[k]+world[12+k];bounds->maximum[k]=bounds->maximum[k]+world[12+k];}
 if(n){partition_sort(p,p+n);for(unsigned i=1;i<n;++i){auto value=p[i];unsigned j=i;while(j&&distance(value)>distance(p[j-1])){p[j]=p[j-1];--j;}p[j]=value;}}return 0;
}
extern "C" int dh2_particle_billboard_basis_v1(ParticleBillboardBasisV1* out,const float* view){
 if(!out||!view)return -1;const math::Vector3f up{view[1],view[5],view[9]},front{-view[2],-view[6],-view[10]};auto right=cross(up,front);dh2_vec3_normalize(&right);auto vertical=up;dh2_vec3_normalize(&vertical);
 out->first[0]=right.x*0.5f;out->first[1]=right.y*0.5f;out->first[2]=right.z*0.5f;out->second[0]=vertical.x*0.5f;out->second[1]=vertical.y*0.5f;out->second[2]=vertical.z*0.5f;return 0;
}
extern "C" int dh2_particle_billboard_corners_v1(float* out,const ParticleBillboardBasisV1* basis,const ParticleSeed100* p){
 if(!out||!basis||!p)return -1;math::Vector3f first{basis->first[0],basis->first[1],basis->first[2]},second{basis->second[0],basis->second[1],basis->second[2]};const float angle=cell(*p,0x50);
 if(angle!=0){auto axis=cross(second,first);dh2_vec3_normalize(&axis);if(cell(*p,0x54)>0){axis.x=-axis.x;axis.y=-axis.y;axis.z=-axis.z;}math::Quaternion q;dh2_quat_from_angle_axis(&q,angle,&axis);math::Vector3f a,b;dh2_quat_transform_vector(&a,&q,&first);dh2_quat_transform_vector(&b,&q,&second);first=a;second=b;}
 const float a[3]{first.x,first.y,first.z},b[3]{second.x,second.y,second.z};for(unsigned k=0;k<3;++k){out[k]=b[k]-a[k];out[3+k]=-a[k]-b[k];out[6+k]=a[k]-b[k];out[9+k]=b[k]+a[k];}return 0;
}
extern "C" int dh2_particle_billboard_vertices_v1(ParticleBillboardVertexV1* out,const ParticleBillboardBasisV1* basis,const ParticleSeed100* p,const float* uv){
 if(!out||!p||!uv)return -1;float corners[12];const int rc=dh2_particle_billboard_corners_v1(corners,basis,p);if(rc)return rc;const float size=cell(*p,0x44);std::uint32_t color;std::memcpy(&color,p->data()+0x18,4);
 for(unsigned i=0;i<4;++i){for(unsigned k=0;k<3;++k)out[i].position[k]=cell(*p,4*k)+size*corners[i*3+k];out[i].color=color;std::memcpy(out[i].uv,uv+i*2,8);}return 0;
}
}
