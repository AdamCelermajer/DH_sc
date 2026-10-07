#include "character_aggro_clear_all_v2.hpp"
#include <algorithm>
#include <vector>
namespace dh2::character {
namespace {
bool table(const data::AggroTable* t){
 if(!t||t->count>t->capacity||t->capacity>1048576||
    (t->capacity&&!t->entries))return false;
 std::uint64_t previous{};
 for(std::uint32_t i=0;i<t->count;++i){const auto& v=t->entries[i];
  if(!v.character||v.character<=previous||v.reserved)return false;
  previous=v.character;
 }return true;
}
bool valid(const AggroClearActorBorrowV2& a){
 return a.identity&&table(a.outgoing)&&table(a.incoming)&&a.outgoing!=a.incoming&&
  a.target&&a.target->state&&a.target->state->owner&&
  a.target->state->owner->identity==a.identity&&!a.target->state->reserved&&
  !a.target->state->reserved8&&!a.target->reserved[0]&&!a.target->reserved[1]&&
  a.target->services.invoke;
}
bool erase(data::AggroTable& t,std::uintptr_t id){
 for(std::uint32_t i=0;i<t.count;++i)if(t.entries[i].character==id){
  std::move(t.entries+i+1,t.entries+t.count,t.entries+i);--t.count;return true;
 }return false;
}
bool run(AggroClearAllResultV2& out,AggroClearActorBorrowV2 owner,
 bool incoming,bool clear_targets,const AggroClearAllServicesV2& s,
 std::string& error,const dh2_script_callback_scope* scope){
 out={};error.clear();
 if(!valid(owner)||!s.actor||!s.on_deaggro||
    (scope&&!dh2_script_callback_scope_valid(scope))){
  error="Required same-actor Aggro/target/callback borrows";return false;
 }
 auto* own=incoming?owner.incoming:owner.outgoing;
 const auto initial_count=own->count;
 std::vector<std::uintptr_t> snapshot;snapshot.reserve(initial_count);
 for(std::uint32_t i=0;i<initial_count;++i){
  if(!table(own)||own->count!=initial_count){
   error="Source Aggro iterator invalidated during reciprocal traversal";return false;
  }
  const auto id=own->entries[i].character;
  AggroClearActorBorrowV2 other{};
  if(!s.actor(s.context,id,other,error))return false;
  if(other.identity!=id||!valid(other)){error="Required retained reciprocal Aggro actor";return false;}
  if(incoming&&clear_targets&&other.target->state->target==owner.target->state->owner->identity){
   auto* b=other.target;const auto* old=b->scope;
   struct Restore {TargetBindings48* b;const dh2_script_callback_scope* old;
    ~Restore(){b->scope=old;}}restore{b,old};b->scope=scope;
   if(dh2_character_ai_set_target(b->state,0,0,&b->services)||
      dh2_character_ai_sync_last_target(b->state)){
    error="Required source ClearAllAggroTowardMe target/sync delivery";return false;
   }
   ++out.targets_cleared;
   // Both source owner and other maps are reloaded after synchronous setter.
   if(!s.actor(s.context,id,other,error))return false;
   if(other.identity!=id||!valid(other)||!table(own)||own->count!=initial_count||
      own->entries[i].character!=id){
    error="Source Aggro iterator invalidated during target delivery";return false;
   }
  }
  auto* reciprocal=incoming?other.outgoing:other.incoming;
  out.reciprocal_erases+=erase(*reciprocal,owner.target->state->owner->identity);
  snapshot.push_back(id);
 }
 own->count=0;out.self_map_cleared=true;
 for(const auto id:snapshot){
  const auto live_owner=owner.target->state->owner->identity;
  if(!s.on_deaggro(s.context,incoming?live_owner:id,
                  incoming?id:live_owner,scope,error))return false;
  ++out.notifications;
 }
 out.complete=true;return true;
}
}
bool character_aggro_clear_all_v2(AggroClearAllResultV2& out,
 AggroClearActorBorrowV2 owner,const AggroClearAllServicesV2& s,
 std::string& error,const dh2_script_callback_scope* scope){
 return run(out,owner,false,false,s,error,scope);
}
bool character_aggro_clear_all_toward_me_v2(AggroClearAllResultV2& out,
 AggroClearActorBorrowV2 owner,bool clear,const AggroClearAllServicesV2& s,
 std::string& error,const dh2_script_callback_scope* scope){
 return run(out,owner,true,clear,s,error,scope);
}
}
