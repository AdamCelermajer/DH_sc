#include "../character_menu_inventory_order_v1.hpp"

#include <cstring>
#include <iostream>

int main() {
    using dh2::data::ItemInstanceV1;
    using dh2::data::ItemRecord164;
    using dh2::ui::character_menu_item_equipment_less_v1;
    using dh2::ui::character_menu_item_equipped_in_requested_slot_v1;
    ItemRecord164 record_a{}, record_b{};
    record_a.words[8] = record_b.words[8] = 0x3f800000; // Warrior value multiplier 1.0f.
    ItemInstanceV1 a{}, b{};
    a.name = "alpha"; b.name = "beta";
    a.value = 20; b.value = 10;
    unsigned checks = 0;
    bool ok = true;
    const auto check = [&](bool value, const char* message) {
        if (!value) std::cerr << "FAIL: " << message << '\n';
        ok &= value;
        ++checks;
    };

    check(character_menu_item_equipment_less_v1(a,record_a,b,record_b,263,
              false,true,true,false,false),
          "an item equipped in the paired hand sorts before other candidates");
    check(character_menu_item_equipment_less_v1(a,record_a,b,record_b,263,
              true,false,false,false,false),
          "equippable item sorts before a non-equippable item");
    check(character_menu_item_equipment_less_v1(a,record_a,b,record_b,263,
              true,true,false,false,false),
          "equal eligibility uses source class-adjusted value descending");
    const bool requested_hand = character_menu_item_equipped_in_requested_slot_v1(true,false);
    const bool paired_hand = character_menu_item_equipped_in_requested_slot_v1(true,true);
    check(requested_hand && !paired_hand,
          "requested-hand projection distinguishes the paired equipped item");
    check(character_menu_item_equipment_less_v1(a,record_a,b,record_b,263,
              true,true,true,true,requested_hand) &&
          !character_menu_item_equipment_less_v1(b,record_b,a,record_a,263,
              true,true,true,true,paired_hand),
          "requested-hand equipped item sorts before the opposite-hand item");
    b.value = a.value; b.name = "zeta";
    check(character_menu_item_equipment_less_v1(a,record_a,b,record_b,263,
              true,true,false,false,false),
          "equal score uses source item-name ordering");

    std::cout << "{\"validation\":\"" << (ok ? "PASS" : "FAIL")
              << "\",\"checks\":" << checks << "}\n";
    return ok ? 0 : 1;
}
