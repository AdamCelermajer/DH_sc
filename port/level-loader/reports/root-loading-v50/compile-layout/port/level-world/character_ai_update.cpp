#include "character_ai_update.hpp"
#include <cstring>

namespace {
using namespace dh2::character;
bool overlap(const void* a,std::uintptr_t n,const void* b,std::uintptr_t m){
 const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
 return x<=y?y-x<n:x-y<m;
}
struct Calls {
 AIUpdateResult16& result;AIUpdateState80& state;const AIUpdateServices24& services;
 int call(std::uint32_t service,std::uintptr_t subject,std::uint32_t& value,
          std::uint32_t argument=0,const float* position=nullptr){
  result.phase=service+1;result.last_service=service;
  if(!(services.available&(1u<<service)))return 2;
  AIUpdateRequest32 request{service,argument,subject,{0,0,0},0};
  if(position)std::memcpy(request.position,position,12);
  ++result.service_calls;value=0;
  return services.invoke(services.context,&state,&request,&value)?3:0;
 }
};
}
extern "C" int dh2_character_ai_update(dh2::character::AIUpdateResult16* result,
 dh2::character::AIUpdateState80* state,const dh2::character::AIUpdateServices24* services){
 using namespace dh2::character;
 if(!result||!state||!services||!services->invoke||!state->owner||
    state->machine_present>1||state->zoned>255||state->flag85>255||state->reserved||
    services->reserved||(services->available&~255u)||
    overlap(result,16,state,80)||overlap(result,16,services,24)||overlap(state,80,services,24))return 1;
 *result={0,~0u,0,0};Calls calls{*result,*state,*services};std::uint32_t value=0;int status;
 if(state->active){status=calls.call(ai_update_active,state->active,value);if(status)return status;}
 const auto current=state->machine_present?state->state:-1;
 const bool idle=current==3||current==13||current==18;
 const bool awaiting=current==17;
 if((!idle&&!awaiting)||state->target408||state->target418){
  // Original r4 captures this BEFORE DisableZoning; callback owner/visual
  // replacement must not change the subsequent SyncVisibility subject.
  const auto visual=state->visual;
  status=calls.call(ai_update_disable_zoning,state->owner,value);if(status)return status;
  if(visual){status=calls.call(ai_update_sync_visibility,visual,value);if(status)return status;}
 }else if(!state->zoned){
  status=calls.call(ai_update_is_zonable,state->owner,value);if(status)return status;
  if(value){
   status=calls.call(ai_update_enable_zoning,state->owner,value);if(status)return status;
   status=calls.call(ai_update_is_dead,state->owner,value);if(status)return status;
   if(!value&&!state->flag85){
    status=calls.call(ai_update_set_position,state->owner,value,1,state->saved_position);if(status)return status;
    if(state->visual&&state->visual_node){
     status=calls.call(ai_update_node_set_position,state->visual_node,value,0,state->saved_position);if(status)return status;
    }
   }
  }
 }
 result->phase=9;return 0;
}
