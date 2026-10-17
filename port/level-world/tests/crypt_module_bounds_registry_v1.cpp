#include "../crypt_module_bounds_registry_v1.hpp"
#include "../module_scene_root_bounds.hpp"
#include "../../world-data/world.hpp"
#include "../../world-data/world_scene.hpp"
#include "../../scene-payloads/scene.hpp"
#include "../../engine-resources/resources.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <limits>
#include <string>
#include <vector>

namespace {
using namespace dh2::crypt_module_bounds_registry_v1;

void require(bool condition, const char* message) {
    if (condition) return;
    std::fprintf(stderr, "Crypt module bounds registry failed: %s\n", message);
    std::exit(1);
}

std::vector<std::uint8_t> read_file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    require(static_cast<bool>(input), "cannot read packaged asset");
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

bool near(float actual, float expected, float epsilon = 0.06f) {
    return std::isfinite(actual) && std::fabs(actual - expected) <= epsilon;
}

void require_owner_preserved(const Owner& owner, const char* message) {
    require(owner.modules.size() == 8 &&
            owner.catalogue_path == "data/3d/modules/crypt/crypt.bdae" &&
            owner.modules[0].root_id == "_module_cemetery_entrance-node" &&
            near(owner.modules[0].bounds.minimum[0], -3198.84f), message);
}

struct InstanceMutation {
    std::vector<std::uint8_t>* bytes;
    const std::uint32_t* records;
    std::uint32_t record_count;
    bool all;
    std::uint32_t changed;
};

bool mutate_module_instances(const dh2::scene_payload::Node* node,
                             const dh2::math::Matrix4f*, std::uint32_t,
                             void* opaque) {
    auto& mutation = *static_cast<InstanceMutation*>(opaque);
    if (std::find(mutation.records, mutation.records + mutation.record_count,
                  node->record) == mutation.records + mutation.record_count)
        return true;
    for (std::uint32_t i = 0; i < node->instances; ++i) {
        dh2::scene_payload::Instance instance{};
        if (dh2_scene_instance(node, static_cast<std::int32_t>(i), &instance) !=
            dh2::scene_payload::Error::ok) return false;
        if (instance.type != 3) continue;
        const auto offset = std::size_t(node->instance_offset) + std::size_t(i) * 8;
        if (offset + 4 > mutation.bytes->size()) return false;
        (*mutation.bytes)[offset] = 1;
        (*mutation.bytes)[offset + 1] = 0;
        (*mutation.bytes)[offset + 2] = 0;
        (*mutation.bytes)[offset + 3] = 0;
        ++mutation.changed;
        if (!mutation.all) return false;
    }
    return true;
}

std::uint32_t change_first_module_geometry(std::vector<std::uint8_t>& bdae,
                                           const std::vector<std::uint8_t>& mlx,
                                           bool all) {
    dh2::world::SourceLevel source{};
    dh2::world::Diagnostic diagnostic{};
    require(dh2_world_import_level(&source, "GOTHICUS_CRYPT_01",
                "data/scene/x07_crypt_backup.mlx", mlx.data(), mlx.size(),
                &diagnostic) == dh2::world::Error::ok,
            "could not import MLX for non-geometry mutation");
    require(source.module_count == 8, "mutation input module count changed");
    dh2::resources::BresView catalogue{};
    require(dh2_bres_open(&catalogue, bdae.data(), bdae.size()) ==
                dh2::resources::BresError::ok,
            "could not open BRES for non-geometry mutation");
    dh2::scene_payload::Scene scene{};
    require(dh2_scene_open(&scene, &catalogue) == dh2::scene_payload::Error::ok,
            "could not open scene for non-geometry mutation");
    dh2::world::ModuleBinding binding{};
    require(dh2_world_bind_module(&binding, &source.modules[0], &scene,
                &diagnostic) == dh2::world::Error::ok,
            "could not bind Crypt module root for non-geometry mutation");
    std::vector<std::uint32_t> records(65536);
    std::uint32_t record_count = 0;
    require(dh2_world_module_records(records.data(),
                static_cast<std::uint32_t>(records.size()), &record_count,
                &binding, &scene, &diagnostic) == dh2::world::Error::ok &&
                record_count != 0,
            "could not enumerate Crypt module subtree for mutation");
    dh2::scene_payload::Visual visual{};
    require(dh2_scene_visual(&scene, static_cast<std::int32_t>(binding.visual_index),
                &visual) == dh2::scene_payload::Error::ok,
            "could not resolve Crypt module visual for mutation");
    InstanceMutation mutation{&bdae, records.data(), record_count, all, 0};
    const auto walked = dh2_scene_walk_visual(&visual, mutate_module_instances,
                                               &mutation, 65536);
    require(mutation.changed != 0, "Crypt module had no geometry to mutate");
    require(all ? walked == dh2::scene_payload::Error::ok
                : walked == dh2::scene_payload::Error::walk_stopped,
            "unexpected scene walk result during mutation");
    dh2_world_free(&source);
    return mutation.changed;
}

} // namespace

