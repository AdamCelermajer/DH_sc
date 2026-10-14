// v21.8 ES-STATUS: unit tests for the status-state owner (stun / scare / knockback), the F_ApplyResult outcome tail,
// the boss gate, the event acceptance table, and one integration path through the real combat roll
// (dh2_combat_result 0x40 stun bit -> outcome plan -> owner -> can_act), with a ViciousStrike-style stun mask.
#include "status_states_v1.hpp"

#include "../../../game-data/combat.hpp"
#include "../../../game-data/combat_result.hpp"

#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>

namespace {
using namespace dh::foundation::status_states;

int failures = 0;
void check(bool ok, const char* what) {
    if (!ok) {
        ++failures;
        std::fprintf(stderr, "FAIL: %s\n", what);
    }
}

constexpr dh::foundation::ActorId kMonster = 4001;
constexpr dh::foundation::ActorId kOther = 4002;
const BossFlagsV1 kNormal{};
const BossFlagsV1 kBoss{true, false};

void test_boss_word() {
    check(!boss_flags_from_ai_word_v1(0).boss && !boss_flags_from_ai_word_v1(0).mini_boss, "word 0 has no boss bits");
    check(boss_flags_from_ai_word_v1(4).boss && !boss_flags_from_ai_word_v1(4).mini_boss, "bit 2 is IsBoss");
    check(!boss_flags_from_ai_word_v1(2).boss && boss_flags_from_ai_word_v1(2).mini_boss, "bit 1 is IsMiniBoss");
}

void test_event_table() {
    check(accepts_event_v1(StatusStateV1::idle, kEventStunV1), "Idle accepts stun event 50012");
    check(accepts_event_v1(StatusStateV1::idle, kEventScareV1), "Idle accepts scare event 50013");
    check(accepts_event_v1(StatusStateV1::idle, kEventKnockBackV1), "Idle accepts knockback event 50011");
    check(!accepts_event_v1(StatusStateV1::stunned, kEventStunV1), "Stunned does not accept 50012 (CSStunned registers 50011 only)");
    check(!accepts_event_v1(StatusStateV1::scared, kEventScareV1), "Scared does not accept 50013 (CSScared registers 50011/50012)");
    check(accepts_event_v1(StatusStateV1::scared, kEventStunV1), "Scared accepts 50012");
    check(!accepts_event_v1(StatusStateV1::cast, kEventStunV1) && !accepts_event_v1(StatusStateV1::cast, kEventKnockBackV1),
          "Cast registers none of the status events");
    check(!accepts_event_v1(StatusStateV1::knocked_back, kEventKnockBackV1), "KnockedBack does not re-accept 50011");
}

void test_outcome_plan() {
    // Property values are raw x256: 3000 ms is 3000 << 8.
    auto props = [](int id) -> std::int32_t {
        switch (id) {
        case 140: return 3000 << 8;
        case 185: return 1500 << 8;
        case 143: return 2000 << 8;
        case 187: return 0;      // zero scare duration: no scare call
        default: return 0;
        }
    };
    const auto stun = status_outcome_plan_v1(0x40, 0, props);
    check(stun.stun && stun.stun_ms == 3000.0, "0x40 without 0x1000 reads property 140");
    check(!stun.scare && !stun.knockback, "0x40 alone sets only stun");
    const auto stun_alt = status_outcome_plan_v1(0x40, 0x1000, props);
    check(stun_alt.stun && stun_alt.stun_ms == 1500.0, "mask 0x1000 selects property 185");
    const auto scare = status_outcome_plan_v1(0x20, 0, props);
    check(scare.scare && scare.scare_ms == 2000.0 && !scare.scare_immediate, "0x20 reads property 143, not immediate");
    const auto scare_alt = status_outcome_plan_v1(0x20, 0x4000 | 0x18000000u, props);
    check(!scare_alt.scare && !scare_alt.stun, "mask 0x4000 selects property 187 (zero) so no scare");
    const auto kb = status_outcome_plan_v1(0x80, 0x100000, props);
    check(kb.knockback && kb.knockback_great && !kb.knockback_immediate, "0x80 with 0x100000 is great, not immediate");
    const auto kb_direct = status_outcome_plan_v1(0x80, 0x8000000, props);
    check(kb_direct.knockback && !kb_direct.knockback_great && kb_direct.knockback_immediate, "0x8000000 makes knockback immediate");
    const auto none = status_outcome_plan_v1(0x100, 0, props);
    check(!none.knockback && !none.stun && !none.scare, "slow 0x100 is not a status-state outcome here");
}

void test_stun_timer_and_gate() {
    StatusStateOwnerV1 owner;
    check(owner.can_act(kMonster), "unknown actor can act");
    check(owner.set_stun(kMonster, 3000, true, false, kNormal, true), "stun applies to a non-boss");
    check(owner.stunned(kMonster) && owner.state(kMonster) == StatusStateV1::stunned, "stun enters Stunned from Idle");
    check(!owner.can_act(kMonster), "stunned monster cannot act");
    owner.advance(1000.0);
    check(owner.stunned(kMonster) && std::fabs(owner.stun_left_ms(kMonster) - 2000.0) < 1e-9, "stun runs for its duration");
    owner.advance(2000.0);
    check(!owner.stunned(kMonster) && owner.state(kMonster) == StatusStateV1::idle, "stun ends and returns to Idle");
    check(owner.can_act(kMonster), "monster can act after the stun");
}

void test_stun_no_restart_and_boss_immunity() {
    StatusStateOwnerV1 owner;
    owner.set_stun(kMonster, 3000, true, false, kNormal, true);
    owner.advance(1000.0);
    owner.set_stun(kMonster, 5000, true, false, kNormal, true);
    check(std::fabs(owner.stun_left_ms(kMonster) - 2000.0) < 1e-9, "re-stun while stunned does not restart the timer");

    StatusStateOwnerV1 boss;
    check(!boss.set_stun(kMonster, 3000, true, false, kBoss, true), "boss refuses stun");
    check(!boss.stunned(kMonster) && boss.can_act(kMonster), "boss is not stunned");
    check(!owner.set_stun(kOther, 3000, true, false, kNormal, false) && !owner.stunned(kOther),
          "missing animation row is a no-op");
}

void test_stun_events_follow_registrations() {
    StatusStateOwnerV1 owner;
    owner.set_knockback(kMonster, false, true, kNormal, true); // immediate -> KnockedBack
    check(owner.state(kMonster) == StatusStateV1::knocked_back, "immediate knockback enters KnockedBack");
    owner.set_stun(kMonster, 3000, true, false, kNormal, true); // event 50012 not accepted by KnockedBack
    check(owner.stunned(kMonster) && owner.state(kMonster) == StatusStateV1::knocked_back,
          "stun bit sets but the state stays KnockedBack when the event is not registered");
    check(!owner.can_act(kMonster), "stun bit alone blocks actions");
}

void test_update_reraise() {
    StatusStateOwnerV1 owner;
    owner.set_stun(kMonster, 3000, true, false, kNormal, true);  // state Stunned, bit set
    owner.set_scare(kMonster, 0, false, true, kNormal, true);    // immediate Scared interrupts the stun
    check(owner.state(kMonster) == StatusStateV1::scared, "immediate scare interrupts the stun state");
    owner.advance(0.0);
    check(owner.state(kMonster) == StatusStateV1::stunned, "active stun re-raises while Scared accepts 50012");
}

void test_zero_duration_never_expires() {
    // CharTimers::Update: an interval of 0 never fires; the port models it as a stun that does not end on its own.
    StatusStateOwnerV1 owner;
    owner.set_stun(kMonster, 0, true, false, kNormal, true);
    owner.advance(1.0e9);
    check(owner.stunned(kMonster), "zero-duration stun does not expire by timer");
}

void test_scare_and_knockback() {
    StatusStateOwnerV1 owner;
    check(owner.set_scare(kMonster, 1000, true, false, kNormal, true), "scare applies");
    check(owner.scared(kMonster) && owner.state(kMonster) == StatusStateV1::scared, "scare enters Scared from Idle");
    owner.advance(1000.0);
    check(!owner.scared(kMonster) && owner.state(kMonster) == StatusStateV1::idle, "scare timer end (event 44) returns to Idle");

    check(owner.set_knockback(kMonster, true, false, kNormal, true), "knockback applies");
    check(owner.state(kMonster) == StatusStateV1::knocked_back && !owner.can_act(kMonster), "knockback enters KnockedBack");
    check(owner.finish_knockback(kMonster) && owner.state(kMonster) == StatusStateV1::idle, "knockback clip end returns to Idle");
    check(!owner.finish_knockback(kMonster), "finish_knockback is a no-op outside KnockedBack");

    StatusStateOwnerV1 boss;
    check(!boss.set_knockback(kMonster, true, true, kBoss, true) && boss.state(kMonster) == StatusStateV1::idle,
          "boss refuses knockback");
    check(!boss.set_scare(kMonster, 1000, true, true, kBoss, true) && !boss.scared(kMonster), "boss refuses scare");
}

void test_apply_outcome_order() {
    StatusStateOwnerV1 owner;
    StatusOutcomePlanV1 plan;
    plan.knockback = true;
    plan.knockback_great = false;
    plan.knockback_immediate = false;
    plan.stun = true;
    plan.stun_ms = 1000.0;
    plan.scare = true;
    plan.scare_ms = 500.0;
    owner.apply_outcome(kMonster, plan, kNormal, true);
    // Knockback first (Idle accepts 50011 -> KnockedBack), then stun (KnockedBack does not accept 50012), then scare.
    check(owner.state(kMonster) == StatusStateV1::knocked_back, "outcome tail applies knockback first");
    check(owner.stunned(kMonster) && owner.scared(kMonster), "outcome tail sets stun and scare bits");

    StatusStateOwnerV1 boss;
    boss.apply_outcome(kMonster, plan, kBoss, true);
    check(!boss.stunned(kMonster) && !boss.scared(kMonster) && boss.state(kMonster) == StatusStateV1::idle,
          "boss immune to the whole outcome tail");
}

void test_integration_vicious_strike_stun() {
    // Real original roll (dh2_combat_result, F_CalculateResult port). Stun chance raw 25600 = 100% (roll is percent << 8).
    std::array<std::int32_t, 224> attacker{}, defender{};
    attacker[139] = 25600;          // attacker stun chance (normal, property 139)
    attacker[140] = 3000 << 8;      // stun duration (raw x256 ms)
    attacker[19] = 0;               // level terms cancel
    defender[19] = 0;
    dh2::data::CombatantView av{attacker.data()};
    dh2::data::CombatantView dv{defender.data()};
    dh2::data::CombatRandom random{1, 0};
    dh2::data::CombatResultRequest request{&av, &dv, &random, 0x800u, -1, -1, 0};
    dh2::data::CombatResult result{};
    check(dh2_combat_result(&result, &request) == 0, "roll accepts the stun mask");
    check(result.outcomes == 0x40u, "100% stun chance yields exactly the stun bit 0x40");

    const auto plan = status_outcome_plan_v1(result.outcomes, request.mask,
        [&](int id) { return attacker[static_cast<std::size_t>(id)]; });
    check(plan.stun && plan.stun_ms == 3000.0, "stun duration comes from property 140");

    StatusStateOwnerV1 owner;
    owner.apply_outcome(kMonster, plan, kNormal, true);
    check(!owner.can_act(kMonster) && owner.state(kMonster) == StatusStateV1::stunned, "stun outcome stops the monster");
    owner.advance(2999.0);
    check(!owner.can_act(kMonster), "monster still stunned before the duration ends");
    owner.advance(1.0);
    check(owner.can_act(kMonster), "monster acts again after the duration");

    // Zero chance: the same mask produces no stun bit and changes nothing.
    attacker[139] = 0;
    dh2::data::CombatRandom random2{1, 0};
    dh2::data::CombatResultRequest none{&av, &dv, &random2, 0x800u, -1, -1, 0};
    dh2::data::CombatResult none_result{};
    check(dh2_combat_result(&none_result, &none) == 0 && (none_result.outcomes & 0x40u) == 0, "0% chance rolls no stun");
    const auto none_plan = status_outcome_plan_v1(none_result.outcomes, none.mask,
        [&](int id) { return attacker[static_cast<std::size_t>(id)]; });
    StatusStateOwnerV1 unaffected;
    unaffected.apply_outcome(kMonster, none_plan, kNormal, true);
    check(unaffected.can_act(kMonster), "no outcome bit leaves the monster free");

    // Boss immunity on the real roll path.
    attacker[139] = 25600;
    dh2::data::CombatRandom random3{1, 0};
    dh2::data::CombatResultRequest boss_request{&av, &dv, &random3, 0x800u, -1, -1, 0};
    dh2::data::CombatResult boss_result{};
    dh2_combat_result(&boss_result, &boss_request);
    const auto boss_plan = status_outcome_plan_v1(boss_result.outcomes, boss_request.mask,
        [&](int id) { return attacker[static_cast<std::size_t>(id)]; });
    StatusStateOwnerV1 boss;
    boss.apply_outcome(kMonster, boss_plan, kBoss, true);
    check(boss_result.outcomes == 0x40u && boss.can_act(kMonster), "a boss rolls the stun bit but takes no stun");
}

} // namespace

