#include "../../world-data/world.hpp"
#include "../../world-data/world_scene.hpp"
#include "../scene_buffers.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace {
void require(bool condition, const char* message, unsigned module = 99) {
    if (condition) return;
    std::fprintf(stderr, "SWAMP all-modules regression failed (module %u): %s\n",
                 module, message);
    std::exit(1);
}
std::vector<std::uint8_t> read_file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    require(static_cast<bool>(input), "cannot open cache input");
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}
}

int main(int argc, char** argv) {
    require(argc == 2, "usage: swamp_all_modules <cache-root>");
    const std::string cache = argv[1];
    const auto mlx = read_file(cache + "/data/scene/001_swamp.mlx");
    dh2::world::SourceLevel level{};
    dh2::world::Diagnostic diagnostic{};
    require(dh2_world_import_level(&level, "SWAMP", "data/scene/001_swamp.mlx",
        mlx.data(), mlx.size(), &diagnostic) == dh2::world::Error::ok,
        diagnostic.message);
    constexpr const char* expected_names[] = {
        "obj_4of4_brdwalk_sw_00_0", "obj_3of4_brdwalk_sw_00_1",
        "obj_1of4_brdwalk_nse_00_2", "corner_ruin_ws_00_3",
        "merchantcamp_ruins_swe_00_4", "corner_brdwalk_se_00_5",
        "deadend_brdwalk_w_00_7", "bossroom_ruins_ns__8",
        "obj_2of4_brdwalk_sw_00_9"
    };
    constexpr const char* expected_roots[] = {
        "_module_obj_4of4_brdwalk_sw_00-node",
        "_module_obj_3of4_brdwalk_sw_00-node",
        "_module_obj_1of4_brdwalk_nse_00-node",
        "_module_corner_ruin_ws_00-node",
        "_module_merchantcamp_ruins_swe_00-node",
        "_module_corner_brdwalk_se_00-node",
        "_module_deadend_brdwalk_w_00-node",
        "_module_bossroom_ruins_ns_-node",
        "_module_obj_2of4_brdwalk_sw_00-node"
    };
    constexpr float expected_placements[9][3] = {
        {0,0,0}, {-6000,0,0}, {-6000,6000,0}, {-6000,12000,0},
        {-12000,12000,0}, {-12000,18000,0}, {-6000,18000,0},
        {-18000,12000,0}, {0,6000,0}
    };
    constexpr float expected_catalogue_origins[9][3] = {
        {52000,3000,0}, {10000,3000,0}, {44999.398f,10100.200f,0.280f},
        {17000,10000,0}, {38000,10000,0}, {10000,24000,0},
        {24000,17000,0}, {45000,17000,0}, {52000,10000,0}
    };
    constexpr std::uint32_t expected_counts[9][4] = {
        {103,54,10816,13284}, {100,55,7950,10557}, {83,46,5720,8298},
        {59,59,6383,9840}, {59,62,9326,12813}, {49,52,4372,6621},
        {42,43,2389,4185}, {12,9,4855,7404}, {108,56,2713,4782}
    };
    constexpr float expected_bounds[9][6] = {
        {-3023.690f,-3000.000f,-35.033f, 3000.000f,3000.710f,3742.400f},
        {-9000.010f,-3000.002f,-128.474f,-2999.979f,3000.000f,3600.000f},
        {-9000.900f,2999.799f,-28.057f,-2994.301f,9000.499f,3600.000f},
        {-9000.000f,9000.000f,-57.643f,-2999.950f,15000.000f,3600.000f},
        {-15000.000f,8953.260f,-75.766f,-9000.000f,15019.460f,3600.000f},
        {-15000.130f,14999.870f,-110.592f,-8999.830f,21000.871f,3600.000f},
        {-9000.120f,14999.840f,-239.154f,-2999.120f,21000.150f,3600.000f},
        {-21024.949f,9000.000f,-947.893f,-14997.860f,15000.010f,3600.000f},
        {-3000.000f,3000.000f,-140.658f,3000.000f,9000.004f,3600.000f}
    };
    require(level.module_count == 9, "SWAMP module count changed");
    const auto& dae = level.modules[0].cache_dae;
    require(dae && std::strcmp(dae, "data/3d/modules/swamp/swamp.bdae") == 0,
            "SWAMP catalogue path changed");
    const auto bres_bytes = read_file(cache + "/" + dae);
    const auto before = bres_bytes;
    dh2::resources::BresView bres{};
    require(dh2_bres_open(&bres, bres_bytes.data(), bres_bytes.size()) ==
            dh2::resources::BresError::ok, "SWAMP catalogue BRES parse failed");
    dh2::scene_payload::Scene scene{};
    require(dh2_scene_open(&scene, &bres) == dh2::scene_payload::Error::ok,
            "SWAMP catalogue scene parse failed");

    for (std::uint32_t i = 0; i < level.module_count; ++i) {
        auto& module = level.modules[i];
        require(std::strcmp(module.record.name, expected_names[i]) == 0,
                "selected MLX module name/order changed", i);
        require(module.catalogue_node_id &&
                std::strcmp(module.catalogue_node_id, expected_roots[i]) == 0,
                "selected catalogue root ID changed", i);
        for (unsigned axis = 0; axis < 3; ++axis)
            require(std::fabs(module.record.local.position[axis] -
                              expected_placements[i][axis]) < 0.001f,
                    "MLX module placement changed", i);

        dh2::world::ModuleBinding binding{};
        auto result = dh2_world_bind_module(&binding, &module, &scene, &diagnostic);
        require(result == dh2::world::Error::ok, diagnostic.message, i);
        dh2::math::Matrix4f correction{};
        require(dh2_world_placement_matrix(&correction, &binding, &diagnostic) ==
                dh2::world::Error::ok, diagnostic.message, i);
        for (unsigned axis = 0; axis < 3; ++axis)
            require(std::isfinite(binding.catalogue_origin[axis]) &&
                    std::fabs(binding.catalogue_origin[axis] -
                              expected_catalogue_origins[i][axis]) < 0.01f &&
                    std::fabs(correction.m[12 + axis] - binding.placement_delta[axis]) < 0.001f &&
                    std::fabs(binding.placement_delta[axis] -
                        (expected_placements[i][axis] - binding.catalogue_origin[axis])) < 0.001f,
                    "catalogue-to-MLX correction changed", i);
        std::vector<std::uint32_t> records(65536);
        std::uint32_t record_count = 0;
        result = dh2_world_module_records(records.data(), records.size(), &record_count,
                                           &binding, &scene, &diagnostic);
        require(result == dh2::world::Error::ok, diagnostic.message, i);
        require(record_count > 0 && record_count <= records.size() &&
                records[0] == binding.node_record &&
                record_count == expected_counts[i][0],
                "selected root is not first in complete subtree", i);
        dh2::viewer::SceneMesh mesh{};
        const auto mesh_result = dh2_world_scene_mesh_nodes(&mesh, &bres, records.data(),
            record_count, &correction);
        if (mesh_result == dh2::viewer::SceneMeshError::limit) {
            std::fprintf(stderr, "module %u reached renderer limit (records=%u)\n", i,
                         record_count);
            std::exit(2);
        }
        require(mesh_result == dh2::viewer::SceneMeshError::ok,
                "module subtree assembly failed", i);
        require(mesh.draw_commands > 0 && mesh.draw_commands <= 8192 &&
                mesh.vertex_count > 0 && mesh.vertex_count <= 65535 &&
                mesh.index_count > 0 && mesh.index_count <= 1000000 && mesh.draws,
                "module draw buffers are empty or exceed renderer bounds", i);
        require(mesh.draw_commands == expected_counts[i][1] &&
                mesh.vertex_count == expected_counts[i][2] &&
                mesh.index_count == expected_counts[i][3],
                "module draw/vertex/index counts changed", i);

        float minimum[3] = {INFINITY, INFINITY, INFINITY};
        float maximum[3] = {-INFINITY, -INFINITY, -INFINITY};
        for (std::uint32_t v = 0; v < mesh.vertex_count; ++v) {
            for (unsigned axis = 0; axis < 3; ++axis) {
                const float value = mesh.vertices[5 * v + axis];
                require(std::isfinite(value), "nonfinite placed vertex", i);
                minimum[axis] = std::min(minimum[axis], value);
                maximum[axis] = std::max(maximum[axis], value);
            }
        }
        for (unsigned axis = 0; axis < 3; ++axis)
            require(std::isfinite(minimum[axis]) && std::isfinite(maximum[axis]) &&
                    maximum[axis] > minimum[axis] &&
                    std::fabs(minimum[axis] - expected_bounds[i][axis]) < 0.02f &&
                    std::fabs(maximum[axis] - expected_bounds[i][axis + 3]) < 0.02f,
                    "module geometry bounds changed", i);
        for (std::uint32_t d = 0; d < mesh.draw_commands; ++d) {
            const auto& draw = mesh.draws[d];
            require(draw.first_vertex + draw.vertex_count <= mesh.vertex_count &&
                    draw.first_index + draw.index_count <= mesh.index_count,
                    "draw descriptor exceeds module buffers", i);
            require(std::find(records.begin(), records.begin() + record_count,
                              draw.node_record) != records.begin() + record_count,
                    "draw descriptor escaped selected subtree", i);
        }
        std::printf("module %u root=%s rec=%u draws=%u verts=%u inds=%u bounds="
                    "[%.3f,%.3f,%.3f]-[%.3f,%.3f,%.3f] origin=(%.3f,%.3f,%.3f) "
                    "correction=(%.3f,%.3f,%.3f)\n", i, module.catalogue_node_id,
                    record_count, mesh.draw_commands, mesh.vertex_count, mesh.index_count,
                    minimum[0], minimum[1], minimum[2], maximum[0], maximum[1], maximum[2],
                    binding.catalogue_origin[0], binding.catalogue_origin[1],
                    binding.catalogue_origin[2], correction.m[12], correction.m[13],
                    correction.m[14]);
        dh2_viewer_scene_mesh_free(&mesh);
    }
    require(bres_bytes == before, "SWAMP catalogue BRES bytes changed during assembly");
    dh2_world_free(&level);
    std::puts("all nine SWAMP modules assembled within current renderer bounds; BRES unchanged");
    return 0;
}
