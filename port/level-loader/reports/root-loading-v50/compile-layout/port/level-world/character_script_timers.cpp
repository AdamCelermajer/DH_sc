#include "character_script_timers.hpp"
#include <cstdio>

namespace {
using namespace dh2::character;
std::int32_t start(void* context,std::uintptr_t owner,std::uint32_t duration,
 std::int32_t repeat,std::int32_t event,std::uintptr_t reference){
 auto& bridge=*static_cast<ScriptTimerBridge*>(context);
 if(!bridge.timers||bridge.timers->owner!=owner){bridge.diagnostic=-1;return -1;}
 const auto id=dh2_character_timer_start(bridge.timers,duration,repeat,event,reference,bridge.services);
 if(id<0){bridge.diagnostic=id;return -1;}
 return id;
}
void stop(void* context,std::uintptr_t owner,std::uint32_t id){
 auto& bridge=*static_cast<ScriptTimerBridge*>(context);
 if(!bridge.timers||bridge.timers->owner!=owner){bridge.diagnostic=-1;return;}
 const auto result=dh2_character_timer_stop(bridge.timers,id);
 if(result<0)bridge.diagnostic=result;
}
int invoke(void* opaque,const dh2_script_value* arguments,std::uint32_t count,
 dh2_script_value* values,std::uint32_t capacity,std::uint32_t* returned,
 char* error,std::size_t error_capacity,bool starting){
 auto& bridge=*static_cast<ScriptTimerBridge*>(opaque);bridge.diagnostic=0;
 const auto status=(starting?dh2_script_game_start_timer:dh2_script_game_stop_timer)(
  &bridge.bindings,arguments,count,values,capacity,returned,error,error_capacity);
 if(status)return status;
 if(bridge.diagnostic){
  if(returned)*returned=0;
  if(error&&error_capacity)std::snprintf(error,error_capacity,
   "native character timer %s failed (%d)",starting?"start":"stop",bridge.diagnostic);
  return 1;
 }
 return 0;
}
int start_callback(void* opaque,const dh2_script_value* a,std::uint32_t n,
 dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* error,std::size_t bytes){
 return invoke(opaque,a,n,out,capacity,returned,error,bytes,true);
}
int stop_callback(void* opaque,const dh2_script_value* a,std::uint32_t n,
 dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* error,std::size_t bytes){
 return invoke(opaque,a,n,out,capacity,returned,error,bytes,false);
}
}
extern "C" int dh2_character_script_bind_timers(dh2_script_vm* vm,
 dh2::character::ScriptTimerBridge* bridge){
 if(!vm||!bridge||!bridge->timers||!bridge->services||!bridge->timers->owner||
  bridge->timers->reserved||bridge->services->reserved||!bridge->services->expired||
  bridge->timers->count>bridge->timers->capacity||
  (bridge->timers->capacity&&!bridge->timers->slots))return -1;
 bridge->diagnostic=0;
 bridge->bindings={bridge,bridge->timers->owner,start,stop,0};
 const auto installed=dh2_script_game_bind(vm,&bridge->bindings);
 if(installed)return installed;
 const auto first=dh2_script_vm_bind_source_values(vm,"StartTimer",start_callback,bridge);
 if(first)return first;
 return dh2_script_vm_bind_source_values(vm,"StopTimer",stop_callback,bridge);
}
