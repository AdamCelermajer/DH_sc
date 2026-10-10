#pragma once
#include "../../playable_actor_bodies.hpp"

namespace dh::foundation::features {
struct SourcePathCommandBindings {
    std::shared_ptr<void> floor_lease;
    dh2::floors::World* floors{};
    OriginalActorPathBindings* controller{};
    dh2::navigation::RouteResult* result{};
    // Source Debug/module search producer, invoked AFTER DropPath and target
    // publication by the recovered FindPath kernel. Never default to enabled.
    std::function<bool(std::uint32_t&,std::string&)> search_enabled;
    // Host executes its actual admitted command publication; this feature
    // does not manufacture state/FSM/SetDestination policies.
    std::function<bool(const dh2::navigation::RouteResult&,std::string&)> publish_command;
    std::uint32_t source_search_limit{};
};
// No navigation world/controller/PFObject is allocated. Caller storage must
// outlive the path and SAME immutable graph; reached failures retain source
// FindPath prefix mutations rather than claiming transactional command success.
bool find_source_destination(PlayableActorBodies&,ActorId,Vec3,
                             SourcePathCommandBindings&,std::string&);
// Explicit borrowed receiver entry for source/native fixtures and hosts that
// already own that exact PFObject. Never constructs or defaults a new object.
bool find_source_destination_for_pf(const dh2::navigation::NavigationObject&,Vec3,
                                    SourcePathCommandBindings&,std::string&);
bool update_source_chase(PlayableActorBodies&,ActorId,OriginalActorPathBindings&,
                         dh2::navigation::ControllerResult&,std::string&);

struct SourceDecorObstacleBinding {
    std::shared_ptr<void> owner_lease,floor_lease,obstacle_lease;
    std::uintptr_t identity{};
    dh2::navigation::NavigationObject* actual_pf{};
    dh2::floors::World* floors{};
    dh2::navigation::ObstacleRegistry* obstacles{};
    const dh2::physical::NativeBody* actual_physical{};
    const float* absolute12c{}; // six SAME live source GameObject bounds
    dh2::navigation::ProducerClass source_class=dh2::navigation::ProducerClass::game_object;
};
// Original generic Decor is not a PF obstacle (ProducerClass::game_object).
// Actual PODecor bodies collide through NativeWorld independently. Containers,
// liftables and traps use their proved concrete producer class supplied here.
// This bridge does not infer class from a mesh name or allocate physical bodies.
bool update_source_decor_obstacle(const SourceDecorObstacleBinding&,std::string&);
}
