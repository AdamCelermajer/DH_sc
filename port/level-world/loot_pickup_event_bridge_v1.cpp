#include "loot_pickup_event_bridge_v1.hpp"

namespace dh2::character::loot_pickup_event_bridge_v1 {

Status after_transfer(const source_level_owner_v1::Owner& level,
                      const data::FreshInventoryOwnedV4& inventory,
                      std::uintptr_t character, std::int32_t item_id,
                      const Services& services, Result* output,
                      std::string& error) {
    Result result{};
    error.clear();
    if (!output || !character || item_id < 0) {
        error = "GatherLoot pickup requires a live Character, Item ID, and result";
        if (output) *output = result;
        return Status::invalid;
    }

    // Preserve the original IsPlayer call before the ItemInventory+0x30 list
    // query. The provider must resolve the live Character, not infer from ID.
    bool player = false;
    if (!services.is_player ||
        !services.is_player(services.context, character, player, error)) {
        if (error.empty()) error = "GatherLoot pickup IsPlayer provider is unavailable";
        result.status = Status::failed;
        *output = result;
        return result.status;
    }
    if (!player) {
        result.status = Status::ignored;
        *output = result;
        return result.status;
    }
    if (inventory.character() != character) {
        error = "GatherLoot pickup IsPlayer Character differs from canonical V4 inventory";
        result.status = Status::failed;
        *output = result;
        return result.status;
    }
    bool registered = false;
    if (!inventory.has_quest_gathering_item_id(item_id, registered, error)) {
        result.status = Status::failed;
        *output = result;
        return result.status;
    }
    result.registered_item = registered;
    if (!registered) {
        result.status = Status::ignored;
        *output = result;
        return result.status;
    }

    // Source Application::GetCurrentLevel returns null when no Level is
    // active; that is a successful no-op. A partially bound or stale active
    // chain is a provider failure and cannot emit into a different Quest owner.
    const auto current = level.snapshot();
    if (current.phase != source_level_owner_v1::Phase::active ||
        !current.renderer_projection || !current.source_gslevel ||
        !current.source_level || !current.source_level_savegame ||
        !current.canonical_player_savegame) {
        result.status = Status::ignored;
        *output = result;
        return result.status;
    }
    result.level_identity = current.source_level;
    result.event_owner_identity = services.event_owner_identity;
    if (current.player_character != character || !current.quest_owner ||
        !services.event_owner_identity ||
        reinterpret_cast<std::uintptr_t>(current.quest_owner) !=
            services.event_owner_identity) {
        error = "GatherLoot pickup Level/Quest binding mismatch: player=" +
            std::to_string(current.player_character) + ", character=" +
            std::to_string(character) + ", quest=" +
            std::to_string(reinterpret_cast<std::uintptr_t>(current.quest_owner)) +
            ", event owner=" + std::to_string(services.event_owner_identity);
        result.status = Status::failed;
        *output = result;
        return result.status;
    }
    if (!services.raise_async) {
        error = "GatherLoot pickup requires the current Quest EventManager RaiseAsync provider";
        result.status = Status::failed;
        *output = result;
        return result.status;
    }

    LootPickupQuestEventV10 event{};
    event.objective_type = services.gather_event_type;
    event.character = character;
    event.item_id = item_id;
    if (!services.raise_async(services.context, services.event_owner_identity,
                              event, error)) {
        if (error.empty()) error = "GatherLoot pickup EventManager RaiseAsync failed";
        result.status = Status::failed;
        *output = result;
        return result.status;
    }
    error.clear();
    result.status = Status::raised;
    *output = result;
    return result.status;
}

} // namespace dh2::character::loot_pickup_event_bridge_v1
