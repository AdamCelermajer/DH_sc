#pragma once

// P16 CONTAINERS2 (T5): persistence of authored container state through the
// existing world objects. Each declaration is bound as the neutral WorldObject
// with its authored stable ID (the source GameObject), so the GameSave object
// roster (save version 2, already in place) carries its feature-owned component
// bytes. The open/broken state is the existing OBJS v1 component
// (dh2.source-container.objs.v1, 7 bytes, state394). No save schema change.
//
// Destructible remaining hits are not part of OBJS. They use a separate
// feature-owned component (dh2.container.hits.v1, u32 little-endian), keyed by
// the same object, so an old save without it loads with the full stage count.

#include "container_runtime_v1.hpp"
#include "../../playable_actor_world.hpp"

#include <string>
#include <vector>

namespace dh::foundation::containers {

inline constexpr const char* container_hits_component_v1 = "dh2.container.hits.v1";

// Binds one neutral WorldObject per loaded declaration and writes its initial OBJS
// (visible, enabled, idle). Declarations whose ID is already bound are noticed and
// skipped (they still interact, but do not persist).
bool bind_container_world_objects_v1(PlayableActorWorld& world, const ContainerRuntimeV1& runtime,
                                     std::vector<std::string>& notices, std::string& error);

// Writes the state of every declaration changed since the last call into its object.
bool persist_container_world_state_v1(PlayableActorWorld& world, ContainerRuntimeV1& runtime,
                                      std::string& error);

// Applies the persisted state of every declaration (after a GameSave load). A
// declaration without its component is idle with the full stage count.
bool restore_container_world_state_v1(const PlayableActorWorld& world, ContainerRuntimeV1& runtime,
                                      std::string& error);

} // namespace dh::foundation::containers