void test_apply_without_knockback() {
    // Knockback (0x80) is left to the Push consumer; stun (0x40) and scare (0x20) go to the owner with the boss gate.
    auto props = [](int id) -> std::int32_t {
        switch (id) {
        case 140: return 2000 << 8;
        case 143: return 1000 << 8;
        default: return 0;
        }
    };
    StatusStateOwnerV1 owner;
    const auto plan = apply_status_outcome_without_knockback_v1(owner, kMonster, 0xC0u, 0, props, kNormal);
    check(!plan.knockback && plan.stun && plan.stun_ms == 2000.0, "0xC0 applies stun only, not knockback");
    check(owner.stunned(kMonster) && owner.state(kMonster) == StatusStateV1::stunned, "stun outcome stops the monster");
    check(owner.state(kMonster) != StatusStateV1::knocked_back, "no knockback state from this entry point");
    StatusStateOwnerV1 boss_owner;
    apply_status_outcome_without_knockback_v1(boss_owner, kMonster, 0x60u, 0, props, kBoss);
    check(!boss_owner.stunned(kMonster) && !boss_owner.scared(kMonster), "boss takes no stun or scare from the outcome");
    StatusStateOwnerV1 none;
    apply_status_outcome_without_knockback_v1(none, kMonster, 0x100u, 0, props, kNormal);
    check(none.can_act(kMonster), "slow alone does not stop the monster here");
}

