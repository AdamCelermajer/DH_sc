#pragma once

#include "../../combat_session.hpp"
#include "../../../game-data/animation_tables.hpp"

#include <functional>

namespace dh::foundation::combat {

// Original states that register event 50011 (SM_SetKnockBackState -> KnockedBack10), from the CharStateMachine OnInit registrations
// (CSSpawn 0x3c7d0c, CSIdle 0x3c7e60, CSMove 0x3c80ac, CSAttack 0x3c8284, CSSkill 0x3c8438, CSScared 0x3c861c, CSStunned 0x3c8730,
// CSInjured 0x3c8850, CSInteract 0x3c8984, CSReviving 0x3c8b8c, CSLiftingIdle 0x3c8d5c, CSLiftingMove 0x3c8ee0). Cast7, Despawn2,
// Dead12, Anim14, Revived16, PreSpawn17, Limbus0 and KnockedBack10 itself do not register it. Attack5/Skill6 register it with the
// CSM_Interrupted 0x3ad244 condition (a character flag). The table is defined in combat_session.cpp (w3-status) and is the same
// one CombatSession::play_actor_source_sequence uses for the non-direct admission.
bool source_knockback_event_accepted_v1(std::int32_t original_state) noexcept;

// Capture before CombatSession::apply_source_result: its normal Injury path
// runs inside that call. The original F_ApplyResult gates the status tail
// (stun, scare, knockback) on Character::IsDead only (w3-status: vtable +52 at
// 0x3b01b8, not IsPlayer); the pre-hit state decides only whether event 50011
// is admitted (see source_knockback_event_accepted_v1).
struct SessionPushAdmissionV1 {
    SessionPushAdmissionV1() = default;
    SessionPushAdmissionV1(const SessionPushAdmissionV1&) = delete;
    SessionPushAdmissionV1& operator=(const SessionPushAdmissionV1&) = delete;
    SessionPushAdmissionV1(SessionPushAdmissionV1&&) noexcept = default;
    SessionPushAdmissionV1& operator=(SessionPushAdmissionV1&&) noexcept = default;

    ActorId attacker = invalid_actor_id;
    ActorId target = invalid_actor_id;
    std::weak_ptr<const void> binding_lease;
    std::uint64_t generation = 0;
    std::uint32_t event_index = 0;
    std::string source_id;
    std::string marker_name;
    std::int32_t target_state_before_hit = -1;
    bool target_was_player = false;
    bool target_was_alive = false;
    bool source_push_gate_admitted = false;
    bool consumed = false;
};

struct SessionPushRequestV1 {
    ActorId attacker = invalid_actor_id;
    ActorId target = invalid_actor_id;
    std::uint64_t generation = 0;
    std::uint32_t event_index = 0;
    std::string source_id;
    std::string marker_name;
    std::uint32_t source_outcomes = 0;
    std::uint32_t source_mask = 0;
    // Source F_ApplyResult: mask bit0x100000 selects GreatKnockedBack;
    // bits0x18000000 select direct FSM transition instead of event50011.
    bool great = false;
    bool direct = false;
    // Source FSM effects are Kill-related session work; no impulse is supplied.
    std::int32_t target_state_before_hit = -1;
};

// Explicit two-root visual bank from one actor's resolved source CharAnimTable.
// The caller must append `plan.config.clips` to that actor's Session visual
// before initialize; this bank does not load or replace a live pose owner.
struct SessionPushAnimationBankV1 {
    OriginalCombatVisualPlan plan;
    OriginalSequencePolicies policies;
    std::string normal_state="KnockedBack";
    std::string great_state="GreatKnockedBack";
    std::int32_t normal_sequence=-1;
    std::int32_t great_sequence=-1;
    std::int32_t stance=0;
    std::uint32_t stanced_animation_mask=0;
};

using SessionPushEffectSinkV1 = std::function<bool(
    CombatSession&, const SessionPushRequestV1&, std::string&)>;

// Common incoming consumer: before Injury snapshot / after Injury Push delivery
// for BOTH authored normal melee markers and explicit calculated source hits.
// The Session owns occurrence retirement and actor lease; the sink retains its
// actual per-actor preinitialized source bank and physical service owner.
bool bind_session_push_effect_v1(CombatSession&,SessionPushEffectSinkV1,std::string& error);

bool capture_session_push_admission_v1(CombatSession&,
    const CombatSessionSourceHit&, SessionPushAdmissionV1&, std::string& error);

// Returns success for a source no-op. `consumed` distinguishes a delivered
// Push request from a source-rejected/no-Push result. A reached Push with no
// same-Session effect provider is an error, never a silent success.
bool consume_session_push_result_v1(CombatSession&,
    SessionPushAdmissionV1&, const DamageEvent&,
    const SessionPushEffectSinkV1&, bool& consumed, std::string& error);

// Build the available source field-16/field-8 sequences from the explicit
// animation-table row, stance and AnimStancedAnim mask supplied by the
// actor/profile owner. An absent source row remains a source no-op for that
// variant; no class, stance, mask or table row is inferred here.
bool build_session_push_animation_bank_v1(const AssetCatalog&,
    const dh2::data::AnimationTables&,const dh2::data::Dictionary&,
    std::int32_t target_animation_table,std::int32_t stance,
    std::uint32_t stanced_animation_mask,const CharacterVisualConfig&,
    const std::string& role,
    SessionPushAnimationBankV1&,std::string& error);

// Start the authored knockback root on the existing same-Session retained pose
// owner. Root motion is consumed by that actor's configured Session motion
// owner during update; no impulse/distance is fabricated here.
bool play_session_push_animation_v1(CombatSession&,const SessionPushRequestV1&,
    const SessionPushAnimationBankV1&,CombatSessionStateAnimationServices,
    std::string& error);

} // namespace dh::foundation::combat
