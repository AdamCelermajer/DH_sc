#include "source_map_room_zone_v1.hpp"

#include <iostream>
#include <stdexcept>

using namespace dh::foundation::map_ui;

namespace {
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
}

int main() {
    try {
        std::string error;
        std::vector<RoomZoneV1> output(1);
        auto original_owner = std::make_shared<int>(17);
        output[0].owner = original_owner;

        check(!source_map_current_level_room_zones_v1({}, output, error),
              "missing current-Level list provider was accepted");
        check(output.size() == 1 && output[0].owner == original_owner,
              "missing provider changed the caller's existing map rooms");

        const ReadCurrentLevelRoomZoneRecordsV1 failed_read =
            [](auto&, std::string& e) { e = "current Level changed"; return false; };
        check(!source_map_current_level_room_zones_v1(failed_read, output, error),
              "failed current-Level list read was accepted");
        check(output.size() == 1 && output[0].owner == original_owner,
              "failed list read published a partial room list");

        const ReadCurrentLevelRoomZoneRecordsV1 empty_level =
            [](auto& records, std::string& e) { records.clear(); e.clear(); return true; };
        check(source_map_current_level_room_zones_v1(empty_level, output, error), error.c_str());
        check(output.empty(), "actual empty current-Level room list was not preserved");

        auto record = std::make_shared<dh2::world::CanonicalRoomZoneRecordV3>();
        auto zone_world = std::make_shared<int>(23);
        record->receiver = std::make_unique<dh2::world::CanonicalRoomZoneV3>(
            zone_world, record->runtime, dh2::world::RoomZoneServicesV3{});
        record->constructor_completed_v91 = true;
        std::weak_ptr<dh2::world::CanonicalRoomZoneRecordV3> weak_record = record;
        ReadCurrentLevelRoomZoneRecordsV1 one_actual_record =
            [record](auto& records, std::string& e) {
                records.push_back(record);
                e.clear();
                return true;
            };
        check(source_map_current_level_room_zones_v1(one_actual_record, output, error), error.c_str());
        check(output.size() == 1 && output[0].owner && output[0].visited,
              "completed moduleless canonical RoomZone did not produce its source visited state");
        one_actual_record = {};
        record.reset();
        check(!weak_record.expired(), "projected RoomZone did not retain its canonical factory record");
        output.clear();
        check(weak_record.expired(), "canonical factory record lease outlived its map RoomZone output");

        output.push_back({original_owner, true});
        const ReadCurrentLevelRoomZoneRecordsV1 incomplete_record =
            [](auto& records, std::string& e) {
                records.emplace_back();
                e.clear();
                return true;
            };
        check(!source_map_current_level_room_zones_v1(incomplete_record, output, error),
              "uncompleted RoomZone factory record was accepted");
        check(output.size() == 1 && output[0].owner == original_owner,
              "incomplete record changed the caller's existing map rooms");

        std::cout << "PASS current-Level RoomZone list composition, empty list, and fail-closed publication\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
