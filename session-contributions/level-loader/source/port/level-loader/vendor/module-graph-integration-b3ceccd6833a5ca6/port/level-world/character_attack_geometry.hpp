#pragma once
#include "character_combat_queries.hpp"
// Read-only source parameter and geometry kernels. Rows/radii are projections
// of genuine decoded ItemTable/AIProps, and properties are resolved cache words.
// Borrowed storage must remain live. Return -1 for malformed native boundaries.
extern "C" {
// Source ItemInventory::CanMeleeAttack(int&): 0 ranged (retain output), 1 melee.
int dh2_attack_equipment_melee_radius(std::int32_t* output,
 const dh2::character::CombatInventory16*,const dh2::character::CombatItemRecord164*,std::uint32_t);
// Source Character::CanRangeAttack(int&,int&,int&): 0 retains all outputs, 1 writes.
int dh2_attack_range_parameters(std::int32_t output[3],
 const dh2::character::CombatProperties896*,const dh2::character::CombatInventory16*,
 const dh2::character::CombatItemRecord164*,std::uint32_t);
// AI_GetMeleeRadius: 0 successful output; fallback AI ID8, cached property index1.
int dh2_attack_melee_radius(float* output,const dh2::character::CombatProperties896*,
 const dh2::character::CombatInventory16*,const dh2::character::CombatItemRecord164*,std::uint32_t,
 const float* decoded_ai_radii,std::uint32_t ai_count);
// Boolean results over already-selected original GetTargetPosition inputs.
// Melee domain is resolved Character/type0/interaction8. Other interaction
// branches require their actual services before calling this pure kernel.
int dh2_attack_melee_distance(const float owner[3],const float target[3],const float radii[2]);
int dh2_attack_ranged_distance(const float owner[3],const float target[3],const std::int32_t limits[2]);
}
