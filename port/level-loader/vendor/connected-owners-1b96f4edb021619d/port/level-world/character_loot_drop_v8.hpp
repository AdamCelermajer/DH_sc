#pragma once
#include "character_world_runtime_v1.hpp"
#include "../game-data/loot_temporary_inventory_v8.hpp"
namespace dh2::character::skills {
struct WorldLootActorBorrowV8 {
 std::uintptr_t identity{};
 std::int32_t type_index{}; // actual GameObject+f4; Character is source index0
 const data::AiProps* ai{};
 data::PropertyView* properties{};
};
struct WorldLootDropServicesV8 {
 void* context{};
 bool(*actor)(void*,std::uintptr_t,WorldLootActorBorrowV8&,std::string&){};
 bool(*is_player)(void*,std::uintptr_t,bool&,std::string&){};
 // Real temporary inventory AddLoot and ItemManager/DropAndAwardLoot bodies.
 // Drop must consume actual temporary items into retained world ItemObjects;
 // automatic pickup is solely the authored PickUpType::Automatic branch.
 bool(*create)(void*,data::LootTemporaryInventoryV8&,std::int32_t table,
               std::int32_t value_bonus,std::int32_t power_bonus,
               std::int32_t fixed_powers,std::string&){};
 bool(*drop)(void*,data::LootTemporaryInventoryV8&,std::uintptr_t source,
             std::uintptr_t killer,std::int32_t owner_index,std::string&){};
};
// Source DropLootTable3ecba0 and GetInventoryToDrop3ecae4, over the SAME
// canonical World handles. Neither actor/properties nor Player inventory are
// copied. Call only from the actual CharacterKill drop service at its ordered
// source point (before contributor XP); this class does not hook death clips.
class CharacterLootDropV8 {
 CharacterWorldRuntimeV1& world_;data::LootTablesV2::Borrow tables_;
 WorldLootDropServicesV8 services_;bool running_{};
 std::unique_ptr<data::LootTemporaryInventoryV8> pending_;
 bool character(std::uintptr_t,WorldLootActorBorrowV8&,std::string&);
public:
 CharacterLootDropV8(CharacterWorldRuntimeV1& w,data::LootTablesV2::Borrow t,WorldLootDropServicesV8 s):world_(w),tables_(std::move(t)),services_(s){}
 bool drop_table(std::int32_t,std::uintptr_t source,std::uintptr_t killer,
                 std::int32_t owner_index,std::string&);
 const data::LootTemporaryInventoryV8* pending_inventory()const noexcept{return pending_.get();}
 std::unique_ptr<data::LootTemporaryInventoryV8> release_pending_inventory()noexcept{return std::move(pending_);}
};
}
