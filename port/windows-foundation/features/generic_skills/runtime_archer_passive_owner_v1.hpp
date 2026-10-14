#pragma once
// v21.8 w2-archer: the two Archer passives that listen on the OnTargetHit bus (StealAIEvent scripts), read from the plain-text
// Lua (.local-inputs/assets-extra/android/data/scripts/skills/):
//   prince_archer_precision.luac: OnSkillUpdate_ (rank change): RemoveBuff, then with HasBow() and rank > 0
//     CreateBuff(Skill_Archer_Precision, 0, false, rank) + ApplyBuff + StartSkillTimerCB(1000, true, checkbuff). OnTargetHit with
//     attacker == self and rank > 0: HasBow() and no buff -> the same CreateBuff/ApplyBuff/timer; no bow and a buff -> RemoveBuff +
//     StopTimerCB. checkbuff (1000 ms): no bow and a buff -> RemoveBuff + StopTimerCB. The effect lives in the class sheet
//     (Slow_Target_Chance 145 / Slow_Duration 146 raised per rank: "a chance to stun with the weapons").
//   prince_archer_sidestep.luac (Deflection): OnTargetHit with defender == self and attacker:TargetInMeleeRange(): GetRand(0,100)
//     below KNOCKBACK (= FromFixed(TempProp1) of the class at the rank, percent) starts StartSkillTimerCB(500, true, check, attacker).
//     check: attacker:SetKnockBackState(true); then by the attacker's state: not Attack -> StopTimerCB + attacker:Stop(); KnockedBack ->
//     StopTimerCB. (Hence the knockback is retried every 500 ms while the attacker stays in Attack.)
// The generic engine pieces used: CombatResultEventBusV1 (attacker side first, outcomes & 3 selects the miss callback, so the hit
// listeners only run on landed hits), the shared timed-buff owner, RuntimeSkillTimerServiceV1, the Session script Stop, and the
// host's same-Session Push sink (RuntimeSkillKnockBackSinkV1), the same one DeathClaw / MultiHitArrow use.
#include "../../combat_session.hpp"
#include "combat_result_event_bus_v1.hpp"
#include "runtime_skill_cast_coordinator_v1.hpp"
#include "runtime_skill_timer_service_v1.hpp"
#include "runtime_timed_buff_v1.hpp"
#include "../../../game-data/class_tables.hpp"
#include "../../../game-data/properties.hpp"
#include "../../../game-data/skill_tables.hpp"
#include "../../character_state.hpp"

#include <cstdint>
#include <functional>
#include <map>
#include <string>

namespace dh::foundation::generic_skills {

enum class ArcherPassiveKindV1 : std::uint8_t { none, precision, side_step };

// SkillTable script name -> passive kind (only the two passives owned here; BowMastery is a plain stat passive).
ArcherPassiveKindV1 archer_passive_kind_for_script_v1(const std::string& script);

struct RuntimeArcherPassiveServicesV1 {
    // Lua HasBow(): the main hand holds a bow right now (ItemTable Type 4).
    std::function<bool()> has_bow;
    // Lua attacker:SetKnockBackState(true): the host's same-Session Push sink.
    RuntimeSkillKnockBackSinkV1 knock_back;
    // Lua GetRand(0, 100): uniform integer in [0, bound) from the shared Session RNG.
    std::function<bool(std::uint32_t bound, std::uint32_t& value, std::string& error)> random;
    std::function<void(const std::string& line)> trace;
};

class RuntimeArcherPassiveOwnerV1 {
public:
    RuntimeArcherPassiveOwnerV1(TimedBuffSinkV1& buffs, const dh2::data::ClassTables& classes,
                                const dh2::data::PropertyRules& rules, CombatResultEventBusV1& bus, ActorId player,
                                RuntimeArcherPassiveServicesV1 services);
    ~RuntimeArcherPassiveOwnerV1();
    RuntimeArcherPassiveOwnerV1(const RuntimeArcherPassiveOwnerV1&) = delete;
    RuntimeArcherPassiveOwnerV1& operator=(const RuntimeArcherPassiveOwnerV1&) = delete;

    // Saved rank (0 = not learned). A changed rank runs the Lua OnSkillUpdate_.
    bool set_rank(ArcherPassiveKindV1 kind, std::uint32_t rank, std::string& error);
    std::uint32_t rank(ArcherPassiveKindV1 kind) const;
    void bind(CombatSession& session);
    // Gameplay clock in ms (Precision checkbuff, SideStep check timers).
    bool advance(CombatSession& session, double dt_ms, std::string& error);

    bool precision_buff_active() const noexcept { return precision_buff_; }
    std::uint32_t side_step_procs() const noexcept { return side_step_procs_; }
    std::uint32_t side_step_checks() const noexcept { return side_step_checks_; }
    std::uint32_t side_step_knockbacks() const noexcept { return side_step_knockbacks_; }
    std::size_t active_timers() const noexcept { return timers_.size(); }
    const std::string& last_error() const noexcept { return error_; }

private:
    void on_hit(const CombatResultEventV1& event, ActorId receiver);
    void precision_hit();
    void side_step_hit(const CombatResultEventV1& event);
    bool precision_create(std::string& error);
    bool precision_remove(std::string& error);
    bool side_step_check(RuntimeSkillTimerIdV1 timer, ActorId attacker, std::string& error);
    bool side_step_knockback(ActorId attacker, std::string& error);
    bool class_sheet(std::uint32_t rank, const char* class_name, dh2::data::PropertySheet& out, std::string& error);
    void log(const std::string&) const;

    TimedBuffSinkV1* buffs_;
    const dh2::data::ClassTables* classes_;
    const dh2::data::PropertyRules* rules_;
    CombatResultEventBusV1* bus_;
    ActorId player_;
    RuntimeArcherPassiveServicesV1 services_;
    CombatSession* session_ = nullptr;
    std::map<ArcherPassiveKindV1, std::uint32_t> ranks_;
    std::uint64_t token_ = 0;
    RuntimeSkillTimerServiceV1 timers_;
    RuntimeSkillTimerIdV1 precision_timer_ = 0;
    bool precision_buff_ = false;
    bool bow_seen_ = false;
    std::uint32_t side_step_procs_ = 0, side_step_checks_ = 0, side_step_knockbacks_ = 0;
    std::uint64_t next_generation_ = 0;
    std::string error_;
};

// Live wiring: maps the saved skill rows of the two passives to their ranks (rank 0 clears the effect).
bool sync_archer_passive_ranks_v1(RuntimeArcherPassiveOwnerV1& owner, const CharacterState& state,
                                  const dh2::data::SkillTables::Borrow& tables, std::string& error);

} // namespace dh::foundation::generic_skills
