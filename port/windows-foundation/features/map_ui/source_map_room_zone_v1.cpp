#include "source_map_room_zone_v1.hpp"

namespace dh::foundation::map_ui {
namespace {
using Zone = dh2::world::CanonicalRoomZoneV3;
struct SourceRoomZoneLeaseV1 {
    std::shared_ptr<Zone> receiver;
};

bool room_receiver(const std::shared_ptr<void>& lease,
                   std::shared_ptr<Zone>& out, std::string& error) {
    if (!lease) {
        error = "Map RoomZone source: required actual retained receiver lease";
        return false;
    }
    // The private wrapper pins the receiver alias from its canonical factory
    // record and keeps this callback from interpreting a foreign owner as a
    // RoomZone receiver.
    auto typed = std::static_pointer_cast<SourceRoomZoneLeaseV1>(lease);
    out = typed ? typed->receiver : nullptr;
    if (!out) {
        error = "Map RoomZone source: required actual CanonicalRoomZoneV3 receiver";
        return false;
    }
    return true;
}
}

bool source_map_room_zone_borrow_v1(
    const std::shared_ptr<dh2::world::CanonicalRoomZoneRecordV3>& record,
    RoomZoneV1& out, std::string& error) {
    if (!record || !record->constructor_completed_v91 || !record->receiver) {
        error = "Map RoomZone source: required completed canonical factory record";
        return false;
    }
    auto actual = std::shared_ptr<Zone>(record, record->receiver.get());
    bool visited{};
    if (!actual->source_has_been_visited_v104(visited, error)) return false;
    RoomZoneV1 next;
    next.owner = std::make_shared<SourceRoomZoneLeaseV1>(SourceRoomZoneLeaseV1{std::move(actual)});
    next.visited = visited;
    out = std::move(next);
    error.clear();
    return true;
}

bool source_map_current_level_room_zones_v1(
    const ReadCurrentLevelRoomZoneRecordsV1& read_current_level,
    std::vector<RoomZoneV1>& out, std::string& error) {
    if (!read_current_level) {
        error = "Map RoomZone source: required current-Level +36 list provider";
        return false;
    }

    std::vector<std::shared_ptr<dh2::world::CanonicalRoomZoneRecordV3>> records;
    if (!read_current_level(records, error)) {
        if (error.empty())
            error = "Map RoomZone source: current-Level +36 list read failed";
        return false;
    }

    std::vector<RoomZoneV1> next;
    next.reserve(records.size());
    for (const auto& record : records) {
        RoomZoneV1 zone;
        if (!source_map_room_zone_borrow_v1(record, zone, error)) return false;
        next.push_back(std::move(zone));
    }
    out = std::move(next);
    error.clear();
    return true;
}

RoomServicesV1 source_map_room_zone_services_v1() {
    RoomServicesV1 services;
    services.has_inside = [](const std::shared_ptr<void>& lease,
                             const Point3V1& point, bool& inside,
                             std::string& error) {
        std::shared_ptr<Zone> actual;
        if (!room_receiver(lease, actual, error)) return false;
        inside = actual->source_has_inside_v104({point.x, point.y, point.z});
        error.clear();
        return true;
    };
    return services;
}

} // namespace dh::foundation::map_ui
