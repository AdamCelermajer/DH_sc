// P16 CONTEXT: Space context-button rules (press edge, held attack) and the action-button icon table.
#include "context_button_v1.hpp"

#include <cstdio>
#include <string>

using namespace dh::foundation;

namespace {
int failures = 0;
void check(bool ok, const char* name) {
    std::printf("%s %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) ++failures;
}

ContextButtonInputV1 press(bool object, int type, bool has_target = false, bool idle_or_moving = true) {
    ContextButtonInputV1 in;
    in.pressed_edge = true;
    in.held = true;
    in.object_present = object;
    in.object_type = type;
    in.owner_has_attack_target = has_target;
    in.owner_idle_or_moving = idle_or_moving;
    return in;
}

ContextButtonInputV1 hold(bool object, int type) {
    auto in = press(object, type);
    in.pressed_edge = false;
    return in;
}
} // namespace

int main() {
    {
        ContextButtonInputV1 none;
        const auto d = decide_context_button_v1(none);
        check(!d.attack_held && !d.use_object_of_interest && d.interaction_type == -1, "no press and no hold: nothing");
    }
    {
        const auto d = decide_context_button_v1(press(false, -1));
        check(d.attack_held && !d.use_object_of_interest, "press with no OOI attacks (Cmd_Attack null)");
    }
    {
        const auto d = decide_context_button_v1(hold(false, -1));
        check(d.attack_held && !d.use_object_of_interest, "held with no OOI keeps the held attack");
    }
    {
        const auto d = decide_context_button_v1(press(true, 0));
        check(d.use_object_of_interest && d.interaction_type == 0 && !d.attack_held,
              "press on a chest uses the OOI and suppresses the attack");
    }
    {
        const auto d = decide_context_button_v1(hold(true, 0));
        check(!d.use_object_of_interest && d.interaction_type == -1 && !d.attack_held,
              "held over a chest never opens it (PC press-edge rule)");
    }
    {
        const auto d = decide_context_button_v1(press(true, 0, true));
        check(!d.use_object_of_interest && !d.attack_held, "press with an attack target already set: UseOOI gate closed");
    }
    {
        const auto d = decide_context_button_v1(press(true, 0, false, false));
        check(!d.use_object_of_interest, "UseOOI gate closed while casting/attacking (not idle or moving)");
    }
    {
        const auto d = decide_context_button_v1(press(true, 3));
        check(d.use_object_of_interest && d.interaction_type == 3, "press on a friendly NPC starts talk (type 3)");
    }
    {
        const auto d = decide_context_button_v1(hold(true, 3));
        check(!d.use_object_of_interest && d.interaction_type == -1, "held over an NPC does not talk");
    }
    {
        const auto d = decide_context_button_v1(press(true, 8));
        check(d.use_object_of_interest && d.attack_held && d.interaction_type == -1,
              "press on an enemy OOI: UseOOI targets it and the attack continues");
    }
    {
        const auto d = decide_context_button_v1(hold(true, 8));
        check(d.use_object_of_interest && d.attack_held, "held enemy OOI keeps the source per-frame UseOOI/attack");
    }

    // Action-button icon: MenuManager table [0,1,2,3,5,5,6,7,5,5,4], out of range -> 5.
    check(action_button_icon_v1(0) == 0, "icon: chest (type 0) -> frame 0");
    check(action_button_icon_v1(3) == 3, "icon: NPC talk (type 3) -> frame 3");
    check(action_button_icon_v1(8) == 5, "icon: enemy (type 8) -> attack frame 5");
    check(action_button_icon_v1(-1) == 5, "icon: no OOI (-1) -> attack frame 5");
    check(action_button_icon_v1(10) == 4, "icon: type 10 -> frame 4");
    check(action_button_icon_v1(11) == 5 && action_button_icon_v1(-5) == 5, "icon: out of range -> 5");

    // P16 SPACEBTN: a non-actor destructible (barrel, type 8) is not a combat OOI: one hit per press edge, no held melee.
    {
        ContextButtonInputV1 barrelPress = press(true, 8);
        barrelPress.object_is_actor = false;
        const auto d = decide_context_button_v1(barrelPress);
        check(d.use_object_of_interest && !d.attack_held, "barrel (non-actor type 8) press uses the OOI, no held melee");
        ContextButtonInputV1 barrelHold = hold(true, 8);
        barrelHold.object_is_actor = false;
        const auto h = decide_context_button_v1(barrelHold);
        check(!h.use_object_of_interest && !h.attack_held, "barrel hold does nothing (no repeated hits)");
        ContextButtonInputV1 enemyHold = hold(true, 8);
        const auto e = decide_context_button_v1(enemyHold);
        check(e.attack_held && e.use_object_of_interest, "enemy actor (type 8) hold keeps the attack (default object_is_actor)");
    }
    check(std::string(action_button_label_v1(0)) == "Chest" && std::string(action_button_label_v1(3)) == "Talk", "label: chest and talk names");
    check(std::string(action_button_label_v1(5)) == "Attack" && std::string(action_button_label_v1(-1)) == "Action", "label: attack default, unknown -> Action");

    std::printf("%s\n", failures == 0 ? "context button tests passed" : "context button tests FAILED");
    return failures == 0 ? 0 : 1;
}
