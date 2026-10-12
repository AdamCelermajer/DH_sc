#pragma once
#include "loot_tables_v2.hpp"
#include "item_inventory_v1.hpp"
namespace dh2::data {
enum class FreshInventoryOperationV2:std::uint32_t {debug_load=0x337888,debug_query=0x337a88,
 current_player=0x31f594,player_count=0x4043a8,
 update_name=0x3fb754,update_stats=0x3fb290,update_requirements=0x3facdc,
 inventory_full=0x3fe330,full_notifications=0x3ff7a8,destroy_item=0x3ff70c};
class FreshInventoryV2;
struct FreshInventoryRequestV2 {FreshInventoryOperationV2 operation;std::uint32_t source_caller;ItemInstanceV1* item;const char* name;std::int32_t argument;std::uint32_t index;};
struct FreshInventoryResponseV2 {std::uintptr_t identity{};std::int32_t value{};};
struct FreshInventoryServicesV2 {void* context{};bool (*invoke)(void*,FreshInventoryV2&,const FreshInventoryRequestV2&,FreshInventoryResponseV2&,std::string&){};};
// New bounded source-owned AddLoot force-add path. Its item and slot addresses
// remain stable. Existing v1 save/load owner is deliberately unchanged.
// Native item effects, tracing, player manager and auto-equip are mandatory
// caller services, never successful defaults. No general randomized loot,
// gold loot, powered loot or difficulty-name variant producer is claimed.
class FreshInventoryV2 {
 LootTablesV2::Borrow tables_;LootRandom8V2* random_;std::uintptr_t character_;
 std::vector<std::unique_ptr<ItemSlotV1>> items_;ItemInstanceV1* potion_{};
 std::array<std::array<ItemSlotV1*,9>,2> equipment_{};std::uint8_t selected_{};
 std::int8_t potion_capacity_;bool running_{};
 bool deliver(const FreshInventoryServicesV2&,FreshInventoryOperationV2,std::uint32_t,ItemInstanceV1*,const char*,std::int32_t,std::uint32_t,FreshInventoryResponseV2&,std::string&);
 bool debug(const FreshInventoryServicesV2&,std::uint32_t,const char*,std::int32_t&,std::string&);
public:
 FreshInventoryV2(std::uintptr_t,LootTablesV2::Borrow,LootRandom8V2&,std::int8_t actual_potion_capacity);
 FreshInventoryV2(const FreshInventoryV2&)=delete;FreshInventoryV2& operator=(const FreshInventoryV2&)=delete;
 // Only fully fixed/no-subloot/no-power source tables with valid ItemLists.
 // Unsupported continuations fail explicitly; reached prefix is retained.
 bool add_fixed_loot(std::int32_t id,const FreshInventoryServicesV2&,std::string&);
 bool record_delivered_equipment(std::uint32_t set,std::uint32_t slot,std::uint32_t index,std::string&);
 const auto& items()const noexcept{return items_;}const auto& equipment()const noexcept{return equipment_;}
 const ItemInstanceV1* potion()const noexcept{return potion_;}std::int32_t num_potions()const noexcept;
 const ItemTable& table()const{return tables_.items();}
 std::uintptr_t character()const noexcept{return character_;}
 std::int32_t current_equipment()const noexcept{return selected_;}
 void swap_equipment()noexcept{selected_=std::uint8_t(!selected_);}
};
}
