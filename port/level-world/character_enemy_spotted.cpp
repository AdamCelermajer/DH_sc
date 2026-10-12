#include "character_enemy_spotted.hpp"
#include <cstring>
namespace {
using namespace dh2::character;
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool valid(const EnemySpottedState16* s){
 if(!aligned(s)||!aligned(s->ai)||s->ai->reserved||!aligned(s->ai->target))return false;
 for(auto x:s->ai->reserved_bytes)if(x)return false;
 const auto* target=s->ai->target;const auto* owner=target->owner;
 return !target->reserved&&!target->reserved8&&aligned(owner)&&!owner->reserved16&&!owner->reserved;
}
int call(EnemySpottedState16* s,const EnemySpottedServices24* svc,const EnemySpottedRequest48& req,std::uint32_t& value){if(!valid(s))return 2;value=0;return svc->invoke(svc->context,s,&req,&value)?2:0;}
EnemySpottedRequest48 request(std::uint32_t service,std::uintptr_t subject,std::uintptr_t enemy){EnemySpottedRequest48 result{};result.service=service;result.subject=subject;result.enemy=enemy;return result;}
int debug(EnemySpottedState16* s,const EnemySpottedServices24* svc,const char* name){
 std::uint32_t ignored=0;auto req=request(enemy_debug_load,0,0);if(call(s,svc,req,ignored))return 2;
 req=request(enemy_debug_construct,0,0);req.text=name;if(call(s,svc,req,ignored))return 2;
 req.service=enemy_debug_query;if(call(s,svc,req,ignored))return 2;
 req.service=enemy_debug_destroy;return call(s,svc,req,ignored);
}
float floating(std::uint32_t word){float result;std::memcpy(&result,&word,4);return result;}
}
extern "C" int dh2_character_enemy_spotted(EnemySpottedState16* s,std::uintptr_t enemy,const EnemySpottedServices24* svc){
 if(!valid(s)||!enemy||!aligned(svc)||!svc->invoke)return 1;
 if(debug(s,svc,"isTracingCharAIEvents"))return 2;
 std::uint32_t value=0;
 if(!valid(s))return 2;
 const auto group=s->group;
 if(group){auto req=request(enemy_group_spotted,group,enemy);req.other=s->ai->target->owner->identity;if(call(s,svc,req,value))return 2;}
 auto req=request(enemy_awaiting_spawn,enemy,0);if(call(s,svc,req,value))return 2;if(value)return 0;
 if(!valid(s))return 2;
 req=request(enemy_awaiting_spawn,s->ai->target->owner->identity,0);if(call(s,svc,req,value))return 2;if(value)return 0;
 req=request(enemy_in_limbus,enemy,0);if(call(s,svc,req,value))return 2;if(value)return 0;
 if(!valid(s))return 2;
 req=request(enemy_in_limbus,s->ai->target->owner->identity,0);if(call(s,svc,req,value))return 2;if(value)return 0;
 if(!valid(s))return 2;
 req=request(enemy_in_combat,s->ai->target->identity,0);if(call(s,svc,req,value))return 2;
 bool aggro=true;
 if(value){req=request(enemy_is_player,enemy,0);if(call(s,svc,req,value))return 2;aggro=value!=0;}
 if(aggro){
  if(!valid(s))return 2;
  req=request(enemy_get_aggro,s->ai->target->identity,enemy);if(call(s,svc,req,value))return 2;
  if(floating(value)==0.0f){
   if(!valid(s)||!aligned(svc->initial_threat))return 2;
   req=request(enemy_add_aggro,s->ai->target->owner->identity,enemy);req.word=*svc->initial_threat;
   if(call(s,svc,req,value))return 2;
   if(floating(value)>0.0f&&debug(s,svc,"isTracingThreatChange"))return 2;
  }
 }
 if(!valid(s))return 2;
 const auto active=s->ai->active;if(!active)return 0;
 req=request(enemy_active_dispatch,active,enemy);return call(s,svc,req,value);
}
