#include "npc_actor_frame_v1.hpp"
#include <cstring>
#include <limits>
namespace dh2::character {
namespace {
struct Frame {
 NpcActorFrameBorrowV1 b;NpcActorFrameServicesV1 s;std::string& error;
 static std::uint32_t invoke(void* p,std::uint32_t event,float* values){
  auto& t=*static_cast<Frame*>(p);
  switch(event){
  // Both whole original base virtual bodies are bx lr (470cf0/470ccc).
  case subobjects::visual_update:
   if(t.b.visual_update_source==0x470cf0)return 1;break;
  case subobjects::visual_apply_rotation:
   if(t.b.visual_apply_rotation_source==0x470ccc)return 1;break;
  case subobjects::get_speed:if(values)*values=actor::base_virtual_speed;return 1;
  case subobjects::visual_sync_scaling:{
   float scale[3];if(!t.s.scale120||!t.s.scale120(t.s.context,scale,t.error))return ~0u;
   auto& root=t.b.visual->root;
   const bool changed=root.scale[0]!=scale[0]||root.scale[1]!=scale[1]||root.scale[2]!=scale[2];
   if(!changed)return 1;
   std::memcpy(root.scale,scale,12);
   if(!t.s.changed_bounds||!t.s.changed_bounds(t.s.context,t.error))return ~0u;
   return t.b.visual->update_world(*t.b.scene,t.error)?1:~0u;
  }
  default:break;
  }
  return t.s.remaining?t.s.remaining(t.s.context,event,values):~0u;
 }
};
}
int npc_actor_frame_v1(actor::RuntimeResult& out,NpcActorFrameBorrowV1 b,
 NpcActorFrameServicesV1 services,std::uint32_t dt,std::string& error){
 if(!b.identity||!b.runtime||!b.state||!b.resolved224||!b.actual_position||!b.commands||!b.world||
  !b.registry||!b.motion_policy||!b.workspace||!b.auxiliary2e0||bool(b.visual)!=bool(b.scene)){
  error="Required actual NPC actor/frame borrows";return 1;
 }
 auto* floor=b.commands->floor_borrow();
 if(!floor||!b.commands->navigation_ready()||b.runtime->object.user!=b.identity||b.world->state_borrow(b.identity)!=b.state){error="NPC frame requires same initialized navigation/World/FSM";return 1;}
 if(*b.auxiliary2e0){error="Required actual NPC auxiliary type/mode producer";return 3;}
 if(b.runtime->controller.validate_boundary){error="Required ordered source DisableWallAvoidance Debug continuation";return 3;}
 target_search::Response16 player{};const target_search::Request24 q{target_search::is_player,0,b.identity,0};
 const auto queries=b.world->targets().search_services();
 if(!queries.invoke||queries.invoke(queries.context,&q,&player)){error="Required source Character IsPlayer camera predicate";return 3;}
 const float* target{};
 if(!services.target_absolute||!services.target_absolute(services.context,target,error)){if(error.empty())error="Required actual NPC target-node/cache producer";return 3;}
 move::Policy decoded{};if(dh2_move_policy(&decoded,&b.state->flags)){error="Invalid NPC source flags";return 1;}
 // Character IsAvoidingObstacles3a3754 is SAME FSM SM_IsIdle(false).
 const actor::RuntimePolicy policy{{decoded.update_path,
  std::uint32_t(dh2_character_state_is_idle(b.state->current,0)),0,decoded.position_from_physics},
  std::uint32_t(player.word!=0),0,0,0,actor::base_virtual_speed};
 Frame frame{b,services,error};const subobjects::Services external{&frame,Frame::invoke};
 std::memcpy(b.runtime->subobjects.position,b.actual_position,12);
 const actor::RuntimeRequest request{b.runtime,b.body,b.visual,b.scene,&floor->collision_world,
  &floor->graph,b.registry,b.motion_policy,b.workspace,b.avoidance,b.resolved224,&policy,&external,
  target,b.identity,b.state->flags,dt};
 const int result=actor::update_actor(out,request,error);
 std::memcpy(b.actual_position,b.runtime->subobjects.position,12);
 b.state->heading_active=b.runtime->controller.heading.active;
 return result;
}
}
