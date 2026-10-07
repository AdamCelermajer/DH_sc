#pragma once
#include "gameplay_camera_animator_v10.hpp"
#include "gameplay_camera_level_v4.hpp"
#include "gameplay_camera_design_v5.hpp"
#include "gameplay_camera_matrix_v8.hpp"
namespace dh2::camera {
struct CameraLoadV11 {
 std::string file,animation_set,node;
 std::int32_t near_plane{},far_plane{};
};
struct CameraViewV11 {
 MatrixV8 view,projection;
 PointV2 eye{},target{},up{0,0,1};
 float fov{},aspect{},near_plane{},far_plane{};
};
struct CameraRuntimeServicesV11 {
 std::shared_ptr<void> world_lease;
 const data::AnimationTables* tables{};
 data::DesignSettingsOwner::Borrow design;
 std::shared_ptr<GameplayCameraAnimationManagerV10> manager;
 std::function<bool(const std::string&,std::vector<std::uint8_t>&,std::string&)> read;
 // Register the SAME retained graph/animator in the actual SceneManager.
 // Caller must preserve the registered prefix if a callback returns failure.
 std::function<bool(std::shared_ptr<GameplayCameraSceneV3>,std::string&)> attach_graph;
 std::function<bool(std::shared_ptr<GameplayCameraSceneV3>,std::shared_ptr<GameplayCameraAnimatorV10>,std::string&)> attach_animator;
 std::function<bool(std::shared_ptr<GameplayCameraSceneV3>,std::shared_ptr<GameplayCameraAnimatorV10>,std::string&)> detach;
 // These bind the process CameraBase active slot and actual SceneManager
 // active-camera selection; no independent active slot is owned here.
 std::function<bool(std::uintptr_t,std::shared_ptr<GameplayCameraSceneV3>,std::uint32_t,std::string&)> activate;
 std::function<bool(std::uintptr_t,bool&,std::string&)> is_active;
 std::function<bool(std::uintptr_t,std::string&)> release_active;
 LevelServicesV4 actor_services;
 // Additive source selection phase: CameraLevel grabs the selected camera
 // before AnimSetManager.Create. Historical standalone clients may omit it.
 std::function<bool(std::shared_ptr<GameplayCameraSceneV3>,std::uint32_t,std::string&)> retain_selected_v19;
};
// Concrete source camera resource/registration/controller/frame composition.
// It owns one graph, animator and CameraLevel fields. Required World/scene/
// Application services remain borrowed from their sole production owners.
class GameplayCameraRuntimeV11 {
 CameraRuntimeServicesV11 services_;
 CameraLoadV11 input_;
 std::shared_ptr<GameplayCameraSceneV3> scene_;
 std::shared_ptr<GameplayCameraAnimatorV10> animator_;
 std::unique_ptr<GameplayCameraLevelV4> level_;
 std::int32_t set_id_{-1},row_{-1};std::uint32_t camera_{};
 CameraViewV11 view_;
 bool attach_attempted_{},attached_{},loaded_{},closed_{};
public:
 explicit GameplayCameraRuntimeV11(CameraRuntimeServicesV11 s):services_(std::move(s)){}
 ~GameplayCameraRuntimeV11(){if(animator_)animator_->set_source_completion({});}
 // Does not substitute source LevelConfig defaults or a current player.
 bool load(const CameraLoadV11&,std::string&);
 bool activate(std::string&);
 bool play_idle(std::string&);
 bool set_target(std::uintptr_t,std::int32_t,std::string&);
 bool update(std::string&);
 bool scene_phase(std::uint32_t source_timestamp,std::string&);
 bool source_update_absolute_v67(std::string&);
 bool source_positions_v67(PointV2& camera,PointV2& parent,std::string&);
 std::uint32_t selected_camera_index_v67()const noexcept{return camera_;}
 bool view(CameraViewV11&,std::string&);
 // Read source CameraBase node8 absolute transform columns +10/+20 on the
 // same selected instance; this does not derive a basis from eye/target.
 bool source_node_vectors_v67(PointV2&look,PointV2&up,std::string&)const;
 // Specialized actual camera onChangedSceneManager writes the SAME data
 // owner. Level.SetData still overrides it later during source load.
 void source_aspect_changed_v18(float aspect)noexcept{view_.aspect=aspect;}
 // Explicit teardown is required while actual SceneManager is alive. Failed
 // detach retains owners; no destructor invokes opaque callbacks or fakes success.
 bool close(std::string&);
 GameplayCameraLevelV4* level()const noexcept{return level_.get();}
 const std::shared_ptr<GameplayCameraSceneV3>& scene()const noexcept{return scene_;}
 const std::shared_ptr<GameplayCameraAnimatorV10>& animator()const noexcept{return animator_;}
 //SAME CameraLevel80 CamAnimSetTable row produced by actual Load.
 std::int32_t source_animation_row80_v115()const noexcept{return row_;}
 std::int32_t set_id()const noexcept{return set_id_;}
 bool loaded()const noexcept{return loaded_;}
};
}
