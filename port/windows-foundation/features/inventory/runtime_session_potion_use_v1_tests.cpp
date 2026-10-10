#include "runtime_session_potion_use_v1.hpp"
#include "../../asset_catalog.hpp"
#include "../../combat_session.hpp"
#include "../../original_combat_visual_plan.hpp"
#include "../../original_combat_properties.hpp"
#include "../../../level-world/character_design_services.hpp"

#include <cassert>
#include <filesystem>
#include <iostream>
#include <map>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::inventory;

namespace {
void check(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

struct SourceDebug {
    dh2::character::DebugSwitches* owner{dh2_character_debug_create()};
    std::map<std::uintptr_t, std::string> strings;
    std::uintptr_t serial{1};
    dh2::character::DebugFileServices24 files{this,
        [](void*, const char*, std::uintptr_t* handle) { *handle = 0; return 0; },
        [](void*, std::uintptr_t) { return -1; }};
    dh2::character::skills::SkillAttackNativeServicesV6 services{this, invoke};

    ~SourceDebug() { dh2_character_debug_destroy(owner); }
    static int invoke(void* raw, const dh2::character::skills::SkillAttackNativeRequestV6* request,
                      std::uintptr_t* output) {
        auto& self = *static_cast<SourceDebug*>(raw);
        if (!request || !output || !self.owner) return -1;
        *output = 0;
        using namespace dh2::character::skills;
        switch (request->service) {
        case skill_attack_debug_load_v6:
            return dh2_character_debug_load(self.owner, &self.files) == 1 ? 0 : -1;
        case skill_attack_string_construct_v6:
            if (!request->name) return -1;
            *output = self.serial++;
            self.strings.emplace(*output, request->name);
            return 0;
        case skill_attack_debug_get_v6: {
            const auto found = self.strings.find(request->subject);
            if (found == self.strings.end()) return -1;
            std::uint32_t ignored{};
            return dh2_character_debug_get(&ignored, self.owner, found->second.c_str(), &self.files) == 1 ? 0 : -1;
        }
        case skill_attack_string_destroy_v6:
            return self.strings.erase(request->subject) == 1 ? 0 : -1;
        default: return -1;
        }
    }
};

struct FailedDebug {
    dh2::character::skills::SkillAttackNativeServicesV6 services{this,
        [](void*, const dh2::character::skills::SkillAttackNativeRequestV6*, std::uintptr_t*) { return -1; }};
};
}

int main(int argc, char** argv) try {
    check(argc == 2, "Supply repository root");
    const auto root = std::filesystem::path(argv[1]);
    AssetCatalog assets(root / ".local-inputs/windows-shared-assets");
    AssetCatalog bindings_assets(root / ".local-inputs/windows-melee-bindings");
    std::string error;
    OriginalPropertyDatabase database;
    check(load_original_property_tables(assets, "original-cache/data/pydata", database, error), error);
    dh2::data::PropertyRules rules;
    check(dh2::data::load_property_rules(database.characters, rules, error), error);
    OriginalMeleeBindings bindings;
    check(bindings.load(bindings_assets, "original-melee-bindings.xml", error), error);

    const std::string profile_id = "KnightPlayerBase";
    auto profile = std::make_shared<CharacterState>(make_default_character("potion-session-player", "Potion Player", profile_id));
    OriginalActorProperties fresh;
    check(resolve_original_fresh_player(database, profile_id, fresh, error), error);
    profile->stats.health = fresh.health;
    profile->stats.max_health = fresh.max_health;
    profile->stats.resource = fresh.resource;
    profile->stats.max_resource = fresh.max_resource;
    profile->inventory.push_back({"potion-instance-1", "Potion0", 5});
    ActorCustomization customization;
    customization.skin_id_contains = "_default_warrior-mesh-skin";
    customization.expected_controller_count = 4;
    customization.allow_missing_animation_targets = true;
    OriginalCombatVisualPlan visual;
    check(build_original_combat_visual_plan(assets, bindings, profile_id, customization,
          "potion-use-v1", visual, error), error);

    CombatSessionConfig config;
    config.diagnosticRngSeed = 11;
    config.playerId = 1;
    config.playerProfileId = profile_id;
    config.selectedPlayerProfile = profile.get();
    config.tableRoot = "original-cache/data/pydata";
    config.playerVisualConfig = visual.config;
    config.playerVisualConfig.motion_node_id = "auto";
    const auto* idle = visual.phase("Idle", 0, {0});
    check(idle, "Authored Knight idle phase absent");
    config.playerVisualConfig.clips = {{"idle", idle->resolvedPath}};
    CombatSessionProfile player;
    player.initialIdle = {"Idle", 0, {0}};
    player.damageMarkerNames = {"attack_mainhand"};
    OriginalAttackSelection attack;
    attack.state = "AttackStatic";
    player.sequenceAction = attack;
    player.retainedPhaseClock = true;
    player.sourceCombo = true;
    player.propertyOptions = {256, true};
    config.profiles.emplace(profile_id, player);
    CharacterVisual player_visual;
    CombatSession session;
    ActorPopulation population;
    check(session.initialize(assets, database, bindings, config, player_visual, population,
          {0, 0, 0}, customization, error), error);
    auto* world = session.world();
    auto* actor = session.actor(1);
    check(world && actor && actor->persistent_character_id &&
          *actor->persistent_character_id == profile->id,
          "Same live player/profile binding was not retained by CombatSession");

    auto item_bytes = assets.read("original-cache/data/pydata/loot_table_pyarray.bin");
    auto item_names = assets.read("original-cache/data/pydata/loot_table_pyarraynames.bin");
    auto item_fields = assets.read("original-cache/data/pydata/loot_table_pystructnames.bin");
    dh2::data::ItemTable items;
    check(dh2::data::load_items({item_bytes.data(), item_bytes.size()},
          {item_names.data(), item_names.size()}, {item_fields.data(), item_fields.size()}, items, error), error);
    const auto potion_id = dh2::data::item_id(items, "Potion0");
    const auto* potion_row = dh2::data::item(items, potion_id);
    check(potion_id == 925 && potion_row && dh2::data::item_type(*potion_row) == 14,
          "Source ItemTable Potion0 row/type changed");

    const auto* bound = world->combat_properties(1);
    check(bound, "Player source properties missing");
    profile->stats.health = actor->health = actor->max_health * 0.5f;
    profile->stats.resource = actor->resource = actor->max_resource * 0.25f;
    const auto potion_count_before = bound->sheets.resolved[219];
    SourceDebug debug;
    check(debug.owner, "Source DebugSwitches owner creation failed");
    RuntimePotionUseServicesV1 services{&items, &rules, &debug.services};
    RuntimeSessionPotionUseV1 use;
    RuntimePotionUseReceiptV1 receipt;
    dh::foundation::platform_input::ButtonEdges pressed{true, false};
    check(use.dispatch(session, 1, *profile, pressed, services, receipt, error), error);
    bound = world->combat_properties(1);
    check(receipt.result == RuntimePotionUseResultV1::used && receipt.consumed &&
          profile->inventory.size() == 1 && profile->inventory.front().quantity == 4,
          "Source Potion0 press did not consume exactly one from the same CharacterState");
    check(receipt.hp_before < bound->sheets.resolved[38] && receipt.mp_before < bound->sheets.resolved[43] &&
          bound->sheets.resolved[36] == bound->sheets.resolved[38] &&
          bound->sheets.resolved[41] == bound->sheets.resolved[43] &&
          actor->health == actor->max_health && actor->resource == actor->max_resource &&
          profile->stats.health == actor->health && profile->stats.resource == actor->resource,
          "Source RegenHP→RegenMP did not refill and publish the same live vitals: result="+
          std::to_string(static_cast<int>(receipt.result))+" hp="+std::to_string(bound->sheets.resolved[36])+"/"+
          std::to_string(bound->sheets.resolved[38])+" mp="+std::to_string(bound->sheets.resolved[41])+"/"+
          std::to_string(bound->sheets.resolved[43])+" actor="+std::to_string(actor->health)+"/"+
          std::to_string(actor->max_health)+" resource="+std::to_string(actor->resource)+"/"+
          std::to_string(actor->max_resource)+" profile="+std::to_string(profile->stats.health)+"/"+
          std::to_string(profile->stats.max_health)+" resource="+std::to_string(profile->stats.resource)+"/"+
          std::to_string(profile->stats.max_resource));
    check(bound->sheets.resolved[219] == potion_count_before + 256 &&
          profile->stats.max_health == actor->max_health && profile->stats.max_resource == actor->max_resource,
          "Source potion achievement count or canonical max vitals changed");
    check(debug.strings.empty(), "Source Debug StringManager token leaked after Regen queries");

    const auto inventory_after_use = profile->inventory;
    dh::foundation::platform_input::ButtonEdges held{false, true};
    check(use.dispatch(session, 1, *profile, held, services, receipt, error), error);
    check(receipt.result == RuntimePotionUseResultV1::not_pressed &&
          profile->inventory.front().quantity == inventory_after_use.front().quantity,
          "Held key repeated potion use without a new pressed edge");
    check(use.dispatch(session, 1, *profile, pressed, services, receipt, error), error);
    check(receipt.result == RuntimePotionUseResultV1::vitals_full && !receipt.consumed &&
          profile->inventory.front().quantity == inventory_after_use.front().quantity,
          "Full HP+MP admitted a source potion consumption");

    // The source RemoveOnePotion path clears/destroys the instance when its
    // signed count is one. A missing Debug provider fails after that exact
    // source prefix, while source properties/CharacterState remain published.
    profile->inventory.front().quantity = 1;
    actor->health = profile->stats.health = actor->max_health * 0.5f;
    const auto count_before_failure = world->combat_properties(1)->sheets.resolved[219];
    RuntimePotionUseServicesV1 missing_debug{&items, &rules, nullptr};
    check(!use.dispatch(session, 1, *profile, pressed, missing_debug, receipt, error),
          "Missing required source Debug provider incorrectly regenerated vitals");
    bound = world->combat_properties(1);
    check(receipt.result == RuntimePotionUseResultV1::consumed_prefix_failure && receipt.consumed &&
          profile->inventory.empty() && bound->sheets.resolved[219] == count_before_failure + 256 &&
          bound->sheets.resolved[36] < bound->sheets.resolved[38] &&
          actor->health < actor->max_health,
          "Reached RegenHP failure did not retain exactly the consumed-item/property prefix");

    // B043 regression: an MP spend made like the original Character::UseMana
    // (PropertyAdd on property 41 with the live actor MP re-synced) must leave
    // the source sheet consistent, so the next potion press syncs and uses.
    profile->inventory.push_back({"potion-instance-2", "Potion0", 2});
    {
        auto spent = world->combat_properties(1)->sheets;
        auto spent_view = dh2::data::property_view(rules, spent);
        check(dh2_property_add(&spent_view, 41, -10 * 256) == 0, "Source UseMana-style PropertyAdd failed");
        OriginalCombatProperties next = *world->combat_properties(1);
        next.sheets = spent;
        check(world->update_combat_properties(1, std::move(next), *world->traits(1), error), error);
        actor->resource = float(spent.resolved[41]) / 256.0f;
        profile->stats.resource = actor->resource;
    }
    check(use.dispatch(session, 1, *profile, pressed, services, receipt, error) &&
          receipt.result == RuntimePotionUseResultV1::used && receipt.consumed &&
          profile->inventory.front().quantity == 1 &&
          actor->resource == actor->max_resource,
          "Potion press after a source MP spend failed to sync live vitals: result=" +
          std::to_string(static_cast<int>(receipt.result)) + " error=" + error);

    // A new press with no potion is a source no-op.
    profile->inventory.clear();
    check(use.dispatch(session, 1, *profile, pressed, services, receipt, error), error);
    check(receipt.result == RuntimePotionUseResultV1::no_potion && profile->inventory.empty(),
          "Empty source potion owner changed state");
    std::cout << "{\"validation\":\"PASS\",\"item_id\":925,\"item_type\":14,\"source_press_edge\":true,\"source_regen_order\":\"HP_then_MP\",\"same_session_character_state\":true,\"held_repeat_rejected\":true,\"full_vitals_rejected\":true,\"source_prefix_failure_retained\":true}\n";
    return 0;
} catch (const std::exception& exception) {
    std::cerr << exception.what() << '\n';
    return 1;
}
