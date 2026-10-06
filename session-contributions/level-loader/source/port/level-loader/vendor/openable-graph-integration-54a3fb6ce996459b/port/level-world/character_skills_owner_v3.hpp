#pragma once
#include "character_skills.hpp"
namespace dh2::character::skills {
class CharacterSkillOwnerV3 {
 struct Impl;std::unique_ptr<Impl> impl_;
public:
 CharacterSkillOwnerV3(std::uintptr_t,data::PropertyView*,data::SkillTables::Borrow,
  data::FaeryTables::Borrow,const Services16& script_services);
 ~CharacterSkillOwnerV3();
 CharacterSkillOwnerV3(const CharacterSkillOwnerV3&)=delete;
 CharacterSkillOwnerV3& operator=(const CharacterSkillOwnerV3&)=delete;
 CharacterSkillOwnerV3(CharacterSkillOwnerV3&&)=delete;
 CharacterSkillOwnerV3& operator=(CharacterSkillOwnerV3&&)=delete;
 int configure();int update();int cleanup_skills();int cleanup_spells();
 // Original cooldown field writer, constrained to retained owned instances.
 // kind0=skill, kind1=spell; absent slots are source no-op. Out-of-range
 // assertion-unsafe source accesses are rejected before touching storage.
 int set_cooldown(std::uint32_t kind,std::uint32_t index,std::int32_t timer);
 const data::SkillRecord* skill(std::uint32_t index)const noexcept;
 int list_count(std::uint32_t kind,std::uint32_t* count)const noexcept;
 const State40& state()const noexcept;const std::string& error()const noexcept;
};

}
