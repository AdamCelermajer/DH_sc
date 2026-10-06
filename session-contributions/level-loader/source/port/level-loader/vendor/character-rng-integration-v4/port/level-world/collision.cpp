#include "collision.hpp"
#include <cmath>
#include <cstring>
#include <limits>
namespace {
using namespace dh2::collision;
float add(float a,float b){volatile float v=a+b;return v;}
float sub(float a,float b){volatile float v=a-b;return v;}
float mul(float a,float b){volatile float v=a*b;return v;}
float div(float a,float b){volatile float v=a/b;return v;}
void difference(float* out,const float* a,const float* b){for(unsigned i=0;i<3;++i)out[i]=sub(a[i],b[i]);}
float dot(const float* a,const float* b){return add(add(mul(a[0],b[0]),mul(a[1],b[1])),mul(a[2],b[2]));}
float distance(const float* a,const float* b){float d[3];difference(d,a,b);return dot(d,d);}
void reverse_cross(float* out,const float* a,const float* b){
 out[0]=add(mul(b[2],-a[1]),mul(a[2],b[1]));out[1]=add(mul(b[0],-a[2]),mul(a[0],b[2]));out[2]=add(mul(b[1],-a[0]),mul(a[1],b[0]));
}
void normalize(float* p){const float n=dot(p,p);if(n==0)return;const float reciprocal=div(1,std::sqrt(n));for(unsigned i=0;i<3;++i)p[i]=mul(p[i],reciprocal);}
bool same_side(const float* p,const float* reference,const float* a,const float* b){
 float edge[3],pdelta[3],rdelta[3],pcross[3],rcross[3];difference(edge,b,a);difference(pdelta,p,a);difference(rdelta,reference,a);
 reverse_cross(pcross,edge,pdelta);reverse_cross(rcross,edge,rdelta);return dot(pcross,rcross)>=0;
}
bool inside(const Triangle& t,const float* p){
 return same_side(p,t.points[0],t.points[1],t.points[2])&&same_side(p,t.points[1],t.points[0],t.points[2])&&same_side(p,t.points[2],t.points[0],t.points[1]);
}
bool line(float* out,const Triangle& t,const float* start,const float* direction){
 float a[3],b[3],normal[3];difference(a,t.points[1],t.points[0]);difference(b,t.points[2],t.points[0]);reverse_cross(normal,a,b);normalize(normal);
 const float denominator=dot(normal,direction);
 if(std::fabs(denominator)<=0x1.0c6f7ap-20f)return false; // 0x358637bd
 const float factor=div(-sub(dot(normal,start),dot(normal,t.points[0])),denominator);
 for(unsigned i=0;i<3;++i)out[i]=add(start[i],mul(factor,direction[i]));
 return inside(t,out);
}
}
extern "C" int dh2_collision_line(float* out,const dh2::collision::Triangle* t,const float* start,const float* direction){
 if(!out||!t||!start||!direction)return -1;
 return line(out,*t,start,direction);
}
extern "C" int dh2_collision_raycast(dh2::collision::Result* out,const dh2::collision::Triangle* triangles,std::uint32_t count,const dh2::collision::Ray* ray){
 if(!out||!ray||(!triangles&&count)||count>100000)return -1;
 out->hit=0;out->index=std::numeric_limits<std::uint32_t>::max();
 float minimum[3],maximum[3],direction[3];
 for(unsigned i=0;i<3;++i){minimum[i]=ray->end[i]<ray->start[i]?ray->end[i]:ray->start[i];maximum[i]=ray->start[i]<ray->end[i]?ray->end[i]:ray->start[i];}
 difference(direction,ray->end,ray->start);normalize(direction);const float ray_length=distance(ray->start,ray->end);float nearest=std::numeric_limits<float>::max();
 for(unsigned i=0;i<count;++i){
  const auto& t=triangles[i];bool rejected=false;
  for(unsigned axis=0;axis<3;++axis)if((t.points[0][axis]<minimum[axis]&&t.points[1][axis]<minimum[axis]&&t.points[2][axis]<minimum[axis])||
     (maximum[axis]<t.points[0][axis]&&t.points[1][axis]>maximum[axis]&&t.points[2][axis]>maximum[axis])){rejected=true;break;}
  if(rejected)continue;
  if(nearest<=distance(ray->start,t.points[0])&&nearest<=distance(ray->start,t.points[1])&&nearest<=distance(ray->start,t.points[2]))continue;
  float point[3]{};if(!line(point,t,ray->start,direction))continue;
  const float from_start=distance(point,ray->start);if(!(ray_length>from_start&&ray_length>distance(point,ray->end)&&nearest>from_start))continue;
  nearest=from_start;out->hit=1;out->index=i;std::memcpy(out->point,point,12);out->triangle=t;
 }
 return out->hit;
}
extern "C" int dh2_collision_floor(dh2::collision::Result* out,const dh2::collision::Floor* floor,const float* point){
 if(!out||!floor||!point||floor->reserved||(!floor->triangles&&floor->count)||floor->count>100000)return -1;
 out->hit=0;out->index=std::numeric_limits<std::uint32_t>::max();
 for(unsigned i=0;i<3;++i)if(!(floor->minimum[i]<=point[i]&&point[i]<=floor->maximum[i]))return 0;
 Ray ray;std::memcpy(ray.start,point,12);std::memcpy(ray.end,point,12);ray.start[2]=add(point[2],1000);ray.end[2]=sub(point[2],1000);
 return dh2_collision_raycast(out,floor->triangles,floor->count,&ray);
}
extern "C" std::uint32_t dh2_collision_floor_query(void* user,std::uint32_t floor,const float* point){
 const auto* set=static_cast<const dh2::collision::FloorSet*>(user);
 if(!set||!set->floors||set->reserved||floor>=set->count)return 0;
 dh2::collision::Result result{};
 return dh2_collision_floor(&result,set->floors+floor,point)>0;
}
