#pragma once
// v21.8 w2-archer: use profiles of the five Archer-only active skills (prince_archer_*.luac, plain Lua text under
// .local-inputs/assets-extra/android/data/scripts/skills/). Same admission model as runtime_skill_use_profile_v1.hpp: the key
// is the SkillTable (name, script) pair. The generic coordinator runs the prefix every one of these Lua files shares:
//   OnSkillCheck_  HasBow() (requires_bow) + HasMana (+ not HasSkillCooldown for ExplodingArrows / PinShot)
//   OnPreSkill_    Enemy / ObjectFilter NONE / FrontalFirst TargetListSearch(RANGE[, ANGLE]) + LookAt(top) + UseMana
//                  (+ SetSkillCooldown(SnS_Cooldown) for ExplodingArrows and PinShot)
//   OnPostSkill_   ClearTarget
// and the host-supplied RuntimeArcherSkillOwnerV1 runs OnSkill_ (SpawnSkillProjectile) and the `delayed` / timer bodies.
//   ExplodingArrows prince_archer_exploding_arrows  RANGE 1200 ANGLE 220  cooldown  ArrowExplode  500 ms DoExplosion
//   MultiHitArrow   prince_archer_multihit          RANGE 600  ANGLE 220  -         ArrowDefault  3 do_skill markers, knockback
//   PinShot         prince_archer_pin_shot          RANGE 600  (no angle) cooldown  ArrowPinShot  750/1500 ms extra arrows, stun
//   PoisonArrows    prince_archer_poison_arrows     RANGE 600  ANGLE 220  -         ArrowPoison
//   Volley          prince_archer_volley            RANGE 900  ANGLE 270  -         ArrowDefault  GetRand rebound
// The Lua ANGLE is a full angle; the native half-angle comes from native_search_max_angle_v1.
#include "runtime_skill_use_profile_v1.hpp"

namespace dh::foundation::generic_skills {

inline constexpr RuntimeSkillUseProfileV1 kRuntimeSkillUseProfilesArcherV1[] = {
    {"ExplodingArrows", "prince_archer_exploding_arrows", "Skill_Archer_ExplodingArrows",
     1200.0f, native_search_max_angle_v1(220.0f), SkillTargetSortV1::frontal_first, true,
     true,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::frontal_first, false,
     false, true, false, nullptr, nullptr, true, false, 0.0f,
     nullptr, nullptr, nullptr, false, nullptr, nullptr, 0, true},
    {"MultiHitArrow", "prince_archer_multihit", "Skill_Archer_MultiHitArrow",
     600.0f, native_search_max_angle_v1(220.0f), SkillTargetSortV1::frontal_first, true,
     false,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::frontal_first, false,
     false, true, false, nullptr, nullptr, true, false, 0.0f,
     nullptr, nullptr, nullptr, false, nullptr, nullptr, 0, true},
    {"PinShot", "prince_archer_pin_shot", "Skill_Archer_PinShot",
     600.0f, kSkillPiV1, SkillTargetSortV1::frontal_first, true,
     true,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::frontal_first, false,
     false, true, false, nullptr, nullptr, true, false, 0.0f,
     nullptr, nullptr, nullptr, false, nullptr, nullptr, 0, true},
    {"PoisonArrows", "prince_archer_poison_arrows", "Skill_Archer_PoisonArrows",
     600.0f, native_search_max_angle_v1(220.0f), SkillTargetSortV1::frontal_first, true,
     false,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::frontal_first, false,
     false, true, false, nullptr, nullptr, true, false, 0.0f,
     nullptr, nullptr, nullptr, false, nullptr, nullptr, 0, true},
    {"Volley", "prince_archer_volley", "Skill_Archer_Volley",
     900.0f, native_search_max_angle_v1(270.0f), SkillTargetSortV1::frontal_first, true,
     false,
     SkillUseTargetsV1::pre_list_all, 0.0f, false, kSkillPiV1,
     SkillTargetSortV1::frontal_first, false,
     false, true, false, nullptr, nullptr, true, false, 0.0f,
     nullptr, nullptr, nullptr, false, nullptr, nullptr, 0, true},
};

// Returns the Archer profile whose SkillTable (name, script) pair matches exactly, or nullptr.
inline const RuntimeSkillUseProfileV1* find_skill_use_profile_archer_v1(
    const std::string& skill_name, const std::string& script) {
    for (const auto& profile : kRuntimeSkillUseProfilesArcherV1) {
        if (skill_name == profile.skill_name && script == profile.script) return &profile;
    }
    return nullptr;
}

} // namespace dh::foundation::generic_skills
