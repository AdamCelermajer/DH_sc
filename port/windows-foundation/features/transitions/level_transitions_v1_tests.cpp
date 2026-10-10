// P17 TRANSITIONS tests. Arguments:
//   1. package asset root with data/levels_pyarray.bin (the level table; Swamp rc3 assets)
//   2. Android asset root with data/scene/001_swamp.mlx, data/scene/003_darkwood.mlx and their modules
#include "level_transitions_v1.hpp"
#include "../menu_metadata/menu_metadata_v1.hpp"

#include <filesystem>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::transitions;
namespace fs = std::filesystem;

namespace {

void check(bool value, const std::string& message) { if (!value) throw std::runtime_error(message); }

ActorDefinition block(const std::string& gametype, const std::string& name,
                      std::map<std::string, std::string> properties, float x = 0, float y = 0, float z = 0) {
    ActorDefinition d;
    d.gametype = gametype;
    d.name = name;
    d.sourcePath = "synthetic/test.mgp";
    d.placement = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x, y, z, 1};
    d.properties = std::move(properties);
    d.properties.emplace("type", "Block");
    return d;
}

std::vector<LevelRow> rows() {
    return {{"DARKWOOD", "003_darkwood.mlx"}, {"SWAMP", "001_swamp.mlx"}, {"SWAMP_CAVE_WITCH_A", "002_swamp_witchcave.rule.xml"}};
}

void test_builders_and_registry() {
    ZoneClassRegistry registry;
    check(registry.find("TriggerZoneExitLevel") && registry.find("CheckpointZone") && registry.find("QuestMoveInZone"),
          "registry binds the three transition classes");
    check(!registry.find("TriggerZone"), "plain TriggerZone is not a transition class");

    std::vector<ActorDefinition> declarations{
        block("TriggerZoneExitLevel", "exit_ok", {{"levelName", "DARKWOOD"}, {"entrypointID", "7"}, {"condition_desc", "Invalid"}}, 10, 0, 0),
        block("TriggerZoneExitLevel", "exit_gated", {{"levelName", "SWAMP"}, {"entrypointID", "13"}, {"activate_cond", "QuestDone_Witch"}}),
        block("TriggerZoneExitLevel", "exit_missing", {{"levelName", "NO_SUCH_LEVEL"}}),
        block("TriggerZoneExitLevel", "exit_procedural", {{"levelName", "SWAMP_CAVE_WITCH_A"}, {"entrypointID", "6"}}),
        block("CheckpointZone", "cp", {}),
        block("QuestMoveInZone", "quest", {}),
        block("TriggerZone", "plain", {{"script", "X"}}),
    };
    ActorDefinition shaped = block("TriggerZoneExitLevel", "exit_sphere", {{"levelName", "SWAMP"}});
    shaped.properties["type"] = "Sphere";
    declarations.push_back(shaped);

    ExitZoneSet set;
    std::string error;
    check(set.build(declarations, rows(), registry, error), "build: " + error);
    const auto& zones = set.zones();
    check(zones.size() == 7, "one entry per transition declaration (plain TriggerZone ignored)");

    const auto& ok = zones[set.find("exit_ok")];
    check(ok.built && ok.kind == ZoneKind::level_exit, "exit to a table level is built");
    check(ok.target_file == "data/scene/003_darkwood.mlx", "target file is data/scene/<table file>: " + ok.target_file);
    check(ok.entry_id == 7 && ok.condition.empty() && !ok.target_procedural, "entry id parsed, Invalid condition is unconditional");

    const auto& gated = zones[set.find("exit_gated")];
    check(gated.built && gated.condition == "QuestDone_Witch", "activate_cond is the authored condition");

    const auto& missing = zones[set.find("exit_missing")];
    check(!missing.built && missing.note.find("not in the level table") != std::string::npos, "unknown levelName is reported, not built");

    const auto& procedural = zones[set.find("exit_procedural")];
    check(procedural.built && procedural.target_procedural, "procedural target is built and flagged, so the transition can refuse it");

    const auto& sphere = zones[set.find("exit_sphere")];
    check(!sphere.built && sphere.note.find("only Block") != std::string::npos, "non-Block shape is reported");

    check(zones[set.find("cp")].kind == ZoneKind::checkpoint && zones[set.find("cp")].built, "checkpoint zone is built");
    check(!zones[set.find("quest")].built && zones[set.find("quest")].kind == ZoneKind::delegated_quest, "QuestMoveInZone is delegated");
    check(set.built_count(ZoneKind::level_exit) == 3 && set.built_count(ZoneKind::checkpoint) == 1, "built counts");
    check(!set.notices().empty(), "rejected zones are kept in notices");
}

void test_edges() {
    ZoneClassRegistry registry;
    std::vector<ActorDefinition> declarations{
        block("TriggerZoneExitLevel", "exit", {{"levelName", "DARKWOOD"}}, 1000, 0, 0),
        block("CheckpointZone", "cp", {}, -1000, 0, 0),
    };
    ExitZoneSet set;
    std::string error;
    check(set.build(declarations, rows(), registry, error), "edge build: " + error);
    const std::size_t exit = set.find("exit");
    const std::size_t cp = set.find("cp");

    // Arrival inside a zone records the state without an event.
    check(set.update({1000, 0, 0}, true).empty(), "arrival inside a zone emits nothing");
    check(set.zones()[exit].inside, "arrival inside is recorded");
    // Staying inside is not a new begin.
    check(set.update({1000, 0, 0}, true).empty(), "staying inside emits nothing");
    // Leaving and re-entering is a new begin.
    check(set.update({0, 0, 0}, true).empty(), "leaving emits nothing");
    auto events = set.update({1000, 0, 0}, true);
    check(events.size() == 1 && events[0].zone == exit && events[0].kind == ZoneKind::level_exit, "re-entry emits the exit begin");
    // A dead player is outside.
    check(set.update({1000, 0, 0}, false).empty() && !set.zones()[exit].inside, "dead player is not inside");
    // Checkpoint begin from outside.
    events = set.update({-1000, 0, 0}, true);
    check(events.size() == 1 && events[0].zone == cp && events[0].kind == ZoneKind::checkpoint, "checkpoint begin is emitted once");
    check(set.zones()[cp].inside, "checkpoint inside recorded");
}

