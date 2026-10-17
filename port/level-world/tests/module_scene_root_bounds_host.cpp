#include "../module_scene_root_bounds.hpp"
#include "../floor_source.hpp"
#include "../selector.hpp"
#include "../../asset-payloads/payloads.hpp"

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
    std::fprintf(stderr, "module root bounds host failed (module %u): %s\n", module, message);
    std::exit(1);
}
std::vector<std::uint8_t> read_file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    require(static_cast<bool>(input), "cannot read cache file");
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}
bool near(float a, float b, float epsilon = 0.03f) {
    return std::fabs(a - b) <= epsilon;
}
struct StreamCheck {
    const dh2::resources::BresView* bres;
    const dh2::scene_payload::Scene* scene;
    const std::uint32_t* records;
    std::uint32_t record_count;
    dh2::world::ModuleBinding binding;
    float owner[3];
    const dh2::octree::Box* bounds;
    unsigned module;
    unsigned instances = 0;
    bool ok = true;
};
bool check_streams(const dh2::scene_payload::Node* node,
                   const dh2::math::Matrix4f* serialized_world,
                   std::uint32_t, void* opaque) {
    auto& check = *static_cast<StreamCheck*>(opaque);
    if (std::find(check.records, check.records + check.record_count, node->record) ==
        check.records + check.record_count) return true;
    auto binding = check.binding;
    for (unsigned axis = 0; axis != 3; ++axis)
        binding.placement_delta[axis] = check.owner[axis] - binding.catalogue_origin[axis];
    dh2::math::Matrix4f placed{};
    dh2::world::Diagnostic diagnostic{};
    if (dh2_world_place_matrix(&placed, &binding, serialized_world, &diagnostic) !=
        dh2::world::Error::ok) { check.ok = false; return false; }
    for (std::uint32_t i = 0; i < node->instances; ++i) {
        dh2::scene_payload::Instance instance{};
        if (dh2_scene_instance(node, static_cast<std::int32_t>(i), &instance) !=
            dh2::scene_payload::Error::ok) { check.ok = false; return false; }
        if (instance.type != 3) continue;
        const int geometry = dh2_scene_geometry_index(check.scene, &instance);
        dh2::assets::Mesh mesh{};
        if (geometry < 0 || dh2_mesh_open(&mesh, check.bres, geometry) !=
            dh2::assets::Error::ok) { check.ok = false; return false; }

        float stream_min[3] = {INFINITY, INFINITY, INFINITY};
        float stream_max[3] = {-INFINITY, -INFINITY, -INFINITY};
        bool saw_position = false;
        for (std::uint32_t p = 0; p < mesh.primitives; ++p) {
            dh2::assets::Primitive primitive{};
            if (dh2_mesh_primitive(&mesh, static_cast<std::int32_t>(p), &primitive) !=
                dh2::assets::Error::ok || primitive.attributes[0] < 0) {
                check.ok = false; return false;
            }
            dh2::assets::Attribute position{};
            if (dh2_mesh_attribute(&mesh, primitive.attributes[0], &position) !=
                dh2::assets::Error::ok || position.components < 3) {
                check.ok = false; return false;
            }
            saw_position = true;
            for (std::uint32_t v = 0; v < mesh.vertices; ++v) {
                float xyz[4]{};
                if (!dh2_attribute_read(&position, v, xyz)) {
                    check.ok = false; return false;
                }
                for (unsigned axis = 0; axis != 3; ++axis) {
                    stream_min[axis] = std::min(stream_min[axis], xyz[axis]);
                    stream_max[axis] = std::max(stream_max[axis], xyz[axis]);
                    const float world = placed.m[axis] * xyz[0] +
                        placed.m[4 + axis] * xyz[1] + placed.m[8 + axis] * xyz[2] +
                        placed.m[12 + axis];
                    if (!std::isfinite(world) ||
                        world < check.bounds->minimum[axis] - 0.04f ||
                        world > check.bounds->maximum[axis] + 0.04f) {
                        check.ok = false; return false;
                    }
                }
            }
        }
        if (!saw_position) { check.ok = false; return false; }
        for (unsigned axis = 0; axis != 3; ++axis) {
            if (!near(stream_min[axis], mesh.minimum[axis]) ||
                !near(stream_max[axis], mesh.maximum[axis])) {
                check.ok = false; return false;
            }
        }
        ++check.instances;
    }
    return true;
}
}

