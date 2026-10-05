#pragma once
#include "character_skills_owner_v3.hpp"
namespace dh2::character::skills {
class CharacterSkillOwnerV6 {
 struct Impl;std::unique_ptr<Impl> impl_;
public:
 CharacterSkillOwnerV6(std::uintptr_t,data::PropertyView*,data::SkillTables::Borrow,
  data::FaeryTables::Borrow,const Services16& script_services);
 ~CharacterSkillOwnerV6();
 CharacterSkillOwnerV6(const CharacterSkillOwnerV6&)=delete;
 CharacterSkillOwnerV6& operator=(const CharacterSkillOwnerV6&)=delete;
 CharacterSkillOwnerV6(CharacterSkillOwnerV6&&)=delete;
 CharacterSkillOwnerV6& operator=(CharacterSkillOwnerV6&&)=delete;
 int native_destroy_skills(std::uint32_t* destroyed);
 int native_delete_skill(std::uint32_t index,std::uintptr_t instance);
 int native_reset_skill_end();
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
