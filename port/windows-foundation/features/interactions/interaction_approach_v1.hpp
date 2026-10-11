#pragma once

// B069: the original context button does not require the hero to stand in range. Source chain
// (decoded in coordination/claude-preview161/B069-report.md): HUD action button -> Cmd_UseOOI -> Character::UseOOI
// (AI_SetTarget(OOI)) -> the AI state AISDefault::OnTargetInMeleeRange / OnTargetInCloseRange /
// OnTargetInRangedRange (0x3dc284 / 0x3dc300 / 0x3dc560): when CharAI::AI_IsInInteractionRange
// (0x3d4f98) is true the hero stops (Cmd_Stop) and CharAI::AI_InteractWith (0x3cff34) interacts; otherwise
// the AI issues Cmd_MoveTo(GameObject::GetInteractionSpot(target)) and the hero walks to the object.
//
// This module holds the two pure rules (range test, world-to-camera movement conversion). The host owns
// the "approach" state (target id) and feeds the walking intent to the normal player movement.

#include <array>

namespace dh::foundation {

struct InteractionRangeInputV1 {
    std::array<float, 3> owner_target_position{};  // GetTargetPosition(owner)
    std::array<float, 3> interaction_spot{};       // GetInteractionSpot(target): node 'interaction_position' or GetTargetPosition
    float owner_melee_radius = 0.0f;               // CharAI::AI_GetMeleeRadius(owner) (AI row MeleeRadius + weapon reach)
    float target_radius = 0.0f;                    // vt+148(target) (GameObject::GetInteractionRadius: half the smaller bbox side)
    float owner_interact_radius = 0.0f;            // owner AIProps.InteractRadius (AITable row +24)
    int target_type = -1;                          // vt+144(target, owner)
    bool has_interaction_node = false;             // target+744 (cached 'interaction_position' node) non-null
};

// Source CharAI::AI_IsInInteractionRange (pseudocode 153119, non-null target):
//   d = |ownerPos - spot| (3D)
//   node present: radiusT = 0, meleeO = 0, limit = 80
//   else:         radiusT = vt+148(target), meleeO = AI_GetMeleeRadius(owner), limit = owner InteractRadius
//   v = (d - meleeO) - radiusT
//   in range = (type == 8) ? v <= 0 : v <= limit
bool interaction_in_range_v1(const InteractionRangeInputV1& input) noexcept;

// Converts a world-plane walking direction into the camera-relative move2D (x right, y forward) that the
// player movement consumes (movement = right*x + forward*y). Returns false for a degenerate basis or a zero
// direction. The result is normalised to unit length.
struct InteractionMove2DV1 { float x = 0.0f; float y = 0.0f; };
bool camera_relative_move_v1(float right_x, float right_y, float forward_x, float forward_y,
                             float world_dx, float world_dy, InteractionMove2DV1& out) noexcept;

} // namespace dh::foundation
