#pragma once

#include "source_map_kernel_v1.hpp"
#include "../../../level-world/canonical_room_zone_factory_v3.hpp"

#include <functional>

namespace dh::foundation::map_ui {

// Adapts a completed record produced by the canonical RoomZone factory. The
// alias lease pins the actual record/receiver; it is not an copied bounds or
// visitation snapshot from another owner.
bool source_map_room_zone_borrow_v1(
    const std::shared_ptr<dh2::world::CanonicalRoomZoneRecordV3>& record,
    RoomZoneV1& out, std::string& error);

// Composes the actual RoomZone records read from the current Level's source
// list into the Map.Show kernel input. The callback must freshly walk the same
// published Level's +36 list and resolve each entry through its real
// CanonicalRoomZoneFactoryV3 record owner. This adapter never derives the list
// from PF metadata or ObjectManager membership.
using ReadCurrentLevelRoomZoneRecordsV1 = std::function<bool(
    std::vector<std::shared_ptr<dh2::world::CanonicalRoomZoneRecordV3>>&,
    std::string&)>;
bool source_map_current_level_room_zones_v1(
    const ReadCurrentLevelRoomZoneRecordsV1&,
    std::vector<RoomZoneV1>& out, std::string& error);

// RoomServicesV1 callback for the same typed receiver aliases returned above.
// HasInside uses the retained RoomZone's actual absolute source AABB and keeps
// the source inclusive-XY-only test (the Z coordinate is intentionally ignored).
RoomServicesV1 source_map_room_zone_services_v1();

} // namespace dh::foundation::map_ui
