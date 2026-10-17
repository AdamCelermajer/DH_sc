#include "../technique_selector.hpp"
#include "../../android-app/scene_buffers.hpp"
#include "../../asset-payloads/payloads.hpp"
#include "../../scene-payloads/scene.hpp"
#include "../../world-data/world.hpp"
#include "../../world-data/world_scene.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace {
using dh2::materials::Effect;
using dh2::materials::EffectGroup;
using dh2::materials::Material;
using dh2::resources::BresView;
using dh2::resources::Library;
using dh2::scene_materials::CurrentTechnique;
using dh2::scene_materials::TechniqueSelectorError;

void require(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "technique selector host check failed: %s\n", message);
        std::exit(1);
    }
}

std::vector<std::uint8_t> read_file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    require(static_cast<bool>(input), "cannot read supplied cache file");
    return {std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>()};
}

void set_word(std::uint8_t* bytes, std::uint32_t value) {
    bytes[0] = static_cast<std::uint8_t>(value);
    bytes[1] = static_cast<std::uint8_t>(value >> 8);
    bytes[2] = static_cast<std::uint8_t>(value >> 16);
    bytes[3] = static_cast<std::uint8_t>(value >> 24);
}

std::uint32_t material_index(const BresView& bres, const char* wanted) {
    const auto count = dh2_bres_library_count(&bres, Library::material);
    for (std::uint32_t i = 0; i < count; ++i) {
        Material material{};
        require(dh2_material_record(&material, &bres,
                    static_cast<std::int32_t>(i)) == dh2::materials::Error::ok,
                "material record failed checked decoding");
        if (std::strcmp(material.id, wanted) == 0) return i;
    }
    require(false, "requested SWAMP material does not exist");
    return 0;
}

Material material_at(const BresView& bres, std::uint32_t index) {
    Material material{};
    require(dh2_material_record(&material, &bres,
                static_cast<std::int32_t>(index)) == dh2::materials::Error::ok,
            "material record failed checked decoding");
    return material;
}

std::uint32_t find_effect(const BresView& bres, const char* wanted) {
    const auto count = dh2_bres_library_count(&bres, Library::effect);
    for (std::uint32_t i = 0; i < count; ++i) {
        Effect effect{};
        require(dh2_effect_record(&effect, &bres,
                    static_cast<std::int32_t>(i)) == dh2::materials::Error::ok,
                "effect record failed checked decoding");
        if (std::strcmp(effect.id, wanted) == 0) return i;
    }
    require(false, "requested external effect does not exist");
    return 0;
}

bool token(const std::string& name, const char* wanted) {
    std::size_t begin = 0;
    while (begin <= name.size()) {
        const auto end = name.find('_', begin);
        if (name.compare(begin, (end == std::string::npos ? name.size() : end) - begin,
                         wanted) == 0)
            return true;
        if (end == std::string::npos) break;
        begin = end + 1;
    }
    return false;
}

void test_ordinal_semantics(const std::vector<std::string>& group_names,
                            const std::string& selected) {
    std::uint8_t ordinal = 0;
    std::string error;
    require(dh2::scene_materials::renderer_technique_ordinal(
                selected, group_names, ordinal, error) ==
                TechniqueSelectorError::ok && ordinal == 4,
            "ordered first-match renderer ordinal differs from the named-table fixture");
    const std::vector<std::string> duplicate{"same", "same"};
    require(dh2::scene_materials::renderer_technique_ordinal(
                "same", duplicate, ordinal, error) ==
                TechniqueSelectorError::ok && ordinal == 0,
            "renderer lookup must choose the first exact name match");
    require(dh2::scene_materials::renderer_technique_ordinal(
                "missing", group_names, ordinal, error) ==
                TechniqueSelectorError::not_found && ordinal == 0xff,
            "missing renderer selector must return the 0xff sentinel");
    auto too_many = group_names;
    too_many.resize(256, "unused");
    require(dh2::scene_materials::renderer_technique_ordinal(
                selected, too_many, ordinal, error) ==
                TechniqueSelectorError::limit && ordinal == 0xff,
            "unrepresentable 256-entry renderer list must reject");
}

