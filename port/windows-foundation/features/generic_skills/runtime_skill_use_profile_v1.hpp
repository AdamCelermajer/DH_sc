#pragma once
// v21.8 skills-a: data-driven use profiles for authored active player skills.
//
// Admission key: the SkillTable row's (name, script) pair. Every numeric and
// behavioural field below is transcribed from the original skill Lua
// (.local-inputs/assets-extra/android/data/scripts/skills/<script>.luac, which
// is plain Lua text despite the extension). Lua line references are per row.
// Anything not present in the Lua is not encoded here.
//
// Fields that are NOT encoded (documented gaps, see coordination/v21.8/skills-a-report.md):
//   - Stun / knockback / DOT / leech tails of F_ApplyResult. The coordinator
//     applies the authored ApplyPropClass row, but not the stun side effect.
//   - Buff owners (Quickness: CreateBuff/ApplyBuff/StartSkillTimerCB).
//   - Conditional Post (Charge: ClearTarget only when not TargetInMeleeRange).
//     The coordinator keeps its existing non-clearing behaviour for Charge.
#include "runtime_skill_target_query_v1.hpp"

#include <cstdint>
#include <string>

namespace dh::foundation::generic_skills {

// How the Use-time (OnSkill_) target list is produced.
enum class SkillUseTargetsV1 : std::uint8_t {
    // OnSkill_ runs TargetListSearch again with use_range/use_cone/use_sort
    // (Charge, GroundSlam, JumpKick, ViciousStrike).
    requery_at_use,
    // OnSkill_ keeps only the single OnPre-selected target (BashDown).
    pre_selected_single,
    // OnSkill_ pops the complete OnPre target list without a new search (Roundhouse).
    pre_list_all
};

struct RuntimeSkillUseProfileV1 {
    const char* skill_name;   // SkillTable.name
    const char* script;       // SkillTable.script (the prince_*.luac file stem)
    const char* class_token;  // ClassTable row used by CalcManaCost / HasMana / ApplyPropClass
    // OnPreSkill_ target search. pre_range == 0 means the Lua has no OnPre query.
    float pre_range;
    float pre_cone_radians;   // full cone; 2*pi/2 = pi (no-angle TargetListSearch default)
    SkillTargetSortV1 pre_sort;
    bool pre_look_at;         // LookAt(top target) when OnPre selects a target
    // OnPreSkill_ calls SetSkillCooldown(SnS_Cooldown). Only Charge and GroundSlam do.
    bool sets_cooldown;
    SkillUseTargetsV1 use_targets;
    // requery_at_use only. use_range_from_temp_prop1 = GroundSlam's
    // TargetListSearch(RANGE) with RANGE = CharacterProperties.TempProp1.
    float use_range;
    bool use_range_from_temp_prop1;
    float use_cone_radians;
    SkillTargetSortV1 use_sort;
    bool use_look_at;         // GroundSlam: LookAt(target) at Use for a selected character
    // false: the Lua pops objects without SkillCombatRoll (ViciousStrike:
    // `if target:IsCharacter() then SkillCombatRoll`). true: SkillCombatRoll
    // on objects is delivered as the typed Interact arm.
    bool rolls_objects;
    // OnPostSkill_ calls ClearTarget unconditionally. Charge's Post is conditional and stays false.
    bool post_clear_target;
    // Charge: a shielded caster uses Skill_Warrior_Charge_ShieldBonus at Use.
    bool shield_variant;
};

inline constexpr float kSkillPiV1 = 3.1415927410125732421875f;

// Rows in the order of the Lua files. Values:
//   BashDown        prince_warrior_bashdown.luac        RANGE 160; OnPre TargetListSearch(RANGE) + LookAt; OnSkill_ uses OnPre target; Post ClearTarget.
//   GroundSlam      prince_warrior_ground_slam.luac     OnPre has no target query; OnSkill_ search range=TempProp1, ClosestFirst, LookAt; SetSkillCooldown in OnPre; Post ClearTarget.
//   Charge          prince_warrior_charge.luac          RANGE 300 FrontalFirst + LookAt; OnSkill_ NoSort, RANGE 300, ANGLE 120; SetSkillCooldown; conditional Post.
//   JumpKick        prince_rogue_jump_kick.luac         INIT_RANGE 250 + MaxAngleSearch + LookAt; OnSkill_ RANGE 160, ANGLE 270; Post ClearTarget.
//   ViciousStrike   prince_rogue_vicious_strike.luac    SEARCH_RANGE 300 + MaxAngleSearch + LookAt; OnSkill_ RANGE 160, ANGLE 180, character-only rolls; CLASS = _Stun; Post ClearTarget.
//   Roundhouse      prince_rogue_roundhouse.luac        RANGE 220 FrontalFirst + LookAt; OnSkill_ pops the OnPre list (no re-search), objects rolled; Post ClearTarget.
inline constexpr RuntimeSkillUseProfileV1 kRuntimeSkillUseProfilesV1[] = {
    {"BashDown", "prince_warrior_bashdown", "Skill_Warrior_BashDown",
     160.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, true,
     false,
     SkillUseTargetsV1::pre_selected_single, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::frontal_first, false,
     true, true, false},
    {"GroundSlam", "prince_warrior_ground_slam", "Skill_Warrior_GroundSlam",
     0.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, false,
     true,
     SkillUseTargetsV1::requery_at_use, 0.0f, true, kSkillPiV1,
     SkillTargetSortV1::closest_first, true,
     true, true, false},
    {"Charge", "prince_warrior_charge", "Skill_Warrior_Charge",
     300.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, true,
     true,
     SkillUseTargetsV1::requery_at_use, 300.0f, false, 2.09439510239319549f,
     SkillTargetSortV1::source_order, false,
     true, false, true},
    {"JumpKick", "prince_rogue_jump_kick", "Skill_Rogue_JumpKick",
     250.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, true,
     false,
     SkillUseTargetsV1::requery_at_use, 160.0f, false, 4.71238898038468986f,
     SkillTargetSortV1::frontal_first, false,
     true, true, false},
    {"ViciousStrike", "prince_rogue_vicious_strike", "Skill_Rogue_ViciousStrike_Stun",
     300.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, true,
     false,
     SkillUseTargetsV1::requery_at_use, 160.0f, false, kSkillPiV1,
     SkillTargetSortV1::frontal_first, false,
     false, true, false},
    {"Roundhouse", "prince_rogue_roundhouse", "Skill_Rogue_Roundhouse",
     220.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, true,
     false,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::frontal_first, false,
     true, true, false},
};

// Returns the profile whose SkillTable (name, script) pair matches exactly, or nullptr.
inline const RuntimeSkillUseProfileV1* find_skill_use_profile_v1(
    const std::string& skill_name, const std::string& script) {
    for (const auto& profile : kRuntimeSkillUseProfilesV1) {
        if (skill_name == profile.skill_name && script == profile.script) return &profile;
    }
    return nullptr;
}

} // namespace dh::foundation::generic_skills
