#include "character_target_events.hpp"
namespace {
using namespace dh2::character;
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool valid(const TargetEventState32* s){
 if(!aligned(s)||!aligned(s->target)||s->reserved||s->target->reserved||s->target->reserved8)return false;
 for(auto x:s->reserved_bytes)if(x)return false;
 const auto owner=s->target->owner;return aligned(owner)&&!owner->reserved16&&!owner->reserved;
}
int call(TargetEventState32* s,const TargetEventServices40* services,const TargetEventRequest64& request,TargetEventResponse24& response){
 if(!valid(s))return 2;
 response={};return services->invoke(services->context,s,&request,&response)?2:0;
}
TargetEventRequest64 request(std::uint32_t op,std::uint32_t event,std::uintptr_t subject=0){TargetEventRequest64 result{};result.service=op;result.event=event;result.subject=subject;return result;}
}
extern "C" int dh2_character_target_event(TargetEventState32* s,std::uint32_t event,const TargetEventServices40* services){
 if(!valid(s)||!aligned(services)||!services->invoke||services->reserved||event<0xa||event>0x11)return 1;
 TargetEventResponse24 response{};auto query=request(target_event_debug_load,event);
 if(call(s,services,query,response))return 2;
 query=request(target_event_debug_construct,event);query.text="isTracingCharAIEvents";
 if(call(s,services,query,response))return 2;
 query.service=target_event_debug_query;if(call(s,services,query,response))return 2;
 query.service=target_event_debug_destroy;if(call(s,services,query,response))return 2;
 if(event==0xc){
  if(!valid(s))return 2;
  query=request(target_event_is_target_character,event,s->target->owner->identity);
  if(call(s,services,query,response))return 2;
  if(response.word){
   if(!valid(s))return 2;
   // Source reloads owner after predicate, then retains its embedded AI
   // receiver across GetTargetAsCharacter before ClearAggro.
   const auto owner=s->target->owner->identity;
   query=request(target_event_get_target_character,event,owner);
   if(call(s,services,query,response))return 2;
   query=request(target_event_clear_aggro,event,owner);query.other=response.identity;
   if(call(s,services,query,response))return 2;
  }
 }else if(event==0xd){
  if(!valid(s))return 2;
  const auto sounds=services->ai_sounds;const auto count=services->ai_count;
  query=request(target_event_owner_ai_id,event,s->target->owner->identity);
  if(call(s,services,query,response))return 2;
  if(!aligned(sounds)||response.word>=count)return 2;
  const auto sound=sounds[response.word];
  const auto manager=services->sound_manager;
  if(!valid(s))return 2;
  query=request(target_event_owner_position,event,s->target->owner->identity);
  if(call(s,services,query,response))return 2;
  query=request(target_event_play_sound,event,manager);query.sound=sound;query.flag=1;query.integer=0;query.parameters[0]=-1.0f;query.parameters[1]=-1.0f;
  for(unsigned i=0;i<3;++i)query.position[i]=response.position[i];
  if(call(s,services,query,response))return 2;
 }
 if(!valid(s))return 2;
 const auto active=s->active;
 if(event==0xa)s->continued=0;
 else if(event==0xc||event==0xe||event==0xf)s->target->changed=0;
 if(!active)return 0;
 query=request(target_event_active_dispatch,event,active);
 return call(s,services,query,response);
}
