#pragma once
#include "source_skill_animation.hpp"
#include "skill_animation_program.hpp"
#include "../../combat_session.hpp"
#include "../../../game-data/properties.hpp"
namespace dh::foundation::skills_animation {
struct SkillActorBorrow {
 ActorState* actor=nullptr;
 dh2::data::PropertyView* properties=nullptr;
 dh2::character::skills::SkillAIContextV3* ai=nullptr;
 dh2::character::skills::SkillStateV4* state=nullptr;
};
struct SessionSkillServices {
 // Fresh same-session borrow. Changed pointers require rebinding before use.
 std::function<bool(CombatSession&,ActorId,SkillActorBorrow&,std::string&)> borrow;
 std::function<bool(ActorId,std::uint32_t native_index,std::int32_t& row,std::string&)> row;
 std::function<bool(ActorId,std::vector<int>& positions,std::string&)> hotbar;
 std::function<bool(ActorId,int class_position,std::uint32_t& native_index,std::string&)> native_index;
 // Provides exact hierarchy choices/rates from the same original animator/RNG.
 std::function<bool(ActorId,std::int32_t root,OriginalAttackSelection&,std::string&)> selection;
 // Must reuse Session's existing retained pose/slots; never ordinary visual.update.
 std::function<bool(CombatSession&,ActorId,const OriginalCombatVisualPlan&,
  const OriginalSequencePolicies&,const OriginalAttackSelection&,
  CombatSessionStateAnimationServices,std::string&)> play;
 std::function<bool(CombatSession&,ActorId,std::string&)> finished;
 dh2::character::skills::SkillAIServices16V3 ai;
 dh2::character::skills::SkillStateServices16V4 state;
};
// Live service composition over the SAME actor/property/skill/FSM/pose owners.
// Does not construct source VM/FSM or replace missing host gameplay providers.
class SessionSkillAnimation final {
 CombatSession& session_;ActorId id_;SkillActorBorrow owner_;SessionSkillServices services_;
 std::weak_ptr<const void> session_lease_;
 const SkillAnimationPrograms& programs_;
 std::unique_ptr<dh2::foundation::skills_animation::SourceSkillAnimation> source_;
 std::string failure_;
 static int state_service(void*,dh2::character::skills::SkillStateV4*,
  const dh2::character::skills::SkillStateRequest32V4*,dh2::character::skills::SkillStateResponse16V4*);
 bool validate(std::string&);
 bool select_animation(std::int32_t,std::string&);
public:
 SessionSkillAnimation(CombatSession&,ActorId,SkillActorBorrow,dh2::data::SkillTables::Borrow,
  const SkillAnimationPrograms&,SessionSkillServices);
 SessionSkillAnimation(const SessionSkillAnimation&)=delete;
 SessionSkillAnimation& operator=(const SessionSkillAnimation&)=delete;
 bool command(std::uint32_t operation,std::uint32_t native_index,std::uint32_t& answer,std::string&);
 bool hotbar_command(std::uint32_t operation,std::size_t hotbar_slot,std::uint32_t& answer,std::string&);
 bool class_position_command(std::uint32_t operation,int class_position,std::uint32_t& answer,std::string&);
 bool state_operation(std::uint32_t operation,std::uint32_t index,std::uint32_t moving,
  std::uintptr_t payload,std::uint32_t force,std::string&);
 bool authored_event(const RetainedAnimationEvent&,std::string&);
};
}
