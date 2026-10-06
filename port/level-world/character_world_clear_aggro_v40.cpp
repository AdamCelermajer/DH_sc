#include "character_world_clear_aggro_v40.hpp"
namespace dh2::character {
int CharacterWorldClearAggroV40::clear(std::uintptr_t owner,std::uintptr_t other,const dh2_script_callback_scope* scope){
 error_.clear();if(!other)return 0;
 if(scope&&!dh2_script_callback_scope_valid(scope)){error_="Required valid current ClearAggro callback scope";return -1;}
 auto borrow=[&](std::uintptr_t id,WorldClearAggroActorV40& v){
  if(!services_.actor||!services_.actor(services_.context,id,v,error_)){if(error_.empty())error_="Required SAME registered ClearAggro actor";return false;}
  if(v.identity!=id||!v.lifetime||!v.target||!v.target->state||!v.target->state->owner||v.target->state->owner->identity!=id){error_="Required canonical Character/Target owner and lifetime for ClearAggro";return false;}return true;
 };
 WorldClearAggroActorV40 a{},b{};if(!borrow(owner,a)||!borrow(other,b))return -1;
 data::AggroQuery query{};if(dh2_aggro_query(&query,a.outgoing,other,0)){error_="Required actual source outgoing aggro map";return -1;}
 bool exists=false;for(unsigned i=0;i<a.outgoing->count;++i)if(a.outgoing->entries[i].character==other){exists=true;break;}
 if(exists){
  data::AggroChange change{};const data::AggroRequest request{a.outgoing,b.incoming,a.target->state->owner->identity,other,0,0};
  if(dh2_aggro_apply(&change,&request,data::aggro_clear)){error_="Source ClearAggro reciprocal erase rejected";return -1;}
  if(!services_.on_deaggro||!services_.on_deaggro(services_.context,other,request.owner,scope,error_)){if(error_.empty())error_="Required selected SAME AIS OnDeAggro";return -2;}
 }
 // Original reloads receiverAI+4 AFTER OnDeAggro. Target+408 is live too:
 // reentry can change either pointer and thereby skip clearing/stopping.
 if(!a.target->state->owner||!b.target->state){error_="ClearAggro live target lifetime changed";return -2;}
 if(a.target->state->owner->identity!=b.target->state->target)return 0;
 auto* binding=b.target;const auto* old=binding->scope;
 struct Restore{TargetBindings48* b;const dh2_script_callback_scope* old;~Restore(){b->scope=old;}} restore{binding,old};binding->scope=scope;
 if(dh2_character_ai_set_target(binding->state,0,0,&binding->services)){error_="Required source ClearAggro SetTarget(NULL)";return -2;}
 // Original reloads Character+378 after SetTarget. A callback may replace
 // the controller; do not freeze it at the initial actor borrow.
 WorldClearAggroActorV40 fresh{};if(!borrow(other,fresh))return -2;
 if(fresh.target!=binding){error_="ClearAggro target binding lifetime changed";return -2;}
 if(!fresh.controller||!services_.command_stop||!services_.command_stop(services_.context,other,fresh.controller,scope,error_)){if(error_.empty())error_="Required SAME target controller Cmd_Stop";return -2;}
 return 0;
}
}
