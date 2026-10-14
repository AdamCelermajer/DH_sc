#include "passive_skill_owner_v1.hpp"

#include <cstring>

namespace dh::foundation::generic_skills {

namespace {
struct StatPassiveScriptV1 {
    const char* script;
    const char* class_name;
};
// Verified against the original scripts (`GetPyOID("ClassTable", ...)` in each prince_*.luac, plain text).
constexpr StatPassiveScriptV1 kStatPassives[] = {
    {"prince_archer_bowmastery", "Skill_Archer_BowMastery"},
    // v21.8 w2-illusionist: OnSkillUpdate_ CreateBuff(CLASS, 0, false, L) + ApplyBuff on self (permanent). Mesmerize's group
    // buff loop (TargetListSearch Player filter, skips self) is not modelled: a single-player session has no other Player.
    {"prince_illusionist_mesmerize", "Skill_Illusionist_Mesmerize"},
    {"prince_illusionist_mirror", "Skill_Illusionist_Mirror"},
    {"prince_assassin_assassinate", "Skill_Assassin_Assassinate"},
    {"prince_assassin_blade_deflect", "Skill_Assassin_BladeDeflect"},
    {"prince_assassin_treasure_hunter", "Skill_Assassin_TreasureHunter"},
    {"prince_berseeker_ravager", "Skill_Berserk_Ravager"},
    {"prince_berseeker_toughness", "Skill_Berserk_Toughness"},
    {"prince_necromancer_leecher", "Skill_Necromancer_Leecher"},
    {"prince_rogue_acrobat", "Skill_Rogue_Acrobat"},
    {"prince_rogue_ambidextrous", "Skill_Rogue_Ambidextrous"},
    {"prince_rogue_combomaster", "Skill_Rogue_ComboMaster"},
    {"prince_rogue_findweakness", "Skill_Rogue_FindWeakness"},
    {"prince_warrior_combat_master", "Skill_Warrior_CombatMaster"},
    {"prince_warrior_fast_metabolism", "Skill_Warrior_FastMetabolism"},
    {"prince_warrior_hardiness", "Skill_Warrior_Hardiness"},
    {"prince_warrior_potion_addict", "Skill_Warrior_PotionAddict"},
};

// v21.8 berserker: second authored CreateBuff of a stat passive, active only from a minimum rank. prince_berseeker_ravager.luac:
// `if L > 6 then SetProp(Special_Equip_2H_In_One, ToFixed(L)); CreateBuff(Skill_Berserk_Ravager_buff, 0, false, L)`. The
// buff sheet sets property 203 = L (two-hand weapon in one hand). Deviation: the Lua SetProp persists after a level drop
// (OPEN-11); the buff is removed when the rank falls to 6 or below.
struct ThresholdStatPassiveV1 {
    const char* script;
    const char* class_name;
    std::uint32_t min_rank;
};
constexpr ThresholdStatPassiveV1 kThresholdStatPassives[] = {
    {"prince_berseeker_ravager", "Skill_Berserk_Ravager_buff", 7},
};
} // namespace

const char* stat_passive_class_for_script_v1(std::string_view script) {
    for (const auto& row : kStatPassives)
        if (script == row.script) return row.class_name;
    return nullptr;
}

bool sync_stat_passive_ranks_v1(RuntimePassiveSkillOwnerV1& owner, CombatSession& session, ActorId actor,
                                const CharacterState& state, const dh2::data::SkillTables::Borrow& tables,
                                std::string& error) {
    error.clear();
    if (!tables) {
        error = "Stat passive sync requires the SkillTable snapshot";
        return false;
    }
    const auto& records = tables.skills();
    for (const auto& progress : state.skills) {
        const auto index = tables.skill_index(progress.id.c_str());
        if (index < 0 || static_cast<std::size_t>(index) >= records.size()) continue;
        const char* class_name = stat_passive_class_for_script_v1(records[static_cast<std::size_t>(index)].script);
        if (!class_name) continue;
        PassiveStatRowV1 row;
        row.skill_key = progress.id;
        row.class_name = class_name;
        row.source = "stat passive " + progress.id;
        if (!owner.set_rank(session, actor, row, progress.rank, error)) return false;
        for (const auto& threshold : kThresholdStatPassives) {
            if (records[static_cast<std::size_t>(index)].script != threshold.script) continue;
            PassiveStatRowV1 gated;
            gated.skill_key = progress.id + "#threshold";
            gated.class_name = threshold.class_name;
            gated.source = "stat passive " + progress.id + " (rank threshold)";
            const std::uint32_t rank = progress.rank >= threshold.min_rank ? progress.rank : 0u;
            if (!owner.set_rank(session, actor, gated, rank, error)) return false;
        }
    }
    return true;
}

bool RuntimePassiveSkillOwnerV1::set_rank(CombatSession& session, ActorId actor, const PassiveStatRowV1& row,
                                          std::uint32_t rank, std::string& error) {
    error.clear();
    const auto key = std::make_pair(actor, row.skill_key);
    const auto found = ranks_.find(key);
    const std::uint32_t previous = found == ranks_.end() ? 0u : found->second;
    if (found != ranks_.end() && previous == rank) return true;
    if (previous != 0 && !buffs_->remove_timed_buff(session, actor, row.class_name, error)) return false;
    if (rank == 0) {
        ranks_.erase(key);
        return true;
    }
    TimedBuffSpecV1 spec;
    spec.class_name = row.class_name;
    spec.strength = rank;
    spec.duration_ms = 0.0;
    spec.permanent = true;
    spec.capacity = 1;
    spec.source = row.source;
    if (!buffs_->apply_timed_buff(session, actor, spec, error)) return false;
    ranks_[key] = rank;
    return true;
}

std::uint32_t RuntimePassiveSkillOwnerV1::rank(ActorId actor, const std::string& skill_key) const {
    const auto found = ranks_.find(std::make_pair(actor, skill_key));
    return found == ranks_.end() ? 0u : found->second;
}

} // namespace dh::foundation::generic_skills
