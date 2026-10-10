#include "source_map_decoded_room_zone_v1.hpp"

#include "../../../asset-payloads/payloads.hpp"
#include "../../../level-world/floor_source.hpp"
#include "../../../level-world/selector.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

namespace dh::foundation::map_ui {
namespace {
using MapBorrow = dh2::loader::FixedMapV1::Borrow;

struct DecodedZoneLeaseV1 {
    MapBorrow map;
    std::uint32_t module_index{};
    Bounds3V1 bounds;
};

bool finite_box(const Bounds3V1& box) {
    return std::isfinite(box.min_x) && std::isfinite(box.min_y) &&
           std::isfinite(box.min_z) && std::isfinite(box.max_x) &&
           std::isfinite(box.max_y) && std::isfinite(box.max_z) &&
           box.min_x <= box.max_x && box.min_y <= box.max_y &&
           box.min_z <= box.max_z;
}

void merge(Bounds3V1& into, const Bounds3V1& box) {
    into.min_x = std::min(into.min_x, box.min_x);
    into.min_y = std::min(into.min_y, box.min_y);
    into.min_z = std::min(into.min_z, box.min_z);
    into.max_x = std::max(into.max_x, box.max_x);
    into.max_y = std::max(into.max_y, box.max_y);
    into.max_z = std::max(into.max_z, box.max_z);
}

bool transformed_bounds(const dh2::assets::Mesh& mesh,
                        const std::array<float, 16>& matrix,
                        Bounds3V1& out, std::string& error) {
    dh2::octree::Box local{}, world{};
    std::copy_n(mesh.minimum, 3, local.minimum);
    std::copy_n(mesh.maximum, 3, local.maximum);
    dh2::selector::Matrix transform{};
    std::copy(matrix.begin(), matrix.end(), transform.values);
    transform.identity = 0;
    if (dh2_floor_transform_bounds(&world, &local, &transform) != 0) {
        error = "Decoded RoomZone: transformed module mesh bounds rejected";
        return false;
    }
    out = {world.minimum[0], world.minimum[1], world.minimum[2],
           world.maximum[0], world.maximum[1], world.maximum[2]};
    if (!finite_box(out)) {
        error = "Decoded RoomZone: non-finite transformed module mesh bounds";
        return false;
    }
    return true;
}

bool module_bounds(const MapBorrow& map, std::uint32_t module_index,
                   Bounds3V1& output, std::string& error) {
    const auto& modules = map.modules();
    const auto& scene = map.scene();
    const auto& assets = map.assets();
    const auto& instances = map.instances();
    if (module_index >= modules.size()) {
        error = "Decoded RoomZone: FixedMap module index outside source map";
        return false;
    }
    const auto& module = modules[module_index];
    if (module.asset >= assets.size() || module.root >= scene.graph.size()) {
        error = "Decoded RoomZone: incomplete transformed module root metadata";
        return false;
    }

    // RootSceneNode::RefreshBoundingBox first unions actual child-mesh cached
    // world AABBs. OptimizeStatic has already made those child poses absolute.
    Bounds3V1 root_box{-1, -1, -1, 1, 1, 1};
    bool has_geometry = false;
    for (const auto& placed : instances) {
        if (placed.module != module_index) continue;
        if (placed.asset != module.asset || placed.scene_instance >= scene.instances.size()) {
            error = "Decoded RoomZone: module instance asset/root association mismatch";
            return false;
        }
        const auto& instance = scene.instances[placed.scene_instance];
        if (instance.controller >= 0) {
            error = "Decoded RoomZone: source transformed-root skinned mesh bounds producer required";
            return false;
        }
        dh2::assets::Mesh mesh{};
        if (dh2_mesh_open(&mesh, &assets.at(placed.asset).view,
                          static_cast<std::int32_t>(instance.geometry)) != dh2::assets::Error::ok) {
            error = "Decoded RoomZone: actual module mesh geometry rejected";
            return false;
        }
        Bounds3V1 world_box{};
        if (!transformed_bounds(mesh, instance.world, world_box, error)) return false;
        if (!has_geometry) {
            root_box = world_box;
            has_geometry = true;
        } else {
            merge(root_box, world_box);
        }
    }

    // CanonicalModuleGraph's root box starts at [-1,+1]. With no recognized
    // mesh child it remains that constructor box. Otherwise its union is
    // relative to the root after subtracting the source root's local position.
    const auto& root = scene.graph[module.root];
    float* box_minimum[] = {&root_box.min_x, &root_box.min_y, &root_box.min_z};
    float* box_maximum[] = {&root_box.max_x, &root_box.max_y, &root_box.max_z};
    for (unsigned axis = 0; axis < 3; ++axis) {
        const auto position = module.transform.position[axis];
        if (!std::isfinite(position)) {
            error = "Decoded RoomZone: non-finite module root position";
            return false;
        }
        *box_minimum[axis] -= position;
        *box_maximum[axis] -= position;
    }
    dh2::octree::Box local{}, world{};
    const float minimum[3]{root_box.min_x, root_box.min_y, root_box.min_z};
    const float maximum[3]{root_box.max_x, root_box.max_y, root_box.max_z};
    std::copy_n(minimum, 3, local.minimum);
    std::copy_n(maximum, 3, local.maximum);
    dh2::selector::Matrix root_matrix{};
    std::copy(root.world.begin(), root.world.end(), root_matrix.values);
    root_matrix.identity = 0;
    if (dh2_floor_transform_bounds(&world, &local, &root_matrix) != 0) {
        error = "Decoded RoomZone: transformed root AABB rejected";
        return false;
    }
    output = {world.minimum[0], world.minimum[1], world.minimum[2],
              world.maximum[0], world.maximum[1], world.maximum[2]};
    if (!finite_box(output)) {
        error = "Decoded RoomZone: invalid transformed root AABB";
        return false;
    }
    return true;
}

bool zone_bounds(const std::shared_ptr<void>& opaque, Bounds3V1& bounds,
                 std::string& error) {
    if (!opaque) {
        error = "Decoded RoomZone: required retained FixedMap/root lease";
        return false;
    }
    auto lease = std::static_pointer_cast<DecodedZoneLeaseV1>(opaque);
    if (!lease || !lease->map || !finite_box(lease->bounds)) {
        error = "Decoded RoomZone: expired or invalid retained module root";
        return false;
    }
    bounds = lease->bounds;
    error.clear();
    return true;
}
}

bool DecodedRoomZoneV1::has_inside(const Point3V1& point) const noexcept {
    return point.x >= bounds.min_x && point.x <= bounds.max_x &&
           point.y >= bounds.min_y && point.y <= bounds.max_y;
}

bool source_map_decoded_room_zones_v1(
    const MapBorrow& map, std::vector<DecodedRoomZoneV1>& out,
    std::string& error) {
    error.clear();
    try {
        if (!map) {
            error = "Decoded RoomZone: required prepared FixedMap borrow";
            return false;
        }
        std::vector<DecodedRoomZoneV1> next;
        const auto& modules = map.modules();
        next.reserve(modules.size());
        for (std::uint32_t i = 0; i < modules.size(); ++i) {
            Bounds3V1 bounds{};
            if (!module_bounds(map, i, bounds, error)) return false;
            auto lease = std::make_shared<DecodedZoneLeaseV1>();
            lease->map = map;
            lease->module_index = i;
            lease->bounds = bounds;
            DecodedRoomZoneV1 zone;
            zone.owner = std::move(lease);
            zone.module_index = i;
            zone.module_name = modules[i].authored_name;
            zone.bounds = bounds;
            zone.visited = std::nullopt;
            next.push_back(std::move(zone));
        }
        out = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& ex) {
        error = ex.what();
        return false;
    }
}

bool source_map_decoded_room_zone_inputs_v1(
    const std::vector<DecodedRoomZoneV1>& decoded,
    const ReadDecodedRoomZoneVisitedV1& read_visited,
    std::vector<RoomZoneV1>& out, std::string& error) {
    error.clear();
    if (!read_visited) {
        error = "Decoded RoomZone: required same-owner live Module visited3fc provider";
        return false;
    }
    std::vector<RoomZoneV1> next;
    next.reserve(decoded.size());
    for (const auto& zone : decoded) {
        if (!zone.owner) {
            error = "Decoded RoomZone: missing retained decoded root owner";
            return false;
        }
        bool visited{};
        if (!read_visited(zone, visited, error)) {
            if (error.empty()) error = "Decoded RoomZone: same-owner visitation read failed";
            return false;
        }
        next.push_back({zone.owner, visited});
    }
    out = std::move(next);
    error.clear();
    return true;
}

RoomServicesV1 source_map_decoded_room_zone_services_v1() {
    RoomServicesV1 services;
    services.has_inside = [](const std::shared_ptr<void>& owner,
                             const Point3V1& point, bool& inside,
                             std::string& error) {
        Bounds3V1 bounds{};
        if (!zone_bounds(owner, bounds, error)) return false;
        inside = point.x >= bounds.min_x && point.x <= bounds.max_x &&
                 point.y >= bounds.min_y && point.y <= bounds.max_y;
        error.clear();
        return true;
    };
    return services;
}

} // namespace dh::foundation::map_ui
