#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace dh2::data {

// Source ItemObject::Interact's AutoTransmute branch. Facts come from the
// retained source Item/ItemRecord and SavedOption; missing facts must not be
// guessed as the normal-pickup path.
struct AutoTransmuteFactsV1 {
    bool ready{};
    std::int32_t item_id{-1};
    std::int32_t item_type{-1};
    std::int32_t base_property{-1};
    std::int32_t power_count{};
    std::int32_t saved_option{};
    std::int32_t item_value{};
    std::int32_t transmute_bonus{};       // Character property 197
    std::uint32_t transmute_multiplier{}; // CharacterDesign constant, fixed point
};

enum class AutoTransmuteProviderResultV1 : std::uint8_t {
    not_applied,
    committed,
    indeterminate
};

struct AutoTransmuteServicesV1 {
    void* context{};
    // Must transfer this exact world Item into the same canonical Player V4
    // inventory and return the actual retained destination Item, including a
    // merge target. not_applied guarantees no mutation; indeterminate is terminal.
    AutoTransmuteProviderResultV1 (*transfer_world_item)(
        void*, std::size_t world_index, std::uintptr_t item_identity,
        std::int32_t item_id, std::int32_t& inventory_index,
        std::uintptr_t& inventory_item_identity,
        std::string& error){};
    // Must consume that exact returned inventory Item, add payout gold, and
    // apply property 213 once through canonical owners. not_applied guarantees no
    // mutation; indeterminate is terminal to prevent a duplicate award.
    AutoTransmuteProviderResultV1 (*consume_for_gold)(
        void*, std::int32_t inventory_index, std::uintptr_t inventory_item_identity,
        std::int32_t item_id, std::int32_t payout,
        std::string& error){};
};

enum class AutoTransmutePhaseV1 : std::uint8_t {
    pending,
    transferred,
    complete,
    terminal_failure
};

// Caller-owned per-world-item continuation state. It contains no Item,
// inventory, property, or timer ownership; the NativeWorldItem projection is
// the natural lifetime for this state if/when Android wires the provider.
struct AutoTransmuteContinuationV1 {
    AutoTransmutePhaseV1 phase{AutoTransmutePhaseV1::pending};
    // Stable identity of the projected world Item; it remains the retry key
    // after the source transfer removes or merges the original allocation.
    std::uintptr_t world_item_identity{};
    // Actual canonical V4 inventory Item returned by AddItemInstance. This can
    // differ from the world identity when transfer merges into a stack.
    std::uintptr_t inventory_item_identity{};
    std::int32_t item_id{-1};
    std::int32_t inventory_index{-1};
    std::int32_t payout{};
};

enum class AutoTransmuteStatusV1 : std::uint8_t {
    normal_pickup,
    completed,
    already_completed,
    awaiting_provider,
    transfer_not_applied,
    consume_not_applied,
    terminal_failure,
    invalid_argument
};

// Exact arithmetic projection of Character::INV_TransmuteItem(item, true),
// including 32-bit wrap and arithmetic right shifts observed in the ARM code.
std::int32_t auto_transmute_value_v1(std::int32_t item_value,
                                     std::int32_t property_197,
                                     std::uint32_t multiplier) noexcept;

// Plans and advances only the AutoTransmute branch. normal_pickup means the
// caller should use its existing ordinary ItemObject::Interact path.
AutoTransmuteStatusV1 auto_transmute_pickup_v1(
    std::size_t world_index, std::uintptr_t stable_world_item_identity,
    std::int32_t live_item_id,
    const AutoTransmuteFactsV1&, const AutoTransmuteServicesV1&,
    AutoTransmuteContinuationV1&, std::string& error) noexcept;

} // namespace dh2::data
