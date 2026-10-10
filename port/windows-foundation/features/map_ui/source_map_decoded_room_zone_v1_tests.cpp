#include "source_map_decoded_room_zone_v1.hpp"
#include "../../../level-world/floor_source.hpp"
#include "../../../level-world/selector.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation::map_ui;
using namespace dh2;

namespace {
void check(bool ok, const std::string& message) {
    if (!ok) throw std::runtime_error(message);
}

assets::ZipAssetPackV1 open_pack(const char* path) {
    auto file = std::make_shared<std::ifstream>(path, std::ios::binary | std::ios::ate);
    check(bool(*file), "actual source APK unavailable");
    const auto length = file->tellg();
    check(length >= 0, "actual source APK length unavailable");
    assets::ZipBackingV1 backing;
    backing.owner = file;
    backing.bytes = std::uint64_t(length);
    backing.read = [file](std::uint64_t at, void* dst, std::size_t size, std::string& error) {
        file->clear();
        file->seekg(std::streamoff(at));
        file->read(static_cast<char*>(dst), std::streamsize(size));
        if (!*file) { error = "APK read failed"; return false; }
        return true;
    };
    assets::ZipAssetPackV1 result;
    std::string error;
    check(result.mount(std::move(backing),
        "com.gameloft.android.GAND.GloftD2SS/files/", error), error);
    return result;
}
}