int main(int argc, char** argv) {
    require(argc == 2, "usage: module_scene_root_bounds_host <cache-root>");
    const std::string cache = argv[1];
    const auto mlx = read_file(cache + "/data/scene/001_swamp.mlx");
    dh2::world::SourceLevel level{};
    dh2::world::Diagnostic diagnostic{};
    require(dh2_world_import_level(&level, "SWAMP", "data/scene/001_swamp.mlx",
        mlx.data(), mlx.size(), &diagnostic) == dh2::world::Error::ok,
        diagnostic.message);
    require(level.module_count == 9, "expected nine source SWAMP modules");

    const auto dae = read_file(cache + "/data/3d/modules/swamp/swamp.bdae");
    const auto pristine = dae;
    dh2::resources::BresView bres{};
    require(dh2_bres_open(&bres, dae.data(), dae.size()) ==
            dh2::resources::BresError::ok, "BRES open failed");
    dh2::scene_payload::Scene scene{};
    require(dh2_scene_open(&scene, &bres) == dh2::scene_payload::Error::ok,
            "scene open failed");
    constexpr std::uint32_t expected_draws[9] = {54,55,46,59,62,52,43,9,56};
    constexpr std::uint32_t expected_instances[9] = {52,52,44,59,59,49,42,9,55};
    constexpr std::uint32_t expected_records[9] = {103,100,83,59,59,49,42,12,108};
    constexpr float expected_vertex_bounds[9][6] = {
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

    for (std::uint32_t i = 0; i < level.module_count; ++i) {
        auto& module = level.modules[i];
        require(module.cache_dae && std::strcmp(module.cache_dae,
                "data/3d/modules/swamp/swamp.bdae") == 0,
                "unexpected module catalogue path", i);
        dh2::module_scene_root_bounds::Result result{};
        const auto status = dh2::module_scene_root_bounds::build(&module, &scene, &bres,
            module.record.world_position, &result);
        require(status == dh2::module_scene_root_bounds::Status::complete,
                "source root bounds composition failed", i);
        require(result.scene_nodes == expected_records[i] &&
                result.geometry_instances == expected_instances[i] &&
                result.draw_buffers == expected_draws[i] &&
                result.ignored_non_geometry_instances == 0,
                "selected subtree composition differs from all-module source counts", i);
        for (unsigned axis = 0; axis != 3; ++axis)
            require(std::isfinite(result.bounds.minimum[axis]) &&
                    std::isfinite(result.bounds.maximum[axis]) &&
                    result.bounds.minimum[axis] < result.bounds.maximum[axis] &&
                    near(result.bounds.minimum[axis], expected_vertex_bounds[i][axis]) &&
                    near(result.bounds.maximum[axis], expected_vertex_bounds[i][axis + 3]),
                    "source root bounds disagree with independent assembled vertex extents", i);

        dh2::world::ModuleBinding binding{};
        require(dh2_world_bind_module(&binding, &module, &scene, &diagnostic) ==
                dh2::world::Error::ok, diagnostic.message, i);
        std::vector<std::uint32_t> records(65536);
        std::uint32_t record_count = 0;
        require(dh2_world_module_records(records.data(), 65536, &record_count,
                &binding, &scene, &diagnostic) == dh2::world::Error::ok &&
                record_count == expected_records[i], diagnostic.message, i);
        dh2::scene_payload::Visual visual{};
        require(dh2_scene_visual(&scene, static_cast<std::int32_t>(binding.visual_index),
                &visual) == dh2::scene_payload::Error::ok, "visual lookup failed", i);
        StreamCheck streams{&bres, &scene, records.data(), record_count, binding,
            {module.record.world_position[0], module.record.world_position[1],
             module.record.world_position[2]}, &result.bounds, i};
        require(dh2_scene_walk_visual(&visual, check_streams, &streams, 65536) ==
                dh2::scene_payload::Error::ok && streams.ok &&
                streams.instances == result.geometry_instances,
                "mesh stream bounds or placed vertices escaped root box", i);

        float moved_owner[3] = {module.record.world_position[0] + 123.25f,
                                module.record.world_position[1] - 456.5f,
                                module.record.world_position[2] + 7.75f};
        dh2::module_scene_root_bounds::Result moved{};
        require(dh2::module_scene_root_bounds::build(&module, &scene, &bres,
                moved_owner, &moved) == dh2::module_scene_root_bounds::Status::complete,
                "runtime owner move rejected", i);
        constexpr float shift[3] = {123.25f, -456.5f, 7.75f};
        for (unsigned axis = 0; axis != 3; ++axis)
            require(near(moved.bounds.minimum[axis] - result.bounds.minimum[axis], shift[axis]) &&
                    near(moved.bounds.maximum[axis] - result.bounds.maximum[axis], shift[axis]),
                    "owner SetPosition translation was not applied exactly once", i);

        std::printf("module %u root=%s nodes=%u instances=%u buffers=%u bounds="
                    "[%.3f,%.3f,%.3f]-[%.3f,%.3f,%.3f]\n", i,
                    module.catalogue_node_id, result.scene_nodes, result.geometry_instances,
                    result.draw_buffers, result.bounds.minimum[0], result.bounds.minimum[1],
                    result.bounds.minimum[2], result.bounds.maximum[0],
                    result.bounds.maximum[1], result.bounds.maximum[2]);
    }

    auto unsupported = level.modules[0];
    unsupported.record.local.scale[0] = 2.0f;
    dh2::module_scene_root_bounds::Result untouched{};
    untouched.bounds.minimum[0] = 1234.0f;
    require(dh2::module_scene_root_bounds::build(&unsupported, &scene, &bres,
            level.modules[0].record.world_position, &untouched) ==
            dh2::module_scene_root_bounds::Status::unsupported_transform &&
            untouched.bounds.minimum[0] == 1234.0f,
            "unsupported non-translation placement must fail without output mutation");
    require(dae == pristine, "catalogue bytes mutated during bounds query");
    dh2_world_free(&level);
    std::puts("PASS: original module/root placement bounds adapter, all 9 SWAMP modules");
    return 0;
}
