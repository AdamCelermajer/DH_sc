#include "navigation_controller.hpp"
#include <cmath>
#include <cstring>
namespace {
using namespace dh2::navigation;
float sub(float a,float b){volatile float v=a-b;return v;}
float mul(float a,float b){volatile float v=a*b;return v;}
float add(float a,float b){volatile float v=a+b;return v;}
bool finite3(const float* p){return p&&std::isfinite(p[0])&&std::isfinite(p[1])&&std::isfinite(p[2]);}
bool path_valid(const PathObject* p){return p&&!p->reserved&&p->count<=p->capacity&&(!p->capacity||p->segments)&&p->owned<=1&&p->owned<=p->count;}
bool controller_valid(const PathController* c){return c&&c->path_requested<=1&&c->validate_boundary<=1&&c->heading.active<=1&&!c->heading.reserved&&finite3(c->position)&&finite3(c->destination)&&std::isfinite(c->heading.angle);}
bool reached(const PathController& c,const PathObject& p){const auto* end=p.count?p.target:c.destination;const float x=sub(end[0],c.position[0]),y=sub(end[1],c.position[1]);return add(mul(x,x),mul(y,y))<6400.f;}
}
extern "C" int dh2_nav_is_at_destination(const PathController* c,const PathObject* p){if(!controller_valid(c)||!path_valid(p)||!finite3(p->target))return -1;return reached(*c,*p);}
extern "C" int dh2_nav_update_path_source_v69(ControllerResult* out,const ControllerRequest* r,const ControllerSourceServicesV69* source){
 if(!out||out->reserved||!r||!controller_valid(r->controller)||!path_valid(r->path)||!r->object||r->object->reserved||!r->policy||!r->workspace)return 1;
 const auto& policy=*r->policy;auto& scratch=*r->workspace;
 if(policy.update_path>1||policy.avoid_obstacles>1||policy.debug_skip_boundary>1||policy.update_physics>1||scratch.reserved0||scratch.reserved1||scratch.reserved2||!finite3(r->path->target))return 1;
 auto state=*r->controller;auto object=*r->object;auto path=*r->path;ControllerResult result{};
 std::memcpy(path.position,state.position,12);std::memcpy(object.motion.position,state.position,12);
 // The original early gate still copies the GameObject position into PFObject.
 if(!policy.update_path){*r->path=path;*r->object=object;*out=result;return 0;}
 if(path.count>scratch.segment_capacity||(path.count&&!scratch.segments))return 2;
 if(path.count)std::memcpy(scratch.segments,path.segments,path.count*sizeof(PathSegment));
 path.segments=scratch.segments;path.capacity=scratch.segment_capacity;
 if(path.count){if(dh2_nav_move_path(&result.move,&path,r->graph))return 1;std::memcpy(state.destination,result.move.target,12);}
 result.at_destination=reached(state,path);
 if(result.at_destination){
  if(state.path_requested&&!path.count){
   if(dh2_nav_drop_path(&path))return 1;
   std::memcpy(state.destination,state.position,12);state.path_requested=0;state.heading.active=0;for(auto& component:state.heading.direction)component=0;result.stopped=1;
   // Stop checks body presence and the physics-position virtual policy. Its
   // actual physical writes are deliberately left to the pending backend.
   if(policy.update_physics&&r->scene){
    if(r->scene->reserved||(r->scene->count&&(!r->scene->keys||!r->scene->actors)))return 1;
    for(unsigned i=0;i<r->scene->count;++i)if(r->scene->keys[i]==r->key){if(r->scene->actors[i].physical.present>1)return 1;result.physical_stop_requested=r->scene->actors[i].physical.present;}
   }
  }
 }else{
  state.path_requested=1;float direction[3];for(unsigned k=0;k<3;++k)direction[k]=sub(state.destination[k],state.position[k]);
  set_heading_unchecked(state.heading,direction,1);
 }
 ObstacleRegistry registry{};bool registry_used=false;
 if(!result.at_destination&&state.heading.active&&policy.avoid_obstacles){
  if(!r->scene||r->scene->reserved||!r->scene->registry||(r->scene->count&&(!r->scene->actors||!r->scene->keys)))return 1;
  const auto& input=*r->scene;registry=*input.registry;
  if(input.count>scratch.actor_capacity||(input.count&&!scratch.actors)||registry.floor_count>scratch.floor_capacity||(scratch.floor_capacity&&!scratch.floors))return 2;
  if(registry.floor_count>registry.floor_capacity||(registry.floor_count&&!registry.floors))return 1;
  if(input.count)std::memcpy(scratch.actors,input.actors,input.count*sizeof(AvoidanceActor));
  if(registry.floor_count)std::memcpy(scratch.floors,registry.floors,registry.floor_count*4);
  registry.floors=scratch.floors;registry.floor_capacity=scratch.floor_capacity;
  unsigned index=input.count;for(unsigned i=0;i<input.count;++i)if(input.keys[i]==r->key)index=i;if(index==input.count)return 1;
  auto& actor=scratch.actors[index];actor.object=object;std::memcpy(actor.target,path.target,12);actor.has_path=bool(path.count);if(path.count)std::memcpy(actor.path_target,path.segments[0].target,12);
  AvoidanceScene scene{&registry,scratch.actors,input.keys,input.count,0,input.collision_context,input.can_collide_v108};AvoidanceRequest avoid{&scene,r->key,nullptr};
  const int status=dh2_nav_avoid_obstacles(&result.avoidance,state.heading.direction,&avoid);if(status)return status;
  set_heading_unchecked(state.heading,state.heading.direction,1);registry_used=true;
 }
 if(state.heading.active){
  object.motion.object_flags|=2;
  if(state.validate_boundary){
   std::uint32_t skip=policy.debug_skip_boundary;
   if(source&&(!source->skip_boundary||source->skip_boundary(source->context,&skip)||skip>1))return 3;
   if(!skip){
   DirectionRequest direction{r->geometry,object.motion.position,object.radius,object.motion.flags,0};
   if(dh2_nav_validate_direction(&result.direction_valid,state.heading.direction,&direction))return 1;
   result.boundary_checked=1;set_heading_unchecked(state.heading,state.heading.direction,1);
   }
  }
 }else object.motion.object_flags&=~2u;
 if(registry_used){auto& original=*r->scene->registry;if(registry.floor_count>original.floor_capacity)return 2;if(registry.floor_count)std::memcpy(original.floors,registry.floors,registry.floor_count*4);original.floor_count=registry.floor_count;}
 if(path.count)std::memcpy(r->path->segments,path.segments,path.count*sizeof(PathSegment));
 path.segments=r->path->segments;path.capacity=r->path->capacity;
 *r->path=path;*r->controller=state;*r->object=object;*out=result;return 0;
}
extern "C" int dh2_nav_update_path(ControllerResult* out,const ControllerRequest* request){return dh2_nav_update_path_source_v69(out,request,nullptr);}
