#pragma once
// v21.8 ES-STATUS: monster/actor status states driven by skill outcomes and Lua calls.
//
// Original (IDA export libDungeonHunter2.so/pseudocode-all.c, read in this pass):
//   Character::_SetStunState 0x3b9580 / _SetScareState 0x3b96b0 / _SetKnockBackState 0x3b7680 (Lua entry points; the Lua
//     arguments are (duration_ms, flag), (duration_ms, flag), (great) with defaults flag=true, great=true).
//   CharStateMachine::SM_SetStunState 0x3c5ffc, SM_SetScareState 0x3c6144, SM_SetKnockBackState 0x3c5ea0:
//     - return at once when Character::IsBoss (0x3a3158) is set; no-op when the animation table row is absent;
//     - stun/scare: start TMR type 43/44 for the duration only when the stunned/scared bit is not set yet (no restart),
//       set the bit (sm+44 bit 0x2 / 0x4), set sm+36 bit 0x800 / 0x400 when the persist flag is set, then enter the
//       state (immediate _SetState) or raise event 50012 / 50013 / 50011 (RaiseStateEvent: only a state that registers
//       the event moves). Knockback starts no timer; the great bank is selected by the bool.
//   CharStateMachine::RaiseStateEvent 0x3c5684: events 42/43/44 clear sm+44 bits 0x1/0x2/0x4.
//   CharTimers::Update 0x3db640: a timer fires after its interval; interval 0 never fires (the loop breaks on !interval).
//   CharStateMachine::Update 0x3c628c: while the stunned bit is set and the state is not Stunned, the stun is re-raised
//     with the same persist flag (no timer restart); the same for scared.
//   Character::F_ApplyResult 0x3b10b4 tail: outcome 0x80 -> SM_SetKnockBackState(great = mask 0x100000, immediate =
//     mask 0x18000000 != 0); outcome 0x40 -> SM_SetStunState(property 140 or 185 (mask 0x1000) >> 8, persist, not
//     immediate); outcome 0x20 -> SM_SetScareState(property 143 or 187 (mask 0x4000) >> 8, only when non-zero, persist,
//     immediate = mask 0x18000000 != 0); outcome bit 0x100 (slow) is handled by runtime_timed_buff_v1 (unchanged).
//   Character::F_ApplyResult does NOT gate that tail on the defender being a Player: the dispatch after the
//     Player/Idle block ("if ((v11&2)) goto LABEL_92; goto LABEL_20;") is common to both paths. The Player/Idle block only
//     sets the dodge/injure bits. Verified from the decompiled goto structure; a second reader should confirm.
//
// The event acceptance table below is the CharStateMachine OnInit registrations (CSSpawn, CSIdle, CSMove, CSAttack,
// CSSkill, CSScared, CSStunned, CSInteract, CSReviving, CSLiftingIdle, CSLiftingMove; CSCast/Injured/KnockedBack as noted).
// State numbers proven in IDA: Idle 3, Move 4, Attack 5, Skill 6, Scared 8, Stunned 9, KnockedBack 10.
//
// Not modelled here (kept as explicit gaps): the anim clip playback itself (the caller supplies anim_ok), the
// KnockedBack state end (the original ends it through the clip, not a timer), and the Stunned->Idle exit when the stun
// timer fires (no 43 registration was found; INFERENCE: the port returns to Idle when the timer clears).
#include "../../actor_state.hpp"

#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <vector>

