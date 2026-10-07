#pragma once
#include "npc_script_commands_v1.hpp"
#include "character_world_runtime_v1.hpp"
namespace dh2::character {
struct NpcActorFrameBorrowV1 {
 std::uintptr_t identity{};actor::RuntimeState* runtime{};
 State* state{};const std::int32_t* resolved224{};
 float* actual_position{};
 NpcScriptCommandsV1* commands{};skills::CharacterWorldRuntimeV1* world{};
 visual::SceneBinding* visual{};scene::Scene* scene{};physical::NativeBody* body{};
 navigation::ObstacleRegistry* registry{};
 const navigation::MotionPolicy* motion_policy{};
 navigation::ControllerWorkspace* workspace{};
 const navigation::AvoidanceScene* avoidance{};
 // Actual source GameObject+2e0, not inferred from visible graphics.
 const std::uintptr_t* auxiliary2e0{};
 std::uint32_t visual_update_source{},visual_apply_rotation_source{};
};
struct NpcActorFrameServicesV1 {
 void* context{};
 bool(*scale120)(void*,float*,std::string&){};
 // Source changed-scale CalcMeshBox→ApplyMeshBox over SAME retained scene.
 bool(*changed_bounds)(void*,std::string&){};
 // Reached actual target-node/cache position; null target produces nullptr.
 bool(*target_absolute)(void*,const float*&,std::string&){};
 // Only actual camera/auxiliary and nonbase visual endpoints, if reached.
 std::uint32_t(*remaining)(void*,std::uint32_t,float*){};
};
// Source GameObject actor phase only, after genuine Scene/FSM/Animator update:
// path→rotation→subobjects→target cache. Does not perform AI, global physics
// Step, profiling counter, idle audio or RequireOnlineUpdate outer tails.
// Caller publishes same raw position into its retained session after return,
// including reached-prefix failure. No second runtime/registry/floor exists.
int npc_actor_frame_v1(actor::RuntimeResult&,NpcActorFrameBorrowV1,
 NpcActorFrameServicesV1,std::uint32_t dt,std::string&);
}
