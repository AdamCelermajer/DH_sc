#include "live_dispatch.hpp"

namespace dh::foundation::platform_input {
namespace {
bool fail(std::string& error,const char* reason) {if(error.empty())error=reason;return false;}
struct CheckedHud {
    const LiveBorrow& b;
    const dh2::ui::HudAttackServicesV46& s;
    bool use_routed{};
    static CheckedHud& self(void* p) {return *static_cast<CheckedHud*>(p);}
    static bool blocked(void* p,std::uint8_t& v,std::string& e) {auto& c=self(p);return c.s.input_blocked30&&c.s.input_blocked30(c.s.context,v,e);}
    static bool store(void* p,std::uint8_t v,std::string& e) {auto& c=self(p);return c.s.store_controller_blocked&&c.s.store_controller_blocked(c.s.context,v,e);}
    static bool level(void* p,std::uintptr_t& v,const std::uint8_t*& f,std::string& e) {auto& c=self(p);return c.s.current_level&&c.s.current_level(c.s.context,v,f,e);}
    static bool root(void* p,std::uintptr_t& v,std::string& e) {auto& c=self(p);return c.s.cached_root658&&c.s.cached_root658(c.s.context,v,e);}
    static bool cached(void* p,std::uint8_t& v,std::string& e) {auto& c=self(p);return c.s.cached_chars8&&c.s.cached_chars8(c.s.context,v,e);}
    static bool init(void* p,std::string& e) {auto& c=self(p);return c.s.init_cached_chars&&c.s.init_cached_chars(c.s.context,e);}
    static bool player(void* p,std::int32_t i,bool remote,dh2::ui::HudAttackActorBorrowV46& a,std::string& e) {
        auto& c=self(p);
        if(!c.s.local_player||!c.s.local_player(c.s.context,i,remote,a,e))return false;
        if(!a.character)return true; // Preserve source genuine absent local player.
        return (a.character==c.b.character&&a.controller378==c.b.controller)||fail(e,"HUD local player changed from borrowed actor/controller");
    }
    static bool use(void* p,std::uintptr_t controller,std::uintptr_t requested,std::string& e) {
        auto& c=self(p);
        if(controller!=c.b.controller)return fail(e,"HUD interaction controller ownership changed");
        if(!c.b.use_ooi||c.b.use_ooi->identity()!=controller)return fail(e,"HUD interaction requires same live ControllerUseOoiV47");
        c.use_routed=true;
        return c.b.use_ooi->command(requested,e);
    }
    static bool attack(void* p,std::uintptr_t controller,std::uintptr_t requested,std::string& e) {
        auto& c=self(p);
        if(controller!=c.b.controller)return fail(e,"HUD attack controller ownership changed");
        return c.s.attack&&c.s.attack(c.s.context,controller,requested,e);
    }
};
}
bool dispatch_live(const Frame& input,std::uint64_t expected,const BorrowLive& borrow,
                   LiveDispatchResult& result,std::string& error) {
    error.clear();result={};result.residual=input.actions;
    bool skill_requested=false;for(const auto& skill:input.skills)skill_requested|=skill.pressed||skill.released;
    const bool attack_activity=input.attack.pressed||input.attack.held||input.attack.released;
    const bool spell_requested=input.spell.pressed||input.spell.released;
    const bool requested=attack_activity||input.actions.interact||input.actions.targetSelect||input.target_point_requested||skill_requested||spell_requested||input.potion.pressed;
    if(!requested)return true;
    LiveBorrow b;
    if(!borrow||!borrow(b,error))return fail(error,"Live semantic command owner borrow unavailable");
    if(!b.receiver_lease||!expected||b.actor_id!=expected||!b.character||!b.controller)return fail(error,"Live semantic command requires same leased actor/controller");
    // Validate all requested endpoints before mutating an earlier owner.
    if(attack_activity&&(!b.hud_attack||!b.joystick_active||!b.hud_services))return fail(error,"Live attack requires original HUD fields/services");
    if(input.actions.interact&&(!b.use_ooi||b.use_ooi->identity()!=b.controller))return fail(error,"Interaction requires same live ControllerUseOoiV47");
    if(skill_requested&&!b.skill)return fail(error,"Skill request requires same controller skill command");
    if(spell_requested&&!b.spell)return fail(error,"Spell request requires same controller command");
    if(input.potion.pressed&&!b.potion)return fail(error,"Potion request requires same controller command");
    if(input.actions.targetSelect&&!b.select_target)return fail(error,"Selection requires same live target owner");
    if(input.target_point_requested&&!b.target_point)return fail(error,"World touch requires live projection/target owner");
    if(attack_activity) {
        bool consumed{};
        if(input.attack.pressed&&!dh2::ui::hud_attack_event_v46(*b.hud_attack,4,consumed,error))return false;
        if(input.attack.released&&!input.attack.held&&!dh2::ui::hud_attack_event_v46(*b.hud_attack,6,consumed,error))return false;
        CheckedHud checked{b,*b.hud_services};
        dh2::ui::HudAttackServicesV46 services{&checked,CheckedHud::blocked,CheckedHud::store,CheckedHud::level,CheckedHud::root,CheckedHud::cached,CheckedHud::init,CheckedHud::player,CheckedHud::use,CheckedHud::attack};
        if(!dh2::ui::hud_attack_update_v46(*b.hud_attack,*b.joystick_active,services,error))return false;
        result.residual.attack=false;result.attack_dispatched=true;
        result.interaction_dispatched=checked.use_routed;
    }
    if(input.actions.interact) {
        // Original NULL request reloads actual current OOI in the retained owner.
        if(!result.interaction_dispatched&&!b.use_ooi->command(0,error))return false;
        result.residual.interact=false;result.interaction_dispatched=true;
    }
    for(std::size_t i=0;i<input.skills.size();++i)if(input.skills[i].pressed||input.skills[i].released) {
        if(!b.skill(b.controller,std::uint32_t(i),input.skills[i],error))return false;
        result.skill_dispatched[i]=true;
    }
    if(spell_requested&&!b.spell(b.controller,input.spell,error))return false;
    if(input.potion.pressed&&!b.potion(b.controller,error))return false;
    if(input.actions.targetSelect) {if(!b.select_target(b.controller,error))return false;result.residual.targetSelect=false;}
    if(input.target_point_requested&&!b.target_point(b.controller,input.target_point,error))return false;
    return true;
}
}