namespace dh::foundation::status_states {

// Original state classes that matter for status transitions (CSxxx OnInit). Other ids map to other.
enum class StatusStateV1 : std::uint8_t {
    spawn, idle, move, attack, skill, cast, scared, stunned, knocked_back, injured,
    interact, reviving, lifting_idle, lifting_move, other
};

inline constexpr std::int32_t kEventKnockBackV1 = 50011; // -> KnockedBack (10)
inline constexpr std::int32_t kEventStunV1 = 50012;      // -> Stunned (9)
inline constexpr std::int32_t kEventScareV1 = 50013;     // -> Scared (8)
inline constexpr std::int32_t kEventStunTimerEndV1 = 43; // TMR type 43 expiry: clears sm+44 bit 0x2
inline constexpr std::int32_t kEventScareTimerEndV1 = 44; // TMR type 44 expiry: clears bit 0x4; CSScared 44 -> Idle

// RaiseStateEvent acceptance from the original OnInit registrations (see header comment).
bool accepts_event_v1(StatusStateV1 state, std::int32_t event) noexcept;

// CharAI flag word (+20): bit 2 = IsBoss (0x3a3158), bit 1 = IsMiniBoss (0x3a3144). Source word = AiProps.flags
// (INFERENCE: the port's AI row word is the CharAI word; not yet proven by a reader of CharAI+20).
struct BossFlagsV1 {
    bool boss = false;
    bool mini_boss = false;
};
BossFlagsV1 boss_flags_from_ai_word_v1(std::uint32_t word) noexcept;

// Per-actor state (the sm+36 / sm+44 bits the original tests, plus timers).
struct StatusActorV1 {
    StatusStateV1 state = StatusStateV1::idle;
    bool stun_bit = false;          // sm+44 bit 0x2 (stun timer active)
    bool scare_bit = false;         // sm+44 bit 0x4 (scare timer active)
    bool stun_persist = false;      // sm+36 bit 0x800
    bool scare_persist = false;     // sm+36 bit 0x400
    bool knockback_great = false;   // bank selected by the bool passed to SM_SetKnockBackState
    double stun_left_ms = 0.0;      // +inf when the duration is 0 (CharTimers never fires an interval of 0)
    double scare_left_ms = 0.0;
};

// Outcome tail computed from the F_ApplyResult bits and the attacker's resolved sheet.
struct StatusOutcomePlanV1 {
    bool knockback = false;
    bool knockback_great = false;
    bool knockback_immediate = false;
    bool stun = false;
    double stun_ms = 0.0;
    bool scare = false;
    double scare_ms = 0.0;
    bool scare_immediate = false;
};

// `attacker_property(id)` returns the resolved raw property (x256 fixed point, the value F_ApplyResult reads).
StatusOutcomePlanV1 status_outcome_plan_v1(std::uint32_t outcomes, std::uint32_t mask,
                                           const std::function<std::int32_t(int)>& attacker_property);

// Owner of status states for any set of actors (monsters and characters). Keyed by ActorId; no session dependency,
// so the Session/AI layers call it and read can_act() before choosing an action.
class StatusStateOwnerV1 {
public:
    // Lua SetStunState(duration_ms, flag). Returns false when the call is a no-op (boss or missing anim row).
    bool set_stun(ActorId actor, double duration_ms, bool persist, bool immediate, BossFlagsV1 flags, bool anim_ok);
    // Lua SetScareState(duration_ms, flag).
    bool set_scare(ActorId actor, double duration_ms, bool persist, bool immediate, BossFlagsV1 flags, bool anim_ok);
    // Lua SetKnockBackState(great). Starts no timer; the caller ends the state with finish_knockback.
    bool set_knockback(ActorId actor, bool great, bool immediate, BossFlagsV1 flags, bool anim_ok);
    // Applies the F_ApplyResult tail in its native order: knockback, stun, scare.
    bool apply_outcome(ActorId actor, const StatusOutcomePlanV1& plan, BossFlagsV1 flags, bool anim_ok);
    // Ages every timer by dt_ms; expiry clears the bit (and returns a stunned actor to Idle), then the native re-raise.
    void advance(double dt_ms);
    // Ends a knockback when its animation completes (the clip is the original end signal).
    bool finish_knockback(ActorId actor);
    void remove_actor(ActorId actor);

    bool can_act(ActorId actor) const;
    StatusStateV1 state(ActorId actor) const;
    bool stunned(ActorId actor) const;
    bool scared(ActorId actor) const;
    double stun_left_ms(ActorId actor) const;
    double scare_left_ms(ActorId actor) const;

    std::vector<std::string> take_log();

private:
    StatusActorV1& actor(ActorId id);
    void enter(StatusActorV1&, StatusStateV1 next, const char* reason, ActorId id);
    std::map<ActorId, StatusActorV1> actors_;
    std::vector<std::string> log_;
};

} // namespace dh::foundation::status_states
