// v21.8 item 8: faery ability registry keyed by authored SpellScript.
// Expected values come from the original Lua bytecode strings of
// scripts/skills/faerie_<name>.luac (see coordination/v21.8/FAERY-SKILL-RESEARCH.md).
#include "../faery_ability_registry_v1.hpp"

#include <cstdio>
#include <string>
#include <vector>

using namespace dh::foundation::faery_menu;

namespace {
int failures = 0;

void check(bool ok, const char* what) {
    if (!ok) {
        ++failures;
        std::printf("FAIL %s\n", what);
    }
}

bool same(const std::vector<std::string>& got, const std::vector<std::string>& want) {
    return got == want;
}
} // namespace

int main() {
    const auto& registry = faery_ability_registry_v1();
    check(registry.size() == 5, "registry lists exactly the five authored faery scripts");

    // Every table script resolves to exactly one entry, and unknown scripts do not.
    for (const char* script : {"faerie_celest", "faerie_rocky", "faerie_wetty",
                               "faerie_windy", "faerie_hotty"})
        check(find_faery_ability_by_script_v1(script) != nullptr, script);
    check(find_faery_ability_by_script_v1("faerie_celest_mage") == nullptr, "mage variants are not registered");
    check(find_faery_ability_by_script_v1("") == nullptr, "empty script is not registered");
    check(find_faery_ability_by_script_v1("Faerie_Hotty") == nullptr, "script lookup is exact");

    // Element and model wiring (FaeryTypes: Lightning 0, Earth 1, Water 2, Air 3, Fire 4).
    const auto* celest = find_faery_ability_by_script_v1("faerie_celest");
    const auto* rocky = find_faery_ability_by_script_v1("faerie_rocky");
    const auto* wetty = find_faery_ability_by_script_v1("faerie_wetty");
    const auto* windy = find_faery_ability_by_script_v1("faerie_windy");
    const auto* hotty = find_faery_ability_by_script_v1("faerie_hotty");
    check(celest && celest->element == 0 && celest->model_file == "faeries_02_celeste", "celest element/model");
    check(rocky && rocky->element == 1 && rocky->model_file == "faeries_03_rocky", "rocky element/model");
    check(wetty && wetty->element == 2 && wetty->model_file == "faeries_01_wetty", "wetty element/model");
    check(windy && windy->element == 3 && windy->model_file == "faeries_04_windy", "windy element/model");
    check(hotty && hotty->element == 4 && hotty->model_file == "faeries_00_hotty", "hotty element/model");

    // Spell class rows (Lua CLASS_ID) and the cast kind.
    check(celest && celest->spell_class == "Spell_Lightning_1" && celest->kind == FaeryAbilityKindV1::celest_lightning, "celest class/kind");
    check(rocky && rocky->spell_class == "Spell_Earth_1" && rocky->kind == FaeryAbilityKindV1::strike, "rocky class/kind");
    check(wetty && wetty->spell_class == "Spell_Water_1" && wetty->kind == FaeryAbilityKindV1::strike, "wetty class/kind");
    check(windy && windy->spell_class == "Spell_Air_1" && windy->kind == FaeryAbilityKindV1::strike, "windy class/kind");
    check(hotty && hotty->spell_class == "Spell_Fire_1" && hotty->kind == FaeryAbilityKindV1::strike, "hotty class/kind");

    // FX names exactly as PlayFX(GetPyOID("AnimatedEffectTable", ...)) in the Lua.
    check(rocky && rocky->player_pre_fx == "Rocky_Level_1_Player_Pre" && rocky->target_fx == "Rocky_Level_1_Target", "rocky FX names");
    check(wetty && wetty->player_pre_fx == "Wetty_Level_1_Player_Pre" && wetty->target_fx == "Wetty_Level_1_Main", "wetty FX names");
    check(windy && windy->player_pre_fx == "Windy_Level_1_Player_Pre" && windy->target_fx == "Windy_Level_1_Main", "windy FX names");
    check(hotty && hotty->player_pre_fx == "Hotty_Level_1_Player_Pre" && hotty->target_fx == "Hotty_Level_1_Target", "hotty FX names");

    // Pre-skill 3D labels (OnPreSkill_), by target count 0, 1, 2, 3, 5, 6.
    check(same(faery_strike_pre_sound_labels_v1(*hotty, 0), {"sfx_mage_staff_elemental_fire"}), "hotty empty list");
    check(same(faery_strike_pre_sound_labels_v1(*hotty, 1), {"sfx_spell_fire_small"}), "hotty 1 target");
    check(same(faery_strike_pre_sound_labels_v1(*hotty, 5), {"sfx_spell_fire_medium"}), "hotty 5 targets");
    check(same(faery_strike_pre_sound_labels_v1(*hotty, 6), {"sfx_spell_fire_big"}), "hotty 6 targets");
    check(same(faery_strike_pre_sound_labels_v1(*wetty, 0), {"sfx_mage_staff_elemental_ice"}), "wetty empty list");
    check(same(faery_strike_pre_sound_labels_v1(*wetty, 2), {"sfx_spell_water_small"}), "wetty 2 targets");
    check(same(faery_strike_pre_sound_labels_v1(*wetty, 3), {"sfx_spell_water_medium"}), "wetty 3 targets");
    check(same(faery_strike_pre_sound_labels_v1(*wetty, 6), {"sfx_spell_water_big"}), "wetty 6 targets");
    check(same(faery_strike_pre_sound_labels_v1(*rocky, 0), {"sfx_mage_staff_elemental_earth"}), "rocky empty list");
    check(same(faery_strike_pre_sound_labels_v1(*rocky, 4), {"RockyEarthSpell"}), "rocky targets use fixed label");
    check(same(faery_strike_pre_sound_labels_v1(*windy, 0), {"sfx_mage_staff_elemental_wind"}), "windy empty list");
    check(same(faery_strike_pre_sound_labels_v1(*windy, 4), {"WindyWhirlwindSpell"}), "windy targets use fixed label");

    // Post-hit labels: Rocky, Wetty and Windy only (Hotty has no PlaySound after the loop).
    check(same(faery_strike_hit_sound_labels_v1(*rocky), {"sfx_faerie_earth"}), "rocky hit label");
    check(same(faery_strike_hit_sound_labels_v1(*wetty), {"sfx_faerie_ice"}), "wetty hit label");
    check(same(faery_strike_hit_sound_labels_v1(*windy), {"sfx_faerie_wind"}), "windy hit label");
    check(faery_strike_hit_sound_labels_v1(*hotty).empty(), "hotty has no hit label");

    // Passive: only the Celest script has CheckFaeries; it covers all five elements.
    check(celest && celest->has_passive, "celest is the passive owner");
    check(rocky && !rocky->has_passive && wetty && !wetty->has_passive &&
              windy && !windy->has_passive && hotty && !hotty->has_passive,
          "only celest has a passive");
    check(std::string(faery_passive_buff_class_v1(0, 0)) == "Celeste_LightningResistance", "lightning buff");
    check(std::string(faery_passive_buff_class_v1(1, 0)) == "Primula_EarthResistance", "earth buff");
    check(std::string(faery_passive_buff_class_v1(2, 0)) == "Maria_WaterResistance", "water buff");
    check(std::string(faery_passive_buff_class_v1(3, 0)) == "Sylph_WindResistance", "air buff");
    check(std::string(faery_passive_buff_class_v1(4, 0)) == "Avalon_FireResistance", "fire buff");
    check(std::string(faery_passive_buff_class_v1(4, 1)) == "AW_Avalon_FireResistance", "level 1 uses AW_ fire buff");
    check(std::string(faery_passive_buff_class_v1(2, 1)) == "AW_Maria_WaterResistance", "level 1 uses AW_ water buff");
    check(faery_passive_buff_class_v1(5, 0) == nullptr, "unknown element has no buff");

    // --test-faery argument: names (any case) and slots 0..4 map to FaeryList slot order.
    std::int32_t slot = -1;
    check(faery_slot_from_test_argument_v1("Celest", slot) && slot == 0, "test arg Celest is slot 0");
    check(faery_slot_from_test_argument_v1("rocky", slot) && slot == 1, "test arg rocky is slot 1");
    check(faery_slot_from_test_argument_v1("WETTY", slot) && slot == 2, "test arg WETTY is slot 2");
    check(faery_slot_from_test_argument_v1("windy", slot) && slot == 3, "test arg windy is slot 3");
    check(faery_slot_from_test_argument_v1("Hotty", slot) && slot == 4, "test arg Hotty is slot 4");
    check(faery_slot_from_test_argument_v1("4", slot) && slot == 4, "test arg 4 is slot 4");
    check(!faery_slot_from_test_argument_v1("5", slot), "test arg 5 is rejected");
    check(!faery_slot_from_test_argument_v1("Hotty2", slot), "test arg must match the whole name");
    check(!faery_slot_from_test_argument_v1("", slot), "empty test arg is rejected");

    if (failures == 0) std::printf("faery_ability_registry_v1: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
