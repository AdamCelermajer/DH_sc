#pragma once

#include "../game-data/quest_objective_factory_v1.hpp"
#include "../quest-data/quests.h"
#include "current_level_quest_event_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::level_world::open_game_object_objective_v1 {

using Objective = data::quest_objective_factory_v1::Record;

using EventRuntime = current_level_quest_event_v1::Runtime;
using Event = current_level_quest_event_v1::Event;
using Receive = current_level_quest_event_v1::Receiver;

struct Services {
    void* context = nullptr;
    // `complete` must call Objective::SetIsCompleted on this same canonical
    // Quest child (including its actual completion-script provider).
    bool (*complete)(void*, Objective&, std::string&) = nullptr;
    // Adapters bind these to the current Level's one EventManager. Attach
    // preserves event type, receiver identity, priority 0, and callback.
    EventRuntime* events = nullptr;
    // Source Objective_EventReceiver::Unregister uses DelayedDetach.
    bool (*delayed_detach)(void*, std::int32_t event_type,
                           std::uintptr_t receiver, bool& scheduled,
                           std::string&) = nullptr;
};

struct Binding {
    Objective* objective = nullptr;
    std::int32_t event_type = -1;
    std::int32_t target_id = -1;
    std::int32_t required_count = 0;
    Services services{};
    bool registered = false;
    bool detach_pending = false;
};

enum class MarkerOperation : std::uint8_t { install, remove };

// Compile the exact OpenGameObject fields in pydata args[0]/args[2], then
// perform the source SetIsCompleted + virtual Unregister tail when satisfied.
bool compile(Objective&, const dh2_quest_objective&, std::int32_t event_type,
             const Services&, Binding&, std::string& error);

// Objective_EventReceiver::Register is guarded by compiled_8. Repeated source
// registration is allowed; EventManager::Attach reports a duplicate without
// replacing the original receiver.
bool register_objective(Objective&, Binding&, std::string& error);
bool unregister_objective(Objective&, Binding&, std::string& error);

// Handle one event already filtered by the current EventManager's event type.
// Return 0 to preserve the source receiver's non-stopping behavior.
std::int32_t receive(void*, EventRuntime&, Event&, std::string& error);

// Objective::InstallObjectiveMarker/RemoveObjectiveMarker are bx lr in the
// source binary for this template (the inherited base implementations).
bool marker(Objective&, MarkerOperation, std::string& error);

}  // namespace dh2::level_world::open_game_object_objective_v1
