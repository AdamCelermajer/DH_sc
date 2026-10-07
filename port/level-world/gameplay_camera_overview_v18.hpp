#pragma once
#include "gameplay_camera_factory_v16.hpp"
#include "gameplay_camera_active_v13.hpp"
#include "event_manager_owner_v12.hpp"
namespace dh2::camera {
struct CameraKeyboardV18 {std::int32_t key{};bool pressed{};};
struct CameraGamepadV18 {
 float x{},x_min{},x_max{},y{},y_min{},y_max{};
 float zoom{},zoom_min{},zoom_max{},stop{},stop_min{},stop_max{};
};
struct CameraOverviewServicesV18 {
 std::shared_ptr<void> application;
 std::shared_ptr<events::EventManagerOwnerV12> events14;
 std::shared_ptr<CameraDefaultFactoryV16> actual_first_factory;
 std::shared_ptr<world::GameObjectSceneRootRegistryV1> manager;
 // Delivered empty Character660 is genuine; missing PlayerInfo is failure.
 std::function<bool(bool&,PointV2&,std::string&)> local_player0;
 // Actual Singleton<PFWorld>::s_inst bound fields1c/28, not model extents.
 std::function<bool(bool&,float&,float&,std::string&)> pf_z_bounds;
 std::function<bool(const events::EventBorrowV12&,CameraKeyboardV18&,std::string&)> keyboard;
 std::function<bool(bool&,CameraGamepadV18&,std::string&)> first_gamepad;
 const PointV2* source_vec3_zero{};
};
class GameplayCameraOverviewV18:public std::enable_shared_from_this<GameplayCameraOverviewV18> {
 CameraOverviewServicesV18 services_;
 std::shared_ptr<CameraProceduralNodeV16> node_;
 PointV2 base10_{};float x1c_{},y20_{},z24_{};
 bool retained_camera_{},initialized_{},event_attached_{};
 static bool receive(void*,const events::EventBorrowV12&,events::EventManagerOwnerV12&,std::int32_t&,std::string&);
public:
 explicit GameplayCameraOverviewV18(CameraOverviewServicesV18 s):services_(std::move(s)){}
 bool initialize(std::string&);
 bool activated(std::string&);
 bool deactivated(std::string&);
 bool on_event(const events::EventBorrowV12&,std::int32_t&,std::string&);
 bool update(std::string&);
 // Does not remove the node from SceneManager root; original Base D2 drops
 // its root+camera references, and the later actual scene clear owns removal.
 bool close_base(GameplayCameraActiveV13&,std::string&);
 CameraBaseBorrowV13 base_borrow();
 const std::shared_ptr<CameraProceduralNodeV16>& node()const noexcept{return node_;}
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
};
}