void test_rejections(const Material& material, const BresView& bres,
                     std::uint32_t material_record_index) {
    std::vector<CurrentTechnique> selections;
    std::string error;
    require(dh2::scene_materials::material_current_techniques(
                material, selections, error) == TechniqueSelectorError::ok &&
                !selections.empty(),
            "valid material selectors did not decode");
    auto parameter_result = dh2_material_parameter(
        nullptr, &material, 0);
    require(parameter_result == dh2::materials::Error::argument,
            "material binding must reject a null output");
    // Re-read one selector parameter, then damage the already-validated view
    // shape presented to the standalone decoder.
    dh2::materials::Parameter parameter{};
    bool found = false;
    for (std::uint32_t i = 0; i < material.parameter_count; ++i) {
        require(dh2_material_parameter(&parameter, &material,
                    static_cast<std::int32_t>(i)) == dh2::materials::Error::ok,
                "fixture parameter failed checked decoding");
        if (parameter.type_code == 20 && std::strstr(parameter.id, "/CurrentTechnique")) {
            found = true;
            break;
        }
    }
    require(found, "fixture material lacks CurrentTechnique");
    CurrentTechnique decoded;
    auto bad = parameter;
    bad.type_code = 11;
    require(dh2::scene_materials::decode_current_technique(
                bad, decoded, error) == TechniqueSelectorError::kind,
            "wrong serialized type must reject");
    bad = parameter;
    bad.value_count = 2;
    require(dh2::scene_materials::decode_current_technique(
                bad, decoded, error) == TechniqueSelectorError::count,
            "non-scalar selector must reject");
    bad = parameter;
    bad.raw_value = bres.bytes + bres.size - 4;
    require(dh2::scene_materials::decode_current_technique(
                bad, decoded, error) == TechniqueSelectorError::range,
            "truncated source payload must reject");
    const char no_profile[] = "x/CurrentTechnique";
    bad = parameter;
    bad.id = no_profile;
    require(dh2::scene_materials::decode_current_technique(
                bad, decoded, error) == TechniqueSelectorError::profile,
            "profile-less CurrentTechnique ID must reject");

    std::vector<std::uint8_t> corrupted_bytes(
        bres.bytes, bres.bytes + bres.size);
    const auto value_offset = static_cast<std::size_t>(
        parameter.raw_value - bres.bytes);
    set_word(corrupted_bytes.data() + value_offset + 4, 0xfffffff0U);
    BresView corrupted_view = bres;
    corrupted_view.bytes = corrupted_bytes.data();
    Material corrupted_material{};
    require(dh2_material_record(&corrupted_material, &corrupted_view,
                static_cast<std::int32_t>(material_record_index)) ==
                dh2::materials::Error::ok,
            "material record failed with an out-of-range selector string");
    dh2::materials::Parameter corrupted_parameter{};
    bool corrupted_found = false;
    for (std::uint32_t i = 0; i < corrupted_material.parameter_count; ++i) {
        require(dh2_material_parameter(&corrupted_parameter,
                    &corrupted_material, static_cast<std::int32_t>(i)) ==
                    dh2::materials::Error::ok,
                "mutated material parameter failed checked decoding");
        if (corrupted_parameter.type_code == 20 &&
            std::strstr(corrupted_parameter.id, "/CurrentTechnique")) {
            corrupted_found = true;
            break;
        }
    }
    require(corrupted_found &&
                dh2::scene_materials::decode_current_technique(
                    corrupted_parameter, decoded, error) ==
                    TechniqueSelectorError::string,
            "out-of-range selector string offset must reject");
}

void print_selection(const CurrentTechnique& item, std::uint8_t group0,
                     std::uint8_t group1) {
    std::printf("{\"parameter_id\":\"%s\",\"profile\":\"%s\","
                "\"selector\":\"%s\",\"raw_tag\":%u,"
                "\"serialized_group0_ordinal\":%u,"
                "\"serialized_group1_ordinal\":%u}",
                item.parameter_id.c_str(), item.profile.c_str(),
                item.name.c_str(), item.raw_tag, group0, group1);
}
}

