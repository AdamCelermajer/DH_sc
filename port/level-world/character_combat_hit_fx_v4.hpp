#pragma once
#include "character_combat_hit_fx_v2.hpp"
#include "character_mesh_fx_owner_v4.hpp"
namespace dh2::character::skills {
bool character_combat_hit_fx_v4(CharacterWorldRuntimeV1&,data::EffectsTables::Borrow,fx::CharacterMeshFxOwnerV4*,const data::CombatResult&,std::uintptr_t,CombatHitFxServicesV1,std::string&);
}
