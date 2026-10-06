#pragma once
#include "loot_table_selection_v8.hpp"
#include "loot_item_selection_v8.hpp"
namespace dh2::data {
enum class LootCreationOperationV8:std::uint32_t {player_count=0x4043a8,current_level=0x31f594};
struct LootCreationQueryV8 {LootCreationOperationV8 operation;std::uint32_t caller;};
struct LootCreationResponseV8 {bool present{};std::int32_t value{};};
struct LootCreationServicesV8 {
 LootEntryServicesV8 entry;
 LootPowerServicesV7 power;
 ItemTextServicesV5 text;
 void* context{};
 bool(*query)(void*,const LootCreationQueryV8&,LootCreationResponseV8&,std::string&){};
 // Must construct the actual source ItemInstance with ordered name/stats/
 // requirements effects. Store consumes into the actual destination inventory
 // through source force=true/convertGold=false AddItemInstance, including its
 // reached notifications. Neither service may report successful empty work.
 bool(*create)(void*,std::int32_t,std::unique_ptr<ItemInstanceV1>&,std::string&){};
 bool(*store)(void*,std::unique_ptr<ItemInstanceV1>&,std::string&){};
};
// Entire source AddLoot selection/expansion/difficulty/creation/power/value/
// insertion order. Give-all is source Inventory+2d, distinct from unlimited+2f.
// Same caller-owned RNG and real item identities flow through all stages.
class LootCreationV8 {
 LootTablesV2::Borrow tables_;LootPowerResourcesV7::Borrow powers_;
 ItemPowerTablesV5::Borrow definitions_;LootRandom8V2& random_;bool running_{};
 std::unique_ptr<ItemInstanceV1> pending_;
public:
 LootCreationV8(LootTablesV2::Borrow t,LootPowerResourcesV7::Borrow p,
                ItemPowerTablesV5::Borrow d,LootRandom8V2& r):tables_(std::move(t)),powers_(std::move(p)),definitions_(std::move(d)),random_(r){}
 bool add(std::int32_t table,std::int32_t value_bonus256,
          std::int32_t power_bonus256,std::int32_t fixed_power_count,
          bool bypass_difficulty_variant,bool give_all,
          const LootCreationServicesV8&,std::string&);
 // Reached constructor/power/value prefixes retain the actual item on failure.
 // The caller must detach/forget its presentation before destroying this item.
 ItemInstanceV1* pending_item()const noexcept{return pending_.get();}
 std::unique_ptr<ItemInstanceV1> release_pending_item()noexcept{return std::move(pending_);}
};
}
