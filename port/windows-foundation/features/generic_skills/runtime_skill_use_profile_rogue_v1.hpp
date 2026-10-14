#pragma once
// v21.8 skills-rogue: use profiles for the Rogue Assassin/Archer trees (subclass of Rogue).
//
// Same admission rule as runtime_skill_use_profile_v1.hpp: the exact SkillTable (name, script)
// pair. Each row is transcribed from the original skill Lua
// (.local-inputs/assets-extra/android/data/scripts/skills/<script>.luac, plain Lua text).
// Class cell indices are CharacterProperties indices verified against the authored
// pystructnames order (Level 19, SnS_Duration 176, Dot_Duration 125, Special_Sneak 198).
//
// Admitted here (partial, see coordination/v21.8/skills-rogue-report.md):
//   Blindness    prince_assassin_blindness.luac   buff arm (Skill_Assassin_Blindness). Duration = SnS_Duration.
//                suppress_selection: SetIsTargetable(false) on the caster for the buff's life (acquisition only).
//                Not reproduced: OnTargetHit break on the caster's own hit (no buff-break hook yet).
//   PoisonSting  prince_assassin_poison_sting.luac buff arm (Skill_Assassin_PoisonSting). Duration = Dot_Duration.
//                Not reproduced: the weapon DOT tick and Slow tails (the class Dot_*/Slow_* cells are applied to
//                the sheet; their consumption by the hit path is not verified).
//   Assassination  prince_assassin_assassination.luac  OnPre query + cooldown; OnSkill_ on the top Character:
//                  execute_below_level gate (not boss and raw Level < caster raw Level + 2 -> Character:Kill with the
//                  caster as killer, sound, no roll); otherwise a combat roll (BOSS_CLASS for bosses).
//
// Not admitted (no row; the coordinator rejects them with the generic "no Use arm" detail):
//   GrapplingHook  prince_assassin_grapling_hook.luac  BLOCKED-PROJECTILE (SpawnSkillProjectile + delayed pull).
//   Archer x5      prince_archer_{exploding_arrows,multihit,pin_shot,poison_arrows,volley}.luac  BLOCKED-PROJECTILE.
//                  The shared executor (features/effects/runtime_session_projectile_v1) is present but only enemy
//                  ranged monsters spawn through it; the player skill path does not.
#include "runtime_skill_use_profile_v1.hpp"

namespace dh::foundation::generic_skills {

// prince_assassin_blindness.luac: BUFF_ID Skill_Assassin_Blindness, BUFF_FX spell_dh2_assassin_blindness_persist,
// BUFF_TIME = GetProp(SnS_Duration, true) (no fallback in the Lua; the projected class value is 8000 + 2000*level ms).
// Blindness also SetIsTargetable(false) on the caster for the buff's life (suppress_selection, acquisition only).
inline constexpr RuntimeSkillBuffProfileV1 kBlindnessBuffV1 = {
    "Skill_Assassin_Blindness", "spell_dh2_assassin_blindness_persist", 0.0f, 0.0f, 0.0f, 176, true};

// prince_assassin_poison_sting.luac: BUFF_ID Skill_Assassin_PoisonSting, BUFF_FX skill_dh2_assassin_poison_sting_persist,
// BUFF_TIME = GetProp(Dot_Duration, true), 4000 ms when that is 0 (the Lua fallback).
inline constexpr RuntimeSkillBuffProfileV1 kPoisonStingBuffV1 = {
    "Skill_Assassin_PoisonSting", "skill_dh2_assassin_poison_sting_persist", 4000.0f, 0.0f, 0.0f, 125};

// Rows in the order of the Lua files. Values:
//   Blindness    prince_assassin_blindness    no OnPre query, no cooldown; OnPre RemoveBuff + UseMana; OnSkill_ CreateBuff/ApplyBuff; no Post.
//   PoisonSting  prince_assassin_poison_sting no OnPre query, no cooldown; OnPre RemoveBuff + UseMana; OnSkill_ CreateBuff/ApplyBuff; Post ClearTarget.
inline constexpr RuntimeSkillUseProfileV1 kRuntimeSkillUseProfilesRogueV1[] = {
    {"Blindness", "prince_assassin_blindness", "Skill_Assassin_Blindness",
     0.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, false,
     false,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::frontal_first, false,
     false, false, false, &kBlindnessBuffV1},
    {"PoisonSting", "prince_assassin_poison_sting", "Skill_Assassin_PoisonSting",
     0.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, false,
     false,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::frontal_first, false,
     false, true, false, &kPoisonStingBuffV1},
    // prince_assassin_assassination.luac: OnPre TargetListSearch(RANGE 100, all angles) ClosestFirst + LookAt(top), UseMana,
    // SetSkillCooldown(SnS_Cooldown). OnSkill_ TargetListSearch(100, ANGLE 180 -> half-angle 90) on the top Character:
    // execution gate (see RuntimeSkillUseProfileV1::execute_below_level). Post ClearTarget.
    // The sort of the OnSkill_ search is not written in the Lua (no sort argument); ClosestFirst is an inference.
    {"Assassination", "prince_assassin_assassination", "Skill_Assassin_Assassination",
     100.0f, kSkillPiV1, SkillTargetSortV1::closest_first, true,
     true,
     SkillUseTargetsV1::requery_at_use, 100.0f, false, native_search_max_angle_v1(180.0f),
     SkillTargetSortV1::closest_first, false,
     false, true, false, nullptr, nullptr, false, true, 0.0f,
     nullptr, nullptr, nullptr, false, nullptr, nullptr, 0,
     nullptr, nullptr, false, false, nullptr, nullptr,
     true, true, "Skill_Assassin_Assassination_boss", "sfx_passive_skill_activate_4"},
};

// Returns the Rogue-tree profile whose SkillTable (name, script) pair matches exactly, or nullptr.
inline const RuntimeSkillUseProfileV1* find_skill_use_profile_rogue_v1(
    const std::string& skill_name, const std::string& script) {
    for (const auto& profile : kRuntimeSkillUseProfilesRogueV1) {
        if (skill_name == profile.skill_name && script == profile.script) return &profile;
    }
    return nullptr;
}

} // namespace dh::foundation::generic_skills
