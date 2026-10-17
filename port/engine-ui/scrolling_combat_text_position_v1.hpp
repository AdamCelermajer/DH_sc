#pragma once

#include <cstdint>
#include <string>

namespace dh2::scene { struct Scene; }

namespace dh2::ui {

// Typed projection of source GameObject::GetTargetPosition and
// Character::SetRelativeAABB. ELF evidence: GameObject::InitFinal (0x38cd48)
// resolves "target_node" and stores its node at +0x180; UpdateTargetPosition
// (0x393d74) caches absolute XYZ at +0x184..+0x18c; GetTargetPosition
// (0x3935dc) chooses that cache only when +0x180 and +0x80 are nonzero, else
// +0x160. SetVisible (0x38b0f0) writes +0x80. The renderer coordinator
// supplies identity-matched live facts; this adapter owns source selection
// and Character-height calculation, not actor state.
struct ScrollingCombatTextPositionFactsV1 {
    std::uintptr_t identity{};
    const float* game_object_position{}; // source +0x160
    const dh2::scene::Scene* visual_scene{}; // retained graph used by InitFinal's lookup
    const float* relative_box{};         // source Character +0x144, six floats
    std::uint8_t source_visible_80{}; // SetVisible effective byte consumed by getter
};

struct ScrollingCombatTextPositionLookupV1 {
    void* context{};
    bool (*lookup)(void*,std::uintptr_t,
                   ScrollingCombatTextPositionFactsV1&,std::string&){};
};

// Mirrors InitFinal's exact-name scene-node lookup using the authored scene
// graph's first depth-first match. SceneBinding has already produced each
// node's absolute world matrix; absent `target_node` is a successful lookup
// with present=false and means source +0x180 would be null.
bool resolve_scrolling_combat_target_node_v1(
    const dh2::scene::Scene&,bool& present,std::uint32_t& node_index,
    float world_xyz[3],std::string& error);

// Resolve target_node from the retained visual graph. Select its absolute
// position iff it exists and the effective visibility byte +0x80 is true;
// otherwise use +0x160. In both cases add relative_box[5]-relative_box[2]
// to Z, matching source GetTargetPosition plus combat-text height offset.
bool resolve_scrolling_combat_text_position_v1(
    std::uintptr_t requested_identity,
    const ScrollingCombatTextPositionFactsV1&,
    float world_xyz[3],std::string& error) noexcept;

// Callback-shaped adapter suitable for ScrollingCombatTextBridgeProvidersV1.
bool scrolling_combat_text_position_v1(
    void* lookup_context,std::uintptr_t requested_identity,
    float world_xyz[3],std::string& error) noexcept;

} // namespace dh2::ui
