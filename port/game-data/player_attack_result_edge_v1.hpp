#pragma once

#include "combat_application.hpp"
#include <cstdint>

namespace dh2::data::player_attack_result_edge_v1 {

enum class Step : std::uint32_t {
    lethal_hit, cancel_sneaking, combat_text, combat_sound, ai_combat_result,
    // Character::F_ApplyResult's status tail (PROPS_AddDot, SM_SetDodging /
    // Blocking / Injure / KnockBack / Stun / Scare State, PROPS_DebuffSlow). It
    // is appended so earlier step numbers stay stable, but it runs between
    // lethal_hit and cancel_sneaking, after HitFor/Kill and RegenHP/MP.
    statuses
};
using Invoke = int (*)(void*, Step, std::uintptr_t attacker,
                       std::uintptr_t defender, CombatResult&);

struct Request {
    const MonsterApplicationRequest* application;
    std::uintptr_t attacker_character;
    std::uintptr_t defender_character;
    void* service_context;
    Invoke invoke;
};

struct Result {
    MonsterApplication application{};
    std::uint32_t application_completed = 0;
    std::uint32_t steps_attempted = 0;
    std::uint32_t steps_completed = 0;
    std::uint32_t ai_dispatch_suppressed = 0;
    // CombatSound is a source side-effect whose return value is ignored. Keep
    // provider availability observable without making it a result-tail gate.
    std::uint32_t sound_available = 0;
    std::int32_t sound_provider_status = 0;
    Step last_step = Step::cancel_sneaking;
    std::int32_t provider_status = 0;
};

enum class Status : std::uint32_t { complete, invalid_argument,
    application_failed, service_failed };

// Player Character melee result edge: apply the existing source-derived
// F_ApplyResult core. If its HitFor projection requests a lethal continuation,
// synchronously dispatch it before the source result tail. Then run the
// status tail (Step::statuses; the provider owns F_ApplyResult's
// !IsDead(defender) guard and reads the live outcome bits), CancelSneaking,
// scrolling text, combat sound, and (unless mask bit 29 is set)
// CharAI::OnCombatResults. It owns none of those objects. Required provider
// failures stop at their source edge. CombatSound's source return is ignored,
// so provider failure is recorded and processing continues.
Status execute(const Request*, Result*);

} // namespace dh2::data::player_attack_result_edge_v1
