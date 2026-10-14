#pragma once
// v21.8 skills-mage-sub: data-driven use profiles for the Illusionist and Necromancer subclass active skills.
//
// Same admission model as runtime_skill_use_profile_v1.hpp: the key is the SkillTable (name, script) pair and every
// numeric/behavioural field is transcribed from the original skill Lua
// (.local-inputs/assets-extra/android/data/scripts/skills/<script>.luac, plain Lua text). Rows live here, not in the
// base table, so the parallel skill batches do not edit the same array. Only skills whose Use arm the generic cast
// coordinator can express are admitted; the rest stay rejected (see coordination/v21.8/magesub-report.md).
//
// Fields NOT encoded (documented gaps; the Lua is the reference):
//   DeathClaw:
//     - SetKnockBackState(true) on a damaged character (no knockback state in CombatSession).
//     - The Use search is TargetListSearch(RANGE, ANGLE, 0, -40, 0, true): the extra origin arguments are not modelled.
//       The port searches from the caster position (inference: the -40 origin shift is not reproduced).
//   Figments (w2-illusionist: stun and FX buff are encoded, see kFigmentsEnemyStunV1 / kFigmentsEnemyBuffV1):
//     - DoT tail: SnS_Dot_Min/Max_Damage poison on targets is applied by F_ApplyResult 0x3b10b4 through PROPS_AddDot
//       (buff dot_<element>, capacity 1, tick in property element+127). Not encoded: the per-tick consumer is not
//       wired on the combat path (see es-buff report item 5). The port applies only the SkillCombatRoll hit.
#include "runtime_skill_use_profile_v1.hpp"

