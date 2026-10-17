#include "../swamp_actor_floor_bridge.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
struct SnapshotHeader {
    char magic[8];
    std::uint32_t version;
    std::uint32_t surface_count;
    std::uint32_t triangle_count;
};
static_assert(sizeof(SnapshotHeader) == 20);

void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

template<class T>
void read_exact(std::ifstream& input, T* value, std::size_t count = 1) {
    input.read(reinterpret_cast<char*>(value),
        static_cast<std::streamsize>(sizeof(T) * count));
    require(bool(input), "Truncated source-navigation snapshot");
}

bool close(float a, float b, float epsilon = 1.0e-4f) {
    return std::fabs(a - b) <= epsilon;
}

dh2::navigation::WorldHit query_bridge(
    const dh2::navigation::CollisionWorld& geometry, const float* point) {
    dh2::navigation::WorldHit result{};
    require(dh2_nav_world_collision(&result, &geometry, point, 0) >= 0,
            "Actor-runtime world collision query failed");
    return result;
}

void emit_hit(const char* name, const dh2::navigation::WorldHit& hit,
              const dh2::irrlicht_game::SwampActorFloorBridge& bridge) {
    std::cout << "\"" << name << "\":{";
    if (!hit.hit) {
        std::cout << "\"found\":false}";
        return;
    }
    const auto& source = bridge.source_floors().at(hit.floor);
    std::cout << "\"found\":true,\"actor_floor\":" << hit.floor
        << ",\"source_surface\":" << source.source_surface_index
        << ",\"flags\":" << source.floor_type_flags
        << ",\"height\":" << hit.collision.point[2]
        << ",\"node_id\":\"" << source.source_node_id << "\"}";
}
} // namespace

