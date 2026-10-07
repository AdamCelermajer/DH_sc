#pragma once
#include <cstdint>
namespace dh2::character {
// Fresh Character C2: 3a9684,3a96a0,3a96a8,3a9750,3a9834.
// Adoption must lend observed storage instead of replaying this constructor.
struct CharacterFrameFieldsV106 {
 std::int16_t ooi_delay14aa{};
 float state_fx_delay14fc{-1.f};
 std::int32_t displayed_gold1500{-1},displayed_damage1504{-1};
 std::uintptr_t timer14e0{};
 std::int32_t state_fx_kind1490{}; //CharacterC2 3a9650/54
 float spot14b0[3]{},previous_spot14bc[3]{}; //3a96b4..e0
 std::uint8_t spot_enabled14c8{};std::int16_t spot_set14ca{-1};
 std::uintptr_t spot_fx14cc{}; //3a96e4..f8
};
struct CharacterTimerUtilFieldsV106 {std::uint32_t remaining0{};std::uint8_t paused4{};};
// Whole TimerUtil.Update317ae4. ARM subtraction wraps before signed clamp.
inline void source_character_timer_util_update_v106(CharacterTimerUtilFieldsV106& timer,std::uint32_t dt)noexcept{
 if(timer.paused4)return;
 const auto value=timer.remaining0-dt;
 timer.remaining0=(value&0x80000000u)?0u:value;
}
}
