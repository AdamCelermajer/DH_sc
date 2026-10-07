#include "character_timer_effects.hpp"
#include <cstring>
namespace {
using namespace dh2::character;
namespace data=dh2::data;
bool aligned(const void* p,std::size_t n){return p&&reinterpret_cast<std::uintptr_t>(p)%n==0;}
bool overlap(const void* a,std::size_t n,const void* b,std::size_t m){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<n:x-y<m;}
std::int32_t bits(std::uint32_t raw){std::int32_t out;std::memcpy(&out,&raw,4);return out;}
bool view_valid(const data::PropertyView* v){
 if(!aligned(v,alignof(data::PropertyView)))return false;
 for(auto p:std::array<const std::int32_t*,6>{v->defaults,v->types,v->base,v->saved,v->gear,v->resolved})if(!aligned(p,4))return false;
 if(v->group_count&&!aligned(v->groups,alignof(data::PropertyBuffGroup)))return false;
 return !dh2_property_validate(v);
}
struct Kernel {
 TimerEffectResult24& out;TimerEffectState32& state;const TimerEffectServices16& services;
 int send(TimerEffectService op,std::uintptr_t owner,std::int32_t& value,unsigned property=0,std::int32_t amount=0,std::int32_t element=0,data::CombatResult* result=nullptr,const char* debug_name="isTracingChar_Stats"){
  out.phase=op+1;++out.calls;const TimerEffectRequest40 r{op,property,amount,element,owner,op==effect_dot_calculate||op==effect_dot_apply?owner:0,op==effect_debug_query?debug_name:nullptr};value=0;
  if(!owner)return -2;
  return services.invoke(services.context,&state,&r,&value,result)?-2:1;
 }
 int debug(std::uintptr_t owner,const char* name="isTracingChar_Stats"){std::int32_t ignored=0;auto status=send(effect_debug_load,owner,ignored);return status<0?status:send(effect_debug_query,owner,ignored,0,0,0,nullptr,name);}
 int regen(data::PropertyView* view,std::uintptr_t owner,unsigned property,unsigned maximum,std::int32_t amount){
  if(!view_valid(view))return -2;
  const auto current=view->resolved[property],limit=view->resolved[maximum];auto delta=amount<0?limit:amount;
  if(bits(std::uint32_t(current)+std::uint32_t(delta))>limit)delta=bits(std::uint32_t(limit)-std::uint32_t(current));
  if(delta<=0)return 1;
  auto status=debug(owner);if(status<0)return status;
  if(!view_valid(view)||dh2_property_add(view,property,delta))return -2;
  ++out.regen_adds;(property==36?out.hp_add:out.mp_add)=delta;return 1;
 }
 int execute(unsigned event){
  std::int32_t value=0;int status=1;
  if(event==0x33){
   status=send(effect_remote_update,state.owner,value);if(status<0||value)return status;
   const auto tick_owner=state.owner;auto* view=state.properties;
   bool combat=state.has_aggro||state.aggroed;
   if(!combat)for(auto wanted:{5,6,7}){status=send(effect_current_state,state.owner,value);if(status<0)return status;if(value==wanted){combat=true;break;}}
   // Original RegenTick captures its property sheet before Debug callbacks.
   status=debug(tick_owner);if(status<0)return status;
   if(!view_valid(view))return -2;
   status=regen(view,tick_owner,36,38,view->resolved[combat?40:39]);if(status<0)return status;
   if(!view_valid(view))return -2;
   return regen(view,tick_owner,41,43,view->resolved[combat?45:44]);
  }
  auto* view=state.properties;
  data::CombatResult result;
  for(int element=-1;element<5;++element){
   const auto amount=view->resolved[127+element];if(amount<=0)continue;
   status=send(effect_is_dead,state.owner,value);if(status<0)return status;if(value)continue;
   const auto attacker=state.owner;
   // Genuine F_DotAttack uses its distinct attack tracing switch.
   status=debug(attacker,"isTracingChar_Attack");if(status<0)return status;
   status=send(effect_dot_calculate,attacker,value,127+element,amount,element,&result);if(status<0)return status;
   // Source rereads owner after calculation before applying to self.
   status=send(effect_dot_apply,state.owner,value,127+element,amount,element,&result);if(status<0)return status;
   ++out.dot_attacks;
  }
  return 1;
 }
};
}
extern "C" int dh2_character_timer_effect(TimerEffectResult24* out,TimerEffectState32* state,std::uint32_t event,const TimerEffectServices16* services){
 if(!aligned(out,4)||!aligned(state,8)||!aligned(services,8)||!state->owner||!services->invoke||(event!=0x33&&event!=0x34)||!view_valid(state->properties)||overlap(out,24,state,32)||overlap(out,24,services,16)||overlap(state,32,services,16)||overlap(out,24,state->properties,sizeof(data::PropertyView)))return -1;
 for(auto p:std::array<const std::int32_t*,6>{state->properties->defaults,state->properties->types,state->properties->base,state->properties->saved,state->properties->gear,state->properties->resolved})if(overlap(out,24,p,896))return -1;
 *out={};Kernel k{*out,*state,*services};auto result=k.execute(event);if(result>0)out->phase=8;return result;
}
extern "C" int dh2_character_cached_reset(data::PropertyView* view){if(!view_valid(view)||overlap(view->resolved,896,view->defaults,896))return -1;std::memcpy(view->resolved,view->defaults,896);return 1;}
extern "C" int dh2_character_dot_remove(data::PropertyView* view,std::int32_t){return view_valid(view)?1:-1;}
extern "C" int dh2_character_timer_owner_query(std::int32_t* out,const TimerOwner8* fields,std::uint32_t kind){if(!aligned(out,4)||!aligned(fields,4)||fields->reserved||kind>1||overlap(out,4,fields,8))return -1;*out=kind?fields->dead:fields->network_id!=-1?1:fields->remote_update;return 1;}
