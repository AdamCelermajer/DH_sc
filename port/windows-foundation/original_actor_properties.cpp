#include "original_actor_properties.hpp"
#include "asset_catalog.hpp"
#include <algorithm>
#include <cstring>

namespace dh::foundation {
bool load_original_property_tables(const AssetCatalog& catalog, const std::string& root,
                                  OriginalPropertyDatabase& output, std::string& error) {
    try {
        const auto read = [&](const char* name) { return catalog.read(std::filesystem::path(root) / name); };
        const auto c = read("character_properties_pyarray.bin"),
                   cn = read("character_properties_pyarraynames.bin"),
                   cf = read("character_properties_pystructnames.bin"),
                   k = read("character_classes_pyarray.bin"),
                   kn = read("character_classes_pyarraynames.bin"),
                   kf = read("character_classes_pystructnames.bin");
        const auto bytes = [](const auto& data) { return dh2::data::Bytes{data.data(), data.size()}; };
        OriginalPropertyDatabase next;
        if (!dh2::data::load_characters(bytes(c),bytes(cn),bytes(cf),next.characters,error) ||
            !dh2::data::load_classes(bytes(k),bytes(kn),bytes(kf),next.classes,error)) return false;
        output = std::move(next); error.clear(); return true;
    } catch (const std::exception& ex) { error = ex.what(); return false; }
}
bool resolve_original_fresh_player(const OriginalPropertyDatabase& database,
    const std::string& row, OriginalActorProperties& output, std::string& error) {
    if (row != "KnightPlayerBase" && row != "RoguePlayerBase" && row != "MagePlayerBase") {
        error = "Not an original fresh playable base"; return false;
    }
    return resolve_original_actor_properties(database.characters, database.classes,
        row, {256, true}, output, error);
}
float original_signed256(std::int32_t raw) noexcept { return float(raw) * (1.0f / 256.0f); }
float original_speed_modifier(std::int32_t raw) noexcept {
    // PROPS_GetWalkSpeed 0x3de6c4; PROPS_GetRotationSpeed 0x3de708.
    const float value = original_signed256(raw) * 0.01f + 1.0f;
    return value > 0.0f ? value : 0.0f;
}
bool resolve_original_actor_properties(const dh2::data::CharacterTable& characters,
    const dh2::data::ClassTables& classes, const std::string& row_name,
    const OriginalActorPropertyOptions& options, OriginalActorProperties& output,
    std::string& error) {
    dh2::data::PropertyRules rules;
    if (!dh2::data::load_property_rules(characters, rules, error)) return false;
    for (const auto& field : {std::pair<unsigned, const char*>{0, "AIFaction"},
         {16, "Collision_Scale"}, {19, "Level"}, {26, "ClassID"}, {36, "HP"},
         {38, "Max_HP"}, {41, "MP"}, {43, "Max_MP"},
         {46, "Speed_Modifier_Walk"}, {47, "Speed_Modifier_Rotation"}}) {
        if (characters.fields[field.first] != field.second) {
            error = "Original actor property layout differs"; return false;
        }
    }
    const auto row = std::find(characters.names.begin(), characters.names.end(), row_name);
    if (row == characters.names.end()) { error = "Original character row absent: " + row_name; return false; }
    OriginalActorProperties next;
    const auto index = static_cast<std::size_t>(row - characters.names.begin());
    dh2::data::reset_properties(rules, next.sheets, &characters.rows[index]);
    if (options.level_raw) {
        if (*options.level_raw < 0) { error = "Original actor level raw must be nonnegative"; return false; }
        next.sheets.base[19] = *options.level_raw;
    }
    if (!dh2::data::recalc_properties_with_class(classes, rules, next.sheets, error)) return false;
    if (options.refill_vitals) {
        auto view = dh2::data::property_view(rules, next.sheets);
        // Original SetLevel regen fills current36/41 up to maximum38/43.
        // Add the missing amount: saved sheets hold an additive contribution,
        // rather than the final current value when base contributes too.
        for (const unsigned current : {36u, 41u}) {
            const unsigned maximum = current == 36 ? 38 : 43;
            const auto delta_bits = std::uint32_t(next.sheets.resolved[maximum]) -
                                    std::uint32_t(next.sheets.resolved[current]);
            std::int32_t delta; std::memcpy(&delta, &delta_bits, sizeof(delta));
            if (delta > 0 && dh2_property_add(&view, current, delta)) {
                error = "Original vital refill failed"; return false;
            }
        }
    }
    const auto& values = next.sheets.resolved;
    next.faction_id = values[0]; next.class_id = values[26]; next.level_raw = values[19];
    next.health = original_signed256(values[36]); next.max_health = original_signed256(values[38]);
    next.resource = original_signed256(values[41]); next.max_resource = original_signed256(values[43]);
    next.walk_multiplier = original_speed_modifier(values[46]);
    next.rotation_multiplier = original_speed_modifier(values[47]);
    const std::uint32_t turn_bits = 0x41490fdb; float turn_base;
    std::memcpy(&turn_base, &turn_bits, sizeof(turn_base));
    next.turn_radians_per_second = turn_base * next.rotation_multiplier;
    next.collision_scale = float(values[16]) * 0.01f;
    output = std::move(next); error.clear(); return true;
}
} // namespace dh::foundation
