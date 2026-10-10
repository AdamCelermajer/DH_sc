#include "native_monster_endpoint.hpp"
namespace dh::foundation::enemy_ai {
bool native_monster_target_event(std::uint32_t event,ActorId payload,const NativeMonsterServices& s,std::string& e){
 e.clear();
 auto missing=[&](const char* text){if(e.empty())e=text;return false;};
 auto read=[&](NativeMonsterReceiver& r){
  if(!s.receiver||!s.receiver(r,e))return missing("Required AISMonster+98 actual receiver reload");
  return validate_live_enemy_borrow(r.live,r.live.actor?r.live.actor->id:invalid_actor_id,e);
 };
 // Native AISMonster OnTargetRevived3dd490 and InSight3dd494 are BX LR.
 if(event==11||event==13)return true;
 if(event!=9&&event!=10&&event!=12&&event!=14&&event!=15&&event!=16&&event!=17)return missing("Unsupported native AISMonster target event");
 NativeMonsterReceiver r{};if(!read(r))return false;
 if(event>=14){
  // Genuine captured vtable inherited AISDefault bodies, distinct from Lua.
  const std::uint32_t functions[]{0x3dc698,0x3dc560,0x3dc300,0x3dc284};
  if(!s.inherited||!s.inherited(functions[event-14],r,e))return missing("Required inherited AISDefault range endpoint");
  return true;
 }
 auto set=[&](ActorId target){
  if(!s.set_target||!s.set_target(r,target,0,e))return missing("Required whole CharAI.AI_SetTarget3d6890(mode0)");
  return true;
 };
 if(event==9){
  if(r.live.target->target)return true;
  if(!set(payload)||!read(r))return false;
  if(!r.heading_enabled412)return missing("Required SAME Character heading-enabled byte412");
  *r.heading_enabled412=1;return true;
 }
 if(event==10){
  if(!s.controller_stop||!s.controller_stop(r.live.controller,e))return missing("Required whole v2Controller.Cmd_Stop40559c");
  if(!read(r))return false;
 }else{
  const auto target=r.live.target->target;
  if(target){
   // Controller is captured BEFORE target.GetTargetPosition3935dc.
   void* controller=r.live.controller;std::array<float,3> position{};
   if(!s.target_position||!s.target_position(target,position,e))return missing("Required whole target.GetTargetPosition3935dc");
   if(!s.controller_move_point||!s.controller_move_point(controller,position,e))return missing("Required whole v2Controller.Cmd_MoveTo(point)4054e4");
   if(!read(r))return false;
  }
 }
 if(!set(invalid_actor_id)||!read(r))return false;
 if(!s.clear_all_aggro||!s.clear_all_aggro(r,e))return missing("Required whole CharAI.ClearAggro3d49c4");
 return true;
}
}
