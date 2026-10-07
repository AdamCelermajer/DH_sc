#pragma once
#include "../scene-materials/scene.hpp"
#include <memory>
namespace dh2::camera {
struct AuthoredCameraV3 {
 std::string id,target_uri;
 std::uint32_t node{},target_node{},kind{};
 float horizontal_fov_or_mag{},aspect{},znear{},zfar{};
 // CCameraSceneNodeC1 constructs an instance child at(0,0,100); the
 // serialized camera-bearing node remains its separate authored parent.
 std::array<float,3> instance_position{0,0,100};
};
// One owning camera BRES/graph, not an auxiliary copied scene. SceneBinding
// and future source AnimSet controller must borrow this same graph.
class GameplayCameraSceneV3 {
 std::vector<std::uint8_t> bytes_;resources::BresView bres_{};
 scene::Scene scene_;std::vector<AuthoredCameraV3> cameras_;
 // Source CRootSceneNode wrapper is above serialized scene nodes. Moving the
 // wrapper must not overwrite an authored top-level node's local translation.
 std::array<float,3> root_position_{};
public:
 bool load(std::vector<std::uint8_t>,std::string&);
 const resources::BresView& bres()const noexcept{return bres_;}
 scene::Scene& graph()noexcept{return scene_;}
 const scene::Scene& graph()const noexcept{return scene_;}
 const std::vector<AuthoredCameraV3>& cameras()const noexcept{return cameras_;}
 bool select(const std::string& node_name,std::uint32_t& camera_index,std::string&)const;
 bool root_position(float[3],std::string&)const;
 bool set_root_position(const float[3],std::string&);
 bool set_camera_instance_position(std::uint32_t,const float[3],std::string&);
 bool eye_and_target(std::uint32_t,float eye[3],float target[3],std::string&)const;
 bool update_selected_absolute_v67(std::uint32_t,std::string&);
 bool camera_parent_position_v67(std::uint32_t,float parent[3],std::string&)const;
};
}
