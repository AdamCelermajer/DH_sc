#pragma once

#include "crypt_module_bounds_registry_v1.hpp"
#include "module_room_zone_bounds.hpp"
#include "room_zone_enrollment.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace dh2::crypt_room_zone_owner_v1 {

using Address = std::uintptr_t;

enum class Status : std::uint32_t {
    complete,
    invalid_argument,
    already_active,
    object_lists_already_initialized,
    bounds_initialization_failed,
    enrollment_failed,
    module_not_found,
    members_present,
    actor_not_tracked,
    reentrant_operation,
    allocation_failure,
    transition_failed
};

struct ActivationResult {
    std::uint32_t module_count;
    std::uint32_t initialized_count;
    std::uint32_t failed_module_index;
    module_room_zone_bounds::Status bounds_status;
};

// Read-only view. Its pointers remain valid until the owner is activated again,
// cleared, or destroyed; members may change after enrollment operations.
struct ZoneView {
    std::uint32_t module_index;
    Address identity;
    const char* module_name;
    const float* position;
    const float* dimensions;
    const float* relative_minimum;
    const float* relative_maximum;
    const float* absolute_minimum;
    const float* absolute_maximum;
    const Address* members;
    std::size_t member_count;
    std::uint8_t active_389;
    std::uint8_t dirty_388;
};

struct EnrollmentResult {
    std::uint32_t module_index;
    room_zone_enrollment::Result enrollment;
};

struct ObjectListResult {
    std::uint32_t zones_completed;
    std::uint32_t object_visits;
    std::uint32_t successful_add_calls;
    std::uint32_t failed_module_index;
    std::uint32_t failed_object_index;
};

enum class TransitionOperation : std::uint8_t {
    zone_entered,
    zone_exited,
    add_room_object,
    remove_room_object,
    query_module_visited,
    set_module_visited
};

struct TransitionRequest {
    TransitionOperation operation;
    std::uint32_t module_index;
    Address room_zone;
    Address actor;
};

using TransitionCallback = std::int32_t (*)(void*, const TransitionRequest*,
                                             std::uint32_t* value);

struct UpdateServices {
    void* context;
    TransitionCallback invoke;
};

struct UpdateResult {
    std::uint32_t zones_tested;
    std::uint32_t planes_tested;
    std::uint32_t zones_visible;
    std::uint32_t zones_activated;
    std::uint32_t zones_deactivated;
    std::uint32_t zones_visited;
    std::uint32_t failed_module_index;
    TransitionOperation failed_operation;
};

// Owns stable host-side zone projections for one active Crypt bounds registry.
// identity is an opaque stable token for adapter callbacks, not a native
// RoomZone allocation. A native integration must map it to its owned source
// object. The Owner must outlive all live GameObject room_zone references.
class Owner {
public:
    Owner();
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;
    Owner(Owner&&) = delete;
    Owner& operator=(Owner&&) = delete;
    ~Owner();

    // Builds a candidate in source module order and publishes it only after
    // every module_room_zone_bounds initialization succeeds. Existing zones
    // remain intact on failure. Replacement is refused while membership exists.
    Status activate(const crypt_module_bounds_registry_v1::Owner&,
                    const module_room_zone_bounds::Services*,
                    ActivationResult*) noexcept;

    // Calls the existing AddInitialObject kernel for the selected module.
    // All list operations are forwarded in source order. Successful append and
    // remove callbacks update this owner's canonical membership projection.
    Status add_initial_object(std::uint32_t module_index,
                              room_zone_enrollment::GameObject*,
                              const room_zone_enrollment::Services*,
                              EnrollmentResult*) noexcept;

    // Mirrors the nested source order: RoomZones in registry/module order,
    // then the caller's global object list in its given order. This is the
    // one-shot InitObjectList behavior; it never uses DACT room IDs.
    Status initialize_object_lists(room_zone_enrollment::GameObject* const* objects,
                                   std::size_t object_count,
                                   const room_zone_enrollment::Services*,
                                   ObjectListResult*) noexcept;

    // Mirrors RoomZone::Update's six-plane negative-vertex test and ordered
    // Activate/DeActivate callbacks. The plane words are [a,b,c,d] binary32;
    // Module visited bytes are queried and written through the transition
    // service; the RoomZone owner does not become a second visited-state store.
    Status update_frustum(const std::uint32_t planes[6][4],
                          const float* player_position,
                          const UpdateServices*, UpdateResult*) noexcept;

    // Source RoomZone::Update sets +0x388 when a zone needs reevaluation.
    Status mark_dirty(std::uint32_t module_index) noexcept;

    // Lifecycle notification only: call after source RoomZone::RemoveObject
    // has unlinked this actor (or after the actor has been destroyed). It does
    // not invoke source callbacks or change GameObject fields.
    Status forget_source_unlinked_actor(std::uint32_t module_index,
                                        Address actor) noexcept;

    // Refuses to release zone identities while any canonical member remains.
    Status clear() noexcept;

    std::size_t zone_count() const noexcept;
    bool zone_at(std::size_t source_order_index, ZoneView*) const noexcept;

private:
    struct Zone;
    struct PositionAdapter;
    struct EnrollmentAdapter;
    std::vector<std::unique_ptr<Zone>> zones_;
    bool operation_in_progress_ = false;

    Zone* find_module(std::uint32_t) noexcept;
    Zone* find_identity(Address) noexcept;
    bool has_members() const noexcept;
    Status enroll(Zone*, room_zone_enrollment::GameObject*,
                  const room_zone_enrollment::Services*, EnrollmentResult*) noexcept;
};

const char* status_name(Status) noexcept;

} // namespace dh2::crypt_room_zone_owner_v1
