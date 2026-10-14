#pragma once
// v21.8 w2-necromancer: use profiles for the Necromancer active skills whose Lua runs skill timers, death watches or
// skill projectiles (FireSkullRain, Pestilence, Rise). DeathClaw stays in runtime_skill_use_profile_magesub_v1.hpp (its
// row carries knockback_great_on_character_hit). Passives have no use profile (see runtime_necromancer_passive_owner_v1).
//
// Values are transcribed from the original Lua (plain text under .local-inputs/assets-extra/android/data/scripts/skills/).
//   FireSkullRain  prince_necromancer_fireskullrain.luac  OnPre: UseMana + SetSkillCooldown(SnS_Cooldown). No target query.
//                  OnSkill_: StartSkillTimerCB(1000, true, DamageTick) once. DamageTick: TargetListSearch(280, 360) around
//                  the caster (enemy characters, object filter NONE, NoSort), SkillCombatRoll on each target; 3 rolling
//                  ticks (tick_count 0,1,2), stopped early when the list is empty. Post ClearTarget.
//   Pestilence     prince_necromancer_pestilence.luac    OnPre: TargetListSearch(RANGE = TempProp1, 240) FrontalFirst +
//                  LookAt(top); UseMana; no SetSkillCooldown (C6). OnSkill_: SkillCombatRoll each new character (infect),
//                  objects rolled; RemoveTarget timer at SnS_Dot_Duration + 1000. UpdateAttackedTargets: 500 ms loop, a dead
//                  victim explodes (TargetListSearch(200, 360, victim)). Post ClearTarget.
//   Rise           prince_necromancer_rise.luac          OnPre: UseMana + DoRise (SpawnSkillProjectile TeleportTool).
//                  OnSkill_: checkslot/spawn (summon: deferred, see the owner). Delayed landing records the position.
//                  Post ClearTarget.
#include "runtime_skill_target_query_v1.hpp"
#include "runtime_skill_use_profile_v1.hpp"

namespace dh::foundation::generic_skills {

inline constexpr RuntimeSkillUseProfileV1 kRuntimeSkillUseProfilesNecromancerV1[] = {
    // FireSkullRain: no OnPre target query (pre_range 0). OnPre cooldown = class SnS_Cooldown (3250 ms flat).
    {"FireSkullRain", "prince_necromancer_fireskullrain", "Skill_Necromancer_FireSkullRain",
     0.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, false,
     true,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::source_order, false,
     false, true, false, nullptr, nullptr, true, true, 0.0f,
     nullptr, nullptr, nullptr, false, nullptr, nullptr, 0, nullptr, nullptr, false},
    // Pestilence: OnPre search range = TempProp1 (400 + 20 L), half cone 240 degrees, LookAt(top); no cooldown.
    {"Pestilence", "prince_necromancer_pestilence", "Skill_Necromancer_Pestilence",
     0.0f, native_search_max_angle_v1(240.0f), SkillTargetSortV1::frontal_first, true,
     false,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::closest_first, false,
     false, true, false, nullptr, nullptr, true, true, 0.0f,
     nullptr, nullptr, nullptr, false, nullptr, nullptr, 0, nullptr, nullptr, true},
    // Rise: no OnPre target query; no cooldown; the Use is the summon/projectile body (external owner).
    {"Rise", "prince_necromancer_rise", "Skill_Necromancer_Rise",
     0.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, false,
     false,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::frontal_first, false,
     false, true, false, nullptr, nullptr, true, true, 0.0f,
     nullptr, nullptr, nullptr, false, nullptr, nullptr, 0, nullptr, nullptr, false},
};

// Returns the Necromancer profile whose SkillTable (name, script) pair matches exactly, or nullptr.
inline const RuntimeSkillUseProfileV1* find_skill_use_profile_necromancer_v1(
    const std::string& skill_name, const std::string& script) {
    for (const auto& profile : kRuntimeSkillUseProfilesNecromancerV1) {
        if (skill_name == profile.skill_name && script == profile.script) return &profile;
    }
    return nullptr;
}

} // namespace dh::foundation::generic_skills
