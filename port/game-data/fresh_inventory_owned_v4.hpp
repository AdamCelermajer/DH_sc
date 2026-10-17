#pragma once
#include "loot_tables_v2.hpp"
#include "loot_entry_selection_v1.hpp"
#include "item_instance.hpp"
#include "properties.hpp"
#include "loot_power_creation_v7.hpp"
#include <cstddef>
#include <cstdint>
namespace dh2::data {
struct OwnedItemSlotV4 {std::unique_ptr<ItemInstanceV1> item;std::array<std::int8_t,2> slots{{-1,-1}};};
struct QuestGatheringItemIdV4 {std::int32_t item_id{};std::uint8_t registrations{};};
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
// Borrow only: the existing Item lifetime owner keeps this slot alive across
// constructor/split/equip failure. No Item registry or additional owner.
struct RetainedItemSlotV4 {std::unique_ptr<ItemInstanceV1>* value{};};
// Effects are mandatory when reached. Storage observers run synchronously with
// the actual Item alive. Retirement may reject by throwing before reset/erase;
// stateful services require this provider. Other observations report storage.
struct OwnedInventoryServicesV4 {
 void* context{};bool (*invoke)(void*,FreshInventoryOwnedV4&,const OwnedInventoryRequestV4&,OwnedInventoryResponseV4&,std::string&){};
 void (*observe_storage)(void*,FreshInventoryOwnedV4&,const OwnedInventoryRequestV4&){};
 // Explicit legacy temporary contract: callbacks retain no Item pointers and
 // create no external per-Item state. Absence of retirement is not proof.
 bool stateless_temporaries=false;
 // ItemObject::Interact's post-transfer quest tail. Runs only after the world
 // Item has left this owner and AddItemInstance accepted the transfer. A
 // failure preserves that source prefix; callers must not retry the pickup.
 bool (*after_world_pickup)(void*,std::uintptr_t character,std::int32_t item_id,std::string& error){};
};
enum class WorldItemTransferResultV4 : std::uint8_t {not_applied,committed,indeterminate};
// Character::ItemInventory::TransferItemTo uses the existing two V4 owners.
// `indeterminate` means the source prefix changed but a required destination
// callback did not complete; callers must not retry the action blindly.
enum class OwnedItemTransferResultV4 : std::uint8_t {not_applied,committed,indeterminate};
// NativeInvDropItem needs a real online-state query and the complete original
// offline ItemObject::DropInventory continuation. The latter must calculate
// the source scatter point, spawn/InitAgain through the active ItemManager and
// apply the 5000 ms/player-id lock to the resulting world object. This owner
// deliberately has no default/fake provider for those world-runtime effects.
struct OfflineWorldItemDropServicesV4 {
 void* context{};
 bool (*is_online)(void*,bool&,std::string&){};
 bool (*spawn_and_lock)(void*,FreshInventoryOwnedV4&,std::size_t,ItemInstanceV1*,std::string&){};
};
// Optional AddLoot continuation. Values are explicit source caller inputs:
// CalcLootItemValue bonus, AddLootItemPowers bonus/request count, and the live
// character difficulty. Creation must borrow this inventory's same RNG and
// power definitions must be the exact V5 snapshot used by that creation owner.
// AddPower/debug transport reuses OwnedInventoryServicesV4; text transport is
// supplied by the same caller-owned presentation/text service.
struct OwnedLootEffectsV7 {
 LootPowerCreationV7* creation{};ItemPowerTablesV5::Borrow powers;
 ItemTextServicesV5 text;
 std::int32_t value_bonus256{},power_bonus256{},requested_power_count{-1},difficulty{};
};
// New authoritative owner. One actual vector/slot/item/equipment graph serves
// creation, equipment, split, merge and removal; no const_cast or V3 mirror.
// It retains immutable tables and borrows the Character's live PropertyState.
class FreshInventoryOwnedV4 {
 LootTablesV2::Borrow tables_;InventoryRandomServiceV4 random_;std::uintptr_t character_;
 PropertyState* properties_{};std::shared_ptr<PropertyState> fixture_properties_;
 std::vector<std::unique_ptr<OwnedItemSlotV4>> items_,world_items_;
 // Source ItemInventory+0x30 / Character+940 unique IDs and byte refcounts.
 std::vector<QuestGatheringItemIdV4> quest_gathering_item_ids_;
 ItemInstanceV1* potion_{};
 std::array<std::array<OwnedItemSlotV4*,9>,2> equipment_{};std::uint8_t selected_{};
 std::int8_t potion_capacity_;std::int32_t gold_{},gold_limit_{INT32_MAX};bool unlimited_{},running_{};std::uint32_t callback_depth_{};
 bool mutation_allowed(std::string&)const;
 struct LootPowerBridgeV4 {FreshInventoryOwnedV4* owner;const OwnedInventoryServicesV4* services;};
 static bool invoke_loot_power_bridge(void*,const LootPowerRequestV7&,std::int32_t&,std::string&);
 bool deliver(const OwnedInventoryServicesV4&,OwnedInventoryOperationV4,std::uint32_t,ItemInstanceV1*,const char*,std::int32_t,std::uint32_t,OwnedInventoryResponseV4&,std::string&);
 bool debug(const OwnedInventoryServicesV4&,std::uint32_t,const char*,std::int32_t&,std::string&);
 void observe(const OwnedInventoryServicesV4&,OwnedInventoryOperationV4,std::uint32_t,ItemInstanceV1*,std::int32_t=0,std::uint32_t=0);
 const Item* metadata(const ItemInstanceV1*,std::string&)const;
 std::int32_t set_for_slot(std::int32_t)const noexcept;
 std::int8_t& slot_for_set(OwnedItemSlotV4&,std::uint32_t)noexcept;
 bool destroy(std::unique_ptr<ItemInstanceV1>&,const OwnedInventoryServicesV4&,std::uint32_t,std::string&);
 bool is_full(bool&,const OwnedInventoryServicesV4&,std::string&);
 bool delete_instance(ItemInstanceV1*,const OwnedInventoryServicesV4&,std::string&,std::uint32_t source_caller=0x3fe838);
 bool has_like(const ItemInstanceV1*,std::uint32_t&,bool&,std::string&)const;
 bool add_quantity(ItemInstanceV1&,std::int32_t,std::string&);
 bool lifetime_slot(RetainedItemSlotV4,const ItemInstanceV1*,std::string&)const;
 bool add_fixed_loot_impl(std::int32_t,std::unique_ptr<ItemInstanceV1>&,bool,const OwnedInventoryServicesV4&,const OwnedLootEffectsV7*,const LootEntrySelectionContextV1*,std::vector<std::unique_ptr<OwnedItemSlotV4>>*,std::string&);
 bool equip_to_slot_impl(std::uint32_t,std::uint32_t,bool,std::unique_ptr<ItemInstanceV1>&,bool,const OwnedInventoryServicesV4&,std::string&);
public:
 // Live mode borrows the Character's one authoritative PropertyState and RNG.
 // Both remain caller-owned and must outlive this inventory.
 FreshInventoryOwnedV4(std::uintptr_t,LootTablesV2::Borrow,InventoryRandomServiceV4,std::int8_t,PropertyState&);
 // Compatibility constructor used only by imported original/host fixtures.
 FreshInventoryOwnedV4(std::uintptr_t,LootTablesV2::Borrow,LootRandom8V2&,std::int8_t,std::shared_ptr<PropertyState>);
 FreshInventoryOwnedV4(const FreshInventoryOwnedV4&)=delete;FreshInventoryOwnedV4& operator=(const FreshInventoryOwnedV4&)=delete;
 // Rebind this same retained Character inventory after a renderer snapshot
 // restores the Character into a new ObjectActor address. Identity must be
 // unchanged; inventory contents/equipment/gold are never copied or reset.
 bool rebind_properties(std::uintptr_t,PropertyState&,std::string&);
 bool add_fixed_loot(std::int32_t,const OwnedInventoryServicesV4&,std::string&);
 bool add_fixed_loot(std::int32_t,RetainedItemSlotV4,const OwnedInventoryServicesV4&,std::string&);
 bool add_fixed_loot(std::int32_t,const OwnedInventoryServicesV4&,const OwnedLootEffectsV7&,std::string&);
 bool add_fixed_loot(std::int32_t,RetainedItemSlotV4,const OwnedInventoryServicesV4&,const OwnedLootEffectsV7&,std::string&);
 // LootTable caller with source-order recursive child expansion. Descendant
 // random rows share the root quantity distribution and this owner's RNG.
 // Requires original class-count/DebugSwitch facts and shared loot effects.
 bool add_loot_table(std::int32_t,const LootEntrySelectionContextV1&,RetainedItemSlotV4,const OwnedInventoryServicesV4&,const OwnedLootEffectsV7&,std::string&);
 // Character::DropLoot stages generated items outside player inventory. A
 // later ItemWorld owner supplies position/visuals and calls pickup_world_item
 // with the same V4 owner when source Interact succeeds.
 bool add_world_loot_table(std::int32_t,const LootEntrySelectionContextV1&,RetainedItemSlotV4,const OwnedInventoryServicesV4&,const OwnedLootEffectsV7&,std::string&);
 bool pickup_world_item(std::size_t,std::int32_t&,const OwnedInventoryServicesV4&,std::string&);
 // ItemObject::Interact AutoTransmute prefix: transfer through this same V4
 // owner but deliberately omit the ordinary GatherLoot/after_world_pickup tail.
 // Returns the actual retained destination Item (which may be a merge target),
 // separately from the stable world-item projection identity. Indeterminate
 // means source ownership changed but AddItem's required callbacks did not all
 // complete; callers must never retry the transfer or award against a guess.
 WorldItemTransferResultV4 transfer_world_item_for_auto_transmute(
     std::size_t,std::int32_t& inventory_index,std::uintptr_t& inventory_item_identity,
     const OwnedInventoryServicesV4&,std::string&);
 bool retire_world_item(std::size_t,const OwnedInventoryServicesV4&,std::string&);
 // NativeInvDropItem's offline TransferItemTo(index,temp,1,false,false),
 // represented in this same owner. Rejects online calls and missing source
 // world providers before inventory mutation. If spawn_and_lock fails after
 // transfer, the source prefix remains in world_items_ and world_index names
 // that retained item so the caller can recover without losing ownership.
 bool drop_inventory_item_offline(std::uint32_t,const OwnedInventoryServicesV4&,const OfflineWorldItemDropServicesV4&,std::int32_t& world_index,std::string&);
 // Source-invalid indices/negative assertions and destructive native reentry
 // reject explicitly; read-only queries and live cached-property/selection writes
 // remain available synchronously in effects. Partial source prefixes persist.
 bool create_item(std::int32_t,std::uint32_t,std::unique_ptr<ItemInstanceV1>&,const OwnedInventoryServicesV4&,std::string&);
 bool create_item(std::int32_t,std::uint32_t,RetainedItemSlotV4,const OwnedInventoryServicesV4&,std::string&);
 // GEAR's direct Item calls share this owner's existing callback guard.
 // No additional store; only SetValue's UpdateName and AddPower are accepted.
 bool saved_item_effect(OwnedInventoryOperationV4,std::uint32_t,ItemInstanceV1*,std::int32_t,std::uint32_t,const OwnedInventoryServicesV4&,std::string&);
 bool split_item(ItemInstanceV1&,std::int32_t,std::unique_ptr<ItemInstanceV1>&,const OwnedInventoryServicesV4&,std::string&);
 bool split_item(ItemInstanceV1&,std::int32_t,RetainedItemSlotV4,const OwnedInventoryServicesV4&,std::string&);
 // Required synchronous retirement precedes reset; rejection/exception keeps
 // the caller's actual Item and full Presentation state in the same slot.
 bool retire_item(RetainedItemSlotV4,const OwnedInventoryServicesV4&,std::string&);
 // Source Character menu mutations on this same authoritative inventory.
 // Removal clears the source equipment pointers directly (it does not call
 // UnEquipSlot and therefore cannot merge a stack) before synchronously
 // retiring presentation attached to the actual Item and erasing its slot.
 bool remove_inventory_item(std::uint32_t,const OwnedInventoryServicesV4&,std::string&);
 // ItemInventory::RemoveOnePotion (0x40e878): decrement the canonical
 // potion Item through AddQty(-1), or retire that exact Item at quantity <= 1.
 bool remove_one_potion(const OwnedInventoryServicesV4&,std::string&);
 // Objective_GatherLoot's list-30 registration state, owned by this exact
 // Character inventory. It is not an EventManager or quest dispatcher.
 bool register_quest_gathering_item_id(std::int32_t,std::string&);
 bool unregister_quest_gathering_item_id(std::int32_t,std::string&);
 bool has_quest_gathering_item_id(std::int32_t,bool&,std::string&)const;
 // Objective_GatherLoot::InitWithCurrentQty's FindItem query against this
 // same inventory. Returns the first matching live Item's signed quantity,
 // or zero when the Character does not currently own that item.
 bool quest_gathering_item_quantity(std::int32_t,bool& found,std::int32_t&,std::string&)const;
 bool add_quantity_to_item(ItemInstanceV1&,std::int32_t,std::string&);
 // Native merchant callbacks mutate ItemInstance's buyback byte before
 // TransferItemTo. Keep that mutation on this same retained Item owner.
 bool set_item_buyback(std::uint32_t,bool,std::string&);
 // Source AddItemInstance owns input only after delivered storage/merge/delete.
 // Prefix is retained on required effect failure; caller retains unconsumed input.
 bool add_item(std::unique_ptr<ItemInstanceV1>&,bool force,bool convert_gold,std::int32_t& index,const OwnedInventoryServicesV4&,std::string&);
 // Cross-Character TransferItemTo over these two existing authoritative
 // owners. The same Item object is moved when the full stack is transferred;
 // partial stack transfers use this source inventory's normal split callbacks.
 // ItemPresentation ownership must be shared by both service bindings.
 OwnedItemTransferResultV4 transfer_item_to(std::uint32_t index,std::int32_t amount,
     FreshInventoryOwnedV4& destination,bool force,bool convert_gold,
     std::int32_t& destination_index,const OwnedInventoryServicesV4& source_services,
     const OwnedInventoryServicesV4& destination_services,RetainedItemSlotV4,
     std::string&);
 bool inventory_full(bool&,const OwnedInventoryServicesV4&,std::string&);
 bool auto_equip(std::uint32_t,std::int32_t& result,const OwnedInventoryServicesV4&,std::string&);
 bool character_auto_equip(std::uint32_t,std::int32_t& result,const OwnedInventoryServicesV4&,std::string&);
 bool equip_to_slot(std::uint32_t,std::uint32_t,bool,const OwnedInventoryServicesV4&,std::string&);
 bool equip_to_slot(std::uint32_t,std::uint32_t,bool,RetainedItemSlotV4,const OwnedInventoryServicesV4&,std::string&);
 bool unequip_from_slot(std::uint32_t,std::int32_t,const OwnedInventoryServicesV4&,std::string&);
 bool is_equipped(std::uint32_t,bool&,std::string&)const;
 bool has_two_hander(bool,bool&,std::string&)const;
 bool set_gold(std::int32_t,const OwnedInventoryServicesV4&,std::string&);
 bool add_gold(std::int32_t,const OwnedInventoryServicesV4&,std::string&);
 // ItemInventory::UpdateLocalizationForItems walks its current dense Item
 // vector in order. Text/power presentation is rebuilt on each same Item.
 bool update_localization(ItemPresentationOwnerV5&,const ItemTextServicesV5&,std::string&);
 static bool equal(const ItemInstanceV1&,const ItemInstanceV1&)noexcept;
 const auto& items()const noexcept{return items_;}const auto& equipment()const noexcept{return equipment_;}
 const auto& world_items()const noexcept{return world_items_;}
 const ItemInstanceV1* potion()const noexcept{return potion_;}std::int32_t num_potions()const noexcept;
 std::int8_t potion_capacity()const noexcept{return potion_capacity_;}
 const ItemTable& table()const{return tables_.items();}
 std::uintptr_t character()const noexcept{return character_;}
 std::int32_t current_equipment()const noexcept{return selected_;}void swap_equipment()noexcept{selected_=std::uint8_t(!selected_);}
 // Exact source GEAR byte store/temporary selection. Caller must use the
 // source 0/1 selection before indexing a weapon set; no second set mirror.
 void project_current_equipment(std::uint8_t value)noexcept{selected_=value;}
 PropertyState* properties()const noexcept{return properties_;}
 std::int32_t gold()const noexcept{return gold_;}
 // Caller must supply genuine source field writes; these are not producers.
 void project_potion_capacity(std::int8_t v)noexcept{potion_capacity_=v;}
 void project_gold_limit(std::int32_t v)noexcept{gold_limit_=v;}
 void project_unlimited(bool v)noexcept{unlimited_=v;}
};
}
