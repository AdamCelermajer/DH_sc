#pragma once
#include "source_metadata_owner_v1.hpp"
#include "../../../game-data/loot_power_resources_v7.hpp"
#include "../../../game-data/loot_tables_v2.hpp"
#include "../../../game-data/loot_creation_v8.hpp"
#include "../../original_actor_properties.hpp"

namespace dh::foundation::inventory {
// Presentation companion of the already loaded RuntimeLootSourceV1. It reuses
// immutable source snapshots and ONE existing ItemText/HudText owner. No cache
// files, V88 resources, design tables, localization book or RNG are loaded.
struct RuntimeItemResourcesV1 : std::enable_shared_from_this<RuntimeItemResourcesV1> {
    dh2::data::LootTablesV2::Borrow loot;
    dh2::data::ItemPowerTablesV5::Borrow definitions;
    dh2::data::LootPowerResourcesV7::Borrow power_resources;
    std::shared_ptr<const OriginalPropertyDatabase> properties;
    std::shared_ptr<dh2::ui::ItemTextOwnerV5> text;
    std::shared_ptr<const void> localization_pin;
    std::shared_ptr<dh2::data::ItemPresentationOwnerV5> presentation;
    std::unique_ptr<SourceMetadataOwnerV1> inventory(SourceMetadataOwnerV1::RowBorrow);
};
bool bind_runtime_item_resources_v1(
    dh2::data::LootTablesV2::Borrow,
    dh2::data::ItemPowerTablesV5::Borrow,
    dh2::data::LootPowerResourcesV7::Borrow,
    std::shared_ptr<const OriginalPropertyDatabase>,
    std::shared_ptr<dh2::ui::ItemTextOwnerV5> existing_text,
    std::shared_ptr<const void> existing_localization_pin,
    std::shared_ptr<RuntimeItemResourcesV1>&,std::string&);
// Same current source debug/class provider and actual session/level/PM query.
// roll_stock_instances supplies its transactional create/store receivers.
class RuntimeItemLootCreationV1 {
    std::shared_ptr<RuntimeItemResourcesV1> resources_;
    dh2::data::LootEntryServicesV8 entry_;
    std::function<bool(const dh2::data::LootCreationQueryV8&,dh2::data::LootCreationResponseV8&,std::string&)> query_;
    static bool power(void*,const dh2::data::LootPowerRequestV7&,std::int32_t&,std::string&);
    static bool query(void*,const dh2::data::LootCreationQueryV8&,dh2::data::LootCreationResponseV8&,std::string&);
public:
    RuntimeItemLootCreationV1(std::shared_ptr<RuntimeItemResourcesV1> r,dh2::data::LootEntryServicesV8 e,
      decltype(query_) q):resources_(std::move(r)),entry_(e),query_(std::move(q)){}
    RuntimeItemLootCreationV1(const RuntimeItemLootCreationV1&)=delete;
    RuntimeItemLootCreationV1& operator=(const RuntimeItemLootCreationV1&)=delete;
    dh2::data::LootCreationServicesV8 services();
    static bool release_generated(void*,dh2::data::ItemInstanceV1&,std::string&);
};
}
