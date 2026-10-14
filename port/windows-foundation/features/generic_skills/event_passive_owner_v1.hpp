#pragma once
// v21.8 es-passive: event passives fed by the OnTargetHit bus (the Lua StealAIEvent scripts of the player's passives).
//
// Implemented from the original scripts (plain-text Lua, read directly):
//   prince_berseeker_killing_frenzy.luac: OnTargetHit with attacker == self. counter 1..5 builds the buff
//     Skill_Berserk_KillingFrenzy with SnS_Level = counter x rank; a 1500 ms reset timer removes the buff (the counter is
//     kept, the next hit with no buff restarts at 1); at counter 5 the buff is rebuilt at 5 x rank with a 5000 ms timer.
//   prince_necromancer_carrioneater.luac: OnTargetHit with attacker == self and defender dead: heal TempProp1 of
//     Skill_Necromancer_CarrionEater at the rank (clamped to max HP).
//   prince_paladin_shield.luac (w2-paladin): OnTargetHit with defender == self and the attacker in melee range (radius
//     reach, INFERENCE as Putrid). Two GetRand(0,100) rolls: SnS_Push_Target_Chance (knockback, SetKnockBackState(false):
//     no displacement in the shipped Lua, logged only) and SnS_Stun_Target_Chance (stun): a 500 ms timer then
//     SetStunState(SnS_Stun_Duration = 3000 ms, true) through the session's script status service (bosses refused).
//     Simplified: one tick (the Lua repeats until Stunned); the Lua Stop() on a non-Attack state is not called.
//   prince_paladin_holy_weapon.luac (w2-paladin): OnTargetHit with defender == self: the attacker loses TempProp1 of
//     Skill_Paladin_HolyWeapon (2 x rank) HP, clamped at 0; at 0 the attacker is killed. The Lua SetProp/ApplyPropClass on the
//     attacker's own sheet (TempProp1 only) is not applied.
//   prince_berseeker_killing_streak.luac (v21.8 berserker): OnTargetHit with attacker == self and defender dead: two buffs
//     for bufftime = SnS_Cooldown of Skill_Berserk_KillingStreak at the rank (class projection, 4000 + 500 x L ms):
//     Skill_Berserk_KillingStreak (capacity 128, the Lua `true`) and its dummy Skill_Berserk_KillingStreak_dummy (capacity 1).
//     The Lua PlayFX(self) and PlaySound("sfx_passive_skill_activate_2") are NOT dispatched here (presentation gap).
//   prince_berseeker_survival.luac (v21.8 berserker): OnTargetHit without an attacker guard. While HP% < 35 and no Survival
//     flag is set: SetProp(SnS_Level, L), CreateBuff(Skill_Berserk_Survival, 8000, false, L), the flag lasts 8000 ms (the Lua
//     StopBuffTimer clears only the flag, not the buff); at HP% >= 35 with the flag set the buff is removed.
//     INFERENCE: the hits delivered to the player as the TARGET are the Lua OnTargetHit calls (IDA caller of
//     CharAI::OnCombatResults not identified, OPEN-4). The HP check reads the actor HP at dispatch time.
//     The Lua PlaySound("sfx_passive_skill_activate_3") is NOT dispatched here (presentation gap).
// Not implemented (listed in the report): the CanAttackInMelee gate of KillingFrenzy (every hit counts), and the
// passives whose outcome needs HasBow or a defender-side faction/targetability change (Precision, SideStep, Mirror, Petrify).
#include "combat_result_event_bus_v1.hpp"
#include "runtime_timed_buff_v1.hpp"
#include "../../../game-data/class_tables.hpp"
#include "../../../game-data/properties.hpp"
#include "../../../game-data/skill_tables.hpp"
#include "../../character_state.hpp"

#include <cstdint>
#include <functional>
#include <map>
#include <random>
#include <string>
#include <string_view>
#include <vector>

namespace dh::foundation::generic_skills {

enum class EventPassiveKindV1 : std::uint8_t { none, killing_frenzy, carrion_eater, disciples, petrify, mirror, killing_streak, survival, holy_weapon, shield, energy_balance, conversion };

// SkillTable script name -> event passive kind (kept next to the implemented scripts only).
EventPassiveKindV1 event_passive_kind_for_script_v1(std::string_view script);

class RuntimeEventPassiveOwnerV1 {
public:
    RuntimeEventPassiveOwnerV1(TimedBuffSinkV1& buffs, const dh2::data::ClassTables& classes,
                               const dh2::data::PropertyRules& rules, CombatResultEventBusV1& bus, ActorId player);
    ~RuntimeEventPassiveOwnerV1();
    RuntimeEventPassiveOwnerV1(const RuntimeEventPassiveOwnerV1&) = delete;
    RuntimeEventPassiveOwnerV1& operator=(const RuntimeEventPassiveOwnerV1&) = delete;

