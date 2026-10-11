#pragma once

#include "character_controller_commands.hpp"
#include "navigation_objects.hpp"
#include "navigation_heading.hpp"

#include <cstdint>
#include <string>

namespace dh2::character_runtime_factory_v1 { class Record; }

namespace dh2::character_faery_controller_v1 {

namespace factory = character_runtime_factory_v1;
using PointServices = character::ControllerPointCommandServices16;
using PointCommand = character::ControllerPointCommand;

// The controller component state is retained by the canonical factory Record.
// It holds no Character, coordinator, timer, VM, or physics body of its own.
struct State {
    std::uintptr_t character{};
    std::uintptr_t controller{};
    std::uintptr_t controllable{};
    std::uint32_t global_blocked{};
    std::uint32_t locked{};
    std::uint32_t forced{};
    // PFObject/heading state is retained on the same Character Record as its
    // controller. These are typed adapters consumed by the selected shared
    // navigation kernels, not a second path or Character owner.
    navigation::NavigationObject navigation{};
    navigation::HeadingState heading{};
    float destination[3]{};
    bool navigation_ready{};
};

enum class Status : std::uint8_t {
    complete,
    invalid_record,
    owner_mismatch,
    service_unavailable,
    command_failed,
};

struct Result {
    std::uint32_t command{};
    std::uint32_t source_return{};
    std::uintptr_t character{};
    std::uintptr_t controller{};
    std::uintptr_t controllable{};
    bool dispatched{};
};

// Bind one already-created source controller/controllable component pair to
// the existing factory Record. The supplied identities are borrowed component
// addresses; this does not allocate or own either component.
Status bind(factory::Record&, std::uintptr_t controller,
            std::uintptr_t controllable, std::string& error);

// Initializes this Record's PFObject from the shared source floor world and
// obstacle registry, using the same Character ProducerFields as player/NPC
// UpdatePFObject. `position`, radius and bounds are borrowed source-derived
// values. It never allocates a world, registry, path or second actor.
Status initialize_navigation(factory::Record&,
    const navigation::CollisionWorld&, navigation::ObstacleRegistry&,
    const float position[3], float physical_radius,
    const float absolute_bounds_xyxy[4], std::string& error);

// Applies Level placement directly to the same Character PFObject and moves
// its obstacle parent when height resolution changes floors. This deliberately
// does not run WarpTo/ValidatePosition policy.
Status synchronize_placement(factory::Record&,
    const navigation::CollisionWorld&, navigation::ObstacleRegistry&,
    const float position[3], std::string& error);

// Source WarpTo validates through the existing floor/room/PFObject kernel and
// moves the obstacle's registered parent transactionally. The accepted
// position is returned for the caller's same-Record GameObject/body sync.
Status validate_warp(factory::Record&, const navigation::CollisionWorld&,
    navigation::ObstacleRegistry&, const navigation::MotionPolicy&,
    const float point[3], float accepted[3],
    navigation::PositionResult*, std::string& error);

// Removes this Record's obstacle from the shared Level registry before the
// Character Record is retired. The Level registry/world remain caller-owned.
Status retire_navigation(factory::Record&,
    const navigation::CollisionWorld&, navigation::ObstacleRegistry&,
    std::string& error);

// Dispatches WarpTo/HeadTowards/HeadTo through the source v2Controller gate,
// then synchronously calls the already-bound Character controllable provider.
// Position, path, scene, and body effects remain that provider's live owners.
Status point_command(factory::Record&, PointCommand, const float point[3],
                     std::uint32_t global_blocked, std::uint32_t locked,
                     std::uint32_t forced, const PointServices&,
                     Result*, std::string& error);

void retire(State&) noexcept;

} // namespace dh2::character_faery_controller_v1
