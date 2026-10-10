#pragma once
#include "../../../level-world/character_skill_ai_v3.hpp"
#include "../../../level-world/character_skill_state_v4.hpp"
#include "../../../game-data/animation_tables.hpp"
#include <functional>

namespace dh2::foundation::skills_animation {
struct Declaration {
 std::int32_t row; std::string name,script,icon;
 std::uint32_t animation,flags,type,level;
 std::uint8_t moving,assignable;
};
// Names and class membership come directly from SkillList/Skill tables.
bool declarations(const data::SkillTables::Borrow&,const std::string& list,
 std::vector<Declaration>&,std::string&);

// Every runtime object is borrowed. The resolver maps an actor's actual skill
// slot to its global table row; SkillList ordinals are not actor slot IDs.
class SourceSkillAnimation final {
 data::SkillTables::Borrow tables_;
 character::skills::SkillAIContextV3& ai_;
 character::skills::SkillStateV4& state_;
 std::function<bool(std::uint32_t,std::int32_t&)> resolve_;
 character::skills::SkillAIServices16V3 ai_services_;
 character::skills::SkillStateServices16V4 state_services_;
 static int ai_service(void*,character::skills::SkillAIContextV3*,const character::skills::SkillAIRequest32V3*,character::skills::SkillAIResponse32V3*);
 static int state_service(void*,character::skills::SkillStateV4*,const character::skills::SkillStateRequest32V4*,character::skills::SkillStateResponse16V4*);
 const data::SkillProjection76* row(std::uint32_t);
 bool coherent()const;
public:
 SourceSkillAnimation(data::SkillTables::Borrow,character::skills::SkillAIContextV3&,
  character::skills::SkillStateV4&,std::function<bool(std::uint32_t,std::int32_t&)>,
  character::skills::SkillAIServices16V3,character::skills::SkillStateServices16V4);
 SourceSkillAnimation(const SourceSkillAnimation&)=delete;
 SourceSkillAnimation& operator=(const SourceSkillAnimation&)=delete;
 // Native status: 0 complete, -1 malformed, -2 missing service. Output is the
 // original answer, including successful denial; it is not a host FSM state.
 int command(std::uint32_t operation,std::uint32_t slot,std::uint32_t& answer);
 int state_operation(std::uint32_t operation,std::uint32_t index=0,
  std::uint32_t moving=0,std::uintptr_t payload=0,std::uint32_t force=0);
 // Dispatch synchronously at original marker phase. No time/marker invented.
 int authored_event(const char* name);
 // Resolves the override written by genuine SetSkillState through the original
 // three-layer selector. RNG, tables, timing and animation owner stay borrowed.
 bool animation_start(const data::AnimationTables&,data::AnimationRandom&,
  data::AnimationStart&,std::string&,bool random_enabled=true)const;
};
}
