#pragma once

#include "runtime_world_item_adapter_v1.hpp"
#include <memory>
#include <optional>

namespace dh::foundation { class CombatSession; }

namespace dh::foundation::loot {

// The original ItemObject::Interact receiver is explicit. Collision sensor
// contact only establishes tooltip/current-target state; it is not pickup.
// This request must be emitted by the source interaction path after that
// path's ownership, local-player, pickup-type and branch checks have admitted
// transfer. It deliberately carries an exact item identity, never a radius or
// nearest-item query.
struct RuntimeWorldItemSourceInteractV1 {
    ActorId character{invalid_actor_id};
    RuntimeWorldItemIdV1 item{invalid_runtime_world_item_v1};
};

struct RuntimeWorldItemInteractionServicesV1 {
    void* context{};
    bool (*resolve_character_state)(void*, ActorId,
                                    std::shared_ptr<CharacterState>&,
                                    std::string&){};
    // Optional same-owner runtime/save override. When absent, pickup uses the
    // original ItemInventory constructor's INT32_MAX baseline.
    std::optional<std::int32_t> source_gold_limit;
    // P14: set by interact_world_item_v1 (world_drop_rules_v1) after it applied
    // the original Interact gates (owner window, AutoTransmute, inventory full,
    // potion capacity). Only then may gold, potions and equippable rows pass;
    // the default keeps the strict legacy refusals for existing callers/tests.
    bool original_interact_gates_applied{false};
};

struct RuntimeWorldItemInteractionReceiptV1 {
    ActorId player{invalid_actor_id};
    RuntimeWorldItemIdV1 item{invalid_runtime_world_item_v1};
    std::array<float, 3> player_position{};
    std::array<float, 3> item_position{};
    RuntimeWorldItemPickupReceiptV1 pickup;
};

// Bridges a caller-confirmed source ItemObject::Interact transfer branch to
// the same gameplay session's world-item store and CharacterState owner.
class RuntimeWorldItemInteractionV1 {
public:
    bool dispatch(dh::foundation::CombatSession&,
                  const RuntimeWorldItemSourceInteractV1&,
                  const RuntimeWorldItemInteractionServicesV1&,
                  RuntimeWorldItemAdapterV1&,
                  RuntimeWorldItemInteractionReceiptV1&,
                  std::string& error) const;

    // Shared strict core used by the CombatSession entry and focused tests.
    // `is_local_player` and `player` must be fresh values from that same live
    // PlayableActorWorld; the test uses the original authored Loot/Item rows.
    bool dispatch_live_player(ActorId current_player, bool is_local_player,
                              const ActorState* player,
                              const RuntimeWorldItemSourceInteractV1&,
                              const RuntimeWorldItemInteractionServicesV1&,
                              RuntimeWorldItemAdapterV1&,
                              RuntimeWorldItemInteractionReceiptV1&,
                              std::string& error) const;
};

} // namespace dh::foundation::loot
