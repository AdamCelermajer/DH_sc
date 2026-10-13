#pragma once
// v21.8 es-passive: generic owner for stat passives (the Lua OnSkillUpdate_ pattern shared by the authored passives).
//
// Original (e.g. prince_berseeker_toughness.luac, prince_assassin_assassinate.luac, faerie_celest.luac CheckFaeries):
//   OnSkillUpdate_: on a rank change, RemoveBuff(CLASS) for the previous instance, then if rank ~= 0:
//   CreateBuff(CLASS, 0, false, rank) + ApplyBuff. Duration 0 = permanent (no timer). Pre/Skill/Post are empty.
// Port: this owner keeps the (actor, skill) rank and performs that sequence through the shared timed-buff owner. It does
// not read SkillTable ranks itself; the host passes the rank it obtained (no live-rank wiring in this change, see report).
#include "runtime_timed_buff_v1.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <utility>

namespace dh::foundation::generic_skills {

struct PassiveStatRowV1 {
    std::string skill_key;   // SkillTable name (owner key for the rank memory)
    std::string class_name;  // ClassTable row applied as the permanent buff (Skill_<Tree>_<Name>)
    std::string source;      // log label
};

class RuntimePassiveSkillOwnerV1 {
public:
    explicit RuntimePassiveSkillOwnerV1(TimedBuffSinkV1& buffs) : buffs_(&buffs) {}

    // Applies a rank for one passive. A rank equal to the remembered rank is a no-op. A changed rank removes the previous
    // instance and, for rank > 0, applies the class permanently with strength = rank (capacity 1, the Lua `false`).
    bool set_rank(CombatSession& session, ActorId actor, const PassiveStatRowV1& row, std::uint32_t rank,
                  std::string& error);
    std::uint32_t rank(ActorId actor, const std::string& skill_key) const;

private:
    TimedBuffSinkV1* buffs_;
    std::map<std::pair<ActorId, std::string>, std::uint32_t> ranks_;
};

} // namespace dh::foundation::generic_skills
