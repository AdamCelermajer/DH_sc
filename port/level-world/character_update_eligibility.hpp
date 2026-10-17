#pragma once

#include <cstdint>

namespace dh2::character_update_eligibility {

// Logical projections read by Character::CanUpdate. These are not native
// overlays of Character, VisualObject, or RootSceneNode.
struct SceneNode {
    std::uintptr_t identity;
    std::uint32_t culling_word_118;
    std::uint8_t update_flag_200;
    std::uint8_t reserved[3];
};
struct Visual {
    std::uintptr_t identity;
    SceneNode* root;
};
struct Character {
    std::uintptr_t identity;
    Visual* visual_2d8;
    std::uintptr_t field_418;
    // ObjectBase current-visibility byte. GameObject::SetVisible writes it;
    // this source predicate does not interpret it as an update-enable flag.
    std::uint8_t current_visibility_80;
    std::uint8_t culling_field_2fc;
    std::uint8_t culling_gate_1480;
    std::uint8_t reserved;
};

enum class Operation : std::uint32_t {
    get_online_byte,
    is_remotely_updated,
    local_player_character,
    test_culling_before_update,
    is_dead,
    can_respawn,
};
struct Request {
    Operation operation;
    std::uintptr_t character;
    std::uintptr_t subject;
    // For TestCullingBeforeUpdate, argument is source AABB byte offset 0x12c,
    // not a fabricated native pointer into the logical Character view.
    std::uintptr_t argument;
    std::uint32_t first, second;
};
struct Response {
    // For get_online_byte this must be an exact uint8 source read (0..255);
    // all virtual/helper predicates preserve their raw 32-bit return word.
    std::uint32_t raw;
    std::uintptr_t identity;
};
struct Services {
    void* context;
    // Return zero on port success. Boolean-like values remain raw source words
    // and the caller applies only the original zero/nonzero branches.
    std::int32_t (*invoke)(void*, const Character*, const Request*, Response*);
};

enum class Outcome : std::uint32_t {
    not_started,
    allowed,
    culled,
    dead_cannot_respawn,
};
struct Result {
    std::uint32_t can_update;
    Outcome outcome;
    std::uintptr_t captured_visual;
    std::uint32_t service_calls;
    std::uint32_t scene_flag_writes;
};
enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    service_unavailable = 2,
    service_failed = 3,
    invalid_source_fact = 4,
    reentrant_call = 5,
};

// Complete 308-byte Character::CanUpdate body. This is the eligibility call
// made by Character::Update at 0x3abf60; it does not reconstruct the rest of
// Character::Update or the subsequent script/AIS scheduler.
// Borrowed Character/Visual/SceneNode and service-context storage must remain
// alive and stable in address through return. Character/Visual identity
// metadata is stable for a call; source fields and Visual::root can still
// mutate through callbacks. The Services value is copied on entry. Calls over
// the same Character are serialized by the caller; recursive same-Character
// evaluation is rejected as port protocol.
Status evaluate(Character*, const Services*, Result*);

}  // namespace dh2::character_update_eligibility
