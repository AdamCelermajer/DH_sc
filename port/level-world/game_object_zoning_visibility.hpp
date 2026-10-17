#pragma once

#include <cstddef>
#include <cstdint>

namespace dh2::game_object_zoning_visibility {

using Address = std::uintptr_t;

struct VisualObject;

// Borrowed source facts for one GameObject. The byte and pointer names are
// their original offsets: zoning_enabled_2ee (+0x2ee), in_zone_2f0 (+0x2f0),
// room_zone_2f4 (+0x2f4), visibility_80 (+0x80), and visual_2d8 (+0x2d8).
// `identity` is opaque and full-width in the port; it is never treated as an
// original 32-bit pointer. Callbacks may update source fields, but must keep
// this projection and its identity alive until the call returns.
struct GameObject {
    Address identity;
    // Current virtual target for calls through vtable offset +0xc4. Unlike the
    // +0x3c callback, the source reloads this target for every IsZonable call.
    Address is_zonable_target_c4;
    // Exact vtable entry captured from +0x3c before the source IsZonable call.
    // The adapter invokes this saved identity even if that callback changes
    // the object's later vtable projection.
    Address set_updating_target_3c;
    Address room_zone_2f4;
    VisualObject* visual_2d8;
    std::uint8_t zoning_enabled_2ee;
    std::uint8_t in_zone_2f0;
    std::uint8_t visibility_80;
};

// VisualObject +4 is the owning GameObject pointer. The renderer/scene-node
// pointer at +8 is intentionally not inferred here; SetVisible is an explicit
// service boundary that owns that source behavior.
struct VisualObject {
    Address identity;
    Address owner_identity;
};

enum class Operation : std::uint32_t {
    is_zonable,
    room_remove_object,
    object_manager_add_no_room,
    object_manager_remove_no_room,
    room_add_object,
    room_is_zoned,
    zone_entered,
    zone_exited,
    set_updating,
    set_visible
};

struct Request {
    Operation operation;
    Address object;
    // Room identity, Visual owner, or the previously captured vtable target,
    // according to `operation`.
    Address related;
    // Exact raw source argument for set_updating; canonical 0/1 for
    // set_visible. Other operations leave this zero.
    std::uint32_t value;
};

struct Services {
    void* context;
    // Optional known extent for alias checks; pass zero only when the adapter
    // cannot describe the backing extent. The callback/context must outlive
    // the entire synchronous operation and may mutate borrowed source facts.
    std::size_t context_extent;
    // Return zero on success. is_zonable writes a raw ARM result word;
    // room_is_zoned writes the source byte value (0..255).
    std::int32_t (*invoke)(void*, const Request*, std::uint32_t* raw_result);
};

enum class Status : std::int32_t {
    complete,
    invalid_argument,
    invalid_source_fact,
    service_unavailable,
    service_failed
};

struct Result {
    std::uint32_t service_calls;
    std::uint32_t source_writes;
    std::uint32_t last_raw_result;
    std::uint32_t last_updating_argument;
    std::uint32_t last_visible_argument;
    Address final_room_zone;
    std::uint8_t final_zoning_enabled;
    std::uint8_t final_in_zone;
    std::uint8_t final_visibility;
};

// Source bodies: GameObject::DisableZoning (0x38c600/156),
// GameObject::EnableZoning (0x38c790/236), and VisualObject::SyncVisibility
// (0x4713d0/108). Each external game action is a typed synchronous service.
// On adapter/service failure, completed source writes and callbacks remain;
// the kernel performs no rollback.
Status disable_zoning(GameObject*, const Services*, Result*);
Status enable_zoning(GameObject*, const Services*, Result*);
Status sync_visibility(VisualObject*, GameObject*, const Services*, Result*);

} // namespace dh2::game_object_zoning_visibility
