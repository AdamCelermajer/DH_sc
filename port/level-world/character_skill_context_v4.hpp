#pragma once
#include "character_skill_state_v4.hpp"
#include "character_player_skills_v3.hpp"
#include "character_animation_instance.hpp"
namespace dh2::character::skills {
// Borrow the already retained V3 owner, FSM State56 and AnimationInstance.
// No VM, timer store, animation clock, target provider or second FSM is created.
// All borrowed objects and provider context must outlive this adapter/callbacks.
class CharacterSkillContextV4 final {
 CharacterPlayerSkillsV3& player_;
 NativeFsm24& fsm_;
 CharacterAnimationInstance& animation_;
 SkillStateV4& fields_;
 SkillStateServices16V4 required_;
 int status_=0;
 static int service(void*,SkillStateV4*,const SkillStateRequest32V4*,SkillStateResponse16V4*);
 bool coherent()const noexcept;
public:
 CharacterSkillContextV4(CharacterPlayerSkillsV3&,NativeFsm24&,
  CharacterAnimationInstance&,SkillStateV4&,const SkillStateServices16V4&);
 CharacterSkillContextV4(const CharacterSkillContextV4&)=delete;
 CharacterSkillContextV4& operator=(const CharacterSkillContextV4&)=delete;
 int execute(std::uint32_t operation,std::uint32_t index=0,
  std::uint32_t moving=0,std::uintptr_t payload=0,std::uint32_t force=0);
 // Caller composes this into the existing observer. Authored event0x28 uses
 // the actual immutable payload string; closure/null payload is not do_skill.
 int animation_event(const actor::BlendedPlaybackEvent&);
 int status()const noexcept{return status_;}
 CharacterScriptSessionV3& session()noexcept{return player_.session();}
 State& state()noexcept{return *fsm_.state;}
 CharacterAnimationInstance& animation()noexcept{return animation_;}
};
}
