#include "../crypt_template_character_projection_v1.hpp"
#include "../character_template_catalog_v1.hpp"

#include "../../game-data/data.hpp"
#include "../../world-data/world.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
using namespace dh2;
using dh2::world::crypt_template_character_projection_v1::Descriptor;
using dh2::world::crypt_template_character_projection_v1::Status;

void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

std::vector<std::uint8_t> read(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Unable to open: " + path);
    return {std::istreambuf_iterator<char>(file), {}};
}

void load_character_table(const std::string& assets,
                          data::CharacterTable& table) {
    const auto records = read(assets + "/data/character_properties_pyarray.bin");
    const auto names = read(assets + "/data/character_properties_pyarraynames.bin");
    const auto fields = read(assets + "/data/character_properties_pystructnames.bin");
    std::string error;
    const bool loaded = data::load_characters(
        {records.data(), records.size()}, {names.data(), names.size()},
        {fields.data(), fields.size()}, table, error);
    require(loaded, error.c_str());
}

void load_template_catalog(const std::string& assets,
                           const data::CharacterTable& characters,
                           character::template_factory::Catalog& catalog) {
    const auto class_names = read(assets + "/data/character_classes_pyarraynames.bin");
    std::string error;
    const auto template_data = read(assets + "/data/character_templates_pyarray.bin");
    const auto template_names = read(assets + "/data/character_templates_pyarraynames.bin");
    require(character::template_catalog_v1::load(template_data.data(),
                template_data.size(), template_names.data(), template_names.size(),
                class_names.data(), class_names.size(), characters, catalog, error),
            error.c_str());
}

const world::Object* find(const world::SourceLevel& level, const char* name) {
    for (std::uint32_t i = 0; i < level.entity_count; ++i) {
        const auto& object = level.entities[i];
        if (object.kind == world::RecordKind::mgp && object.name &&
            std::strcmp(object.name, name) == 0)
            return &object;
    }
    return nullptr;
}

void test_project_authored_records(const world::SourceLevel& level,
                                   const character::template_factory::Catalog& catalog) {
    using namespace world::crypt_template_character_projection_v1;
    using FactoryStatus = character::template_factory::Status;

    constexpr std::array<const char*, 4> names{{
        "_prim_tmp_ambusher01", "_prim_tmp_ambusher02",
        "_prim_tmp_ambusher03", "_prim_tmp_ambusher04"}};
    for (std::uint32_t i = 0; i < names.size(); ++i) {
        const auto* source = find(level, names[i]);
        require(source != nullptr, "Exact ambusher is absent from imported Crypt MGP");
        require(source->module_index == 1 && source->source_record == i,
                "Crypt ambusher module or original MGP record index changed");

        Descriptor result;
        std::string error;
        const auto status = project(*source, catalog,
            character::template_factory::selection_required, &result, error);
        require(status == Status::complete, error.c_str());
        require(result.module_index == 1 && result.source_record == i &&
                    result.source_path == source->source_path &&
                    result.object_name == names[i] &&
                    result.object_type == "Character",
                "Generated projection lost the exact source identity");
        require(result.property_source.editor_template_name ==
                    "MonsterCommonType1" &&
                    result.property_source.template_data_class ==
                    "Charater_Templates" &&
                    result.property_source.template_name ==
                    "GothicusCrypt_Ghosts" &&
                    result.property_source.explicit_property_name.empty(),
                "Generated projection changed or conflated the authored property route");
        require(result.property_resolution.status == FactoryStatus::selection_required &&
                    result.property_resolution_attempted &&
                    result.property_resolution.authored_alternatives ==
                        std::vector<std::int32_t>({35, 35, 35, 35, 37}) &&
                    result.property_resolution.property_id == -1,
                "Source weighted slots must remain ordered, duplicated, and unselected");
        require(result.property_source.editor_template_name !=
                    result.property_source.template_name,
                "Editor primitive metadata must not replace the runtime template key");
        require(result.world_position[0] == source->world_position[0] &&
                    result.local_rotation_degrees[2] ==
                        source->local.rotation_degrees[2] &&
                    result.local_scale[0] == source->local.scale[0],
                "Generated projection changed an authored transform field");
    }

    const auto* source = find(level, names[0]);
    Descriptor selected;
    std::string error;
    require(source != nullptr, "First source ambusher is missing");
    const auto selected_status = project(*source, catalog, 4, &selected, error);
    require(selected_status == Status::complete, error.c_str());
        require(selected.property_resolution.status == FactoryStatus::resolved &&
                selected.property_resolution_attempted &&
                selected.property_resolution.alternative_index == 4 &&
                selected.property_resolution.property_id == 37 &&
                selected.property_resolution.property_name == "Crypt_Ghost_RE" &&
                selected.property_resolution.class_id == 18 &&
                selected.property_resolution.class_name == "BaseMonster" &&
                selected.property_resolution.authored_alternatives ==
                    std::vector<std::int32_t>({35, 35, 35, 35, 37}),
            "Selected authored slot did not resolve against CharacterTable names");

    auto invalid_catalog = catalog;
    const auto ghost_template = std::find_if(invalid_catalog.templates.begin(),
        invalid_catalog.templates.end(), [](const auto& item) {
            return item.name == "GothicusCrypt_Ghosts";
        });
    require(ghost_template != invalid_catalog.templates.end(),
            "Loaded catalog lost GothicusCrypt_Ghosts");
    ghost_template->name = "MonsterCommonType1";
    Descriptor rejected;
    error.clear();
    require(project(*source, invalid_catalog,
                    character::template_factory::selection_required,
                    &rejected, error) == Status::template_resolution_failed &&
                rejected.object_name.empty(),
            "Unknown runtime template must fail without editor-name fallback or partial output");
}

}  // namespace

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: crypt_template_character_projection_v1 assets-root\n";
        return 2;
    }
    try {
        const std::string assets = argv[1];
        const auto mlx = read(assets + "/worlds/x07_crypt_backup.mlx");
        world::SourceLevel level{};
        world::Diagnostic diagnostic{};
        require(dh2_world_import_level(&level, "CRYPT",
                    "data/scene/x07_crypt_backup.mlx", mlx.data(), mlx.size(),
                    &diagnostic) == world::Error::ok,
                diagnostic.message);
        for (std::uint32_t i = 0; i < level.module_count; ++i) {
            const auto slash = std::string(level.modules[i].cache_mgp).find_last_of("/\\");
            const auto file_name = std::string(level.modules[i].cache_mgp).substr(
                slash == std::string::npos ? 0 : slash + 1);
            const auto mgp = read(assets + "/worlds/" + file_name);
            require(dh2_world_import_module_objects(&level, i,
                        world::RecordKind::mgp, level.modules[i].cache_mgp,
                        mgp.data(), mgp.size(), &diagnostic) == world::Error::ok,
                    diagnostic.message);
        }

        data::CharacterTable characters;
        load_character_table(assets, characters);
        character::template_factory::Catalog catalog;
        load_template_catalog(assets, characters, catalog);
        test_project_authored_records(level, catalog);
        dh2_world_free(&level);
        std::cout << "Crypt template Character projection: four source records retained, duplicate-weighted choices preserved, optional slot resolution verified; no RNG/runtime actor construction\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
