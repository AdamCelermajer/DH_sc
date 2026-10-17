#pragma once

#include "../game-data/combat_result.hpp"

#include <cstdint>
#include <string>

namespace dh2::ui {

// Borrowed services for Character::F_ApplyScrollingCombatText (ELF 0x3af77c).
// No duplicate StringManager, PyDataConstants, FlashAnimManager or event queue
// is created here. `position` must return GetTargetPosition(receiver), with
// source Character height delta (+344 - +332) applied to Z.
struct ScrollingCombatTextServicesV1 {
    void* context{};
    bool (*is_follower)(void*, std::uintptr_t receiver, bool&, std::string&){};
    bool (*position)(void*, std::uintptr_t receiver, float xyz[3], std::string&){};
    bool (*source_property)(void*, std::uintptr_t source, std::int32_t property,
                            std::int32_t& fixed_value, std::string&){};
    bool (*is_dual_wielding)(void*, std::uintptr_t source, bool&, std::string&){};
    bool (*is_local_player)(void*, std::uintptr_t receiver, bool&, std::string&){};
    bool (*constant)(void*, const char* group, const char* name,
                    std::int32_t&, std::string&){};
    bool (*localized_string)(void*, std::int32_t string_id, std::string&,
                             std::string&){};
    bool (*style_id)(void*, const char* style, std::int32_t&, std::string&){};
    bool (*play_text)(void*, std::int32_t style_id, const float xyz[3],
                      const char* text, std::int32_t color, std::string&){};
    bool (*play_value)(void*, std::int32_t style_id, const float xyz[3],
                       std::int32_t value, std::int32_t color, std::string&){};
    // Native port adapter equivalent: preserves the original style name so
    // the retained dqhud movie can bind its matching exported clip without a
    // parallel FlashAnimManager integer table.
    bool (*play_authored_text)(void*, const char* style, const float xyz[3],
                      const char* text, std::int32_t color, std::string&){};
    bool (*play_authored_value)(void*, const char* style, const float xyz[3],
                       std::int32_t value, std::int32_t color, std::string&){};
    bool (*localized_formatted_string)(void*,std::int32_t string_id,
                             std::int32_t argument,std::string&,std::string&){};
};

enum class ScrollingCombatTextStatusV1 : std::uint32_t {
    complete,
    invalid_argument,
    service_unavailable,
    service_failed,
};

struct ScrollingCombatTextResultV1 {
    ScrollingCombatTextStatusV1 status{ScrollingCombatTextStatusV1::complete};
    std::uint32_t source_checks{};
    std::uint32_t emitted{};
    std::int32_t last_style_id{};
    std::int32_t last_color{};
    std::int32_t last_value{};
};

// Synchronous, stateless reconstruction of the source Character combat-text
// selector and FlashAnimManager dispatch. `receiver` is the source method's
// Character `this` (the rendered defender); `source_character` is its first
// Character argument (the attacker). Reached provider failures stop at that
// source point; earlier displayed events are retained.
ScrollingCombatTextStatusV1 apply_scrolling_combat_text_v1(
    const data::CombatResult&, std::uintptr_t receiver,
    std::uintptr_t source_character,
    const ScrollingCombatTextServicesV1&,
    ScrollingCombatTextResultV1&, std::string& error);

// Character::F_ApplyScrollingCombatTextXP, reached from DistributeXP only
// after the source recipient/local-player/difficulty decisions. The wrapper
// keeps caller's already-adjusted integer and uses source XP style/color,
// localization formatting, and the same positional display owner.
ScrollingCombatTextStatusV1 apply_scrolling_combat_xp_v1(
    std::uintptr_t receiver,std::int32_t displayed_xp,
    const ScrollingCombatTextServicesV1&,
    ScrollingCombatTextResultV1&,std::string& error);

} // namespace dh2::ui
