#pragma once

// Preview 17 DOORS: persistence of authored Door state through the neutral world objects (the same
// binding the containers use). Each Door is a WorldObject with its authored stable ID; component
// "dh2.world-gate.door.v1" (1 byte, DoorStateV1) is written when the state changes. The GameSave object roster
// carries it; no schema change. An object without the component keeps the authored start state.

#include "door_runtime_v1.hpp"
#include "../../playable_actor_world.hpp"

#include <string>
#include <vector>

namespace dh::foundation::doors {

// Binds one object per door (objects already bound by another owner are reused, not replaced).
bool bind_door_world_objects_v1(PlayableActorWorld& world, const DoorRuntimeV1& runtime,
                                std::vector<std::string>& notices, std::string& error);

// Writes the state of every door changed since the last call.
bool persist_door_world_state_v1(PlayableActorWorld& world, DoorRuntimeV1& runtime, std::string& error);

// Applies the persisted state of every door (after a GameSave load). No sound, no replay.
bool restore_door_world_state_v1(PlayableActorWorld& world, DoorRuntimeV1& runtime, std::string& error);

} // namespace dh::foundation::doors