namespace dh::foundation::generic_skills {

// Rows in the order of the Lua files. Values:
//   DeathClaw   prince_necromancer_deathclaw.luac  OnPre TargetListSearch(RANGE 680, ANGLE 135) FrontalFirst + LookAt;
//               SetSkillCooldown(COOLDOWN_BASE 1500) in OnPre; OnSkill_ re-searches 680/135 (objects rolled too, they
//               reach Interact); Post ClearTarget.
//   Figments    prince_illusionist_figments.luac   no OnPre query; OnPre UseMana + SetSkillCooldown(SnS_Cooldown);
//               OnSkill_ TargetListSearch(RANGE 1000, ANGLE 360) ClosestFirst, enemy characters only (object filter
//               NONE), SkillCombatRoll each; no Post.
// v21.8 w2-illusionist enemy arms (see RuntimeSkillEnemyBuffProfileV1 in runtime_skill_use_profile_v1.hpp).
// Eclipse: CreateBuff(Skill_Illusionist_Eclipse, SnS_Duration or 3000+1000*L, false, L, skill_dh2_..._persist) + ApplyBuff on
//   every non-player Character target; the class speed rows (walk/rotation/attack, -25-L %) apply to bosses too.
inline constexpr RuntimeSkillEnemyBuffProfileV1 kEclipseEnemyBuffV1 = {
    "Skill_Illusionist_Eclipse", "skill_dh2_prince_mage_illusionist_eclipse_persist", 176, 3000.0f, 1000.0f, false, false};
// TimeControl: CreateBuff(Buff_Illusionist_TimeControl, 4000+1000*L, false, L, spell_dh2_..._timecontrol) for every Character
//   target that is neither player, boss nor miniboss. Duration is hard-coded in the Lua (no class read): prop index -1.
inline constexpr RuntimeSkillEnemyBuffProfileV1 kTimeControlEnemyBuffV1 = {
    "Buff_Illusionist_TimeControl", "spell_dh2_prince_illusionist_timecontrol", -1, 4000.0f, 1000.0f, true, true};
// Figments: stun (1500..4000 ms, non-boss non-miniboss) and CreateBuff(Skill_Illusionist_Figments_buff, SnS_Dot_Duration, false,
//   L, spell_dh2_prince_illusionist_figments) on every enemy target (no boss gate in the Lua). SnS_Dot_Duration = property 181.
inline constexpr RuntimeSkillEnemyBuffProfileV1 kFigmentsEnemyBuffV1 = {
    "Skill_Illusionist_Figments_buff", "spell_dh2_prince_illusionist_figments", 181, 4000.0f, 250.0f, false, false};
inline constexpr RuntimeSkillEnemyStunProfileV1 kFigmentsEnemyStunV1 = {1500.0f, 4000.0f, true};
// Terror: FactionBuff (class row AIFaction = 8) with SnS_Fear_Duration = property 187; conversion memory of 3 slots.
inline constexpr RuntimeSkillEnemyBuffProfileV1 kTerrorEnemyBuffV1 = {
    "FactionBuff", "spell_dh2_prince_illusionist_terror", 187, 2000.0f, 1000.0f, false, false, 3};

inline constexpr RuntimeSkillUseProfileV1 kRuntimeSkillUseProfilesMageSubV1[] = {
    {"DeathClaw", "prince_necromancer_deathclaw", "Skill_Necromancer_DeathClaw",
     680.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, true,
     true,
     SkillUseTargetsV1::requery_at_use, 680.0f, false, native_search_max_angle_v1(135.0f),
     SkillTargetSortV1::frontal_first, false,
     true, true, false, nullptr, nullptr, false, true, 1500.0f},
    {"Figments", "prince_illusionist_figments", "Skill_Illusionist_Figments",
     0.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, false,
     true,
     SkillUseTargetsV1::requery_at_use, 1000.0f, false, kSkillPiV1,
     SkillTargetSortV1::closest_first, false,
     false, false, false, nullptr, nullptr, false, true, 0.0f,
     "sfx_skill_mage_ilu_nightmare", nullptr, nullptr, false, nullptr, nullptr, 0, nullptr, nullptr, &kFigmentsEnemyBuffV1, &kFigmentsEnemyStunV1},
    // prince_illusionist_eclipse.luac: OnPre TargetListSearch(RANGE 600, no angle, NoSort, AttackableOnly objects, Enemy) +
    // LookAt(top) + UseMana; no cooldown. OnSkill_ repeats the search, then per popped non-player Character: PlayFX(spell_...)
    // and the enemy CreateBuff/ApplyBuff. No Post, no roll (the buff is the whole effect).
    {"Eclipse", "prince_illusionist_eclipse", "Skill_Illusionist_Eclipse",
     600.0f, kSkillPiV1, SkillTargetSortV1::source_order, true,
     false,
     SkillUseTargetsV1::requery_at_use, 600.0f, false, kSkillPiV1,
     SkillTargetSortV1::source_order, false,
     false, false, false, nullptr, nullptr, false, false, 0.0f,
     nullptr, nullptr, "spell_dh2_prince_mage_illusionist_eclipse", true, nullptr, nullptr, 0, nullptr, nullptr, &kEclipseEnemyBuffV1, nullptr},
    // prince_illusionist_timecontrol.luac: OnPre TargetListSearch(RANGE 1000, Enemy, AttackableOnly, ClosestFirst) + LookAt(top)
    // + SetSkillCooldown(SnS_Cooldown, 6000 fallback) + UseMana. OnSkill_ PlaySound(_hit), ApplyPropClass(caster), then the enemy
    // CreateBuff/ApplyBuff loop over Character targets that are not player, boss or miniboss. No roll, no Post.
    {"TimeControl", "prince_illusionist_timecontrol", "Skill_Illusionist_TimeControl",
     1000.0f, kSkillPiV1, SkillTargetSortV1::closest_first, true,
     true,
     SkillUseTargetsV1::requery_at_use, 1000.0f, false, kSkillPiV1,
     SkillTargetSortV1::closest_first, false,
     false, false, false, nullptr, nullptr, false, false, 0.0f,
     "sfx_skill_mage_ilu_hold_time_hit", nullptr, nullptr, false, nullptr, nullptr, 0, nullptr, nullptr, &kTimeControlEnemyBuffV1, nullptr},
    // prince_illusionist_terror.luac: OnSkillCheck HasMana, empty OnPre. OnSkill_ TargetListSearch(RANGE 650, ANGLE 240, Enemy,
    // FrontalFirst) and one conversion per Use: the first popped Character that holds no slot gets UseMana, ApplyPropClass,
    // SkillCombatRoll (not modelled: Terror has no damage rows), CreateBuff(FactionBuff, SnS_Fear_Duration, false, L, FX) +
    // ApplyBuff, and a slot timer. Three slots, oldest freed when full. Mana: the generic OnPre prefix charges it on every cast
    // (deviation: the Lua charges per conversion, so a cast with no eligible target costs nothing there).
    {"Terror", "prince_illusionist_terror", "Skill_Illusionist_Terror",
     0.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, false,
     false,
     SkillUseTargetsV1::requery_at_use, 650.0f, false, native_search_max_angle_v1(240.0f),
     SkillTargetSortV1::frontal_first, false,
     false, false, false, nullptr, nullptr, false, false, 0.0f,
     nullptr, nullptr, nullptr, false, nullptr, nullptr, 0, nullptr, nullptr, &kTerrorEnemyBuffV1, nullptr},
};

// Returns the subclass profile whose SkillTable (name, script) pair matches exactly, or nullptr.
inline const RuntimeSkillUseProfileV1* find_skill_use_profile_magesub_v1(
    const std::string& skill_name, const std::string& script) {
    for (const auto& profile : kRuntimeSkillUseProfilesMageSubV1) {
        if (skill_name == profile.skill_name && script == profile.script) return &profile;
    }
    return nullptr;
}

} // namespace dh::foundation::generic_skills
