#pragma once
// Character-menu Map page marker producers (Preview 16, stream MAPMARKERS). Pure rules and a provider registry;
// the host gathers the live inputs each frame (main.cpp) and projects the returned markers with the Map camera.
//
// Original (IDA, MenuCharMenu_Map::Show 0x455138 calls the producers in this order):
//   ShowPlayersIcons, ShowNpcIcons 0x454fb8, ShowMapExitsIcons 0x454d5c, ShowRoomExitsIcons 0x454b8c,
//   ShowObjectivesIcons 0x4548ac. Every marker is a DuplicateIcon frame of MapIconsDynamic: frame = type.
// Marker kinds (sprite 614 labels): 0 Objective, 1 Entrance, 2 Exit, 3 Character, 4 Enemies, 5 Champion, 6 Boss,
// 7..9 Player2..4, 10 QuestGiver, 11 Merchant, 12 Checkpoint, 13 Arrow (room exits 14..17 use the Arrow frame).
//
// Rules (see coordination/claude-preview16/MAPMARKERS-report.md for the evidence):
//   - Character (NPC) markers: visible, alive, inside a visited room. QuestGiver when the NPC's CharacterTable row is
//     the oid of an installed TalkToNPC objective (+762 && !+763); otherwise Merchant when the AI row type is 7.
//   - Level objects: CheckpointZone -> Checkpoint (12), SpawnPoint -> Entrance (1), TriggerZoneExitLevel -> Exit (2),
//     all inside a visited room; Exit and Entrance also need the object's active byte (activation gate).
//   - Objectives: only the current quest's MoveInZone objectives, at the zone position. No visited check.
//   - Enemies: live hostile actors inside a visited room (combat session owner, host-gathered).
//   - Room exit arrows: registered by name only when a producer exists; none is decoded (report gap).
#include <array>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace dh::foundation::map_visit {

// MapIconsDynamic frame labels (IDA DuplicateIcon frame = icon type).
enum class MapMarkerKindV1 : std::uint8_t {
    objective = 0,
    entrance = 1,
    exit = 2,
    character = 3,
    enemy = 4,
    champion = 5,
    boss = 6,
    player2 = 7,
    player3 = 8,
    player4 = 9,
    quest_giver = 10,
    merchant = 11,
    checkpoint = 12,
    arrow = 13,
};

struct MapMarkerV1 {
    MapMarkerKindV1 kind{};
    std::array<float, 3> position{};
};
using MapMarkersV1 = std::vector<MapMarkerV1>;

// One authored level object reduced to the fields the marker rules read. The host fills it from the level's
// ActorDefinition list and evaluates activate_cond / deactivate_cond with the active condition names.
struct MapLevelObjectV1 {
    std::string gametype;              // authored class (CheckpointZone, SpawnPoint, TriggerZoneExitLevel, ...)
    std::array<float, 3> position{};   // world translation of the placement
    bool active = true;                // activation gate (map_activation_gate_v1)
};

// One placed NPC/character with the facts the NPC rules read (host-gathered; facts cached per actor).
struct MapCharacterV1 {
    std::array<float, 3> position{};
    bool enabled = true;               // population activation (not dead, not deactivated)
    std::int32_t character_row = -1;   // CharacterTable row (TalkToNPC oid1 match)
    bool merchant = false;             // Character::IsMerchant: AI row type == 7
};

// A MoveInZone objective target of the current quest: the authored zone and its world position.
struct MapObjectiveZoneV1 {
    std::string name;
    std::array<float, 3> position{};
};

struct MapMarkerInputsV1 {
    // IDA IsInsideRooms(point, visited): true when the point is inside a visited room (host supplies map_point_visited_v1).
    std::function<bool(const std::array<float, 3>&)> visited;
    std::vector<MapLevelObjectV1> level_objects;
    std::vector<MapCharacterV1> characters;
    // NPC rows whose installed TalkToNPC objective is still open (IDA +762 && !+763).
    std::set<std::int32_t> quest_talk_rows;
    std::vector<MapObjectiveZoneV1> objective_zones;
    // Live hostile enemies inside the visited rooms are gathered by the combat owner: already filtered.
    MapMarkersV1 enemies;
};

// Provider registry. A provider appends markers for the inputs it owns; the order of registration is the order of
// the original Show (npc, level objects, objectives, enemies). Names are for reports and duplicate checks.
using MapMarkerProviderV1 = std::function<void(const MapMarkerInputsV1&, MapMarkersV1&)>;

class MapMarkerRegistryV1 {
public:
    // Returns false when the name is already registered (one producer per family).
    bool add(std::string name, MapMarkerProviderV1 provider);
    MapMarkersV1 collect(const MapMarkerInputsV1& inputs) const;
    std::vector<std::string> names() const;
private:
    std::vector<std::pair<std::string, MapMarkerProviderV1>> providers_;
};

// The providers that exist for this port, registered in original Show order.
MapMarkerRegistryV1 standard_map_marker_registry_v1();

// Class -> marker kind (IDA GO_IDS: CheckpointZone 12, SpawnPoint 13, TriggerZoneExitLevel 14). Unknown classes: none.
struct MapObjectClassRuleV1 {
    const char* gametype;
    MapMarkerKindV1 kind;
    bool needs_active;   // ShowMapExitsIcons reads the active byte (obj+138) for entrance and exit only
};
const std::vector<MapObjectClassRuleV1>& map_object_class_rules_v1();
std::optional<MapObjectClassRuleV1> map_object_class_rule_v1(const std::string& gametype);

// Activation gate of an authored object (the rule the population already uses): activate_cond must be in the active
// set when authored, and deactivate_cond must not be. Empty condition strings are not authored.
bool map_activation_gate_v1(const std::map<std::string, std::string>& properties, const std::set<std::string>& activeConditions);

// Quest-giver / merchant decision for one placed character (IDA ShowNpcIcons order: quest giver first).
std::optional<MapMarkerKindV1> map_character_marker_kind_v1(const MapCharacterV1& character,
                                                            const std::set<std::int32_t>& questTalkRows);

// Providers (also exposed for tests).
void map_provider_characters_v1(const MapMarkerInputsV1& inputs, MapMarkersV1& out);
void map_provider_level_objects_v1(const MapMarkerInputsV1& inputs, MapMarkersV1& out);
void map_provider_objectives_v1(const MapMarkerInputsV1& inputs, MapMarkersV1& out);
void map_provider_enemies_v1(const MapMarkerInputsV1& inputs, MapMarkersV1& out);

} // namespace dh::foundation::map_visit
