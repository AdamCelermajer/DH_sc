#include "map_markers_v1.hpp"

namespace dh::foundation::map_visit {

bool MapMarkerRegistryV1::add(std::string name, MapMarkerProviderV1 provider) {
    for (const auto& entry : providers_)
        if (entry.first == name) return false;
    providers_.emplace_back(std::move(name), std::move(provider));
    return true;
}

MapMarkersV1 MapMarkerRegistryV1::collect(const MapMarkerInputsV1& inputs) const {
    MapMarkersV1 out;
    for (const auto& entry : providers_) entry.second(inputs, out);
    return out;
}

std::vector<std::string> MapMarkerRegistryV1::names() const {
    std::vector<std::string> out;
    out.reserve(providers_.size());
    for (const auto& entry : providers_) out.push_back(entry.first);
    return out;
}

MapMarkerRegistryV1 standard_map_marker_registry_v1() {
    // Original order of MenuCharMenu_Map::Show. Room exit arrows (ShowRoomExitsIcons) are not registered: their
    // producer (writer of the global exit list) is not decoded, so there is no honest source to provide.
    MapMarkerRegistryV1 registry;
    registry.add("characters", map_provider_characters_v1);
    registry.add("level_objects", map_provider_level_objects_v1);
    registry.add("objectives", map_provider_objectives_v1);
    registry.add("enemies", map_provider_enemies_v1);
    return registry;
}

const std::vector<MapObjectClassRuleV1>& map_object_class_rules_v1() {
    static const std::vector<MapObjectClassRuleV1> rules{
        // IDA ShowMapExitsIcons: type 12 = Checkpoint (no active byte read), 13 = Entrance, 14 = Exit.
        {"CheckpointZone", MapMarkerKindV1::checkpoint, false},
        {"SpawnPoint", MapMarkerKindV1::entrance, true},
        {"TriggerZoneExitLevel", MapMarkerKindV1::exit, true},
    };
    return rules;
}

std::optional<MapObjectClassRuleV1> map_object_class_rule_v1(const std::string& gametype) {
    for (const auto& rule : map_object_class_rules_v1())
        if (gametype == rule.gametype) return rule;
    return std::nullopt;
}

bool map_activation_gate_v1(const std::map<std::string, std::string>& properties, const std::set<std::string>& activeConditions) {
    const auto activate = properties.find("activate_cond");
    if (activate != properties.end() && !activate->second.empty() && !activeConditions.count(activate->second)) return false;
    const auto deactivate = properties.find("deactivate_cond");
    if (deactivate != properties.end() && !deactivate->second.empty() && activeConditions.count(deactivate->second)) return false;
    return true;
}

std::optional<MapMarkerKindV1> map_character_marker_kind_v1(const MapCharacterV1& character,
                                                            const std::set<std::int32_t>& questTalkRows) {
    // IDA ShowNpcIcons: +762 && !+763 -> QuestGiver (10); else IsMerchant -> Merchant (11).
    if (character.character_row >= 0 && questTalkRows.count(character.character_row)) return MapMarkerKindV1::quest_giver;
    if (character.merchant) return MapMarkerKindV1::merchant;
    return std::nullopt;
}

namespace {
bool visible_on_map(const MapMarkerInputsV1& inputs, const std::array<float, 3>& position) {
    return inputs.visited && inputs.visited(position);
}
}

void map_provider_characters_v1(const MapMarkerInputsV1& inputs, MapMarkersV1& out) {
    for (const auto& character : inputs.characters) {
        if (!character.enabled || !visible_on_map(inputs, character.position)) continue;
        const auto kind = map_character_marker_kind_v1(character, inputs.quest_talk_rows);
        if (kind) out.push_back({*kind, character.position});
    }
}

void map_provider_level_objects_v1(const MapMarkerInputsV1& inputs, MapMarkersV1& out) {
    for (const auto& object : inputs.level_objects) {
        const auto rule = map_object_class_rule_v1(object.gametype);
        if (!rule) continue;
        if (rule->needs_active && !object.active) continue;
        if (!visible_on_map(inputs, object.position)) continue;
        out.push_back({rule->kind, object.position});
    }
}

void map_provider_objectives_v1(const MapMarkerInputsV1& inputs, MapMarkersV1& out) {
    // IDA ShowObjectivesIcons: no visited check (the objective position is drawn whether or not its room is visited).
    for (const auto& zone : inputs.objective_zones) out.push_back({MapMarkerKindV1::objective, zone.position});
}

void map_provider_enemies_v1(const MapMarkerInputsV1& inputs, MapMarkersV1& out) {
    for (const auto& enemy : inputs.enemies)
        if (visible_on_map(inputs, enemy.position)) out.push_back({MapMarkerKindV1::enemy, enemy.position});
}

} // namespace dh::foundation::map_visit
