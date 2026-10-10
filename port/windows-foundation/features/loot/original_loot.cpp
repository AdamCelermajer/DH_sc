#include "original_loot.hpp"

namespace dh::foundation::loot {
bool select_items(dh2::data::LootTablesV2::Borrow tables,
                  dh2::data::LootPowerResourcesV7::Borrow powers,
                  dh2::data::LootRandom8V2& random, dh2::data::LootEntryServicesV8 services,
                  std::int32_t table, bool give_all, SelectedLoot& output, std::string& error) {
    output = {};
    error.clear();
    output.tables = tables;
    if (!tables) { error = "Missing actual source loot tables"; return false; }
    if (table < 0 || static_cast<std::size_t>(table) >= tables.loots().size()) return true;
    dh2::data::LootTableSelectionV8 select(tables, powers, random, services);
    std::vector<const dh2::data::LootEntry32V2*> entries;
    if (!select.select(table, entries, error)) return false;
    dh2::data::LootItemSelectionV8 expand(tables, random, services);
    return expand.expand(entries, give_all, output.items, error);
}
bool transfer_drop(dh2::data::LootTemporaryInventoryV8& inventory,
                   const PickupTransferServices& services, PickupReceipt& receipt,
                   std::string& error) {
    receipt = {};
    error.clear();
    receipt.items_before = inventory.items().size();
    receipt.items_after = receipt.items_before;
    if (receipt.items_before == 0) {
        receipt.outcome = PickupOutcome::already_empty;
        receipt.completed = true;
        return true;
    }
    if (!services.eligible) { error = "Missing original pickup eligibility provider"; return false; }
    bool allowed = false;
    if (!services.eligible(services.context, inventory, allowed, error)) return false;
    if (!allowed) {
        receipt.outcome = PickupOutcome::retained;
        receipt.completed = true;
        return true;
    }
    // Require every receiver before mutation; source effect failures still retain
    // reached ownership transfers and block destructive retry in native owner.
    if (!services.add || !services.after_add || !services.add_gold) {
        error = "Missing actual pickup inventory/quest/gold receiver";
        return false;
    }
    receipt.completed = inventory.transfer_all_source_v10(false, true, services.context,
        services.add, services.after_add, services.add_gold, error);
    receipt.items_after = inventory.items().size();
    receipt.outcome = !receipt.completed ? PickupOutcome::failed :
        receipt.items_after < receipt.items_before ? PickupOutcome::transferred : PickupOutcome::retained;
    return receipt.completed;
}
OriginalLootFlow::OriginalLootFlow(dh2::data::LootTablesV2::Borrow tables,
                                 dh2::data::LootPowerResourcesV7::Borrow powers,
                                 dh2::data::ItemPowerTablesV5::Borrow definitions,
                                 dh2::data::LootRandom8V2& random)
    : tables_(std::move(tables)), random_(random), inventory_(tables_),
      creation_(tables_, std::move(powers), std::move(definitions), random) {}
bool OriginalLootFlow::create(void* context, std::int32_t id,
                             std::unique_ptr<dh2::data::ItemInstanceV1>& item, std::string& error) {
    auto& bridge = *static_cast<Bridge*>(context);
    return bridge.self->inventory_.create(id, item, bridge.services->text, error);
}
bool OriginalLootFlow::store(void* context,
                            std::unique_ptr<dh2::data::ItemInstanceV1>& item, std::string& error) {
    auto& bridge = *static_cast<Bridge*>(context);
    return bridge.self->inventory_.store(item, bridge.services->entry, bridge.services->context,
                                        bridge.services->full_notifications, error);
}
bool OriginalLootFlow::query(void* context, const dh2::data::LootCreationQueryV8& request,
                            dh2::data::LootCreationResponseV8& response, std::string& error) {
    const auto& services = *static_cast<Bridge*>(context)->services;
    if (!services.query) { error = "Missing actual loot player/difficulty query"; return false; }
    return services.query(services.context, request, response, error);
}
bool OriginalLootFlow::generate(const LootRequest& request, const GenerationServices& services,
                               GenerationReceipt& receipt, std::string& error) {
    receipt = {};
    receipt.items_before = inventory_.items().size();
    receipt.random_calls_before = random_.calls;
    if (running_) { error = "Unsupported loot flow reentry"; return false; }
    running_ = true;
    struct Reset { bool& running; ~Reset() { running = false; } } reset{running_};
    receipt.source_no_table = tables_ && (request.table < 0 ||
        static_cast<std::size_t>(request.table) >= tables_.loots().size());
    Bridge bridge{this, &services};
    dh2::data::LootCreationServicesV8 source{services.entry, services.power, services.text,
                                          &bridge, query, create, store};
    receipt.completed = creation_.add(request.table, request.value_bonus256, request.power_bonus256,
                                     request.fixed_power_count, request.bypass_difficulty_variant,
                                     request.give_all, source, error);
    receipt.items_after = inventory_.items().size();
    receipt.random_calls_after = random_.calls;
    receipt.pending_failed_item = creation_.pending_item() != nullptr;
    return receipt.completed;
}
} // namespace dh::foundation::loot
