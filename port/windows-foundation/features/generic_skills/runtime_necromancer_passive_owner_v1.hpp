#pragma once
// v21.8 w2-necromancer: Wrath (prince_necromancer_wrath.luac) and Putrid (prince_necromancer_putrid.luac).
//
// Wrath: StartSkillTimerCB(3000, true, canApplyBuff) while the rank is non-zero. canApplyBuff: HP% < 40 and no buff ->
//   CreateBuff(Skill_Necromancer_Wrath, 0, false, rank) + ApplyBuff + PlaySound(sfx_passive_skill_activate_3); HP% >= 40
//   and a buff -> RemoveBuff. The bonus lives in the buff's class sheet (Spell_Damage_Bonus_* 166-170).
// Putrid: StealAIEvent("OnTargetHit"): when the player is the defender and the attacker is in melee range, GetRand(0,100)
//   below the chance (TempProp1, percent) gives the attacker CreateBuff(Skill_Necromancer_Putrid, TempProp2 ms, false,
//   rank, AUTO_DOT_00_NORMAL) + ApplyBuff. The DoT ticks are NOT applied (no native DoT tail in the port, see the report).
#include "../../combat_session.hpp"
#include "combat_result_event_bus_v1.hpp"
#include "runtime_skill_timer_service_v1.hpp"
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

namespace dh::foundation::generic_skills {

enum class NecromancerPassiveKindV1 : std::uint8_t { none, wrath, putrid };

// SkillTable script name -> passive kind (only the two passives owned here).
NecromancerPassiveKindV1 necromancer_passive_kind_for_script_v1(const std::string& script);

class RuntimeNecromancerPassiveOwnerV1 {
public:
    using PlaySoundFn = std::function<bool(const std::string& sound, std::string& error)>;
    RuntimeNecromancerPassiveOwnerV1(TimedBuffSinkV1& buffs, const dh2::data::ClassTables& classes,
                                     const dh2::data::PropertyRules& rules, CombatResultEventBusV1& bus, ActorId player,
                                     PlaySoundFn play_sound);
    ~RuntimeNecromancerPassiveOwnerV1();
    RuntimeNecromancerPassiveOwnerV1(const RuntimeNecromancerPassiveOwnerV1&) = delete;
    RuntimeNecromancerPassiveOwnerV1& operator=(const RuntimeNecromancerPassiveOwnerV1&) = delete;

    // Saved rank (0 = not learned: the Wrath loop stops and Putrid does nothing).
    void set_rank(NecromancerPassiveKindV1 kind, std::uint32_t rank);
    std::uint32_t rank(NecromancerPassiveKindV1 kind) const;
    // The live Session the listener and the Wrath loop act on (bound per frame by the host).
    void bind(CombatSession& session);
    // Wrath loop clock (gameplay ms, once per unpaused frame).
    bool advance(CombatSession& session, double dt_ms, std::string& error);

    bool wrath_buff_active() const noexcept { return wrath_active_; }
    std::uint32_t putrid_procs() const noexcept { return putrid_procs_; }
    std::uint32_t putrid_applied() const noexcept { return putrid_applied_; }
    const std::string& last_error() const noexcept { return error_; }
    // Test seam: the Putrid roll source (GetRand(0,100) equivalent). Default: a seeded engine.
    void seed_roll(std::uint32_t seed) { rng_.seed(seed); }

private:
    void on_hit(const CombatResultEventV1& event, ActorId receiver);
    void putrid_hit(const CombatResultEventV1& event);
    bool wrath_tick(CombatSession& session, std::string& error);
    bool class_sheet(std::uint32_t rank, const char* class_name, dh2::data::PropertySheet& out, std::string& error);

    TimedBuffSinkV1* buffs_;
    const dh2::data::ClassTables* classes_;
    const dh2::data::PropertyRules* rules_;
    CombatResultEventBusV1* bus_;
    ActorId player_;
    PlaySoundFn play_sound_;
    CombatSession* session_ = nullptr;
    std::map<NecromancerPassiveKindV1, std::uint32_t> ranks_;
    std::uint64_t token_ = 0;
    RuntimeSkillTimerServiceV1 timers_;
    RuntimeSkillTimerIdV1 wrath_timer_ = 0;
    bool wrath_active_ = false;
    std::uint32_t putrid_procs_ = 0, putrid_applied_ = 0;
    std::mt19937 rng_{1u};
    std::string error_;
};

// Live wiring: maps the saved skill rows of the two passives to their ranks (rank 0 clears the effect).
bool sync_necromancer_passive_ranks_v1(RuntimeNecromancerPassiveOwnerV1& owner, const CharacterState& state,
                                       const dh2::data::SkillTables::Borrow& tables, std::string& error);

} // namespace dh::foundation::generic_skills
