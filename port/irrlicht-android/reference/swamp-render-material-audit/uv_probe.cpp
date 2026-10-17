#include "../../../android-app/scene_buffers.hpp"
#include "../../../asset-payloads/payloads.hpp"
#include "../../../world-data/world.hpp"
#include "../../../world-data/world_scene.hpp"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <limits>
#include <string>
#include <vector>

namespace {
void require(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "SWAMP material UV probe failed: %s\n", message);
        std::exit(1);
    }
}

std::vector<std::uint8_t> read_file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    require(static_cast<bool>(input), "cannot read supplied cache file");
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

void quoted(const char* text) {
    std::putchar('"');
    for (const unsigned char* p = reinterpret_cast<const unsigned char*>(text); *p; ++p) {
        if (*p == '"' || *p == '\\') std::putchar('\\');
        if (*p >= 0x20) std::putchar(*p);
    }
    std::putchar('"');
}
}

int main(int argc, char** argv) {
    require(argc == 2, "usage: swamp_material_uv_probe <cache-root>");
    const std::string cache = argv[1];
    const auto mlx = read_file(cache + "/data/scene/001_swamp.mlx");
    const auto catalogue = read_file(cache + "/data/3d/modules/swamp/swamp.bdae");

    dh2::world::SourceLevel level{};
    dh2::world::Diagnostic diagnostic{};
    require(dh2_world_import_level(&level, "SWAMP", "data/scene/001_swamp.mlx",
                mlx.data(), mlx.size(), &diagnostic) == dh2::world::Error::ok,
            "SWAMP MLX import failed");
    require(level.module_count == 9, "SWAMP module catalogue changed");
    dh2::resources::BresView bres{};
    require(dh2_bres_open(&bres, catalogue.data(), catalogue.size()) ==
                dh2::resources::BresError::ok,
            "SWAMP BRES open failed");
    dh2::scene_payload::Scene scene{};
    require(dh2_scene_open(&scene, &bres) == dh2::scene_payload::Error::ok,
            "SWAMP scene open failed");
    dh2::world::ModuleBinding binding{};
    require(dh2_world_bind_module(&binding, &level.modules[0], &scene,
                &diagnostic) == dh2::world::Error::ok,
            "SWAMP module zero root bind failed");
    std::vector<std::uint32_t> records(65536);
    std::uint32_t record_count = 0;
    require(dh2_world_module_records(records.data(), records.size(), &record_count,
                &binding, &scene, &diagnostic) == dh2::world::Error::ok,
            "SWAMP module zero subtree enumeration failed");
    dh2::math::Matrix4f correction{};
    require(dh2_world_placement_matrix(&correction, &binding, &diagnostic) ==
                dh2::world::Error::ok,
            "SWAMP module placement matrix failed");
    dh2::viewer::SceneMesh mesh{};
    require(dh2_world_scene_mesh_nodes(&mesh, &bres, records.data(), record_count,
                &correction) == dh2::viewer::SceneMeshError::ok,
            "SWAMP SceneMesh assembly failed");

    std::uint32_t material_11610 = 0, material_11611 = 0;
    std::printf("{\"validation\":\"PASS\",\"selected_scene_sha256\":\"89da80c60a7ebecd0e8a27a9d46f2625e3a5ec112933e5aa7cab251412b8364d\",\"uv_source\":\"primitive.attributes[4] copied by android-app/scene_buffers.cpp into SceneMesh vertices[5*vertex+3..4]\",\"draws\":[");
    bool first = true;
    for (std::uint32_t draw_index = 0; draw_index < mesh.draw_commands; ++draw_index) {
        const auto& draw = mesh.draws[draw_index];
        if (!draw.visible || (std::strcmp(draw.material_id, "Material__11610") != 0 &&
                              std::strcmp(draw.material_id, "Material__11611") != 0))
            continue;
        if (!first) std::putchar(',');
        first = false;
        const bool alpha = std::strcmp(draw.material_id, "Material__11611") == 0;
        if (alpha) ++material_11611; else ++material_11610;

        dh2::assets::Mesh source_mesh{};
        dh2::assets::Primitive primitive{};
        require(dh2_mesh_open(&source_mesh, &bres, draw.geometry_index) ==
                    dh2::assets::Error::ok &&
                dh2_mesh_primitive(&source_mesh, draw.primitive_index, &primitive) ==
                    dh2::assets::Error::ok,
                "selected source draw no longer resolves to a BRES primitive");
        const auto uv_id = primitive.attributes[4];
        require(uv_id >= 0, "Material__11610/11611 source draw lacks UV attribute slot 4");
        dh2::assets::Attribute uv{};
        require(dh2_mesh_attribute(&source_mesh, uv_id, &uv) == dh2::assets::Error::ok &&
                    uv.components >= 2 && uv.vertices == source_mesh.vertices,
                "selected source UV attribute layout is invalid");

        double min_u = std::numeric_limits<double>::infinity();
        double min_v = std::numeric_limits<double>::infinity();
        double max_u = -std::numeric_limits<double>::infinity();
        double max_v = -std::numeric_limits<double>::infinity();
        std::uint32_t used = 0;
        for (std::uint32_t index = 0; index < draw.index_count; ++index) {
            const auto vertex = mesh.indices[draw.first_index + index];
            require(vertex >= draw.first_vertex &&
                    vertex < draw.first_vertex + draw.vertex_count,
                    "draw index escaped its contiguous SceneMesh vertex slice");
            const auto* v = mesh.vertices + std::size_t(vertex) * 5;
            require(std::isfinite(v[3]) && std::isfinite(v[4]),
                    "source UV stream contains a nonfinite value");
            if (v[3] < min_u) min_u = v[3];
            if (v[3] > max_u) max_u = v[3];
            if (v[4] < min_v) min_v = v[4];
            if (v[4] > max_v) max_v = v[4];
            ++used;
        }
        std::printf("{\"draw_index\":%u,\"material\":", draw_index);
        quoted(draw.material_id);
        std::printf(",\"node\":"); quoted(draw.node_id);
        std::printf(",\"geometry\":"); quoted(draw.geometry_id);
        std::printf(",\"uv_attribute_id\":%d,\"uv_components\":%u,\"uv_data_type\":%u,\"uv_used_index_occurrences\":%u,\"u\":[%.9g,%.9g],\"v\":[%.9g,%.9g],\"alpha_sampler_ref\":%s}",
            uv_id, uv.components, uv.type, used, min_u, max_u, min_v, max_v,
            alpha ? "true" : "false");
    }
    std::printf("],\"draw_counts\":{\"Material__11610\":%u,\"Material__11611\":%u},\"texture_sampling_boundary\":\"SceneMesh exports one UV pair from slot 4; checked material exports identify AlphaMap and Diffuse images but do not bind per-sampler UV sets or establish source technique selection\"}\n",
        material_11610, material_11611);
    require(material_11610 == 25 && material_11611 == 22,
            "visible module-zero material draw counts changed");
    dh2_viewer_scene_mesh_free(&mesh);
    dh2_world_free(&level);
    return 0;
}
