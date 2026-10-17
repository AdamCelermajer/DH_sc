#pragma once

#include <cstdint>

namespace dh2::room_zone_enrollment {

using Address = std::uintptr_t;

// Borrowed source projections. The floats are RoomZone bounds at +0x12c,
// +0x130, +0x138, +0x13c; the source-plane XY coordinates are GameObject
// +0x160/+0x164. This does not assert how those axes map to rendered XYZ.
struct Bounds {
    const float* min_x;
    const float* min_y;
    const float* max_x;
    const float* max_y;
};

struct RoomZone {
    Address identity;
    Bounds bounds;
};

// Field pointers address live source-backed fields: room_zone is +0x2f4,
// in_room_list is +0x2ef, in_zone is +0x2f0, zoning_enabled is +0x2ee,
// visual_object is +0x2d8, and visible is +0x80.
struct GameObject {
    Address identity;
    const float* world_x;
    const float* world_y;
    Address* room_zone;
    std::uint8_t* in_room_list;
    std::uint8_t* in_zone;
    const std::uint8_t* zoning_enabled;
    const Address* visual_object;
    const std::uint8_t* visible;
};

enum class Operation : std::uint32_t {
    is_zonable,
    remove_from_room,
    append_to_room,
    sync_visibility,
    zone_state_callback
};

struct Request {
    Operation operation;
    Address object;
    Address room_zone;
    // VisualObject* for sync_visibility; otherwise zero.
    Address auxiliary;
    // For zone_state_callback this is the raw argument passed to the source
    // vtable slot at +0x3c. Other operations leave it zero.
    std::uint32_t value;
};

struct Services {
    void* context;
    // Return zero on success. is_zonable writes its source raw result to value.
    // All other operations are source-owner/list adapters with observable order.
    std::int32_t (*invoke)(void*, const Request*, std::uint32_t* value);
};

enum class Status : std::int32_t {
    complete,
    invalid_argument,
    service_unavailable,
    service_failed
};

enum class Rejection : std::uint32_t {
    none,
    not_zonable,
    outside_bounds
};

struct Result {
    bool accepted;
    Rejection rejection;
    std::uint32_t calls;
    Address resulting_room_zone;
    std::uint8_t resulting_in_room_list;
    std::uint8_t resulting_in_zone;
};

// Equivalent to the source decision/effect sequence of
// RoomZone::AddInitialObject (0x396a90). The trace-only DebugSwitches strings
// and logging are omitted. Spatial inclusion uses source XY (world_x/world_y)
// and source float <=, including equality and NaN behavior.
Status add_initial_object(RoomZone*, GameObject*, const Services*, Result*);

// Source leaf behavior from GameObject::ZoneEntered (0x38c710) and
// ZoneExited (0x38c69c), retaining the raw vtable callback value and order.
Status zone_entered(GameObject*, const Services*, Result*);
Status zone_exited(GameObject*, const Services*, Result*);

} // namespace dh2::room_zone_enrollment
