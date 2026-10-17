#include "player_ai_death_v1.hpp"
#include <cstring>
#include <stdexcept>

namespace dh2::player_ai_death_v1 { namespace {
namespace st=character::set_target;namespace ac=character_aggro_cleanup;
namespace ds=character_sm_set_dead_state_v1;
struct Range{std::uintptr_t b,e;};
template<class T>bool range(const T* p,Range& r){const auto a=reinterpret_cast<std::uintptr_t>(p);if(!p||a%alignof(T)||a>UINTPTR_MAX-sizeof(T))return false;r={a,a+sizeof(T)};return true;}
bool overlap(Range a,Range b){return a.b<b.e&&b.b<a.e;}
bool valid(const Bindings& b){return b.ai&&b.ai->identity&&b.ai->owner_04&&b.coordinator&&b.coordinator->bound()&&
 b.coordinator->owner()==b.ai->owner_04&&b.target_owner&&b.target_owner->identity==b.ai->owner_04&&!b.target_owner->reserved&&
 b.target_services&&b.dead_state.coordinator==b.coordinator&&
 b.dead_state.character.identity==b.ai->owner_04&&b.dead_state.character.properties&&
 b.dead_state.tables&&b.dead_state.constants&&b.group_identity;}
bool separate(const Bindings& b,Range r){
 const auto clear=[&](const auto* p){Range c;return range(p,c)&&!overlap(c,r);};
 if(!clear(b.ai)||!clear(b.coordinator)||!clear(b.target_owner)||!clear(b.target_services)||
    !clear(&b.dead_state)||!clear(b.dead_state.character.properties)||!clear(b.dead_state.tables)||
    !clear(b.dead_state.constants)||!clear(&b.dead_state.anim_stance)||!clear(b.group_identity))return false;
 const auto& timers=b.coordinator->timers();
 for(std::uint32_t i=0;i<timers.count;++i)if(!clear(timers.slots+i))return false;
 return true;
}
struct Call {
 Bindings& b;Result& out;std::string& error;const std::uintptr_t ai,character;
 bool release_failed=false;
 Call(Bindings& bindings,Result& result,std::string& why):b(bindings),out(result),error(why),ai(b.ai->identity),character(b.ai->owner_04){}
 bool coherent()const{return b.ai->identity==ai&&b.ai->owner_04==character&&b.coordinator->owner()==character&&b.target_owner->identity==character&&b.dead_state.coordinator==b.coordinator&&b.dead_state.character.identity==character;}
 int fail(const char* why){if(error.empty())error=why;return 1;}
 Request request(Operation operation)const{Request q{};q.operation=operation;q.ai=ai;q.character=character;return q;}
 int invoke(const Request& q,Reply& r){
  ++out.calls;out.last_operation=std::uint32_t(q.operation);r={};
  if(!coherent())return fail("Player death borrowed owners changed");
  if(!b.backend.invoke)return fail("Player death reached provider unavailable");
  try{if(b.backend.invoke(b.backend.context,&q,&r,error))return fail("Player death reached provider failed");}
  catch(...){return fail("Player death provider exception");}
  return coherent()?0:fail("Player death borrowed owners changed");
 }
 struct TargetCall {
  Call& c;st::State state;
  explicit TargetCall(Call& call):c(call){load();}
  void load(){const auto& a=*c.b.ai;state={a.identity,c.b.target_owner,a.requested_target_3c,a.target_40,a.last_target_44,a.alive_48,a.sight_49,a.sticky_4c,0};}
  void publish(){auto& a=*c.b.ai;a.requested_target_3c=state.requested_target;a.target_40=state.target;a.last_target_44=state.last_target;
   a.alive_48=state.alive_snapshot;a.sight_49=state.sight_snapshot;a.sticky_4c=state.sticky;}
  static int invoke(void* raw,const st::Request* q,st::Response* r){
   auto& s=*static_cast<TargetCall*>(raw);s.publish();
   int code=1;
   try{if(s.c.coherent()&&s.c.b.target_services->invoke)code=s.c.b.target_services->invoke(s.c.b.target_services->context,q,r);}
   catch(...){s.c.fail("Player death target provider exception");}
   s.load();return code;
  }
 };
 int target(){
  TargetCall t(*this);const st::Services services{&t,b.target_services->ai_property_count,TargetCall::invoke};
  const auto status=st::dh2_character_ai_set_target(&t.state,0,0,&services);t.publish();
  if(status!=st::complete)return fail("Player death source SetTarget failed");
  out.target_completed=1;b.ai->last_target_44=b.ai->target_40;out.sync_completed=1;return 0;
 }
 static int get_anim_stance(void* raw,std::uintptr_t character,std::int32_t* stance){
  auto& self=*static_cast<Call*>(raw);if(!stance||character!=self.character)return 1;
  auto q=self.request(Operation::anim_stance);q.subject=character;Reply reply{};
  if(self.invoke(q,reply))return 1;
  *stance=reply.word;
  return 0;
 }
 int dead_state(){
  auto bindings=b.dead_state;
  bindings.anim_stance={this,get_anim_stance};
  const ds::Request request{0,0,1};ds::Result result{};
  const auto status=ds::execute(bindings,request,&result,error);
  if(status!=ds::Status::complete)return fail("Player AI_SetDead SM_SetDeadState failed");
  out.animation_table_valid=result.table_valid;
  out.death_animation=result.death_animation;
  out.despawn_animation=result.despawn_animation;
  out.state_completed=result.state_transitioned;
  return 0;
 }
 struct AggroCall {
  Call& c;Direction direction;
  Request request(Operation op,std::uint64_t peer=0,void* handle=nullptr)const{
   auto q=c.request(op);q.direction=direction;q.subject=std::uintptr_t(peer);q.lifetime_handle=handle;return q;
  }
  static bool contains(void* raw,std::uint64_t peer,void* handle,std::uint64_t owner){
   auto& s=*static_cast<AggroCall*>(raw);if(owner!=s.c.character)throw std::runtime_error("Player death mirror owner differs");Reply r{};
   if(s.c.invoke(s.request(Operation::contains_mirror,peer,handle),r))throw std::runtime_error("Player death mirror query failed");
   return r.word!=0;
  }
  static void erase(void* raw,std::uint64_t peer,void* handle,std::uint64_t owner){
   auto& s=*static_cast<AggroCall*>(raw);if(owner!=s.c.character)throw std::runtime_error("Player death mirror owner differs");Reply r{};
   if(s.c.invoke(s.request(Operation::erase_mirror,peer,handle),r))throw std::runtime_error("Player death mirror erase failed");
  }
  static std::int32_t retain(void* raw,std::uint64_t peer,void** handle){
   auto& s=*static_cast<AggroCall*>(raw);Reply r{};const auto status=s.c.invoke(s.request(Operation::retain_peer,peer),r);*handle=r.lifetime_handle;return status;
  }
  static void clear(void* raw,std::uint64_t owner){
   auto& s=*static_cast<AggroCall*>(raw);if(owner!=s.c.character)throw std::runtime_error("Player death map owner differs");Reply r{};
   if(s.c.invoke(s.request(Operation::clear_relations),r))throw std::runtime_error("Player death map clear failed");
  }
  static void notify(void* raw,std::uint64_t peer,void* handle,std::uint64_t owner){
   auto& s=*static_cast<AggroCall*>(raw);if(owner!=s.c.character)throw std::runtime_error("Player death notification owner differs");Reply r{};
   if(s.c.invoke(s.request(Operation::notify_deaggro,peer,handle),r))throw std::runtime_error("Player death OnDeAggro failed");
  }
  static void release(void* raw,std::uint64_t peer,void* handle)noexcept{
   auto& s=*static_cast<AggroCall*>(raw);Reply r{};
   try{if(s.c.invoke(s.request(Operation::release_peer,peer,handle),r))s.c.release_failed=true;}
   catch(...){s.c.release_failed=true;}
  }
 };
 int aggro(Direction direction){
  auto q=request(Operation::relations);q.direction=direction;Reply reply{};
  if(invoke(q,reply))return 1;
  const auto count=direction==Direction::outgoing?b.ai->tree_7c.count:b.ai->tree_94.count;
  if(reply.count!=count)return fail("Player death canonical aggro count differs");
  const ac::Facts facts{character,reply.count,reply.peers,reply.count};AggroCall callback{*this,direction};
  const ac::Services services{&callback,AggroCall::contains,AggroCall::erase,AggroCall::retain,AggroCall::clear,AggroCall::notify,AggroCall::release};
  auto& result=direction==Direction::outgoing?out.outgoing:out.incoming;
  if(ac::clear_all(&facts,&services,&result)!=ac::Status::complete||release_failed)return fail("Player death source aggro cleanup failed");
  if(direction==Direction::outgoing)out.outgoing_completed=1;else out.incoming_completed=1;
  return 0;
 }
 int execute(std::uintptr_t killer){
  Reply reply{};auto q=request(Operation::group_died);q.subject=*b.group_identity;q.killer=killer;
  if(q.subject){++out.group_calls;if(invoke(q,reply))return 1;}
  const auto active=b.ai->active_ais_1c;
  if(active){++out.ais_calls;
   if(active==b.player_ais_identity)out.default_ais_died=1; // Proven 4-byte inherited bx lr.
   else{q=request(Operation::ais_died);q.subject=active;q.killer=killer;if(invoke(q,reply))return 1;}
  }
  if(target()||dead_state())return 1;
  if(b.coordinator->stop_timer(b.ai->word_10)<0)return fail("Player death timer33 stop failed");
  ++out.timer_stops;
  if(b.coordinator->stop_timer(b.ai->word_14)<0)return fail("Player death timer34 stop failed");
  ++out.timer_stops;
  b.ai->word_14=UINT32_MAX;b.ai->word_10=UINT32_MAX;out.timer_ids_cleared=1;
  if(aggro(Direction::outgoing)||aggro(Direction::incoming))return 1;
  if(invoke(request(Operation::skill_cleanup),reply))return 1;
  out.skill_completed=1;
  if(invoke(request(Operation::spell_cleanup),reply))return 1;
  out.spell_completed=1;return 0;
 }
};
}
Runtime::Runtime(Bindings b):bindings_(b){if(!valid(b))throw std::invalid_argument("Invalid borrowed Player death owners");}
Status Runtime::died(std::uintptr_t killer,Result* out,std::string& error){
 if(busy_)return Status::busy;
 Range r,e,t;
 if(!valid(bindings_)||!range(out,r)||!range(&error,e)||!range(this,t)||overlap(r,e)||overlap(r,t)||overlap(e,t)||
  !separate(bindings_,r)||!separate(bindings_,e))return Status::invalid_argument;
 *out={};error.clear();busy_=true;struct Scope{bool& busy;~Scope(){busy=false;}} scope{busy_};Call call(bindings_,*out,error);
 try{return call.execute(killer)?Status::failed:Status::complete;}
 catch(...){if(error.empty())error="Player death provider exception";return Status::failed;}
}
}
