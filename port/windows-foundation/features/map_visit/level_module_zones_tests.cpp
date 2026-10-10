// RoomZone sources of real levels: module zones from the level manifest and its module geometry.
// Usage: level_module_zones_tests <assets root with data/scene/001_swamp.mlx> [<assets root with data/scene/003_darkwood.mlx>]
// The Swamp (act 1) is the proving ground; Gothicus Darkwood (act 2 map reference) checks that the same
// loader gives zones for a second level without any level-specific code.
#include "../../assembled_level.hpp"
#include "../../asset_catalog.hpp"
#include "map_page_v1.hpp"
#include "room_zone_visit_v1.hpp"

#include <cmath>
#include <iostream>
#include <set>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::map_visit;

namespace {
void require(bool yes, const std::string& message) {
    if (!yes) throw std::runtime_error(message);
}

// Loads one level and checks the invariants every level must satisfy.
std::vector<LevelModuleZone> load_checked(const std::filesystem::path& root, const std::string& level, std::size_t expectedZones) {
    AssetCatalog assets(root);
    OriginalScene scene;
    std::vector<LevelModuleZone> zones;
    std::string error;
    require(load_level_with_module_zones(assets, level, scene, zones, error), level + ": " + error);
    require(zones.size() == expectedZones, level + ": zone count " + std::to_string(zones.size()) + " != " + std::to_string(expectedZones));
    std::set<std::uint32_t> ids;
    std::size_t rangeTotal = 0;
    for (const auto& zone : zones) {
        require(ids.insert(zone.id).second, level + ": duplicate zone id");
        require(zone.rangeCount > 0, level + ": module without draw ranges " + zone.name);
        require(zone.firstRange == rangeTotal, level + ": module ranges are not contiguous in manifest order");
        rangeTotal += zone.rangeCount;
        for (int axis = 0; axis < 3; ++axis)
            require(std::isfinite(zone.bounds[axis]) && std::isfinite(zone.bounds[axis + 3]) && zone.bounds[axis] <= zone.bounds[axis + 3],
                    level + ": invalid bounds for " + zone.name);
    }
    require(rangeTotal == scene.mesh.ranges.size(), level + ": zone ranges do not cover the scene draw ranges");

    // Visiting every zone through the tracker (player inside each box, camera looking at the whole level) marks all of them.
    RoomZoneVisitTrackerV1 tracker;
    require(tracker.configure(zones, {}, error), error);
    const auto extent = map_extent_v1(zones);
    require(extent.valid, level + ": no extent");
    Camera overview;
    require(map_camera_v1(extent, std::nullopt, MapViewV1{}, overview, error), error);
    CameraBasisV1 basis;
    require(camera_basis_v1(overview, 1.5f, basis, error), error);
    const auto planes = frustum_planes_v1(basis);
    std::size_t inView = 0;
    for (const auto& zone : zones) {
        const float x = (zone.bounds[0] + zone.bounds[3]) * 0.5f, y = (zone.bounds[1] + zone.bounds[4]) * 0.5f;
        const auto found = tracker.update(planes, std::array<float, 3>{x, y, zone.bounds[2]});
        if (tracker.in_view(zone.id)) ++inView;
        // Zones are visited only when the overview frustum sees them; the reset view must see the whole level.
        require(!tracker.in_view(zone.id) || tracker.visited(zone.id), level + ": in-view zone not visited by its own centre");
        (void)found;
    }
    require(inView == zones.size(), level + ": reset map view does not contain every module zone (" + std::to_string(inView) + "/" + std::to_string(zones.size()) + ")");
    std::cout << level << ": zones=" << zones.size() << " ranges=" << rangeTotal << " in reset view=" << inView << '\n';
    return zones;
}
} // namespace

int main(int argc, char** argv) {
    try {
        if (argc < 2) throw std::runtime_error("usage: level_module_zones_tests <swamp assets root> [<darkwood assets root>]");
        const auto swamp = load_checked(argv[1], "data/scene/001_swamp.mlx", 9);
        require(swamp.front().id == 0 && swamp.back().id == 8, "Swamp zone ids are not the manifest placement indices");
        if (argc >= 3) {
            // Gothicus Darkwood: 24 placements of one module BDAE (act 2 reference map level).
            load_checked(argv[2], "data/scene/003_darkwood.mlx", 24);
        }
        std::cout << "level_module_zones tests passed\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "level_module_zones: " << ex.what() << '\n';
        return 1;
    }
}
