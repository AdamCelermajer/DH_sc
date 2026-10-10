#include "live_dispatch.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation::platform_input;
void check(bool ok,const char* text) {if(!ok)throw std::runtime_error(text);}
struct Fixture {
    std::shared_ptr<void> lease=std::make_shared<int>(1);
    dh2::ui::HudAttackHeldFieldsV46 held;
    std::uint8_t joystick{},level_enabled{1},blocked{},stored{},cached{1},click{1};
    std::uintptr_t character{10},controller{20},ooi{},controllable{10};
    std::uint32_t forced{},locked{},network{},global{};
    int attacks{},uses{},skill_begin{},skill_end{},borrows{};
    bool mismatch{};
    dh2::ui::HudAttackServicesV46 hud;
    dh2::character::ControllerUseOoiV47 use;
    Fixture():hud{this,input,store,level,root,cache,init,player,nullptr,attack},
        use({lease,controller,&forced,&locked,&network,&global,&character,&controllable},
            {this,online,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,use_control}) {}
    static Fixture& self(void* p) {return *static_cast<Fixture*>(p);}
    static bool input(void* p,std::uint8_t& v,std::string&) {v=self(p).blocked;return true;}
    static bool store(void* p,std::uint8_t v,std::string&) {self(p).stored=v;return true;}
    static bool level(void* p,std::uintptr_t& id,const std::uint8_t*& v,std::string&) {id=99;v=&self(p).level_enabled;return true;}
    static bool root(void*,std::uintptr_t& v,std::string&) {v=100;return true;}
    static bool cache(void* p,std::uint8_t& v,std::string&) {v=self(p).cached;return true;}
    static bool init(void* p,std::string&) {self(p).cached=1;return true;}
    static bool player(void* p,std::int32_t index,bool remote,dh2::ui::HudAttackActorBorrowV46& a,std::string&) {
        auto& f=self(p);check(index==0&&!remote,"HUD local player parameters changed");
        a={f.character+(f.mismatch?1:0),f.controller,&f.ooi,&f.click};return true;
    }
    static bool attack(void* p,std::uintptr_t c,std::uintptr_t requested,std::string&) {auto& f=self(p);check(c==f.controller&&requested==0,"Attack dispatch changed original NULL request");++f.attacks;return true;}
    static bool online(void*,bool& v,std::string&) {v=false;return true;}
    static bool use_control(void* p,std::uintptr_t c,std::uintptr_t requested,std::string&) {auto& f=self(p);check(c==f.controllable&&requested==0,"Interaction changed original NULL request");++f.uses;return true;}
    bool borrow(LiveBorrow& b,std::string&) {
        ++borrows;b.receiver_lease=lease;b.actor_id=7;b.character=character;b.controller=controller;
        b.hud_attack=&held;b.joystick_active=&joystick;b.hud_services=&hud;b.use_ooi=&use;
        b.skill=[&](std::uintptr_t c,std::uint32_t slot,ButtonEdges edge,std::string&) {check(c==controller&&slot==1,"Skill slot/controller changed");skill_begin+=edge.pressed;skill_end+=edge.released;return true;};
        return true;
    }
};
int main() {try {
    Fixture f;LiveDispatchResult result;std::string error;Frame frame;
    auto borrow=[&](LiveBorrow& b,std::string& e){return f.borrow(b,e);};
    frame.attack={true,true,false};frame.actions.attack=true;
    check(dispatch_live(frame,7,borrow,result,error),error.c_str());
    check(f.attacks==1&&!result.residual.attack&&f.click==0,"Attack did not use borrowed source HUD");
    frame.attack={false,true,false};f.ooi=42;
    check(dispatch_live(frame,7,borrow,result,error),error.c_str());
    check(f.uses==1&&f.attacks==1,"Source HUD OOI branch failed");
    f.global=1;check(dispatch_live(frame,7,borrow,result,error),error.c_str());check(f.uses==1,"Blocked actual controller accepted interaction");
    f.global=0;f.level_enabled=0;f.joystick=1;
    check(dispatch_live(frame,7,borrow,result,error),error.c_str());check(!f.held.held9&&!f.joystick,"Original disabled-level gate lost");
    f.level_enabled=1;frame.attack={true,true,false};f.mismatch=true;
    check(!dispatch_live(frame,7,borrow,result,error)&&error.find("changed")!=std::string::npos,"Mismatched HUD player borrowed");f.mismatch=false;
    frame={};frame.skills[1]={true,true,false};check(dispatch_live(frame,7,borrow,result,error),error.c_str());check(f.skill_begin==1,"Skill press lost");
    frame.skills[1]={false,true,false};auto prior=f.borrows;check(dispatch_live(frame,7,borrow,result,error),error.c_str());check(f.borrows==prior&&f.skill_begin==1,"Held skill fabricated repeated command");
    frame.skills[1]={false,false,true};check(dispatch_live(frame,7,borrow,result,error),error.c_str());check(f.skill_end==1,"Skill release lost");
    frame={};frame.actions.interact=true;check(dispatch_live(frame,7,borrow,result,error),error.c_str());check(result.interaction_dispatched&&!result.residual.interact,"Interaction double dispatch not removed");
    check(!dispatch_live(frame,8,borrow,result,error),"Actor identity mismatch accepted");
    frame={};frame.attack={true,true,false};frame.spell.pressed=true;prior=f.attacks;
    check(!dispatch_live(frame,7,borrow,result,error)&&f.attacks==prior,"Missing required endpoint mutated attack before failure");
    check(f.borrows>=7,"Per-dispatch owner borrow not refreshed");
    std::cout<<"live_dispatch_tests PASS: same HUD/controller, OOI choice, gate masks, skill begin/end, fresh borrows, missing owner rejection\n";return 0;
}catch(const std::exception& e) {std::cerr<<e.what()<<'\n';return 1;} }
