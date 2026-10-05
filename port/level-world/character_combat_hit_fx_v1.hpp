#pragma once
#include "character_mesh_fx_owner_v1.hpp"
#include "character_world_runtime_v1.hpp"
#include "../game-data/combat_result.hpp"
namespace dh2::character::skills {
struct CombatHitFxServicesV1 {
 void* context{};
 // Source Character rotation+16c. The position uses the same target owner
 // GetTargetPosition; rotation must be supplied by its actual live transform.
 int(*rotation)(void*,std::uintptr_t,float[3]){};
};
// F_ApplyResult 3b1648/3b1cd0: category+124 else death/blood getter using
// cached Effects7/Character1014 with original invalid-row fallback row0.
bool character_combat_hit_fx_v1(CharacterWorldRuntimeV1&,data::EffectsTables::Borrow,
 fx::CharacterMeshFxOwnerV1*,const data::CombatResult&,std::uintptr_t target,
 CombatHitFxServicesV1,std::string&);
}
