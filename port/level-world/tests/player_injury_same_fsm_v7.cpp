#include "../player_injure_state_v7.hpp"
#include "../character_state_owner.hpp"
#include <cassert>
#include <vector>
using namespace dh2::character;
// Named animator/controller fixtures exercise routing, not a fabricated
// renderer or a fabricated animation completion. End event is an explicit input.
struct Fixture {
 CharacterStateOwner owner{0x123456789ull};float gate=-1.f;
 PlayerInjureBorrowV7 borrow{owner.native_fsm().character,&owner.state(),&gate};
 PlayerInjureServicesV7 kernel{};StateOwnerServices16 outer{};
 std::vector<int> calls;std::uintptr_t target=0x987654321ull,looked=0;
 bool animation_missing=false;
 Fixture(){
  kernel.context=this;
  kernel.is_player=[](void*,std::uintptr_t,bool* value){*value=true;return 0;};
  kernel.animation_table=[](void*,std::uintptr_t,int* value){*value=0;return 0;};
  kernel.injure_animation=[](void*,int,bool* found,int* value){*found=true;*value=1127;return 0;};
  kernel.constant=[](void*,const char*,const char*,int* value){*value=0x1000;return 0;};
  kernel.stance=[](void*,std::uintptr_t,int* value){*value=2;return 0;};
  kernel.event=[](void* p,int event,std::uintptr_t payload){auto& f=*static_cast<Fixture*>(p);return f.owner.event(event,payload,f.outer)<0?-1:0;};
  kernel.transition=[](void* p,int state,int event,std::uintptr_t payload){auto& f=*static_cast<Fixture*>(p);return f.owner.transition(state,event,payload,f.outer)<0?-1:0;};
  kernel.debug=[](void* p,const char*){static_cast<Fixture*>(p)->calls.push_back(1);return 0;};
  kernel.set_animation=[](void* p,int value){auto& f=*static_cast<Fixture*>(p);assert(value==-1);f.calls.push_back(2);return f.animation_missing?-1:0;};
  kernel.cancel_sneaking=[](void* p,std::uintptr_t){static_cast<Fixture*>(p)->calls.push_back(3);return 0;};
  kernel.look_at_source_408=[](void* p,std::uintptr_t){auto& f=*static_cast<Fixture*>(p);f.looked=f.target;f.calls.push_back(4);return 0;};
  outer={this,[](void* p,StateOwnerMachine40* m,const StateOwnerRequest48* request,StateOwnerResponse8* response){
   auto& f=*static_cast<Fixture*>(p);assert(m==&f.owner.machine());assert(request->character==f.borrow.character);
   if(request->operation==state_owner_predicate){StateOwnerPredicateFacts16 facts{};facts.flags=f.owner.state().flags;return dh2_character_state_owner_predicate(response,request->source_function,request->state,&facts)==1?0:-1;}
   if(request->state==11){
    int result=1;
    if(request->operation==state_owner_focus)result=player_injure_focus_v7(&f.borrow,&f.kernel);
    else if(request->operation==state_owner_blur)result=player_injure_blur_v7(&f.borrow,&f.kernel);
    else if(request->operation==state_owner_event)result=player_injure_empty_v7(request->source_function);
    return result==1?0:-1;
   }
   return 0; // Explicit other-state body fixture.
  }};
 }
};
int main(){
 Fixture f;assert(f.owner.transition(3,-1,0,f.outer)==1);
 assert(player_set_injure_v7(&f.borrow,44,false,&f.kernel)==1);
 assert(f.owner.state().current==11&&f.owner.machine().current_index==11);
 assert(f.owner.state().flags==0x2b41&&f.owner.state().animation_override==1129);
 assert(f.gate==3000.f&&(f.calls==std::vector<int>{1,1,2,3}));
 f.calls.clear();assert(player_set_injure_v7(&f.borrow,45,false,&f.kernel)==1&&f.calls.empty());
 // Real CSInjured registration routes explicit animator end0x22 to Idle3.
 assert(f.owner.event(0x22,0,f.outer)==1&&f.owner.state().current==3);
 assert(f.looked==f.target&&(f.calls==std::vector<int>{1,4}));
 assert(player_injure_gate_tick_v7(&f.gate,3001)==1&&f.gate==-1.f);
 f.animation_missing=true;f.calls.clear();
 assert(player_set_injure_v7(&f.borrow,46,false,&f.kernel)==-2);
 // Reached source state/flags/gate persist when genuine animation is missing.
 assert(f.owner.state().current==11&&f.gate==3000.f&&f.owner.state().flags==0x2b41);
 assert((f.calls==std::vector<int>{1,1,2}));
}
