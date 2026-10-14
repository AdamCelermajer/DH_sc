#pragma once
// v21.8 skills-knight: data-driven use profiles for the remaining Knight-tree active skills
// (Berserker and Paladin subclasses). BashDown, Charge and GroundSlam stay in runtime_skill_use_profile_v1.hpp.
//
// Admission key: the SkillTable (name, script) pair, exactly as in runtime_skill_use_profile_v1.hpp.
// Every number and behaviour below is transcribed from the original skill Lua under
// .local-inputs/assets-extra/android/data/scripts/skills/<script>.luac (plain Lua text). The class rows
// (ClassTable, read with probe/classdump) supply the damage/aura formulas; they are not re-encoded here.
//
// Berserker rows (phase 2b, coordination/v21.8/w2-berserker-report.md): Berserk (three volleys through the per-volley
// plan, OnPre self-HP cost), Intimidation (scare/stun arm over the OnPre list, pre_list_all).
//
// Fields NOT encoded (documented gaps):
//   - Pushback / knockback / push-chance tails (WhirlingDrift SnS_Push_Target_Chance, DivineJudgement push row).
//   - Warcry scare/stun states (SetScareState / SetStunState) and their FX/sound. The port has no such state.
//   - Divine Judgement per-target OnPre PlayFX (spell_dh2_prince_warrior_paladin_divine_judgement): pre_target_fx_name (ES-API).
//   - Aura self PlayFX(BUFF_FX) at Use: self_fx_name (w2-paladin). The persistent buff FX (CreateBuff fx arg) is played by the buff owner.
//   - Purifying Aura Heal(self): self_heal_per_rank = HEAL_AMOUNT 15 (w2-paladin), an immediate SetProp(HP) write.
//     Ally Heal / ally buffs are not applied (see the ally note above).
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

// prince_berseeker_berserk.luac OnSkill_: skill_inc cycle of three volleys, each re-searching TargetListSearch(200, 360).
// Volley 0 stuns every Character (Skill_Berserk_Berserk_Stun), volley 1 damages the top target (_Damage, also the mana
// row), volley 2 knocks back the top target (_KnockBack). Rows of the plan are chosen by the per-(actor, skill) cursor.
inline constexpr RuntimeSkillVolleyV1 kBerserkVolleysV1[] = {
    {"Skill_Berserk_Berserk_Stun", false},
    {"Skill_Berserk_Berserk_Damage", true},
    {"Skill_Berserk_Berserk_KnockBack", true},
};
// prince_berseeker_berserk.luac OnPreSkill_: `if HPPercentCurr > 10 then SetProp(HP, health - health/10)`.
inline constexpr RuntimeSkillSelfHpCostV1 kBerserkSelfHpCostV1 = {10.0f, 10.0f};
// prince_berseeker_warcry.luac: GetRand(1, 4) (stun on 3, scare otherwise), fallback bufftime 2000 ms, sounds at the end of OnSkill_.
inline constexpr RuntimeSkillStatusArmV1 kWarcryStatusArmV1 = {
    4u, 3u, 2000.0f, "sfx_skill_warrior_bsk_bloodmoon_howl_hit_fear", "sfx_skill_warrior_bsk_bloodmoon_howl_hit_stun"};

// Rows in the order of the Lua files. Values:
//   Berserk         prince_berseeker_berserk.luac         OnPre TargetListSearch(RANGE 200) FrontalFirst + LookAt(top), HP cost, UseMana;
//                                                         OnSkill_ three volleys (kBerserkVolleysV1), SkillCombatRoll per Character
//                                                         (volley 0) or top target (1, 2); Post ClearTarget.
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
//                   UseMana; OnSkill_ pops the OnPre list: scare/stun per non-boss Character (kWarcryStatusArmV1), no
//                   SkillCombatRoll; Post empty. Bufftime timer (StartSkillTimerCB BuffExpired) is cosmetic here, not modelled.
inline constexpr RuntimeSkillUseProfileV1 kRuntimeSkillUseProfilesKnightV1[] = {
    {"Berserk", "prince_berseeker_berserk", "Skill_Berserk_Berserk_Damage",
     200.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, true,
     false,
     SkillUseTargetsV1::requery_at_use, 200.0f, false, kSkillPiV1,
     SkillTargetSortV1::frontal_first, false,
     false, true, false, nullptr, nullptr, false, true, 0.0f,
     nullptr, nullptr, nullptr, false, nullptr,
     kBerserkVolleysV1, 3, &kBerserkSelfHpCostV1, nullptr},
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
     true, true, false, nullptr, nullptr, false, true, 0.0f,
     nullptr, nullptr, nullptr, false, "spell_dh2_prince_warrior_paladin_divine_judgement"},
    {"OffensiveAura", "prince_paladin_offensive_aura", "Skill_Paladin_OffensiveAura",
     0.0f, kSkillPiV1, SkillTargetSortV1::source_order, false,
     false,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::source_order, false,
     false, true, false, &kOffensiveAuraBuffV1, nullptr, false, false, 0.0f, nullptr, "skill_dh2_prince_warrior_paladin_offensive_aura", nullptr, false, nullptr, nullptr, 0, nullptr, nullptr, false, false, 0.0f},
    {"GuardingAura", "prince_paladin_guarding_aura", "Skill_Paladin_GuardingAura",
     0.0f, kSkillPiV1, SkillTargetSortV1::source_order, false,
     false,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::source_order, false,
     false, true, false, &kGuardingAuraBuffV1, nullptr, false, false, 0.0f, nullptr, "skill_dh2_prince_warrior_paladin_guarding_aura", nullptr, false, nullptr, nullptr, 0, nullptr, nullptr, false, false, 0.0f},
    {"PurifyingAura", "prince_paladin_purifying_aura", "Skill_Paladin_PurifyingAura",
     0.0f, kSkillPiV1, SkillTargetSortV1::source_order, false,
     false,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::source_order, false,
     false, true, false, &kPurifyingAuraBuffV1, nullptr, false, false, 0.0f, nullptr, "skill_dh2_prince_warrior_paladin_purifying_aura", nullptr, false, nullptr, nullptr, 0, nullptr, nullptr, false, false, 15.0f},
    // Warcry's OnSkill_ loops the OnPre list (PopTargetList, no re-search): the scare/stun arm reaches every non-boss
    // Character of that list, not only the OnPre target (es-berserker phase-1 correction, prince_berseeker_warcry.luac 95-127).
    {"Intimidation", "prince_berseeker_warcry", "Skill_Berserk_Intimidation",
     600.0f, kSkillPiV1, SkillTargetSortV1::closest_first, true,
     false,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::closest_first, false,
     false, false, false, nullptr, nullptr, false, false, 0.0f,
     nullptr, nullptr, nullptr, false, nullptr,
     nullptr, 0, nullptr, &kWarcryStatusArmV1},
};


} // namespace dh::foundation::generic_skills
