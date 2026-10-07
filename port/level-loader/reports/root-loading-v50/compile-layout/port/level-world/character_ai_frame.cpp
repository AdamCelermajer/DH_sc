#include "character_ai_frame.hpp"
namespace {
using namespace dh2::character;
bool overlap(const void* a,std::uintptr_t n,const void* b,std::uintptr_t m){
 const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
 return x<=y?y-x<n:x-y<m;
}
bool valid(const AIFrameOwner48* owner){
 return owner&&owner->owner&&owner->controller&&owner->forced<=255&&owner->locked<=255&&
  owner->zoned<=255&&owner->in_zone<=255&&owner->updated88<=255&&!owner->reserved0&&!owner->reserved1;
}
}
extern "C" int dh2_character_ai_frame(dh2::character::AIFrameResult16* out,
 dh2::character::AIFrameState32* state,const dh2::character::AIFrameServices24* services){
 using namespace dh2::character;
 if(!out||!state||!services||!services->invoke||!state->ai||!state->owner||
  state->paused>255||state->global_blocked>255||state->reserved0||state->reserved1||
  services->reserved||(services->available&~31u)||
  overlap(out,16,state,32)||overlap(out,16,services,24)||overlap(state,32,services,24)||
  overlap(state->owner,48,state,32)||overlap(state->owner,48,services,24)||overlap(state->owner,48,out,16)||!valid(state->owner))return 1;
 *out={0,0,~0u,0};
 auto done=[&](std::uint32_t skip){out->phase=6;out->skip=skip;return 0;};
 if(state->paused)return done(ai_frame_paused);
 auto* captured=state->owner;
 if(!captured->forced){
  if(state->global_blocked)return done(ai_frame_global_blocked);
  if(captured->locked)return done(ai_frame_locked);
 }
 if(!(captured->flags520&0x100))return done(ai_frame_policy_disabled);
 auto call=[&](std::uint32_t service,std::uintptr_t subject,std::uint32_t& value){
  out->phase=service+1;out->last_service=service;
  if(!(services->available&(1u<<service)))return 2;
  const AIFrameRequest16 request{service,0,subject};++out->service_calls;value=0;
  return services->invoke(services->context,state,&request,&value)?3:0;
 };
 std::uint32_t value=0;int status=call(ai_frame_is_zonable,captured->owner,value);if(status)return status;
 if(value&&captured->zoned&&!captured->in_zone)return done(ai_frame_outside_zone);
 // Original reloads AI owner+4 after virtual IsZonable and before this store.
 state->owner->updated88=1;
 for(std::uint32_t service=ai_frame_update_target;service<=ai_frame_on_update;++service){
  status=call(service,state->ai,value);if(status)return status;
 }
 return done(ai_frame_not_skipped);
}
