#include "native_loot_pickup_quest_binding_v1.hpp"

namespace dh2::native::loot_pickup_quest_binding_v1 {
namespace {

struct RaiseContext { quests::Owner* quest{}; };

bool raise_current_level(void* raw, std::uintptr_t owner_identity,
                         const character::LootPickupQuestEventV10& source,
                         std::string& error) {
    auto* context = static_cast<RaiseContext*>(raw);
    if (!context || !context->quest ||
        reinterpret_cast<std::uintptr_t>(context->quest) != owner_identity) {
        error = "Pickup RaiseAsync owner differs from the current native Quest";
        return false;
    }
    auto event = source;
    return context->quest->raise_current_level_event(event, error);
}

} // namespace

character::loot_pickup_event_bridge_v1::Status Binding::after_transfer(
    std::uintptr_t character, std::int32_t item_id,
    character::loot_pickup_event_bridge_v1::Result* result,
    std::string& error) const {
    using namespace character::loot_pickup_event_bridge_v1;
    if (!level || !quest || !inventory || !is_player) {
        if (result) *result = {};
        error = "Pickup binding requires the existing Level, Quest, V4 inventory, and IsPlayer provider";
        return Status::invalid;
    }
    RaiseContext raise_context{quest};
    Services services{const_cast<Binding*>(this), is_player,
                      reinterpret_cast<std::uintptr_t>(quest),
                      gather_event_type, raise_current_level};
    // RaiseAsync needs the Quest as context while IsPlayer keeps the renderer
    // caller's context. The small callback context forwards both without
    // retaining or duplicating any game owner.
    struct CombinedContext {
        const Binding* binding;
        RaiseContext* raise;
    } combined{this, &raise_context};
    services.context = &combined;
    services.is_player = [](void* raw, std::uintptr_t id, bool& player,
                            std::string& message) {
        auto* combined = static_cast<CombinedContext*>(raw);
        return combined->binding->is_player(
            combined->binding->is_player_context, id, player, message);
    };
    services.raise_async = [](void* raw, std::uintptr_t owner,
                              const character::LootPickupQuestEventV10& event,
                              std::string& message) {
        auto* combined = static_cast<CombinedContext*>(raw);
        return raise_current_level(combined->raise, owner, event, message);
    };
    return character::loot_pickup_event_bridge_v1::after_transfer(
        *level, *inventory, character, item_id, services, result, error);
}

} // namespace dh2::native::loot_pickup_quest_binding_v1
