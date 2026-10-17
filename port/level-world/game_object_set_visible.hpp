#pragma once

#include <cstddef>
#include <cstdint>

namespace dh2::game_object_set_visible {
using Address = std::uintptr_t;
struct SceneNode {
    Address identity;
    Address set_visible_target_48;
    std::uint32_t flags_11c;
    // GameObject::SetVisualObject writes its owner into the root node at
    // VisualObject+8+0x204 after publishing the VisualObject at GameObject+0x2d8.
    Address owner_game_object_204{};
    // Optional borrowed backing field for a live renderer-owned scene node.
    // The host projection remains the fallback when no live owner is bound.
    Address* owner_game_object_204_live{};
};
struct VisualObject { Address identity; SceneNode* root_8; };
struct GameObject {
    Address identity;
    VisualObject* visual_2d8;
    std::uint8_t visibility_80;
    std::uint8_t enabled_8a;
};
struct Device { Address scene_manager_1c; };
struct Application { Device* device_10; };
// Borrowed projection of the inline Singleton<Application>::s_inst object,
// not a source Application* stored in that symbol.
struct Globals { Application* application; };

enum class Operation : std::uint32_t { sync_visibility, force_register, node_set_visible };
struct Request {
    Operation operation;
    Address object;
    Address target; // Captured SceneNode virtual +0x48 target; otherwise zero.
    std::uint32_t value; // Exact raw R1 for node_set_visible; otherwise zero.
};
struct Services {
    void* context;
    std::size_t context_extent; // Optional extent; result must not alias it.
    std::int32_t (*invoke)(void*, const Request*); // Zero success; exceptions/nonzero stop.
};
enum class Status : std::int32_t {
    complete, invalid_argument, invalid_source_fact, service_unavailable, service_failed
};
struct Result {
    std::uint32_t service_calls;
    std::uint32_t source_writes;
    std::uint32_t last_operation;
    std::uint32_t last_argument;
    Address captured_visual;
    Address dispatched_root;
    std::uint8_t written_visibility;
};

// Complete original callers: GameObject::SetVisible 0x38b0f0/32 and
// VisualObject::SetVisible 0x471368/104. SyncVisibility, ForceRegister and the
// current scene-node virtual setter remain mandatory external source providers.
// Raw arguments/bytes are not canonicalized. All reached projections and their
// identities stay live through synchronous return, including callbacks; mutable
// pointer/flag fields may change. Services are copied before callbacks. Separate
// result storage permits nested calls, including the same owner; active result
// storage cannot be reused. No rollback/extra cleanup on provider failure.
// A null visual/root skips services. Null application/device on a reached source
// dereference is a port error, not an invented source null branch.
Status set_game_object(GameObject*, std::uint32_t raw_argument, const Services*, Result*);
Status set_visual_object(VisualObject*, std::uint32_t raw_argument, const Globals*, const Services*, Result*);
}
