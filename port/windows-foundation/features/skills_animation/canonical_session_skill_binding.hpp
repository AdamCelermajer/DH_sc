#pragma once

#include "native_skill_lua_services.hpp"
#include "session_skill_animation.hpp"

namespace dh::foundation::skills_animation {

// Install the original scoped SkillCallback transport on an adapter that
// already borrows the canonical actor's native owners. The Session is borrowed
// and must outlive the adapter. Scope is re-read for each synchronous callback;
// it is never retained by this binding.
void bind_canonical_skill_callbacks(NativeSkillLuaServicesV1&,
 dh2::character::CharacterScriptSession&);
void bind_canonical_skill_callbacks(NativeSkillLuaServices&,
 dh2::character::CharacterScriptSessionV3&);

// Wire the native skill AI/FSM delegates into SessionSkillAnimation. The
// NativeSkillLuaServices object must already have been constructed from the
// canonical loan's actual Session, owner, FSM, AI, state, and providers.
void bind_canonical_session_skill_services(SessionSkillServices&,
 NativeSkillLuaServicesV1&,dh2::character::CharacterScriptSession&);
void bind_canonical_session_skill_services(SessionSkillServices&,
 NativeSkillLuaServices&,dh2::character::CharacterScriptSessionV3&);

// Bind source-plan playback to CombatSession's existing retained actor pose
// owner. Completion is accepted only once the same retained pose reports its
// source timeline ended; no second playback owner or clock is created.
void bind_session_skill_pose_services(SessionSkillServices&);

// V3 player counterpart to enemy_ai::scoped_skill_callback. It runs the
// original SkillCallback choreography and uses the same-session current
// native capability when called reentrantly from DoSkill.
int scoped_canonical_skill_callback(dh2::character::CharacterScriptSessionV3&,
 const dh2::character::skills::Instance32*,std::uint32_t operation,
 std::uint32_t* result,const dh2_script_callback_scope*,std::string& error);

}
