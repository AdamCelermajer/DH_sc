#pragma once

#include "game_object_zoning_visibility.hpp"
#include "game_object_set_visible.hpp"
#include "object_manager_runtime_owner_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::game_object_visual_attachment_v1 {

// Borrowed candidate from the renderer's existing visual owner. The scene root
// comes from that owner; this adapter never creates
// a VisualObject, SceneNode, or Character.
struct Candidate {
    game_object_zoning_visibility::VisualObject* visual{};
    game_object_set_visible::SceneNode* root_scene_node{};
};

struct Services {
    void* context{};
    // Mirrors the source virtual destructor call through VisualObject vtable
    // byte offset +4. Failure must occur before the callback mutates its owner.
    int (*destroy_visual)(void*, std::uintptr_t visual_identity){};
};

struct Result {
    std::uintptr_t previous_visual{};
    std::uintptr_t current_visual{};
    std::uint32_t destructor_calls{};
    std::uint32_t game_object_stores{};
    std::uint32_t root_owner_stores{};
    bool rejected_missing_scene_node{};
};

enum class Status {
    complete,
    invalid_argument,
    invalid_visual_owner,
    missing_scene_node,
    service_unavailable,
    service_failed,
};

// The source GameObject::SetVisualObject(VisualObject*) transaction at 0x394338
// plus the caller's root link at 0x394d34. A replacement destroys the old
// VisualObject via vtable+4, clears GameObject+0x2d8, stores the new identity,
// then writes root+0x204. The constructor has already set VisualObject+4 to
// the owning GameObject.
// If a newly constructed candidate has no scene node, source deletes that
// candidate and keeps the previously attached VisualObject untouched.
Status set_visual_object(object_manager_runtime_owner_v1::GameObject*,
                         Candidate, const Services*, Result*,
                         std::string& error);

} // namespace dh2::game_object_visual_attachment_v1
