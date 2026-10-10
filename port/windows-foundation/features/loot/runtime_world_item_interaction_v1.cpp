#include "runtime_world_item_interaction_v1.hpp"
#include "../../combat_session.hpp"
#include "../../playable_actor_world.hpp"
#include <algorithm>
#include <cmath>

namespace dh::foundation::loot {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}
bool finite_position(const std::array<float, 3>& position) noexcept {
    return std::all_of(position.begin(), position.end(),
                       [](float value) { return std::isfinite(value); });
}
}

bool RuntimeWorldItemInteractionV1::dispatch(
    dh::foundation::CombatSession& session,
    const RuntimeWorldItemSourceInteractV1& request,
    const RuntimeWorldItemInteractionServicesV1& services,
    RuntimeWorldItemAdapterV1& items,
    RuntimeWorldItemInteractionReceiptV1& receipt,
    std::string& error) const {
    receipt = {};
    error.clear();
    auto* world = session.world();
    if (!world) return fail(error, "Source ItemObject::Interact requires the same live CombatSession world");
    const auto player_id = session.player_id();
    const auto* actor = session.actor(player_id);
    const auto* traits = world->traits(player_id);
    if (!actor || !traits)
        return fail(error, "Current CombatSession player actor/traits are unavailable");
    return dispatch_live_player(player_id, traits->is_player, actor, request,
                                services, items, receipt, error);
}

bool RuntimeWorldItemInteractionV1::dispatch_live_player(
    ActorId current_player, bool is_local_player, const ActorState* player,
    const RuntimeWorldItemSourceInteractV1& request,
    const RuntimeWorldItemInteractionServicesV1& services,
    RuntimeWorldItemAdapterV1& items,
    RuntimeWorldItemInteractionReceiptV1& receipt,
    std::string& error) const {
    receipt = {};
    error.clear();
    if (!is_local_player || !player || current_player == invalid_actor_id ||
        player->id != current_player)
        return fail(error, "Source pickup requires the current local player ActorState");
    if (request.character != current_player)
        return fail(error, "Source Interact character is not the current local player");
    if (!services.resolve_character_state)
        return fail(error, "Same-session CharacterState resolver is unavailable");
    if (!finite_position(player->transform.position))
        return fail(error, "Current player position is nonfinite");

    RuntimeWorldItemEntryV1 drop;
    if (!items.inspect(request.item, drop, error)) return false;
    if (!drop.authored_item)
        return fail(error, "Stored item has no retained original ItemTable row");
    if (!finite_position(drop.source_position))
        return fail(error, "Stored source item position is nonfinite");

    // Potion capacity and equipment/power/auto-transmute rules depend on
    // native owners not represented by the generic CharacterState Presenter.
    // GetPickUpType() reads ItemInstance+0x58 and, when it is -1 (the
    // unmodified table-created instance case represented by this record),
    // returns ItemTable word3. This store has no per-instance override.
    const auto source_pickup_type = drop.authored_item->record.words[3];
    const bool gold_token = drop.authored_item->record.words[22] == 13;
    if (gold_token && !drop.source_outcome.resolved_gold_value)
        return fail(error, "Source gold-token creation value is unavailable");
    if (!services.original_interact_gates_applied) {
        if (source_pickup_type == 13 && !gold_token)
            return fail(error, "Unmapped source pickup type 13 transfer branch");
        if (source_pickup_type == 14)
            return fail(error, "Source potion-capacity/consumption pickup owner is unavailable");
        if (drop.authored_item->record.words[26] != -1)
            return fail(error, "Source equippable pickup requires native power/Gear/transmute owners");
    }

    std::shared_ptr<CharacterState> character;
    try {
        if (!services.resolve_character_state(services.context, current_player,
                                              character, error)) {
            if (error.empty()) error = "Same-session CharacterState resolution failed";
            return false;
        }
    } catch (...) {
        return fail(error, "Same-session CharacterState resolver threw before pickup");
    }
    if (!character) return fail(error, "Current player has no canonical CharacterState owner");

    RuntimeWorldItemPickupReceiptV1 pickup;
    if (!items.pickup(request.item, *character, pickup, error,
                      gold_token ? services.source_gold_limit : std::nullopt)) return false;
    receipt.player = current_player;
    receipt.item = request.item;
    receipt.player_position = player->transform.position;
    receipt.item_position = drop.source_position;
    receipt.pickup = std::move(pickup);
    return true;
}

} // namespace dh::foundation::loot
