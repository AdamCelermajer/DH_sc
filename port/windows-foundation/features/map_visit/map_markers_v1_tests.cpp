// Map page marker rules, registry and providers (P16 MAPMARKERS). Pure data, no assets.
#include "map_markers_v1.hpp"

#include <iostream>

using namespace dh::foundation::map_visit;

namespace {
int failures = 0;
#define CHECK(cond) do { if (!(cond)) { std::cerr << __FILE__ << ':' << __LINE__ << " CHECK failed: " #cond "\n"; ++failures; } } while (0)

// Visited rooms: x < 0 is visited (a stand-in for map_point_visited_v1 in the host).
bool visited_west(const std::array<float, 3>& p) { return p[0] < 0.0f; }

MapMarkerInputsV1 base_inputs() {
    MapMarkerInputsV1 in;
    in.visited = visited_west;
    return in;
}

void kind_table_tests() {
    CHECK(map_object_class_rule_v1("CheckpointZone") && map_object_class_rule_v1("CheckpointZone")->kind == MapMarkerKindV1::checkpoint);
    CHECK(map_object_class_rule_v1("SpawnPoint") && map_object_class_rule_v1("SpawnPoint")->kind == MapMarkerKindV1::entrance);
    CHECK(map_object_class_rule_v1("TriggerZoneExitLevel") && map_object_class_rule_v1("TriggerZoneExitLevel")->kind == MapMarkerKindV1::exit);
    CHECK(!map_object_class_rule_v1("TriggerZone"));   // not a map marker class (IDA type list has no entry)
    CHECK(!map_object_class_rule_v1("Dummy"));
    CHECK(map_object_class_rule_v1("CheckpointZone")->needs_active == false);
    CHECK(map_object_class_rule_v1("SpawnPoint")->needs_active == true);
}

void gate_tests() {
    std::set<std::string> active{"IsAfter_Swamp_Escape"};
    CHECK(map_activation_gate_v1({}, active));                                            // nothing authored: active
    CHECK(map_activation_gate_v1({{"activate_cond", "IsAfter_Swamp_Escape"}}, active));  // authored and active
    CHECK(!map_activation_gate_v1({{"activate_cond", "IsBefore_Swamp_KillWitch"}}, active));
    CHECK(!map_activation_gate_v1({{"deactivate_cond", "IsAfter_Swamp_Escape"}}, active));
    CHECK(map_activation_gate_v1({{"deactivate_cond", "IsBefore_Swamp_KillWitch"}}, active));
    CHECK(map_activation_gate_v1({{"activate_cond", ""}}, active));                       // empty string: not authored
}

void character_rule_tests() {
    MapCharacterV1 npc;
    npc.character_row = 7;
    npc.merchant = false;
    std::set<std::int32_t> talk{7};
    CHECK(map_character_marker_kind_v1(npc, talk) == MapMarkerKindV1::quest_giver);
    // IDA order: a talk target wins over the merchant test.
    npc.merchant = true;
    CHECK(map_character_marker_kind_v1(npc, talk) == MapMarkerKindV1::quest_giver);
    // A merchant without an open talk objective.
    CHECK(map_character_marker_kind_v1(npc, {}) == MapMarkerKindV1::merchant);
    // Neither: no marker.
    npc.merchant = false;
    CHECK(!map_character_marker_kind_v1(npc, {}));
    CHECK(!map_character_marker_kind_v1(npc, {99}));
    // Unknown row (-1) never matches a talk objective.
    MapCharacterV1 unknown;
    CHECK(!map_character_marker_kind_v1(unknown, {-1}));
}

void provider_tests() {
    auto in = base_inputs();
    // Character: visited and enabled gives a marker; hidden, disabled or unvisited do not.
    MapCharacterV1 merchant;
    merchant.position = {-10, 0, 0};
    merchant.merchant = true;
    MapCharacterV1 disabled = merchant;
    disabled.enabled = false;
    MapCharacterV1 far = merchant;
    far.position = {10, 0, 0};
    in.characters = {merchant, disabled, far};
    MapMarkersV1 out;
    map_provider_characters_v1(in, out);
    CHECK(out.size() == 1);
    CHECK(out.size() == 1 && out[0].kind == MapMarkerKindV1::merchant);

    // Level objects: checkpoint needs only a visited room; exit and entrance also need the active gate.
    in = base_inputs();
    in.level_objects = {
        {"CheckpointZone", {-5, 0, 0}, false},
        {"CheckpointZone", {10, 0, 0}, true},             // unvisited
        {"TriggerZoneExitLevel", {-5, 1, 0}, true},
        {"TriggerZoneExitLevel", {-5, 2, 0}, false},      // inactive: no exit icon
        {"SpawnPoint", {-6, 0, 0}, true},
        {"SpawnPoint", {-6, 1, 0}, false},                // inactive: no entrance icon
        {"Dummy", {-5, 0, 0}, true},                      // not a map class
    };
    out.clear();
    map_provider_level_objects_v1(in, out);
    CHECK(out.size() == 3);   // checkpoint (no gate), exit and entrance (both active); inactive, unvisited, Dummy excluded
    int checkpoints = 0, exits = 0, entrances = 0;
    for (const auto& m : out) {
        if (m.kind == MapMarkerKindV1::checkpoint) ++checkpoints;
        if (m.kind == MapMarkerKindV1::exit) ++exits;
        if (m.kind == MapMarkerKindV1::entrance) ++entrances;
    }
    CHECK(checkpoints == 1);
    CHECK(exits == 1);
    CHECK(entrances == 1);

    // Objectives: drawn without a visited check (IDA ShowObjectivesIcons).
    in = base_inputs();
    in.objective_zones = {{"_prim_WitchQuestStart", {10, 0, 0}}};
    out.clear();
    map_provider_objectives_v1(in, out);
    CHECK(out.size() == 1 && out[0].kind == MapMarkerKindV1::objective);

    // Enemies: the visited check still applies.
    in = base_inputs();
    in.enemies = {{MapMarkerKindV1::enemy, {-1, 0, 0}}, {MapMarkerKindV1::enemy, {1, 0, 0}}};
    out.clear();
    map_provider_enemies_v1(in, out);
    CHECK(out.size() == 1 && out[0].kind == MapMarkerKindV1::enemy);
}

void registry_tests() {
    auto registry = standard_map_marker_registry_v1();
    const auto names = registry.names();
    CHECK(names.size() == 4);
    CHECK(names.size() == 4 && names[0] == "characters");
    CHECK(!registry.add("characters", map_provider_characters_v1));   // one producer per family
    CHECK(registry.add("probe", map_provider_enemies_v1));

    auto in = base_inputs();
    in.characters = {{{-1, 0, 0}, true, 3, false}};
    in.quest_talk_rows = {3};
    in.level_objects = {{"CheckpointZone", {-2, 0, 0}, true}};
    in.objective_zones = {{"zone", {5, 0, 0}}};
    in.enemies = {{MapMarkerKindV1::enemy, {-3, 0, 0}}};
    const auto markers = standard_map_marker_registry_v1().collect(in);
    CHECK(markers.size() == 4);
    // Order follows the original Show: characters, level objects, objectives, enemies.
    CHECK(markers.size() == 4 && markers[0].kind == MapMarkerKindV1::quest_giver);
    CHECK(markers.size() == 4 && markers[1].kind == MapMarkerKindV1::checkpoint);
    CHECK(markers.size() == 4 && markers[2].kind == MapMarkerKindV1::objective);
    CHECK(markers.size() == 4 && markers[3].kind == MapMarkerKindV1::enemy);
}

} // namespace

int main() {
    kind_table_tests();
    gate_tests();
    character_rule_tests();
    provider_tests();
    registry_tests();
    if (failures) {
        std::cerr << failures << " map marker test(s) failed\n";
        return 1;
    }
    std::cout << "map_markers tests passed\n";
    return 0;
}
