#pragma once
#include "../../original_combat_visual_plan.hpp"
#include "../../../level-world/character_attack_animation_v1.hpp"
#include "../../../game-data/animation_scheduler.hpp"
#include "../../retained_sequence_playback.hpp"
#include <functional>

namespace dh::foundation::combo {
struct Operation;
// Caller supplies the SAME source animator cursor and SAME live AI projection.
// This module stores no attack, cooldown, input latch, or animation clock.
struct Boundary {
    std::uint32_t depth=0,step=0,count=0;
    std::uintptr_t lookAt=0;
    bool hasCombo=false;
    std::function<bool()> canRange;
    std::function<bool(std::uintptr_t)> targetDead;
    // Reached operations execute synchronously when supplied, preserving source
    // reentry. Effects also retain the same operation order for diagnostics.
    std::function<bool(dh2::character::AttackState64&,const Operation&)> onOperation;
};
struct Operation {
    dh2::character::AttackAnimationServiceV1 service;
    std::uint32_t value=0;
    std::uintptr_t subject=0;
};
struct BoundaryEffects {std::vector<Operation> operations;};
bool begin(dh2::character::AttackState64&,const Boundary&,BoundaryEffects&,std::string& error);
bool end(dh2::character::AttackState64&,const Boundary&,BoundaryEffects&,std::string& error);
// Device policy belongs to the caller: true means an actual command was
// delivered. Repeated held commands and individual presses use the same source
// continuation acceptance; commands during last/pre/recovery are not buffered.
bool command(dh2::character::AttackState64&,bool delivered,std::uintptr_t requestedTarget,
             const dh2::character::AttackServices16&,std::string& error);
bool controller_command(dh2::character::ControllerAttackState32&,dh2::character::AttackState64&,
                        bool delivered,std::uintptr_t requestedTarget,
                        const dh2::character::AttackServices16&,std::string& error);
// Maps actual retained hierarchy metadata into the exact source callbacks and
// converts their SetStep/SkipNext outputs to this same owner's cursor decision.
bool retained_boundary(dh2::character::AttackState64&,const RetainedSequenceBoundary&,
                       Boundary liveProviders,BoundaryEffects&,
                       RetainedSequenceCursorDecision&,std::string& error);
// Applies recorded source SetStep/SkipNext to the SAME caller animator cursor.
// Caller dispatches all other recorded operations to original owners in order.
bool apply_cursor_operations(const BoundaryEffects&,dh2::data::AnimationFrame&,std::string& error);
struct GroupAdvance {std::uint32_t completion=2,nextStep=0;std::int32_t loops=0;};
// Uses original live finite/type/loop completion after OnAnimStepEnd, including
// source cursor overrides to count (stop) and skipped recovery indices.
bool advance_group(std::int32_t sourceType,std::uint32_t sourceCount,
                   std::uint32_t sameStep,std::int32_t sameLoops,
                   const BoundaryEffects&,GroupAdvance&,std::string& error);
// Type2 choices consume the existing original AnimationRandom stream; type1
// begins at zero. No arbitrary cycling, CombatRandom substitution, or new seed.
bool select_start(const dh2::data::AnimationTables&,std::int32_t sourceSequence,
                  dh2::data::AnimationRandom&,dh2::data::AnimationStart&,
                  std::string& error,bool originalRandomEnabled=true);
}
