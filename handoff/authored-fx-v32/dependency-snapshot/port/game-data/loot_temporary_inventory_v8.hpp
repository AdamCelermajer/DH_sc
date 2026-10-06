#pragma once
#include "loot_creation_v8.hpp"
namespace dh2::data {
// Actual source ItemInventoryC1 NULL-character drop inventory, distinct from
// the live Player inventory. It owns the items that have not yet been spawned;
// it never constructs a dummy Character/property state to reuse Player logic.
class LootTemporaryInventoryV8 {
 LootTablesV2::Borrow tables_;
 std::vector<std::unique_ptr<ItemSlotV1>> items_;
 ItemInstanceV1* potion_{};bool running_{},destructive_failed_v2_{};
 ItemSlotV1* transferred_slot_{};ItemInstanceV1* transferred_item_{};
public:
 explicit LootTemporaryInventoryV8(LootTablesV2::Borrow t):tables_(std::move(t)){}
 LootTemporaryInventoryV8(const LootTemporaryInventoryV8&)=delete;
 LootTemporaryInventoryV8& operator=(const LootTemporaryInventoryV8&)=delete;
 bool create(std::int32_t,std::unique_ptr<ItemInstanceV1>&,
             const ItemTextServicesV5&,std::string&);
 // The recovered AddLoot caller always force=true, convertGold=false. Source
 // full-notification tail is required after actual storage when reached.
 bool store(std::unique_ptr<ItemInstanceV1>&,const LootEntryServicesV8&,
            void*,bool(*full_notifications)(void*,LootTemporaryInventoryV8&,ItemInstanceV1&,std::string&),std::string&);
 const auto& items()const noexcept{return items_;}
 ItemInstanceV1* peek(std::uint32_t index=0)const noexcept;
 const Item* metadata(const ItemInstanceV1& item)const noexcept{return tables_?data::item(tables_.items(),item.id):nullptr;}
 const ItemInstanceV1* potion()const noexcept{return potion_;}
 static constexpr std::uintptr_t character()noexcept{return 0;}
 static constexpr std::int32_t gold()noexcept{return 0;}
 static constexpr std::int32_t gold_limit()noexcept{return INT32_MAX;}
 static constexpr std::int8_t potion_capacity()noexcept{return -1;}
 static constexpr bool give_all_items()noexcept{return false;}
 static constexpr bool unlimited()noexcept{return false;}
 // Actual Spawn/RemoveItemInstance service uses this ownership transfer only
 // after delivering the source remove body. No automatic pickup is implied.
 bool release_spawned(ItemInstanceV1&,std::unique_ptr<ItemInstanceV1>&,std::string&);
 // Source RemoveAllItems(true) over this NULL-character inventory. Its two
 // equipment vectors are genuinely absent/empty. Optional destruction hook
 // releases the SAME retained ItemPresentation owner before native deletion.
 bool remove_all_owned_v2(void*,bool(*before_destroy)(void*,ItemInstanceV1&,std::string&),std::string&);
 // Whole positive-quantity TransferItemTo into a real destination. Source
 // slot removal follows destination AddItemInstance. A reached destination
 // failure retains a borrowed source slot alias if the actual item already
 // moved into destination storage; it never creates another item owner.
 bool transfer(std::uint32_t index,bool force,bool convert_gold,void*,
               bool(*add_destination)(void*,std::unique_ptr<ItemInstanceV1>&,
                                      bool,bool,std::int32_t&,std::string&),
               std::int32_t& result,std::string&);
 // Whole TransferInventoryTo3ffa68 for this actual NULL-character owner:
 // Add -> quest tail -> source slot deletion, declaration order; vector end
 // reset after all slots; destination AddGold(source gold0) tail still runs.
 // after_add receives captured source ID, avoiding a legacy post-Add item read
 // when the real destination may destroy/merge the transferred instance.
 bool transfer_all_source_v10(bool force,bool convert_gold,void*,
  bool(*add)(void*,std::unique_ptr<ItemInstanceV1>&,bool,bool,std::int32_t&,std::string&),
  bool(*after_add)(void*,std::int32_t,std::string&),
  bool(*add_gold)(void*,std::int32_t,std::string&),std::string&);
};
}
