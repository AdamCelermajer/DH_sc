#pragma once

#include "../../combat_session.hpp"
#include "../map_ui/source_map_decoded_room_zone_v1.hpp"

#include <optional>

namespace dh::foundation::map_page {

// A decoded source Module root paired with a fresh position read from the
// same current CombatSession. `player_inside` is the exact RoomZone inclusive
// XY containment result; it is not a visitation result.
struct SessionRoomZoneV1 {
    map_ui::DecodedRoomZoneV1 decoded;
    ActorId player_id{};
    map_ui::Point3V1 player_position;
    bool player_inside{};
    std::shared_ptr<void> actor_binding_owner;
    std::optional<bool> visited;
};

// Join actual decoded FixedMap module-root records to the current Session's
// local player transform. Module order and decoded bounds are preserved.
// Source RoomZone visitation remains unknown because FixedMap and
// CombatSession do not publish the live Module::visited3fc association.
bool source_map_current_session_room_zones_v1(
    const dh2::loader::FixedMapV1::Borrow& map,
    const CombatSession& session,
    std::vector<SessionRoomZoneV1>& out,
    std::string& error);

// Lower-level composition entry for callers that already retain decoded
// module records. This lets the production caller share a single FixedMap
// decode with its other same-frame map providers.
bool source_map_bind_session_to_decoded_room_zones_v1(
    const std::vector<map_ui::DecodedRoomZoneV1>& decoded,
    const CombatSession& session,
    std::vector<SessionRoomZoneV1>& out,
    std::string& error);

} // namespace dh::foundation::map_page
