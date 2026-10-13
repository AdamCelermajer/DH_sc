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
//   - (done) Buff owner: Quickness CreateBuff/ApplyBuff/StartSkillTimerCB run through runtime_timed_buff_v1.hpp.
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

// Lua CreateBuff/ApplyBuff arm (prince_rogue_quickness.luac): the Use call applies one timed buff instead of rolling hits.
struct RuntimeSkillBuffProfileV1 {
    const char* buff_class;       // ClassTable row of BUFF_ID (Skill_Rogue_Quickness_buff)
    const char* fx_set;           // AnimatedEffectTable name of BUFF_FX (spell_dh2_prince_rogue_quickness)
    float fallback_base_ms;       // TIME_BASE: used only when the projected SnS_Duration is 0
    float fallback_inc_ms;        // TIME_INC per skill level
    // v21.8 skills-knight: authored BUFF_TIME constant (Paladin auras: CreateBuff(BUFF_ID, 12000, ...)).
    // 0 = use the projected SnS_Duration / fallback as Quickness does.
    float fixed_duration_ms = 0.0f;
    // CharacterProperties index read as the buff duration (ms) after the projection. 176 = SnS_Duration
    // (Lua BUFF_TIME = GetProp(SnS_Duration, true)); PoisonSting reads 125 = Dot_Duration.
    int duration_prop_index = 176;
};

// One step of a volley cycle: the ClassTable row projected for that marker, and whether it rolls every target
// (top_target_only = false, Lua loop over the list) or only the first one (GetTargetListTop, no pop).
struct RuntimeSkillVolleyV1 {
    const char* class_token;
    bool top_target_only;
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
    // Timed-buff arm, or nullptr for hit-rolling skills.
    const RuntimeSkillBuffProfileV1* buff;
    // v21.8 mage: the Lua `target:PlayFX(GetPyOID("AnimatedEffectTable", NAME))` run for every Character target whose
    // SkillCombatRoll returned dmg > 0 (ColdRay: elem_dmg_02_water). Presentation only; nullptr when the Lua has none.
    const char* target_hit_fx_name = nullptr;
    // v21.8 mage: the Lua OnPreSkill_/OnSkill_ bodies are executed by the host-supplied
    // RuntimeSkillExternalOwnerV1 (skill projectiles / timers) instead of the generic SkillCombatRoll loop.
    bool external_owner = false;
    // v21.8 skills-knight: false when the authored Use loop applies no SkillCombatRoll (Paladin auras: buff only;
    // Warcry: SetScareState/SetStunState only). The Use still runs its target list and mana/cooldown prefix.
    bool rolls_combat = true;
    // Lua constant cooldown written by OnPreSkill_ (SetSkillCooldown(COOLDOWN)) that overrides the
    // class projection of SnS_Cooldown. 0 = use the projected class value (default for every row).
    float fixed_cooldown_ms = 0.0f;
    // v21.8 ES-API presentation arms (authored PlaySound / PlayFX). nullptr / false = the Lua has no such call.
    // Recorded on the cast receipt in authored order; the host dispatches them (see SkillPresentationCueV1).
    const char* use_sound_name = nullptr;        // PlaySound(name, false, 0, false) at Use, before the target loop
    const char* self_fx_name = nullptr;          // PlayFX(name) on the caster at Use
    const char* target_fx_each_name = nullptr;   // target:PlayFX(name) for EVERY Character target at Use (no dmg gate)
    bool target_fx_each_non_player_only = false; // Eclipse: that PlayFX sits under `if not target:IsPlayer()`
    const char* pre_target_fx_name = nullptr;    // target:PlayFX(name) for every Character target popped at OnPre
    // v21.8 es-runtime: per-volley Use (Lua OnSkill_ `skill_inc` cycle). The n-th distinct do_skill marker of a cast uses
    // volleys[n % volley_count]. nullptr / 0 = every marker runs the row's own class and targets (default for every row).
    const RuntimeSkillVolleyV1* volleys = nullptr;
    std::uint8_t volley_count = 0;
};

// The volley that a Use selects. `cursor` counts the Uses already delivered for this actor and skill (it persists across
// casts, like the Lua file-level skill_inc). Returns nullptr when the profile has no volley plan.
inline const RuntimeSkillVolleyV1* select_volley_v1(const RuntimeSkillUseProfileV1& profile, std::uint32_t cursor) {
    if (!profile.volleys || profile.volley_count == 0) return nullptr;
    return &profile.volleys[cursor % profile.volley_count];
}

inline constexpr float kSkillPiV1 = 3.1415927410125732421875f;

// prince_rogue_quickness.luac: BUFF_ID Skill_Rogue_Quickness_buff, BUFF_FX spell_dh2_prince_rogue_quickness, TIME_BASE 4000, TIME_INC 1000.
inline constexpr RuntimeSkillBuffProfileV1 kQuicknessBuffV1 = {
    "Skill_Rogue_Quickness_buff", "spell_dh2_prince_rogue_quickness", 4000.0f, 1000.0f};

