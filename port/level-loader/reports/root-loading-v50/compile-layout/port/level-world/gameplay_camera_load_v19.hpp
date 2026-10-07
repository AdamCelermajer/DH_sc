#pragma once
#include "application_services_owner_v5.hpp"
#include "gameplay_camera_scene_membership_v15.hpp"
#include "gameplay_camera_overview_v18.hpp"
#include "gameplay_camera_zoom_v17.hpp"
namespace dh2::camera {
struct CameraLevelConfigV19 {CameraLoadV11 camera;std::string skybox234;};
struct CameraLoadServicesV19 {
 std::shared_ptr<application::ApplicationServicesOwnerV5> application;
 std::shared_ptr<world::GameObjectSceneRootRegistryV1> manager;
 std::shared_ptr<CameraDefaultFactoryV16> actual_first_factory;
 std::shared_ptr<GameplayCameraZoomV17> actual_zoom_handler50;
 CameraFactoryBackendV16 backend;
 CameraRuntimeServicesV11 runtime;
 CameraOverviewServicesV18 overview;
 // Source GetLocalPlayer(0,true)->Character660, may legally be NULL only
 // when a real existing PlayerInfo currently has no Character.
 std::function<bool(std::uintptr_t&,std::string&)> local_character0;
 // Whole SceneManager.AddSkyBoxSceneNode(file,NULL), required only when
 // the actual LevelConfig skybox234 is non-empty (source359a38 leaf).
 std::function<bool(const std::string&,std::string&)> skybox;
};
class GameplayCameraLoadV19 {
 CameraLoadServicesV19 services_;
 std::shared_ptr<GameplayCameraOverviewV18> overview12c_;
 std::shared_ptr<GameplayCameraRuntimeV11> level128_;
 std::shared_ptr<GameplayCameraSceneMembershipV15> membership_;
 std::shared_ptr<CameraNodeLifetimeV15> camera_node_;
 std::string configured_node_;
 bool attempted_{},loaded_{},base_registered_{};
 CameraRuntimeServicesV11 runtime_services();
public:
 explicit GameplayCameraLoadV19(CameraLoadServicesV19 s):services_(std::move(s)){}
 bool load(const CameraLevelConfigV19&,std::string&);
 bool update(std::string&);
 bool scene_phase(std::uint32_t,std::string&);
 bool view(CameraViewV11&,std::string&);
 // Camera slice of source LevelD1 order: Zoom NULL, actual roots removed,
 // manager active camera NULL, AnimSetManager Flush. Root owns the physical,
 // PF, event/quest/body flush stages between these source phases.
 bool source_unbind_zoom_v19(std::string&);
 bool source_clear_camera_roots_v19(std::string&);
 bool source_clear_scene_active_v19(std::string&);
 void source_flush_animation_sets_v19()noexcept;
 // Explicit native ownership repair after those phases: original captured
 // LevelD1 has no explicit delete of128/12c. Preserves destructor order and
 // removes only these same camera owners, not other World roots.
 bool release_native_owners_v19(std::string&);
 const std::shared_ptr<GameplayCameraRuntimeV11>& level()const noexcept{return level128_;}
 const std::shared_ptr<GameplayCameraOverviewV18>& overview()const noexcept{return overview12c_;}
 bool loaded()const noexcept{return loaded_;}
};
}
