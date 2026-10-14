#pragma once
// v21.8 w3-ally: the CharAI regeneration tick (Paladin PurifyingAura's Regen_HP / CombatRegen_HP cells were never read).
//
// Original (IDA export pseudocode-all.c):
//   CharAI::OnInit (0x3d12b0), OnRevived (0x3d0f1c), AI_ScriptInit (0x3cfde4): CharTimers::TMR_Start(CharacterDesign.AI_Tick,
//     -1 (repeats), event 51). AI_Tick = 3000 ms: design_pycst.bin row "AI_Tick" = 0x0bb8 (the DoT_Tick row beside it = 1000 ms).
//   CharAI::Update event 51 -> CharAI::_UpdateRegen (0x3cb77c): returns when the Character IsDead (vtable +84); otherwise
//     Character::RegenTick(AI_IsInCombat()) (0x3bdd90).
//   RegenTick(combat): RegenHP(CombatRegen_HP = cell 40), RegenMP(CombatRegen_MP = cell 45).
//     RegenTick(idle):   RegenHP(Regen_HP = cell 39),       RegenMP(Regen_MP = cell 44).
//   RegenHP / RegenMP (0x3bdca4 / 0x3bdbb8): a2 = amount; a2 cut to (maximum - current) when it would pass the maximum;
//     PROPS_Add(current cell, a2) when a2 > 0. Values are raw x256, as the sheet stores them.
//   AI_IsInCombat (0x3d4bc4) = AI_HasAggro || AI_IsAggroed || SM_IsAttacking || SM_IsUsingSkill || SM_IsCasting.
//     AI_HasAggro = the Character's own aggro list is non-empty (CharAI+140), AI_IsAggroed = someone holds aggro on it (CharAI+164).
//
// Port mapping (INFERENCE where marked):
//   - The tick clock is a per-actor accumulator over the frame dt, repeating every kAiTickMsV1 (one entry per due tick).
//   - AI_HasAggro: a non-player actor with a live target (monsters chase their target_id). The player's target_id is a soft
//     selection, not an aggro list, so the player's own target does not count (INFERENCE).
//   - AI_IsAggroed: some other live actor whose target_id is this actor.
//   - SM_IsAttacking / SM_IsUsingSkill / SM_IsCasting: CharacterAction attacking or casting.
//   - Dead actors are skipped (regen_vitals).
#include "../../playable_actor_world.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace dh::foundation::status_effects {

// CharacterDesign.AI_Tick in milliseconds (design_pycst.bin). The regen timer period.
inline constexpr double kAiTickMsV1 = 3000.0;
// CharacterProperties cells (the same indices the native RegenTick reads).
inline constexpr unsigned kRegenHpIdleCellV1 = 39, kRegenHpCombatCellV1 = 40;
inline constexpr unsigned kRegenMpIdleCellV1 = 44, kRegenMpCombatCellV1 = 45;

// AI_IsInCombat for a bound actor (see the header comment for the port mapping).
inline bool actor_in_combat_v1(const PlayableActorWorld& world, ActorId id) {
    const auto* actor = world.find_actor(id);
    if (!actor) return false;
    if (actor->action == CharacterAction::attacking || actor->action == CharacterAction::casting) return true;
    const auto* traits = world.traits(id);
    if (traits && !traits->is_player && actor->target_id != invalid_actor_id) {
        const auto* target = world.find_actor(actor->target_id);
        if (target && target->alive()) return true;
    }
    for (const auto& [other_id, other] : world.actors()) {
        if (other_id != id && other.target_id == id && other.alive()) return true;
    }
    return false;
}

// Advances every bound actor's regen clock by dt_ms and returns one entry per due AI tick, in actor order.
// Clock entries of actors that left the world are dropped.
inline std::vector<ActorId> advance_vital_regen_clock_v1(std::map<ActorId, double>& clock,
                                                         const PlayableActorWorld& world, double dt_ms) {
    std::vector<ActorId> due;
    for (auto it = clock.begin(); it != clock.end();) {
        if (!world.find_actor(it->first)) it = clock.erase(it);
        else ++it;
    }
    for (const auto& [id, actor] : world.actors()) {
        (void)actor;
        double& elapsed = clock[id];
        elapsed += dt_ms;
        while (elapsed >= kAiTickMsV1) {
            elapsed -= kAiTickMsV1;
            due.push_back(id);
        }
    }
    return due;
}

// One Character::RegenTick for a live actor: the idle or combat cells, applied through PlayableActorWorld::regen_vitals.
inline bool regen_tick_v1(PlayableActorWorld& world, ActorId id, std::string& error) {
    const auto* properties = world.combat_properties(id);
    if (!properties) {
        error = "RegenTick requires the actor's source property sheet";
        return false;
    }
    const bool combat = actor_in_combat_v1(world, id);
    const auto& resolved = properties->sheets.resolved;
    const std::int32_t hp = resolved[combat ? kRegenHpCombatCellV1 : kRegenHpIdleCellV1];
    const std::int32_t mp = resolved[combat ? kRegenMpCombatCellV1 : kRegenMpIdleCellV1];
    return world.regen_vitals(id, hp, mp, error);
}

} // namespace dh::foundation::status_effects
