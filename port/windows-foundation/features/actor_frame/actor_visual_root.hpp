#pragma once
#include "../../original_character.hpp"
#include "../../original_actor_motion_frame.hpp"
#include "../../original_actor_subobjects.hpp"
#include "../../../level-world/visual_motion.hpp"
#include "../../../level-world/level_can_move_towards_v108.hpp"
#include "../../../level-world/player_manager_owner_v1.hpp"
namespace dh::foundation::features {
struct ActorVisualRootBindings {
    std::shared_ptr<void> actor_lease,model_lease;
    ActorState* actor{};
    CharacterVisual* model{};
    const std::uintptr_t* visual2d8{};
    std::uintptr_t identity{};
    // Actual initial Visual root state. Scale is source root scale (not a
    // guessed percent conversion); owner Euler cells use source radians.
    dh2::visual::Root initial_root{};
    std::uint32_t update_endpoint{},apply_rotation_endpoint{};
    std::function<bool(std::string&)> update_override;
    std::function<bool(ActorState&,std::string&)> apply_rotation_override;
    // SAME owner+120 source scaling getter and changed-scale mesh-box tail.
    std::function<bool(std::array<float,3>&,std::string&)> scale120;
    std::function<bool(std::string&)> changed_mesh_bounds;
};
// Only a visual receiver/root. Actor position remains separate until the
// source SubObjects ApplyPosition callback. No actor/FSM/PF/path is allocated.
class ActorVisualRoot : public std::enable_shared_from_this<ActorVisualRoot> {
    ActorVisualRootBindings binding_;
    dh2::visual::Root root_{};
    Mat4 placement_{};
    bool bound_=false;
    bool validate(std::string&)const;
public:
    bool bind(ActorVisualRootBindings,std::string&);
    // Earlier Scene/animator phase has already delivered its authored events.
    // Caller supplies real per-slot deltas after source MoveGO/rate semantics.
    bool stage_displacement(const std::vector<Vec3>& raw_deltas,
                            Vec3 sampled_root,bool& moved,std::string&);
    bool sync_position(std::string&);
    bool sync_rotation(std::string&);
    bool sync_scaling(std::string&);
    bool update_absolute(std::string&);
    bool subobjects_visual(OriginalActorSubobjectsVisual&,std::string&);
    const dh2::visual::Root& source_root()const noexcept{return root_;}
    const Mat4& render_placement()const noexcept{return placement_;}
    std::uintptr_t identity()const noexcept{return binding_.identity;}
    ActorState* actor()const noexcept{return binding_.actor;}
    // Actual renderer/queue consumes this root matrix, not ActorState XYZ.
    using Submit=std::function<bool(const Mesh&,const Mat4&,std::string&)>;
    bool submit(const Mat4& game_to_render,const Submit&,std::string&)const;
};
struct ActorFrameResult {
    OriginalActorMotionFrameResult motion;
    OriginalActorSubobjectsResult subobjects;
};
// Runs late motion prefix with actual caller path/rotation/target services,
// replacing only SubObjects with the concrete SAME root/native/PF adapter.
bool reconcile_actor_frame(const std::shared_ptr<ActorVisualRoot>&,
    const OriginalActorMotionFrameBorrow&,OriginalActorMotionFrameServices,
    OriginalActorSubobjectsInput,ActorFrameResult&,std::string&);
// Count comes from actual PlayerManagerOwner storage. Single-character source
// route returns before online/camera/cooperative viewport dependencies.
bool actor_frame_can_move_towards(
    const std::shared_ptr<dh2::player::PlayerManagerOwnerV1>&,
    dh2::world::LevelCanMoveServicesV108,const float* position,const float* heading,
    bool& accepted,std::string&);
}
