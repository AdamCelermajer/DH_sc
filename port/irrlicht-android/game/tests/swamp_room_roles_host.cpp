#include "../scene_mesh_adapter.hpp"
#include "../../../android-app/scene_buffers.hpp"
#include "../../../asset-payloads/payloads.hpp"
#include "../../../world-data/world.hpp"
#include "../../../world-data/world_scene.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace {
void require(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "SWAMP room-role host check failed: %s\n", message);
        std::exit(1);
    }
}

std::vector<std::uint8_t> read_file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    require(static_cast<bool>(input), "cannot read supplied cache file");
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

void set_node_id(dh2::viewer::SceneDrawDescriptor& draw, const char* text) {
    const auto size = std::strlen(text);
    require(size < sizeof(draw.node_id), "synthetic role test ID is too long");
    std::memcpy(draw.node_id, text, size + 1);
}

void check_classifier() {
    using dh2::irrlicht_adapter::SourceRoomDrawRole;
    dh2::viewer::SceneDrawDescriptor draw{};
    draw.node_record = 73;
    set_node_id(draw, "ordinary_mesh-node");
    require(dh2::irrlicht_adapter::classify_source_room_draw(draw, 73) ==
                SourceRoomDrawRole::room_root_bounds,
            "the exact source root record must classify as root bounds");
    draw.node_record = 74;
    require(dh2::irrlicht_adapter::classify_source_room_draw(draw, 73) ==
                SourceRoomDrawRole::scenery,
            "a non-root mesh must remain scenery");
    set_node_id(draw, "_floor_wood-node");
    require(dh2::irrlicht_adapter::classify_source_room_draw(draw, 73) ==
                SourceRoomDrawRole::navigation_floor,
            "an authored floor component at the start of the ID must filter");
    set_node_id(draw, "room0/_floor_water-node");
    require(dh2::irrlicht_adapter::classify_source_room_draw(draw, 73) ==
                SourceRoomDrawRole::navigation_floor,
            "an authored floor component after a path separator must filter");
    set_node_id(draw, "room0/_exit_north-node");
    require(dh2::irrlicht_adapter::classify_source_room_draw(draw, 73) ==
                SourceRoomDrawRole::exit_marker,
            "an authored exit component after a path separator must filter");
    for (const char* valid_scenery : {
             "floorboards-node", "decor_floor_lamp-node", "my_exit_sign-node",
             "room0/bridge_floor_trim-node", "room0/decor_exit_arch-node"}) {
        set_node_id(draw, valid_scenery);
        require(dh2::irrlicht_adapter::classify_source_room_draw(draw, 73) ==
                    SourceRoomDrawRole::scenery,
                "a non-role substring in a scenery ID must not be filtered");
    }
    std::memset(draw.node_id, 'x', sizeof(draw.node_id));
    require(dh2::irrlicht_adapter::classify_source_room_draw(draw, 73) ==
                SourceRoomDrawRole::scenery,
            "an unterminated ID must fail open as scenery rather than truncate-match");
}

}

