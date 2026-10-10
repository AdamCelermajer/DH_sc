#pragma once
#include "../level-world/move_state.hpp"
#include <array>
#include <optional>
#include <functional>
#include <string>

namespace dh::foundation {
enum class OriginalMotionFocusPrefix { idle, move };
struct OriginalMotionPrefixResult { bool flags_written=false,movement_type_written=false; };
struct OriginalMotionPolicy {
    dh2::move::Policy decoded{};
    float rotation_speed=0;
};
// Character constructor's override of inherited GameObject byte+452(0x1c4):
// base default0, then CharacterC1/C2 store1. Prefix initializes ONLY this cell,
// not motion flags or a complete Character constructor/state machine.
bool original_character_motion_ctor_prefix(std::optional<std::uint32_t>& source_validate_boundary452,
                                           std::string& error);
bool original_motion_boundary_validation(const std::optional<std::uint32_t>& source_validate_boundary452,
                                         bool& enabled,std::string& error);
// References are SAME caller-owned optional source fields, not another FSM.
// Move invokes exact dh2_move_focus_begin: flags23c1 + movement type0.
// Idle writes2380 only if ACTUAL idle_suppressed byte0; otherwise retains all.
// These are focus PREFIXES only, not UpdateType/animation/pin/blur/full focus.
bool original_motion_focus_prefix(OriginalMotionFocusPrefix,
    std::optional<std::uint32_t>& source_flags520,
    std::optional<std::uint32_t>& source_movement_type,
    std::uint8_t actual_idle_suppressed,OriginalMotionPrefixResult&,std::string& error);
// Explicit source NewFlags/set_flags effect assignment. Does not infer flags
// from state ID, animation name, input or source AITable flags.
bool original_motion_source_set_flags(std::optional<std::uint32_t>& source_flags520,
                                     std::uint32_t actual_flags,std::string& error);
// Fails on missing actual flags; no synthetic default. SAME resolved properties.
bool original_motion_policy(const std::optional<std::uint32_t>& source_flags520,
    const std::array<std::int32_t,224>& resolved,OriginalMotionPolicy&,std::string& error);
using OriginalMotionAttackDelayGetter=std::function<bool(std::uint32_t&,std::string&)>;
struct OriginalAttackFocusPrefixResult { bool flags_written=false,attack_gate_written=false; };
// Post-debug CSAttack focus prefix: flags2341 -> actual GetAttackDelay ->
// if nonzero, actual attack_gate528|=1. No animation/pin/speed/sneaking effects.
// Reached writes remain on failure; no rollback of source-prefix side effects.
bool original_motion_attack_focus_prefix(std::optional<std::uint32_t>& source_flags520,
    std::optional<std::uint32_t>& source_attack_gate528,const OriginalMotionAttackDelayGetter&,
    OriginalAttackFocusPrefixResult&,std::string& error);
enum class OriginalMotionBlurBody { idle,move,attack };
enum class OriginalMotionBlurPhase { not_started,idle_store,stop,delay_query,timer,body_query,pin,completed };
struct OriginalMotionBlurResult {
    OriginalMotionBlurPhase phase=OriginalMotionBlurPhase::not_started;
    std::uint32_t service_calls=0;
    bool idle_suppressed_written=false;
};
struct OriginalMotionBlurServices {
    // COMPLETE GameObject::Stop, not Ctrl_Stop or a guessed attack interrupt.
    std::function<bool(std::string&)> stop;
    OriginalMotionAttackDelayGetter attack_delay;
    // TMR_Start(duration,repeat0,event2a,payload0); same actual Character timer owner.
    std::function<bool(std::uint32_t duration_ms,std::uint32_t repeat,
                       std::uint32_t event,std::uintptr_t payload,std::string&)> start_timer;
    // Fresh body+2dc lookup AFTER preceding Stop/timer effects; false means absent.
    std::function<bool(bool&,std::string&)> body_present;
    std::function<bool(std::string&)> pin;
};
// Source post-debug blur bodies only. Idle clears actual byte538; Move Stop->
// fresh body->pin. Attack GetDelay->(if nonzero RE-GET delay->timer2a)->body->pin.
// No flags clearing or state change. Reached side effects remain on failure.
bool original_motion_blur_body(OriginalMotionBlurBody,
    std::optional<std::uint32_t>& source_idle_suppressed538,
    const OriginalMotionBlurServices&,OriginalMotionBlurResult&,std::string& error);
} // namespace dh::foundation
