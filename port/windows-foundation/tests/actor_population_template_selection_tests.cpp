#include "../actor_population.hpp"
#include "../asset_catalog.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;

static void require(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

int main(int argc, char** argv) {
    try {
        require(argc == 2, "Supply repository root");
        const std::filesystem::path root(argv[1]);
        AssetCatalog assets(root / ".local-inputs/windows-shared-assets");
        AssetCatalog schema_assets(root / "port/windows-foundation/features/encounters/fixtures");
        std::string error;

        const std::string table_root = "original-cache/data/pydata/";
        const auto character_records = assets.read(table_root + "character_properties_pyarray.bin");
        const auto character_names = assets.read(table_root + "character_properties_pyarraynames.bin");
        const auto character_schema = assets.read(table_root + "character_properties_pystructnames.bin");
        dh2::data::CharacterTable characters;
        require(dh2::data::load_characters({character_records.data(), character_records.size()},
                    {character_names.data(), character_names.size()},
                    {character_schema.data(), character_schema.size()}, characters, error), error);

        const auto template_records = assets.read(table_root + "character_templates_pyarray.bin");
        const auto template_names = assets.read(table_root + "character_templates_pyarraynames.bin");
        const auto template_schema = schema_assets.read("character_templates_pystructnames.bin");
        dh2::data::CharacterTemplateTableV78 templates;
        require(templates.load({template_records.data(), template_records.size()},
                    {template_names.data(), template_names.size()},
                    {template_schema.data(), template_schema.size()}, error), error);
        dh2::data::CharacterTemplateTableV78 fixed_layout;
        require(fixed_layout.load({template_records.data(),template_records.size()},
                    {template_names.data(),template_names.size()},error),error);
        require(fixed_layout.names()==templates.names()&&fixed_layout.rows().size()==templates.rows().size(),"Fixed source layout differs from external schema");
        for(std::size_t row=0;row<templates.rows().size();++row)
            require(fixed_layout.rows()[row].char_info==templates.rows()[row].char_info&&fixed_layout.rows()[row].selected_ids==templates.rows()[row].selected_ids,"Fixed layout changed authored template weights/IDs");

        ActorProfileLibrary profiles;
        require(profiles.load(assets, "actor-profiles-v2.xml", error), error);
        std::vector<ActorDefinition> definitions;
        require(load_actor_definitions(assets, "original-cache/data/scene/001_swamp.mlx",
                                       definitions, error), error);
        const auto symbolic = std::find_if(definitions.begin(), definitions.end(), [](const auto& definition) {
            const auto name = definition.properties.find("name");
            const auto character_template = definition.properties.find("char_template");
            const auto table = definition.properties.find("char_template_pydata");
            const auto profile = definition.properties.find("charpropsname");
            return name != definition.properties.end() && name->second == "_prim_LizTemplate_01" &&
                   character_template != definition.properties.end() && !character_template->second.empty() &&
                   table != definition.properties.end() && table->second == "Charater_Templates" &&
                   profile == definition.properties.end();
        });
        require(symbolic != definitions.end(), "Authored Swamp symbolic template declaration missing");
        const auto original_id = symbolic->stableId;
        const auto original_source = symbolic->sourceId;
        const auto original_template = symbolic->properties.at("char_template");

        PopulationTemplateSelectionV1 selection;
        selection.characters = &characters;
        selection.templates = &templates;
        unsigned random_calls = 0;
        std::int32_t requested_bound = 0;
        selection.random_index = [&](std::int32_t bound, std::int32_t& index, std::string&) {
            ++random_calls;
            requested_bound = bound;
            index = bound - 1; // Select the authored respawn member at the final weighted slot.
            return true;
        };

        ActorPopulation population;
        auto policy = [&](const ActorDefinition& definition) {
            if (definition.stableId == original_id) return PopulationDecision::include;
            if (definition.name == "_prim_MothTemplate_13") return PopulationDecision::unknown;
            return PopulationDecision::exclude;
        };
        auto customize = [](const ActorDefinition&, const ActorProfile&) {
            ActorCustomization result;
            result.allow_missing_animation_targets = true;
            result.use_authored_modular_defaults = true;
            return result;
        };
        require(population.load(assets, "original-cache/data/scene/001_swamp.mlx",
                               profiles, policy, customize, error, &selection), error);
        require(population.actors().size() == 1, "Exactly the caller-admitted symbolic actor should load");
        const auto& selected = population.actors().front();
        const auto template_id = templates.find(original_template.c_str());
        require(template_id >= 0, "Selected authored template absent from actual CharacterTemplates table");
        const auto& members = templates.rows()[static_cast<std::size_t>(template_id)].selected_ids;
        require(members.size() == 5 && requested_bound == static_cast<std::int32_t>(members.size()),
                "External RNG was not bounded by the actual weighted member array");
        const auto expected_row = members.back();
        require(expected_row >= 0 && static_cast<std::size_t>(expected_row) < characters.names.size(),
                "Actual template respawn member does not reference CharacterTable");
        require(selected.profileId == characters.names[static_cast<std::size_t>(expected_row)] &&
                selected.profileId == "Swamp_LizadMan_Type1_Respawn",
                "Template's selected CharacterTable name did not resolve the existing profile");
        require(selected.source_character_cache == expected_row && selected.source_template_cache == template_id,
                "Actual source signed16 CharacterTable/template cache facts were not retained");
        require(selected.definition.stableId == original_id && selected.definition.sourceId == original_source &&
                selected.definition.properties.at("char_template") == original_template,
                "Selected profile mutated the authored definition or stable identity");
        require(random_calls == 1 && selected.visual.loaded(),
                "Template selection did not use exactly one external draw and load its original visual");
        bool unknown_gated = false;
        for (const auto& notice : population.notices())
            if (notice.sourceId.find("_prim_MothTemplate_13") != std::string::npos &&
                notice.reason.find("unknown") != std::string::npos) unknown_gated = true;
        require(unknown_gated, "Unknown condition policy was not kept gated for the Invalid activation declaration");

        PopulationTemplateSelectionV1 missing_rng;
        missing_rng.characters = &characters;
        missing_rng.templates = &templates;
        ActorPopulation gated;
        require(gated.load(assets, "original-cache/data/scene/001_swamp.mlx", profiles,
                    policy, customize, error, &missing_rng), error);
        require(gated.actors().empty(), "Missing shared RNG silently selected a symbolic actor");
        bool missing_rng_notice = false;
        for (const auto& notice : gated.notices())
            if (notice.sourceId == original_source && notice.reason.find("shared source RNG") != std::string::npos)
                missing_rng_notice = true;
        require(missing_rng_notice, "Missing RNG did not leave an explicit per-actor selection gap");

        std::cout << "PASS symbolicTemplateRow=" << template_id
                  << " selectedCharacterRow=" << expected_row
                  << " weightedBound=" << requested_bound
                  << " externalDraws=" << random_calls
                  << " unknownConditionGated=true missingRngGated=true\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "ActorPopulation template selector FAIL: " << exception.what() << '\n';
        return 1;
    }
}
