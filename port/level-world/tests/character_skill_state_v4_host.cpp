#include "../character_skill_state_v4.hpp"
#include <cassert>
#include <cstring>
#include <vector>
using namespace dh2::character::skills;
struct Provider {
 std::vector<unsigned> calls;
 unsigned fail=0,current=6;
 static int invoke(void* p,SkillStateV4*,const SkillStateRequest32V4* q,SkillStateResponse16V4* r){
  auto& self=*static_cast<Provider*>(p);self.calls.push_back(q->operation);
  if(q->operation==self.fail)return 1;
  if(q->operation==skill_state_debug_construct_v4)r->identity=0x1000;
  if(q->operation==skill_state_current_v4)r->word=self.current;
  return 0;
 }
};
int main(){
 dh2::character::State state{};SkillStateV4 fields{&state,0x2000,0,0x3000,0,0,0,1,{0,0}};
 Provider provider;const SkillStateServices16V4 services{&provider,Provider::invoke};
 assert(dh2_character_skill_state_v4(&fields,skill_state_focus_v4,0,0,0,0,&services)==0);
 assert(fields.state==&state&&state.flags==0x6341&&fields.heading_enabled==0);
 provider.calls.clear();provider.fail=skill_state_raise_v4;
 assert(dh2_character_skill_state_v4(&fields,skill_state_blur_v4,0,0,0,0,&services)==-2);
 assert(fields.last_target==fields.target&&provider.calls.back()==skill_state_raise_v4);
 provider.calls.clear();provider.fail=skill_state_use_v4;
 const char payload[]="do_skill";
 assert(dh2_character_skill_state_v4(&fields,skill_state_ai_event_v4,0,0,reinterpret_cast<std::uintptr_t>(payload),0,&services)==-2);
 assert((provider.calls==std::vector<unsigned>{21,22,23,1,2,3,4,24}));
 provider.calls.clear();provider.fail=0;
 const char prefix[]="fx_missing";provider.fail=skill_state_named_prefix_v4;
 assert(dh2_character_skill_state_v4(&fields,skill_state_ai_event_v4,0,0,reinterpret_cast<std::uintptr_t>(prefix),0,&services)==-2);
 assert((provider.calls==std::vector<unsigned>{21,22,25}));
 provider.calls.clear();fields.reserved[0]=1;
 assert(dh2_character_skill_state_v4(&fields,0,0,0,0,0,&services)==-1&&provider.calls.empty());
 fields.reserved[0]=0;provider.calls.clear();
 assert(dh2_character_skill_state_v4(&fields,skill_state_ai_event_v4,0,0,0,0,&services)==-2&&provider.calls.empty());
 assert(dh2_character_skill_state_v4(&fields,skill_state_event_v4,0x28,0,reinterpret_cast<std::uintptr_t>("is_stoppable"),0,&services)==0);
 assert(state.flags&0x8000);
}
