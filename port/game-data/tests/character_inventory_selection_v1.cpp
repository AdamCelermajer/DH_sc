#include "../character_inventory_selection_v1.hpp"
#include <cstdlib>
#include <iostream>

using namespace dh2::data;
namespace {
unsigned checks = 0;
void check(bool value, const char* message) {
    ++checks;
    if (!value) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(1);
    }
}
}

int main() {
    ItemRecord164 row{};
    PropertySheet properties{};

    row.words[26] = -1;
    check(inventory_slot_matches_v1(row, properties, 9), "valuables category includes ItemTable slot -1");
    check(!inventory_slot_matches_v1(row, properties, 0), "equipment categories exclude valuables");

    row.words[26] = 3;
    check(inventory_slot_matches_v1(row, properties, 3), "regular item matches its authored equipment slot");
    check(!inventory_slot_matches_v1(row, properties, 9), "valuables category does not include regular gear");

    row.words[26] = -3;
    check(inventory_slot_matches_v1(row, properties, 1) && inventory_slot_matches_v1(row, properties, 2),
          "paired hand category is listed for either hand");
    row.words[26] = -2;
    check(inventory_slot_matches_v1(row, properties, 5) && inventory_slot_matches_v1(row, properties, 6),
          "paired ring category is listed for either finger");
    row.words[26] = -4;
    check(inventory_slot_matches_v1(row, properties, 1), "two-handed item is listed in right-hand category");
    check(!inventory_slot_matches_v1(row, properties, 2), "two-handed item is not duplicated in left-hand category");

    row.words[26] = 1;
    row.words[22] = 0;
    properties[202] = 1;
    check(inventory_slot_matches_v1(row, properties, 1) && inventory_slot_matches_v1(row, properties, 2),
          "dual-wield property exposes an ordinary weapon in both hands");
    row.words[22] = 4;
    check(inventory_slot_matches_v1(row, properties, 1), "authored weapon type 4 remains right-hand only");
    check(!inventory_slot_matches_v1(row, properties, 2), "authored weapon type 4 is not dual-listed");
    check(!inventory_slot_matches_v1(row, properties, 9), "out-of-range categories are rejected");

    std::cout << "PASS character_inventory_selection_v1 " << checks << " checks\n";
}
