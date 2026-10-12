#pragma once
#include "character_skills.hpp"
#include "character_script_session.hpp"
#include "character_native_fsm.hpp"
#include "visual_fx_preload.hpp"
namespace dh2::character::skills {
// Borrowed existing private Session, genuine shared Debug services and live
// native FSM projection. All survive this object and every SkillOwner call.
// This adapter does not publish AIS, choose initial state, or deliver Init.
class CharacterSkillSessionServices {
 struct Impl;std::unique_ptr<Impl> impl_;
public:
 CharacterSkillSessionServices(CharacterScriptSession&,const fx::PreloadServices16&,
  const NativeFsm24&);
 ~CharacterSkillSessionServices();
 CharacterSkillSessionServices(const CharacterSkillSessionServices&)=delete;
 CharacterSkillSessionServices& operator=(const CharacterSkillSessionServices&)=delete;
 CharacterSkillSessionServices(CharacterSkillSessionServices&&)=delete;
 CharacterSkillSessionServices& operator=(CharacterSkillSessionServices&&)=delete;
 Services16 services()noexcept;const std::string& error()const noexcept;
};
}
