#pragma once
#include "character_ai_death.hpp"
#include "character_ai_events.hpp"
#include "character_aggro_clear_all_v2.hpp"
#include "character_script_lifecycle.hpp"
#include <functional>
namespace dh2::character {
struct CharacterDeathBorrowV56 {
 AIDeathState64* death{};AIEventState64* events{};TargetBindings48* targets{};
 AggroClearActorBorrowV2 aggro;
 // Borrow the selected ScriptOwner's existing timers, not another AI clock.
 ScriptLifecycleState64* lifecycle{};
};
struct CharacterDeathServicesV56 {
 AggroClearAllServicesV2 aggro;
 std::function<bool(std::uintptr_t,std::uintptr_t,std::uintptr_t,std::string&)> group;
 std::function<bool(std::uintptr_t,std::uintptr_t,std::uintptr_t,const dh2_script_callback_scope*,std::string&)> ais;
 std::function<bool(bool,std::uintptr_t,bool,std::string&)> set_dead;
 // Separate original _SkillCleanUp and _SpellCleanUp reload points. Both
 // must call the SAME retained skill owner, never instantiate one for death.
 std::function<bool(std::string&)> cleanup_skills,cleanup_spells;
};
// Shared whole OnDied3d1000/AI_SetDead3d6cdc composition. No rewards, HP write,
// direct State12 selection, fake animation completion or duplicate event2.
class CharacterDeathOwnerV56 final {
 CharacterDeathBorrowV56 b_;CharacterDeathServicesV56 s_;std::string error_;
 const dh2_script_callback_scope* scope_{};
 static int invoke(void*,AIDeathState64*,const AIDeathRequest48*);
 bool coherent()const noexcept;
 void refresh()noexcept;void publish_timers()noexcept;
public:
 CharacterDeathOwnerV56(CharacterDeathBorrowV56 b,CharacterDeathServicesV56 s):b_(b),s_(std::move(s)){}
 int on_died(AIDeathResult24&,std::uintptr_t,const dh2_script_callback_scope* =nullptr);
 // Saved dead-state restoration reaches AI_SetDead directly. It must not
 // replay OnDied, Kill, quest notifications or reward distribution.
 int set_dead(AIDeathResult24&);
 const std::string& error()const noexcept{return error_;}
};
}
