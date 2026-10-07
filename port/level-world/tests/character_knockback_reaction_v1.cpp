#include "character_knockback_reaction_v1.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
using namespace dh2::character;
namespace {
unsigned checks{};
void check(bool value){++checks;if(!value)throw std::runtime_error("knockback check "+std::to_string(checks));}
struct Receiver {
 State state{};NativeFsm24 fsm{&state,0x100000006ull,1,0};
 dh2::data::PropertyRules rules{};dh2::data::PropertyState sheets{};
 dh2::data::PropertyView properties=dh2::data::property_view(rules,sheets);
 std::vector<int> calls;int fail=-1;bool dead{};
 int call(int n){calls.push_back(n);return n==fail?1:0;}
 KnockbackReactionBorrowV1 borrow(){return {&fsm,&properties,nullptr,nullptr};}
 KnockbackReactionServicesV1 services(){KnockbackReactionServicesV1 s{};s.context=this;
  s.debug=[](void* p,const char* key){check(!std::strcmp(key,"isTracingCharState")||!std::strcmp(key,"isTracingCSKnockedBack"));return static_cast<Receiver*>(p)->call(!std::strcmp(key,"isTracingCharState")?0:1);};
  s.animation=[](void* p,int value){check(value==-1);return static_cast<Receiver*>(p)->call(2);};
  s.filter=[](void* p,int group,int category,int mask,bool secondary){check(group==0&&category==0x51c&&mask==3&&!secondary);return static_cast<Receiver*>(p)->call(3);};
  s.look_at=[](void* p,std::uintptr_t target){check(target==0x100000001ull);return static_cast<Receiver*>(p)->call(4);};
  s.cancel_sneaking=[](void* p){return static_cast<Receiver*>(p)->call(5);};
  s.unpin=[](void* p){return static_cast<Receiver*>(p)->call(6);};
  s.reset_filter=[](void* p){return static_cast<Receiver*>(p)->call(7);};
  s.pin=[](void* p){return static_cast<Receiver*>(p)->call(8);};
  s.is_dead=[](void* p,bool* out){auto& r=*static_cast<Receiver*>(p);*out=r.dead;return r.call(9);};
  s.stop_animation=[](void* p){return static_cast<Receiver*>(p)->call(10);};
  s.set_dead=[](void* p,bool mode,std::uintptr_t payload,bool force){check(mode&&!payload&&force);return static_cast<Receiver*>(p)->call(11);};return s;
 }
};
}
int main(){try {
 Receiver r;auto s=r.services();r.state.body_present=1;r.state.attack_gate=0x18;
 check(character_knockback_focus_v1(r.borrow(),0x100000001ull,s)==1);
 check(r.state.flags==0x2341&&r.state.attack_gate==0x20&&r.state.controller_locked==1);
 check(r.calls==std::vector<int>({0,1,2,3,4,5,6}));
 r.calls.clear();check(character_knockback_blur_v1(r.borrow(),s)==1);
 check(!r.state.controller_locked&&r.calls==std::vector<int>({0,7,8}));
 r.calls.clear();check(character_knockback_event_v1(r.borrow(),0x27,s)==1&&r.calls==std::vector<int>({7}));
 r.calls.clear();check(character_knockback_event_v1(r.borrow(),0x23,s)==1&&r.calls==std::vector<int>({9}));
 r.dead=true;r.calls.clear();check(character_knockback_event_v1(r.borrow(),0x23,s)==1&&r.calls==std::vector<int>({9,10,11}));
 r.calls.clear();check(character_knockback_event_v1(r.borrow(),0x22,s)==1&&r.calls.empty());
 for(int failure=2;failure<=6;++failure){Receiver f;f.fail=failure;f.state.body_present=1;f.state.attack_gate=0x18;auto fs=f.services();check(character_knockback_focus_v1(f.borrow(),0x100000001ull,fs)==-2);check(f.calls.back()==failure);check(f.state.flags==0x2341);if(failure>=4)check(f.state.attack_gate==0x20&&f.state.controller_locked==1);}
 Receiver missing;auto ms=missing.services();ms.look_at=nullptr;check(character_knockback_focus_v1(missing.borrow(),0x100000001ull,ms)==-2);check(missing.state.flags==0x2341&&missing.state.controller_locked==1);
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"actual_source_callback_order\":true,\"failure_prefixes\":5}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
