#pragma once

#include "source_map_kernel_v1.hpp"
#include "../../../level-loader/fixed_map_v1.hpp"

#include <optional>

namespace dh::foundation::map_ui {

// A source-equivalent RoomZone geometry candidate derived from the actual
// decoded module root. The FixedMap borrow is retained with the record. The
// decoded map has no live Module::visited3fc cell, so visitation stays unknown
// until a same-owner runtime provider resolves it.
struct DecodedRoomZoneV1 {
    std::shared_ptr<void> owner;
    std::uint32_t module_index{};
    std::string module_name;
    Bounds3V1 bounds;
    std::optional<bool> visited;

    bool has_inside(const Point3V1&) const noexcept;
};

// Computes all Module root AABBs from FixedMap's real BRES meshes and cached
// transformed instances. Failed or unsupported input preserves `out`.
bool source_map_decoded_room_zones_v1(
    const dh2::loader::FixedMapV1::Borrow&,
    std::vector<DecodedRoomZoneV1>& out, std::string& error);

// Existing Map.Show collector requires a definite visitation partition. This
// conversion only succeeds when its caller resolves every zone from the same
// live runtime owner; it never substitutes a new-zone false default.
using ReadDecodedRoomZoneVisitedV1 = std::function<bool(
    const DecodedRoomZoneV1&, bool& visited, std::string& error)>;
bool source_map_decoded_room_zone_inputs_v1(
    const std::vector<DecodedRoomZoneV1>&,
    const ReadDecodedRoomZoneVisitedV1&,
    std::vector<RoomZoneV1>& out, std::string& error);

// RoomServicesV1 adapter for the exact inclusive-XY RoomZone::HasInside rule.
RoomServicesV1 source_map_decoded_room_zone_services_v1();

} // namespace dh::foundation::map_ui
