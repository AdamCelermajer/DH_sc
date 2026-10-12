#pragma once
#include "character_cancel_sneaking.hpp"
#include "../game-data/skill_tables.hpp"

namespace dh2::character::sneaking {
// Owns the complete CancelSneaking scalar/list projection and pins every source
// payload. Keep this adapter alive through all synchronous provider callbacks.
// It is immovable: addresses installed in Character48::tables remain stable.
class SneakingTables final {
 dh2::data::SkillTables::Borrow source_;
 std::vector<List16> lists_;
 std::vector<Skill76> skills_;
 Tables32 view_{};
public:
 explicit SneakingTables(dh2::data::SkillTables::Borrow);
 SneakingTables(const SneakingTables&)=delete;
 SneakingTables& operator=(const SneakingTables&)=delete;
 SneakingTables(SneakingTables&&)=delete;
 SneakingTables& operator=(SneakingTables&&)=delete;
 const Tables32& view()const noexcept{return view_;}
 const dh2::data::SkillTables::Borrow& source()const noexcept{return source_;}
};
}
