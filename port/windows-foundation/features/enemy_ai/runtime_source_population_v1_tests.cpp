#include "runtime_source_population_v1.hpp"
#include "../../asset_catalog.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::enemy_ai;

static void check(bool value, const std::string& error) {
    if (!value) throw std::runtime_error(error);
}

static const ActorDefinition& find_definition(const std::vector<ActorDefinition>& definitions,
                                              const std::string& name) {
    const auto found = std::find_if(definitions.begin(), definitions.end(), [&](const auto& row) {
        return row.name == name;
    });
    if (found == definitions.end()) throw std::runtime_error("Missing authored source declaration: " + name);
    return *found;
}

static const SourcePopulationPlanRowV1& find_plan(const std::vector<SourcePopulationPlanRowV1>& rows,
                                                  const std::string& name) {
    const auto found = std::find_if(rows.begin(), rows.end(), [&](const auto& row) {
        return row.authored_name == name;
    });
    if (found == rows.end()) throw std::runtime_error("Missing planned source declaration: " + name);
    return *found;
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply repository root");
        const std::filesystem::path root(argv[1]);
        AssetCatalog assets(root / ".local-inputs/windows-shared-assets");
        AssetCatalog schema_assets(root / "port/windows-foundation/features/encounters/fixtures");
        std::string error;

        std::vector<ActorDefinition> definitions;
        check(load_actor_definitions(assets, "original-cache/data/scene/001_swamp.mlx",
                                     definitions, error), error);
        const auto& moth1 = find_definition(definitions, "_prim_MothTemplate_01");
        const auto& moth2 = find_definition(definitions, "_prim_MothTemplate_02");
        check(moth1.properties.at("char_template") == "Swamp_CommonType2" &&
              moth2.properties.at("char_template") == "Swamp_CommonType2" &&
              moth1.properties.at("spawn_prob") == "100" &&
              moth2.properties.at("spawn_prob") == "100",
              "Reference Swamp declarations differ from the authored CommonType2 / spawn_prob=100 rows");
        check(moth1.properties.find("activate_cond") == moth1.properties.end() &&
              moth1.properties.find("deactivate_cond") == moth1.properties.end() &&
              moth2.properties.find("activate_cond") == moth2.properties.end() &&
              moth2.properties.find("deactivate_cond") == moth2.properties.end(),
              "The reference Moth declarations unexpectedly gained authored activation gates");

        const std::string table_root = "original-cache/data/pydata/";
        const auto char_records = assets.read(table_root + "character_properties_pyarray.bin");
        const auto char_names = assets.read(table_root + "character_properties_pyarraynames.bin");
        const auto char_schema = assets.read(table_root + "character_properties_pystructnames.bin");
        dh2::data::CharacterTable characters;
        check(dh2::data::load_characters({char_records.data(), char_records.size()},
                    {char_names.data(), char_names.size()}, {char_schema.data(), char_schema.size()},
                    characters, error), error);
        const auto template_records = assets.read(table_root + "character_templates_pyarray.bin");
        const auto template_names = assets.read(table_root + "character_templates_pyarraynames.bin");
        const auto template_schema = schema_assets.read("character_templates_pystructnames.bin");
        dh2::data::CharacterTemplateTableV78 templates;
        check(templates.load({template_records.data(), template_records.size()},
                    {template_names.data(), template_names.size()},
                    {template_schema.data(), template_schema.size()}, error), error);
        ActorProfileLibrary profiles;
        check(profiles.load(assets, "actor-profiles-v2.xml", error), error);
        PopulationTemplateSelectionV1 selection;
        selection.characters = &characters;
        selection.templates = &templates;
        std::int32_t template_draws = 0;
        selection.random_index = [&](std::int32_t bound, std::int32_t& index, std::string&) {
            ++template_draws;
            index = 0;
            return bound > 0;
        };
        ActorPopulation population;
        const auto condition = [&](const ActorDefinition& definition) {
            if (definition.name == moth1.name || definition.name == moth2.name)
                return PopulationDecision::include;
            if (definition.name == "_prim_MothTemplate_13") return PopulationDecision::unknown;
            return PopulationDecision::exclude;
        };
        auto customize = [](const ActorDefinition&, const ActorProfile&) {
            ActorCustomization value;
            value.allow_missing_animation_targets = true;
            value.use_authored_modular_defaults = true;
            return value;
        };
        check(population.load(assets, "original-cache/data/scene/001_swamp.mlx", profiles,
                             condition, customize, error, &selection), error);
        check(population.actors().size() == 2 && template_draws == 2,
              "Reference Moths did not use the existing source ActorPopulation selector");

        dh2::data::LootRandom8V2 source_random{0x12345678u, 0};
        SourcePopulationPlannerInputV1 input;
        input.definitions = &definitions;
        input.selected_actors = &population.actors();
        input.activation_decision = condition;
        input.bind_spawn_context = [&](const ActorDefinition& definition,
                                       SourcePopulationSpawnContextV1& context,
                                       std::string& bind_error) {
            if (definition.name != moth1.name && definition.name != moth2.name) {
                bind_error = "Unexpected declaration requested source spawn facts";
                return false;
            }
            context.cached_roll = -1;
            context.probability = 100;
            context.is_player = false;
            context.online = false;
            context.network_id = -1;
            context.random0 = &source_random;
            return true;
        };
        std::vector<SourcePopulationPlanRowV1> plan;
        check(plan_source_population_v1(input, plan, error), error);
        const auto& planned1 = find_plan(plan, moth1.name);
        const auto& planned2 = find_plan(plan, moth2.name);
        check(planned1.status == SourcePopulationPlanStatusV1::include &&
              planned1.decision_roll == -2 && planned1.roll_cache_before == -1 &&
              planned1.roll_cache_after == -2 && planned1.rng_calls_before == 0 &&
              planned1.rng_calls_after == 1,
              "First authored non-player declaration must consume one shared RNG draw");
        check(planned2.status == SourcePopulationPlanStatusV1::include &&
              planned2.decision_roll == -2 && planned2.roll_cache_before == -1 &&
              planned2.roll_cache_after == -2 && planned2.rng_calls_before == 1 &&
              planned2.rng_calls_after == 2 && source_random.calls == 2,
              "Second authored non-player declaration must continue the same RNG stream once");
        check(planned1.stable_id == moth1.stableId && planned1.source_id == moth1.sourceId &&
              planned1.authored_placement == moth1.placement &&
              planned1.selected_population_index < population.actors().size() &&
              planned1.source_character_cache == population.actors()[planned1.selected_population_index].source_character_cache &&
              planned1.source_template_cache == population.actors()[planned1.selected_population_index].source_template_cache,
              "Planner did not preserve the actual declaration placement and selected Character/template row");
        const auto& unknown = find_plan(plan, "_prim_MothTemplate_13");
        check(unknown.status == SourcePopulationPlanStatusV1::unknown &&
              unknown.selected_population_index == static_cast<std::size_t>(-1),
              "Unknown Invalid activation condition must stay out of the plan");

        // The canonical player branch uses the same source -2 selection
        // sentinel. Exercise it with a caller-provided player fact separately;
        // the authored Moth declarations above remain non-player actors.
        PopulationActor player_actor;
        player_actor.definition = moth1;
        player_actor.profileId = population.actors()[0].profileId;
        std::vector<PopulationActor> player_selected;
        player_selected.push_back(std::move(player_actor));
        std::vector<ActorDefinition> player_definition{moth1};
        dh2::data::LootRandom8V2 player_rng{0x12345678u, 0};
        SourcePopulationPlannerInputV1 player_input;
        player_input.definitions = &player_definition;
        player_input.selected_actors = &player_selected;
        player_input.activation_decision = [](const ActorDefinition&) { return PopulationDecision::include; };
        player_input.bind_spawn_context = [&](const ActorDefinition&, SourcePopulationSpawnContextV1& context,
                                              std::string&) {
            context.probability = 100;
            context.is_player = true;
            context.random0 = &player_rng;
            return true;
        };
        std::vector<SourcePopulationPlanRowV1> player_plan;
        check(plan_source_population_v1(player_input, player_plan, error), error);
        check(player_plan.size() == 1 && player_plan[0].status == SourcePopulationPlanStatusV1::include &&
              player_plan[0].decision_roll == -2 && player_plan[0].roll_cache_after == -2 &&
              player_rng.calls == 0,
              "Player source -2 sentinel must include without consuming the source RNG");

        // Strict source threshold test using an actual source RNG draw. Replaying
        // the same pre-state obtains the exact roll, so equality must fail while
        // the next signed probability passes.
        auto run_threshold = [&](std::int32_t probability) {
            ActorDefinition definition = moth2;
            definition.stableId += 900000;
            definition.sourceId += "#threshold";
            definition.properties["spawn_prob"] = std::to_string(probability);
            PopulationActor selected_actor;
            selected_actor.definition = definition;
            selected_actor.profileId = "test-selected-source-row";
            std::vector<ActorDefinition> one_definition{definition};
            std::vector<PopulationActor> one_actor;
            one_actor.push_back(std::move(selected_actor));
            auto random = dh2::data::LootRandom8V2{0xabcddcbau, 0};
            SourcePopulationPlannerInputV1 one;
            one.definitions = &one_definition;
            one.selected_actors = &one_actor;
            one.activation_decision = [](const ActorDefinition&) { return PopulationDecision::include; };
            one.bind_spawn_context = [&](const ActorDefinition&, SourcePopulationSpawnContextV1& context,
                                         std::string&) {
                context.probability = probability;
                context.random0 = &random;
                return true;
            };
            std::vector<SourcePopulationPlanRowV1> result;
            std::string local_error;
            check(plan_source_population_v1(one, result, local_error), local_error);
            check(result.size() == 1, "Planner omitted the source threshold row");
            return result[0];
        };
        auto zero_threshold = run_threshold(0);
        const auto exact_roll = zero_threshold.decision_roll;
        check(exact_roll >= 0 && exact_roll < 99,
              "Canonical planner RNG did not return an in-bound test roll");
        check(zero_threshold.status == SourcePopulationPlanStatusV1::exclude,
              "Probability zero must reject the nonnegative bounded source roll");
        check(zero_threshold.failed_spawn_visibility_requested &&
              zero_threshold.failed_spawn_delete_requested && zero_threshold.failed_spawn_mark_requested,
              "Failed source roll did not record lifecycle requests through diagnostics");
        const auto equal_threshold = run_threshold(exact_roll);
        check(equal_threshold.status == SourcePopulationPlanStatusV1::exclude &&
              equal_threshold.decision_roll == exact_roll &&
              equal_threshold.failed_spawn_visibility_requested &&
              equal_threshold.failed_spawn_delete_requested && equal_threshold.failed_spawn_mark_requested,
              "The source comparison is strict: roll equal to probability must fail");
        const auto above_threshold = run_threshold(exact_roll + 1);
        check(above_threshold.status == SourcePopulationPlanStatusV1::include &&
              above_threshold.decision_roll == -2 &&
              !above_threshold.failed_spawn_visibility_requested &&
              !above_threshold.failed_spawn_delete_requested && !above_threshold.failed_spawn_mark_requested,
              "The source comparison must pass when roll is strictly below probability");

        // A cached ordinary roll is not the -2 sentinel and still goes through
        // the source caller's signed threshold comparison without another draw.
        ActorDefinition cached_definition = moth2;
        cached_definition.stableId += 900001;
        cached_definition.sourceId += "#cached";
        cached_definition.properties["spawn_prob"] = "100";
        PopulationActor cached_actor;
        cached_actor.definition = cached_definition;
        cached_actor.profileId = "test-selected-source-row";
        std::vector<ActorDefinition> cached_definitions{cached_definition};
        std::vector<PopulationActor> cached_actors;
        cached_actors.push_back(std::move(cached_actor));
        SourcePopulationPlannerInputV1 cached_input;
        dh2::data::LootRandom8V2 cached_rng{55, 0};
        cached_input.definitions = &cached_definitions;
        cached_input.selected_actors = &cached_actors;
        cached_input.activation_decision = [](const ActorDefinition&) { return PopulationDecision::include; };
        cached_input.bind_spawn_context = [&](const ActorDefinition&, SourcePopulationSpawnContextV1& context,
                                              std::string&) {
            context.cached_roll = 98;
            context.probability = 100;
            context.random0 = &cached_rng;
            return true;
        };
        std::vector<SourcePopulationPlanRowV1> cached_plan;
        check(plan_source_population_v1(cached_input, cached_plan, error), error);
        check(cached_plan[0].status == SourcePopulationPlanStatusV1::include &&
              cached_plan[0].decision_roll == 98 && cached_rng.calls == 0,
              "A cached non-sentinel roll should be compared without a new draw");

        std::cout << "PASS actualMoths=2 playerSentinelNoDraw=true sharedNpcDraws=2 "
                     "strictThreshold=" << exact_roll << " unknownActivationGated=true\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "Runtime source population test FAIL: " << exception.what() << '\n';
        return 1;
    }
}
