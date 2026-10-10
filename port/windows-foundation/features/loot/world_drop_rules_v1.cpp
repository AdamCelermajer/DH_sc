#include "world_drop_rules_v1.hpp"
#include "../inventory/inventory_feature.hpp"
#include <algorithm>
#include <cmath>

namespace dh::foundation::loot {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

const dh2::data::Item* item_row(const RuntimeWorldItemAdapterV1& store, const std::string& definition_id) {
    if (!store.tables()) return nullptr;
    const auto& table = store.tables().items();
    for (std::size_t i = 0; i < table.identifiers.size() && i < table.rows.size(); ++i)
        if (table.identifiers[i] == definition_id) return &table.rows[i];
    return nullptr;
}
}

bool world_item_is_automatic_pickup_v1(const RuntimeWorldItemEntryV1& entry) noexcept {
    return entry.authored_item &&
           entry.authored_item->record.words[item_word_pickup_type_v1] == pickup_type_automatic_v1;
}

RuntimeWorldItemIdV1 select_world_item_target_v1(const RuntimeWorldItemAdapterV1& store,
                                                 const std::array<float, 3>& player) {
    RuntimeWorldItemIdV1 best = invalid_runtime_world_item_v1;
    float best_distance = 0.0f;
    for (const auto& pair : store.entries()) {
        const auto& item = pair.second;
        const float dx = item.source_position[0] - player[0];
        const float dy = item.source_position[1] - player[1];
        if (std::fabs(dx) > world_item_sensor_half_extent_v1 ||
            std::fabs(dy) > world_item_sensor_half_extent_v1) continue;
        const float distance = dx * dx + dy * dy;
        if (best == invalid_runtime_world_item_v1 || distance < best_distance) {
            best = pair.first;
            best_distance = distance;
        }
    }
    return best;
}

std::int32_t potion_capacity_from_property_v1(std::int32_t q8_property) noexcept {
    return q8_property <= 0 ? 0 : q8_property / 256;
}

bool interact_world_item_v1(RuntimeWorldItemAdapterV1& store, RuntimeWorldItemIdV1 item,
                            ActorId player, bool is_local_player, const ActorState* player_state,
                            const RuntimeWorldItemInteractionServicesV1& services,
                            const WorldItemPickupRulesV1& rules, WorldItemPickupReportV1& report) {
    report = {};
    RuntimeWorldItemEntryV1 entry;
    std::string error;
    if (!store.inspect(item, entry, error) || !entry.authored_item) {
        report.outcome = WorldItemPickupOutcomeV1::rejected_looted_or_unknown;
        report.error = error.empty() ? "Unknown or already looted world item" : error;
        return false;
    }
    const auto& row = *entry.authored_item;
    report.item_type = row.record.words[item_word_type_v1];
    if (entry.source_outcome.item_id >= 0 && store.tables())
        report.item_identifier = store.tables().items().identifiers.at(
            static_cast<std::size_t>(entry.source_outcome.item_id));
    if (!is_local_player || !player_state || player_state->id != player) {
        report.outcome = WorldItemPickupOutcomeV1::rejected_not_local_player;
        report.error = "Source pickup requires the current local player";
        return false;
    }
    // Interact step 3: the owner may not collect its own drop while the window runs.
    if (entry.owner_actor == player && entry.owner_actor != invalid_actor_id &&
        entry.owner_protect_ms > 0) {
        report.outcome = WorldItemPickupOutcomeV1::rejected_owner_protection;
        report.error = "Owner protection window";
        return false;
    }
    std::shared_ptr<CharacterState> character;
    if (!services.resolve_character_state ||
        !services.resolve_character_state(services.context, player, character, error) || !character) {
        report.outcome = WorldItemPickupOutcomeV1::failed;
        report.error = error.empty() ? "CharacterState unavailable" : error;
        return false;
    }
    const bool equippable = row.record.words[item_word_slotting_v1] != -1;
    if (equippable) {
        // Dropped items carry no powers in this port, so NumPowers is 0.
        if (0u < rules.auto_transmute_option) {
            report.outcome = WorldItemPickupOutcomeV1::auto_transmute_unavailable;
            report.error = "AutoTransmute option requires the transmute owner (not implemented)";
            return false;
        }
        if (!rules.infinite_inventory && character->inventory.size() >= source_inventory_slot_limit_v1) {
            report.outcome = WorldItemPickupOutcomeV1::inventory_full;
            report.error = "GAMEPLAYMENUS_INVENTORY_FULL";
            return false;
        }
    }
    if (report.item_type == item_type_potion_v1) {
        std::int64_t potions = 0;
        for (const auto& owned : character->inventory) {
            const auto* owned_row = item_row(store, owned.definition_id);
            if (owned_row && owned_row->record.words[item_word_type_v1] == item_type_potion_v1)
                potions += owned.quantity;
        }
        if (potions >= rules.potion_capacity) {
            report.outcome = WorldItemPickupOutcomeV1::potion_capacity;
            report.error = "Potion capacity reached";
            return false;
        }
    }
    auto admitted = services;
    admitted.original_interact_gates_applied = true;
    RuntimeWorldItemInteractionV1 interaction;
    if (!interaction.dispatch_live_player(player, is_local_player, player_state, {player, item},
                                          admitted, store, report.receipt, error)) {
        report.outcome = WorldItemPickupOutcomeV1::failed;
        report.error = error;
        return false;
    }
    report.outcome = WorldItemPickupOutcomeV1::picked_up;
    return true;
}

bool drop_item_to_world(RuntimeWorldItemAdapterV1& store, dh::foundation::CharacterState& character,
                        const std::string& instance_id, std::uint32_t quantity,
                        const std::array<float, 3>& position, ActorId owner,
                        RuntimeWorldItemIdV1& published, std::string& error) {
    published = invalid_runtime_world_item_v1;
    error.clear();
    if (!store.tables()) return fail(error, "Missing same original LootTables/ItemTable snapshot");
    if (quantity == 0 || quantity > 255) return fail(error, "Dropped world item quantity must be 1..255");
    try {
        dh::foundation::CharacterState staged = character;
        dh::foundation::inventory::Presenter inventory(staged, store.tables().items());
        dh::foundation::InventoryItem removed;
        if (!inventory.drop(instance_id, quantity, removed, error)) return false;
        const auto* row = item_row(store, removed.definition_id);
        if (!row) return fail(error, "Dropped item has no original ItemTable row");
        if (row->record.words[item_word_type_v1] == item_type_gold_v1)
            return fail(error, "Gold is not an inventory item and cannot be dropped");
        if (!store.publish_inventory_drop(removed, position, owner, player_drop_protection_ms_v1,
                                          published, error)) return false;
        using std::swap;
        swap(character, staged);
        return true;
    } catch (const std::exception& exception) {
        error = std::string("World drop staging failed: ") + exception.what();
        return false;
    }
}

} // namespace dh::foundation::loot
