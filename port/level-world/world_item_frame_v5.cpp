#include "world_item_frame_v5.hpp"
#include "gameobject_scene_root_registry_v1.hpp"
#include <cstring>
#include <limits>
namespace dh2::character {
bool WorldItemFrameV5::visual_world(void* raw,std::string& e){auto v=static_cast<WorldItemFrameV5*>(raw)->graph_.visual().visual();if(!v){e="Required SAME Item visual root";return false;}return v->binding().update_world(v->scene(),e);}
bool WorldItemFrameV5::visual_rotation(void* raw,const float* xyz,std::string& e){auto v=static_cast<WorldItemFrameV5*>(raw)->graph_.visual().visual();if(!v){e="Required SAME Item visual rotation receiver";return false;}return v->binding().set_rotation(xyz);}
std::uint32_t WorldItemFrameV5::service(void* raw,std::uint32_t event,float* payload){
 auto& self=*static_cast<WorldItemFrameV5*>(raw);
 if(event==subobjects::visual_update){
  if(!self.services_.roots){self.error_="Required SAME SceneManager visibility owner";return ~0u;}
  return self.graph_.visual().update_v3(self.absolute_ms_,[&self](std::string&){self.services_.roots->notify_visibility_changed_v3();return true;},self.error_)?1u:~0u;
 }
 if(!self.services_.actual_world.invoke){self.error_="Required actual generic Item camera/auxiliary/source event "+std::to_string(event);return ~0u;}
 const auto value=self.services_.actual_world.invoke(self.services_.actual_world.context,event,payload);
 if(value==~0u&&self.error_.empty())self.error_="Actual Item world receiver rejected event "+std::to_string(event);
 return value;
}
bool WorldItemFrameV5::stop(std::string& e){
 auto& item=graph_.receiver_v4();auto& state=item.runtime();
 if(dh2_nav_drop_path(&state.path)!=0){e="Required SAME Item PF DropPath";return false;}
 std::memcpy(state.controller.destination,state.subobjects.position,12);
 state.controller.heading.active=0;state.controller.path_requested=0;
 for(auto& component:state.controller.heading.direction)component=0;
 auto* physical=item.base().pointer(0x2dc);if(!physical){e="Required actual Item physical pointer field";return false;}
 if(!*physical)return true;
 if(*physical!=reinterpret_cast<std::uintptr_t>(&graph_.physical())){e="Item Stop cannot substitute another physical receiver";return false;}
 // Genuine inherited IsUpdatingPositionFromPhysics340064 returns1.
 if(dh2_native_body_stop(&graph_.physical().native(),state.subobjects.position)!=0){e="Actual Item NativeBody Stop rejected";return false;}return true;
}
bool WorldItemFrameV5::route(const WorldItemRequestV1& q,std::int32_t& value,bool& handled,std::string& e){
 handled=false;
 if(q.operation!=WorldItemOperationV1::is_at_destination&&q.operation!=WorldItemOperationV1::stop&&q.operation!=WorldItemOperationV1::game_update)return true;
 handled=true;auto& item=graph_.receiver_v4();if(q.object!=item.base().identity()){e="Required SAME Item frame identity";return false;}
 if(q.operation==WorldItemOperationV1::is_at_destination){value=dh2_nav_is_at_destination(&item.runtime().controller,&item.runtime().path);if(value<0){e="Malformed actual Item IsAtDestination";return false;}return true;}
 if(q.operation==WorldItemOperationV1::stop)return stop(e);
 if(!services_.begin_update||!services_.begin_update(item,e)){if(e.empty())e="Required GameObject Update profiler/Debug/Application count prefix";return false;}
 auto* collision_target=item.base().pointer(0x2e4);if(!collision_target){e="Required SAME Item collision target2e4";return false;}
 if(*collision_target){if(!services_.collision_interact||!services_.collision_interact(*collision_target,e)){if(e.empty())e="Required source Item Update collision Interact";return false;}*collision_target=0;}
 error_.clear();if(!services_.actual_runtime_policy){e="Required actual generic Item camera/auxiliary RuntimePolicy facts";return false;}
 actor::RuntimePolicy policy=*services_.actual_runtime_policy;actor::GenericRuntimeRequestV4 request{};
 if(!item_frame_virtual_policy_v4(item,request,policy,e))return false;
 auto* physical=item.base().pointer(0x2dc);if(!physical){e="Required Item physical assignment authority";return false;}
 if(*physical&&*physical!=reinterpret_cast<std::uintptr_t>(&graph_.physical())){e="Required SAME Item generic physical body";return false;}
 subobjects::Services callbacks{this,service};auto& actor=request.actor;
 actor.state=&item.runtime();actor.native_body=*physical?&graph_.physical().native():nullptr;
 actor.geometry=services_.geometry;actor.graph=services_.paths;actor.registry=services_.obstacles;
 actor.motion_policy=services_.motion;actor.workspace=services_.workspace;actor.avoidance=services_.avoidance;
 actor.services=&callbacks;actor.key=item.base().identity();actor.dt_ms=static_cast<std::uint32_t>(q.integer);
 auto* node=item.base().pointer(0x180);if(!node){e="Required SAME Item target node180 field";return false;}
 if(*node&&(!services_.target_node_position||!services_.target_node_position(services_.target_context,*node,actor.target_absolute_position,e)||!actor.target_absolute_position)){if(e.empty())e="Required actual Item target node position backing";return false;}
 if(auto v=graph_.visual().visual())request.visual={this,&v->binding().root,visual_world,visual_rotation};
 actor::RuntimeResult result{};const auto status=actor::update_gameobject_v4(result,request,e);
 if(status){if(!error_.empty())e=error_;else if(e.empty())e="Actual Item generic frame failed at phase "+std::to_string(result.phase);return false;}
 if(!world::gameobject_require_online_update_v5(item.base(),services_.online,e))return false;
 auto* sound=item.base().integer(0x370);if(!sound){e="Required SAME GameObject idle sound370 field";return false;}
 if(static_cast<std::int16_t>(*sound)>=0&&(!services_.idle_sound||!services_.idle_sound(item,static_cast<std::int16_t>(*sound),e))){if(e.empty())e="Required actual GameObject PlayIdleSound38ae2c";return false;}
 if(!services_.end_update||!services_.end_update(item,e)){if(e.empty())e="Required GameObject Update profiler end";return false;}return true;
}
}
