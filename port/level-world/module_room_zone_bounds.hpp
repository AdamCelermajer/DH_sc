#pragma once

#include <cstdint>
#include <cstddef>

namespace dh2::module_room_zone_bounds {

using Address = std::uintptr_t;

// Borrowed six-float box passed by Module::InitPost to
// Zone::InitWithBoundingBox. The two triples are minimum then maximum.
struct Box3 {
    const float* minimum;
    const float* maximum;
};

// Live projections of a RoomZone object. These point at the original
// GameObject/Zone fields: dimensions +0x374..0x37c, relative min/max
// +0x144..0x158, and the Zone optional-visual switch +0x380.
struct RoomZone {
    Address identity;
    float* dimensions[3];
    float* relative_minimum[3];
    float* relative_maximum[3];
    const std::uint8_t* optional_visual_enabled;
};

enum class Operation : std::uint32_t {
    set_position
};

struct Request {
    Operation operation;
    Address room_zone;
    float position[3];
    // Zone::InitWithBoundingBox passes true to GameObject::SetPosition.
    std::uint8_t update_position;
};

struct Services {
    void* context;
    // Zero is success. The owner implements source GameObject::SetPosition,
    // including any live physical/visual-object adapters it actually owns.
    std::int32_t (*invoke)(void*, const Request*);
};

enum class Status : std::int32_t {
    complete,
    invalid_argument,
    service_unavailable,
    service_failed,
    optional_visual_branch_unsupported
};

struct Result {
    std::uint32_t calls;
    float center[3];
};

// Bounded equivalent of the RoomZone path through
// Zone::InitWithBoundingBox (original ELF 0x397594). A RoomZone constructor
// sets +0x380 to zero, so the optional scene/debug branch is excluded.
Status initialize(RoomZone*, const Box3*, const Services*, Result*);

} // namespace dh2::module_room_zone_bounds
