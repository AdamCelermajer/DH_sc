#pragma once
#include "../level-world/character_design_services.hpp"
namespace dh2::ui {
// Borrow actual Application GetDt(+8c) and selected Level load phase(+130).
// A zero selected_level is genuine GetCurrentLevel null; do not infer it from
// a missing native phase producer. This owner creates no level or Debug map.
struct CombatFlashTickBorrowV1 {
 const std::uint32_t* application_dt{};
 std::uintptr_t selected_level{};
 const std::int32_t* load_phase{};
};
struct CombatFlashTickInputsV1 {std::uint32_t application_dt{};std::int32_t load_phase{};};
int combat_flash_tick_inputs_v1(CombatFlashTickInputsV1*,const CombatFlashTickBorrowV1&);
// Source Draw337888 load ->337a88 GetSwitch(IsDisablingFlashAnimation).
//1 delivered, negative required failure. Output unchanged on failure.
int combat_flash_debug_gate_v1(bool*,character::DebugSwitches*,const character::DebugFileServices24*);
}
