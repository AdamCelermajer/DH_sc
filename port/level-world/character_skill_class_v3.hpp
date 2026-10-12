#pragma once
#include "../game-data/properties.hpp"
// Original uncached ApplyClass target=owner.resolved, preserving genuine base,
// saved, gear, and buff sources. Linear operands resolve the live owner. Other
// formulas use the current target sheet. Invalid recursive data preserves any
// completed source prefix; this is not an atomic class-table transaction.
extern "C" unsigned dh2_character_skill_class_v3(const dh2::data::ClassRow*,
 std::uint32_t count,std::int32_t id,dh2::data::PropertyView*);
