#include "equipment_inventory_actions_v1.hpp"
#include <iostream>
#include <string>

using namespace dh::foundation;
using namespace dh::foundation::equipment_menu;

namespace {
int failures = 0;
void check(bool ok, const std::string& text) {
    if (!ok) { std::cerr << "FAIL: " << text << '\n'; ++failures; }
}
CharacterState make() {
    CharacterState c;
    c.gold = 13;
    c.inventory = {{"sword", "Longsword01", 1}, {"boots", "Boots01", 1}, {"stack", "Potion0", 3}};
    c.equipment = {{"slot1", "sword", 0, 1}};
    return c;
}
}  // namespace

int main() {
    std::string error;
    // Transmute: single item leaves the bag and gold is added.
    {
        auto c = make();
        check(transmute_inventory_item(c, "boots", 11, error), error);
        check(c.gold == 24 && c.inventory.size() == 2, "single transmute must remove the item and add its amount");
        check(c.inventory[0].instance_id == "sword" && c.inventory[1].instance_id == "stack", "transmute removed the wrong row");
    }
    // Stack: AddQty(-1) keeps the row.
    {
        auto c = make();
        check(transmute_inventory_item(c, "stack", 5, error), error);
        check(c.gold == 18 && c.inventory.size() == 3 && c.inventory[2].quantity == 2, "stack transmute must take one unit");
    }
    // Failures leave the state untouched.
    {
        auto c = make();
        const auto before = c;
        check(!transmute_inventory_item(c, "sword", 5, error) && !error.empty(), "equipped item transmuted");
        check(!transmute_inventory_item(c, "missing", 5, error), "unknown item transmuted");
        check(!transmute_inventory_item(c, "", 5, error), "empty id transmuted");
        check(!transmute_inventory_item(c, "boots", 0, error), "amount 0 accepted (source clamps to 1 before this call)");
        check(c.gold == before.gold && c.inventory.size() == before.inventory.size() && c.equipment.size() == 1, "failed transmute mutated state");
        c.gold = UINT64_MAX;
        check(!transmute_inventory_item(c, "boots", 1, error) && c.inventory.size() == 3, "gold overflow accepted");
    }
    // Drop: publishes ONE unit through the world seam, then removes it from the bag; refuses equipped/unknown.
    {
        auto c = make();
        static int calls = 0;
        static InventoryItem last;
        calls = 0;
        const WorldDropFn publish = [](const InventoryItem& unit, std::string&) { ++calls; last = unit; return true; };
        check(drop_inventory_item(c, "boots", error, publish) && calls == 1 && last.quantity == 1 && c.inventory.size() == 2,
              "drop did not publish the item and remove it from the bag");
        check(!drop_inventory_item(c, "sword", error, publish) && calls == 1, "equipped item reached the world seam");
        check(!drop_inventory_item(c, "boots", error, publish) && calls == 1, "already dropped item reached the world seam");
        check(drop_inventory_item(c, "stack", error, publish) && last.quantity == 1 && c.inventory[1].quantity == 2,
              "stack drop must publish one unit and keep the row");
    }
    // Drop failure: the bag is unchanged and the reason is returned.
    {
        auto c = make();
        const WorldDropFn failing = [](const InventoryItem&, std::string& e) { e = "world full"; return false; };
        check(!drop_inventory_item(c, "stack", error, failing) && error == "world full" && c.inventory.size() == 3 && c.inventory[2].quantity == 3,
              "publish failure must keep the item");
        check(!drop_inventory_item(c, "stack", error) && error == "no world store bound" && c.inventory.size() == 3,
              "unbound world store must fail loudly and keep the item");
    }
    if (failures) return 1;
    std::cout << "equipment_inventory_actions PASS\n";
    return 0;
}
