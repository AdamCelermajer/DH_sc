#pragma once
#include "character_skills.hpp"
#include "character_script_owner.hpp"
#include <functional>
namespace dh2::character::skills {
// Native callbacks stored by the original Skill.SpawnProjectile. Source
// userdata is the SAME owned Instance32, never a reconstructed skill/index.
enum class ProjectileSkillCallbackV112 {hit,check};
struct ProjectileSkillScriptLoanV112 {
 std::shared_ptr<void> owner;
 ScriptSessionView script{}; // identity0 is the observed Character3e4 NULL
 const dh2_script_callback_scope* scope{}; // synchronous current capability
};
struct ProjectileSkillCallbackServicesV112 {
 std::shared_ptr<void> owner;
 std::function<bool(const Instance32&,std::string&)> current_instance;
 // Query Character3e4 afresh at BOTH original call sites. A positive script
 // requires its actual VM/aliases and receiver lifetime; absence is valid only
 // at the first source null guard. Scope may only name this same live VM.
 std::function<bool(std::uintptr_t,ProjectileSkillScriptLoanV112&,std::string&)> main_script;
};
// Whole _ProjectileHit3dae7c/_ProjectileCheck3daff4 return choreography:
// NULL main script=>1; SetSkill error=>0; reload main script; exact delayed
// callback collision UserData + projectile pointer; first BOOL false=>1.
// Lua errors are source return0; native required failures remain failures.
bool skill_projectile_callback_v112(ProjectileSkillCallbackV112,
 const Instance32&,std::uintptr_t projectile,std::uintptr_t collision_object,
 const ProjectileSkillCallbackServicesV112&,std::int32_t&,std::string&);
}
