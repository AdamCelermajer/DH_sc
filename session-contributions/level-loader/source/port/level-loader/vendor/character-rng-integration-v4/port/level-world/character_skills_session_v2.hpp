#pragma once
#include "character_skills.hpp"
#include "character_script_session_v2.hpp"
#include "character_native_fsm.hpp"
#include "visual_fx_preload.hpp"
namespace dh2::character::skills {
// Borrowed existing private Session, genuine shared Debug services and live
// native FSM projection. All survive this object and every SkillOwner call.
// This adapter does not publish AIS, choose initial state, or deliver Init.
class CharacterSkillSessionServicesV2 {
 struct Impl;std::unique_ptr<Impl> impl_;
public:
 CharacterSkillSessionServicesV2(CharacterScriptSessionV2&,const fx::PreloadServices16&,
  const NativeFsm24&);
 ~CharacterSkillSessionServicesV2();
 CharacterSkillSessionServicesV2(const CharacterSkillSessionServicesV2&)=delete;
 CharacterSkillSessionServicesV2& operator=(const CharacterSkillSessionServicesV2&)=delete;
 CharacterSkillSessionServicesV2(CharacterSkillSessionServicesV2&&)=delete;
 CharacterSkillSessionServicesV2& operator=(CharacterSkillSessionServicesV2&&)=delete;
 Services16 services()noexcept;const std::string& error()const noexcept;
};
}
