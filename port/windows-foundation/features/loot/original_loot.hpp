#pragma once
#include "../../../game-data/loot_temporary_inventory_v8.hpp"
#include <cstdint>
#include <string>

namespace dh2::character {
class WorldLootItemRuntimeV1;
class WorldItemLiveOwnerV5;
}
namespace dh::foundation::loot {
struct LootRequest {
    std::int32_t table = -1;
    std::int32_t value_bonus256 = 0, power_bonus256 = 0, fixed_power_count = -1;
    bool bypass_difficulty_variant = false, give_all = false;
};
struct SelectedLoot {
    dh2::data::LootTablesV2::Borrow tables;
    std::vector<dh2::data::LootItemInfoV8> items;
};
bool select_items(dh2::data::LootTablesV2::Borrow, dh2::data::LootPowerResourcesV7::Borrow,
                  dh2::data::LootRandom8V2&, dh2::data::LootEntryServicesV8,
                  std::int32_t table, bool give_all, SelectedLoot&, std::string&);
// These are original source providers, sharing the actual world/player/query
// state. Creation/storage are bound internally to the SAME temporary inventory.
struct GenerationServices {
    dh2::data::LootEntryServicesV8 entry;
    dh2::data::LootPowerServicesV7 power;
    dh2::data::ItemTextServicesV5 text;
    void* context = nullptr;
    bool (*query)(void*, const dh2::data::LootCreationQueryV8&,
                  dh2::data::LootCreationResponseV8&, std::string&) = nullptr;
    bool (*full_notifications)(void*, dh2::data::LootTemporaryInventoryV8&,
                               dh2::data::ItemInstanceV1&, std::string&) = nullptr;
};
struct GenerationReceipt {
    bool completed = false, source_no_table = false;
    std::size_t items_before = 0, items_after = 0;
    std::uint32_t random_calls_before = 0, random_calls_after = 0;
    bool pending_failed_item = false;
};
struct DropReceipt {
    bool completed = false;
    std::size_t items_before = 0, items_after = 0;
};
enum class PickupOutcome { transferred, retained, already_empty, unknown_item, failed };
struct PickupReceipt {
    PickupOutcome outcome = PickupOutcome::failed;
    std::size_t items_before = 0, items_after = 0;
    bool completed = false;
};
// Clean-runtime bridge over caller-owned world-item inventory. The caller's
// gate implements original fullness/potion/transmute eligibility. Transfer uses
// source force=false/convertGold=true, exact quest tail and AddGold(0) tail.
struct PickupTransferServices {
    void* context = nullptr;
    bool (*eligible)(void*, dh2::data::LootTemporaryInventoryV8&, bool&, std::string&) = nullptr;
    bool (*add)(void*, std::unique_ptr<dh2::data::ItemInstanceV1>&, bool, bool,
                std::int32_t&, std::string&) = nullptr;
    bool (*after_add)(void*, std::int32_t, std::string&) = nullptr;
    bool (*add_gold)(void*, std::int32_t, std::string&) = nullptr;
};
bool transfer_drop(dh2::data::LootTemporaryInventoryV8&, const PickupTransferServices&,
                   PickupReceipt&, std::string&);
// No second RNG, player inventory, spawn IDs, balances, or world object pool.
// Reached constructor/transfer prefixes are retained by original native owners.
class OriginalLootFlow {
public:
    OriginalLootFlow(dh2::data::LootTablesV2::Borrow,
                     dh2::data::LootPowerResourcesV7::Borrow,
                     dh2::data::ItemPowerTablesV5::Borrow,
                     dh2::data::LootRandom8V2&);
    bool generate(const LootRequest&, const GenerationServices&, GenerationReceipt&, std::string&);
    bool drop(dh2::character::WorldLootItemRuntimeV1&, std::uintptr_t source,
              std::uintptr_t killer, DropReceipt&, std::string&);
    static bool pickup(dh2::character::WorldItemLiveOwnerV5&, std::uintptr_t item,
                       std::uintptr_t character, PickupReceipt&, std::string&);
    dh2::data::LootTemporaryInventoryV8& inventory() noexcept { return inventory_; }
    dh2::data::ItemInstanceV1* pending_item() const noexcept { return creation_.pending_item(); }
    // Caller must release the original presentation before destroying a failed
    // constructor item; explicit recovery prevents an unsafe hidden cleanup.
    std::unique_ptr<dh2::data::ItemInstanceV1> release_pending_item() noexcept {
        return creation_.release_pending_item();
    }
private:
    dh2::data::LootTablesV2::Borrow tables_;
    dh2::data::LootRandom8V2& random_;
    dh2::data::LootTemporaryInventoryV8 inventory_;
    dh2::data::LootCreationV8 creation_;
    bool running_ = false;
    struct Bridge { OriginalLootFlow* self; const GenerationServices* services; };
    static bool create(void*, std::int32_t, std::unique_ptr<dh2::data::ItemInstanceV1>&, std::string&);
    static bool store(void*, std::unique_ptr<dh2::data::ItemInstanceV1>&, std::string&);
    static bool query(void*, const dh2::data::LootCreationQueryV8&,
                      dh2::data::LootCreationResponseV8&, std::string&);
};
} // namespace dh::foundation::loot