int main(int argc, char** argv) {
    require(argc == 2, "usage: swamp_room_roles_host <cache-root>");
    check_classifier();
    const std::string cache = argv[1];
    const auto mlx = read_file(cache + "/data/scene/001_swamp.mlx");
    const auto catalogue = read_file(cache + "/data/3d/modules/swamp/swamp.bdae");
    dh2::world::SourceLevel level{};
    dh2::world::Diagnostic diagnostic{};
    require(dh2_world_import_level(&level, "SWAMP", "data/scene/001_swamp.mlx",
                mlx.data(), mlx.size(), &diagnostic) == dh2::world::Error::ok,
            "SWAMP MLX source import failed");
    require(level.module_count == 9, "source MLX must contain all nine SWAMP modules");

    dh2::resources::BresView bres{};
    require(dh2_bres_open(&bres, catalogue.data(), catalogue.size()) ==
                dh2::resources::BresError::ok,
            "SWAMP BRES source parse failed");

    std::uint32_t total_draws = 0, total_visible = 0, total_scenery = 0;
    std::uint32_t total_root = 0, total_floors = 0, total_exits = 0;
    std::uint32_t total_floor_triangles = 0, module_zero_floor_triangles = 0;
    std::uint32_t module_zero_floor_draws = 0;
    std::printf("{\"validation\":\"PASS\",\"modules\":[");
    for (std::uint32_t module_index = 0; module_index < level.module_count;
         ++module_index) {
        dh2::scene_payload::Scene source_scene{};
        require(dh2_scene_open(&source_scene, &bres) ==
                    dh2::scene_payload::Error::ok,
                "SWAMP BRES scene open failed");
        dh2::world::ModuleBinding binding{};
        require(dh2_world_bind_module(&binding, &level.modules[module_index],
                    &source_scene, &diagnostic) == dh2::world::Error::ok,
                "source module root did not bind");
        std::vector<std::uint32_t> records(65536);
        std::uint32_t record_count = 0;
        require(dh2_world_module_records(records.data(), records.size(),
                    &record_count, &binding, &source_scene, &diagnostic) ==
                    dh2::world::Error::ok,
                "source module subtree enumeration failed");
        dh2::math::Matrix4f correction{};
        require(dh2_world_placement_matrix(&correction, &binding, &diagnostic) ==
                    dh2::world::Error::ok,
                "source module placement matrix failed");
        dh2::viewer::SceneMesh mesh{};
        require(dh2_world_scene_mesh_nodes(&mesh, &bres, records.data(),
                    record_count, &correction) == dh2::viewer::SceneMeshError::ok,
                "source module SceneMesh assembly failed");

        std::uint32_t visible = 0, scenery = 0, root = 0, floors = 0, exits = 0;
        std::uint32_t floor_triangles = 0, scenery_triangles = 0;
        const std::vector<dh2::viewer::SceneDrawDescriptor> source_draw_snapshot(
            mesh.draws, mesh.draws + mesh.draw_commands);
        const std::vector<float> source_vertex_snapshot(
            mesh.vertices, mesh.vertices + std::size_t(mesh.vertex_count) * 5);
        const std::vector<std::uint16_t> source_index_snapshot(
            mesh.indices, mesh.indices + mesh.index_count);
        for (std::uint32_t i = 0; i < mesh.draw_commands; ++i) {
            const auto& draw = mesh.draws[i];
            if (!draw.visible) continue;
            ++visible;
            switch (dh2::irrlicht_adapter::classify_source_room_draw(
                        draw, binding.node_record)) {
            case dh2::irrlicht_adapter::SourceRoomDrawRole::room_root_bounds:
                ++root;
                break;
            case dh2::irrlicht_adapter::SourceRoomDrawRole::navigation_floor:
                ++floors;
                floor_triangles += draw.index_count / 3;
                break;
            case dh2::irrlicht_adapter::SourceRoomDrawRole::exit_marker:
                ++exits;
                break;
            case dh2::irrlicht_adapter::SourceRoomDrawRole::scenery:
                ++scenery;
                scenery_triangles += draw.index_count / 3;
                break;
            }
        }
        require(visible == scenery + root + floors + exits,
                "room-role categories do not partition visible source draws");
        require(mesh.draws && mesh.vertices && mesh.indices,
                "assembled module is missing immutable source arrays");

        if (module_index == 0) {
            module_zero_floor_draws = floors;
            module_zero_floor_triangles = floor_triangles;
            require(root == 1 && floors == 2 && exits == 2,
                    "module-zero roles differ from source root, wood/water floors and exits");
            require(floor_triangles == 99,
                    "module-zero source floor draw triangle count changed");
        }
        if (module_index) std::printf(",");
        std::printf("{\"index\":%u,\"name\":\"%s\",\"source_draws\":%u,\"source_visible\":%u,\"scenery_draws\":%u,\"root_bounds\":%u,\"floor_draws\":%u,\"floor_triangles\":%u,\"exit_markers\":%u,\"scenery_triangles\":%u,\"vertices\":%u,\"indices\":%u}",
            module_index, level.modules[module_index].record.name,
            mesh.draw_commands, visible, scenery, root, floors, floor_triangles,
            exits, scenery_triangles,
            mesh.vertex_count, mesh.index_count);
        total_draws += mesh.draw_commands;
        total_visible += visible;
        total_scenery += scenery;
        total_root += root;
        total_floors += floors;
        total_exits += exits;
        total_floor_triangles += floor_triangles;

        // Classification is a view over descriptors. It must not mutate the
        // source SceneMesh from which navigation and later views are built.
        require(mesh.draw_commands == source_draw_snapshot.size() &&
                    std::memcmp(mesh.draws, source_draw_snapshot.data(),
                        source_draw_snapshot.size() * sizeof(source_draw_snapshot[0])) == 0 &&
                    std::memcmp(mesh.vertices, source_vertex_snapshot.data(),
                        source_vertex_snapshot.size() * sizeof(source_vertex_snapshot[0])) == 0 &&
                    std::memcmp(mesh.indices, source_index_snapshot.data(),
                        source_index_snapshot.size() * sizeof(source_index_snapshot[0])) == 0,
                "role classification mutated the source SceneMesh arrays");
        dh2_viewer_scene_mesh_free(&mesh);
    }
    std::printf("],\"source_draws\":%u,\"source_visible\":%u,\"scenery_draws\":%u,\"root_bounds\":%u,\"floor_draws\":%u,\"exit_markers\":%u,\"floor_triangles\":%u,\"module_zero_floor_draws\":%u,\"module_zero_floor_triangles\":%u,\"classification\":\"source room-root record + slash-bounded _floor_/_exit_ node components; no substring fallback\"}\n",
        total_draws, total_visible, total_scenery, total_root, total_floors,
        total_exits, total_floor_triangles, module_zero_floor_draws,
        module_zero_floor_triangles);
    dh2_world_free(&level);
    return 0;
}
