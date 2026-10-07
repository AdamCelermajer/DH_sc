#pragma once
#include "world_item_object_owner_v1.hpp"
#include "character_loot_drop_award_v8.hpp"
namespace dh2::character {
struct LootInventorySourceV9;struct WorldItemDropInventoryServicesV9;
struct WorldLootFactoryServicesV1 {
 void* context{};
 // Must run actual ObjectManager.Spawn("Item",name,false,true), retaining
 // the canonical published receiver/lease. This is not an allocator shortcut.
 bool(*spawn)(void*,const char*,const char*,bool,bool,std::shared_ptr<RetainedWorldItemObjectV1>&,std::string&){};
 // Same ItemInstance's constructor-backed signed16 +58, absent in the old
 // ItemInstanceV1 projection. It must not be inferred from ItemTable pickup.
 bool(*pickup_override58)(void*,data::ItemInstanceV1&,const std::int16_t*&,std::string&){};
 LootDropAwardServicesV8 drop;
};
// Holds receiver leases returned by the canonical factory; lookup is a
// delivery index, not a second ObjectManager or independently published IDs.
class WorldLootItemRuntimeV1 {
 WorldLootFactoryServicesV1 services_;
 std::map<std::uintptr_t,std::shared_ptr<RetainedWorldItemObjectV1>> receivers_;
 CharacterLootItemManagerV8 manager_;
 CharacterLootDropAwardV8 award_;
 static bool spawn(void*,const char*,const char*,bool,bool,LootItemObjectBorrowV8&,std::string&);
 static bool invoke(void*,const LootItemRequestV8&,std::string&);
 static bool position(void*,std::uintptr_t,const float*&,std::string&);
 static bool random_position(void*,std::uintptr_t,std::uintptr_t,float[3],std::string&);
 static bool local_player(void*,std::int32_t,bool,std::uintptr_t&,std::string&);
 static bool item(void*,std::uintptr_t,LootItemPickupBorrowV8&,std::string&);
 static bool constant(void*,const char*,const char*,std::int32_t&,std::string&);
 static bool interact(void*,std::uintptr_t,std::uintptr_t,std::string&);
public:
 WorldLootItemRuntimeV1(data::LootAudioVisualV8::Borrow,WorldLootFactoryServicesV1);
 bool precache(std::string& e){return manager_.precache(e);}
 bool drop(data::LootTemporaryInventoryV8& inventory,std::uintptr_t source,std::uintptr_t killer,std::string& e){return award_.drop(inventory,source,killer,e);}
 // Original DropInventory3ec974 (different from DropAndAwardLoot): no
 // automatic pickup, source Character drop-lock/friendly-index tail retained.
 bool drop_inventory_source_v9(LootInventorySourceV9&,std::uintptr_t source,
  std::uintptr_t scatter_target,std::uintptr_t owner_character,
  const WorldItemDropInventoryServicesV9&,std::string&);
 CharacterLootItemManagerV8& manager()noexcept{return manager_;}
 RetainedWorldItemObjectV1* receiver(std::uintptr_t)const noexcept;
 // Call after actual canonical deleting/removal; a live pooled receiver must
 // not be erased while source manager still borrows its fields.
 bool erased(std::uintptr_t,std::string&);
};
}
