#pragma once
#include "gameplay_camera_load_v19.hpp"
#include "gameplay_camera_layout_v14.hpp"
#include "gameplay_camera_picking_v20.hpp"
#include "gameplay_camera_gpu_projection_v12.hpp"
namespace dh2::camera {
struct CameraApplicationBindingsV23 {
 std::shared_ptr<application::ApplicationServicesOwnerV5> application;
 std::shared_ptr<world::GameObjectSceneRootRegistryV1> actual_roots;
 std::shared_ptr<void> actual_dictionary_lease,actual_file_system;
 const data::Dictionary* actual_dictionary{};
 std::uintptr_t actual_cursor{};
 CameraFactoryBackendV16 backend;
 CameraZoomServicesV17 zoom;
 std::function<bool(std::uint8_t&,std::string&)> actual_lg_devices;
 std::function<bool(const std::string&,std::vector<std::uint8_t>&,std::string&)> read;
 std::function<bool(std::string&)> debug_after_add;
};
struct CameraWorldBindingsV23 {
 std::shared_ptr<void> world_lease;
 std::shared_ptr<const void> actual_tables_lease;
 const data::AnimationTables* actual_tables{};
 data::DesignSettingsOwner::Borrow design;
 LevelServicesV4 actors;
 CameraLayoutServicesV14 layout;
 CameraOverviewServicesV18 overview;
 std::function<bool(std::uintptr_t&,std::string&)> local_character0;
 std::function<bool(const std::string&,std::string&)> positive_skybox;
 // Real source Level12c/128 stores happen after each corresponding allocation
 // and constructor, before later Load/Init callbacks can fail.
 std::function<bool(std::uintptr_t,std::string&)> publish_overview12c,publish_level128;
};
struct CameraWorldSessionV23 {
 CameraWorldBindingsV23 bindings;
 std::unique_ptr<GameplayCameraLoadV19> camera;
};
// Native Application integration lease, explicitly not an invented recovered
// Application offset. Root's one Application slot retains this across GL/World.
// Context has weak App; each current V19 World pins actual App while alive.
class GameplayCameraApplicationV23 {
 std::weak_ptr<application::ApplicationServicesOwnerV5> application_;
 std::shared_ptr<events::EventManagerOwnerV12> events14_;
 std::shared_ptr<void> dictionary_lease_,file_system_;
 const data::Dictionary* dictionary_{};
 std::shared_ptr<world::GameObjectSceneRootRegistryV1> roots_;
 CameraFactoryBackendV16 backend_;
 std::function<bool(const std::string&,std::vector<std::uint8_t>&,std::string&)> resource_read_;
 std::shared_ptr<CameraDefaultFactoryV16> factory_;
 std::shared_ptr<GameplayCameraAnimationManagerV10> animations_;
 std::shared_ptr<GameplayCameraZoomV17> zoom_;
 // Strong session belongs to the actual World integration field, never the
 // App-owned context (session's V19 pins App). This avoids App->World->App.
 std::weak_ptr<CameraWorldSessionV23> world_;
 bool attempted_{},ready_{};
public:
 bool initialize(CameraApplicationBindingsV23,std::string&);
 bool load(const CameraLevelConfigV19&,CameraWorldBindingsV23,std::shared_ptr<CameraWorldSessionV23>& actual_world_slot,std::string&);
 bool update(std::string&);
 bool source_update_zoom_v67(std::string&);
 bool source_update_level_v67(std::string&);
 bool source_update_absolute_v67(std::string&);
 bool source_positions_v67(PointV2& camera,PointV2& parent,std::string&);
 bool scene_phase(std::uint32_t actual_scene_stamp,std::string&);
 bool view(CameraViewV11&,std::string&);
 bool picking(CameraPickingViewV20 actual_driver,GameplayCameraPickingV20&,std::string&);
 // Root's actual physical/PF flush belongs BETWEEN begin_release and
 // flush_animation_sets; actual Level event/App cleanup precedes release.
 bool begin_world_release(std::string&);
 void flush_animation_sets()noexcept;
 bool release_world_owners(std::string&);
 bool source_release_level128_v88(std::uintptr_t,std::string&);
 bool source_release_overview12c_v88(std::uintptr_t,std::string&);
 bool close_application(std::string&);
 bool loaded()const noexcept{auto w=world_.lock();return w&&w->camera&&w->camera->loaded();}
 bool ready()const noexcept{return ready_;}
 const std::shared_ptr<world::GameObjectSceneRootRegistryV1>& roots()const noexcept{return roots_;}
 const std::shared_ptr<GameplayCameraAnimationManagerV10>& animation_manager()const noexcept{return animations_;}
 const std::shared_ptr<GameplayCameraZoomV17>& zoom_handler50()const noexcept{return zoom_;}
 std::shared_ptr<CameraWorldSessionV23> world()const noexcept{return world_.lock();}
};
}
