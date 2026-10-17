#include "../../world-data/world.hpp"
#include "../../world-data/world_scene.hpp"
#include "../../asset-payloads/payloads.hpp"
#include "../../scene-draw/draw.hpp"
#include "../scene_buffers.hpp"
#include "../swamp_render_policy.hpp"
#include "../../irrlicht-android/swamp-smoke/alpha_map_policy.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <set>
#include <string>
#include <vector>

namespace {
void require(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "SWAMP scene regression failed: %s\n", message);
        std::exit(1);
    }
}

std::vector<std::uint8_t> read_file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    require(static_cast<bool>(input), "cannot open input file");
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

bool dump_visibility(const dh2::draw::Command* command, void*) {
    if (!command->visible ||
        (command->node_id && std::strcmp(command->node_id,
            "_module_obj_4of4_brdwalk_sw_00-node") == 0))
        std::printf("  visibility: %u node=%s material=%s\n", command->visible,
            command->node_id, command->material_id);
    return true;
}

}

int main(int argc, char** argv) {
    require(argc == 2 || (argc == 3 && std::strcmp(argv[2], "--dump-draws") == 0),
            "usage: swamp_scene <cache-root> [--dump-draws]");
    const bool dump_draws = argc == 3;
    const std::string cache = argv[1];
        const auto mlx_bytes = read_file(cache + "/data/scene/001_swamp.mlx");
        dh2::world::SourceLevel level{};
        dh2::world::Diagnostic diagnostic{};
        auto result = dh2_world_import_level(&level, "SWAMP", "data/scene/001_swamp.mlx",
            mlx_bytes.data(), mlx_bytes.size(), &diagnostic);
        require(result == dh2::world::Error::ok, diagnostic.message);
        require(level.module_count == 9, "SWAMP MLX module count changed");
        const auto& module = level.modules[0];
        require(std::strcmp(module.record.name, "obj_4of4_brdwalk_sw_00_0") == 0,
                "module zero identity changed");
        require(std::strcmp(module.cache_dae, "data/3d/modules/swamp/swamp.bdae") == 0,
                "module zero DAE path changed");
        require(std::strcmp(module.catalogue_node_id,
                            "_module_obj_4of4_brdwalk_sw_00-node") == 0,
                "module zero catalogue node changed");

        const auto bres_bytes = read_file(cache + "/" + module.cache_dae);
        const auto bres_before = bres_bytes;
        dh2::resources::BresView bres{};
        require(dh2_bres_open(&bres, bres_bytes.data(), bres_bytes.size()) ==
                    dh2::resources::BresError::ok,
                "swamp catalogue BRES parse failed");
        const auto material_count = dh2_bres_library_count(
            &bres, dh2::resources::Library::material);
        std::vector<dh2::irrlicht_swamp::AlphaMapMode> material_alpha_modes(
            material_count, dh2::irrlicht_swamp::AlphaMapMode::unsupported);
        std::uint32_t alpha_selector_materials = 0;
        for (std::uint32_t i = 0; i < material_count; ++i) {
            dh2::materials::Material material{};
            require(dh2_material_record(&material, &bres,
                        static_cast<std::int32_t>(i)) == dh2::materials::Error::ok,
                    "scene material inventory failed checked BRES decoding");
            std::vector<dh2::scene_materials::CurrentTechnique> selectors;
            std::string selector_error;
            require(dh2::scene_materials::material_current_techniques(
                        material, selectors, selector_error) ==
                        dh2::scene_materials::TechniqueSelectorError::ok,
                    "scene CurrentTechnique material failed checked decoding");
            const auto selection = dh2::irrlicht_swamp::resolve_alpha_map_selection(selectors);
            material_alpha_modes[i] = selection.mode;
            if (!selectors.empty()) ++alpha_selector_materials;
            if (std::strcmp(material.id, "Material__11610") == 0)
                require(selection.mode == dh2::irrlicht_swamp::AlphaMapMode::no_alpha_variant,
                        "Material__11610 GLES/GLES2 selector pair must remain opaque");
            if (std::strcmp(material.id, "Material__11611") == 0)
                require(selection.mode == dh2::irrlicht_swamp::AlphaMapMode::source_al_fractional,
                        "Material__11611 source CurrentTechnique must derive AL mode");
        }
        require(alpha_selector_materials == 2,
                "source SWAMP BRES selector material inventory changed");
        dh2::scene_payload::Scene scene{};
        require(dh2_scene_open(&scene, &bres) == dh2::scene_payload::Error::ok,
                "swamp catalogue scene parse failed");
        dh2::draw::Stats scene_stats{};
        require(dh2_static_scene_draws(&scene_stats, &bres, dump_visibility, nullptr,
                    20000, 8192) == dh2::draw::Error::ok,
                "raw draw visibility pass failed");

        dh2::world::ModuleBinding binding{};
        result = dh2_world_bind_module(&binding, &module, &scene, &diagnostic);
        require(result == dh2::world::Error::ok, diagnostic.message);
        require(binding.catalogue_origin[0] == 52000.0f &&
                binding.catalogue_origin[1] == 3000.0f &&
                binding.catalogue_origin[2] == 0.0f,
                "module zero catalogue origin changed");
        require(binding.placement_delta[0] == -52000.0f &&
                binding.placement_delta[1] == -3000.0f &&
                binding.placement_delta[2] == 0.0f,
                "module zero MLX placement correction changed");

        std::vector<std::uint32_t> records(65536);
        std::uint32_t record_count = 0;
        result = dh2_world_module_records(records.data(), records.size(), &record_count,
                                           &binding, &scene, &diagnostic);
        require(result == dh2::world::Error::ok, diagnostic.message);
        require(record_count > 0 && record_count <= records.size(),
                "invalid selected module subtree size");

        dh2::math::Matrix4f correction{};
        result = dh2_world_placement_matrix(&correction, &binding, &diagnostic);
        require(result == dh2::world::Error::ok, diagnostic.message);
        dh2::viewer::SceneMesh placed{};
        dh2::viewer::SceneMesh uncorrected{};
        dh2::viewer::SceneMesh root_prefix{};
        auto mesh_result = dh2_world_scene_mesh_nodes(&placed, &bres, records.data(),
            record_count, &correction);
        require(mesh_result == dh2::viewer::SceneMeshError::ok,
                "selected module mesh assembly failed");
        // A null correction assembles the exact same selected subtree in the
        // catalogue's uncorrected coordinate space. The legacy prefix API is
        // also exercised below; its prefix selects the root's own draw only,
        // because descendant node IDs are not prefixed by the root ID.
        mesh_result = dh2_world_scene_mesh_nodes(&uncorrected, &bres, records.data(),
            record_count, nullptr);
        require(mesh_result == dh2::viewer::SceneMeshError::ok,
                "uncorrected selected module mesh assembly failed");
        mesh_result = dh2_world_scene_mesh(&root_prefix, &bres, module.catalogue_node_id);
        require(mesh_result == dh2::viewer::SceneMeshError::ok,
                "root prefix-filtered mesh assembly failed");
        require(placed.draw_commands > 0 && placed.vertex_count > 8192 &&
                placed.vertex_count <= 65535 && placed.index_count <= 1000000,
                "module zero geometry counts exceed expected production bounds");
        if (placed.draw_commands != uncorrected.draw_commands ||
            placed.vertex_count != uncorrected.vertex_count ||
            placed.index_count != uncorrected.index_count) {
            std::fprintf(stderr, "corrected=%u/%u/%u, uncorrected=%u/%u/%u\n",
                placed.draw_commands, placed.vertex_count, placed.index_count,
                uncorrected.draw_commands, uncorrected.vertex_count, uncorrected.index_count);
        }
        require(placed.draw_commands == uncorrected.draw_commands &&
                placed.vertex_count == uncorrected.vertex_count &&
                placed.index_count == uncorrected.index_count,
                "corrected and uncorrected selected-subtree counts differ");
        require(root_prefix.draw_commands == 1 && root_prefix.vertex_count == 24 &&
                root_prefix.index_count == 36,
                "module root prefix no longer selects its checked root draw");
        require(placed.draws && placed.draw_capacity >= placed.draw_commands,
                "scene draw descriptors were not exported");
        std::uint32_t next_vertex = 0, next_index = 0, texture_refs = 0;
        std::uint32_t visible_source_draws = 0, omitted_diagnostic_draws = 0;
        std::uint32_t alpha_map_parameter_refs = 0, alpha_map_refs = 0;
        std::uint32_t alpha_selector_draws = 0;
        std::uint32_t alpha_al_draws = 0, alpha_at_draws = 0;
        std::uint32_t alpha_selector_unresolved_draws = 0;
        std::uint32_t alpha_material_draws = 0;
        std::uint32_t additive_source_draws = 0;
        std::set<std::string> sampler_paths, sampler_parameters, material_ids;
        std::uint32_t resolved_diffuse_refs = 0;
        const auto bres_begin = reinterpret_cast<std::uintptr_t>(bres_bytes.data());
        const auto bres_end = bres_begin + bres_bytes.size();
        for (std::uint32_t i = 0; i < placed.draw_commands; ++i) {
            const auto& draw = placed.draws[i];
            if (dh2::viewer::swamp_material_uses_additive_one_one(draw.material_id)) {
                require((i == 13 || i == 24) && draw.visible && draw.index_count == 12 &&
                        std::strcmp(draw.material_id, "Material__11598") == 0,
                        "additive pass policy escaped the two source-verified overlay draws");
                require(!dh2::viewer::swamp_draw_writes_depth(draw.material_id, false),
                        "source additive overlay incorrectly writes depth");
                ++additive_source_draws;
            }
            require(dh2::viewer::swamp_draw_writes_depth("ordinary-material", false) &&
                    dh2::viewer::swamp_draw_writes_depth("ordinary-material", true),
                    "ordinary opaque/cutout material depth policy changed");
            if (dh2::viewer::omit_unresolved_swamp_draw(draw.node_id, draw.material_id)) {
                require(std::strcmp(draw.geometry_id, "_module_obj_4of4_brdwalk_sw_00-mesh") == 0 &&
                        draw.index_count == 36 && draw.visible,
                        "unresolved ColorMaterial policy no longer targets the verified source root draw");
                ++omitted_diagnostic_draws;
            }
            require(draw.first_vertex == next_vertex && draw.vertex_count > 0 &&
                    draw.first_index == next_index && draw.index_count > 0,
                    "per-command vertex/index ranges are not contiguous");
            require(draw.visible <= 1, "scene draw visibility is not boolean");
            visible_source_draws += draw.visible ? 1u : 0u;
            require(draw.first_vertex + draw.vertex_count <= placed.vertex_count &&
                    draw.first_index + draw.index_count <= placed.index_count &&
                    draw.node_id[0] && draw.geometry_id[0] && draw.material_id[0],
                    "draw metadata is missing identity or exceeds combined buffers");
            material_ids.insert(draw.material_id);
            require(draw.first_texture == texture_refs &&
                    draw.first_texture + draw.texture_count <= placed.texture_reference_count,
                    "material sampler ranges are not contiguous");
            const char* owned_strings[] = {draw.node_id, draw.geometry_id,
                draw.material_id, draw.material_name, draw.external_effect_file, draw.effect_url};
            for (const char* value : owned_strings) {
                const auto address = reinterpret_cast<std::uintptr_t>(value);
                require(address < bres_begin || address >= bres_end,
                        "draw metadata string still borrows BRES storage");
            }
            for (std::uint32_t j = 0; j < draw.texture_count; ++j) {
                const auto& texture = placed.texture_references[draw.first_texture + j];
                require(texture.parameter_id[0], "sampler parameter identity is absent");
                sampler_parameters.insert(texture.parameter_id);
                if (texture.source_path[0]) sampler_paths.insert(texture.source_path);
                if (std::strstr(texture.parameter_id, "Diffuse") ||
                    std::strstr(texture.parameter_id, "diffuse")) {
                    constexpr char original_prefix[] = "q:/data/iphone/3d/textures/";
                    require(std::strncmp(texture.source_path, original_prefix,
                                sizeof(original_prefix) - 1) == 0,
                            "SWAMP diffuse sampler path prefix changed");
                    const std::string filename = texture.source_path + sizeof(original_prefix) - 1;
                    require(std::strchr(filename.c_str(), '/') == nullptr &&
                            std::strchr(filename.c_str(), '\\') == nullptr,
                            "diffuse sampler is not a texture basename");
                    require(std::ifstream(cache + "/data/3d/textures/" + filename).good(),
                            "source-referenced SWAMP diffuse texture is absent from cache");
                    ++resolved_diffuse_refs;
                }
                if (std::strcmp(texture.parameter_id, "AlphaMap") == 0) {
                    ++alpha_map_parameter_refs;
                    if (texture.image_index >= 0) {
                        require(dh2::irrlicht_swamp::is_swamp_alpha_map_reference(texture),
                                "resolved SWAMP AlphaMap sampler identity/path changed");
                        require(std::ifstream(cache +
                                    "/data/3d/textures/pvr2_env_swamp_alpha.tga").good(),
                                "source-referenced SWAMP AlphaMap texture is absent from cache");
                        ++alpha_map_refs;
                    } else {
                        require(!texture.source_path[0],
                                "unresolved AlphaMap sampler unexpectedly has a source path");
                    }
                }
                const char* sampler_strings[] = {texture.parameter_id, texture.image_id,
                    texture.image_name, texture.source_path};
                for (const char* value : sampler_strings) {
                    const auto address = reinterpret_cast<std::uintptr_t>(value);
                    require(address < bres_begin || address >= bres_end,
                            "sampler metadata still borrows BRES storage");
                }
            }
            const auto* draw_references = placed.texture_references + draw.first_texture;
            const auto alpha_mode = dh2::irrlicht_swamp::alpha_map_mode_for_material(
                draw.material_index, material_alpha_modes);
            const bool uses_alpha_map = dh2::irrlicht_swamp::swamp_draw_uses_alpha_map_mode(
                draw, draw_references, draw.texture_count, alpha_mode);
            if (std::strcmp(draw.material_id, "Material__11611") == 0) {
                require(uses_alpha_map && draw.visible &&
                        alpha_mode == dh2::irrlicht_swamp::AlphaMapMode::source_al_fractional,
                        "Material__11611 source selector no longer maps its visible AlphaMap as AL");
                ++alpha_material_draws;
            } else {
                require(!uses_alpha_map ||
                        alpha_mode == dh2::irrlicht_swamp::AlphaMapMode::source_at_cutout,
                        "alpha treatment must be supported by the material's source selector");
            }
            alpha_selector_draws += uses_alpha_map ? 1U : 0U;
            alpha_al_draws += uses_alpha_map &&
                alpha_mode == dh2::irrlicht_swamp::AlphaMapMode::source_al_fractional ? 1U : 0U;
            alpha_at_draws += uses_alpha_map &&
                alpha_mode == dh2::irrlicht_swamp::AlphaMapMode::source_at_cutout ? 1U : 0U;
            alpha_selector_unresolved_draws +=
                dh2::irrlicht_swamp::swamp_draw_has_alpha_map_pair(
                    draw, draw_references, draw.texture_count) && !uses_alpha_map ? 1U : 0U;
            texture_refs += draw.texture_count;
            next_vertex += draw.vertex_count;
            next_index += draw.index_count;
        }
        require(omitted_diagnostic_draws == 1,
                "unresolved translucent diagnostic policy must select exactly one source draw");
        require(additive_source_draws == 2,
                "source additive state must select exactly the two verified overlay draws");
        require(alpha_map_refs == 22 && alpha_selector_draws == 22 &&
                alpha_al_draws == 22 && alpha_at_draws == 0 &&
                alpha_selector_unresolved_draws == 0 &&
                alpha_material_draws == 22,
                "source CurrentTechnique AL mapping must cover all 22 AlphaMap draws");
        require(visible_source_draws == 54,
                "module zero source-visible draw count changed from the tested baseline");
        require(!dh2::viewer::omit_unresolved_swamp_draw("other-node", "ColorMaterial") &&
                !dh2::viewer::omit_unresolved_swamp_draw(
                    "_module_obj_4of4_brdwalk_sw_00-node", "other-material") &&
                !dh2::viewer::omit_unresolved_swamp_draw(nullptr, "ColorMaterial"),
                "unresolved material policy broadened beyond the verified node/material pair");
        require(next_vertex == placed.vertex_count && next_index == placed.index_count &&
                texture_refs == placed.texture_reference_count && texture_refs > 0,
                "draw metadata does not cover all source geometry/material samplers");
        require(resolved_diffuse_refs > 0,
                "no source-derived SWAMP diffuse sampler resolved to a cache texture");
        for (std::uint32_t root_vertex = 0; root_vertex < root_prefix.vertex_count; ++root_vertex) {
            bool present = false;
            for (std::uint32_t vertex = 0; vertex < uncorrected.vertex_count; ++vertex) {
                bool same = true;
                for (unsigned component = 0; component < 5; ++component)
                    same = same && std::fabs(root_prefix.vertices[root_vertex * 5 + component] -
                        uncorrected.vertices[vertex * 5 + component]) <= 0.00001f;
                present = present || same;
            }
            require(present, "root prefix geometry is absent from complete subtree assembly");
        }

        constexpr float epsilon = 0.002f;
        for (std::uint32_t vertex = 0; vertex < placed.vertex_count; ++vertex) {
            for (unsigned axis = 0; axis < 3; ++axis) {
                const float expected = uncorrected.vertices[vertex * 5 + axis] +
                    binding.placement_delta[axis];
                require(std::isfinite(placed.vertices[vertex * 5 + axis]) &&
                        std::fabs(placed.vertices[vertex * 5 + axis] - expected) <= epsilon,
                        "corrected vertex does not equal catalogue vertex plus MLX delta");
            }
            require(std::fabs(placed.vertices[vertex * 5 + 3] -
                              uncorrected.vertices[vertex * 5 + 3]) <= epsilon &&
                    std::fabs(placed.vertices[vertex * 5 + 4] -
                              uncorrected.vertices[vertex * 5 + 4]) <= epsilon,
                    "placement correction changed texture coordinates");
        }
        require(bres_bytes == bres_before, "BRES input bytes were modified");
        std::printf("SWAMP module 0: %u subtree records, %u source draw commands (%u source-visible; "
                    "%u drawn diagnostics; "
                    "%u unresolved diagnostic draw omitted), %u resolved AlphaMap refs/%u source-selector-derived AL draws (AT=%u unresolved=%u; %u AlphaMap parameter refs total), %u vertices, "
                    "%u indices, %u materials, %u sampler refs; correction=(%.0f,%.0f,%.0f); BRES unchanged\n",
                    record_count, placed.draw_commands, visible_source_draws,
                    visible_source_draws - omitted_diagnostic_draws, omitted_diagnostic_draws,
                    alpha_map_refs, alpha_al_draws, alpha_at_draws,
                    alpha_selector_unresolved_draws, alpha_map_parameter_refs,
                    placed.vertex_count,
                    placed.index_count, static_cast<unsigned>(material_ids.size()),
                    placed.texture_reference_count, binding.placement_delta[0],
                    binding.placement_delta[1], binding.placement_delta[2]);
        for (std::uint32_t i = 0; i < placed.draw_commands; ++i) {
            const auto& draw = placed.draws[i];
            if (dump_draws) {
                float draw_min[3] = {INFINITY, INFINITY, INFINITY};
                float draw_max[3] = {-INFINITY, -INFINITY, -INFINITY};
                float index_min[3] = {INFINITY, INFINITY, INFINITY};
                float index_max[3] = {-INFINITY, -INFINITY, -INFINITY};
                for (std::uint32_t vertex = 0; vertex < draw.vertex_count; ++vertex)
                    for (unsigned axis = 0; axis < 3; ++axis) {
                        const auto value = placed.vertices[(draw.first_vertex + vertex) * 5 + axis];
                        draw_min[axis] = std::min(draw_min[axis], value);
                        draw_max[axis] = std::max(draw_max[axis], value);
                    }
                for (std::uint32_t element = 0; element < draw.index_count; ++element) {
                    const auto vertex = placed.indices[draw.first_index + element];
                    for (unsigned axis = 0; axis < 3; ++axis) {
                        const auto value = placed.vertices[vertex * 5 + axis];
                        index_min[axis] = std::min(index_min[axis], value);
                        index_max[axis] = std::max(index_max[axis], value);
                    }
                }
                std::printf("  source draw %02u visible=%u node=%s geometry=%s material=%s (%s) "
                            "effect_file=%s effect_url=%s indices=%u bounds=[%.1f..%.1f,%.1f..%.1f,%.1f..%.1f] "
                            "indexed_bounds=[%.1f..%.1f,%.1f..%.1f,%.1f..%.1f]\n",
                    i, draw.visible, draw.node_id, draw.geometry_id, draw.material_id,
                    draw.material_name, draw.external_effect_file[0] ? draw.external_effect_file : "<local>",
                    draw.effect_url, draw.index_count,
                    draw_min[0], draw_max[0], draw_min[1], draw_max[1], draw_min[2], draw_max[2],
                    index_min[0], index_max[0], index_min[1], index_max[1],
                    index_min[2], index_max[2]);
                for (std::uint32_t texture = 0; texture < draw.texture_count; ++texture) {
                    const auto& ref = placed.texture_references[draw.first_texture + texture];
                    std::printf("    sampler parameter=%s image=%s path=%s\n",
                        ref.parameter_id, ref.image_id, ref.source_path[0] ? ref.source_path : "<unresolved>");
                }
                if (draw.index_count <= 36) {
                    for (std::uint32_t element = 0; element < draw.index_count; element += 3) {
                        std::printf("    tri %u:", element / 3);
                        for (std::uint32_t corner = 0; corner < 3; ++corner) {
                            const auto vertex = placed.indices[draw.first_index + element + corner];
                            const auto* v = placed.vertices + vertex * 5;
                            std::printf(" %u=(%.3f,%.3f,%.3f;%.3f,%.3f)",
                                vertex, v[0], v[1], v[2], v[3], v[4]);
                        }
                        std::printf("\n");
                    }
                }
            }
            if (!draw.texture_count)
                std::printf("  no-texture draw: visible=%u node=%s material=%s (%s), geometry=%s, %u indices\n",
                    draw.visible, draw.node_id, draw.material_id, draw.material_name, draw.geometry_id,
                    draw.index_count);
            if (!draw.texture_count) {
                dh2::assets::Mesh source_mesh{};
                dh2::assets::Primitive source_primitive{};
                if (dh2_mesh_open(&source_mesh, &bres, draw.geometry_index) == dh2::assets::Error::ok &&
                    dh2_mesh_primitive(&source_mesh, draw.primitive_index, &source_primitive) ==
                        dh2::assets::Error::ok) {
                    std::printf("    attribute slots:");
                    for (unsigned slot = 0; slot < 18; ++slot) {
                        if (source_primitive.attributes[slot] < 0) continue;
                        dh2::assets::Attribute attribute{};
                        if (dh2_mesh_attribute(&source_mesh, source_primitive.attributes[slot],
                                               &attribute) == dh2::assets::Error::ok)
                            std::printf(" %u=%d/%u/%u", slot, source_primitive.attributes[slot],
                                        attribute.components, attribute.type);
                    }
                    std::printf("\n");
                }
            }
        }
        for (const auto& path : sampler_paths) std::printf("  sampler: %s\n", path.c_str());
        for (const auto& parameter : sampler_parameters)
            std::printf("  parameter: %s\n", parameter.c_str());
        float bounds_min[3] = {INFINITY, INFINITY, INFINITY};
        float bounds_max[3] = {-INFINITY, -INFINITY, -INFINITY};
        for (std::uint32_t i = 0; i < placed.vertex_count; ++i)
            for (int axis = 0; axis < 3; ++axis) {
                const float value = placed.vertices[5 * i + axis];
                if (value < bounds_min[axis]) bounds_min[axis] = value;
                if (value > bounds_max[axis]) bounds_max[axis] = value;
            }
        std::printf("  bounds XYZ: %.1f..%.1f, %.1f..%.1f, %.1f..%.1f\n",
            bounds_min[0], bounds_max[0], bounds_min[1], bounds_max[1],
            bounds_min[2], bounds_max[2]);
        dh2_viewer_scene_mesh_free(&placed);
        dh2_viewer_scene_mesh_free(&uncorrected);
        dh2_viewer_scene_mesh_free(&root_prefix);
        dh2_world_free(&level);
        return 0;
}
