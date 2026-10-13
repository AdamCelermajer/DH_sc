#include "faery_ability_registry_v1.hpp"

namespace dh::foundation::faery_menu {

const std::vector<FaeryAbilityV1>& faery_ability_registry_v1() {
    // Lua constants verified from the bytecode strings: MANA_BASE {10,20},
    // MANA_PLVL 0.2, RANGE {600,800}, COOLDOWN_BASE 5000 (overridden by
    // GetInt("Faery_Cooldown")). Those are shared and handled by the cast arms.
    static const std::vector<FaeryAbilityV1> registry{
        {"faerie_celest", "Celest", 0, FaeryAbilityKindV1::celest_lightning,
         "faeries_02_celeste", "Spell_Lightning_1",
         "Celest_Level_1_Player", "Celest_Level_1_Main",
         "sfx_spell_lightning_", "", "StaticBallKilled", "", true},
        {"faerie_rocky", "Rocky", 1, FaeryAbilityKindV1::strike,
         "faeries_03_rocky", "Spell_Earth_1",
         "Rocky_Level_1_Player_Pre", "Rocky_Level_1_Target",
         "", "RockyEarthSpell", "sfx_mage_staff_elemental_earth", "sfx_faerie_earth", false},
        {"faerie_wetty", "Wetty", 2, FaeryAbilityKindV1::strike,
         "faeries_01_wetty", "Spell_Water_1",
         "Wetty_Level_1_Player_Pre", "Wetty_Level_1_Main",
         "sfx_spell_water_", "", "sfx_mage_staff_elemental_ice", "sfx_faerie_ice", false},
        {"faerie_windy", "Windy", 3, FaeryAbilityKindV1::strike,
         "faeries_04_windy", "Spell_Air_1",
         "Windy_Level_1_Player_Pre", "Windy_Level_1_Main",
         "", "WindyWhirlwindSpell", "sfx_mage_staff_elemental_wind", "sfx_faerie_wind", false},
        {"faerie_hotty", "Hotty", 4, FaeryAbilityKindV1::strike,
         "faeries_00_hotty", "Spell_Fire_1",
         "Hotty_Level_1_Player_Pre", "Hotty_Level_1_Target",
         "sfx_spell_fire_", "", "sfx_mage_staff_elemental_fire", "", false},
    };
    return registry;
}

const FaeryAbilityV1* find_faery_ability_by_script_v1(std::string_view script) {
    for (const auto& entry : faery_ability_registry_v1())
        if (entry.script == script) return &entry;
    return nullptr;
}

const FaeryAbilityV1* faery_ability_for_list_slot_v1(const dh2::data::FaeryTables::Borrow& tables,
                                                     std::int32_t list_id, std::int32_t slot) {
    if (!tables || list_id < 0 || static_cast<std::size_t>(list_id) >= tables.lists().size()) return nullptr;
    const auto& list = tables.lists()[static_cast<std::size_t>(list_id)];
    if (list.size() != 5 || slot < 0 || slot >= 5) return nullptr;
    const auto row_id = list[static_cast<std::size_t>(slot)];
    if (row_id < 0 || static_cast<std::size_t>(row_id) >= tables.faeries().size() ||
        static_cast<std::size_t>(row_id) >= tables.faery_names().size()) return nullptr;
    const auto& row = tables.faeries()[static_cast<std::size_t>(row_id)];
    const auto* entry = find_faery_ability_by_script_v1(row.script);
    if (!entry || tables.faery_names()[static_cast<std::size_t>(row_id)] != entry->faery_name) return nullptr;
    return entry;
}

namespace {
std::string tier_of(std::size_t target_count) {
    if (target_count > 0 && target_count < 3) return "small";
    if (target_count < 6) return "medium";
    return "big";
}
} // namespace

bool faery_slot_from_test_argument_v1(std::string_view argument, std::int32_t& slot) {
    // Registry order is the FaeryList slot order (Celest 0, Rocky 1, Wetty 2, Windy 3, Hotty 4).
    if (argument.size() == 1 && argument[0] >= '0' && argument[0] <= '4') {
        slot = argument[0] - '0';
        return true;
    }
    for (std::size_t i = 0; i < faery_ability_registry_v1().size(); ++i) {
        const auto& name = faery_ability_registry_v1()[i].faery_name;
        if (name.size() != argument.size()) continue;
        bool same = true;
        for (std::size_t k = 0; k < name.size() && same; ++k) {
            const char a = name[k] >= 'A' && name[k] <= 'Z' ? char(name[k] - 'A' + 'a') : name[k];
            const char b = argument[k] >= 'A' && argument[k] <= 'Z' ? char(argument[k] - 'A' + 'a') : argument[k];
            same = a == b;
        }
        if (same) {
            slot = static_cast<std::int32_t>(i);
            return true;
        }
    }
    return false;
}

std::vector<std::string> faery_strike_pre_sound_labels_v1(const FaeryAbilityV1& ability,
                                                          std::size_t target_count) {
    if (target_count == 0) return {std::string(ability.no_target_label)};
    if (!ability.tier_prefix.empty())
        return {std::string(ability.tier_prefix) + tier_of(target_count)};
    return {std::string(ability.fixed_target_label)};
}

std::vector<std::string> faery_strike_hit_sound_labels_v1(const FaeryAbilityV1& ability) {
    if (ability.hit_label.empty()) return {};
    return {std::string(ability.hit_label)};
}

const char* faery_passive_buff_class_v1(std::int32_t element, std::int32_t faery_level) {
    const bool aw = faery_level == 1;
    switch (element) {
    case 0: return aw ? "AW_Celeste_LightningResistance" : "Celeste_LightningResistance";
    case 1: return aw ? "AW_Primula_EarthResistance" : "Primula_EarthResistance";
    case 2: return aw ? "AW_Maria_WaterResistance" : "Maria_WaterResistance";
    case 3: return aw ? "AW_Sylph_WindResistance" : "Sylph_WindResistance";
    case 4: return aw ? "AW_Avalon_FireResistance" : "Avalon_FireResistance";
    default: return nullptr;
    }
}

} // namespace dh::foundation::faery_menu
