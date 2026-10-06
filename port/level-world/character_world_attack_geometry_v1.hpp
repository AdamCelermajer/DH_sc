#pragma once
#include "character_world_runtime_v1.hpp"
#include "character_world_ai_can_attack_v1.hpp"
#include "character_attack_geometry.hpp"
#include "character_design_services.hpp"
#include "../game-data/fresh_inventory_owned_v4.hpp"
namespace dh2::character::skills {
// Borrow the source-selected inventory, never an inferred weapon capability.
// A constructor-owned NPC inventory may use its genuine empty equipment sets
// via the projection callback; absence of an inventory is a required failure.
struct WorldAttackInventoryBorrowV1 {
 const data::FreshInventoryOwnedV4* owned{};
 const CombatInventory16* projection{};
 const CombatItemRecord164* rows{};
 std::uint32_t row_count{};
 const std::int32_t* resolved{};
};
// Sole query backing for the actual constructor-only NPC inventory period.
// ItemInventory C1 3ff268..270 initializes selected byte+2e=0;
// 3ff288..304 creates two sets of nine null references. Do not use after a
// source equipment mutation: borrow the resulting actual inventory instead.
class CharacterAttackEmptyInventoryV1 {
 const std::int32_t* resolved_{};
 CombatEquipSet8 sets_[2]{};
 CombatInventory16 inventory_{sets_,2,0};
public:
 explicit CharacterAttackEmptyInventoryV1(const std::int32_t* resolved):resolved_(resolved){}
 CharacterAttackEmptyInventoryV1(const CharacterAttackEmptyInventoryV1&)=delete;
 WorldAttackInventoryBorrowV1 borrow()const noexcept{return {nullptr,&inventory_,nullptr,0,resolved_};}
};
struct WorldAttackGeometryServicesV1 {
 void* context{};
 int(*inventory)(void*,std::uintptr_t,WorldAttackInventoryBorrowV1*){};
 int(*object_kind)(void*,std::uintptr_t,std::int32_t*){}; // source ObjectBase+f4
 // Non-Character/type0/interaction8 path: exact AI_IsInInteractionRange,
 // including GetInteractionPosition and target+2e8. Required when reached.
 int(*interaction_range)(void*,std::uintptr_t,std::uintptr_t,std::int32_t*){};
};
class CharacterWorldAttackGeometryV1 {
 CharacterWorldRuntimeV1& world_;const data::AiTables& ai_;
 DebugSwitches& debug_;const DebugFileServices24& files_;
 WorldAttackGeometryServicesV1 services_;
 struct Projection;
 bool inventory(std::uintptr_t,Projection&,std::string&);
 bool radius(std::uintptr_t,float&,std::string&);
 bool position(std::uintptr_t,const float*&,std::string&);
 bool debug(std::string&);
 static bool query(void*,std::uintptr_t,std::uintptr_t,WorldAIAttackQueryV1,std::int32_t&,std::string&);
public:
 CharacterWorldAttackGeometryV1(CharacterWorldRuntimeV1&,const data::AiTables&,
  DebugSwitches&,const DebugFileServices24&,WorldAttackGeometryServicesV1);
 WorldAIAttackServicesV1 queries()noexcept{return {this,query};}
 bool melee_radius(std::uintptr_t id,float& value,std::string& error){return radius(id,value,error);}
 bool read(std::uintptr_t,std::uintptr_t,WorldAIAttackQueryV1,std::int32_t&,std::string&);
 // Source3d63d8 over SAME registered owners/inventory; no chosen radius.
 bool close_range_v38(std::uintptr_t owner,std::uintptr_t explicit_target,std::uintptr_t current_target,std::int32_t&,std::string&);
};
}
