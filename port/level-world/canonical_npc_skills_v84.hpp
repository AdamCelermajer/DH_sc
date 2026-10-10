#pragma once
#include "character_skills_session.hpp"
#include "character_skill_ai_v3.hpp"
namespace dh2::character {
class RetainedCharacterActorV1;
class CanonicalNpcSkillsV84 final {
 std::unique_ptr<skills::CharacterSkillSessionServices> services_;
 std::unique_ptr<skills::CharacterSkillOwner> owner_;
 mutable std::string error_;
 skills::SkillAIStateV3 ai_fields_v115_{};bool ai_fields_produced_v115_{};
public:
 CanonicalNpcSkillsV84(RetainedCharacterActorV1&,data::SkillTables::Borrow,
  data::FaeryTables::Borrow,const fx::PreloadServices16&);
 CanonicalNpcSkillsV84(const CanonicalNpcSkillsV84&)=delete;
 skills::CharacterSkillOwner& owner()noexcept{return *owner_;}
 skills::SkillAIStateV3* source_ai_fields_v115()noexcept{return ai_fields_produced_v115_?&ai_fields_v115_:nullptr;}
 int configure(){return owner_->configure();}
 int update(){return owner_->update();}
 int cleanup_skills(){return owner_->cleanup_skills();}
 int cleanup_spells(){return owner_->cleanup_spells();}
 int unload_script_v105(RetainedCharacterActorV1&,bool final);
 const std::string& error()const noexcept;
};
}
