// Oracle fixture only: exact immutable source registry, explicit game services.
#include "../character_state_owner_behavior.hpp"
using namespace dh2::character;
namespace {
#include "../character_state_owner_data.inc"
std::uint32_t transitions=0,events=0;
struct F {
 StateOwnerInfo40 infos[20];StateOwnerEvent16 records[20][16]{};
 NativeFsm24 native;StateOwnerMachine40 machine;StateOwnerBehaviorPredicate8 predicates{};
 StateOwnerBehaviorContext40 context;StateOwnerServices16 services{};
 static int remaining(void* opaque,StateOwnerMachine40* m,const StateOwnerRequest48* r,StateOwnerResponse8*){
  auto& f=*static_cast<F*>(opaque);
  if(r->operation==state_owner_profile_begin||r->operation==state_owner_profile_end)return 0;
  Request request{};
  if(r->operation==state_owner_character_event){request.service=raise_event;request.argument[0]=static_cast<std::int32_t>(r->event);request.argument[1]=r->state;request.identity=r->payload;}
  else if(r->operation==state_owner_pin)request.service=pin;
  else return 1;
  f.context.bodies->invoke(f.context.bodies->context,m->fsm->state,&request);return 0;
 }
 F(State* state,const Facts* facts,const Services* body):native{state,0xa123456789abcdefull,std::uint32_t(state->current!=-1),0},machine{&native,infos,20,state->current,0,0},context{facts,body,&predicates,{this,remaining}}{
  for(unsigned i=0;i<20;++i){infos[i]=source_infos[i];infos[i].events=records[i];}
  for(const auto& r:registrations)records[r.state][infos[r.state].event_count++]=r.event;
  for(auto& i:infos)for(unsigned j=1;j<i.event_count;++j){auto v=records[i.id][j];unsigned k=j;while(k&&records[i.id][k-1].event>v.event){records[i.id][k]=records[i.id][k-1];--k;}records[i.id][k]=v;}
  if(state->body_present)machine.physical=0xb123456789abcdefull;
  dh2_character_state_owner_behavior_bind(&services,&context);
 }
};
}
extern "C" int dh2_state_owner_behavior_transition_fixture(State* s,const Facts* f,std::int32_t next,std::int32_t event,std::uintptr_t payload,const Services* body){if(!s||!f||!body||!body->invoke)return -1;++transitions;F fixture(s,f,body);return dh2_character_state_owner_transition(&fixture.machine,next,event,payload,&fixture.services);}
extern "C" int dh2_state_owner_behavior_event_fixture(State* s,const Facts* f,std::uint32_t event,std::uintptr_t payload,const Services* body){if(!s||!f||!body||!body->invoke)return -1;++events;F fixture(s,f,body);return dh2_character_state_owner_event(&fixture.machine,static_cast<std::int32_t>(event),payload,&fixture.services);}
extern "C" std::uint32_t dh2_state_owner_behavior_fixture_count(std::uint32_t selector){return selector?events:transitions;}

