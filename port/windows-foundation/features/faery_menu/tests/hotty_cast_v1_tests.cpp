#include "../hotty_cast_v1.hpp"
#include "../../../original_actor_properties.hpp"
#include "../../../../game-data/ai.hpp"
#include "../../../../game-data/data.hpp"
#include "../../../../game-data/effects_tables.hpp"
#include "../hotty_effects_v1.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

namespace {
using Bytes = std::vector<std::uint8_t>;
void check(bool ok, const char* message) {
    if (!ok) throw std::runtime_error(message);
}
Bytes file(const std::string& path) {
    std::ifstream stream(path, std::ios::binary);
    check(bool(stream), "missing original source table input");
    return {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
}
dh2::data::Bytes view(const Bytes& bytes) { return {bytes.data(), bytes.size()}; }

dh::foundation::OriginalCombatProperties combat_properties(
    const dh::foundation::OriginalActorProperties& actor) {
    dh::foundation::OriginalCombatProperties output;
    output.sheets = actor.sheets;
    output.facts.main_damage_class = -1;
    output.facts.off_damage_class = -1;
    return output;
}

dh::foundation::ActorState actor_state(std::uint64_t id, const std::string& def,
    const dh::foundation::OriginalActorProperties& properties, float x,
    std::string persistent = {}) {
    dh::foundation::ActorState actor;
    actor.id = id;
    actor.definition_id = def;
    actor.class_id = def;
    actor.faction_id = properties.faction_id;
    actor.transform.position = {x, 0.0f, 0.0f};
    actor.health = properties.health;
    actor.max_health = properties.max_health;
    actor.resource = properties.resource;
    actor.max_resource = properties.max_resource;
    if (!persistent.empty()) actor.persistent_character_id = std::move(persistent);
    return actor;
}
}

int main(int argc, char** argv) {
    using namespace dh::foundation;
    using namespace dh::foundation::faery_menu;
    try {
        check(argc == 5, "expected actual source pydata, Faery table root, Hotty Lua source, and EffectsTables root");
        const std::string root = argv[1];
        const std::string py = root + "/pydata/";
        auto properties_raw = file(py + "character_properties_pyarray.bin");
        auto properties_names = file(py + "character_properties_pyarraynames.bin");
        auto properties_schema = file(py + "character_properties_pystructnames.bin");
        auto classes_raw = file(py + "character_classes_pyarray.bin");
        auto classes_names = file(py + "character_classes_pyarraynames.bin");
        auto classes_schema = file(py + "character_classes_pystructnames.bin");
        OriginalPropertyDatabase property_db;
        std::string error;
        check(dh2::data::load_characters(view(properties_raw), view(properties_names),
              view(properties_schema), property_db.characters, error),
              "actual CharacterProperties table rejected");
        check(dh2::data::load_classes(view(classes_raw), view(classes_names),
              view(classes_schema), property_db.classes, error),
              "actual CharacterClasses table rejected");
        dh2::data::PropertyRules rules;
        check(dh2::data::load_property_rules(property_db.characters, rules, error),
              "actual source PropertyRules rejected");

        const std::string effect_root = argv[4];
        auto effects_raw = file(effect_root + "/effects_pyarray.bin");
        auto effects_names = file(effect_root + "/effects_pyarraynames.bin");
        auto effects_schema = file(effect_root + "/effects_pystructnames.bin");
        auto effects_dictionary_names = file(effect_root + "/effects_dictionary_pyarraynames.bin");
        auto effects_dictionary_paths = file(effect_root + "/effects_dictionary_pyarray.bin");
        dh2::data::EffectsTables effect_tables;
        check(effect_tables.load(view(effects_raw), view(effects_names), view(effects_schema),
              view(effects_dictionary_names), view(effects_dictionary_paths), error),
              "actual source EffectsTables rejected");
        HottyEffectIdsV1 effect_ids;
        check(resolve_hotty_effect_ids_v1(effect_tables.borrow(), effect_ids, error),
              "original Hotty animated effect names did not resolve");
        const auto effect_borrow = effect_tables.borrow();
        check(effect_borrow.set_names().at(std::size_t(effect_ids.player_pre)) ==
                  "Hotty_Level_1_Player_Pre" &&
              effect_borrow.set_names().at(std::size_t(effect_ids.character_target)) ==
                  "Hotty_Level_1_Target" &&
              !effect_borrow.sets().at(std::size_t(effect_ids.player_pre)).steps.empty() &&
              !effect_borrow.sets().at(std::size_t(effect_ids.character_target)).steps.empty(),
              "Hotty FX binding did not preserve the authored table rows/steps");
        const auto& player_pre_set = effect_borrow.sets().at(static_cast<std::size_t>(effect_ids.player_pre));
        const auto& target_set = effect_borrow.sets().at(static_cast<std::size_t>(effect_ids.character_target));
        check(player_pre_set.type == 0 && target_set.type == 0 &&
              player_pre_set.steps.front().file >= 0 && target_set.steps.front().file >= 0 &&
              static_cast<std::size_t>(player_pre_set.steps.front().file) < effect_borrow.dictionary().values.size() &&
              static_cast<std::size_t>(target_set.steps.front().file) < effect_borrow.dictionary().values.size() &&
              effect_borrow.dictionary().values[static_cast<std::size_t>(player_pre_set.steps.front().file)] ==
                  "data/3D/interface/skill_dh2_faery_fire.bdae" &&
              effect_borrow.dictionary().values[static_cast<std::size_t>(target_set.steps.front().file)] ==
                  "data/3D/interface/spell_dh2_faery_fire.bdae",
              "Hotty FX source paths/type differ from the exact EffectTables records");

        auto faery_raw = file(std::string(argv[2]) + "/faeries_pyarray.bin");
        auto faery_names = file(std::string(argv[2]) + "/faeries_pyarraynames.bin");
        auto faery_schema = file(std::string(argv[2]) + "/faeries_pystructnames.bin");
        dh2::data::FaeryTables faeries;
        check(faeries.load(view(faery_raw), view(faery_names), view(faery_schema), error),
              "actual Faery table rejected");
        const auto table_borrow = faeries.borrow();
        if (!(table_borrow.lists().at(2) == std::vector<std::int32_t>({1, 13, 14, 15, 7}) &&
              table_borrow.faery_names()[7] == "Hotty" &&
              table_borrow.faeries()[7].script == "faerie_hotty")) {
            std::cerr << "source list=" << table_borrow.list_names().front()
                      << " row7=" << table_borrow.faery_names()[7]
                      << " script=" << table_borrow.faeries()[7].script << '\n';
            throw std::runtime_error("actual Mage FaeryList/Hotty source script differs");
        }
        const auto hotty_script = file(argv[3]);
        const std::string hotty_text(hotty_script.begin(), hotty_script.end());
        check(hotty_text.find("ToFixed(0.2)") != std::string::npos &&
              hotty_text.find("local RANGE\t\t\t\t= {600, 800}") != std::string::npos &&
              hotty_text.find("SpellCombatRoll(target)") != std::string::npos,
              "actual faerie_hotty.luac source program differs from the reviewed action");

        std::int32_t level{}, cost{};
        check(hotty_mana_cost_v1(0, 1 * 256, level, cost, error) && level == 0 && cost == 10 * 256,
              "source rank0 ToFixed/MulFixed Hotty cost differs");
        check(hotty_mana_cost_v1(1, 100 * 256, level, cost, error) && level == 1 && cost == 20 * 256,
              "source rank1 cost incorrectly applies its ToFixed(0.2) slope");
        check(hotty_mana_cost_v1(65535, 1000 * 256, level, cost, error) &&
              level == 1 && cost == 20 * 256,
              "Hotty source level cap or fixed cost differs");

        OriginalActorProperties player_properties, enemy_properties;
        check(resolve_original_fresh_player(property_db, "KnightPlayerBase",
              player_properties, error), "actual Knight source player property sheet rejected");
        check(resolve_original_actor_properties(property_db.characters, property_db.classes,
              "Swamp_LizadMan_Type1", {std::nullopt, true}, enemy_properties, error),
              "actual target Character property sheet rejected");
        const auto player_record = combat_properties(player_properties);
        const auto enemy_record = combat_properties(enemy_properties);

        auto ai_raw = file(py + "ai_pyarray.bin");
        auto ai_names = file(py + "ai_pyarraynames.bin");
        auto ai_schema = file(py + "ai_pystructnames.bin");
        auto factions_raw = file(py + "ai_factions_pyarray.bin");
        auto factions_names = file(py + "ai_factions_pyarraynames.bin");
        auto factions_schema = file(py + "ai_factions_pystructnames.bin");
        dh2::data::AiTables ai;
        check(dh2::data::load_ai(view(ai_raw), view(ai_names), view(ai_schema),
              view(factions_raw), view(factions_names), view(factions_schema), ai, error),
              "actual source AI/faction tables rejected");
        dh2::data::CombatRandom random{0x12345678u, 0};
        PlayableActorWorld world(std::move(ai), random);
        auto player = actor_state(1, "KnightPlayerBase", player_properties, 0.0f, "hotty-test");
        auto enemy = actor_state(2, "Swamp_LizadMan_Type1", enemy_properties, 700.0f);
        check(world.bind_actor(std::move(player), player_record, {true, true, {}}, error),
              "same-world actual source player did not bind");
        if (!world.bind_actor(std::move(enemy), enemy_record, {false, true, {}}, error))
            throw std::runtime_error("same-world actual source target did not bind: " + error);

        std::vector<ActorId> targets;
        check(source_hotty_character_targets_v1(world, 1, 600, targets, error),
              "same-world source Hotty target query failed");
        check(targets.empty(),
              "same-world 600-unit Hotty query included an out-of-range target");
        check(source_hotty_character_targets_v1(world, 1, 800, targets, error) &&
              targets == std::vector<ActorId>({2}),
              "same-world upgraded 800-unit Hotty query missed the actual source target");

        CharacterState character;
        character.id = "hotty-test";
        character.name = "Knight";
        character.class_id = "mage";
        character.source_faery_state_known = true;
        character.source_faery_list_id = 2;
        character.faery_by_difficulty[0].current_faery = 4;
        character.faery_by_difficulty[0].faeries[4].state = 1;
        character.faery_by_difficulty[0].faeries[4].level = 1;
        character.stats.resource = player_properties.resource;
        HottyCooldownClockV1 clock;
        check(advance_hotty_cooldown_clock_v1(1, 0.016, clock, error),
              "source session update serial 1 was not accepted");
        HottySourcePolicyV1 policy;
        policy.complete = true;
        const auto mp_before_unknown_target_list =
            world.combat_properties(1)->sheets.resolved[41];
        HottySourceTargetListV1 unknown_targets;
        unknown_targets.character_targets_in_source_order = {2};
        HottyPreparedCastV1 rejected_unknown_targets;
        check(!prepare_hotty_spell_v1(world, 1, 1, character, 0, table_borrow,
              property_db.classes, rules, policy, clock, rejected_unknown_targets,
              error, &unknown_targets) &&
              error.find("complete current Character-population") != std::string::npos &&
              world.combat_properties(1)->sheets.resolved[41] == mp_before_unknown_target_list &&
              clock.spell_ready_at_ms.empty(),
              "unknown source NoSort ordering mutated mana/cooldown instead of failing before commit");
        HottySourceTargetListV1 object_targets;
        object_targets.query_complete = true;
        object_targets.authored_world_order_preserved = true;
        object_targets.scope = HottyTargetScopeV1::source_object_targets_included;
        object_targets.character_targets_in_source_order = {2};
        HottyPreparedCastV1 rejected_object_targets;
        check(!prepare_hotty_spell_v1(world, 1, 1, character, 0, table_borrow,
              property_db.classes, rules, policy, clock, rejected_object_targets,
              error, &object_targets) &&
              error.find("complete current Character-population") != std::string::npos &&
              world.combat_properties(1)->sheets.resolved[41] == mp_before_unknown_target_list &&
              clock.spell_ready_at_ms.empty(),
              "unsupported source GameObject branch mutated mana/cooldown instead of failing before commit");
        HottySourceTargetListV1 source_targets;
        HottyAuthoredActorOrderV1 actor_order;
        actor_order.complete = true;
        actor_order.actor_ids = {1, 2};
        const std::vector<HottySourceActorFactsV1> source_actor_facts{
            {2, true, false, {}, {}, true}};

        // GameObject::GetTargetPosition returns transform+0x160 when
        // node180==0, even though the constructor has already seeded cache184
        // with {0,0,0}. Exercise the exact-list validator independently of the
        // query so a forged/stale near-zero cache cannot admit an offscreen
        // target before mana/cooldown commit.
        auto* live_player = world.find_actor(1);
        auto* live_enemy = world.find_actor(2);
        check(live_player && live_enemy, "same-world Hotty actors disappeared");
        live_player->source_target_node180 = std::uintptr_t(0);
        live_player->source_target_position184 = std::array<float, 3>{0, 0, 0};
        live_enemy->source_target_node180 = std::uintptr_t(0);
        live_enemy->source_target_position184 = std::array<float, 3>{0, 0, 0};
        live_enemy->transform.position = {2000.0f, 0.0f, 0.0f};
        HottySourceTargetListV1 stale_zero_cache_list;
        stale_zero_cache_list.query_complete = true;
        stale_zero_cache_list.authored_world_order_preserved = true;
        stale_zero_cache_list.scope = HottyTargetScopeV1::current_character_population;
        stale_zero_cache_list.character_targets_in_source_order = {2};
        const auto mp_before_stale_zero_cache = world.combat_properties(1)->sheets.resolved[41];
        HottyPreparedCastV1 rejected_stale_zero_cache;
        check(!prepare_hotty_spell_v1(world, 1, 1, character, 0, table_borrow,
              property_db.classes, rules, policy, clock, rejected_stale_zero_cache,
              error, &stale_zero_cache_list) &&
              error.find("outside 600/800 range") != std::string::npos &&
              world.combat_properties(1)->sheets.resolved[41] == mp_before_stale_zero_cache &&
              clock.spell_ready_at_ms.empty(),
              "Hotty exact-list validation trusted the zero cache when original node180 was null");

        // With actual nonzero authored nodes, the same source getter selects
        // cache184 over transform. A far transform plus 700-unit cache is the
        // complementary branch and remains in the 800-unit query.
        live_enemy->transform.position = {5000.0f, 0.0f, 0.0f};
        live_player->source_target_node180 = std::uintptr_t(10);
        live_player->source_target_position184 = std::array<float, 3>{0, 0, 0};
        live_enemy->source_target_node180 = std::uintptr_t(20);
        live_enemy->source_target_position184 = std::array<float, 3>{700, 0, 0};
        if (!query_hotty_character_targets_v1(world, 1, actor_order,
              source_actor_facts, 800, source_targets, error))
            throw std::runtime_error("Hotty cached-node Character target query failed: " + error);
        check(source_targets.query_complete && source_targets.authored_world_order_preserved &&
              source_targets.scope == HottyTargetScopeV1::current_character_population &&
              source_targets.character_targets_in_source_order == std::vector<ActorId>{2},
              "Hotty nonzero-node query did not use the authored cache and preserve ActorPopulation order/scope");

        live_player->source_target_node180 = std::uintptr_t(0);
        live_player->source_target_position184 = std::array<float, 3>{0, 0, 0};
        live_enemy->source_target_node180 = std::uintptr_t(0);
        live_enemy->source_target_position184 = std::array<float, 3>{0, 0, 0};
        live_enemy->transform.position = {2000.0f, 0.0f, 0.0f};
        HottySourceTargetListV1 null_node_query;
        check(query_hotty_character_targets_v1(world, 1, actor_order,
              source_actor_facts, 800, null_node_query, error),
              "Hotty null-node Character target query failed");
        check(null_node_query.query_complete && null_node_query.character_targets_in_source_order.empty(),
              "Hotty null-node query admitted an offscreen actor from constructor cache {0,0,0}");

        // Restore the ordinary world-transform setup for the remaining source
        // formula and mana tests.
        live_player->source_target_node180.reset();
        live_player->source_target_position184.reset();
        live_enemy->source_target_node180.reset();
        live_enemy->source_target_position184.reset();
        live_enemy->transform.position = {700.0f, 0.0f, 0.0f};
        if (!query_hotty_character_targets_v1(world, 1, actor_order,
              source_actor_facts, 800, source_targets, error))
            throw std::runtime_error("Hotty current Character target query failed: " + error);
        check(source_targets.query_complete && source_targets.authored_world_order_preserved &&
              source_targets.scope == HottyTargetScopeV1::current_character_population &&
              source_targets.character_targets_in_source_order == std::vector<ActorId>{2},
              "Hotty current Character query did not preserve authored ActorPopulation order/scope");
        HottyPreparedCastV1 prepared;
        if (!prepare_hotty_spell_v1(world, 1, 1, character, 0, table_borrow,
              property_db.classes, rules, policy, clock, prepared, error,
              &source_targets))
            throw std::runtime_error("same-world Hotty source prefix rejected actual source state: " + error);
        check(prepared.status == HottyPrepareStatusV1::prepared_pending_spell_combat &&
              prepared.faery_slot == 4 && prepared.faery_record_id == 7 &&
              prepared.skill_level == 1 && prepared.fixed_mana_cost == 20 * 256 &&
              prepared.range == 800 && prepared.source_spell_type ==
                  static_cast<std::int32_t>(table_borrow.faeries()[3].scalar.words[7]) &&
              prepared.character_targets == std::vector<ActorId>({2}) &&
              prepared.original_target_order_known,
              "Hotty source prefix did not preserve same-world/list/source facts");
        check(prepared.mana_debited && character.stats.resource == player_properties.resource - 20.0f &&
              world.find_actor(1)->resource == character.stats.resource &&
              world.combat_properties(1)->sheets.resolved[41] ==
                  player_properties.sheets.resolved[41] - 20 * 256,
              "same-world UseMana prefix failed to debit the shared source MP/property state");
        // Regression (B043): the debit must live in the saved-based MP property,
        // so a full re-resolve keeps it (a direct resolved[] write was reverted).
        {
            auto recalculated = world.combat_properties(1)->sheets;
            std::string recalc_error;
            check(dh2::data::recalc_properties(rules, recalculated, recalc_error) &&
                  recalculated.resolved[41] == world.combat_properties(1)->sheets.resolved[41],
                  "Hotty UseMana MP debit did not survive a source property re-resolve");
        }
        OriginalMeleeResolution hotty_result;
        const auto old_target_health = world.find_actor(2)->health;
        check(world.resolve_source_result("faerie_hotty", 1, 2, "SpellCombatRoll",
              0x1005554Au, -1, prepared.source_spell_type, 0,
              hotty_result, error, &prepared.spell_properties),
              "actual Hotty SpellCombatRoll source formula did not resolve on the same world");
        check(hotty_result.original.mask == 0x1005554Au &&
              world.find_actor(2)->health == old_target_health,
              "Hotty same-world calculated result changed authored mask or applied health before the lifecycle owner");
        HottyPreparedCastV1 second{};
        check(!prepare_hotty_spell_v1(world, 1, 1, character, 0, table_borrow,
              property_db.classes, rules, policy, clock, second, error) &&
              error.find("cooldown") != std::string::npos,
              "same-session Hotty cooldown did not reject a duplicate cast");

        check(advance_hotty_cooldown_clock_v1(2, 4.984, clock, error),
              "next session update was not accepted");
        check(!clock.spell_ready_at_ms.empty(),
              "Hotty 5000ms cooldown expired before five source seconds");
        check(advance_hotty_cooldown_clock_v1(3, 0.016, clock, error),
              "third session update was not accepted");
        check(clock.spell_ready_at_ms.empty(),
              "Hotty 5000ms cooldown did not expire at same-session elapsed time");
        std::cout << "Hotty source preparation/query: actual tables, same world/CharacterState, target-order gates, mana debit, cooldown, and named authored FX rows passed; CombatSession apply adapter is compile-checked but not exercised by this fixture.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
