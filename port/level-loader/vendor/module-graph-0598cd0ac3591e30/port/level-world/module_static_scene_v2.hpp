#pragma once
#include "module_selected_scene_v2.hpp"
#include "decor_scene.hpp"
#include <algorithm>
namespace dh2::world {
struct ModuleStaticMeshFieldsV2 {
 std::array<float,3> position{};
 std::array<float,4> quaternion{0,0,0,1};
 std::array<float,3> scale{1,1,1};
 std::uint32_t flags{0x721};
};
// Native successor of the factory-created node graph used by static Module
// SetParent. Owns the selected graph, its source flags and cached transforms.
// After OptimizeStatic, local poses are world poses while caches remain the
// source authoritative draw/PF transforms. Calling scene::update_world on
// this graph would apply the parent transforms a second time.
class ModuleStaticSceneV2 {
 ModuleSelectedSceneV2 graph_;
 std::vector<std::uint32_t> flags_;
 std::vector<ModuleStaticMeshFieldsV2> mesh_fields_;
 std::array<float,16> root_relative_{};
 std::array<float,16> root_cached_{};
 std::array<float,3> root_position_{},root_scale_{};
 std::array<float,4> root_quaternion_{};
 std::array<float,6> root_box_{-1,-1,-1,1,1,1};
 std::array<float,6> root_absolute_box_{-1,-1,-1,1,1,1};
 std::vector<std::array<float,6>> node_boxes_;
 std::uint32_t root_flags_{0x60f};
 bool ready_{},optimized_{};
 std::vector<std::uint8_t> detached_meshes_,detached_nodes_;
 std::vector<std::uint8_t> authored_node_visibility_,authored_mesh_visibility_;
 void update_absolute(unsigned);
 void optimize(unsigned);
public:
 bool initialize(ModuleSelectedSceneV2&&,const physical::ObjectVisualTransformV1&,std::string&);
 bool optimize_static(std::string&);
 // _LoadNavMesh hides and removes the ORIGINAL mesh after cloning. These
 // methods mutate this retained factory graph, never its immutable BRES.
 bool remove_floor_mesh(unsigned,std::string&);
 bool remove_node(unsigned,std::string&);
 bool notify_root_visibility(bool,std::string&);
 // Source setPosition followed by Root::updateAbsolutePosition(false).
 // Rebuilds root cache only; optimized child caches are left untouched.
 bool sync_root_position(const float*,std::string&);
 void store_root_rotation(const std::array<float,4>& q)noexcept{root_quaternion_=q;root_flags_|=4u;}
 void store_root_scale(const float* scale)noexcept{std::copy_n(scale,3,root_scale_.data());root_flags_|=2u;}
 const std::array<float,4>& root_quaternion()const noexcept{return root_quaternion_;}
 const std::array<float,3>& root_scale()const noexcept{return root_scale_;}
 bool mesh_attached(unsigned i)const noexcept{return i<detached_meshes_.size()&&!detached_meshes_[i];}
 bool node_attached(unsigned i)const noexcept{return i<detached_nodes_.size()&&!detached_nodes_[i];}
 bool source_node_relative(unsigned,std::array<float,16>&,std::string&);
 bool source_mesh_relative(unsigned,std::array<float,16>&,std::string&);
 bool source_root_relative(std::array<float,16>&,std::string&);
 std::array<float,6>& source_node_box(unsigned i){return node_boxes_.at(i);}
 std::array<float,6>& source_root_box()noexcept{return root_box_;}
 const std::array<float,16>& root_cached()const noexcept{return root_cached_;}
 const std::array<float,3>& root_position()const noexcept{return root_position_;}
 std::uint32_t& source_root_flags()noexcept{return root_flags_;}
 std::array<float,6>& source_root_absolute_box()noexcept{return root_absolute_box_;}
 const ModuleSelectedSceneV2& selected()const noexcept{return graph_;}
 const std::vector<std::uint32_t>& node_flags()const noexcept{return flags_;}
 const std::vector<ModuleStaticMeshFieldsV2>& mesh_fields()const noexcept{return mesh_fields_;}
 std::uint32_t root_flags()const noexcept{return root_flags_;}
 const std::array<float,16>& root_relative()const noexcept{return root_relative_;}
};
}
