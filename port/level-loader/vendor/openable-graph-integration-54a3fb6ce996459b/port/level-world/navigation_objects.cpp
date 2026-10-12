#include "navigation_objects.hpp"
#include <cmath>
#include <cstring>
namespace {
using namespace dh2::navigation;
bool finite3(const float* p){return std::isfinite(p[0])&&std::isfinite(p[1])&&std::isfinite(p[2]);}
bool geometry_valid(const CollisionWorld* w){
 if(!w||(w->room_count&&!w->rooms)||(w->floor_count&&!w->floors))return false;
 for(unsigned i=0;i<w->floor_count;++i)if(!w->floors[i].selector)return false;
 for(unsigned i=0;i<w->room_count;++i){const auto& room=w->rooms[i];if(room.reserved||(room.count&&!room.floors))return false;for(unsigned j=0;j<room.count;++j)if(room.floors[j]>=w->floor_count)return false;}return true;
}
bool object_valid(const NavigationObject* o){return o&&!o->reserved&&finite3(o->motion.position)&&std::isfinite(o->radius)&&o->radius>=0&&std::isfinite(o->obstacle_weight)&&std::isfinite(o->obstacle_extent);}
bool registry_valid(const ObstacleRegistry* r){
 if(!r||r->count>r->capacity||r->floor_count>r->floor_capacity||(r->capacity&&!r->entries)||(r->floor_capacity&&!r->floors))return false;
 for(unsigned i=0;i<r->floor_count;++i)for(unsigned j=0;j<i;++j)if(r->floors[i]==r->floors[j])return false;
 for(unsigned i=0;i<r->count;++i){if(r->entries[i].reserved||!r->entries[i].object)return false;bool found=false;for(unsigned j=0;j<r->floor_count;++j)found|=r->entries[i].floor==r->floors[j];if(!found)return false;}return true;
}
bool has_floor(const ObstacleRegistry& r,unsigned floor){for(unsigned i=0;i<r.floor_count;++i)if(r.floors[i]==floor)return true;return false;}
unsigned first_entry(const ObstacleRegistry& r,unsigned floor,std::uint64_t key){for(unsigned i=0;i<r.count;++i)if(r.entries[i].floor==floor&&r.entries[i].object==key)return i;return r.count;}
void touch_floor(ObstacleRegistry& r,unsigned floor){if(!has_floor(r,floor))r.floors[r.floor_count++]=floor;}
void erase(ObstacleRegistry& r,unsigned i){for(unsigned j=i+1;j<r.count;++j)r.entries[j-1]=r.entries[j];--r.count;}
void append(ObstacleRegistry& r,unsigned floor,std::uint64_t key){touch_floor(r,floor);r.entries[r.count++]={floor,0,key};}
int height(NavigationObject& o,const CollisionWorld* w){
 HeightHit hit{};hit.height=o.motion.position[2];std::memcpy(hit.normal,o.motion.normal,12);
 const int found=dh2_nav_world_height(&hit,w,o.motion.position,0);if(found<0)return 1;
 if(found){o.motion.room=hit.room;o.motion.floor=hit.floor;if(!(o.motion.object_flags&1))o.motion.position[2]=hit.height;std::memcpy(o.motion.normal,hit.normal,12);}return 0;
}
}
extern "C" int dh2_nav_object_defaults(NavigationObject* o){if(!o)return 1;*o={{2,8,~0u,~0u,{0,0,0},{0,0,1}},0,1,1,0,0};return 0;}
extern "C" int dh2_nav_motion_policy_defaults(MotionPolicy* p){if(!p)return 1;*p={100,0};return 0;}
extern "C" int dh2_nav_object_set_flying(NavigationObject* o,unsigned value){if(!o||o->reserved||value>1)return 1;o->motion.flags=(o->motion.flags&~1u)|(value?1u:0u);return 0;}
extern "C" int dh2_nav_object_set_swimming(NavigationObject* o,unsigned value){if(!o||o->reserved||value>1)return 1;o->motion.flags=(o->motion.flags&~2u)|(value?2u:0u);return 0;}
extern "C" int dh2_nav_object_is_flying(const NavigationObject* o){return !o||o->reserved?-1:int(o->motion.flags&1);}
extern "C" int dh2_nav_object_is_swimming(const NavigationObject* o){return !o||o->reserved?-1:int((o->motion.flags>>1)&1);}
extern "C" int dh2_nav_init_object(const ObjectInitRequest* r){
 if(!r||r->reserved||r->flying>1||!r->object||r->object->reserved||!geometry_valid(r->geometry)||!finite3(r->position)||!std::isfinite(r->radius))return 1;
 auto next=*r->object;next.user=r->user;next.radius=r->radius<1?1:r->radius;next.motion.object_flags=(next.motion.object_flags&~1u)|(r->flying?1u:0u);std::memcpy(next.motion.position,r->position,12);
 if(height(next,r->geometry))return 1;
 *r->object=next;return 0;
}
extern "C" int dh2_nav_init_obstacle(const ObstacleInitRequest* r){
 if(!r||r->reserved||r->enabled>1||!r->key||!object_valid(r->object)||!geometry_valid(r->geometry)||!registry_valid(r->registry)||!std::isfinite(r->weight)||r->weight<0||!std::isfinite(r->extent))return 1;
 auto& registry=*r->registry;auto next=*r->object;
 if(next.motion.floor!=~0u&&next.motion.floor>=r->geometry->floor_count)return 1;
 if(next.motion.room!=~0u&&next.motion.room>=r->geometry->room_count)return 1;
 if(!r->enabled||r->extent==0){
  if(next.motion.object_flags&4){const unsigned floor=next.motion.floor;if(!has_floor(registry,floor)&&registry.floor_count==registry.floor_capacity)return 2;touch_floor(registry,floor);const unsigned i=first_entry(registry,floor,r->key);if(i<registry.count)erase(registry,i);}
  next.motion.object_flags&=~4u;next.obstacle_weight=next.obstacle_extent=0;*r->object=next;return 0;
 }
 if(!(next.motion.object_flags&4)){
  if(next.motion.floor==~0u){if(height(next,r->geometry))return 1;if(next.motion.floor==~0u){*r->object=next;return 0;}}
  if(registry.count==registry.capacity||(!has_floor(registry,next.motion.floor)&&registry.floor_count==registry.floor_capacity))return 2;
  append(registry,next.motion.floor,r->key);
 }
 next.motion.object_flags|=4;next.obstacle_weight=r->weight;next.obstacle_extent=r->extent;*r->object=next;return 0;
}
extern "C" int dh2_nav_change_obstacle_parent(ObstacleRegistry* r,const NavigationObject* o,std::uint64_t key,unsigned target){
 if(!registry_valid(r)||!object_valid(o)||!key)return 1;
 const unsigned old=o->motion.floor;if(!(o->motion.object_flags&4)||old==target)return 0;
 const unsigned i=first_entry(*r,old,key);const unsigned required=unsigned(!has_floor(*r,old))+unsigned(i<r->count&&!has_floor(*r,target));
 if(required>r->floor_capacity-r->floor_count)return 2;
 touch_floor(*r,old);if(i==r->count)return 0;
 erase(*r,i);append(*r,target,key);return 0;
}
extern "C" int dh2_nav_validate_object_position(PositionResult* out,const ObjectPositionRequest* r){
 if(!out||!r||!r->key||!object_valid(r->object)||!registry_valid(r->registry)||!r->position)return 1;
 auto next=*r->object;float point[3];std::memcpy(point,r->position,12);PositionResult result{};
 const int status=dh2_nav_validate_position(&result,r->geometry,&next.motion,point,r->policy);if(status)return status;
 if(result.parent_service){const int parent=dh2_nav_change_obstacle_parent(r->registry,r->object,r->key,next.motion.floor);if(parent)return parent;}
 *r->object=next;std::memcpy(r->position,point,12);*out=result;return 0;
}
