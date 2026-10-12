#pragma once
#include "properties.hpp"
namespace dh2::data {
struct VitalsChange {std::int32_t raw_add=0,before=0,after=0;};
struct SpawnVitals {VitalsChange first_hp,first_mp,second_hp,second_mp;};
// HP/MP portion of a fresh Character::InitPost: Revive invokes _InitHpMp,
// then InitPost explicitly invokes it again. Other revive/spawn effects are
// outside this helper. Apply to sheets after normal class recalculation.
bool initialize_spawn_vitals(const PropertyRules&,PropertyState&,SpawnVitals&,std::string&);
}
// Exact raw fixed-point regen arithmetic, including ARM32 wrapping before the
// signed cap comparison. Negative requests use the maximum, nonpositive
// resulting requests do nothing. The result records the requested positive
// AddProperty amount and actual resolved before/after values.
extern "C" unsigned dh2_vitals_regen(dh2::data::PropertyView*,unsigned mana,std::int32_t raw_amount,dh2::data::VitalsChange*);
// One _InitHpMp call, in original HP-then-MP order.
extern "C" unsigned dh2_vitals_initialize(dh2::data::PropertyView*,dh2::data::VitalsChange* hp,dh2::data::VitalsChange* mp);
extern "C" unsigned dh2_vitals_spawn_init(dh2::data::PropertyView*,dh2::data::SpawnVitals*);
