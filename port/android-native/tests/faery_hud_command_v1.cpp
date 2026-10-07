#include "faery_gameplay_v1.hpp"
#include <cstdio>
#include <vector>
using namespace dh2::android_ui;
struct Fixture {bool allowed{};int begin{},end{};std::vector<int> calls;};
int allowed(void* c,std::uintptr_t id,bool* out){auto& f=*static_cast<Fixture*>(c);if(id!=42)return -1;f.calls.push_back(1);*out=f.allowed;return 0;}
int begin(void* c,std::uintptr_t id,bool network){auto& f=*static_cast<Fixture*>(c);if(id!=42||network)return -1;f.calls.push_back(2);return f.begin;}
int end(void* c,std::uintptr_t id,bool network){auto& f=*static_cast<Fixture*>(c);if(id!=42||network)return -1;f.calls.push_back(3);return f.end;}
int main(){
 // Explicit isolated routing fixture, not a produced live gameplay actor.
 model_renderer::PlayerGameplayBinding p;p.active=true;p.character=42;
 Fixture f;FaeryWorldServicesV1 s{};s.context=&f;s.controller_allowed=allowed;s.begin_cast=begin;s.end_cast=end;std::string error;
 if(faery_hud_use_v1(p,s,error)||f.calls!=std::vector<int>{1})return 1;
 f={true,0,0,{}};if(faery_hud_use_v1(p,s,error)||f.calls!=std::vector<int>{1,2,3})return 2;
 f={true,-1,0,{}};if(faery_hud_use_v1(p,s,error)!=-2||f.calls!=std::vector<int>{1,2})return 3;
 f={true,0,-1,{}};if(faery_hud_use_v1(p,s,error)!=-2||f.calls!=std::vector<int>{1,2,3})return 4;
 f={true,0,0,{}};s.end_cast=nullptr;if(faery_hud_use_v1(p,s,error)!=-2||f.calls!=std::vector<int>{1,2})return 5;
 std::puts("PASS original NativeHUDSpell routing: controller rejection, ordered begin/end, required failures and reached prefixes (5 cases)");return 0;
}
