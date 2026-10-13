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
//   Figments:
//     - SetStunState(2000..4000 ms random, non-boss) on each target (no stun state in CombatSession).
//     - CreateBuff/ApplyBuff of Skill_Illusionist_Figments_buff on each target (presentation FX BUFF_FX and the
//       duration only; the buff class has no property rows). The caster-side buff arm is not used.
//     - SnS_Dot_Min/Max_Damage poison on targets is class-driven in the Lua; the port applies only the SkillCombatRoll hit.
#include "runtime_skill_use_profile_v1.hpp"

namespace dh::foundation::generic_skills {

// Rows in the order of the Lua files. Values:
//   DeathClaw   prince_necromancer_deathclaw.luac  OnPre TargetListSearch(RANGE 680, ANGLE 135) FrontalFirst + LookAt;
//               SetSkillCooldown(COOLDOWN_BASE 1500) in OnPre; OnSkill_ re-searches 680/135 (objects rolled too, they
//               reach Interact); Post ClearTarget.
//   Figments    prince_illusionist_figments.luac   no OnPre query; OnPre UseMana + SetSkillCooldown(SnS_Cooldown);
//               OnSkill_ TargetListSearch(RANGE 1000, ANGLE 360) ClosestFirst, enemy characters only (object filter
//               NONE), SkillCombatRoll each; no Post.
inline constexpr RuntimeSkillUseProfileV1 kRuntimeSkillUseProfilesMageSubV1[] = {
    {"DeathClaw", "prince_necromancer_deathclaw", "Skill_Necromancer_DeathClaw",
     680.0f, 2.35619449019234492f, SkillTargetSortV1::frontal_first, true,
     true,
     SkillUseTargetsV1::requery_at_use, 680.0f, false, 2.35619449019234492f,
     SkillTargetSortV1::frontal_first, false,
     true, true, false, nullptr, 1500.0f},
    {"Figments", "prince_illusionist_figments", "Skill_Illusionist_Figments",
     0.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, false,
     true,
     SkillUseTargetsV1::requery_at_use, 1000.0f, false, kSkillPiV1,
     SkillTargetSortV1::closest_first, false,
     false, false, false, nullptr, 0.0f},
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
