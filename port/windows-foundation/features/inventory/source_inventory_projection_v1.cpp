#include "source_inventory_projection_v1.hpp"
#include "source_instance_resolver.hpp"

#include <algorithm>
#include <atomic>
#include <limits>
#include <map>
#include <set>

namespace dh::foundation::inventory {
namespace {
using Gear = dh2::player::PlayerEquipmentRenderOwnerV1;
using Inventory = dh2::data::FreshInventoryOwnedV4;
using Instance = dh2::data::ItemInstanceV1;

std::atomic<std::uint64_t> next_ui_instance_id{1};

std::string fresh_ui_instance_id() {
    const auto id = next_ui_instance_id.fetch_add(1, std::memory_order_relaxed);
    return "source-native-ui-v1:" + std::to_string(id);
}

bool same_owner_control_block(const std::shared_ptr<const Gear>& a,
                              const std::shared_ptr<const Gear>& b) {
    const std::owner_less<std::shared_ptr<const Gear>> less;
    return !less(a, b) && !less(b, a);
}

const char* slot_name(std::size_t slot) {
    static constexpr const char* names[] = {
        "slot0", "slot1", "slot2", "slot3", "slot4",
        "slot5", "slot6", "slot7", "slot8"};
    return slot < 9 ? names[slot] : nullptr;
}
} // namespace

bool SourceInventoryProjectionV1::project_locked(
    bool allow_owner_replacement, SourceInventoryProjectionRowsV1& output,
    std::string& error) {
    if (!borrow_) {
        error = "Native Gear inventory lease borrower is unavailable";
        return false;
    }
    if (!invalidate_instance_binding_) {
        error = "Caller native instance-resolver invalidation callback is unavailable";
        return false;
    }
    NativeGearInventoryLeaseV1 loan;
    if (!borrow_(loan, error)) return false;
    if (!loan.gear || !loan.gear->ready() || !loan.gear->inventory()) {
        error = "Borrowed native Gear owner is not ready with its actual FreshInventoryOwnedV4";
        return false;
    }
    const auto* native_inventory = loan.gear->inventory();
    if (initialized_ && !allow_owner_replacement) {
        if (loan.gear.get() != gear_.get() ||
            !same_owner_control_block(loan.gear, gear_)) {
            error = "Native Gear owner/control block changed; use restore rebind";
            return false;
        }
        if (native_inventory != inventory_) {
            error = "Native Gear inventory owner changed without a Gear restore rebind";
            return false;
        }
    }
    const auto& rows = native_inventory->items();
    const auto& table = native_inventory->table();
    if (rows.size() > std::numeric_limits<std::uint32_t>::max() ||
        table.identifiers.size() != table.rows.size()) {
        error = "Native Gear inventory order or ItemTable identifiers exceed projection bounds";
        return false;
    }

    SourceInventoryProjectionRowsV1 next;
    next.inventory.reserve(rows.size());
    next.source_indices.reserve(rows.size());
    next.native_instances.reserve(rows.size());
    std::unordered_map<const Instance*, std::string> next_live_ids;
    next_live_ids.reserve(rows.size());
    std::unordered_map<const Instance*, std::string> id_by_instance;
    id_by_instance.reserve(rows.size());

    for (std::size_t index = 0; index < rows.size(); ++index) {
        const auto* instance = rows[index] && rows[index]->item
            ? rows[index]->item.get() : nullptr;
        if (!instance) {
            error = "Native Gear inventory contains a tombstone; source row projection cannot skip order";
            return false;
        }
        if (instance->id < 0 ||
            static_cast<std::size_t>(instance->id) >= table.identifiers.size() ||
            !dh2::data::item(table, instance->id)) {
            error = "Native Gear ItemInstance has no actual ItemTable definition";
            return false;
        }
        const auto quantity = instance->signed_quantity();
        if (quantity <= 0) {
            error = "Native Gear ItemInstance has a nonpositive source signed quantity";
            return false;
        }
        const auto existing = live_ids_.find(instance);
        const auto id = existing == live_ids_.end()
            ? fresh_ui_instance_id() : existing->second;
        if (id.empty() || !next_live_ids.emplace(instance, id).second ||
            !id_by_instance.emplace(instance, id).second) {
            error = "Duplicate native ItemInstance pointer in actual Gear inventory order";
            return false;
        }
        next.inventory.push_back({id, table.identifiers[static_cast<std::size_t>(instance->id)],
                                  static_cast<std::uint32_t>(quantity)});
        next.source_indices.push_back(static_cast<std::uint32_t>(index));
        next.native_instances.push_back(instance);
    }

    const auto selected_set = native_inventory->current_equipment();
    if (selected_set < 0 || selected_set >= 2) {
        error = "Native Gear selected equipment set is outside source two-set bounds";
        return false;
    }
    const auto& equipment = native_inventory->equipment()[static_cast<std::size_t>(selected_set)];
    for (std::size_t slot = 0; slot < equipment.size(); ++slot) {
        const auto* source_slot = equipment[slot];
        if (!source_slot) continue;
        if (!source_slot->item) {
            error = "Native Gear equipment slot has no owned ItemInstance";
            return false;
        }
        const auto found = id_by_instance.find(source_slot->item.get());
        if (found == id_by_instance.end()) {
            error = "Native Gear equipment pointer does not alias its actual inventory order";
            return false;
        }
        next.equipment.push_back({slot_name(slot), found->second});
    }

    next.gear_epoch = gear_epoch_;
    gear_ = std::move(loan.gear);
    inventory_ = native_inventory;
    live_ids_ = std::move(next_live_ids); // Removed pointer IDs retire at publication.
    initialized_ = true;
    valid_ = true;
    output = std::move(next);
    error.clear();
    return true;
}

bool SourceInventoryProjectionV1::bind_initial(
    SourceInventoryProjectionRowsV1& output, std::string& error) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (initialized_) {
        error = "Native Gear inventory projection is already initialized";
        return false;
    }
    return project_locked(false, output, error);
}

