#pragma once

#include "../../../game-data/faery_tables.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace dh::foundation::faery_menu {

// v21.8 faery ability registry. Keyed by the authored Faery table SpellScript
// (faeries_pyarray.bin row word SpellScript, e.g. "faerie_wetty"), never by
// slot number. Values are transcribed from the original Lua bytecode strings in
// ANDROID scripts/skills/faerie_<name>.luac (constants and PlayFX/PlaySound
// calls); see coordination/v21.8/FAERY-SKILL-RESEARCH.md section 1.

enum class FaeryAbilityKindV1 : std::uint8_t {
    celest_lightning, // faerie_celest: Celest-specific cast arm (CelestPreparedCast)
    strike            // Hotty-shaped skeleton: fire/water/earth/wind strike arm
};

struct FaeryAbilityV1 {
    std::string_view script;        // SpellScript, e.g. "faerie_wetty"
    std::string_view faery_name;    // Faery table name, e.g. "Wetty"
    std::int32_t element;           // FaeryTypes: Lightning=0 Earth=1 Water=2 Air=3 Fire=4
    FaeryAbilityKindV1 kind;
    std::string_view model_file;    // character_models name, e.g. "faeries_01_wetty"
    std::string_view spell_class;   // ClassTable base row (+ skill_level), e.g. "Spell_Water_1"
    std::string_view player_pre_fx; // AnimatedEffectTable set name for PlayFX on the player
    std::string_view target_fx;     // AnimatedEffectTable set name for PlayFX on each hit target
    // Pre-skill 3D sound with targets: tiered (tier_prefix + small/medium/big)
    // when tier_prefix is non-empty, otherwise the fixed label.
    std::string_view tier_prefix;
    std::string_view fixed_target_label;
    // Pre-skill 3D sound when the target list is empty (always one label).
    std::string_view no_target_label;
    // Post-hit 2D PlaySound (OnSkill_, after the target loop, unconditional).
    // Empty when the Lua has no such call. Not resolved through the catalog:
    // see faery_hit_sound_label_resolution_v1.
    std::string_view hit_label;
    bool has_passive{};             // true only for faerie_celest (CheckFaeries)
};

// Registry entry for the Faery at FaeryList[list_id][slot] (the actual Character
// list, not a hard-coded slot). nullptr when the row is missing, its name does
// not match the registry, or its script is not registered.
const FaeryAbilityV1* faery_ability_for_list_slot_v1(const dh2::data::FaeryTables::Borrow& tables,
                                                     std::int32_t list_id, std::int32_t slot);

// The five authored faery scripts. Order is the table list order (slot 0..4).
const std::vector<FaeryAbilityV1>& faery_ability_registry_v1();

// Lookup by exact SpellScript string. nullptr when the script is not registered.
const FaeryAbilityV1* find_faery_ability_by_script_v1(std::string_view script);

// --test-faery argument: a Faery name (case-insensitive: celest, rocky, wetty,
// windy, hotty) or a list slot 0..4. Returns false for anything else.
bool faery_slot_from_test_argument_v1(std::string_view argument, std::int32_t& slot);

// Pre-skill 3D labels in play order, matching the Lua OnPreSkill_ branches for
// strike-kind faeries. For a non-empty target list the tier or fixed label is
// used; for an empty list only the no_target_label plays.
std::vector<std::string> faery_strike_pre_sound_labels_v1(const FaeryAbilityV1&,
                                                          std::size_t target_count);

// Post-hit labels: empty vector when the Lua has no PlaySound after the loop.
std::vector<std::string> faery_strike_hit_sound_labels_v1(const FaeryAbilityV1&);

// Passive resistance buff ClassTable row for the equipped element, as set by
// the Celest script's CheckFaeries(). Level 1 uses the AW_ variant. Returns
// nullptr for an unknown element.
const char* faery_passive_buff_class_v1(std::int32_t element, std::int32_t faery_level);

} // namespace dh::foundation::faery_menu
