#include "context_button_v1.hpp"

#include <array>

namespace dh::foundation {
namespace {
constexpr int combat_type_v1 = 8;   // enemy/monster Character, DestructibleContainer
constexpr int invalid_icon_v1 = 5;  // source: negative or above 10 -> icon 5
constexpr std::array<int, 11> icon_table_v1{0, 1, 2, 3, 5, 5, 6, 7, 5, 5, 4}; // MenuManager table 0x8c9f28
} // namespace

ContextButtonDecisionV1 decide_context_button_v1(const ContextButtonInputV1& in) noexcept {
    ContextButtonDecisionV1 out;
    if (!in.pressed_edge && !in.held) return out;

    if (!in.object_present) {
        // Source HUD: no OOI -> Cmd_Attack(null), the held melee on the current/last target.
        out.attack_held = true;
        return out;
    }

    // A destructible container (non-actor type 8) is not a combat OOI: the press hits it once.
    const bool combat = in.object_type == combat_type_v1 && in.object_is_actor;
    // Source UseOOI gate: no current attack target, and the state is idle or moving.
    const bool use_gate_open = !in.owner_has_attack_target && in.owner_idle_or_moving;

    // Source Cmd_UseOOI runs instead of Cmd_Attack while an OOI exists. Enemy OOIs keep the
    // attack flowing (now aimed at the OOI); non-combat OOIs suppress the attack.
    out.attack_held = combat;
    // PC adaptation: non-combat interactions start on the press edge only (never by holding).
    out.use_object_of_interest = use_gate_open && (in.pressed_edge || combat);
    if (in.pressed_edge && !combat) out.interaction_type = in.object_type;
    return out;
}

int action_button_icon_v1(int cached_type) noexcept {
    if (cached_type < 0 || cached_type > 10) return invalid_icon_v1;
    return icon_table_v1[static_cast<std::size_t>(cached_type)];
}

const char* action_button_label_v1(int icon) noexcept {
    switch (icon) {
        case 0: return "Chest";
        case 1: return "Item";
        case 2: return "Lever";
        case 3: return "Talk";
        case 4: return "Revive";
        case 5: return "Attack";
        default: return "Action";
    }
}

} // namespace dh::foundation