void SourceInventoryProjectionV1::invalidate_before_native_mutation() noexcept {
    if (invalidate_instance_binding_) {
        try { invalidate_instance_binding_(); } catch (...) {}
    }
    std::lock_guard<std::mutex> lock(mutex_);
    valid_ = false;
}

bool SourceInventoryProjectionV1::retire_before_native_item_removal(
    const dh2::data::ItemInstanceV1* item) noexcept {
    if (!item) return false;
    if (invalidate_instance_binding_) {
        try { invalidate_instance_binding_(); } catch (...) {}
    }
    std::lock_guard<std::mutex> lock(mutex_);
    valid_ = false;
    return live_ids_.erase(item) != 0;
}

bool SourceInventoryProjectionV1::refresh_after_native_mutation(
    SourceInventoryProjectionRowsV1& output, std::string& error) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!initialized_ || valid_) {
        error = "Native Gear projection refresh requires prior mutation invalidation";
        return false;
    }
    return project_locked(false, output, error);
}

bool SourceInventoryProjectionV1::rebind_after_gear_restore(
    SourceInventoryProjectionRowsV1& output, std::string& error) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (gear_epoch_ == std::numeric_limits<std::uint64_t>::max()) {
        valid_ = false;
        error = "Native Gear projection epoch exhausted";
        return false;
    }
    ++gear_epoch_;
    valid_ = false;
    live_ids_.clear();
    return project_locked(true, output, error);
}

bool SourceInventoryProjectionV1::projection_valid() const noexcept {
    std::lock_guard<std::mutex> lock(mutex_);
    return valid_;
}

std::uint64_t SourceInventoryProjectionV1::gear_epoch() const noexcept {
    std::lock_guard<std::mutex> lock(mutex_);
    return gear_epoch_;
}

bool apply_source_inventory_projection_v1(
    const SourceInventoryProjectionRowsV1& projection, CharacterState& target,
    std::string& error) {
    if (projection.inventory.size() != projection.source_indices.size() ||
        projection.inventory.size() != projection.native_instances.size()) {
        error = "Native Gear projection row/source-index/pointer association is inconsistent";
        return false;
    }
    auto inventory = projection.inventory;
    auto equipment = projection.equipment;
    target.inventory.swap(inventory);
    target.equipment.swap(equipment);
    error.clear();
    return true;
}

bool bind_source_inventory_projection_v1(
    const SourceInventoryProjectionRowsV1& projection, CharacterState& target,
    SourceInventoryInstanceResolver& resolver, std::string& error) {
    auto previous_inventory = target.inventory;
    auto previous_equipment = target.equipment;
    if (!apply_source_inventory_projection_v1(projection, target, error)) return false;
    if (!resolver.bind_projection(target, error)) {
        target.inventory.swap(previous_inventory);
        target.equipment.swap(previous_equipment);
        return false;
    }
    for (std::size_t i = 0; i < projection.inventory.size(); ++i) {
        SourceInstanceLease lease;
        if (!resolver.resolve(projection.inventory[i].instance_id, lease, error) ||
            lease.item != projection.native_instances[i] ||
            lease.source_index != projection.source_indices[i] ||
            !lease.owns || !lease.owns(lease.item)) {
            resolver.invalidate_projection();
            if (error.empty())
                error = "Stable UI instance ID did not bind to its projected native pointer/index witness";
            target.inventory.swap(previous_inventory);
            target.equipment.swap(previous_equipment);
            return false;
        }
    }
    error.clear();
    return true;
}

} // namespace dh::foundation::inventory
