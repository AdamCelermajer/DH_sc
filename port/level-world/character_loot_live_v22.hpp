#pragma once
#include "character_kill_loot_connection_v10.hpp"
#include "world_item_loot_creation_v10.hpp"
#include "world_loot_item_runtime_v1.hpp"
#include "player_manager_owner_v1.hpp"
#include "character_loot_scatter_connection_v9.hpp"
#include "character_world_runtime_v1.hpp"
#include <map>
namespace dh2::character {
struct CharacterLootLiveBorrowV22 {
 skills::WorldLootActorBorrowV8 actor;
 const std::int32_t* table101c{};
 const float* position160{};
 const std::uint32_t* source_type_f4{};
 std::shared_ptr<void> receiver_lease;
};
struct CharacterLootLiveServicesV22 {
 std::shared_ptr<void> provider_lease;
 void* context{};
 bool(*actor)(void*,std::uintptr_t,CharacterLootLiveBorrowV22&,std::string&){};
 bool(*is_player)(void*,std::uintptr_t,bool&,std::string&){};
 // Required actual script constants; position/scatter/local are composed here.
 LootDropAwardServicesV8 drop;
 WorldItemLootCreationServicesV10 creation;
};
// Composition only: authoritative actor fields, tables, RNG, manager and Item
// receivers remain their existing owners. A destructive failure is latched per
// source actor; reissuing Kill cannot retry a partly transferred drop.
class CharacterLootLiveV22 {
 skills::CharacterWorldRuntimeV1& world_;
 player::PlayerManagerOwnerV1& players_;
 data::LootRandom8V2& random_;
 CharacterLootLiveServicesV22 services_;
 WorldItemLootCreationV10 creation_;
 WorldLootItemRuntimeV1* items_{}; // SAME WorldItemLiveOwnerV5 pool, never another145.
 skills::CharacterLootDropV8 drop_;
 CharacterKillLootConnectionV10 kill_;
 std::map<std::uintptr_t,std::string> failures_;
 bool running_{};
 bool borrow(std::uintptr_t,CharacterLootLiveBorrowV22&,std::string&);
 static bool actor(void*,std::uintptr_t,skills::WorldLootActorBorrowV8&,std::string&);
 static bool player(void*,std::uintptr_t,bool&,std::string&);
 static bool create(void*,data::LootTemporaryInventoryV8&,std::int32_t,std::int32_t,std::int32_t,std::int32_t,std::string&);
 static bool drop(void*,data::LootTemporaryInventoryV8&,std::uintptr_t,std::uintptr_t,std::int32_t,std::string&);
 static bool table(void*,std::uintptr_t,const std::int32_t*&,std::string&);
 static bool position(void*,std::uintptr_t,const float*&,std::string&);
 static bool scatter(void*,std::uintptr_t,std::uintptr_t,float[3],std::string&);
 static bool local(void*,std::int32_t,bool,std::uintptr_t&,std::string&);
 static bool constant(void*,const char*,const char*,std::int32_t&,std::string&);
public:
 CharacterLootLiveV22(skills::CharacterWorldRuntimeV1&,player::PlayerManagerOwnerV1&,
  data::LootTablesV2::Borrow,data::LootPowerResourcesV7::Borrow,data::ItemPowerTablesV5::Borrow,
  data::LootRandom8V2&,CharacterLootLiveServicesV22);
 CharacterLootLiveV22(const CharacterLootLiveV22&)=delete;
 // Original ItemManager PreCache boundary, never implicit in a kill request.
 bool bind_pool(WorldLootItemRuntimeV1&,std::string&);
 LootDropAwardServicesV8 drop_services()noexcept{return {this,position,scatter,local,nullptr,constant,nullptr};}
 bool precache(std::string& error);
 bool route(KillActor56&,const KillRequest56&,KillResponse16&,bool& handled,std::string&);
 //Same original ItemObject.DropLootTable used by Openable/Destructible.
 //Neither a synthetic KillActor nor a second temporary inventory producer.
 bool drop_table_v104(std::int32_t table,std::uintptr_t source,std::uintptr_t opener,std::int32_t fixed_powers,bool source_flag,std::string&);
 bool drop_explicit_v108(std::int32_t table,std::uintptr_t character,std::string&);
 bool drop_character_v117(std::uintptr_t character,std::uintptr_t nullable_target,std::string&);
 WorldLootItemRuntimeV1* items()noexcept{return items_;}
 const data::LootTemporaryInventoryV8* pending_inventory()const noexcept{return drop_.pending_inventory();}
};
}
