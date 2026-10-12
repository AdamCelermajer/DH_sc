#include "navigation_avoidance.hpp"
#include <cmath>
#include <cstring>
namespace {
using namespace dh2::navigation;
float add(float a,float b){volatile float v=a+b;return v;}
float sub(float a,float b){volatile float v=a-b;return v;}
float mul(float a,float b){volatile float v=a*b;return v;}
float div(float a,float b){volatile float v=a/b;return v;}
float dot(const float* a,const float* b){return add(add(mul(a[0],b[0]),mul(a[1],b[1])),mul(a[2],b[2]));}
float length(const float* p){return std::sqrt(dot(p,p));}
float distance_xy(const float* a,const float* b){const float x=sub(a[0],b[0]),y=sub(a[1],b[1]);return add(mul(x,x),mul(y,y));}
void normalize(float* p){const float n=length(p);for(unsigned k=0;k<3;++k)p[k]=div(p[k],n);}
bool finite3(const float* p){return std::isfinite(p[0])&&std::isfinite(p[1])&&std::isfinite(p[2]);}
bool contact_valid(const PhysicalContact* p){return p&&p->present<=1&&p->disabled<=1&&p->owner_present<=1&&p->owner_enabled<=1&&p->primary.present<=1&&p->secondary.present<=1;}
const ContactFilter* filter(const PhysicalContact& p){return p.primary.present?&p.primary:p.secondary.present?&p.secondary:nullptr;}
int can_collide(const PhysicalContact& a,const PhysicalContact& b){
 if(!a.present||!b.present||a.disabled||b.disabled)return 0;
 const auto* af=filter(a);const auto* bf=filter(b);if(!af||!bf||(a.owner_present&&!a.owner_enabled)||(b.owner_present&&!b.owner_enabled))return 0;
 if(af->group&&af->group==bf->group)return af->group>0;
 return (bf->category&af->mask)&&(bf->mask&af->category);
}
const AvoidanceActor* find(const AvoidanceScene& s,std::uint64_t key){for(unsigned i=0;i<s.count;++i)if(s.keys[i]==key)return &s.actors[i];return nullptr;}
bool has_floor(const ObstacleRegistry& r,unsigned floor){for(unsigned i=0;i<r.floor_count;++i)if(r.floors[i]==floor)return true;return false;}
bool valid(const AvoidanceRequest* request){
 if(!request||!request->scene||!request->object)return false;
 const auto& s=*request->scene;const auto* r=s.registry;
 if(s.reserved||(s.count&&(!s.actors||!s.keys))||!find(s,request->object)||!r||r->count>r->capacity||r->floor_count>r->floor_capacity||(r->capacity&&!r->entries)||(r->floor_capacity&&!r->floors))return false;
 for(unsigned i=0;i<s.count;++i){const auto& a=s.actors[i];const auto& o=a.object;if(!s.keys[i]||o.reserved||a.reserved||a.has_path>1||!finite3(o.motion.position)||!finite3(a.target)||(a.has_path&&!finite3(a.path_target))||!std::isfinite(o.radius)||o.radius<0||!std::isfinite(o.obstacle_weight)||!std::isfinite(o.obstacle_extent)||!contact_valid(&a.physical))return false;for(unsigned j=0;j<i;++j)if(s.keys[i]==s.keys[j])return false;}
 for(unsigned i=0;i<r->floor_count;++i)for(unsigned j=0;j<i;++j)if(r->floors[i]==r->floors[j])return false;
 for(unsigned i=0;i<r->count;++i)if(r->entries[i].reserved||!find(s,r->entries[i].object)||!has_floor(*r,r->entries[i].floor))return false;
 const auto* b=request->records;if(b&&(b->count>b->capacity||(b->capacity&&!b->entries)))return false;return true;
}
bool contribution(const AvoidanceActor& a,const AvoidanceActor& b,std::uint64_t key,ObstacleForce& out){
 if(!(b.object.motion.object_flags&4)||!(b.object.motion.object_flags&8))return false;
 if(a.object.user&&b.object.user&&a.physical.present&&b.physical.present&&!can_collide(a.physical,b.physical))return false;
 const auto* p=a.object.motion.position;const auto* q=b.object.motion.position;const float d=distance_xy(p,q);
 if(a.has_path&&d>=distance_xy(a.path_target,p))return false;
 if(d>=distance_xy(a.target,p))return false;
 const float reach=add(add(a.object.radius,b.object.radius),b.object.obstacle_weight);const float radius_squared=mul(reach,reach);if(!(radius_squared>d))return false;
 out.coefficient=sub(1.f,div(d,radius_squared));out.object=key;out.direction[0]=sub(p[0],q[0]);out.direction[1]=sub(p[1],q[1]);out.direction[2]=0;normalize(out.direction);
 const float scale=mul(out.coefficient,b.object.obstacle_extent);for(unsigned k=0;k<3;++k)out.direction[k]=mul(scale,out.direction[k]);return true;
}
int force(ForceResult& out,const AvoidanceRequest& request){
 const auto& s=*request.scene;auto& registry=*s.registry;const auto& actor=*find(s,request.object);const unsigned floor=actor.object.motion.floor;unsigned needed=0;ObstacleForce record{};
 // Preflight avoids partial output/state writes when modern bounded storage
 // cannot represent the original vector/map mutation.
 for(unsigned i=0;i<registry.count;++i){const auto& entry=registry.entries[i];if(entry.floor==floor&&entry.object!=request.object&&contribution(actor,*find(s,entry.object),entry.object,record))++needed;}
 if((!has_floor(registry,floor)&&registry.floor_count==registry.floor_capacity)||(request.records&&needed>request.records->capacity-request.records->count))return 2;
 if(!has_floor(registry,floor))registry.floors[registry.floor_count++]=floor;
 out={{0,0,0},0};
 for(unsigned i=0;i<registry.count;++i){const auto& entry=registry.entries[i];if(entry.floor!=floor||entry.object==request.object||!contribution(actor,*find(s,entry.object),entry.object,record))continue;
  if(request.records)request.records->entries[request.records->count++]=record;
  for(unsigned k=0;k<3;++k)out.direction[k]=add(out.direction[k],record.direction[k]);
  ++out.count;
 }return 0;
}
}
extern "C" int dh2_nav_can_collide(const PhysicalContact* a,const PhysicalContact* b){return !contact_valid(a)||!contact_valid(b)?-1:can_collide(*a,*b);}
extern "C" int dh2_nav_obstacle_force(ForceResult* out,const AvoidanceRequest* request){if(!out||!valid(request))return 1;ForceResult result{};const int status=force(result,*request);if(status)return status;*out=result;return 0;}
extern "C" int dh2_nav_avoid_obstacles(AvoidanceResult* out,float* direction,const AvoidanceRequest* request){
 if(!out||!direction||!finite3(direction)||!valid(request))return 1;
 const auto& actor=*find(*request->scene,request->object);AvoidanceResult result{};
 if(actor.object.motion.floor==~0u||(actor.object.motion.object_flags&1)||!(actor.object.motion.object_flags&2)){*out=result;return 0;}
 const int status=force(result.force,*request);if(status)return status;result.evaluated=1;
 if(!result.force.count||!(dot(direction,result.force.direction)<0)){*out=result;return 0;}
 result.adjusted=1;const float up[3]{0,0,1};float tangent[]{sub(mul(direction[1],up[2]),mul(direction[2],up[1])),sub(mul(direction[2],up[0]),mul(direction[0],up[2])),sub(mul(direction[0],up[1]),mul(direction[1],up[0]))};normalize(tangent);
 const float sign=dot(tangent,result.force.direction),force_length=length(result.force.direction),direction_length=length(direction);
 if(!(std::fabs(sign)<0x1.a36e2ep-14f)){const float multiplier=div(sign,std::fabs(sign));for(float& value:tangent)value=mul(value,multiplier);}
 float candidate[3];for(unsigned k=0;k<3;++k)candidate[k]=add(mul(force_length,tangent[k]),direction[k]);
 if(result.force.count>1){const float angle=std::acos(div(dot(candidate,direction),mul(length(candidate),length(direction))));if(angle>=0x1.657184p-4f){result.turn_limited=1;const float scale=mul(direction_length,0x1.665a82p-4f);for(unsigned k=0;k<3;++k)candidate[k]=add(direction[k],mul(scale,tangent[k]));}}
 std::memcpy(direction,candidate,12);*out=result;return 0;
}
