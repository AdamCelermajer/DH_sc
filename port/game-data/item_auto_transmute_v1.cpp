#include "item_auto_transmute_v1.hpp"

#include <limits>

namespace dh2::data {
namespace {
std::int32_t signed_word(std::uint32_t value) noexcept {
    if (value <= std::uint32_t(std::numeric_limits<std::int32_t>::max()))
        return static_cast<std::int32_t>(value);
    return -1 - static_cast<std::int32_t>(~value);
}

std::int32_t arithmetic_shift_right(std::int32_t value, unsigned shift) noexcept {
    if (!shift) return value;
    const auto bits = static_cast<std::uint32_t>(value);
    const auto shifted = bits >> shift;
    const auto sign = value < 0 ? (~std::uint32_t{0} << (32u - shift)) : 0u;
    return signed_word(shifted | sign);
}

std::int32_t multiply_word(std::int32_t left, std::int32_t right) noexcept {
    return signed_word(static_cast<std::uint32_t>(left) *
                       static_cast<std::uint32_t>(right));
}

bool same_item(const AutoTransmuteContinuationV1& continuation,
               std::uintptr_t identity, std::int32_t item_id) noexcept {
    return continuation.world_item_identity == identity &&
           continuation.item_id == item_id;
}
} // namespace

std::int32_t auto_transmute_value_v1(std::int32_t item_value,
                                     std::int32_t property_197,
                                     std::uint32_t multiplier) noexcept {
    const auto fixed_value = signed_word(static_cast<std::uint32_t>(item_value) << 8);
    const auto bonus = signed_word(static_cast<std::uint32_t>(property_197) + 256u);
    const auto scaled = arithmetic_shift_right(multiply_word(fixed_value, bonus), 8);
    const auto result = arithmetic_shift_right(
        multiply_word(signed_word(multiplier), scaled), 16);
    return result < 1 ? 1 : result;
}

AutoTransmuteStatusV1 auto_transmute_pickup_v1(
    std::size_t world_index, std::uintptr_t stable_world_item_identity,
    std::int32_t live_item_id,
    const AutoTransmuteFactsV1& facts, const AutoTransmuteServicesV1& services,
    AutoTransmuteContinuationV1& continuation, std::string& error) noexcept {
    try {
        if (!stable_world_item_identity || live_item_id < 0) {
            error = "AutoTransmute requires a stable live Item identity";
            return AutoTransmuteStatusV1::invalid_argument;
        }

        if (continuation.phase != AutoTransmutePhaseV1::pending) {
            if (!same_item(continuation, stable_world_item_identity, live_item_id)) {
                error = "AutoTransmute continuation belongs to another Item";
                return AutoTransmuteStatusV1::invalid_argument;
            }
            if (continuation.phase == AutoTransmutePhaseV1::complete) {
                error.clear();
                return AutoTransmuteStatusV1::already_completed;
            }
            if (continuation.phase == AutoTransmutePhaseV1::terminal_failure) {
                error = "AutoTransmute stopped after an indeterminate provider result";
                return AutoTransmuteStatusV1::terminal_failure;
            }
        } else {
            if (!facts.ready || facts.item_id < 0 || facts.item_id != live_item_id || facts.item_type < 0 ||
                facts.power_count < 0 || facts.saved_option < 0) {
                error = "AutoTransmute source Item/SavedOption facts are unavailable or invalid";
                return AutoTransmuteStatusV1::invalid_argument;
            }
            if (facts.item_type == 14 || facts.base_property == -1 ||
                facts.power_count >= facts.saved_option) {
                error.clear();
                return AutoTransmuteStatusV1::normal_pickup;
            }
            continuation.world_item_identity = stable_world_item_identity;
            continuation.inventory_item_identity = 0;
            continuation.item_id = facts.item_id;
            continuation.inventory_index = -1;
            continuation.payout = auto_transmute_value_v1(
                facts.item_value, facts.transmute_bonus,
                facts.transmute_multiplier);
        }

        if (continuation.phase == AutoTransmutePhaseV1::pending) {
            if (!services.transfer_world_item) {
                error = "AutoTransmute requires the canonical V4 world-to-inventory transfer provider";
                return AutoTransmuteStatusV1::awaiting_provider;
            }
            std::int32_t inventory_index = -1;
            std::uintptr_t inventory_item_identity = 0;
            AutoTransmuteProviderResultV1 transfer_result;
            try {
                transfer_result = services.transfer_world_item(
                    services.context, world_index, stable_world_item_identity,
                    continuation.item_id, inventory_index,
                    inventory_item_identity, error);
            } catch (...) {
                continuation.phase = AutoTransmutePhaseV1::terminal_failure;
                error = "AutoTransmute transfer provider threw; side effects are indeterminate";
                return AutoTransmuteStatusV1::terminal_failure;
            }
            if (transfer_result == AutoTransmuteProviderResultV1::not_applied) {
                if (error.empty()) error = "AutoTransmute transfer provider did not commit";
                continuation = {};
                return AutoTransmuteStatusV1::transfer_not_applied;
            }
            if (transfer_result != AutoTransmuteProviderResultV1::committed ||
                inventory_index < 0 || !inventory_item_identity) {
                continuation.phase = AutoTransmutePhaseV1::terminal_failure;
                if (error.empty()) error = "AutoTransmute transfer result is indeterminate";
                return AutoTransmuteStatusV1::terminal_failure;
            }
            continuation.inventory_index = inventory_index;
            continuation.inventory_item_identity = inventory_item_identity;
            continuation.phase = AutoTransmutePhaseV1::transferred;
        }

        if (!services.consume_for_gold) {
            error = "AutoTransmute requires the canonical V4 consume/gold/property-213 provider";
            return AutoTransmuteStatusV1::awaiting_provider;
        }
        AutoTransmuteProviderResultV1 consume_result;
        try {
            consume_result = services.consume_for_gold(
                services.context, continuation.inventory_index,
                continuation.inventory_item_identity, continuation.item_id,
                continuation.payout, error);
        } catch (...) {
            continuation.phase = AutoTransmutePhaseV1::terminal_failure;
            error = "AutoTransmute consume provider threw; gold/property side effects are indeterminate";
            return AutoTransmuteStatusV1::terminal_failure;
        }
        if (consume_result == AutoTransmuteProviderResultV1::not_applied) {
            if (error.empty()) error = "AutoTransmute consume provider did not commit";
            return AutoTransmuteStatusV1::consume_not_applied;
        }
        if (consume_result == AutoTransmuteProviderResultV1::indeterminate) {
            continuation.phase = AutoTransmutePhaseV1::terminal_failure;
            if (error.empty()) error = "AutoTransmute consume result is indeterminate";
            return AutoTransmuteStatusV1::terminal_failure;
        }
        if (consume_result != AutoTransmuteProviderResultV1::committed) {
            error = "AutoTransmute consume provider returned an unknown result";
            continuation.phase = AutoTransmutePhaseV1::terminal_failure;
            return AutoTransmuteStatusV1::terminal_failure;
        }
        continuation.phase = AutoTransmutePhaseV1::complete;
        error.clear();
        return AutoTransmuteStatusV1::completed;
    } catch (...) {
        error = "AutoTransmute kernel failed unexpectedly";
        return AutoTransmuteStatusV1::terminal_failure;
    }
}

} // namespace dh2::data