int main(int argc, char** argv) {
    try {
        require(argc == 6, "Usage: swamp_actor_floor_bridge_host <snapshot.bin> <water-x> <water-y> <water-z> <water-source-surface>");
        std::ifstream input(argv[1], std::ios::binary);
        require(bool(input), "Could not open source-navigation snapshot");
        SnapshotHeader header{};
        read_exact(input, &header);
        require(std::memcmp(header.magic, "DH2SWF01", 8) == 0 && header.version == 1
                    && header.surface_count > 0 && header.surface_count <= 256
                    && header.triangle_count > 0 && header.triangle_count <= 100000,
                "Invalid source-navigation snapshot header");
        std::vector<dh2::irrlicht_game::SwampSourceSurfaceView> surfaces(header.surface_count);
        std::vector<dh2::irrlicht_game::SwampSourceTriangleView> triangles(header.triangle_count);
        read_exact(input, surfaces.data(), surfaces.size());
        read_exact(input, triangles.data(), triangles.size());
        require(input.peek() == std::char_traits<char>::eof(),
                "Unexpected trailing bytes in source-navigation snapshot");

        const dh2::irrlicht_game::SwampSourceNavigationView source{
            surfaces.data(), header.surface_count, header.surface_count,
            triangles.data(), header.triangle_count, header.triangle_count};
        dh2::irrlicht_game::SwampActorFloorBridge bridge;
        std::string error;
        require(bridge.build_module_zero(source,
                    dh2::irrlicht_game::swamp_module_zero_player_path_mask, error),
                error.c_str());
        const auto* geometry = bridge.collision_world();
        const auto* graph = bridge.graph();
        require(geometry && graph && geometry->room_count == 1
                    && geometry->floor_count == bridge.source_floors().size()
                    && graph->query && graph->node_count,
                "Actor-runtime floor or graph view is incomplete");
        require(bridge.object_path_mask() == 2,
                "Module-zero player path mask was not retained as 0x2");

        std::uint32_t expected_module_zero = 0;
        std::uint32_t source_triangles = 0;
        bool boardwalk_seen = false;
        bool water_seen = false;
        for (const auto& surface : surfaces) {
            if (surface.module_index == 0) ++expected_module_zero;
        }
        require(expected_module_zero == bridge.source_floors().size(),
                "Bridge did not preserve every module-zero source floor");
        for (std::size_t i = 0; i < bridge.source_floors().size(); ++i) {
            const auto& mapped = bridge.source_floors()[i];
            require(mapped.actor_floor_index == i && mapped.module_index == 0
                        && mapped.floor_type_flags_known,
                    "Mapped floor lost actor index/module/type provenance");
            const auto& original = surfaces.at(mapped.source_surface_index);
            require(original.module_index == mapped.module_index
                        && original.module_source_record == mapped.module_source_record
                        && original.node_record == mapped.node_record
                        && original.geometry_index == mapped.geometry_index
                        && original.floor_type_flags == mapped.floor_type_flags
                        && original.floor_type_flags_known == 1
                        && bool(original.floor_type_tag_present) == mapped.floor_type_tag_present
                        && mapped.source_triangle_count == original.triangle_count,
                    "Mapped floor provenance differs from source Navigation");
            const auto& collision_floor = geometry->floors[i];
            require(collision_floor.selector && collision_floor.selector->selector.tree
                        && collision_floor.selector->selector.tree->triangle_count == original.triangle_count
                        && mapped.source_triangles.size() == original.triangle_count,
                    "Collision triangles lost their source index mapping");
            for (std::uint32_t j = 0; j < original.triangle_count; ++j) {
                const auto& source_triangle = triangles[original.first_triangle + j];
                const auto& actor_triangle = collision_floor.selector->selector.tree->triangles[j];
                require(mapped.source_triangles[j].source_surface_index == mapped.source_surface_index
                            && mapped.source_triangles[j].primitive_index == source_triangle.primitive_index
                            && mapped.source_triangles[j].source_triangle_index == source_triangle.source_triangle_index
                            && std::memcmp(actor_triangle.points[0], source_triangle.c, sizeof(source_triangle.c)) == 0
                            && std::memcmp(actor_triangle.points[1], source_triangle.b, sizeof(source_triangle.b)) == 0
                            && std::memcmp(actor_triangle.points[2], source_triangle.a, sizeof(source_triangle.a)) == 0,
                        "Source triangle winding adaptation or source index changed in bridge");
            }
            require(mapped.source_node_id == original.source_node_id
                        && mapped.source_node_name == original.source_node_name
                        && mapped.source_geometry_id == original.source_geometry_id
                        && mapped.source_geometry_name == original.source_geometry_name
                        && mapped.module_name == original.module_name
                        && mapped.floor_type_tag == original.floor_type_tag,
                    "Mapped source node/type text changed");
            source_triangles += mapped.source_triangle_count;
            boardwalk_seen |= mapped.source_node_id == "_floor_obj_4of4_brdwalk_sw_-node"
                && mapped.floor_type_flags == 0;
            water_seen |= mapped.source_node_id == "_floor_water_obj_4of4_brdwalk_sw_-node"
                && mapped.floor_type_flags == 2 && mapped.floor_type_tag == "water";
        }
        require(boardwalk_seen && water_seen,
                "Expected authored boardwalk/water source surfaces were not mapped");

        const float start[3]{1090.75f, -212.202f, 258.0f};
        const auto bridge_start = query_bridge(*geometry, start);
        require(bridge_start.hit, "Actor-runtime bridge rejected source entrypoint");
        const float moved[3]{1091.75f, -212.202f, 255.0f};
        const auto bridge_moved = query_bridge(*geometry, moved);
        require(bridge_moved.hit, "Actor-runtime bridge rejected moved endpoint");
        char* end = nullptr;
        float water_point[3]{};
        for (unsigned i = 0; i < 3; ++i) {
            water_point[i] = std::strtof(argv[2 + i], &end);
            require(end && *end == '\0' && std::isfinite(water_point[i]),
                    "Invalid water fixture coordinate");
        }
        const auto water_source_surface = static_cast<std::uint32_t>(std::strtoul(argv[5], &end, 10));
        require(end && *end == '\0' && water_source_surface < surfaces.size()
                    && surfaces[water_source_surface].module_index == 0
                    && surfaces[water_source_surface].floor_type_flags == 2,
                "Water fixture is not an authored module-zero water surface");
        const auto bridge_water = query_bridge(*geometry, water_point);
        require(bridge_water.hit, "Actor-runtime bridge rejected source water endpoint");
        const float outside[3]{5000.0f, 0.0f, 255.0f};
        const auto bridge_outside = query_bridge(*geometry, outside);
        require(!bridge_outside.hit, "Actor-runtime bridge accepted outside source point");

        dh2::navigation::MotionObject actor{};
        actor.flags = 2;
        actor.room = std::numeric_limits<std::uint32_t>::max();
        actor.floor = std::numeric_limits<std::uint32_t>::max();
        std::copy(start, start + 3, actor.position);
        dh2::navigation::MotionPolicy policy{100.0f, 0};
        dh2::navigation::PositionResult accepted{};
        float candidate[3]{start[0], start[1], start[2] + 0.5f};
        require(dh2_nav_validate_position(&accepted, geometry, &actor,
                    candidate, &policy) == 0
                    && accepted.valid && accepted.kind == 2
                    && close(candidate[2], bridge_start.collision.point[2]),
                "Actor-runtime entrypoint position validation failed");
        candidate[0] = moved[0]; candidate[1] = moved[1]; candidate[2] = moved[2];
        require(dh2_nav_validate_position(&accepted, geometry, &actor,
                    candidate, &policy) == 0
                    && accepted.valid && accepted.kind == 2
                    && close(candidate[2], bridge_moved.collision.point[2]),
                "Actor-runtime moved endpoint position validation failed");
        dh2::navigation::MotionObject no_water{};
        no_water.flags = 0;
        no_water.room = std::numeric_limits<std::uint32_t>::max();
        no_water.floor = std::numeric_limits<std::uint32_t>::max();
        no_water.position[0] = water_point[0] - 1.0f;
        no_water.position[1] = water_point[1];
        no_water.position[2] = water_point[2] + 1.0f;
        float water_candidate[3]{water_point[0], water_point[1], water_point[2] + 0.5f};
        require(dh2_nav_validate_position(&accepted, geometry, &no_water,
                    water_candidate, &policy) == 0 && accepted.kind == 3
                    && close(water_candidate[0], no_water.position[0])
                    && close(water_candidate[1], no_water.position[1]),
                "Actor-runtime admitted a water floor without source path bit 0x2");
        candidate[0] = outside[0]; candidate[1] = outside[1]; candidate[2] = outside[2];
        require(dh2_nav_validate_position(&accepted, geometry, &actor,
                    candidate, &policy) == 0 && !accepted.valid && accepted.kind == 3
                    && close(candidate[0], actor.position[0])
                    && close(candidate[1], actor.position[1]),
                "Actor-runtime outside endpoint was not rejected/clamped");

        dh2::irrlicht_game::SwampActorFloorBridge wrong_mask;
        require(!wrong_mask.build_module_zero(source, 0, error)
                    && error.find("path mask 0x2") != std::string::npos,
                "Bridge accepted an unapproved player path mask");
        auto unknown_surfaces = surfaces;
        unknown_surfaces[bridge.source_floors().front().source_surface_index]
            .floor_type_flags_known = 0;
        const dh2::irrlicht_game::SwampSourceNavigationView unknown_source{
            unknown_surfaces.data(), header.surface_count, header.surface_count,
            triangles.data(), header.triangle_count, header.triangle_count};
        dh2::irrlicht_game::SwampActorFloorBridge unknown_flags;
        require(!unknown_flags.build_module_zero(unknown_source, 2, error)
                    && error.find("unknown native flags") != std::string::npos,
                "Bridge accepted a module-zero floor without known source flags");

        auto mismatched_flag_surfaces = surfaces;
        const auto boardwalk = std::find_if(bridge.source_floors().begin(),
            bridge.source_floors().end(), [](const auto& floor) {
                return floor.source_node_id == "_floor_obj_4of4_brdwalk_sw_-node";
            });
        require(boardwalk != bridge.source_floors().end(),
                "Boardwalk source record is required for type-mask mutation check");
        mismatched_flag_surfaces[boardwalk->source_surface_index].floor_type_flags = 2;
        const dh2::irrlicht_game::SwampSourceNavigationView mismatched_flags_source{
            mismatched_flag_surfaces.data(), header.surface_count, header.surface_count,
            triangles.data(), header.triangle_count, header.triangle_count};
        dh2::irrlicht_game::SwampActorFloorBridge mismatched_flags;
        require(!mismatched_flags.build_module_zero(mismatched_flags_source, 2, error)
                    && error.find("differs between adapters") != std::string::npos,
                "Bridge accepted a claimed water mask on an untyped boardwalk");

        auto mismatched_surface_ids = surfaces;
        mismatched_surface_ids[boardwalk->source_surface_index].source_surface_index++;
        const dh2::irrlicht_game::SwampSourceNavigationView mismatched_ids_source{
            mismatched_surface_ids.data(), header.surface_count, header.surface_count,
            triangles.data(), header.triangle_count, header.triangle_count};
        dh2::irrlicht_game::SwampActorFloorBridge mismatched_ids;
        require(!mismatched_ids.build_module_zero(mismatched_ids_source, 2, error)
                    && error.find("surface index differs") != std::string::npos,
                "Bridge accepted a source surface ID that differs from its owner record");

        std::cout << "{\"validation\":\"PASS\",\"module\":0,\"path_mask\":2"
                  << ",\"source_module_zero_surfaces\":" << expected_module_zero
                  << ",\"mapped_surfaces\":" << bridge.source_floors().size()
                  << ",\"mapped_triangles\":" << source_triangles
                  << ",\"graph_nodes\":" << graph->node_count
                  << ",\"graph_edges\":" << graph->edge_count << ",";
        emit_hit("start", bridge_start, bridge);
        std::cout << ',';
        emit_hit("moved", bridge_moved, bridge);
        std::cout << ',';
        emit_hit("water", bridge_water, bridge);
        std::cout << ",\"water_point\":[" << water_point[0] << ','
                  << water_point[1] << ',' << water_point[2]
                  << "],\"water_source_surface\":" << water_source_surface << ',';
        emit_hit("outside", bridge_outside, bridge);
        std::cout << ",\"source_floor_queries\":3,\"rejection_checks\":7"
                  << ",\"scope\":\"actual-cache module-zero source triangles converted to native PFFloor C/B/A winding with source IDs and decoded masks retained; no actor physics or gameplay session\"}\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "FAIL: " << exception.what() << '\n';
        return 1;
    }
}