int main(int argc, char** argv) {
    using namespace dh2::crypt_module_bounds_registry_v1;
    require(argc == 2, "usage: crypt_module_bounds_registry_v1 <worlds-dir>");
    const std::string worlds = argv[1];
    const auto mlx = read_file(worlds + "/x07_crypt_backup.mlx");
    const auto bdae = read_file(worlds + "/crypt.bdae");

    constexpr const char* expected_names[8] = {
        "cemetery_entrance_0", "straight_ns_1", "corner_sw_2", "corner_ne_3",
        "t_sew_4", "deadend_e_5", "corner_nw_6", "straight_c_ns_7"};
    constexpr const char* expected_roots[8] = {
        "_module_cemetery_entrance-node", "_module_straight_ns-node",
        "_module_corner_sw-node", "_module_corner_ne-node", "_module_t_sew-node",
        "_module_deadend_e-node", "_module_corner_nw-node",
        "_module_straight_c_ns-node"};
    constexpr float expected_origins[8][3] = {
        {0, 0, 0}, {0, 4800, 0}, {0, 9600, 0}, {-4800, 9600, 0},
        {-4800, 14400, 0}, {-9600, 14400, 0}, {0, 14400, 0}, {0, 19200, 0}};
    constexpr float expected_bounds[8][6] = {
        {-3198.84f,-2400,-403.951f, 2402.06f,2422.48f,4800},
        {-2400,2354.77f,-29.2846f, 2400,7208.83f,4800},
        {-2453.29f,7151.06f,-11.3556f, 2400,12000,4800},
        {-7200,7197.09f,-20.051f, -2399.49f,12000.5f,4800},
        {-7243.41f,11951.5f,-28.704f, -2399.73f,16800,4800},
        {-12004.6f,12000,-82.744f, -7169.71f,16800,4800},
        {-2453.29f,12000,-28.704f, 2400,16800.6f,4800},
        {-2400,16752.6f,-107.693f, 3513.81f,25525.1f,4800}};
    constexpr std::uint32_t expected_nodes[8] = {32,11,15,18,12,7,12,39};
    constexpr std::uint32_t expected_geometry[8] = {29,9,12,14,12,6,10,35};

    Owner owner{};
    Result result{};
    auto status = build_from_assets("GOTHICUS_CRYPT_01",
        "data/scene/x07_crypt_backup.mlx", mlx.data(), mlx.size(),
        bdae.data(), bdae.size(), &owner, &result);
    require(status == Status::complete, "packaged Crypt registry build failed");
    require(owner.modules.size() == 8 && result.module_count == 8,
            "Crypt module count differs from eight");
    require(owner.catalogue_path == "data/3d/modules/crypt/crypt.bdae",
            "source catalogue path differs");

    std::uint64_t scene_nodes = 0, geometry = 0, draw_buffers = 0;
    for (std::size_t i = 0; i < owner.modules.size(); ++i) {
        const auto& entry = owner.modules[i];
        require(entry.module_index == i && entry.module_name == expected_names[i] &&
                entry.root_id == expected_roots[i],
                "module order, name, or selected root changed");
        require(entry.scene_nodes == expected_nodes[i] &&
                entry.geometry_instances == expected_geometry[i] &&
                entry.draw_buffers != 0 && entry.ignored_non_geometry_instances == 0,
                "module root geometry statistics changed or include ignored instances");
        for (unsigned axis = 0; axis != 3; ++axis) {
            require(near(entry.origin[axis], expected_origins[i][axis]),
                    "module world origin changed");
            require(std::isfinite(entry.bounds.minimum[axis]) &&
                    std::isfinite(entry.bounds.maximum[axis]) &&
                    entry.bounds.minimum[axis] < entry.bounds.maximum[axis],
                    "module bounds are non-finite or empty");
            require(near(entry.bounds.minimum[axis], expected_bounds[i][axis]) &&
                    near(entry.bounds.maximum[axis], expected_bounds[i][axis + 3]),
                    "module root bounds differ from packaged Crypt source");
        }
        scene_nodes += entry.scene_nodes;
        geometry += entry.geometry_instances;
        draw_buffers += entry.draw_buffers;
    }
    require(scene_nodes == result.scene_nodes && geometry == result.geometry_instances &&
            draw_buffers == result.draw_buffers,
            "aggregate statistics differ from per-module entries");

    // A rotated source module is unsupported. Failed builds leave the prior
    // owner intact and identify the module that could not be bounded.
    std::string rotated_text(mlx.begin(), mlx.end());
    const auto module_marker = rotated_text.find("name=\"cemetery_entrance_0\"");
    require(module_marker != std::string::npos, "first Crypt module marker missing");
    const auto rotation = rotated_text.find("rotation=\"0.0,0.0,0.0\"", module_marker);
    require(rotation != std::string::npos, "first Crypt module rotation field missing");
    rotated_text.replace(rotation, std::string("rotation=\"0.0,0.0,0.0\"").size(),
                         "rotation=\"1.0,0.0,0.0\"");
    const auto rotated = std::vector<std::uint8_t>(rotated_text.begin(), rotated_text.end());
    Result failure{};
    status = build_from_assets("GOTHICUS_CRYPT_01", "data/scene/x07_crypt_backup.mlx",
        rotated.data(), rotated.size(), bdae.data(), bdae.size(), &owner, &failure);
    require(status == Status::unsupported_root && failure.failed_module_index == 0,
            "unsupported module rotation did not fail closed");
    require_owner_preserved(owner, "unsupported-root failure replaced the previous owner");

    // Change one instance type to a non-geometry type while leaving geometry
    // in the root. The registry must reject the partial approximation.
    auto mixed_bdae = bdae;
    const auto mixed_count = change_first_module_geometry(mixed_bdae, mlx, false);
    require(mixed_count == 1, "mixed non-geometry mutation changed an unexpected count");
    status = build_from_assets("GOTHICUS_CRYPT_01", "data/scene/x07_crypt_backup.mlx",
        mlx.data(), mlx.size(), mixed_bdae.data(), mixed_bdae.size(), &owner, &failure);
    require(status == Status::non_geometry_instance && failure.failed_module_index == 0,
            "non-geometry root instance did not fail closed");
    require_owner_preserved(owner, "non-geometry failure replaced the previous owner");

    // Removing every geometry instance makes the selected root empty; that
    // path must also be rejected instead of producing a zero/default box.
    auto empty_bdae = bdae;
    require(change_first_module_geometry(empty_bdae, mlx, true) == 29,
            "empty-root mutation did not cover all first-module geometry");
    status = build_from_assets("GOTHICUS_CRYPT_01", "data/scene/x07_crypt_backup.mlx",
        mlx.data(), mlx.size(), empty_bdae.data(), empty_bdae.size(), &owner, &failure);
    require(status == Status::empty_root && failure.failed_module_index == 0,
            "empty root did not fail closed");
    require_owner_preserved(owner, "empty-root failure replaced the previous owner");

    // Source/BRES parser failures are bounded and do not retire the last good
    // owner. Keep these compact to avoid broad malformed-file fuzzing here.
    status = build_from_assets("GOTHICUS_CRYPT_01", "data/scene/x07_crypt_backup.mlx",
        mlx.data(), 8, bdae.data(), bdae.size(), &owner, &failure);
    require(status == Status::import_failure, "truncated MLX was not rejected");
    require_owner_preserved(owner, "MLX failure replaced the previous owner");
    status = build_from_assets("GOTHICUS_CRYPT_01", "data/scene/x07_crypt_backup.mlx",
        mlx.data(), mlx.size(), bdae.data(), 8, &owner, &failure);
    require(status == Status::catalogue_failure, "truncated BRES was not rejected");
    require_owner_preserved(owner, "BRES failure replaced the previous owner");

    std::printf("{\"validation\":\"PASS\",\"modules\":%u,\"scene_nodes\":%llu,"
                "\"geometry_instances\":%llu,\"draw_buffers\":%llu,"
                "\"ignored_non_geometry\":0,\"unsupported_transform\":\"rejected\","
                "\"mixed_non_geometry\":\"rejected\",\"empty_root\":\"rejected\"}\n",
                result.module_count,
                static_cast<unsigned long long>(result.scene_nodes),
                static_cast<unsigned long long>(result.geometry_instances),
                static_cast<unsigned long long>(result.draw_buffers));
    for (const auto& entry : owner.modules) {
        std::printf("module=%u name=%s root=%s origin=%.3f,%.3f,%.3f "
                    "bounds=%.3f,%.3f,%.3f..%.3f,%.3f,%.3f "
                    "nodes=%u geometry=%u buffers=%u ignored=%u\n",
                    entry.module_index, entry.module_name.c_str(), entry.root_id.c_str(),
                    entry.origin[0], entry.origin[1], entry.origin[2],
                    entry.bounds.minimum[0], entry.bounds.minimum[1],
                    entry.bounds.minimum[2], entry.bounds.maximum[0],
                    entry.bounds.maximum[1], entry.bounds.maximum[2],
                    entry.scene_nodes, entry.geometry_instances, entry.draw_buffers,
                    entry.ignored_non_geometry_instances);
    }
    return 0;
}
