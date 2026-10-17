#pragma once
#include "combat_application.hpp"

namespace dh2::data::melee_pipeline {
// Borrowed actor projection. The source owner adapter supplies equipped
// weapon facts and its current Character state; properties/state remain the
// same backing objects used by the result and application kernels.
struct Owner {
    PropertyView* properties;
    CombatActorState* state;
    std::int32_t main_damage_class, off_damage_class;
    std::uint32_t two_hander, dual_wield, shield;
    std::int32_t character_state;
};
struct Request {
    const Owner* attacker; // Nonplayer source Character (for example Ghost).
    Owner* player;          // Main player defender.
    CombatRandom* random;
    std::uint32_t offhand, player_idle;
    // Optional owner service seam for source aggro: invoked synchronously for
    // positive damage after result calculation and immediately before HitFor.
    // It must not mutate either borrowed PropertyView or CombatActorState.
    void (*before_hit)(void*, float) = nullptr;
    void* before_hit_context = nullptr;
    // Optional exact lethal callback from HitFor HP=0 to Character::Kill.
    // It runs while the player state still reports alive and must complete
    // the canonical kill prefix before returning.
    int (*before_dead)(void*, CombatResult&) = nullptr;
    void* before_dead_context = nullptr;
};
struct Exchange {
    CombatResult result;
    MonsterApplication application;
};
enum class Status : std::int32_t {
    complete, invalid_argument, calculation_failed, application_failed
};

// Source-order kernel for the bounded offline nonplayer -> player path:
// Character::F_MeleeAttack followed by F_ApplyResult. Damage and RNG are
// calculated first; the existing player-defender application then mutates
// owner properties/state. Offhand is the source argument; the alternate mode
// and its SnS_CleanUp buff side effect are outside this slice. Equipment/debug
// service production and F_ApplyResult's other external AI, FX, status, audio,
// reward, and full kill services remain separate; this is not live integration.
Status monster_to_player(const Request*, Exchange*);
} // namespace dh2::data::melee_pipeline
