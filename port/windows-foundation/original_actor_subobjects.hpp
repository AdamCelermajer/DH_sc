#pragma once
#include "actor_state.hpp"
#include "original_actor_physical.hpp"
#include "../level-world/subobjects_update.hpp"
#include "../level-world/floors.hpp"
#include "../level-world/navigation_path.hpp"
#include <optional>
namespace dh::foundation {
struct OriginalActorSubobjectsVisual {
    std::shared_ptr<void> receiver;
    std::uintptr_t identity{};
    float* position_ac{}; // Actual staged visual root storage, not actor copy.
    std::function<bool(std::string&)> update;
    std::function<bool(std::string&)> update_absolute;
    std::function<bool(ActorState&,std::string&)> apply_rotation;
    std::function<bool(const ActorState&,std::string&)> sync_rotation,sync_scaling;
};
struct OriginalActorSubobjectsInput {
    ActorState* actor{};
    OriginalActorSubobjectsBorrow owner;
    // SAME embedded PF and caller-owned actual path fields, no private path.
    std::shared_ptr<void> navigation_lease,path_lease;
    dh2::navigation::NavigationObject* pf{};
    const dh2::navigation::PathObject* path{};
    std::shared_ptr<void> floor_lease,obstacle_lease;
    dh2::floors::World* floors{};
    dh2::navigation::ObstacleRegistry* obstacles{};
    // Actual virtual getter facts; unavailable values fail when reached.
    std::optional<bool> validating_camera;
    std::optional<float> virtual_speed;
    std::function<bool(std::uintptr_t,OriginalActorSubobjectsVisual&,std::string&)> visual;
    // Whole SAME GameObject.SetPosition/NativeBody/aux/visual source setter.
    std::function<bool(const std::array<float,3>&,bool destination,std::string&)> set_position;
    std::function<bool(std::uintptr_t,std::string&)> auxiliary_update;
    std::function<bool(std::uintptr_t,dh2::subobjects::AuxiliaryBranchV69&,std::string&)> auxiliary_branch;
    // Actual camera services distinguish a source false/presence result from
    // unavailable delivery. Event/payload are the recovered source contracts.
    std::function<bool(dh2::subobjects::Event,float*,std::uint32_t&,std::string&)> camera;
};
struct OriginalActorSubobjectsResult {
    dh2::subobjects::Result source{};
    std::uint32_t failed_event=0,completed_events=0;
    bool completed=false;
};
// Concrete native/PF/visual adapter around the original SubObjects kernel.
// ActorState remains position/Euler authority; transport projections exist only
// for this synchronous invocation and publish/reload at source service edges.
bool update_original_actor_subobjects(const OriginalActorSubobjectsInput&,
    OriginalActorSubobjectsResult&,std::string& error);
}