    // Equipped faery element for Energy Balance (GetEquippedFaeryElement); -1 = no faery (the Lua falls back to Lightning).
    void set_equipped_element(std::int32_t element) { equipped_element_ = element; }
    // Rank of one event passive (0 = not learned: its listener does nothing).
    void set_rank(EventPassiveKindV1 kind, std::uint32_t rank);
    std::uint32_t rank(EventPassiveKindV1 kind) const;
    // The live session the listeners act on; the host binds it before the hits of a frame are dispatched.
    void bind(CombatSession& session);
    // v21.8 w2-illusionist: SetStunState(duration, true) on the attacker (Mirror). The host binds the ES-STATUS owner.
    void set_stun_target(std::function<bool(CombatSession&, ActorId, double, std::string&)> stun) { stun_target_ = std::move(stun); }
    // Ages the Killing Frenzy reset timers (wall seconds, same clock as the buff owner).
    bool advance(CombatSession& session, double dt_seconds, std::string& error);

    // One record per Carrion Eater kill heal (the host logs them; HP in whole units).
    struct CarrionHealV1 { float before; float heal; float after; std::uint32_t rank; };
    std::vector<CarrionHealV1> take_carrion_heals() { std::vector<CarrionHealV1> out; out.swap(carrion_heals_); return out; }

    std::uint32_t frenzy_counter() const noexcept { return frenzy_counter_; }
    bool frenzy_buff_active() const noexcept { return frenzy_active_; }
    bool survival_flag() const noexcept { return survival_flag_; }
    bool survival_buff_active() const noexcept { return survival_buff_; }
    // Last failure raised inside a listener (listeners cannot return errors); empty when none.
    const std::string& last_error() const noexcept { return error_; }

private:
    void on_hit(const CombatResultEventV1& event, ActorId receiver);
    void frenzy_hit();
    bool frenzy_set_buff(std::uint32_t strength);
    void carrion_kill();
    void holy_hit(ActorId attacker);
    void disciples_hit(const CombatResultEventV1& event);
    void petrify_hit(const CombatResultEventV1& event);
    void mirror_hit(const CombatResultEventV1& event);
    // The ClassTable row projected at SnS_Level = level (as CarrionEater does); false with error_ set on failure.
    bool projected_class_cell(const char* class_name, std::uint32_t level, unsigned cell, float& value);
    void streak_kill();
    void survival_hit();
    void shield_hit(ActorId attacker);
    void shield_advance(CombatSession& session, double dt_ms);
    void energy_advance(CombatSession& session, double dt_ms, std::string& error);
    void conversion_hit(ActorId defender);

    TimedBuffSinkV1* buffs_;
    const dh2::data::ClassTables* classes_;
    const dh2::data::PropertyRules* rules_;
    CombatResultEventBusV1* bus_;
    ActorId player_;
    CombatSession* session_ = nullptr;
    std::map<EventPassiveKindV1, std::uint32_t> ranks_;
    std::function<bool(CombatSession&, ActorId, double, std::string&)> stun_target_;
    std::mt19937 rng_{0x1DC0DEu}; // Disciples GetRand(1,100) and Mirror GetRand(0,100); fixed seed for reproducible runs
    std::uint64_t token_ = 0;
    std::uint32_t frenzy_counter_ = 0;
    bool frenzy_active_ = false;
    double frenzy_timer_ms_ = 0.0;
    bool survival_flag_ = false;       // Lua buff_survival ~= nil (the 8000 ms StopBuffTimer clears it)
    double survival_flag_ms_ = 0.0;
    bool survival_buff_ = false;       // the Survival buff instance is present (removed at HP >= 35%)
    std::string error_;
    std::vector<CarrionHealV1> carrion_heals_;
    // Guardian Shield stun ticks (StartSkillTimerCB(500): the stun is applied when the tick fires).
    struct ShieldStunV1 { ActorId actor; double left_ms; double duration_ms; };
    std::vector<ShieldStunV1> shield_stuns_;
    // Energy Balance: CreateBuff(class of the equipped element, 3000, false, rank) on a rank change and every 3000 ms.
    std::int32_t equipped_element_ = -1;  // faery element of the equipped faery (FaeryTypes), -1 = none
    std::uint32_t energy_rank_seen_ = 0;
    bool energy_active_ = false;
    std::string energy_class_;
    double energy_timer_ms_ = 0.0;
    // Conversion: the monsters converted by a FactionBuff3 buff (Lua memory_object/memory_buff, 6 slots).
    std::vector<ActorId> converted_;
    std::uint32_t conversion_procs_ = 0;
};

// Live wiring: maps the saved skill rows to their event passives and sets the ranks (rank 0 clears the listener's effect).
bool sync_event_passive_ranks_v1(RuntimeEventPassiveOwnerV1& owner, const CharacterState& state,
                                 const dh2::data::SkillTables::Borrow& tables, std::string& error);

} // namespace dh::foundation::generic_skills
