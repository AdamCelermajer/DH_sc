#include "navigation_motion.hpp"
#include <cmath>
#include <cstring>
namespace {
using namespace dh2::navigation;
float add(float a,float b){volatile float v=a+b;return v;}
float sub(float a,float b){volatile float v=a-b;return v;}
float mul(float a,float b){volatile float v=a*b;return v;}
float divide(float a,float b){volatile float v=a/b;return v;}
float dot(const float* a,const float* b){return add(add(mul(a[0],b[0]),mul(a[1],b[1])),mul(a[2],b[2]));}
float length(const float* v){return std::sqrt(dot(v,v));}
void normalize(float* v){const float n=length(v);for(unsigned k=0;k<3;++k)v[k]=divide(v[k],n);}
bool inside(const dh2::octree::Box& b,const float* p){for(unsigned k=0;k<3;++k)if(!(b.minimum[k]<=p[k]&&p[k]<=b.maximum[k]))return false;return true;}
bool geometry_valid(const CollisionWorld* w){
 if(!w||(w->room_count&&!w->rooms)||(w->floor_count&&!w->floors))return false;
 for(unsigned i=0;i<w->floor_count;++i)if(!w->floors[i].selector)return false;
 for(unsigned i=0;i<w->room_count;++i){const auto& room=w->rooms[i];if(room.reserved||(room.count&&!room.floors))return false;for(unsigned j=0;j<room.count;++j)if(room.floors[j]>=w->floor_count)return false;}return true;
}
void clear(HeightHit& out){out.hit=0;out.room=out.floor=~0u;}
bool can_path(unsigned flags,const FloorTraits& floor){return !floor.type||(flags&floor.type)==floor.type;}
bool finite3(const float* p){return p&&std::isfinite(p[0])&&std::isfinite(p[1])&&std::isfinite(p[2]);}
bool equivalent(const float* a,const float* b){constexpr float eps=0x1.a36e2ep-14f;for(unsigned k=0;k<3;++k)if(!(std::fabs(sub(a[k],b[k]))<eps))return false;return true;}
}
extern "C" int dh2_nav_floor_height(HeightHit* out,const dh2::selector::Floor* floor,const float* p){
 if(!out||out->reserved||!floor||!p)return -1;
 clear(*out);dh2::collision::Result hit{};const int status=dh2_selector_floor(&hit,floor,p);if(status!=1)return status;
 float a[3],b[3];for(unsigned k=0;k<3;++k){a[k]=sub(hit.triangle.points[1][k],hit.triangle.points[0][k]);b[k]=sub(hit.triangle.points[2][k],hit.triangle.points[0][k]);}
 out->height=hit.point[2];out->normal[0]=add(mul(b[2],-a[1]),mul(a[2],b[1]));out->normal[1]=add(mul(b[0],-a[2]),mul(a[0],b[2]));out->normal[2]=add(mul(b[1],-a[0]),mul(a[1],b[0]));out->hit=1;return 1;
}
extern "C" int dh2_nav_room_height(HeightHit* out,const CollisionWorld* world,unsigned id,const float* p,unsigned special){
 if(!out||out->reserved||!p||special>1||!geometry_valid(world)||id>=world->room_count)return -1;
 clear(*out);const auto& room=world->rooms[id];if(!inside(room.bounds,p))return 0;
 for(unsigned i=0;i<room.count;++i){const unsigned fi=room.floors[i];const auto& floor=world->floors[fi];if(!special&&(floor.traits.type&0x03000000))continue;
  auto hit=*out;const int status=dh2_nav_floor_height(&hit,floor.selector,p);if(status<0)return -1;if(status==1){*out=hit;out->room=id;out->floor=fi;return 1;}}
 return 0;
}
extern "C" int dh2_nav_world_height(HeightHit* out,const CollisionWorld* world,const float* p,unsigned special){
 if(!out||out->reserved||!p||special>1||!geometry_valid(world))return -1;
 clear(*out);if(!inside(world->bounds,p))return 0;
 for(unsigned i=0;i<world->room_count;++i){auto hit=*out;const int status=dh2_nav_room_height(&hit,world,i,p,special);if(status<0)return -1;if(status==1){*out=hit;return 1;}}return 0;
}
extern "C" int dh2_nav_validate_position(PositionResult* out,const CollisionWorld* world,MotionObject* object,float* point,const MotionPolicy* policy){
 if(!out||!finite3(point)||!policy||policy->ignore_height_delta>1||!std::isfinite(policy->maximum_height_delta)||policy->maximum_height_delta<0||!geometry_valid(world))return 1;
 if(object&&(!finite3(object->position)||(object->room!=~0u&&object->room>=world->room_count)||(object->floor!=~0u&&object->floor>=world->floor_count)))return 1;
 *out={0,0,0,0,object?object->floor:~0u,object?object->floor:~0u};
 if(object&&equivalent(object->position,point)){out->valid=1;out->kind=1;return 0;}
 HeightHit hit{};unsigned room=object?object->room:~0u,floor=object?object->floor:~0u;bool found=false;
 if(object&&floor!=~0u){found=dh2_nav_floor_height(&hit,world->floors[floor].selector,point)==1;if(found){hit.room=room;hit.floor=floor;}}
 if(object&&!found&&room!=~0u){found=dh2_nav_room_height(&hit,world,room,point,0)==1;if(found)floor=hit.floor;}
 if(!found){found=dh2_nav_world_height(&hit,world,point,0)==1;if(found){floor=hit.floor;room=hit.room;}}
 if(!object){out->valid=found;out->kind=found?2:0;if(found)point[2]=hit.height;return 0;}
 if(found&&can_path(object->flags,world->floors[floor].traits)&&(policy->ignore_height_delta||policy->maximum_height_delta>std::fabs(sub(hit.height,point[2])))){
  out->valid=1;out->kind=2;out->parent_service=1;out->parent_change=(object->object_flags&4)&&object->floor!=floor;out->new_floor=floor;point[2]=hit.height;std::memcpy(object->position,point,12);std::memcpy(object->normal,hit.normal,12);object->room=room;object->floor=floor;
 }else{out->valid=found;out->kind=3;std::memcpy(point,object->position,12);}return 0;
}
extern "C" int dh2_nav_segment_intersect(float* out,const float* first,const float* second){
 if(!out||!first||!second)return -1;
 const float ax=first[0],ay=first[1],ux=sub(first[2],ax),uy=sub(first[3],ay),bx=second[0],by=second[1],vx=sub(bx,second[2]),vy=sub(by,second[3]);
 const float denominator=sub(mul(ux,vy),mul(uy,vx));constexpr float epsilon=0x1.0c6f7ap-20f;
 if(denominator>-epsilon&&denominator<epsilon)return 0;
 const float reciprocal=divide(1.f,denominator),qx=sub(bx,ax),qy=sub(by,ay);
 const float first_parameter=mul(sub(mul(vy,qx),mul(vx,qy)),reciprocal);if(first_parameter<0||first_parameter>1)return 0;
 const float second_parameter=mul(sub(mul(ux,qy),mul(uy,qx)),reciprocal);if(second_parameter<0||second_parameter>1)return 0;
 out[0]=sub(bx,mul(second_parameter,vx));out[1]=sub(by,mul(second_parameter,vy));return 1;
}
extern "C" int dh2_nav_validate_direction(unsigned* out,float* direction,const DirectionRequest* request){
 if(!out||!finite3(direction)||!request||request->reserved||!finite3(request->position)||!std::isfinite(request->radius)||request->radius<0||!geometry_valid(request->world))return 1;
 const auto& world=*request->world;const auto* position=request->position;WorldHit collision{};*out=0;
 if(dh2_nav_world_collision(&collision,&world,position,0)!=1||!can_path(request->flags,world.floors[collision.floor].traits))return 0;
 float forward[3];std::memcpy(forward,direction,12);normalize(forward);for(float& component:forward)component=mul(component,10);
 float query[3];for(unsigned k=0;k<3;++k)query[k]=add(position[k],forward[k]);HeightHit height{};
 if(dh2_nav_world_height(&height,&world,query,0)==1&&can_path(request->flags,world.floors[height.floor].traits)){*out=1;return 0;}
 const float ray[]{position[0],position[1],add(position[0],forward[0]),add(position[1],forward[1])};const auto& triangle=collision.collision.triangle;constexpr unsigned pairs[3][2]{{0,1},{0,2},{1,2}};float intersection[2]{};
 for(const auto& pair:pairs){const float edge[]{triangle.points[pair[0]][0],triangle.points[pair[0]][1],triangle.points[pair[1]][0],triangle.points[pair[1]][1]};if(!dh2_nav_segment_intersect(intersection,ray,edge))continue;
  float slide[]{sub(edge[0],edge[2]),sub(edge[1],edge[3]),0};forward[2]=0;const float angle=std::acos(divide(dot(slide,forward),mul(length(slide),length(forward))));
  if(!(std::fabs(angle)<0x1.921fb6p+0f))for(float& component:slide)component=-component;
  normalize(slide);for(unsigned k=0;k<3;++k)query[k]=add(mul(slide[k],10),position[k]);
  if(dh2_nav_world_height(&height,&world,query,0)!=1)return 0;
  const float magnitude=length(direction);for(unsigned k=0;k<3;++k)direction[k]=mul(magnitude,slide[k]);*out=1;return 0;
 }
 *out=1;return 0;
}
