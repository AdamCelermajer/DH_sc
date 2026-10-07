#include "character_ranged_attack_v41.hpp"
#include <cstring>
namespace dh2::character {namespace {
float floating(std::uint32_t bits){float f;std::memcpy(&f,&bits,4);return f;}
bool list_valid(const AttackTargetList24* l){return l&&l->count<=65536&&l->cursor<=l->count&&(!l->count||l->entries);}
}
int character_ranged_attack_v41(AttackState64& state,std::uintptr_t requested,std::uint32_t speculative,const RangedAttackServicesV41& services,std::string& error){
 error.clear();if(!state.owner||speculative>255){error="Invalid source ranged attack entry";return -1;}
 auto call=[&](RangedAttackOperationV41 op,RangedAttackResponseV41& r,std::uintptr_t other=0,const char* name=nullptr,float radius=0,float cone=0){r={};if(!services.invoke||services.invoke(services.context,state,{op,state.owner,other,speculative,radius,cone,name},r,error)){if(error.empty())error="Required source ranged operation "+std::to_string(unsigned(op));return false;}return true;};
 using O=RangedAttackOperationV41;RangedAttackResponseV41 response;
 if(!call(O::owner_dead,response))return -2;if(response.word)return 0;
 if(state.owner_flags528&1)return 0;
 if(!call(O::range_parameters,response))return -2;
 if(!response.word)return call(O::melee_fallback,response,requested)?0:-2;
 const auto maximum=response.parameters[1];
 if(!call(O::is_attacking,response))return -2;const bool attacking=response.word!=0;
 if(attacking&&state.last)return 0;
 if(!call(O::debug,response,0,"isTracingCharAICommands"))return -2;
 if(!attacking)state.continued=0; // original3d0964; continuing path retains78
 if(!attacking&&requested){
  if(!call(O::look_at,response,requested))return -2;
  if(!speculative&&!call(O::set_attack_state,response,0))return -2;return 0;
 }
 if(!call(O::list_create,response)||!list_valid(response.list)){if(error.empty())error="Required source ranged target list";return -2;}
 auto* list=response.list;
 const bool narrow=state.heading_active!=0; // captured before CanAttack callbacks
 auto destroy=[&](){RangedAttackResponseV41 r;return call(O::list_destroy,r,reinterpret_cast<std::uintptr_t>(list));};
 bool search=true;
 if(attacking&&!narrow&&state.target){
  if(!call(O::can_attack_current,response)){destroy();return -2;}
  if(response.word){
   if(!state.target){error="Source ranged current target disappeared after CanAttack";destroy();return -2;}
   if(!call(O::target_dead,response,state.target)){destroy();return -2;}
   if(!response.word){search=false;if(!call(O::owner_player,response)){destroy();return -2;}if(response.word&&!call(O::debug,response,0,"isTracingChar_MeleePotentialTarget")){destroy();return -2;}}
  }
 }
 if(search){float cone=floating(0x40c90fdb);
  if(narrow){if(!call(O::frontal_angle,response)){destroy();return -2;}std::int32_t angle;std::memcpy(&angle,&response.word,4);const auto half=angle/2-(angle<0&&angle%2);volatile float c=float(half)*floating(0x3c8efa35);cone=c;}
  else if(!call(O::list_frontal_sort,response,reinterpret_cast<std::uintptr_t>(list))){destroy();return -2;}
  if(!call(O::list_search,response,reinterpret_cast<std::uintptr_t>(list),nullptr,float(maximum),cone)){destroy();return -2;}
 }
 if(!list_valid(list)){error="Invalid ranged list after source Search";destroy();return -2;}
 if(list->cursor<list->count){
  if(!call(O::set_target,response,list->entries[list->cursor])){destroy();return -2;}
  // Original reloads deque begin AFTER SetTarget callback, before LookAt.
  if(!list_valid(list)||list->cursor>=list->count){error="Source ranged first target lifetime changed";destroy();return -2;}
  if(!call(O::look_at,response,list->entries[list->cursor])){destroy();return -2;}
 }
 if(!destroy())return -2;
 if(!attacking&&!speculative&&!call(O::set_attack_state,response,0))return -2;
 return 0;
}
}
