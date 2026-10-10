#pragma once

// P16 CONTEXT: the single PC context button (Space) and the HUD action-button icon.
//
// Source (decoded in coordination/claude-preview16/CONTEXT-report.md section 1.4-1.5):
// - HUDControls::Update 0x41a780 runs each frame while the action button is held (HUDControls+9 level flag).
//   OOI != 0 -> Cmd_UseOOI (Ctrl_UseOOI -> UseOOI: AI target = OOI when no attack target is set and the
//   state is idle or moving). OOI == 0 -> Cmd_Attack(null) (melee on the current target).
// - MenuManager::Update 0x42eab4 maps the cached OOI type (Character+0x14a8) to the btn_interact icon.
//
// PC adaptation (user decision, explicit): the source button is held-level; here it is a press EDGE for
// every non-combat interaction. A held button never opens a chest or starts a talk; it only keeps the
// attack going. Enemy OOIs keep the source held behaviour (attack the OOI while held).

#include <cstdint>

namespace dh::foundation {

// Snapshot of the inputs read for one frame. No pointers; the caller reads the live owner.
struct ContextButtonInputV1 {
    bool pressed_edge = false;          // Space went down this frame
    bool held = false;                  // Space is down (includes the edge frame)
    bool object_present = false;        // Character OOI != 0 (source HUD test is on the OOI pointer)
    int object_type = -1;               // cached OOI type (Character+0x14a8)
    bool owner_has_attack_target = false;   // Character+1032 != 0 (UseOOI gate)
    bool owner_idle_or_moving = true;       // SM_IsIdle || SM_IsMoving (UseOOI gate)
};

struct ContextButtonDecisionV1 {
    // Source Cmd_UseOOI: AI_SetTarget(OOI) when the UseOOI gate is open. The AI then uses the type.
    bool use_object_of_interest = false;
    // Source Cmd_Attack(null): held melee on the current/OOI target.
    bool attack_held = false;
    // Interaction the press starts (type 0 chest, 3 talk, ...). -1 when none.
    int interaction_type = -1;
};

ContextButtonDecisionV1 decide_context_button_v1(const ContextButtonInputV1& input) noexcept;

// Source MenuManager::Update icon table (0x8c9f28) for the cached OOI type. Out-of-range -> 5.
// Frame index of the authored btn_interact btimg: 0 chest, 3 talk, 4 revive, 5 attack/sword.
int action_button_icon_v1(int cached_type) noexcept;

} // namespace dh::foundation
