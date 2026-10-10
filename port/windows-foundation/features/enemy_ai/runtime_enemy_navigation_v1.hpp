#pragma once

#include "runtime_enemy_controller_v1.hpp"
#include "../../collision_scene.hpp"
#include "../../playable_actor_bodies.hpp"
#include "../../source_navigation_world_storage.hpp"

namespace dh::foundation::enemy_ai {

// Caller pins the session's already loaded static collision scene, the same
// SourceNavigationWorldStorage passed to PlayableActorBodies initialization,
// and the body registry for at least as long as the returned services live.
struct RuntimeEnemyNavigationConfigV1 {
    std::shared_ptr<SourceNavigationWorldStorage> navigation_world;
    PlayableActorBodies* bodies = nullptr;
    const CollisionScene* scene_collision = nullptr;
    std::string walk_alias;
    CombatSessionChoice walk_choice;
    double walk_rate = 1.0;
    std::string idle_alias;
    CombatSessionChoice idle_choice;
    double idle_rate = 1.0;
};

struct RuntimeEnemyNavigationReportV1 {
    std::size_t line_tests = 0;
    std::size_t line_blocked = 0;
    std::size_t paths_built = 0;
    std::size_t routes_with_waypoints_built = 0;
    std::size_t paths_unreachable = 0;
    std::size_t paths_blocked_by_collision = 0;
    std::size_t waypoints_selected = 0;
    std::size_t idle_selections = 0;
    std::size_t routes_released = 0;
    std::size_t geometry_identity_checks = 0;
    std::size_t boundary_heading_checks = 0;
    std::size_t boundary_heading_slides = 0;
};

// Route state is per stable ActorId, while ActorState, PFObject, floor world,
// shared animation clock and root-motion admission remain their existing sole
// owners. This adapter computes a route/heading and selects supplied authored
// locomotion; it never writes actor position or advances animation.
class RuntimeEnemyNavigationV1 final {
public:
    explicit RuntimeEnemyNavigationV1(RuntimeEnemyNavigationConfigV1);
    const RuntimeEnemyNavigationReportV1& report() const noexcept { return report_; }
    void clear_report() noexcept { report_ = {}; }

    bool line_of_sight(Vec3 from, Vec3 to, bool& visible, std::string& error);
    bool approach(CombatSession&, ActorState&, ActorState&,
                  const dh2::data::AiProps&, std::string& error);
    // Releases this adapter's cached route for a source Stop/Blur transition.
    // It does not select locomotion or implement the source PF/physical Stop.
    bool release_route(CombatSession&, ActorId, std::string& error);
    bool stop(CombatSession&, ActorState&, std::string& error);

private:
    struct ActorRoute {
        std::vector<dh2::navigation::PathSegment> segments;
        std::vector<std::uint32_t> search_path;
        dh2::navigation::PathObject path{};
        ActorId target = invalid_actor_id;
        std::array<float,3> goal{};
        bool initialized = false;
        bool found = false;
    };
    bool validate_actor_geometry(ActorId, const dh2::navigation::NavigationObject*&,
                                 std::string& error);
    bool bind_locomotion(CombatSession&, ActorId, std::string& error);
    bool select(CombatSession&, ActorId, const std::string&, std::string& error);
    void reset_for_session(const std::weak_ptr<const void>&);

    RuntimeEnemyNavigationConfigV1 config_;
    std::map<ActorId, ActorRoute> routes_;
    std::map<ActorId, bool> locomotion_bound_;
    std::weak_ptr<const void> session_lease_;
    bool lease_initialized_ = false;
    RuntimeEnemyNavigationReportV1 report_;
};

std::shared_ptr<RuntimeEnemyNavigationV1> make_runtime_enemy_navigation_v1(
    RuntimeEnemyNavigationConfigV1);
RuntimeEnemyControllerServicesV1 make_runtime_enemy_navigation_services_v1(
    const std::shared_ptr<RuntimeEnemyNavigationV1>&);

} // namespace dh::foundation::enemy_ai