int main(int argc, char** argv) {
    if (argc != 2) return 2;
    try {
        const auto pack = open_pack(argv[1]);
        std::string error;
        loader::FixedSourcesV1 sources;
        check(sources.prepare(pack, "SWAMP", "data/scene/001_swamp.mlx", error), error);
        loader::FixedMapV1 map;
        check(map.prepare(pack, sources.borrow(), error), error);
        const auto decoded_map = map.borrow();
        check(decoded_map.modules().size() == 9, "actual SWAMP module inventory changed");

        auto original = DecodedRoomZoneV1{};
        std::vector<DecodedRoomZoneV1> zones{original};
        check(source_map_decoded_room_zones_v1(decoded_map, zones, error), error);
        check(zones.size() == decoded_map.modules().size(),
              "decoded RoomZone candidates do not preserve every source module occurrence");
        const auto services = source_map_decoded_room_zone_services_v1();
        for (std::size_t i = 0; i < zones.size(); ++i) {
            const auto& zone = zones[i];
            check(zone.owner && zone.module_index == i &&
                  zone.module_name == decoded_map.modules()[i].authored_name,
                  "decoded module/root source identity changed");
            check(!zone.visited.has_value(),
                  "missing live Module visited3fc state was flattened to false or true");
            check(std::isfinite(zone.bounds.min_x) && std::isfinite(zone.bounds.min_y) &&
                  std::isfinite(zone.bounds.min_z) && std::isfinite(zone.bounds.max_x) &&
                  std::isfinite(zone.bounds.max_y) && std::isfinite(zone.bounds.max_z) &&
                  zone.bounds.min_x <= zone.bounds.max_x &&
                  zone.bounds.min_y <= zone.bounds.max_y &&
                  zone.bounds.min_z <= zone.bounds.max_z,
                  "actual decoded transformed-root AABB is invalid");
            Bounds3V1 expected{};
            bool have_mesh = false;
            const auto& module = decoded_map.modules()[i];
            const auto& root = decoded_map.scene().graph.at(module.root);
            check(std::abs(root.world[0] - 1.0f) < 1e-6f &&
                  std::abs(root.world[5] - 1.0f) < 1e-6f &&
                  std::abs(root.world[10] - 1.0f) < 1e-6f &&
                  std::abs(root.world[12] - module.transform.position[0]) < 1e-6f &&
                  std::abs(root.world[13] - module.transform.position[1]) < 1e-6f &&
                  std::abs(root.world[14] - module.transform.position[2]) < 1e-6f,
                  "SWAMP fixture unexpectedly changed source module root orientation/scale");
            for (const auto& placed : decoded_map.instances()) {
                if (placed.module != i) continue;
                const auto& instance = decoded_map.scene().instances.at(placed.scene_instance);
                check(instance.controller < 0,
                      "SWAMP fixture unexpectedly requires unsupported skinned root bounds");
                assets::Mesh mesh{};
                check(dh2_mesh_open(&mesh, &decoded_map.assets().at(placed.asset).view,
                                    static_cast<std::int32_t>(instance.geometry)) == assets::Error::ok,
                      "independent actual module mesh bound read failed");
                octree::Box local{}, world{};
                std::copy_n(mesh.minimum, 3, local.minimum);
                std::copy_n(mesh.maximum, 3, local.maximum);
                selector::Matrix matrix{};
                std::copy(instance.world.begin(), instance.world.end(), matrix.values);
                matrix.identity = 0;
                check(dh2_floor_transform_bounds(&world, &local, &matrix) == 0,
                      "independent actual module mesh AABB transform failed");
                if (!have_mesh) {
                    expected = {world.minimum[0], world.minimum[1], world.minimum[2],
                                world.maximum[0], world.maximum[1], world.maximum[2]};
                    have_mesh = true;
                } else {
                    expected.min_x = std::min(expected.min_x, world.minimum[0]);
                    expected.min_y = std::min(expected.min_y, world.minimum[1]);
                    expected.min_z = std::min(expected.min_z, world.minimum[2]);
                    expected.max_x = std::max(expected.max_x, world.maximum[0]);
                    expected.max_y = std::max(expected.max_y, world.maximum[1]);
                    expected.max_z = std::max(expected.max_z, world.maximum[2]);
                }
            }
            check(have_mesh, "actual SWAMP Module has no transformed-root geometry");
            check(std::abs(zone.bounds.min_x - expected.min_x) < 1e-4f &&
                  std::abs(zone.bounds.min_y - expected.min_y) < 1e-4f &&
                  std::abs(zone.bounds.min_z - expected.min_z) < 1e-4f &&
                  std::abs(zone.bounds.max_x - expected.max_x) < 1e-4f &&
                  std::abs(zone.bounds.max_y - expected.max_y) < 1e-4f &&
                  std::abs(zone.bounds.max_z - expected.max_z) < 1e-4f,
                  "decoded RoomZone root bounds differ from independent source mesh union");
            bool inside = false;
            check(services.has_inside(zone.owner,
                {zone.bounds.min_x, zone.bounds.max_y, zone.bounds.max_z},
                inside, error), error);
            check(inside, "source HasInside rejected an inclusive XY boundary point");
            check(services.has_inside(zone.owner,
                {zone.bounds.max_x + 0.01f, zone.bounds.min_y, zone.bounds.min_z},
                inside, error), error);
            check(!inside, "source HasInside admitted a point beyond the XY AABB");
            check(services.has_inside(zone.owner,
                {(zone.bounds.min_x + zone.bounds.max_x) * 0.5f,
                 (zone.bounds.min_y + zone.bounds.max_y) * 0.5f,
                 zone.bounds.max_z + 10000.f}, inside, error), error);
            check(inside, "RoomZone::HasInside incorrectly tested Z");
        }

        auto marker = std::make_shared<int>(71);
        std::vector<RoomZoneV1> map_inputs{{marker, true}};
        check(!source_map_decoded_room_zone_inputs_v1(zones, {}, map_inputs, error),
              "unknown visitation was accepted by the existing Map.Show input adapter");
        check(map_inputs.size() == 1 && map_inputs[0].owner == marker && map_inputs[0].visited,
              "unknown visitation changed existing map inputs");

        const ReadDecodedRoomZoneVisitedV1 explicit_same_owner_state =
            [](const DecodedRoomZoneV1& zone, bool& visited, std::string& e) {
                check(bool(zone.owner), "visitation callback lost decoded map owner");
                visited = zone.module_index % 2 == 0;
                e.clear();
                return true;
            };
        check(source_map_decoded_room_zone_inputs_v1(
            zones, explicit_same_owner_state, map_inputs, error), error);
        check(map_inputs.size() == zones.size(), "resolved same-owner visits were not composed");
        for (std::size_t i = 0; i < map_inputs.size(); ++i)
            check(map_inputs[i].owner == zones[i].owner &&
                  map_inputs[i].visited == (i % 2 == 0),
                  "same-owner visitation was not preserved in source order");

        map_inputs = {{marker, false}};
        const ReadDecodedRoomZoneVisitedV1 failed_state =
            [](const DecodedRoomZoneV1& zone, bool& visited, std::string& e) {
                if (zone.module_index == 4) { e = "same-session Module state unavailable"; return false; }
                visited = true;
                return true;
            };
        check(!source_map_decoded_room_zone_inputs_v1(zones, failed_state, map_inputs, error),
              "incomplete same-owner visitation was accepted");
        check(map_inputs.size() == 1 && map_inputs[0].owner == marker &&
              !map_inputs[0].visited,
              "partial same-owner visitation changed existing Map.Show input");

        std::vector<DecodedRoomZoneV1> preserved{original};
        check(!source_map_decoded_room_zones_v1({}, preserved, error),
              "empty FixedMap borrow was accepted");
        check(preserved.size() == 1 && preserved[0].owner == original.owner,
              "incomplete FixedMap borrow changed existing decoded zones");
        std::cout << "PASS actual decoded SWAMP transformed-root RoomZone subset, exact XY containment, unknown visitation and transactional publication\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
        return 1;
    }
}
