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
 LootPowerResourcesV7::Borrow resources_;LootRandom8V2* random_;bool running_{};
public:
 LootPowerCreationV7(LootPowerResourcesV7::Borrow,LootRandom8V2&);
 bool add_powers(const LootEntry32V2&,ItemInstanceV1&,std::int32_t bonus256,
                 std::int32_t requested_count,std::int32_t difficulty,
                 const LootPowerServicesV7&,std::string&);
 const LootPowerResourcesV7::Borrow& resources()const noexcept{return resources_;}
};
// Source CalcLootItemValue: mutates value, then performs genuine UpdateName.
// Gold uses the same supplied source RNG; other items include each actual power
// definition's GoldBonus*GoldBonusMultiplier with source wrapping arithmetic.
bool loot_item_value_v7(ItemInstanceV1&,const ItemTable&,ItemPowerTablesV5::Borrow,
                        LootRandom8V2&,std::int32_t bonus256,
                        const ItemTextServicesV5&,std::string&);
struct LootPowerGold8V7 {std::int32_t multiplier,bonus;};
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
