#include "actor_runtime.hpp"
#include <cmath>
#include <cstring>
#include <limits>

namespace dh2::actor {
namespace {
bool finite(const float* p,unsigned n){for(unsigned i=0;i<n;++i)if(!std::isfinite(p[i]))return false;return true;}
bool overlaps(const void* a,std::size_t an,const void* b,std::size_t bn){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return a&&b&&(x<=y?y-x<an:x-y<bn);}
struct Bridge {
 const RuntimeRequest& request;
 RuntimeResult& result;
 std::string& error;
 const GenericVisualBorrowV4* generic_visual{};
 physical::NativeSubobjectsBridge native{};
 subobjects::Services services{};
 bool failed=false;
 void fail(std::uint32_t event,const char* reason){if(!failed){failed=true;result.failed_event=event;error=reason;}}
 std::uint32_t external(std::uint32_t event,float* values){
  const auto& s=*request.services;
  const auto status=s.invoke(s.context,event,values);
  if(status==std::numeric_limits<std::uint32_t>::max())fail(event,"Actor virtual service failed");
  return status;
 }
 bool world(std::uint32_t event){
  if(generic_visual){
   if(!generic_visual->root)return true;
   if(!generic_visual->update_world(generic_visual->context,error)){fail(event,"Generic GameObject visual world update failed");return false;}
   return true;
  }
  if(!request.binding)return true;
  if(!request.binding->update_world(*request.scene,error)){fail(event,"Actor visual world update failed");return false;}
  return true;
 }
 std::uint32_t invoke(std::uint32_t event,float* values){
  using namespace subobjects;
  if(failed)return 0;
  auto& state=*request.state;
  switch(event){
   case physical_update:case physical_set_velocity:case physical_wake:case apply_body_transform:{
    if(!request.native_body){fail(event,"Actor physical service has no native body");return 0;}
    if(values&&!finite(values,event==apply_body_transform?3:2)){fail(event,"Actor physical service produced nonfinite values");return 0;}
    // Original transform callers ignore a false SetXForm result. Frozen/outside
    // bodies therefore remain genuine state, rather than an adapter error.
    return dh2_native_body_subobject_service(&native,event,values);
   }
   case validate_position:{
    navigation::PositionResult output{};
    const navigation::ObjectPositionRequest input{request.geometry,request.registry,&state.object,request.key,values,request.motion_policy};
    const int status=dh2_nav_validate_object_position(&output,&input);
    if(status){fail(event,status==2?"Actor floor registry capacity exhausted":"Actor floor validation request failed");return 0;}
    std::memcpy(state.subobjects.previous_position,state.object.motion.position,12);
    return output.valid;
   }
   case visual_update:external(event,values);return !failed&&world(event);
   case visual_apply_position:case visual_sync_position:{
    auto* root=generic_visual?generic_visual->root:request.binding?&request.binding->root:nullptr;
    if(!root)return 1;
    const visual::Request input{&state.subobjects,root,request.native_body?&state.body:nullptr,request.native_body?&state.transform:nullptr,&services,(request.policy->has_auxiliary?1u:0u)|2u,0};
    const int status=event==visual_apply_position?dh2_visual_apply_position(&input):dh2_visual_sync_position(&input);
    if(status){fail(event,"Actor visual position service failed");return 0;}
    return !failed;
   }
   case visual_sync_rotation:{
    if(generic_visual?!generic_visual->root:!request.binding)return 1;
    const float angles[]{state.rotation.rotation[0],state.rotation.rotation[1],state.subobjects.rotation};
    if(generic_visual?!generic_visual->set_rotation(generic_visual->context,angles,error):!request.binding->set_rotation(angles)){fail(event,"Actor visual rotation service failed");return 0;}
    return world(event);
   }
   case visual::absolute_position:return world(event);
   default:return external(event,values);
  }
 }
 static std::uint32_t callback(void* opaque,std::uint32_t event,float* values){return static_cast<Bridge*>(opaque)->invoke(event,values);}
};
}
namespace {
int update_actor_phases_v4(RuntimeResult& out,const RuntimeRequest& r,const GenericRuntimeRequestV4* generic,std::string& error){
 if(!r.state||!r.geometry||!r.registry||!r.motion_policy||!r.workspace||(!generic&&!r.resolved224)||!r.policy||!r.services||!r.services->invoke||!r.key||bool(r.binding)!=bool(r.scene)||overlaps(&out,sizeof out,r.state,sizeof(*r.state))||overlaps(&out,sizeof out,&r,sizeof r)||overlaps(&r,sizeof r,r.state,sizeof(*r.state))||overlaps(r.policy,sizeof(*r.policy),r.state,sizeof(*r.state))){error="Malformed actor request";return 1;}
 if(generic){
  const auto& v=generic->actual_virtual_policy;const auto& g=generic->visual;
  if(r.binding||r.scene||v.position_from_visual>1||v.position_from_physics>1||v.rotation_from_visual>1||v.rotation_from_physics>1||v.visual_with_rotation>1||v.update_path>1||v.validate_floor>2||!std::isfinite(generic->actual_rotation_speed)||
    (g.root&&(!g.context||!g.update_world||!g.set_rotation))||overlaps(&out,sizeof out,generic,sizeof(*generic))||overlaps(generic,sizeof(*generic),r.state,sizeof(*r.state))){error="Malformed generic GameObject virtual policy/Root borrow";return 1;}
 }
 auto& s=*r.state;const auto& policy=*r.policy;
 if(policy.path.update_path>1||policy.path.avoid_obstacles>1||policy.path.debug_skip_boundary>1||policy.path.update_physics>1||policy.validating_camera>1||policy.has_auxiliary>1||!std::isfinite(policy.virtual_speed)||s.body.flags>65535||s.rotation.reserved||s.rotation.turn_positive>1||s.object.reserved||s.path.reserved||s.path.count>s.path.capacity||(s.path.capacity&&!s.path.segments)||s.path.owned>1||s.path.owned>s.path.count||s.controller.path_requested>1||s.controller.validate_boundary>1||s.controller.heading.active>1||s.controller.heading.reserved||r.workspace->reserved0||r.workspace->reserved1||r.workspace->reserved2||!finite(s.subobjects.position,3)||!finite(s.subobjects.destination,3)||!finite(s.path.target,3)||!finite(s.rotation.rotation,3)||!finite(s.controller.heading.direction,3)||!std::isfinite(s.controller.heading.angle)||(r.target_absolute_position&&!finite(r.target_absolute_position,3))||(r.native_body&&(!r.native_body->body||r.native_body->pinned>1))||(r.binding&&(r.binding->animated_node()<0||unsigned(r.binding->animated_node())>=r.scene->graph.size()||(r.binding->root.presence&~3u)))){error="Malformed actor state or policy";return 1;}
 if(r.avoidance){
  if(r.avoidance->reserved||(r.avoidance->count&&(!r.avoidance->actors||!r.avoidance->keys))){error="Malformed actor avoidance scene";return 1;}
  if(policy.path.update_physics){bool found=false;for(unsigned i=0;i<r.avoidance->count;++i)if(r.avoidance->keys[i]==r.key){found=true;if(r.avoidance->actors[i].physical.present!=std::uint32_t(bool(r.native_body))){error="Actor body presence disagrees with avoidance scene";return 1;}}
   if(!found){error="Actor physical Stop presence is absent from avoidance scene";return 1;}
  }
 }
 move::Policy decoded{};float rotation_speed=0;
 if(generic){decoded=generic->actual_virtual_policy;rotation_speed=generic->actual_rotation_speed;}
 else if(dh2_move_policy(&decoded,&r.character_flags)||dh2_move_rotation_speed(&rotation_speed,&r.character_flags,r.resolved224)){error="Actor source policy decode failed";return 1;}
 RuntimeResult result{};error.clear();result.phase=path_phase;
 std::memcpy(s.previous_position,s.subobjects.position,12);
 std::memcpy(s.previous_rotation,s.rotation.rotation,12);s.previous_rotation[2]=s.subobjects.rotation;
 std::memcpy(s.controller.position,s.subobjects.position,12);
 // Canonical generic source1a8 is controller.destination. The subobjects
 // value is only this phase's transport projection; it is not another owner.
 if(generic)std::memcpy(s.subobjects.destination,s.controller.destination,12);
 else std::memcpy(s.controller.destination,s.subobjects.destination,12);
 s.controller.heading.angle=s.rotation.heading_angle;
 auto path_policy=policy.path;path_policy.update_path=decoded.update_path;
 navigation::AvoidanceActor self{};self.object=s.object;self.physical.present=bool(r.native_body);const std::uint64_t self_key=r.key;
 const navigation::AvoidanceScene fallback_scene{r.registry,&self,&self_key,1,0};
 const auto* avoidance=r.avoidance?r.avoidance:&fallback_scene;
 const navigation::ControllerRequest path_request{&s.controller,&s.path,&s.object,r.geometry,r.graph,avoidance,&path_policy,r.workspace,r.key};
 const int path_status=dh2_nav_update_path(&result.path,&path_request);
 if(path_status){out=result;error="Actor path update failed";return path_status;}
 std::memcpy(s.subobjects.destination,s.controller.destination,12);std::memcpy(s.subobjects.heading,s.controller.heading.direction,12);
 s.subobjects.path_count=s.path.count;std::memcpy(s.subobjects.path_target,s.path.target,12);
 std::memcpy(s.subobjects.previous_position,s.object.motion.position,12);
 if(result.path.physical_stop_requested){
  if(!r.native_body||dh2_native_body_stop(r.native_body,s.subobjects.position)){out=result;error="Actor physical Stop failed";return 3;}
  dh2_native_body_refresh_view(&s.body,r.native_body);dh2_physical_stop_finish(&s.body);result.physical_stop_applied=1;
 }
 result.phase=rotation_phase;s.rotation.rotation[2]=s.subobjects.rotation;s.rotation.heading_angle=s.controller.heading.angle;
 const bool visual_present=generic?generic->visual.root!=nullptr:bool(r.binding);
 const RotationPolicy rotation_policy{rotation_speed,r.dt_ms,std::uint32_t(visual_present),decoded.visual_with_rotation};
 if(dh2_actor_update_rotation(&s.rotation,&rotation_policy,&result.visual_rotation_requested)){out=result;error="Actor rotation update failed";return 3;}
 s.subobjects.rotation=s.rotation.rotation[2];
 Bridge bridge{r,result,error,generic?&generic->visual:nullptr};bridge.services={&bridge,Bridge::callback};bridge.native={r.native_body,&s.body,&s.transform,&bridge,Bridge::callback};
 if(result.visual_rotation_requested)bridge.invoke(subobjects::visual_sync_rotation,nullptr);
 if(bridge.failed){out=result;return 3;}
 result.phase=subobjects_phase;
 const subobjects::Policy sub_policy{decoded.position_from_visual,decoded.position_from_physics,decoded.rotation_from_visual,decoded.rotation_from_physics,decoded.visual_with_rotation,decoded.validate_floor,policy.validating_camera,std::uint32_t(visual_present),policy.has_auxiliary,policy.auxiliary_type,policy.auxiliary_mode,0,policy.virtual_speed};
 const subobjects::Request sub_request{&s.subobjects,r.native_body?&s.body:nullptr,r.native_body?&s.transform:nullptr,&sub_policy,&bridge.services};
 if(dh2_subobjects_update(&result.subobjects,&sub_request)){out=result;error="Actor subobject request failed";return 3;}
 s.rotation.rotation[2]=s.subobjects.rotation;
 if(bridge.failed){out=result;return 3;}
 result.phase=target_phase;
 if(r.target_absolute_position)std::memcpy(s.target_position,r.target_absolute_position,12);
 result.phase=completed;out=result;return 0;
}
} // Shared source phases; old Character entry keeps its original decoder.
int update_actor(RuntimeResult& out,const RuntimeRequest& r,std::string& error){return update_actor_phases_v4(out,r,nullptr,error);}
int update_gameobject_v4(RuntimeResult& out,const GenericRuntimeRequestV4& r,std::string& error){return update_actor_phases_v4(out,r.actor,&r,error);}
}
