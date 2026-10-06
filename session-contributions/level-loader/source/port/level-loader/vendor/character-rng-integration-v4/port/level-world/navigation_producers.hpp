#pragma once
#include "navigation_objects.hpp"

namespace dh2::navigation {
enum class ProducerClass : std::uint32_t {
 game_object=0, character=1, container=2, liftable=3, trigger_trap=4
};
struct ObstacleTraits {
 std::uint32_t obstacle;float radius,strength;std::uint32_t reserved;
};
// Fields read by GameObject::UpdatePFObject. The body radius is in physical
// units; bounds are the existing GameObject min/max XY values, not recomputed
// model bounds. Body construction and bounds production are separate services.
struct ProducerFields {
 ProducerClass type;std::uint32_t physical_present;float physical_radius;
 std::uint32_t reserved;float minimum[2],maximum[2];
};
struct ProducerRequest {
 const CollisionWorld* geometry;ObstacleRegistry* registry;
 NavigationObject* object;std::uint64_t key;const ProducerFields* fields;
};
static_assert(sizeof(ObstacleTraits)==16&&sizeof(ProducerFields)==32&&sizeof(ProducerRequest)==40);
}
extern "C" {
// Recovered concrete IsObstacle/GetObstacleRadius/GetObstacleStrength values.
// 0 success, 1 malformed input; output preserved on failure.
int dh2_nav_producer_traits(dh2::navigation::ObstacleTraits*,std::uint32_t type);
// Original UpdatePFObject order: null user early return, concrete obstacle
// dispatch and InitObstacle, then physical radius * 100 or half max XY extent.
// 0 completed, 1 malformed caller, 2 bounded registry capacity failure.
// Caller owns distinct object/registry/field storage. Finite, nonnegative
// resulting radii are the native caller contract. Failure is atomic.
int dh2_nav_update_game_object(const dh2::navigation::ProducerRequest*);
}
