#pragma once

// Registry lookup over every use-profile group table: the shared table first, then the Knight and Rogue groups.
// Admission stays exact on the SkillTable (name, script) pair. A new group adds one header and one line here.
#include "runtime_skill_use_profile_knight_v1.hpp"
#include "runtime_skill_use_profile_rogue_v1.hpp"

namespace dh::foundation::generic_skills {

inline const RuntimeSkillUseProfileV1* find_skill_use_profile_registered_v1(
    const std::string& skill_name, const std::string& script) {
    if (const auto* base = find_skill_use_profile_v1(skill_name, script)) return base;
    for (const auto& profile : kRuntimeSkillUseProfilesKnightV1) {
        if (skill_name == profile.skill_name && script == profile.script) return &profile;
    }
    return find_skill_use_profile_rogue_v1(skill_name, script);
}

} // namespace dh::foundation::generic_skills
