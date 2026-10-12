#include "character_target_update.hpp"
namespace {
using namespace dh2::character;
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool owner_valid(const TargetOwner16* p){return aligned(p)&&!p->reserved16&&!p->reserved;}
bool valid(const TargetState48* p){return aligned(p)&&!p->reserved8&&!p->reserved&&owner_valid(p->owner);}
int call(TargetState48* s,const TargetUpdateServices16* services,std::uint32_t op,std::uintptr_t subject,std::uintptr_t other,std::uint32_t& result,std::uint32_t event=0){
 if(!valid(s))return 2;
 const TargetUpdateRequest24 request{op,event,subject,other};result=0;
 return services->invoke(services->context,s,&request,&result)?2:0;
}
int owner_call(TargetState48* s,const TargetUpdateServices16* services,std::uint32_t op,std::uint32_t& result){
 if(!owner_valid(s->owner))return 2;
 return call(s,services,op,s->owner->identity,0,result);
}
int event(TargetState48* s,const TargetUpdateServices16* services,std::uint32_t id,std::uintptr_t payload){
 if(!owner_valid(s->owner))return 2;
 std::uint32_t ignored=0;return call(s,services,target_update_raise_event,s->owner->identity,payload,ignored,id);
}
}
extern "C" int dh2_character_target_update(TargetState48* s,const TargetUpdateServices16* services){
 if(!valid(s)||!aligned(services)||!services->invoke)return 1;
 std::uint32_t value=0;
 if(owner_call(s,services,target_update_awaiting_spawn,value))return 2;
 if(value)return 0;
 if(owner_call(s,services,target_update_in_limbus,value))return 2;
 if(value||!s->target)return 0;
 if(!owner_valid(s->owner)||call(s,services,target_update_interactive,s->target,s->owner->identity,value))return 2;
 if(!value){s->target=0;s->last_target=0;return event(s,services,0xc,0);}
 if(!s->target)return 0;
 if(owner_call(s,services,target_update_owner_ai_id,value))return 2;
 if(!s->target)return 2; // Original now dereferences target; invalid provider lifetime.
 if(call(s,services,target_update_dead,s->target,0,value))return 2;
 const auto alive=static_cast<std::uint8_t>(value^1u);
 if(s->alive){if(!alive&&event(s,services,0xa,s->target))return 2;}
 else if(alive&&event(s,services,0xb,s->target))return 2;
 s->alive=alive;
 if(!s->target)return 0;
 if(call(s,services,target_update_sight,s->identity,s->target,value))return 2;
 const auto sight=value;
 if(s->sight){if(!sight&&event(s,services,0xc,s->target))return 2;}
 else if(sight&&event(s,services,0xd,s->target))return 2;
 s->sight=static_cast<std::uint8_t>(sight);
 if(!s->target||!sight)return 0;
 if(!owner_valid(s->owner)||call(s,services,target_update_interactive,s->target,s->owner->identity,value))return 2;
 if(!value)return 0;
 if(owner_call(s,services,target_update_can_range,value))return 2;
 if(value){
  if(call(s,services,target_update_close_range,s->identity,s->target,value))return 2;
  if(value)return event(s,services,0x10,s->target);
  if(call(s,services,target_update_ranged_range,s->identity,s->target,value))return 2;
  return event(s,services,value?0xf:0xe,s->target);
 }
 if(call(s,services,target_update_melee_range,s->identity,s->target,value))return 2;
 return event(s,services,value?0x11:0xe,s->target);
}
