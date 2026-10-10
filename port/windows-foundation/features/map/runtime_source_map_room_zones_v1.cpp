#include "runtime_source_map_room_zones_v1.hpp"

#include "runtime_source_map_actor_markers_v1.hpp"

#include <cmath>

namespace dh::foundation::map_page {
namespace {
bool same_owner(const std::shared_ptr<const void>& a,
                const std::shared_ptr<const void>& b) {
    return a && b && a.get() == b.get() &&
           !a.owner_before(b) && !b.owner_before(a);
}
}

bool source_map_bind_session_to_decoded_room_zones_v1(
    const std::vector<map_ui::DecodedRoomZoneV1>& decoded,
    const CombatSession& session,
    std::vector<SessionRoomZoneV1>& out,
    std::string& error) {
    SourceMarkerV1 player;
    if (!source_map_current_player_marker_v1(session, player, error)) return false;
    const auto binding = session.actor_binding_lease().lock();
    const auto* world = session.world();
    const auto* actor = session.actor(player.source_id);
    const auto serial = session.update_serial();
    if (!binding || !world || !actor) {
        error = "Map RoomZone join: current Session player binding expired";
        return false;
    }
    auto retained_binding = std::const_pointer_cast<void>(binding);

    std::vector<SessionRoomZoneV1> next;
    next.reserve(decoded.size());
    for (const auto& zone : decoded) {
        if (!zone.owner || zone.visited.has_value()) {
            error = "Map RoomZone join: decoded root owner missing or carries unsupported visitation";
            return false;
        }
        SessionRoomZoneV1 joined;
        joined.decoded = zone;
        joined.player_id = player.source_id;
        joined.player_position = player.world_position;
        joined.player_inside = player.world_position.x >= zone.bounds.min_x &&
            player.world_position.x <= zone.bounds.max_x &&
            player.world_position.y >= zone.bounds.min_y &&
            player.world_position.y <= zone.bounds.max_y;
        joined.actor_binding_owner = retained_binding;
        joined.visited = std::nullopt;
        next.push_back(std::move(joined));
    }

    const auto* after_actor = session.actor(player.source_id);
    const auto after_binding = session.actor_binding_lease().lock();
    if (session.player_id() != player.source_id || session.world() != world ||
        after_actor != actor || session.update_serial() != serial ||
        !same_owner(binding, after_binding)) {
        error = "Map RoomZone join: Session changed while composing current player containment";
        return false;
    }
    out = std::move(next);
    error.clear();
    return true;
}

bool source_map_current_session_room_zones_v1(
    const dh2::loader::FixedMapV1::Borrow& map,
    const CombatSession& session,
    std::vector<SessionRoomZoneV1>& out,
    std::string& error) {
    std::vector<map_ui::DecodedRoomZoneV1> decoded;
    if (!map_ui::source_map_decoded_room_zones_v1(map, decoded, error)) return false;
    return source_map_bind_session_to_decoded_room_zones_v1(
        decoded, session, out, error);
}

} // namespace dh::foundation::map_page
