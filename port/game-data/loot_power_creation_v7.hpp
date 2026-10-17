#pragma once
#include "loot_power_resources_v7.hpp"
#include "loot_tables_v2.hpp"
#include "item_presentation_v5.hpp"

namespace dh2::data {
enum class LootPowerOperationV7:std::uint32_t {debug_load=0x337888,debug_query=0x337a88,add_power=0x3fbc60};
struct LootPowerRequestV7 {
 LootPowerOperationV7 operation;std::uint32_t source_caller;
 ItemInstanceV1* item;const char* key;std::int32_t power,difficulty;
};
// Debug and AddPower are actual required providers. An AddPower provider must
// operate on this exact item and its actual V5 presentation owner. Its reached
// failed append prefix remains visible. No successful empty service is assumed.
struct LootPowerServicesV7 {
 void* context{};
 bool(*invoke)(void*,const LootPowerRequestV7&,std::int32_t&,std::string&){};
};
class LootPowerCreationV7 {
 LootPowerResourcesV7::Borrow resources_;InventoryRandomServiceV4 random_;bool running_{};
public:
 // Shares the live inventory's caller-owned RNG descriptor, stream 0. Its
 // context outlives this owner; no seed, counter or second random store exists.
 LootPowerCreationV7(LootPowerResourcesV7::Borrow,InventoryRandomServiceV4);
 // Frozen source-fixture compatibility; borrows this exact RNG without copying.
 LootPowerCreationV7(LootPowerResourcesV7::Borrow,LootRandom8V2&);
 bool add_powers(const LootEntry32V2&,ItemInstanceV1&,std::int32_t bonus256,
                 std::int32_t requested_count,std::int32_t difficulty,
                 const LootPowerServicesV7&,std::string&);
 const LootPowerResourcesV7::Borrow& resources()const noexcept{return resources_;}
 const InventoryRandomServiceV4& random_service()const noexcept{return random_;}
};
// Source CalcLootItemValue: mutates value, then performs genuine UpdateName.
// Gold uses the same supplied source RNG; other items include each actual power
// definition's GoldBonus*GoldBonusMultiplier with source wrapping arithmetic.
bool loot_item_value_v7(ItemInstanceV1&,const ItemTable&,ItemPowerTablesV5::Borrow,
                        const InventoryRandomServiceV4&,std::int32_t bonus256,
                        const ItemTextServicesV5&,std::string&);
bool loot_item_value_v7(ItemInstanceV1&,const ItemTable&,ItemPowerTablesV5::Borrow,
                        LootRandom8V2&,std::int32_t bonus256,
                        const ItemTextServicesV5&,std::string&);
struct LootPowerGold8V7 {std::int32_t multiplier,bonus;};
// Same original arithmetic/selection kernels as the frozen C fixtures below.
// 0 success, -1 malformed projection, -2 required selection/RNG continuation.
// Callback failures preserve draws and reached item prefixes. Output changes
// only after success; callers retain the returned provider error separately.
int loot_power_select_v7(std::uint32_t*,const InventoryRandomServiceV4&,
 const LootPowerChoiceV7*,std::uint32_t,std::string&) noexcept;
int loot_quantity_v7(std::int32_t*,const InventoryRandomServiceV4&,
 const LootQuantityChoiceV7*,std::uint32_t,std::int32_t,std::string&) noexcept;
int loot_value_v7(std::int32_t*,const InventoryRandomServiceV4&,
 const ItemRecord164*,const LootPowerGold8V7*,std::uint32_t,std::int32_t,std::string&) noexcept;
}
extern "C" {
// Return 0 success/-1 malformed/-2 required original Debug continuation.
// Outputs change only on success; RNG prefixes persist after a real draw.
int dh2_loot_power_select_v7(std::uint32_t*,dh2::data::LootRandom8V2*,
 const dh2::data::LootPowerChoiceV7*,std::uint32_t) noexcept;
int dh2_loot_quantity_v7(std::int32_t*,dh2::data::LootRandom8V2*,
 const dh2::data::LootQuantityChoiceV7*,std::uint32_t,std::int32_t bonus) noexcept;
int dh2_loot_item_value_v7(std::int32_t*,dh2::data::LootRandom8V2*,
 const dh2::data::ItemRecord164*,const dh2::data::LootPowerGold8V7*,
 std::uint32_t,std::int32_t bonus256) noexcept;
}
