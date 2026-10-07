#include "character_ai_events.hpp"
namespace {
using namespace dh2::character;
bool overlap(const void* a,std::uintptr_t n,const void* b,std::uintptr_t m){const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<n:x-y<m;}
bool valid(AIEventResult16* out,AIEventState64* state,const AIEventServices24* svc,const AIEventPayload24* payload){
 if(!out||!state||!svc)return false;
 if(overlap(out,16,state,64)||overlap(out,16,svc,24)||overlap(state,64,svc,24))return false;
 if(!svc->invoke||!state->ai||!state->owner||state->paused>255||state->seeking>255||state->global_blocked>255||state->reserved0||state->reserved1||state->reserved2||svc->reserved||(svc->available&~63u))return false;
 const void* objects[4]={out,state,svc,state->owner};const std::uintptr_t sizes[4]={16,64,24,48};
 for(unsigned i=0;i<4;++i)for(unsigned j=i+1;j<4;++j)if(overlap(objects[i],sizes[i],objects[j],sizes[j]))return false;
 if(payload){for(unsigned i=0;i<4;++i)if(overlap(payload,24,objects[i],sizes[i]))return false;if(payload->reserved0||payload->reserved1)return false;}
 const std::uintptr_t* tables[2]={state->ai_virtuals,state->ais_virtuals};
 for(const auto* table:tables)if(table){for(unsigned i=0;i<4;++i)if(overlap(table,408,objects[i],sizes[i]))return false;if(payload&&overlap(table,408,payload,24))return false;}
 const auto& owner=*state->owner;return owner.owner&&owner.controller&&owner.state_machine&&owner.properties&&owner.forced<=255&&owner.locked<=255&&!owner.reserved0&&!owner.reserved1;
}
struct Kernel {
 AIEventResult16& out;AIEventState64& state;const AIEventServices24& svc;std::uint32_t event;
 int call(std::uint32_t service,std::uint32_t operation,std::uintptr_t subject,std::uintptr_t callee,std::uintptr_t payload,std::uint32_t argument,std::uint32_t& value){
  out.phase=service+1;out.last_service=service;
  if(!(svc.available&(1u<<service))||((service==ai_event_virtual||service==ai_event_ais_virtual||service==ai_event_timer_id)&&!callee))return 2;
  const AIEventRequest40 request{service,operation,event,argument,subject,callee,payload};++out.service_calls;value=0;
  return svc.invoke(svc.context,&state,&request,&value)?3:0;
 }
 std::uintptr_t virtual_at(unsigned slot)const{return state.ai_virtuals?state.ai_virtuals[slot/4]:0;}
 int virtual_call(unsigned slot,std::uintptr_t payload=0,std::uint32_t argument=0){std::uint32_t value=0;return call(ai_event_virtual,slot,state.ai,virtual_at(slot),payload,argument,value);}
 int machine(AIEventOwner48* owner,std::uintptr_t payload){std::uint32_t value=0;return call(ai_event_state_event,0,owner->state_machine,0,payload,0,value);}
 int helper(unsigned address,std::uintptr_t subject,std::uintptr_t payload,std::uint32_t& value){return call(ai_event_helper,address,subject,address,payload,0,value);}
 int execute(std::uintptr_t payload,std::uintptr_t timer_get_id){
  std::uint32_t value=0;int status=0;
  if(event==0||event==1){event=event==0?50001u:50002u;return machine(state.owner,payload);}
  if(event==0x30)return machine(state.owner,payload);
  if(event==0x31){state.paused=0;return 0;}
  if(event==0x32){state.seeking=1;return 0;}
  if(event==0x33)return helper(0x3cb77c,state.ai,0,value);
  if(event==0x34)return helper(0x3df3f0,state.owner->properties,0,value);
  if(event==0x35){
   const auto callee=virtual_at(0x90);value=~0u;
   if(payload){status=call(ai_event_timer_id,0,payload,timer_get_id,0,0,value);if(status)return status;}
   std::uint32_t ignored=0;return call(ai_event_virtual,0x90,state.ai,callee,0,value,ignored);
  }
  if(event==0x3f){status=helper(0x394a3c,state.owner->owner,0,value);if(status)return status;return machine(state.owner,payload);}
  if(event==2||event==0x29){status=virtual_call(event==2?0x24:0x80,payload);if(status)return status;return machine(state.owner,payload);}
  if(event==3)return virtual_call(0x28,payload);
  if(event==0x22||event==0x23){
   // Both original private end helpers are exact mov r0,1 / bx lr bodies.
   status=virtual_call(0x98);if(status)return status;return machine(state.owner,payload);
  }
  if(event==0x28){status=helper(0x3d4434,state.ai,payload,value);if(status)return status;return value?machine(state.owner,payload):0;}
  auto* captured=state.owner;
  if(!captured->forced&&(state.global_blocked||captured->locked))return machine(state.owner,payload);
  if(event==4)return virtual_call(0xb0,payload);
  if(event==7||event==8)return virtual_call(event==7?0x2c:0x30,payload);
  if(event==9){status=virtual_call(0x34,payload);if(status)return status;return machine(state.owner,payload);}
  if(event>=0xa&&event<=0x19){
   static constexpr unsigned slots[16]={0x40,0x44,0x48,0x4c,0x50,0x54,0x58,0x5c,0x60,0x64,0x68,0x6c,0x70,0x74,0x78,0x7c};
   return virtual_call(slots[event-0xa]);
  }
  if(event==0x24||event==0x25||event==0x26||event==0x27){
   const unsigned address=event==0x24?0x3d3d4c:event==0x25?0x3d3d30:event==0x26?0x3d4204:0x3d3ff8;
   status=helper(address,state.ai,0,value);if(status)return status;return value?machine(state.owner,payload):0;
  }
  if(event>=0x1e&&event<=0x21){
   const unsigned address=event==0x1e?0x3d808c:event==0x1f?0x3d8b7c:event==0x20?0x3d8038:0x3d8b28;
   return helper(address,state.ai,0,value);
  }
  if(event==0x1d){
   const auto callee=virtual_at(0x20);
   status=call(ai_event_state_getter,0,captured->state_machine,0,0,0,value);if(status)return status;
   std::uint32_t ignored=0;return call(ai_event_virtual,0x20,state.ai,callee,payload,value,ignored);
  }
  if(event>=0x2a&&event<=0x2c){
   status=virtual_call(event==0x2a?0x8c:event==0x2b?0x84:0x88);if(status)return status;return machine(state.owner,payload);
  }
  if(event>=0x37&&event<=0x3e){
   // Source table 37..3e uses Begin/Persist/End/Result, true then false.
   const unsigned pair=(event-0x37)/2;const unsigned slot=0xbc+pair*4;
   return virtual_call(slot,payload,event&1u);
  }
  return machine(captured,payload);
 }
};
}
extern "C" int dh2_character_ai_event(dh2::character::AIEventResult16* out,dh2::character::AIEventState64* state,std::uint32_t event,const dh2::character::AIEventPayload24* payload,const dh2::character::AIEventServices24* svc){
 if(!payload||!valid(out,state,svc,payload))return 1;
 *out={0,~0u,0,0};Kernel kernel{*out,*state,*svc,event};const int status=kernel.execute(payload->value,payload->timer_get_id);if(!status)out->phase=7;return status;
}
extern "C" int dh2_character_ai_event_script_timer(dh2::character::AIEventResult16* out,dh2::character::AIEventState64* state,std::uint32_t timer_id,const dh2::character::AIEventServices24* svc){
 if(!valid(out,state,svc,nullptr))return 1;
 *out={0,~0u,0,0};if(!state->active){out->phase=7;return 0;}
 Kernel kernel{*out,*state,*svc,0x35};std::uint32_t ignored=0;
 const auto callee=state->ais_virtuals?state->ais_virtuals[0x90/4]:0;
 const int status=kernel.call(dh2::character::ai_event_ais_virtual,0x90,state->active,callee,0,timer_id,ignored);if(!status)out->phase=7;return status;
}
