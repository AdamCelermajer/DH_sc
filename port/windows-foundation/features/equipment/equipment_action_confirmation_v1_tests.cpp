#include "equipment_action_confirmation_v1.hpp"
#include <cstdlib>
#include <iostream>

using namespace dh::foundation;
using namespace dh::foundation::equipment_menu::confirmation_v1;

namespace {
void check(bool value, const char* name) {
    if (!value) { std::cerr << "FAIL: " << name << '\n'; std::exit(1); }
}
}

int main() {
    std::string error;
    CharacterState owner;
    owner.inventory.push_back({"instance-a", "definition-a", 2});
    Pending popup;
    check(!popup.open(Action::none, owner.inventory[0], 1, 0, 44, error) && !popup.visible(), "no command opens no popup");
    check(popup.open(Action::transmute, owner.inventory[0], 1, 7, 44, error), "admitted transmute opens");
    const auto snapshot_gold = owner.gold;
    const auto cancel = popup.release(Button::no, owner, 44, [](const Request&, const InventoryItem&, std::string&) { return true; }, error);
    check(cancel.outcome == Outcome::cancelled && !popup.visible(), "No cancels popup");
    check(owner.inventory[0].quantity == 2 && owner.gold == snapshot_gold, "cancel does not mutate");

    check(popup.open(Action::drop, owner.inventory[0], 1, 0, 44, error), "admitted drop opens");
    check(popup.release(Button::other, owner, 44, {}, error).outcome == Outcome::swallowed && popup.visible(), "outside release swallowed");
    bool current_called = false;
    const auto yes = popup.release(Button::yes, owner, 44, [&](const Request& admitted, const InventoryItem& item, std::string&) {
        current_called = item.instance_id == "instance-a" && admitted.admitted_item.quantity == item.quantity; return current_called;
    }, error);
    check(yes.outcome == Outcome::confirmed && yes.request.action == Action::drop && current_called && !popup.visible(), "Yes confirms exact current row once");
    check(popup.release(Button::yes, owner, 44, {}, error).outcome == Outcome::swallowed, "repeated Yes cannot duplicate action");

    check(popup.open(Action::transmute, owner.inventory[0], 1, 7, 44, error), "stale case opens");
    owner.inventory[0].quantity++;
    const auto stale = popup.release(Button::yes, owner, 44, [](const Request&, const InventoryItem&, std::string&) { return true; }, error);
    check(stale.outcome == Outcome::rejected && !popup.visible(), "quantity change rejects stale selection");

    check(popup.open(Action::transmute, owner.inventory[0], 1, 7, 44, error), "source payload case opens");
    const auto changed_payload = popup.release(Button::yes, owner, 44, [](const Request& admitted, const InventoryItem& current, std::string& e) {
        if (admitted.transmute_value != 7 || current.instance_id != admitted.instance_id) return false;
        e = "current source value/identified/powers differ from admitted snapshot";
        return false;
    }, error);
    check(changed_payload.outcome == Outcome::rejected && !popup.visible(), "changed source payload rejects confirmation");

    check(popup.open(Action::drop, owner.inventory[0], 1, 0, 44, error), "foreign case opens");
    owner.inventory.clear();
    const auto foreign = popup.release(Button::yes, owner, 44, [](const Request&, const InventoryItem&, std::string&) { return true; }, error);
    check(foreign.outcome == Outcome::rejected && !popup.visible(), "foreign/unowned instance rejected");

    owner.inventory.push_back({"instance-b", "definition-b", 1});
    check(popup.open(Action::drop, owner.inventory[0], 1, 0, 44, error), "equipped case opens");
    owner.equipment.push_back({"slot1", "instance-b"});
    check(popup.release(Button::yes, owner, 44, [](const Request&, const InventoryItem&, std::string&) { return true; }, error).outcome == Outcome::rejected, "newly equipped row rejected");

    owner.equipment.clear();
    check(popup.open(Action::drop, owner.inventory[0], 1, 0, 44, error), "expired session case opens");
    check(popup.release(Button::yes, owner, 45, [](const Request&, const InventoryItem&, std::string&) { return true; }, error).outcome == Outcome::rejected,
          "session replacement invalidates confirmation");
    std::cout << "equipment_action_confirmation_v1_tests passed\n";
}
