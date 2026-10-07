#include "player_recurring_effects_v7.hpp"
namespace dh2::android_ui {
PlayerRecurringResultV7 player_recurring_effects_v7(
 character::skills::CharacterPlayerSkillsV6& player,std::uint32_t dt,
 std::uint32_t blocked) {
 PlayerRecurringResultV7 result;
 result.owner_ready_before=player.ready();
 result.owner_error_before=player.error();
 // Character::UpdateTimers owns native traversal independently of loading/VM
 // diagnostics. Preserve the frozen V6::update_timers guard; call the SAME
 // session and its SAME expiry service, not a replacement native timer loop.
 result.timer_scan=player.session().update_timers(dt,blocked);
 result.owner_ready_after=player.ready();
 result.owner_error_after=player.error();
 result.session_error_after=player.session().error();
 return result;
}
}
