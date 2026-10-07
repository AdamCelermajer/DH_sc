#pragma once
#include "../game-data/loot_temporary_inventory_v8.hpp"
namespace dh2::character {
struct WorldItemLootCreationServicesV10 {
 data::LootCreationServicesV8 source;
 void* notifications_context{};
 bool(*full_notifications)(void*,data::LootTemporaryInventoryV8&,data::ItemInstanceV1&,std::string&){};
};
// Actual AddLoot pipeline into the SAME pending NULL-character inventory.
// Immutable table/power/text providers and Application RNG are borrowed; no
// player inventory, extra random owner or alternate drop table is created.
class WorldItemLootCreationV10 {
 data::LootCreationV8 source_;
 WorldItemLootCreationServicesV10 services_;
 data::LootTemporaryInventoryV8* current_{};
 static bool create(void*,std::int32_t,std::unique_ptr<data::ItemInstanceV1>&,std::string&);
 static bool store(void*,std::unique_ptr<data::ItemInstanceV1>&,std::string&);
 static bool query(void*,const data::LootCreationQueryV8&,data::LootCreationResponseV8&,std::string&);
public:
 WorldItemLootCreationV10(data::LootTablesV2::Borrow t,data::LootPowerResourcesV7::Borrow p,
  data::ItemPowerTablesV5::Borrow d,data::LootRandom8V2& rng,WorldItemLootCreationServicesV10 s)
  :source_(std::move(t),std::move(p),std::move(d),rng),services_(std::move(s)){}
 bool add(data::LootTemporaryInventoryV8&,std::int32_t table,std::int32_t value_bonus,
  std::int32_t power_bonus,std::int32_t fixed_powers,std::string&);
 data::ItemInstanceV1* pending_item()const noexcept{return source_.pending_item();}
};
}
