// Optional adapter for hosts that already own the recovered canonical world.
#include "original_loot.hpp"
#include "../../../level-world/world_item_live_owner_v5.hpp"
namespace dh::foundation::loot {
bool OriginalLootFlow::drop(dh2::character::WorldLootItemRuntimeV1& pool, std::uintptr_t source,
                           std::uintptr_t killer, DropReceipt& receipt, std::string& error) {
    receipt = {};
    receipt.items_before = inventory_.items().size();
    if (running_) { error = "Unsupported loot flow reentry"; return false; }
    if (creation_.pending_item()) { error = "Pending failed loot item requires explicit recovery"; return false; }
    running_ = true;
    struct Reset { bool& running; ~Reset() { running = false; } } reset{running_};
    receipt.completed = pool.drop(inventory_, source, killer, error);
    receipt.items_after = inventory_.items().size();
    return receipt.completed;
}
bool OriginalLootFlow::pickup(dh2::character::WorldItemLiveOwnerV5& world, std::uintptr_t item,
                             std::uintptr_t character, PickupReceipt& receipt, std::string& error) {
    receipt = {};
    error.clear();
    auto* receiver = world.pool().receiver(item);
    if (!receiver) { receipt.outcome = PickupOutcome::unknown_item; error = "Unknown canonical world loot item"; return false; }
    receipt.items_before = receiver->inventory().items().size();
    if (receipt.items_before == 0) {
        receipt.outcome = PickupOutcome::already_empty;
        receipt.completed = true;
        return true;
    }
    // Fullness/potion/transmute/quest/gold/stack/FX/despawn decisions remain in
    // the SAME live world item and its actual interaction services.
    receipt.completed = world.interact(item, character, error);
    receiver = world.pool().receiver(item);
    receipt.items_after = receiver ? receiver->inventory().items().size() : 0;
    if (!receipt.completed) receipt.outcome = PickupOutcome::failed;
    else if (receipt.items_after < receipt.items_before) receipt.outcome = PickupOutcome::transferred;
    else receipt.outcome = PickupOutcome::retained;
    return receipt.completed;
}
} // namespace dh::foundation::loot