// w3-pose: with external state authority the owner keeps the bits, timers and intent flags only; the host mirrors the real state.
void test_external_state_authority() {
    StatusStateOwnerV1 owner;
    owner.set_external_state_authority(true);
    check(owner.external_state_authority(), "external authority flag is stored");
    check(owner.set_stun(kMonster, 500.0, true, false, kNormal, true), "stun accepted in external mode");
    check(owner.stunned(kMonster) && owner.state(kMonster) == StatusStateV1::idle, "external mode never switches the internal state");
    check(!owner.can_act(kMonster), "the bit alone blocks decisions");
    check(!owner.stun_forced(kMonster), "an event-path stun is not forced");
    check(owner.take_log().empty(), "external mode logs no internal state change");
    check(owner.set_stun(kOther, 500.0, true, true, kNormal, true) && owner.stun_forced(kOther), "an immediate stun records the forced flag");
    owner.clear_forced(kOther, true);
    check(!owner.stun_forced(kOther), "the host consumes the forced flag");
    check(!owner.pose_refused(kMonster, true), "no refusal by default");
    owner.refuse_pose(kMonster, true);
    check(owner.pose_refused(kMonster, true) && !owner.pose_refused(kMonster, false), "a refusal is per kind");
    owner.sync_state(kMonster, StatusStateV1::stunned);
    check(owner.state(kMonster) == StatusStateV1::stunned, "sync_state mirrors the real state");
    check(owner.tracked_actors().size() == 2, "both actors are tracked");
    owner.advance(300.0);
    check(owner.stunned(kMonster), "still stunned before the duration ends");
    owner.advance(300.0);
    check(!owner.stunned(kMonster) && !owner.stun_forced(kOther) && !owner.pose_refused(kMonster, true),
          "expiry clears the bit, the forced flag and the refusal");
    check(owner.state(kMonster) == StatusStateV1::stunned, "the host, not the owner, ends the Stunned state (CSStunned::OnUpdate)");
    owner.sync_state(kMonster, StatusStateV1::idle);
    check(owner.can_act(kMonster), "actor acts again once the real state is Idle and the bit is clear");
    owner.set_scare(kMonster, 400.0, true, false, kNormal, true);
    check(owner.scared(kMonster) && !owner.scare_forced(kMonster), "scare bit without forced flag");
    owner.remove_actor(kMonster);
    check(!owner.scared(kMonster), "remove_actor drops the bits");
}

int main() {
    test_apply_without_knockback();
    test_boss_word();
    test_event_table();
    test_outcome_plan();
    test_stun_timer_and_gate();
    test_stun_no_restart_and_boss_immunity();
    test_stun_events_follow_registrations();
    test_update_reraise();
    test_zero_duration_never_expires();
    test_scare_and_knockback();
    test_apply_outcome_order();
    test_integration_vicious_strike_stun();
    test_external_state_authority();
    if (failures == 0) std::printf("status_states_v1 tests passed\n");
    return failures == 0 ? 0 : 1;
}
