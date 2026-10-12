#pragma once
#include "items.hpp"
#include <memory>
namespace dh2::data {
struct ItemInstanceV1 {
 std::int32_t id{-1};std::uint16_t quantity{};std::int32_t value{};std::int16_t requirement{-1};
 std::uint8_t identified{1},buyback{};std::string name,description,requirements;std::vector<std::int32_t> powers;
 // Quantity is source signed16 even though its constructor stores low16.
 std::int32_t signed_quantity()const noexcept;
};
struct ItemSlotV1 {std::unique_ptr<ItemInstanceV1> item;std::int8_t equipment_set{-1},equipment_slot{-1};};
class ItemInventoryV1;
enum class InventoryOperationV1:std::uint32_t {
 update_name=0x3fb754,update_stats=0x3fb290,update_requirements=0x3facdc,
 add_power=0x3fbc60,gold_notifications=0x3fdfd8,inventory_full=0x3fe330,
 full_notifications=0x3ff7a8,equip_item=0x400634,destroy_item=0x3ff70c
};
struct InventoryRequestV1 {InventoryOperationV1 operation;ItemInstanceV1* item;std::int32_t argument;std::uint32_t index,set;};
struct InventoryResponseV1 {bool flag{};};
struct InventoryServicesV1 {void* context{};bool (*invoke)(void*,ItemInventoryV1&,const InventoryRequestV1&,InventoryResponseV1&,std::string&){};};
struct InventoryLoadReceiptV1 {std::size_t consumed{},items_read{},items_retained{},powers_read{},equipment_deliveries{};bool completed{};};
// Stable heap-owned source item/slot identities. The table is copied into an
// immutable owned snapshot so caller vectors may die. A blank instance has
// two nine-slot equipment sets and no potion; this is not a ready Player.
class ItemInventoryV1 {
 std::shared_ptr<const ItemTable> table_;std::uintptr_t character_{};
 std::vector<std::unique_ptr<ItemSlotV1>> items_;
 std::array<std::array<ItemSlotV1*,9>,2> equipment_{};ItemInstanceV1* potion_{};
 std::int32_t gold_{},gold_limit_{INT32_MAX};std::int8_t potion_capacity_{-1},selected_equipment_{};
 bool load_active_{};
 bool deliver(const InventoryServicesV1&,InventoryOperationV1,ItemInstanceV1*,std::int32_t,std::uint32_t,std::uint32_t,InventoryResponseV1&,std::string&);
public:
 explicit ItemInventoryV1(const ItemTable& actual_table);
 ItemInventoryV1(const ItemInventoryV1&)=delete;ItemInventoryV1& operator=(const ItemInventoryV1&)=delete;
 void set_character(std::uintptr_t identity)noexcept{character_=identity;}
 std::uintptr_t character()const noexcept{return character_;}
 // Source __LoadInventory section only. Field/parser errors retain the reached
 // prefix. Missing required services return false; no incomplete load success.
 // Source reentrant loading is not supported by this owned adapter.
 bool load_section(Bytes,const std::vector<std::string>& genuine_power_names,const InventoryServicesV1&,InventoryLoadReceiptV1&,std::string&);
 // Actual Potion0 producer; existing quantity0 deletes, missing quantity0
 // still constructs a real item. Negative setter requires unrecovered Debug
 // continuation policy and is rejected rather than claimed successful.
 bool set_potion_quantity(std::int32_t,const InventoryServicesV1&,std::string&);
 bool remove_one_potion(const InventoryServicesV1&,std::string&);
 // Called ONLY by genuine equip provider after it delivers source checks,
 // un/equip, recalc, localization and visual work. No guessed auto-equip.
 bool record_delivered_equipment(std::uint32_t set,std::uint32_t slot,std::uint32_t index,std::string&);
 const ItemTable& table()const noexcept{return *table_;}
 const std::vector<std::unique_ptr<ItemSlotV1>>& items()const noexcept{return items_;}
 const std::array<std::array<ItemSlotV1*,9>,2>& equipment()const noexcept{return equipment_;}
 const ItemInstanceV1* potion()const noexcept{return potion_;}
 std::int32_t num_potions()const noexcept;
 std::int32_t gold()const noexcept{return gold_;}std::int32_t gold_limit()const noexcept{return gold_limit_;}
 std::int32_t current_equipment(std::int32_t requested=-1)const noexcept;
 static constexpr std::int32_t current_skill_set()noexcept{return 0;}
 void swap_equipment()noexcept;
 // These fields require actual upstream owner/property producers; explicit
 // setters project their source stores without claiming those producers.
 void project_gold_limit(std::int32_t x)noexcept{gold_limit_=x;}
 void project_potion_capacity(std::int8_t x)noexcept{potion_capacity_=x;}
 std::int8_t potion_capacity()const noexcept{return potion_capacity_;}
};
}
