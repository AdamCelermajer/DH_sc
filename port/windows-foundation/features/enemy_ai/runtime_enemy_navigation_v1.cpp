#include "runtime_enemy_navigation_v1.hpp"

#include "../../../level-world/navigation_heading.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

namespace dh::foundation::enemy_ai {
namespace {

bool fail(std::string& error, const char* message) {
    if (error.empty()) error = message;
    return false;
}

bool same_owner(const std::shared_ptr<void>& left, const std::shared_ptr<void>& right) {
    return !left.owner_before(right) && !right.owner_before(left);
}

bool same_owner(const std::weak_ptr<const void>& left,
                const std::weak_ptr<const void>& right) {
    return !left.owner_before(right) && !right.owner_before(left);
}

bool finite(Vec3 p) {
    return std::isfinite(p.x) && std::isfinite(p.y) && std::isfinite(p.z);
}

Vec3 subtract(Vec3 a, Vec3 b) { return {a.x-b.x,a.y-b.y,a.z-b.z}; }
Vec3 cross(Vec3 a, Vec3 b) {
    return {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};
}
double dot(Vec3 a, Vec3 b) {
    return double(a.x)*b.x+double(a.y)*b.y+double(a.z)*b.z;
}

// Two-sided Moller-Trumbore over the existing loaded _colbox_ triangles.
// Source/target endpoint contacts are excluded so an actor touching a wall
// does not occlude every ray that starts or ends on that wall.
bool segment_hits_interior(Vec3 from, Vec3 to, const CollisionTriangle& triangle) {
    const Vec3 direction = subtract(to, from);
    const Vec3 edge1 = subtract(triangle.b, triangle.a);
    const Vec3 edge2 = subtract(triangle.c, triangle.a);
    const Vec3 p = cross(direction, edge2);
    const double determinant = dot(edge1, p);
    if (std::abs(determinant) < 1e-10) return false;
    const double inverse = 1.0 / determinant;
    const Vec3 distance = subtract(from, triangle.a);
    const double u = dot(distance, p) * inverse;
    if (u < 0.0 || u > 1.0) return false;
    const Vec3 q = cross(distance, edge1);
    const double v = dot(direction, q) * inverse;
    if (v < 0.0 || u + v > 1.0) return false;
    const double along = dot(edge2, q) * inverse;
    constexpr double endpoint_epsilon = 1e-4;
    return along > endpoint_epsilon && along < 1.0-endpoint_epsilon;
}

bool same_position(const std::array<float,3>& left, const ActorState& right) {
    return left[0] == right.transform.position[0] &&
           left[1] == right.transform.position[1] &&
           left[2] == right.transform.position[2];
}

} // namespace

RuntimeEnemyNavigationV1::RuntimeEnemyNavigationV1(RuntimeEnemyNavigationConfigV1 config)
    : config_(std::move(config)) {}

void RuntimeEnemyNavigationV1::reset_for_session(const std::weak_ptr<const void>& lease) {
    if (!lease_initialized_ || !same_owner(session_lease_, lease)) {
        routes_.clear();
        locomotion_bound_.clear();
        session_lease_ = lease;
        lease_initialized_ = true;
    }
}

bool RuntimeEnemyNavigationV1::line_of_sight(Vec3 from, Vec3 to,
                                              bool& visible, std::string& error) {
    error.clear();
    if (!config_.scene_collision || !finite(from) || !finite(to))
        return fail(error, "Current loaded original scene collision and finite actor endpoints are required for LOS");
    const auto& scene = *config_.scene_collision;
    // A loaded collision scene with no floor or collision helper instances is
    // indistinguishable from an uninitialized object; fail closed there. A
    // complete scene with floors but no _colbox_ helpers is valid clear LOS.
    if (!scene.floorInstances && !scene.collisionInstances)
        return fail(error, "Current scene collision has no loaded authored helper geometry");
    ++report_.line_tests;
    visible = true;
    for (const auto& triangle : scene.triangles) {
        if (triangle.floor) continue;
        if (segment_hits_interior(from, to, triangle)) {
            visible = false;
            ++report_.line_blocked;
            break;
        }
    }
    return true;
}

bool RuntimeEnemyNavigationV1::validate_actor_geometry(
    ActorId id, const dh2::navigation::NavigationObject*& pf, std::string& error) {
    if (!config_.navigation_world || !config_.navigation_world->floors ||
        !config_.navigation_world->floors->sewn || !config_.bodies)
        return fail(error, "Enemy route requires the actual sewn SourceNavigationWorldStorage and body registry");
    pf = config_.bodies->navigation(id);
    if (!pf) return fail(error, "Enemy route requires the same actor's retained PFObject");
    OriginalActorNavigationWorldBindings retained;
    if (!config_.bodies->navigation_world_binding(id, retained, error)) return false;
    std::shared_ptr<void> expected_floor = config_.navigation_world->floors;
    std::shared_ptr<void> expected_obstacles = config_.navigation_world;
    if (retained.floors != config_.navigation_world->floors.get() ||
        retained.obstacles != &config_.navigation_world->registry ||
        !same_owner(retained.floor_lease, expected_floor) ||
        !same_owner(retained.obstacle_lease, expected_obstacles))
        return fail(error, "Enemy route storage differs from the exact floor/obstacle owners attached to this actor PFObject");
    ++report_.geometry_identity_checks;
    return true;
}

bool RuntimeEnemyNavigationV1::bind_locomotion(CombatSession& session, ActorId id,
                                               std::string& error) {
    if (locomotion_bound_[id]) return true;
    if (config_.walk_alias.empty() || config_.idle_alias.empty() ||
        !std::isfinite(config_.walk_rate) || config_.walk_rate <= 0.0 ||
        !std::isfinite(config_.idle_rate) || config_.idle_rate <= 0.0)
        return fail(error, "Explicit authored enemy walk/idle aliases and positive rates are required");
    if (!session.bind_actor_locomotion(id, config_.walk_alias, config_.walk_choice,
                                       config_.walk_rate, true, error)) return false;
    if (!session.bind_actor_locomotion(id, config_.idle_alias, config_.idle_choice,
                                       config_.idle_rate, true, error)) return false;
    locomotion_bound_[id] = true;
    return true;
}

bool RuntimeEnemyNavigationV1::select(CombatSession& session, ActorId id,
                                      const std::string& alias, std::string& error) {
    if (session.owns_pose(id)) return true;
    if (!bind_locomotion(session, id, error)) return false;
    return session.select_actor_locomotion(id, alias, error);
}

bool RuntimeEnemyNavigationV1::approach(CombatSession& session, ActorState& owner,
                                        ActorState& target, const dh2::data::AiProps&,
                                        std::string& error) {
    error.clear();
    reset_for_session(session.actor_binding_lease());
    if (&owner != session.actor(owner.id) || &target != session.actor(target.id) ||
        owner.target_id != target.id)
        return fail(error, "Enemy route must receive the same live actor records and canonical target id");
    if (session.owns_pose(owner.id)) return true;
    if (!config_.scene_collision ||
        (!config_.scene_collision->floorInstances && !config_.scene_collision->collisionInstances))
        return fail(error, "Enemy route requires the current loaded original scene collision geometry");
    const dh2::navigation::NavigationObject* pf = nullptr;
    if (!validate_actor_geometry(owner.id, pf, error)) return false;
    auto& world = *config_.navigation_world->floors;
    const std::uint64_t capacity64 = std::uint64_t(world.graph.node_count) + 1;
    if (!capacity64 || capacity64 > std::numeric_limits<std::uint32_t>::max())
        return fail(error, "Original route graph exceeds bounded path storage");

    auto& route = routes_[owner.id];
    const bool must_build = !route.initialized || route.target != target.id ||
                            !same_position(route.goal, target);
    if (must_build) {
        route = ActorRoute{};
        const auto capacity = static_cast<std::uint32_t>(capacity64);
        route.segments.resize(capacity);
        route.search_path.resize(capacity);
        route.path.route.flags = pf->motion.flags;
        route.path.route.radius = pf->radius;
        route.path.segments = route.segments.data();
        route.path.capacity = capacity;
        std::copy(owner.transform.position.begin(), owner.transform.position.end(), route.path.position);
        std::copy(target.transform.position.begin(), target.transform.position.end(), route.goal.begin());
        route.target = target.id;
        dh2::navigation::RouteResult result{};
        result.search.path = route.search_path.data();
        result.search.path_capacity = capacity;
        if (dh2::floors::find_path(world, route.path, target.transform.position.data(),
                                   std::numeric_limits<unsigned>::max(), result))
            return fail(error, "Shared sewn-floor path kernel rejected the actor route request");
        route.initialized = true;
        route.found = result.found != 0;
        ++report_.paths_built;
        if (!result.found) ++report_.paths_unreachable;
        else if (route.path.count) ++report_.routes_with_waypoints_built;
    }

    std::copy(owner.transform.position.begin(), owner.transform.position.end(), route.path.position);
    if (!route.found) return select(session, owner.id, config_.idle_alias, error);
    float destination[3]{};
    if (route.path.count) {
        dh2::navigation::MoveResult move{};
        if (dh2_nav_move_path(&move, &route.path, &world.graph))
            return fail(error, "Shared navigation MovePath kernel rejected the cached route");
        std::copy(move.target, move.target+3, destination);
    } else {
        // FindPath owns the validated final leg from the last graph waypoint
        // to the requested target. Continue following that exact result after
        // MovePath consumes its last segment; never synthesize a new chord.
        std::copy(route.path.target, route.path.target+3, destination);
    }
    float direction[]{destination[0]-owner.transform.position[0],
                      destination[1]-owner.transform.position[1],0.0f};
    const Vec3 current{owner.transform.position[0],owner.transform.position[1],owner.transform.position[2]};
    const Vec3 waypoint{destination[0],destination[1],destination[2]};
    for (const auto& triangle : config_.scene_collision->triangles) {
        if (triangle.floor || !segment_hits_interior(current, waypoint, triangle)) continue;
        ++report_.paths_blocked_by_collision;
        return select(session, owner.id, config_.idle_alias, error);
    }
    // Original Character construction stores validate_boundary=1 at +0x1c4.
    // Respect an explicitly retained source value when available; generic
    // same-session monster actors use that recovered Character default.
    if (owner.source_validate_boundary452.value_or(1) != 0) {
        const auto before = std::array<float,3>{direction[0],direction[1],direction[2]};
        dh2::navigation::DirectionRequest boundary{
            &world.collision_world, pf->motion.position, pf->radius,
            pf->motion.flags, 0};
        unsigned diagnostic_valid = 0;
        if (dh2_nav_validate_direction(&diagnostic_valid, direction, &boundary))
            return fail(error, "Original PFWorld ValidateDirection rejected same-actor floor geometry");
        ++report_.boundary_heading_checks;
        if (direction[0] != before[0] || direction[1] != before[1] || direction[2] != before[2])
            ++report_.boundary_heading_slides;
        // Source UpdatePath publishes its mutated direction even when the
        // diagnostic valid bit is false; preserve that behavior here.
        (void)diagnostic_valid;
    }
    if ((direction[0] == 0.0f && direction[1] == 0.0f) ||
        dh2_nav_look_towards(&owner.transform.rotation[2], direction))
        return select(session, owner.id, config_.idle_alias, error);
    if (!select(session, owner.id, config_.walk_alias, error)) return false;
    ++report_.waypoints_selected;
    return true;
}

bool RuntimeEnemyNavigationV1::stop(CombatSession& session, ActorState& owner,
                                    std::string& error) {
    error.clear();
    reset_for_session(session.actor_binding_lease());
    if (&owner != session.actor(owner.id))
        return fail(error, "Enemy stop must receive the same live session actor");
    routes_.erase(owner.id);
    if (session.owns_pose(owner.id)) return true;
    if (!select(session, owner.id, config_.idle_alias, error)) return false;
    ++report_.idle_selections;
    return true;
}

bool RuntimeEnemyNavigationV1::release_route(CombatSession& session, ActorId id,
                                             std::string& error) {
    error.clear();
    reset_for_session(session.actor_binding_lease());
    auto* world = session.world();
    auto* actor = session.actor(id);
    if (!world || !actor || world->find_actor(id) != actor)
        return fail(error, "Enemy route release requires a current same-Session actor");
    const auto route = routes_.find(id);
    if (route != routes_.end()) {
        routes_.erase(route);
        ++report_.routes_released;
    }
    // Original DropPath copies current PF position into its requested target
    // only when its source path list is nonempty. This cache release owns no
    // live PF fields, so that source-visible destination effect remains with
    // the caller's GameObject Stop/transition recipe.
    error.clear();
    return true;
}

std::shared_ptr<RuntimeEnemyNavigationV1> make_runtime_enemy_navigation_v1(
    RuntimeEnemyNavigationConfigV1 config) {
    return std::make_shared<RuntimeEnemyNavigationV1>(std::move(config));
}

RuntimeEnemyControllerServicesV1 make_runtime_enemy_navigation_services_v1(
    const std::shared_ptr<RuntimeEnemyNavigationV1>& navigation) {
    if (!navigation) return {};
    RuntimeEnemyControllerServicesV1 services;
    services.can_see = [navigation](CombatSession& session, const ActorState& from,
                                    const ActorState& to, const OriginalCombatProperties&,
                                    const OriginalCombatProperties&, const dh2::data::AiProps&,
                                    bool& visible, std::string& error) {
        if (&from != session.actor(from.id) || &to != session.actor(to.id))
            return fail(error, "Enemy LOS must query the same live session actor records");
        return navigation->line_of_sight(
            {from.transform.position[0],from.transform.position[1],from.transform.position[2]},
            {to.transform.position[0],to.transform.position[1],to.transform.position[2]},
            visible, error);
    };
    services.approach = [navigation](CombatSession& session, ActorState& from, ActorState& to,
                                     const dh2::data::AiProps& ai, std::string& error) {
        return navigation->approach(session, from, to, ai, error);
    };
    services.stop = [navigation](CombatSession& session, ActorState& actor, std::string& error) {
        return navigation->stop(session, actor, error);
    };
    return services;
}

} // namespace dh::foundation::enemy_ai
