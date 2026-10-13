#pragma once
// v21.8 skills-knight: data-driven use profiles for the remaining Knight-tree active skills
// (Berserker and Paladin subclasses). BashDown, Charge and GroundSlam stay in runtime_skill_use_profile_v1.hpp.
//
// Admission key: the SkillTable (name, script) pair, exactly as in runtime_skill_use_profile_v1.hpp.
// Every number and behaviour below is transcribed from the original skill Lua under
// .local-inputs/assets-extra/android/data/scripts/skills/<script>.luac (plain Lua text). The class rows
// (ClassTable, read with probe/classdump) supply the damage/aura formulas; they are not re-encoded here.
//
// Rows NOT admitted here (see coordination/v21.8/skills-knight-report.md):
//   - Berserk (prince_berseeker_berserk): three Use volleys with three different class sheets (stun over all
//     enemies in 360, damage and knockback on the top target) and an OnPre self-HP cost. One class sheet per
//     cast cannot express it.
//
// Fields NOT encoded (documented gaps):
//   - Pushback / knockback / push-chance tails (WhirlingDrift SnS_Push_Target_Chance, DivineJudgement push row).
//   - Warcry scare/stun states (SetScareState / SetStunState) and their FX/sound. The port has no such state.
//   - Divine Judgement per-target OnPre PlayFX (spell_dh2_prince_warrior_paladin_divine_judgement).
//   - Aura self PlayFX(BUFF_FX) at Use. The persistent buff FX (CreateBuff fx arg) is played by the buff owner.
//   - Purifying Aura Heal(): SetBuffProp(HP) by skill_level * 15 (HEAL_AMOUNT). The class row only carries
//     Regen_HP / CombatRegen_HP.
//   - Ally buffs. The Lua applies the aura to other "Player" characters inside 6000 units. The single-player
//     port has no other player character, so only the caster's own buff is applied (inference, see report).
//   - WhirlingDrift forward movement (animation moving_flag=1 in the AnimTable; the Lua has no movement call).
#include "runtime_skill_use_profile_v1.hpp"

#include <string>

namespace dh::foundation::generic_skills {

// prince_paladin_offensive_aura.luac: BUFF_ID Skill_Paladin_OffensiveAura, BUFF_FX_PERSIST
// skill_dh2_prince_warrior_paladin_offensive_aura_persist, BUFF_TIME 12000.
inline constexpr RuntimeSkillBuffProfileV1 kOffensiveAuraBuffV1 = {
    "Skill_Paladin_OffensiveAura", "skill_dh2_prince_warrior_paladin_offensive_aura_persist",
    12000.0f, 0.0f, 12000.0f};

// prince_paladin_guarding_aura.luac: BUFF_ID Skill_Paladin_GuardingAura, BUFF_PERSIST
// skill_dh2_prince_warrior_paladin_guarding_aura_persist, BUFF_TIME 12000.
inline constexpr RuntimeSkillBuffProfileV1 kGuardingAuraBuffV1 = {
    "Skill_Paladin_GuardingAura", "skill_dh2_prince_warrior_paladin_guarding_aura_persist",
    12000.0f, 0.0f, 12000.0f};

// prince_paladin_purifying_aura.luac: BUFF_ID Skill_Paladin_PurifyingAura, BUFF_PERSIST
// skill_dh2_prince_warrior_paladin_purifying_aura_persist, BUFF_TIME 12000.
inline constexpr RuntimeSkillBuffProfileV1 kPurifyingAuraBuffV1 = {
    "Skill_Paladin_PurifyingAura", "skill_dh2_prince_warrior_paladin_purifying_aura_persist",
    12000.0f, 0.0f, 12000.0f};

// Rows in the order of the Lua files. Values:
//   WhirlingDrift   prince_berseeker_whirling_drift.luac  OnPre TargetListSearch(RANGE 360), NoSort, no LookAt, UseMana;
//                   OnSkill_ pops the OnPre list (no re-search), ApplyPropClass(CLASS_ID), SkillCombatRoll per target;
//                   Post ClearTarget. No cooldown.
//   DivineJudgement prince_paladin_divine_judgement.luac  OnPre TargetListSearch(RANGE 600), ClosestFirst, PlayFX per
//                   target (gap), SetSkillCooldown(SnS_Cooldown), UseMana; OnSkill_ requeries TargetListSearch(600, 360)
//                   with ClosestFirst, ApplyPropClass, SkillCombatRoll per target; Post ClearTarget.
//   OffensiveAura   prince_paladin_offensive_aura.luac    no OnPre query; OnPre RemoveBuff + UseMana; OnSkill_ CreateBuff
//                   (BUFF_TIME 12000, strength = skill level) + ApplyBuff; no combat roll; Post ClearTarget.
//   GuardingAura    prince_paladin_guarding_aura.luac     as OffensiveAura (Skill_Paladin_GuardingAura, BUFF_TIME 12000,
//                   BUFF_PERSIST): OnPre RemoveBuff + UseMana; OnSkill_ CreateBuff + ApplyBuff; Post ClearTarget.
//   PurifyingAura   prince_paladin_purifying_aura.luac    no OnPre query; OnPre UseMana; OnSkill_ RemoveBuff + heal (gap)
//                   + CreateBuff/ApplyBuff (BUFF_TIME 12000); Post ClearTarget.
//   Intimidation    prince_berseeker_warcry.luac          OnPre TargetListSearch(RANGE 600), ClosestFirst, LookAt(top),
//                   UseMana; OnSkill_ scare/stun states on the OnPre target only (gap), no SkillCombatRoll; Post empty.
inline constexpr RuntimeSkillUseProfileV1 kRuntimeSkillUseProfilesKnightV1[] = {
    {"WhirlingDrift", "prince_berseeker_whirling_drift", "Skill_Berserk_WhirlingDrift",
     360.0f, kSkillPiV1, SkillTargetSortV1::source_order, false,
     false,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::source_order, false,
     true, true, false, nullptr, nullptr, false, true},
    {"DivineJudgement", "prince_paladin_divine_judgement", "Skill_Paladin_DivineJudgementAura",
     600.0f, kSkillPiV1, SkillTargetSortV1::closest_first, false,
     true,
     SkillUseTargetsV1::requery_at_use, 600.0f, false, kSkillPiV1,
     SkillTargetSortV1::closest_first, false,
     true, true, false, nullptr, nullptr, false, true},
    {"OffensiveAura", "prince_paladin_offensive_aura", "Skill_Paladin_OffensiveAura",
     0.0f, kSkillPiV1, SkillTargetSortV1::source_order, false,
     false,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::source_order, false,
     false, true, false, &kOffensiveAuraBuffV1, nullptr, false, false},
    {"GuardingAura", "prince_paladin_guarding_aura", "Skill_Paladin_GuardingAura",
     0.0f, kSkillPiV1, SkillTargetSortV1::source_order, false,
     false,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::source_order, false,
     false, true, false, &kGuardingAuraBuffV1, nullptr, false, false},
    {"PurifyingAura", "prince_paladin_purifying_aura", "Skill_Paladin_PurifyingAura",
     0.0f, kSkillPiV1, SkillTargetSortV1::source_order, false,
     false,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::source_order, false,
     false, true, false, &kPurifyingAuraBuffV1, nullptr, false, false},
    {"Intimidation", "prince_berseeker_warcry", "Skill_Berserk_Intimidation",
     600.0f, kSkillPiV1, SkillTargetSortV1::closest_first, true,
     false,
     SkillUseTargetsV1::pre_selected_single, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::source_order, false,
     false, false, false, nullptr, nullptr, false, false},
};


} // namespace dh::foundation::generic_skills
