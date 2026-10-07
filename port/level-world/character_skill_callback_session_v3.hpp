#pragma once
#include "character_skill_callbacks_v3.hpp"
#include "character_script_session_v3.hpp"
namespace dh2::character::skills {
// Calls the actual same owned VM once per SetSkill/callback, projects ALL
// source returns, and snapshots only the return selected by the source caller.
// No generic same-VM reentry. Instance and Session survive this synchronous call.
int skill_callback_session_v3(CharacterScriptSessionV3&,const Instance32*,
 std::uint32_t operation,std::uint32_t* result,std::string& error);
int skill_callback_session_v3(CharacterScriptSession&,const Instance32*,
 std::uint32_t operation,std::uint32_t* result,std::string& error);
}
