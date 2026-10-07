#pragma once
#include "character_player_skills_v6.hpp"
#include <string>
namespace dh2::android_ui {
struct PlayerRecurringResultV7 {
 int timer_scan{};
 bool owner_ready_before{},owner_ready_after{};
 std::string owner_error_before,owner_error_after,session_error_after;
 // A successful TimerStore traversal is not evidence of successful callbacks.
 bool delivered_without_diagnostic()const noexcept {
  return timer_scan>=0&&owner_ready_before&&owner_ready_after&&
   owner_error_before.empty()&&owner_error_after.empty()&&session_error_after.empty();
 }
};
// Borrow only the retained player's V6 owner. This has no TimerStore, script,
// properties, Save, callback table or failure-reset authority of its own.
PlayerRecurringResultV7 player_recurring_effects_v7(
 character::skills::CharacterPlayerSkillsV6&,std::uint32_t dt_ms,
 std::uint32_t source_script_blocked);
}
