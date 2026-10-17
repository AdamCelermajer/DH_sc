#pragma once

#include "crypt_module_bounds_registry_v1.hpp"
#include "crypt_room_zone_owner_v1.hpp"
#include "object_manager_runtime_owner_v1.hpp"

#include <cstdint>

namespace dh2::crypt_room_zone_manager_bridge_v1 {

enum class Status : std::uint32_t {
    complete,
    invalid_argument,
    module_registry_mismatch,
    room_zone_activation_failed,
    object_count_limit,
    object_manager_enumeration_failed,
    object_lists_initialization_failed,
    allocation_failure
};

struct Result {
    Status status;
    std::uint32_t object_count;
    object_manager_runtime_owner_v1::Status object_manager_status;
    crypt_room_zone_owner_v1::Status room_zone_status;
    crypt_room_zone_owner_v1::ActivationResult activation;
    crypt_room_zone_owner_v1::ObjectListResult object_lists;
};

// Compose the copied Crypt module bounds, stable RoomZone projections, and
// canonical ObjectManager GameObject projections for the source InitObjectList
// pass. Objects are enrolled in signed ObjectManager key order; zones are
// visited in the source module-vector order. The temporary views borrow the
// ObjectManager-owned fields and live only for this synchronous call.
//
// The caller must keep the ObjectManager entries stable until this call
// returns. Callback services must not remove entries while the source-style
// RoomZone scan is using their borrowed field views. This function never
// creates another object owner and never reads or changes ObjectManager's
// separate +0x88 no-room list.
Status initialize_object_lists(
    const crypt_module_bounds_registry_v1::Owner& module_bounds,
    crypt_room_zone_owner_v1::Owner& room_zones,
    object_manager_runtime_owner_v1::Owner& object_manager,
    const module_room_zone_bounds::Services* position_services,
    const room_zone_enrollment::Services* enrollment_services,
    Result*) noexcept;

const char* status_name(Status) noexcept;

} // namespace dh2::crypt_room_zone_manager_bridge_v1
