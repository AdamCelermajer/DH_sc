#pragma once

#include <array>
#include <cmath>

namespace dh2::item_drop_motion_v1 {

// ItemObject::Update calls IsAtDestination before GameObject::Update. The
// source predicate uses XY distance squared < 6400; GameObject's physical
// update separately stops and snaps at full XYZ distance squared <= 400.
constexpr float item_stop_distance_squared = 6400.0f;
constexpr float physical_snap_distance_squared = 400.0f;
constexpr float source_speed = 2.5f;

enum class State { moving, item_stopped, physical_snap, invalid };

struct Result {
    State state{State::invalid};
    // Source world units per second. The renderer's coordinates are scaled
    // 100x and its physics bodies are in meters, so these values map directly
    // to Box2D meters per second.
    std::array<float, 2> velocity{};
};

// ItemManager::DeSpawn detaches the physical object from its shell. The native
// projection keeps its last position for a later EGL restore, but must not
// carry a pending destination into the next ItemInstance bound to that shell.
inline void de_spawn_projection(const std::array<float, 3>& current,
                                std::array<float, 3>& destination,
                                bool& active) noexcept {
    destination = current;
    active = false;
}

inline Result gameobject_physical_update(
    const std::array<float, 3>& current,
    const std::array<float, 3>& destination) noexcept {
    for (float value : current) if (!std::isfinite(value)) return {};
    for (float value : destination) if (!std::isfinite(value)) return {};

    const float dx = destination[0] - current[0];
    const float dy = destination[1] - current[1];
    const float dz = destination[2] - current[2];
    const float distance_squared = dx * dx + dy * dy + dz * dz;
    if (!std::isfinite(distance_squared)) return {};
    if (distance_squared <= physical_snap_distance_squared)
        return {State::physical_snap, {0.0f, 0.0f}};
    if (!(distance_squared > 0.0f))
        return {State::physical_snap, {0.0f, 0.0f}};

    const float inverse_length = 1.0f / std::sqrt(distance_squared);
    return {State::moving,
            {source_speed * dx * inverse_length,
             source_speed * dy * inverse_length}};
}

inline Result update(const std::array<float, 3>& current,
                     const std::array<float, 3>& destination) noexcept {
    const float dx = destination[0] - current[0];
    const float dy = destination[1] - current[1];
    const float xy_squared = dx * dx + dy * dy;
    if (!std::isfinite(xy_squared)) return {};
    if (xy_squared < item_stop_distance_squared)
        return {State::item_stopped, {0.0f, 0.0f}};
    // ItemObject::Update runs its 80-unit XY stop first. For ItemObjects this
    // shadows GameObject's 20-unit XYZ snap, but retain the generic base case.
    return gameobject_physical_update(current, destination);
}

} // namespace dh2::item_drop_motion_v1
