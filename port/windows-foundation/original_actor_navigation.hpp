#pragma once
#include "original_actor_physical.hpp"
#include "../level-world/navigation_producers.hpp"
#include "../level-world/floors.hpp"
#include "../level-world/navigation_controller.hpp"
namespace dh::foundation {
struct OriginalSourceFloorBinding {
 unsigned instance{},room{};
 std::array<float,4> mesh_local_quaternion{};
 std::array<float,3> mesh_local_scale{};
};
// Actual source floor instances/child TRS selected by caller's module loader.
// Already placed Scene instance.world is authoritative. Does not infer rooms,
// static OptimizeStatic child TRS or select visual triangles as navigation.
 bool append_original_module_floors(const dh2::resources::BresView&,const dh2::scene::Scene&,
   const std::vector<OriginalSourceFloorBinding>&,dh2::floors::World&,std::string&);
struct OriginalActorNavigationBindings {
 std::shared_ptr<void> actor_lease,world_lease;
 std::uintptr_t identity{};
 const float* position160{};const float* relative144{};const float* absolute12c{};
 dh2::navigation::NavigationObject* object{};
 dh2::navigation::ObstacleRegistry* obstacles{};
 dh2::floors::World* floors{};
 std::function<bool(std::uint8_t&,std::string&)> static84;
};
struct OriginalActorNavigationWorldBindings {
 std::shared_ptr<void> floor_lease,obstacle_lease;
 dh2::floors::World* floors{};
 dh2::navigation::ObstacleRegistry* obstacles{};
};
struct OriginalActorNavigationMoveResult {
 Vec3 position{};
 bool direction_checked=false,direction_allowed=false,position_valid=false,grounded=false;
 std::uint32_t kind=0;
};
struct OriginalActorPathBindings {
 std::shared_ptr<void> controller_lease;
 dh2::navigation::PathController* controller{};
 dh2::navigation::PathObject* path{};
 dh2::navigation::ControllerWorkspace* workspace{};
 const dh2::navigation::AvoidanceScene* avoidance{};
 std::function<bool(dh2::navigation::ControllerPolicy&,std::string&)> source_policy;
 std::function<bool(std::uint32_t& skip,std::string&)> skip_boundary;
 std::function<bool(std::string&)> physical_stop;
 // Publish borrowed semantic controller fields to SAME host owner/motor.
 std::function<bool(std::string&)> publish;
};
struct OriginalManualHeadingBindings {
 std::shared_ptr<void> controller_lease;
 std::function<bool(dh2::navigation::HeadingState&,std::string&)> read_heading;
 std::function<bool(const dh2::navigation::HeadingState&,std::string&)> publish_heading;
 // Actual flags520/ValidateBoundary providers; not guessed from state IDs.
 std::function<bool(std::uint32_t& flags520,std::uint32_t& validate_boundary,std::string&)> source_policy;
 std::function<bool(std::uint32_t& skip,std::string&)> skip_boundary;
};
struct OriginalManualHeadingResult {
 dh2::navigation::HeadingState heading{};
 bool path_policy_enabled=false,boundary_checked=false;
 std::uint32_t direction_valid=0; // Source diagnostic, never admission veto.
};
// Borrows caller's sole PFObject and registry; no second navigation authority.
class OriginalActorNavigation {
public:
 explicit OriginalActorNavigation(OriginalActorNavigationBindings);
 bool construct_defaults(std::string&);
 bool attach_world(OriginalActorNavigationWorldBindings,const float* relative144,
                   const float* absolute12c,std::string&);
 bool initialize_pf(std::string&);
 // Original UpdatePath uses this on heading, then always reapplies heading;
 // returned bool is diagnostic, not a per-frame root displacement veto.
 bool validate_direction(Vec3 current_position,Vec3 direction,Vec3& admitted,
                         bool& allowed,std::string&);
 bool validate_position(Vec3 requested,OriginalActorNavigationMoveResult&,std::string&);
 bool update_path(OriginalActorPathBindings&,dh2::navigation::ControllerResult&,std::string&);
 // Explicit no-path input adapter: caller already ran real command admission.
 // Preserves source PF-copy/active-bit/Debug/boundary/final-heading order only.
 bool update_manual_heading(const OriginalManualHeadingBindings&,OriginalManualHeadingResult&,std::string&);
 bool update_pf(std::uintptr_t,const dh2::physical::NativeBody&,
                const dh2::physical::CharacterBodyConfig&,std::string&);
 // Appropriate directly as OriginalActorPhysicalBindings.update_pf callback.
 std::function<bool(std::uintptr_t,const dh2::physical::NativeBody&,
                    const dh2::physical::CharacterBodyConfig&,std::string&)> physical_callback();
private:
 OriginalActorNavigationBindings bindings_;
 bool constructed_=false,initialized_=false;
 OriginalActorNavigationWorldBindings world_backing_;
};
}