void test_spawn_resolution() {
    std::vector<ActorDefinition> declarations;
    auto spawn = [](const std::string& name, const std::string& entry, float x) {
        ActorDefinition d;
        d.gametype = "SpawnPoint";
        d.name = name;
        d.properties["entrypointID"] = entry;
        d.placement = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x, 2 * x, 3 * x, 1};
        return d;
    };
    declarations.push_back(spawn("a", "0", 1));
    declarations.push_back(spawn("b", "5", 2));
    declarations.push_back(spawn("c", "0", 3));
    const auto first = resolve_entry_spawn(declarations, 0);
    check(first.found && first.name == "a" && first.candidates == 2, "first SpawnPoint in placement order wins");
    check(first.position[0] == 1 && first.position[1] == 2 && first.position[2] == 3, "spawn position is the placement translation");
    check(!resolve_entry_spawn(declarations, 9).found, "absent entry id resolves to nothing");
    check(resolve_entry_spawn(declarations, 5).name == "b", "entry 5 resolves to its own SpawnPoint");
}

void test_condition_stub() {
    LoggedConditionStub stub;
    check(stub.condition_active("QuestDone_Witch") && stub.condition_active("QuestDone_Witch"), "stub answers true");
    check(stub.logged().size() == 1, "stub logs each name once");
}

void test_real_levels(const fs::path& swamp_root, const fs::path& darkwood_root) {
    std::string error;
    dh2::data::LevelTables tables;
    {
        AssetCatalog catalogue(swamp_root);
        check(menu_metadata::load_level_tables(catalogue, tables, error), "level table: " + error);
    }
    const auto levels = level_rows(tables);
    check(find_level(levels, "DARKWOOD") && find_level(levels, "DARKWOOD")->file == "003_darkwood.mlx", "DARKWOOD row names 003_darkwood.mlx");

    std::vector<ActorDefinition> swamp;
    {
        AssetCatalog catalogue(swamp_root);
        check(load_actor_definitions(catalogue, "data/scene/001_swamp.mlx", swamp, error), "Swamp declarations: " + error);
    }
    ExitZoneSet swamp_zones;
    ZoneClassRegistry registry;
    check(swamp_zones.build(swamp, levels, registry, error), "Swamp zones: " + error);
    const auto to_darkwood = swamp_zones.find("_prim_ExitLevelZone_001");
    check(to_darkwood != ExitZoneSet::npos, "Swamp bossroom exit present in the loaded level");
    const auto& dw = swamp_zones.zones()[to_darkwood];
    check(dw.built && dw.target_name == "DARKWOOD" && dw.target_file == "data/scene/003_darkwood.mlx" && dw.entry_id == 0,
          "Swamp -> Darkwood exit: built, entry 0");
    check(!dw.target_procedural, "Darkwood target is a fixed map");
    check(swamp_zones.built_count(ZoneKind::checkpoint) >= 1, "Swamp checkpoint zones are built");

    std::size_t procedural_exits = 0;
    for (const auto& zone : swamp_zones.zones()) if (zone.built && zone.target_procedural) ++procedural_exits;
    check(procedural_exits >= 2, "Swamp exits to procedural levels are flagged (witch cave, Swamp 2)");

    const auto swamp_entry_13 = resolve_entry_spawn(swamp, 13);
    check(swamp_entry_13.found, "Swamp has the entry 13 SpawnPoint (Darkwood return)");

    std::vector<ActorDefinition> darkwood;
    {
        AssetCatalog catalogue(darkwood_root);
        check(load_actor_definitions(catalogue, "data/scene/003_darkwood.mlx", darkwood, error), "Darkwood declarations: " + error);
    }
    const auto darkwood_entry_0 = resolve_entry_spawn(darkwood, 0);
    check(darkwood_entry_0.found && darkwood_entry_0.candidates >= 1, "Darkwood has an entry 0 SpawnPoint");
    ExitZoneSet darkwood_zones;
    check(darkwood_zones.build(darkwood, levels, registry, error), "Darkwood zones: " + error);
    bool back_to_swamp = false;
    for (const auto& zone : darkwood_zones.zones())
        if (zone.built && zone.kind == ZoneKind::level_exit && zone.target_name == "SWAMP" && zone.entry_id == 13) back_to_swamp = true;
    check(back_to_swamp, "Darkwood has the authored return exit to SWAMP entry 13");
    std::cout << "level_transitions real data: swamp exits=" << swamp_zones.built_count(ZoneKind::level_exit)
              << " checkpoints=" << swamp_zones.built_count(ZoneKind::checkpoint)
              << " darkwood exits=" << darkwood_zones.built_count(ZoneKind::level_exit)
              << " swamp entry13=" << swamp_entry_13.candidates << " darkwood entry0=" << darkwood_entry_0.candidates << '\n';
}

} // namespace

int main(int argc, char** argv) {
    try {
        test_builders_and_registry();
        test_edges();
        test_spawn_resolution();
        test_condition_stub();
        if (argc >= 3) test_real_levels(fs::path(argv[1]), fs::path(argv[2]));
        std::cout << "level_transitions_v1 tests passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "level_transitions_v1 test failed: " << e.what() << '\n';
        return 1;
    }
}
