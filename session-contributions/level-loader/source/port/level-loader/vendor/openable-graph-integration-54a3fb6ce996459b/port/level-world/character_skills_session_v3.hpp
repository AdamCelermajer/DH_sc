#pragma once
#include "character_skills.hpp"
#include "character_script_session_v3.hpp"
#include "character_native_fsm.hpp"
#include "visual_fx_preload.hpp"
namespace dh2::character::skills {
// Borrowed existing private Session, genuine shared Debug services and live
// native FSM projection. All survive this object and every SkillOwner call.
// This adapter does not publish AIS, choose initial state, or deliver Init.
class CharacterSkillSessionServicesV3 {
 struct Impl;std::unique_ptr<Impl> impl_;
public:
 CharacterSkillSessionServicesV3(CharacterScriptSessionV3&,const fx::PreloadServices16&,
  const NativeFsm24&);
 ~CharacterSkillSessionServicesV3();
 CharacterSkillSessionServicesV3(const CharacterSkillSessionServicesV3&)=delete;
 CharacterSkillSessionServicesV3& operator=(const CharacterSkillSessionServicesV3&)=delete;
 CharacterSkillSessionServicesV3(CharacterSkillSessionServicesV3&&)=delete;
 CharacterSkillSessionServicesV3& operator=(CharacterSkillSessionServicesV3&&)=delete;
 Services16 services()noexcept;const std::string& error()const noexcept;
};
}
