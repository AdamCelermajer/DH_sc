#pragma once
#include "character_buffs.hpp"
#include "character_skill_buff_bindings_v3.hpp"
#include "../game-data/ai.hpp"
#include "../game-data/effects_tables.hpp"
namespace dh2::character {
struct SlowReactionBorrowV1 {
 data::PropertyView* properties{};BuffOwner* buffs{};
 const data::AiTables* ai{};const data::ClassTables* classes{};
 data::EffectsTables::Borrow effects;
};
// Whole original PROPS_DebuffSlow3e2a5c, same BuffOwner/PropertyView.
//1 delivered source (including actual boss/zero/missing-class gates), negative
//required failure preserving real AddBuff/class/property prefixes.
int character_slow_reaction_v1(const SlowReactionBorrowV1&,std::uint32_t duration,
 BuffResult24&,std::string& error);
}
