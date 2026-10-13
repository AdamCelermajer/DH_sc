#pragma once
// v21.8 es-passive: generic owner for stat passives (the Lua OnSkillUpdate_ pattern shared by the authored passives).
//
// Original (e.g. prince_berseeker_toughness.luac, prince_assassin_assassinate.luac, faerie_celest.luac CheckFaeries):
//   OnSkillUpdate_: on a rank change, RemoveBuff(CLASS) for the previous instance, then if rank ~= 0:
//   CreateBuff(CLASS, 0, false, rank) + ApplyBuff. Duration 0 = permanent (no timer). Pre/Skill/Post are empty.
// Port: this owner keeps the (actor, skill) rank and performs that sequence through the shared timed-buff owner. It does
// not read SkillTable ranks itself; the host passes the rank it obtained (no live-rank wiring in this change, see report).
#include "runtime_timed_buff_v1.hpp"
#include "../../character_state.hpp"
#include "../../../game-data/skill_tables.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <utility>

namespace dh::foundation::generic_skills {

struct PassiveStatRowV1 {
    std::string skill_key;   // SkillTable name (owner key for the rank memory)
    std::string class_name;  // ClassTable row applied as the permanent buff (Skill_<Tree>_<Name>)
    std::string source;      // log label
};

// Stat passives whose original script is the plain OnSkillUpdate_ pattern (no StealAIEvent, a permanent
// CreateBuff(CLASS_ID, 0, false, slvl)). Keyed by the SkillTable script name; class rows from `GetPyOID("ClassTable", ...)`.
// Mage passives (elemental mastery, faery blood, magi shielding, staff master) and Warrior Leadership (group buff) are
// deliberately absent: they belong to other owners.
const char* stat_passive_class_for_script_v1(std::string_view script);

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

// Live wiring: walks the saved skill rows (CharacterState::skills; the ids are SkillTable dictionary names), maps each
// row to its SkillTable script, and applies the rank of every stat passive through `owner` (rank 0 removes). Rows whose
// script is not a stat passive are ignored. Returns false with `error` when the table or a buff application fails.
bool sync_stat_passive_ranks_v1(RuntimePassiveSkillOwnerV1& owner, CombatSession& session, ActorId actor,
                                const CharacterState& state, const dh2::data::SkillTables::Borrow& tables,
                                std::string& error);

} // namespace dh::foundation::generic_skills
