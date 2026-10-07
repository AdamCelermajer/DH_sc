#pragma once
#include "world_item_graph_v3.hpp"
#include "item_frame_virtual_policy_v4.hpp"
#include "gameobject_scene_root_registry_v1.hpp"
#include "gameobject_online_update_v5.hpp"
namespace dh2::character {
struct WorldItemFrameServicesV5 {
 const navigation::CollisionWorld* geometry{};const navigation::Graph* paths{};
 navigation::ObstacleRegistry* obstacles{};const navigation::MotionPolicy* motion{};
 navigation::ControllerWorkspace* workspace{};const navigation::AvoidanceScene* avoidance{};
 subobjects::Services actual_world;
 std::shared_ptr<world::GameObjectSceneRootRegistryV1> roots;
 const actor::RuntimePolicy* actual_runtime_policy{};
 void* target_context{};
 bool(*target_node_position)(void*,std::uintptr_t,const float*&,std::string&){};
 std::function<bool(RetainedWorldItemObjectV1&,std::string&)> begin_update{},end_update{};
 std::function<bool(std::uintptr_t,std::string&)> collision_interact{};
 world::GameObjectOnlineUpdateServicesV5 online{};
 std::function<bool(RetainedWorldItemObjectV1&,std::int16_t,std::string&)> idle_sound{};
};
// Generic GameObject virtuals, actual Item speed and one current scene/body.
// No Character flags, FSM, property sheet or animation-root surrogate.
class WorldItemFrameV5 {
 WorldItemGraphV3& graph_;WorldItemFrameServicesV5 services_;
 std::uint32_t absolute_ms_{};std::string error_;
 static std::uint32_t service(void*,std::uint32_t,float*);
 static bool visual_world(void*,std::string&);
 static bool visual_rotation(void*,const float*,std::string&);
 bool stop(std::string&);
public:
 WorldItemFrameV5(WorldItemGraphV3& graph,WorldItemFrameServicesV5 s):graph_(graph),services_(std::move(s)){}
 void absolute_time(std::uint32_t source_ms)noexcept{absolute_ms_=source_ms;}
 void rebind(WorldItemFrameServicesV5 actual)noexcept{services_=std::move(actual);}
 bool route(const WorldItemRequestV1&,std::int32_t&,bool& handled,std::string&);
};
}
