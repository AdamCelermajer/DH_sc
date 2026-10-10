#pragma once
#include "../game-data/properties.hpp"
#include <optional>

namespace dh::foundation {
class AssetCatalog;
struct OriginalPropertyDatabase {
    dh2::data::CharacterTable characters;
    dh2::data::ClassTables classes;
};
bool load_original_property_tables(const AssetCatalog&, const std::string& table_root,
                                  OriginalPropertyDatabase&, std::string& error);
struct OriginalActorPropertyOptions {
    // Caller supplies the original campaign/spawn level, already encoded x256.
    // Absence preserves the exact CharacterTable base value.
    std::optional<std::int32_t> level_raw;
    // Explicit new-spawn policy, not a save restore policy.
    bool refill_vitals = false;
};
struct OriginalActorProperties {
    dh2::data::PropertyState sheets;
    std::int32_t faction_id = -1, class_id = -1, level_raw = 0;
    float health = 0, max_health = 0, resource = 0, max_resource = 0;
    float walk_multiplier = 0, rotation_multiplier = 0;
    float turn_radians_per_second = 0;
    // Collision_Scale is a plain percent; it is NOT signed256.
    float collision_scale = 0;
};
// Resolves the original class formula graph and default/type rules, retaining
// all 224 raw words. No equipment, buff, difficulty or co-op modifiers inferred.
bool resolve_original_actor_properties(const dh2::data::CharacterTable&,
    const dh2::data::ClassTables&, const std::string& character_row,
    const OriginalActorPropertyOptions&, OriginalActorProperties&, std::string& error);
// Only use for properties whose original accessor proves this representation.
float original_signed256(std::int32_t raw) noexcept;
float original_speed_modifier(std::int32_t raw) noexcept;
// Fresh profile source PLVL=1; only the three authored playable bases accepted.
bool resolve_original_fresh_player(const OriginalPropertyDatabase&,
                                  const std::string& character_row,
                                  OriginalActorProperties&, std::string& error);
} // namespace dh::foundation
