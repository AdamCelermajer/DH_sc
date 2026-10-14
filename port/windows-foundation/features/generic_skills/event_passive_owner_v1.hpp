#pragma once
// v21.8 es-passive: event passives fed by the OnTargetHit bus (the Lua StealAIEvent scripts of the player's passives).
//
// Implemented from the original scripts (plain-text Lua, read directly):
//   prince_berseeker_killing_frenzy.luac: OnTargetHit with attacker == self. counter 1..5 builds the buff
//     Skill_Berserk_KillingFrenzy with SnS_Level = counter x rank; a 1500 ms reset timer removes the buff (the counter is
//     kept, the next hit with no buff restarts at 1); at counter 5 the buff is rebuilt at 5 x rank with a 5000 ms timer.
//   prince_necromancer_carrioneater.luac: OnTargetHit with attacker == self and defender dead: heal TempProp1 of
//     Skill_Necromancer_CarrionEater at the rank (clamped to max HP).
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
// passives whose outcome needs knockback/stun state or HasBow (Precision, SideStep, Shield, Mirror, Petrify).
#include "combat_result_event_bus_v1.hpp"
#include "runtime_timed_buff_v1.hpp"
#include "../../../game-data/class_tables.hpp"
#include "../../../game-data/properties.hpp"
#include "../../../game-data/skill_tables.hpp"
#include "../../character_state.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace dh::foundation::generic_skills {

enum class EventPassiveKindV1 : std::uint8_t { none, killing_frenzy, carrion_eater, killing_streak, survival, holy_weapon };

// SkillTable script name -> event passive kind (kept next to the implemented scripts only).
EventPassiveKindV1 event_passive_kind_for_script_v1(std::string_view script);

class RuntimeEventPassiveOwnerV1 {
public:
    RuntimeEventPassiveOwnerV1(TimedBuffSinkV1& buffs, const dh2::data::ClassTables& classes,
                               const dh2::data::PropertyRules& rules, CombatResultEventBusV1& bus, ActorId player);
    ~RuntimeEventPassiveOwnerV1();
    RuntimeEventPassiveOwnerV1(const RuntimeEventPassiveOwnerV1&) = delete;
    RuntimeEventPassiveOwnerV1& operator=(const RuntimeEventPassiveOwnerV1&) = delete;

    // Rank of one event passive (0 = not learned: its listener does nothing).
    void set_rank(EventPassiveKindV1 kind, std::uint32_t rank);
    std::uint32_t rank(EventPassiveKindV1 kind) const;
    // The live session the listeners act on; the host binds it before the hits of a frame are dispatched.
    void bind(CombatSession& session);
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
    void streak_kill();
    void survival_hit();

    TimedBuffSinkV1* buffs_;
    const dh2::data::ClassTables* classes_;
    const dh2::data::PropertyRules* rules_;
    CombatResultEventBusV1* bus_;
    ActorId player_;
    CombatSession* session_ = nullptr;
    std::map<EventPassiveKindV1, std::uint32_t> ranks_;
    std::uint64_t token_ = 0;
    std::uint32_t frenzy_counter_ = 0;
    bool frenzy_active_ = false;
    double frenzy_timer_ms_ = 0.0;
    bool survival_flag_ = false;       // Lua buff_survival ~= nil (the 8000 ms StopBuffTimer clears it)
    double survival_flag_ms_ = 0.0;
    bool survival_buff_ = false;       // the Survival buff instance is present (removed at HP >= 35%)
    std::string error_;
    std::vector<CarrionHealV1> carrion_heals_;
};

// Live wiring: maps the saved skill rows to their event passives and sets the ranks (rank 0 clears the listener's effect).
bool sync_event_passive_ranks_v1(RuntimeEventPassiveOwnerV1& owner, const CharacterState& state,
                                 const dh2::data::SkillTables::Borrow& tables, std::string& error);

} // namespace dh::foundation::generic_skills
