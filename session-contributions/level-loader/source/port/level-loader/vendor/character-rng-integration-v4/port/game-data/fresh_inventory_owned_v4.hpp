#pragma once
#include "loot_tables_v2.hpp"
#include "item_inventory_v1.hpp"
#include "properties.hpp"
namespace dh2::data {
struct OwnedItemSlotV4 {std::unique_ptr<ItemInstanceV1> item;std::array<std::int8_t,2> slots{{-1,-1}};};
enum class OwnedInventoryOperationV4:std::uint32_t {
 debug_load=0x337888,debug_query=0x337a88,current_player=0x31f594,player_count=0x4043a8,
 update_name=0x3fb754,update_stats=0x3fb290,update_requirements=0x3facdc,add_power=0x3fbc60,
 inventory_full=0x3fe330,full_notifications=0x3ff7a8,destroy_item=0x3ff70c,
 update_gear_properties=0x3e08a8,skin=0x3a999c,validate_hp_mp=0x3bd140,
 gold_notifications=0x3fdfd8
};
class FreshInventoryOwnedV4;
struct OwnedInventoryRequestV4 {OwnedInventoryOperationV4 operation;std::uint32_t source_caller;ItemInstanceV1* item;const char* name;std::int32_t argument;std::uint32_t index;};
struct OwnedInventoryResponseV4 {std::uintptr_t identity{};std::int32_t value{};};
// Effects are mandatory when reached. Native storage observations are optional
// audit notifications and cannot veto/fake native allocation/free/fullness.
struct OwnedInventoryServicesV4 {
 void* context{};bool (*invoke)(void*,FreshInventoryOwnedV4&,const OwnedInventoryRequestV4&,OwnedInventoryResponseV4&,std::string&){};
 void (*observe_storage)(void*,FreshInventoryOwnedV4&,const OwnedInventoryRequestV4&){};
};
// New authoritative owner. One actual vector/slot/item/equipment graph serves
// creation, equipment, split, merge and removal; no const_cast or V3 mirror.
// It retains immutable tables and the SAME shared live Character property state.
class FreshInventoryOwnedV4 {
 LootTablesV2::Borrow tables_;LootRandom8V2* random_;std::uintptr_t character_;
 std::shared_ptr<PropertyState> properties_;
 std::vector<std::unique_ptr<OwnedItemSlotV4>> items_;ItemInstanceV1* potion_{};
 std::array<std::array<OwnedItemSlotV4*,9>,2> equipment_{};std::uint8_t selected_{};
 std::int8_t potion_capacity_;std::int32_t gold_{},gold_limit_{INT32_MAX};bool unlimited_{},running_{};std::uint32_t callback_depth_{};
 bool mutation_allowed(std::string&)const;
 bool deliver(const OwnedInventoryServicesV4&,OwnedInventoryOperationV4,std::uint32_t,ItemInstanceV1*,const char*,std::int32_t,std::uint32_t,OwnedInventoryResponseV4&,std::string&);
 bool debug(const OwnedInventoryServicesV4&,std::uint32_t,const char*,std::int32_t&,std::string&);
 void observe(const OwnedInventoryServicesV4&,OwnedInventoryOperationV4,std::uint32_t,ItemInstanceV1*,std::int32_t=0,std::uint32_t=0);
 const Item* metadata(const ItemInstanceV1*,std::string&)const;
 std::int32_t set_for_slot(std::int32_t)const noexcept;
 std::int8_t& slot_for_set(OwnedItemSlotV4&,std::uint32_t)noexcept;
 bool destroy(std::unique_ptr<ItemInstanceV1>&,const OwnedInventoryServicesV4&,std::uint32_t,std::string&);
 bool is_full(bool&,const OwnedInventoryServicesV4&,std::string&);
 bool delete_instance(ItemInstanceV1*,const OwnedInventoryServicesV4&,std::string&);
 bool has_like(const ItemInstanceV1*,std::uint32_t&,bool&,std::string&)const;
 bool add_quantity(ItemInstanceV1&,std::int32_t,std::string&);
 FreshInventoryOwnedV4(LootTablesV2::Borrow,LootRandom8V2&,std::shared_ptr<PropertyState>);
public:
 FreshInventoryOwnedV4(std::uintptr_t,LootTablesV2::Borrow,LootRandom8V2&,std::int8_t,std::shared_ptr<PropertyState>);
 FreshInventoryOwnedV4(const FreshInventoryOwnedV4&)=delete;FreshInventoryOwnedV4& operator=(const FreshInventoryOwnedV4&)=delete;
 // Additive original NULL-character ItemInventory constructor for menu Drop.
 // Historical Character constructor continues rejecting identity0. This owns
 // fresh empty vectors/slots, cap−1 and gold0 over SAME immutable tables/RNG.
 static std::unique_ptr<FreshInventoryOwnedV4> create_drop_temporary_v4(
  LootTablesV2::Borrow,LootRandom8V2&,std::shared_ptr<PropertyState>);
 bool add_fixed_loot(std::int32_t,const OwnedInventoryServicesV4&,std::string&);
 // Source-invalid indices/negative assertions and destructive native reentry
 // reject explicitly; read-only queries and live cached-property/selection writes
 // remain available synchronously in effects. Partial source prefixes persist.
 bool create_item(std::int32_t,std::uint32_t,std::unique_ptr<ItemInstanceV1>&,const OwnedInventoryServicesV4&,std::string&);
 bool split_item(ItemInstanceV1&,std::int32_t,std::unique_ptr<ItemInstanceV1>&,const OwnedInventoryServicesV4&,std::string&);
 // Source AddItemInstance owns input only after delivered storage/merge/delete.
 // Prefix is retained on required effect failure; caller retains unconsumed input.
 bool add_item(std::unique_ptr<ItemInstanceV1>&,bool force,bool convert_gold,std::int32_t& index,const OwnedInventoryServicesV4&,std::string&);
 bool inventory_full(bool&,const OwnedInventoryServicesV4&,std::string&);
 bool auto_equip(std::uint32_t,std::int32_t& result,const OwnedInventoryServicesV4&,std::string&);
 bool character_auto_equip(std::uint32_t,std::int32_t& result,const OwnedInventoryServicesV4&,std::string&);
 bool equip_to_slot(std::uint32_t,std::uint32_t,bool,const OwnedInventoryServicesV4&,std::string&);
 bool unequip_from_slot(std::uint32_t,std::int32_t,const OwnedInventoryServicesV4&,std::string&);
 bool is_equipped(std::uint32_t,bool&,std::string&)const;
 bool has_two_hander(bool,bool&,std::string&)const;
 bool set_gold(std::int32_t,const OwnedInventoryServicesV4&,std::string&);
 bool add_gold(std::int32_t,const OwnedInventoryServicesV4&,std::string&);
 static bool equal(const ItemInstanceV1&,const ItemInstanceV1&)noexcept;
 const auto& items()const noexcept{return items_;}const auto& equipment()const noexcept{return equipment_;}
 const ItemInstanceV1* potion()const noexcept{return potion_;}std::int32_t num_potions()const noexcept;
 std::int8_t potion_capacity_v4()const noexcept{return potion_capacity_;}
 const ItemTable& table()const{return tables_.items();}
 const LootTablesV2::Borrow& tables_borrow()const noexcept{return tables_;}
 std::uintptr_t character()const noexcept{return character_;}
 std::int32_t current_equipment()const noexcept{return selected_;}void swap_equipment()noexcept{selected_=std::uint8_t(!selected_);}
 const std::shared_ptr<PropertyState>& properties()const noexcept{return properties_;}
 std::int32_t gold()const noexcept{return gold_;}
 // Caller must supply genuine source field writes; these are not producers.
 void project_potion_capacity(std::int8_t v)noexcept{potion_capacity_=v;}
 void project_gold_limit(std::int32_t v)noexcept{gold_limit_=v;}
 void project_unlimited(bool v)noexcept{unlimited_=v;}
};
}
