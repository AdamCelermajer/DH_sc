#pragma once

#include "../../character_state.hpp"
#include "../../../game-data/fresh_inventory_owned_v4.hpp"
#include "../../../level-world/player_equipment_render_owner_v1.hpp"

#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace dh::foundation::inventory {
class SourceInventoryInstanceResolver;

// A fresh typed loan from the completed-character owner. Keeping this shared
// Gear pointer pins the same actual owner that holds FreshInventoryOwnedV4.
struct NativeGearInventoryLeaseV1 {
    std::shared_ptr<const dh2::player::PlayerEquipmentRenderOwnerV1> gear;
};

using NativeGearInventoryBorrowV1 = std::function<bool(
    NativeGearInventoryLeaseV1&, std::string&)>;
using SourceInventoryProjectionInvalidateV1 = std::function<void()>;

// Read-only menu projection. Source associations are aligned with inventory:
// these pointers and indices are ephemeral witnesses, never serialized fields.
struct SourceInventoryProjectionRowsV1 {
    std::vector<InventoryItem> inventory;
    std::vector<std::uint32_t> source_indices;
    std::vector<const dh2::data::ItemInstanceV1*> native_instances;
    std::vector<EquipmentBinding> equipment;
    std::uint64_t gear_epoch{};
};

// Projects only the real native inventory order and current equipment set.
// IDs are process-local UI lifecycle witnesses, never source identifiers or
// Save IDs. Call retire_before_native_item_removal before the native owner
// destroys an item, so a later allocation at the same address gets a new ID.
class SourceInventoryProjectionV1 {
    NativeGearInventoryBorrowV1 borrow_;
    SourceInventoryProjectionInvalidateV1 invalidate_instance_binding_;
    mutable std::mutex mutex_;
    std::shared_ptr<const dh2::player::PlayerEquipmentRenderOwnerV1> gear_;
    const dh2::data::FreshInventoryOwnedV4* inventory_{};
    std::unordered_map<const dh2::data::ItemInstanceV1*, std::string> live_ids_;
    std::uint64_t gear_epoch_{1};
    bool initialized_{};
    bool valid_{};

    bool project_locked(bool allow_owner_replacement,
                        SourceInventoryProjectionRowsV1&, std::string&);

public:
    SourceInventoryProjectionV1(NativeGearInventoryBorrowV1 borrow,
                                SourceInventoryProjectionInvalidateV1 invalidate)
        : borrow_(std::move(borrow)),
          invalidate_instance_binding_(std::move(invalidate)) {}

    bool bind_initial(SourceInventoryProjectionRowsV1&, std::string& error);

    // Invalidate before any native mutation. Existing row leases/witnesses are
    // not actionable until refresh_after_native_mutation succeeds.
    void invalidate_before_native_mutation() noexcept;

    // Must be called before native deletion/free to prevent address reuse from
    // inheriting the removed item's process-local UI identity.
    bool retire_before_native_item_removal(
        const dh2::data::ItemInstanceV1*) noexcept;

    bool refresh_after_native_mutation(SourceInventoryProjectionRowsV1&,
                                      std::string& error);

    // A GEAR restore retires every previous witness and advances the epoch,
    // even if the same Gear wrapper/control block performs the restore.
    bool rebind_after_gear_restore(SourceInventoryProjectionRowsV1&,
                                   std::string& error);

    bool projection_valid() const noexcept;
    std::uint64_t gear_epoch() const noexcept;
};

// Copies only inventory/equipment into the existing menu CharacterState.
// All other fields (including real stats, class, gold and skills) are kept.
bool apply_source_inventory_projection_v1(
    const SourceInventoryProjectionRowsV1&, CharacterState&,
    std::string& error);

// Applies the two projected vectors to the existing host CharacterState,
// binds the canonical resolver, then verifies every UI ID against the exact
// pointer/index witness in this projection.
bool bind_source_inventory_projection_v1(
    const SourceInventoryProjectionRowsV1&, CharacterState&,
    SourceInventoryInstanceResolver&, std::string& error);

} // namespace dh::foundation::inventory
