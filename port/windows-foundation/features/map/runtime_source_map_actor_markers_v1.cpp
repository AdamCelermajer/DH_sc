#include "runtime_source_map_actor_markers_v1.hpp"

#include <cmath>

namespace dh::foundation::map_page {
namespace {
bool same_owner(const std::shared_ptr<const void>& a,
                const std::shared_ptr<const void>& b) {
    return a && b && a.get() == b.get() &&
           !a.owner_before(b) && !b.owner_before(a);
}

bool finite(const std::array<float, 3>& p) {
    return std::isfinite(p[0]) && std::isfinite(p[1]) && std::isfinite(p[2]);
}
}

bool source_map_current_player_marker_v1(
    const CombatSession& session, SourceMarkerV1& output, std::string& error) {
    const auto player = session.player_id();
    const auto* world = session.world();
    const auto* actor = player ? session.actor(player) : nullptr;
    const auto* traits = world && player ? world->traits(player) : nullptr;
    const auto binding = session.actor_binding_lease().lock();
    if (!player || !world || !actor || world->find_actor(player) != actor ||
        !traits || !traits->is_player || !binding) {
        error = "Map family3 requires the current local player actor in the same CombatSession";
        return false;
    }

    const auto serial = session.update_serial();
    const auto position = actor->transform.position;
    if (!finite(position)) {
        error = "Map family3 local-player transform is nonfinite";
        return false;
    }

    // The session exposes this token as const because it is a lifetime witness,
    // not mutable source data. Erase const only on the opaque shared owner type.
    std::shared_ptr<void> pin = std::const_pointer_cast<void>(binding);
    const auto* after = session.actor(player);
    const auto after_binding = session.actor_binding_lease().lock();
    if (session.player_id() != player || session.world() != world ||
        after != actor || session.update_serial() != serial ||
        !same_owner(binding, after_binding)) {
        error = "Map family3 session/actor lease changed while reading current position";
        return false;
    }

    SourceMarkerV1 next;
    next.owner = std::move(pin);
    next.source_id = player;
    next.family = 3; // Original ShowPlayersIcons local-player vector.
    next.world_position = {position[0], position[1], position[2]};
    output = std::move(next);
    error.clear();
    return true;
}

bool source_map_append_current_player_marker_v1(
    const CombatSession& session, std::vector<SourceMarkerV1>& markers,
    std::string& error) {
    SourceMarkerV1 player;
    if (!source_map_current_player_marker_v1(session, player, error)) return false;

    // A source producer may already have emitted the exact local player. Do
    // not silently create a duplicate marker or choose between conflicting
    // coordinates; make the composition conflict visible and leave the vector
    // unchanged.
    for (const auto& marker : markers) {
        if (marker.family == player.family && marker.source_id == player.source_id) {
            error = "Map family3 local player was already emitted by another source producer";
            return false;
        }
    }
    markers.push_back(std::move(player));
    error.clear();
    return true;
}

} // namespace dh::foundation::map_page