int main(int argc, char** argv) {
    require(argc == 2, "usage: technique_selector_host <cache-root>");
    const std::string cache = argv[1];
    const auto scene_bytes = read_file(cache + "/data/3d/modules/swamp/swamp.bdae");
    const auto effect_bytes = read_file(
        cache + "/data/gfx/effects/gl_diffuse_l1_vc_iphone.bdae");
    const auto mlx = read_file(cache + "/data/scene/001_swamp.mlx");
    BresView scene_bres{}, effect_bres{};
    require(dh2_bres_open(&scene_bres, scene_bytes.data(), scene_bytes.size()) ==
                dh2::resources::BresError::ok,
            "SWAMP scene BRES rejected");
    require(dh2_bres_open(&effect_bres, effect_bytes.data(), effect_bytes.size()) ==
                dh2::resources::BresError::ok,
            "external effect BRES rejected");

    Effect effect{};
    const auto effect_index = find_effect(effect_bres, "Multilight-fx");
    require(dh2_effect_record(&effect, &effect_bres,
                static_cast<std::int32_t>(effect_index)) == dh2::materials::Error::ok,
            "Multilight effect record failed checked decoding");
    EffectGroup groups[2]{};
    std::vector<std::string> group_names[2];
    std::uint8_t al_without_specular[2] = {0xff, 0xff};
    std::uint8_t al_with_specular[2] = {0xff, 0xff};
    for (std::uint32_t group_index = 0; group_index < 2; ++group_index) {
        require(dh2_effect_group(&groups[group_index], &effect,
                    static_cast<std::int32_t>(group_index)) ==
                    dh2::materials::Error::ok,
                "Multilight effect group failed checked decoding");
        require(groups[group_index].named_count == 24,
                "Multilight group named-technique count changed");
        for (std::uint32_t i = 0; i < groups[group_index].named_count; ++i) {
            std::string name, error;
            require(dh2::scene_materials::effect_technique_name(
                        groups[group_index], i, name, error) ==
                        TechniqueSelectorError::ok,
                    "effect named record failed bounded decoding");
            group_names[group_index].push_back(name);
            if (name == "L1_Vc_Al_----_----_----_----")
                al_without_specular[group_index] = static_cast<std::uint8_t>(i);
            if (name == "L1_Vc_Al_Sp_----_----_----")
                al_with_specular[group_index] = static_cast<std::uint8_t>(i);
        }
        require(al_without_specular[group_index] == 2 &&
                    al_with_specular[group_index] == 4,
                "AL source technique ordinals differ in effect group");
    }
    require(group_names[0] == group_names[1],
            "SWAMP effect groups expose different ordered technique names");
    test_ordinal_semantics(group_names[0], "L1_Vc_Al_Sp_----_----_----");

    const auto opaque_index = material_index(scene_bres, "Material__11610");
    const auto alpha_index = material_index(scene_bres, "Material__11611");
    const Material opaque = material_at(scene_bres, opaque_index);
    const Material alpha = material_at(scene_bres, alpha_index);
    std::vector<CurrentTechnique> opaque_selectors, alpha_selectors;
    std::string error;
    require(dh2::scene_materials::material_current_techniques(
                opaque, opaque_selectors, error) == TechniqueSelectorError::ok &&
                opaque_selectors.size() == 2,
            "Material__11610 must expose its GLES/GLES2 selector pair");
    require(dh2::scene_materials::material_current_techniques(
                alpha, alpha_selectors, error) == TechniqueSelectorError::ok &&
                alpha_selectors.size() == 2,
            "Material__11611 must expose its GLES/GLES2 selector pair");
    std::uint32_t catalogue_selector_materials = 0;
    const auto scene_material_count = dh2_bres_library_count(
        &scene_bres, Library::material);
    for (std::uint32_t i = 0; i < scene_material_count; ++i) {
        Material candidate{};
        require(dh2_material_record(&candidate, &scene_bres,
                    static_cast<std::int32_t>(i)) == dh2::materials::Error::ok,
                "scene material inventory failed checked decoding");
        std::vector<CurrentTechnique> selectors;
        require(dh2::scene_materials::material_current_techniques(
                    candidate, selectors, error) == TechniqueSelectorError::ok,
                "scene CurrentTechnique inventory failed checked decoding");
        if (selectors.empty()) continue;
        ++catalogue_selector_materials;
        require(std::strcmp(candidate.id, "Material__11610") == 0 ||
                    std::strcmp(candidate.id, "Material__11611") == 0,
                "scene BRES has an unhandled type-20 selector material");
    }
    require(catalogue_selector_materials == 2,
            "scene BRES CurrentTechnique material inventory changed");
    test_rejections(alpha, scene_bres, alpha_index);

    const char* opaque_expected[2] = {
        "L1_Vc_----_----_----_----_----",
        "L1_Vc_----_Sp_----_----_----"};
    const char* alpha_expected[2] = {
        "L1_Vc_Al_----_----_----_----",
        "L1_Vc_Al_Sp_----_----_----"};
    for (std::uint32_t i = 0; i < 2; ++i) {
        require(opaque_selectors[i].profile == (i ? "GLES2" : "GLES") &&
                    opaque_selectors[i].name == opaque_expected[i],
                "Material__11610 selector/profile changed");
        require(alpha_selectors[i].profile == (i ? "GLES2" : "GLES") &&
                    alpha_selectors[i].name == alpha_expected[i],
                "Material__11611 selector/profile changed");
        require(!token(opaque_selectors[i].name, "Al") &&
                    !token(opaque_selectors[i].name, "At"),
                "Material__11610 unexpectedly selects an alpha variant");
        require(token(alpha_selectors[i].name, "Al") &&
                    !token(alpha_selectors[i].name, "At"),
                "Material__11611 must select AL and not AT");
    }
    require(std::strcmp(alpha.external_effect_file,
                "GL_Diffuse_L1_VC_iPhone.bdae") == 0 &&
                std::strcmp(alpha.effect_url, "#Multilight-fx") == 0 &&
                std::strcmp(opaque.external_effect_file,
                "GL_Diffuse_L1_VC_iPhone.bdae") == 0 &&
                std::strcmp(opaque.effect_url, "#Multilight-fx") == 0,
            "SWAMP materials no longer link to the audited external effect");

    for (std::size_t selector_index = 0;
         selector_index < alpha_selectors.size(); ++selector_index) {
        const auto& item = alpha_selectors[selector_index];
        const auto expected_ordinal = static_cast<std::uint8_t>(
            token(item.name, "Sp") ? 4 : 2);
        for (std::uint32_t group = 0; group < 2; ++group) {
            std::uint8_t ordinal = 0xff;
            require(dh2::scene_materials::effect_technique_ordinal(
                        groups[group], item.name, ordinal, error) ==
                        TechniqueSelectorError::ok && ordinal == expected_ordinal,
                    "material selector did not resolve to source effect name record");
        }
    }
    for (const auto& item : opaque_selectors) {
        for (std::uint32_t group = 0; group < 2; ++group) {
            std::uint8_t ordinal = 0xff;
            require(dh2::scene_materials::effect_technique_ordinal(
                        groups[group], item.name, ordinal, error) ==
                        TechniqueSelectorError::ok,
                    "opaque material selector did not resolve to source effect name record");
        }
    }

    dh2::world::SourceLevel level{};
    dh2::world::Diagnostic world_diagnostic{};
    require(dh2_world_import_level(&level, "SWAMP", "data/scene/001_swamp.mlx",
                mlx.data(), mlx.size(), &world_diagnostic) == dh2::world::Error::ok,
            world_diagnostic.message);
    require(level.module_count == 9,
            "source SWAMP selection must contain all nine modules");
    dh2::scene_payload::Scene source_scene{};
    require(dh2_scene_open(&source_scene, &scene_bres) ==
                dh2::scene_payload::Error::ok,
            "source scene payload failed checked decoding");

    std::uint32_t total_opaque_draws = 0, total_alpha_draws = 0;
    std::uint32_t module_zero_opaque_draws = 0, module_zero_alpha_draws = 0;
    std::uint32_t visible_selector_draws = 0;
    std::printf("{\"validation\":\"PASS\",\"effect\":\"%s\","
                "\"effect_groups\":[%u,%u],\"group_names_identical\":true,"
                "\"alpha_selector_mappings\":[",
                effect.id, groups[0].named_count, groups[1].named_count);
    for (std::size_t i = 0; i < alpha_selectors.size(); ++i) {
        if (i) std::printf(",");
        std::uint8_t ord0 = 0xff, ord1 = 0xff;
        require(dh2::scene_materials::effect_technique_ordinal(
                    groups[0], alpha_selectors[i].name, ord0, error) ==
                    TechniqueSelectorError::ok &&
                    dh2::scene_materials::effect_technique_ordinal(
                    groups[1], alpha_selectors[i].name, ord1, error) ==
                    TechniqueSelectorError::ok,
                "AL selector mapping was not found in both source groups");
        print_selection(alpha_selectors[i], ord0, ord1);
    }
    std::printf("],\"opaque_selector_mappings\":[");
    for (std::size_t i = 0; i < opaque_selectors.size(); ++i) {
        if (i) std::printf(",");
        std::uint8_t ord0 = 0xff, ord1 = 0xff;
        require(dh2::scene_materials::effect_technique_ordinal(
                    groups[0], opaque_selectors[i].name, ord0, error) ==
                    TechniqueSelectorError::ok &&
                    dh2::scene_materials::effect_technique_ordinal(
                    groups[1], opaque_selectors[i].name, ord1, error) ==
                    TechniqueSelectorError::ok,
                "opaque selector mapping was not found in both source groups");
        print_selection(opaque_selectors[i], ord0, ord1);
    }
    std::printf("],\"modules\":[");
    for (std::uint32_t module_index = 0; module_index < level.module_count;
         ++module_index) {
        const auto& module = level.modules[module_index];
        require(std::strcmp(module.cache_dae,
                    "data/3d/modules/swamp/swamp.bdae") == 0,
                "a SWAMP module no longer uses the audited shared BRES catalogue");
        dh2::world::ModuleBinding binding{};
        require(dh2_world_bind_module(&binding, &module, &source_scene,
                    &world_diagnostic) == dh2::world::Error::ok,
                "source SWAMP module root failed to bind");
        std::vector<std::uint32_t> records(65536);
        std::uint32_t record_count = 0;
        require(dh2_world_module_records(records.data(), records.size(),
                    &record_count, &binding, &source_scene,
                    &world_diagnostic) == dh2::world::Error::ok,
                "source module subtree enumeration failed");
        dh2::math::Matrix4f correction{};
        require(dh2_world_placement_matrix(&correction, &binding,
                    &world_diagnostic) == dh2::world::Error::ok,
                "source module placement correction failed");
        dh2::viewer::SceneMesh mesh{};
        require(dh2_world_scene_mesh_nodes(&mesh, &scene_bres, records.data(),
                    record_count, &correction) ==
                    dh2::viewer::SceneMeshError::ok,
                "source module draw assembly failed");
        std::uint32_t opaque_draws = 0, alpha_draws = 0;
        for (std::uint32_t draw_index = 0; draw_index < mesh.draw_commands;
             ++draw_index) {
            const auto& draw = mesh.draws[draw_index];
            if (!draw.visible) continue;
            if (draw.material_index == static_cast<std::int32_t>(opaque_index)) {
                require(std::strcmp(draw.material_id, "Material__11610") == 0,
                        "opaque source draw material ID/index disagree");
                ++opaque_draws;
            }
            if (draw.material_index == static_cast<std::int32_t>(alpha_index)) {
                ++alpha_draws;
                ++visible_selector_draws;
                require(alpha_selectors.size() == 2 &&
                            std::strcmp(draw.material_id, "Material__11611") == 0,
                        "visible AlphaMap draw lost its source material mapping");
            }
        }
        if (module_index == 0) {
            module_zero_opaque_draws = opaque_draws;
            module_zero_alpha_draws = alpha_draws;
            require(opaque_draws == 25 && alpha_draws == 22,
                    "module-zero source draw counts for the two materials changed");
        }
        if (module_index) std::printf(",");
        std::printf("{\"index\":%u,\"name\":\"%s\","
                    "\"catalogue\":\"%s\",\"Material__11610_draws\":%u,"
                    "\"Material__11611_draws\":%u}",
                    module_index, module.record.name, module.cache_dae,
                    opaque_draws, alpha_draws);
        total_opaque_draws += opaque_draws;
        total_alpha_draws += alpha_draws;
        dh2_viewer_scene_mesh_free(&mesh);
    }
    std::printf("],\"catalogue_selector_materials\":%u,"
                "\"draw_totals\":{\"Material__11610\":%u,"
                "\"Material__11611\":%u},\"visible_alpha_selector_draws\":%u,"
                "\"native_getTechniqueID_model\":\"first exact ordered-name match returns uint8 ordinal; absent=0xff\","
                "\"serialized_group_ordinal_is_runtime_ordinal\":false,"
                "\"effect_group_profile_mapping\":\"unresolved\","
                "\"shader_selection\":\"Material__11611 GLES/GLES2 select Al and not At; Material__11610 selects neither\","
                "\"blend_depth_state\":\"not inferred from selector or shader branch\"}\n",
                catalogue_selector_materials, total_opaque_draws,
                total_alpha_draws, visible_selector_draws);
    require(visible_selector_draws == total_alpha_draws &&
                module_zero_opaque_draws == 25 && module_zero_alpha_draws == 22,
            "nine-module selector draw inventory lost module-zero source evidence");
    dh2_world_free(&level);
    return 0;
}