// Rows in the order of the Lua files. Values:
//   BashDown        prince_warrior_bashdown.luac        RANGE 160; OnPre TargetListSearch(RANGE) + LookAt; OnSkill_ uses OnPre target; Post ClearTarget.
//   GroundSlam      prince_warrior_ground_slam.luac     OnPre has no target query; OnSkill_ search range=TempProp1, ClosestFirst, LookAt; SetSkillCooldown in OnPre; Post ClearTarget.
//   Charge          prince_warrior_charge.luac          RANGE 300 FrontalFirst + LookAt; OnSkill_ NoSort, RANGE 300, ANGLE 120; SetSkillCooldown; conditional Post.
//   JumpKick        prince_rogue_jump_kick.luac         INIT_RANGE 250 + MaxAngleSearch + LookAt; OnSkill_ RANGE 160, ANGLE 270; Post ClearTarget.
//   ViciousStrike   prince_rogue_vicious_strike.luac    SEARCH_RANGE 300 + MaxAngleSearch + LookAt; OnSkill_ RANGE 160, ANGLE 180, character-only rolls; CLASS = _Stun; Post ClearTarget.
//   Roundhouse      prince_rogue_roundhouse.luac        RANGE 220 FrontalFirst + LookAt; OnSkill_ pops the OnPre list (no re-search), objects rolled; Post ClearTarget.
//   Quickness       prince_rogue_quickness.luac         no OnPre query, no cooldown; OnPre RemoveBuff + UseMana; OnSkill_ CreateBuff(BUFF_ID, bufftime, false, level, BUFF_FX) + ApplyBuff; Post ClearTarget.
//   ColdRay         prince_mage_coldray.luac            RANGE 650 FrontalFirst + MaxAngleSearch backup + LookAt(top); OnSkill_ TargetListSearch(RANGE, ANGLE 90, "MaxAngleSearch"),
//                                                       ApplyPropClass(Skill_Mage_ColdRay), SkillCombatRoll for every popped target (objects too); no SetSkillCooldown;
//                                                       target:PlayFX(elem_dmg_02_water) per Character with dmg > 0 (presentation, see runtime_skill_use_fx); Post ClearTarget.
inline constexpr RuntimeSkillUseProfileV1 kRuntimeSkillUseProfilesV1[] = {
    {"BashDown", "prince_warrior_bashdown", "Skill_Warrior_BashDown",
     160.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, true,
     false,
     SkillUseTargetsV1::pre_selected_single, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::frontal_first, false,
     true, true, false, nullptr},
    {"GroundSlam", "prince_warrior_ground_slam", "Skill_Warrior_GroundSlam",
     0.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, false,
     true,
     SkillUseTargetsV1::requery_at_use, 0.0f, true, kSkillPiV1,
     SkillTargetSortV1::closest_first, true,
     true, true, false, nullptr},
    {"Charge", "prince_warrior_charge", "Skill_Warrior_Charge",
     300.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, true,
     true,
     SkillUseTargetsV1::requery_at_use, 300.0f, false, 2.09439510239319549f,
     SkillTargetSortV1::source_order, false,
     true, false, true, nullptr},
    {"JumpKick", "prince_rogue_jump_kick", "Skill_Rogue_JumpKick",
     250.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, true,
     false,
     SkillUseTargetsV1::requery_at_use, 160.0f, false, 4.71238898038468986f,
     SkillTargetSortV1::frontal_first, false,
     true, true, false, nullptr},
    {"ViciousStrike", "prince_rogue_vicious_strike", "Skill_Rogue_ViciousStrike_Stun",
     300.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, true,
     false,
     SkillUseTargetsV1::requery_at_use, 160.0f, false, kSkillPiV1,
     SkillTargetSortV1::frontal_first, false,
     false, true, false, nullptr},
    {"Roundhouse", "prince_rogue_roundhouse", "Skill_Rogue_Roundhouse",
     220.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, true,
     false,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::frontal_first, false,
     true, true, false, nullptr},
    {"Quickness", "prince_rogue_quickness", "Skill_Rogue_Quickness",
     0.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, false,
     false,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::frontal_first, false,
     false, true, false, &kQuicknessBuffV1},
    // v21.8 mage. ANGLE 90 degrees -> 1.57079637 rad (same degree->radian convention as JumpKick 270 / Charge 120).
    {"ColdRay", "prince_mage_coldray", "Skill_Mage_ColdRay",
     650.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, true,
     false,
     SkillUseTargetsV1::requery_at_use, 650.0f, false, 1.57079637050628662109375f,
     SkillTargetSortV1::frontal_first, false,
     true, true, false, nullptr, "elem_dmg_02_water"},
    // prince_mage_chain_lightning.luac: OnPre TargetListSearch(RANGE 1000) FrontalFirst + LookAt(top); OnSkill_ pops the Pre
    // list until the first Character, plays sfx_skill_mage_thunder_braid and SpawnSkillProjectile(SimplifiedChainLightning,
    // self, target); the chain continues in delayed(). No SetSkillCooldown. Post ClearTarget.
    {"ChainLightning", "prince_mage_chain_lightning", "Skill_Chain_Lightning",
     1000.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, true,
     false,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::frontal_first, false,
     false, true, false, nullptr, nullptr, true},
    // prince_mage_cyclone.luac: OnPre TargetListSearch(LOOK_AT_RANGE 1500, ANGLE 360) FrontalFirst + LookAt(top); OnSkill_
    // CreateTornado() = SpawnSkillProjectile(Tornado) + a 500 ms tick timer. Post ClearTarget.
    {"Cyclone", "prince_mage_cyclone", "Skill_Cyclone",
     1500.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, true,
     false,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::frontal_first, false,
     false, true, false, nullptr, nullptr, true},
    // prince_mage_stone_escape.luac: OnPre has no target query (PlayFX at the old position, TeleportTool projectile,
    // UseMana); SetSkillCooldown is commented out in the Lua; OnSkill_ WarpTo + statue + 2200 ms explosion. The Lua
    // registers no Post, so no ClearTarget.
    {"StoneEscape", "prince_mage_stone_escape", "Skill_Mage_StoneEscape",
     0.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, false,
     false,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::frontal_first, false,
     false, false, false, nullptr, nullptr, true},
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
