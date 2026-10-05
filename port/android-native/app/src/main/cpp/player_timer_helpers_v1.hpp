#pragma once
#include "player_gameplay_binding.hpp"
#include "character_skill_combat_v6.hpp"
namespace dh2::android_ui {
struct PlayerTimerServicesV1 {
 void* context{};
 int(*remote_updated)(void*,std::uintptr_t,bool*){};
 int(*aggro)(void*,std::uintptr_t,bool* has_aggro,bool* is_aggroed){};
 // Actual self/self F_DotAttack then F_ApplyResult; positive raw damage only.
 int(*dot)(void*,std::uintptr_t,int raw_amount,int element){};
 const dh2::character::skills::SkillAttackNativeServicesV6* debug{};
};
// Handles original AI-event helper keys3cb77c(UpdateRegen),3df3f0(HandleDots).
// No separate TimerStore: parent calls same V6.update_timers each frame.
int player_timer_helper_v1(const model_renderer::PlayerGameplayBinding&,unsigned source_key,const PlayerTimerServicesV1&,std::string&);
}
