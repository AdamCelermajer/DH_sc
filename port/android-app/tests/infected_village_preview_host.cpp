#include "../infected_village_scene.hpp"
#include "../infected_village_render_policy.hpp"

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
void require(bool condition, const char* message) {
    if (condition) return;
    std::fprintf(stderr, "Infected Village preview host check failed: %s\n", message);
    std::exit(1);
}

std::vector<std::uint8_t> read_file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    require(static_cast<bool>(input), path.c_str());
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

bool path_is(const char* value, const char* expected) {
    return value && std::string(value) == expected;
}
}

int main(int argc, char** argv) {
    require(argc == 2, "usage: infected_village_preview_host <cache-root>");
    const std::string cache = argv[1];
    auto mlx = read_file(cache + "/data/scene/005_infectedvillage.mlx");
    auto catalogue = read_file(cache + "/data/3d/modules/infectedvillage/infectedvillage.bdae");
    auto mgp0 = read_file(cache + "/data/3d/modules/infectedvillage/mgp/infected01.mgp");
    auto mvp0 = read_file(cache + "/data/3d/modules/infectedvillage/mvp/infected01.mvp");
    auto mgp1 = read_file(cache + "/data/3d/modules/infectedvillage/mgp/infected02.mgp");
    auto mvp1 = read_file(cache + "/data/3d/modules/infectedvillage/mvp/infected02.mvp");

    dh2::infectedpreview::SourceFiles files{
        mlx.data(), mlx.size(), catalogue.data(), catalogue.size(),
        {mgp0.data(), mgp1.data()}, {mgp0.size(), mgp1.size()},
        {mvp0.data(), mvp1.data()}, {mvp0.size(), mvp1.size()}};
    dh2::infectedpreview::Preview preview{};
    dh2::infectedpreview::Diagnostic diagnostic{};
    require(dh2::infectedpreview::load(&preview, &files, &diagnostic) ==
            dh2::infectedpreview::Error::ok, diagnostic.message);
    require(preview.level.module_count == 2 && preview.level.entity_count == 50,
            "must import both MGP/MVP sources (50 module records)");

    const float origins[2][3] = {{-3448.5f, 3000.0f, 0.0f},
                                  {2551.5f, 3000.0f, 0.0f}};
    const char* roots[2] = {"_module_infectedvillage_01-node",
                            "_module_infectedvillage_02-node"};
    std::uint32_t total_draws = 0;
    std::uint32_t total_vertices = 0;
    std::uint32_t total_indices = 0;
    std::uint32_t shown_draws = 0, omitted_roots = 0, omitted_floors = 0;
    std::uint32_t diffuse_refs = 0, alpha_refs = 0, specular_refs = 0;
    const char* expected_diffuse = "q:/data/iphone/3d/textures/env_infectedvillage.tga";
    const char* expected_alpha = "q:/data/iphone/3d/textures/pvr2_env_infectedvillage_alpha.tga";
    const char* expected_specular = "q:/data/iphone/3d/textures/env_infectedvillage_spec.tga";
    for (std::uint32_t i = 0; i < 2; ++i) {
        auto& module = preview.level.modules[i];
        require(path_is(module.catalogue_node_id, roots[i]), "catalogue root order changed");
        for (unsigned axis = 0; axis < 3; ++axis)
            require(std::fabs(module.record.local.position[axis] - origins[i][axis]) < 0.001f,
                    "authored module placement changed");
        require(module.mgp_loaded && module.mvp_loaded, "module object sources not imported");
        const auto& mesh = preview.modules[i];
        require(mesh.draw_commands > 0 && mesh.vertex_count > 0 && mesh.index_count > 0,
                "module root emitted no drawable geometry");
        require(path_is(mesh.draws[0].node_id, roots[i]) &&
                path_is(mesh.draws[0].material_id, "ColorMaterial") &&
                dh2::infectedpreview::omit_unresolved_root_guide(
                    mesh.draws[0].node_id, mesh.draws[0].material_id),
                "the exact unresolved root-guide draw identities changed");
        require(!dh2::infectedpreview::omit_unresolved_root_guide(
                    "_module_infectedvillage_03-node", "ColorMaterial") &&
                !dh2::infectedpreview::omit_unresolved_root_guide(
                    roots[i], "Material__2341") &&
                !dh2::infectedpreview::omit_unresolved_root_guide(
                    "_colbox-node", "ColorMaterial") &&
                !dh2::infectedpreview::omit_unresolved_root_guide(
                    "TemplateDefs-node", "ColorMaterial"),
                "root-guide policy broadened beyond the two exact node/material pairs");
        const std::uint32_t floor_index = i == 0 ? 3 : 2;
        const auto& floor = mesh.draws[floor_index];
        const char* floor_node = i == 0 ? "_floor_infectedvillage_01-node_PIVOT"
                                       : "_floor_infectedvillage_02-node_PIVOT";
        require(path_is(floor.node_id, floor_node) &&
                path_is(floor.material_id, "Standard_8") && floor.visible &&
                floor.texture_count == 0 &&
                dh2::infectedpreview::omit_unresolved_floor_fallback(
                    floor.node_id, floor.material_id),
                "the source-visible untextured floor fallback identity/binding changed");
        require(!dh2::infectedpreview::omit_unresolved_floor_fallback(
                    "_floor_infectedvillage_03-node", "Standard_8") &&
                !dh2::infectedpreview::omit_unresolved_floor_fallback(
                    floor_node, "Standard_7") &&
                !dh2::infectedpreview::omit_unresolved_floor_fallback(
                    "_some_other_node", "Standard_8"),
                "floor fallback policy broadened beyond the exact two node/material pairs");
        float minimum[3] = {std::numeric_limits<float>::infinity(),
                            std::numeric_limits<float>::infinity(),
                            std::numeric_limits<float>::infinity()};
        float maximum[3] = {-std::numeric_limits<float>::infinity(),
                            -std::numeric_limits<float>::infinity(),
                            -std::numeric_limits<float>::infinity()};
        for (std::uint32_t v = 0; v < mesh.vertex_count; ++v)
            for (unsigned axis = 0; axis < 3; ++axis) {
                const float coordinate = mesh.vertices[5U * v + axis];
                require(std::isfinite(coordinate), "nonfinite placed vertex");
                if (coordinate < minimum[axis]) minimum[axis] = coordinate;
                if (coordinate > maximum[axis]) maximum[axis] = coordinate;
            }
        const float mesh_center[3] = {
            (minimum[0] + maximum[0]) * 0.5f,
            (minimum[1] + maximum[1]) * 0.5f,
            (minimum[2] + maximum[2]) * 0.5f};
        require(std::fabs(mesh_center[0] - origins[i][0]) < 500.0f,
                "assembled module bounds no longer reflect the authored MLX placement");
        for (std::uint32_t draw_index = 0; draw_index < mesh.draw_commands; ++draw_index) {
            const auto& draw = mesh.draws[draw_index];
            require(draw.visible != 0, "module draw unexpectedly invisible");
            require(draw.first_vertex + draw.vertex_count <= mesh.vertex_count,
                    "draw vertex range exceeds flattened module mesh");
            require(draw.first_index + draw.index_count <= mesh.index_count,
                    "draw index range exceeds flattened module mesh");
            float draw_min[3] = {std::numeric_limits<float>::infinity(),
                                 std::numeric_limits<float>::infinity(),
                                 std::numeric_limits<float>::infinity()};
            float draw_max[3] = {-std::numeric_limits<float>::infinity(),
                                 -std::numeric_limits<float>::infinity(),
                                 -std::numeric_limits<float>::infinity()};
            for (std::uint32_t v = 0; v < draw.vertex_count; ++v)
                for (unsigned axis = 0; axis < 3; ++axis) {
                    const float coordinate = mesh.vertices[5U * (draw.first_vertex + v) + axis];
                    draw_min[axis] = std::min(draw_min[axis], coordinate);
                    draw_max[axis] = std::max(draw_max[axis], coordinate);
                }
            std::printf("  draw=%u node=%s geometry=%s material=%s bounds=(%.1f..%.1f,%.1f..%.1f,%.1f..%.1f) indices=%u\n",
                draw_index, draw.node_id, draw.geometry_id, draw.material_id,
                draw_min[0], draw_max[0], draw_min[1], draw_max[1],
                draw_min[2], draw_max[2], draw.index_count);
            for (std::uint32_t r = 0; r < draw.texture_count; ++r) {
                const auto& ref = mesh.texture_references[draw.first_texture + r];
                std::printf("    sampler=%s image=%s name=%s index=%d path=%s\n",
                    ref.parameter_id, ref.image_id, ref.image_name, ref.image_index,
                    ref.source_path);
                if (ref.image_index >= 0 && std::string(ref.parameter_id) == "Diffuse" &&
                    path_is(ref.source_path, expected_diffuse)) ++diffuse_refs;
                if (ref.image_index >= 0 && std::string(ref.parameter_id) == "AlphaMap" &&
                    path_is(ref.source_path, expected_alpha)) ++alpha_refs;
                if (ref.image_index >= 0 && std::string(ref.parameter_id) == "Specular" &&
                    path_is(ref.source_path, expected_specular)) ++specular_refs;
            }
            if (dh2::infectedpreview::omit_unresolved_root_guide(
                    draw.node_id, draw.material_id)) ++omitted_roots;
            else if (dh2::infectedpreview::omit_unresolved_floor_fallback(
                         draw.node_id, draw.material_id)) ++omitted_floors;
            else ++shown_draws;
        }
        total_draws += mesh.draw_commands;
        total_vertices += mesh.vertex_count;
        total_indices += mesh.index_count;
        std::printf("module=%u root=%s source_records=\"%s\"+\"%s\" draws=%u vertices=%u indices=%u placement=(%.1f,%.1f,%.1f) world_bounds=(%.1f..%.1f,%.1f..%.1f,%.1f..%.1f) bound_center=(%.1f,%.1f,%.1f)\n",
            i, roots[i], module.cache_mgp, module.cache_mvp, mesh.draw_commands,
            mesh.vertex_count, mesh.index_count, module.record.local.position[0],
            module.record.local.position[1], module.record.local.position[2],
            minimum[0], maximum[0], minimum[1], maximum[1], minimum[2], maximum[2],
            mesh_center[0], mesh_center[1], mesh_center[2]);
    }
    require(diffuse_refs > 0 && alpha_refs > 0 && specular_refs > 0,
            "checked BDAE material sampler references changed");
    require(total_draws == 20 && shown_draws == 16 &&
            omitted_roots == 2 && omitted_floors == 2,
            "preview visibility policy no longer excludes exactly 2 guides and 2 untextured floors");
    require(dh2::infectedpreview::load(&preview, &files, &diagnostic) ==
                dh2::infectedpreview::Error::ok && preview.ready &&
                preview.level.module_count == 2,
            "a second load must replace, not leak or invalidate, a populated Preview");
    std::printf("pass=true static_only=true modules=2 imported_module_records=%u draws=%u vertices=%u indices=%u diffuse_refs=%u alpha_refs=%u specular_refs=%u gameplay_activated=false\n",
        preview.level.entity_count, total_draws, total_vertices, total_indices,
        diffuse_refs, alpha_refs, specular_refs);
    dh2::infectedpreview::free(&preview);
    return 0;
}
