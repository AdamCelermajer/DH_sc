#pragma once
#include <canonical_object_manager_v1.hpp>
#include <retained_scene_node_borrow_v109.hpp>
#include <canonical_level_context_v1.hpp>
#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <utility>
namespace dh2::world {class NativeBatchNodeV110;}
namespace dh2::loader {
//Actual selected ObjectHandle->GameObject receipt. A genuine NULL conversion
//is represented by empty identity; ObjectBase-only Config/Light is not cast.
struct BatchObjectBorrowV96 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};
 const std::uint8_t* deleted83{};const std::string* name30{};const std::string* archetype48{};
 std::uint8_t* linked2fc{};const std::uintptr_t* visual2d8{};
};
//SAME ISceneNode instance/name/ordered child-f4 membership. Methods borrow the
//actual current receiver, not a copied Scene/mesh/pose/inspection hierarchy.
struct BatchNodeBorrowV96 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};const std::string* name24{};
 std::function<bool(std::size_t&,std::string&)> child_count;
 std::function<bool(std::size_t,BatchNodeBorrowV96&,std::string&)> child;
 std::function<bool(bool,std::string&)> set_visible48;
 world::RetainedSceneNodeBorrowV109 source_v111;
};
struct BatchMeshBorrowV96 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};
 std::function<bool(std::string&)> grab,drop;
};
struct BatchRootBorrowV96 {
 std::shared_ptr<void> owner;std::uintptr_t identity{},mesh130{};
 BatchNodeBorrowV96 node;
 std::uint32_t* source_word138{};
 std::function<bool(std::uint32_t,std::string&)> set_automatic_culling;
 std::function<bool(std::string&)> drop; //Actual intrusive drop, genuine GPU/lifetime admission.
 std::shared_ptr<world::NativeBatchNodeV110> native_v111; //Checked typed constructor loan, never rawcast.
};
struct BatchDriverFieldsV96 {std::shared_ptr<void> owner;std::int32_t* vertices22c{};std::int32_t* indices230{};};
struct BatchSegmentBorrowV96 {std::shared_ptr<void> owner;std::uintptr_t* game_object2c{};};
using BatchLinkedCallbackV96=std::function<bool(std::uintptr_t mesh,std::uintptr_t segment_index,std::string&)>;
//Real native resource/renderer primitives only. Loader owns compiler/list/map/
//selection/visibility traversal and all stage order. No whole-stage callback.
struct BatchNativeServicesV96 {
 std::shared_ptr<void> provider; //Independent authority, never containing World/Level.
 std::function<bool(std::string&)> validate_current;
 std::function<bool(const world::CanonicalObjectBorrowV1&,BatchObjectBorrowV96&,std::string&)> as_gameobject;
 std::function<bool(std::uintptr_t,BatchObjectBorrowV96&,std::string&)> object;
 std::function<bool(const BatchObjectBorrowV96&,BatchNodeBorrowV96&,std::string&)> visual_root;
 std::function<bool(std::uintptr_t,bool&,std::string&)> is_faerie;
 std::function<bool(const char*,std::string&)> trace;
 std::function<bool(const BatchNodeBorrowV96&,std::string&)> scene_add_child5c;
 std::function<bool(std::uint32_t,BatchMeshBorrowV96&,std::string&)> construct_mesh;
 std::function<bool(std::int32_t,const BatchMeshBorrowV96&,BatchRootBorrowV96&,std::string&)> construct_root;
 //Exact CSceneManager.compile58ff10: boolfalse, callback, NULL split choice,
 //literalzero Point3D. Resource implementation consumes actual SAME nodes.
 std::function<bool(const std::vector<BatchNodeBorrowV96>&,BatchRootBorrowV96&,bool,
  const BatchLinkedCallbackV96&,std::nullptr_t,const std::array<float,3>&,std::string&)> scene_compile50;
 std::function<bool(std::uintptr_t&,std::string&)> current_rendered_node9c;
 //Lends actual (mesh.buffer8 + index*mesh.stride70 +2c) GameObject cell.
 std::function<bool(std::uintptr_t,std::uintptr_t,BatchSegmentBorrowV96&,std::string&)> segment;
 std::function<bool(std::uint32_t&,std::string&)> driver_type5c;
 std::function<bool(std::uintptr_t,bool,bool,std::string&)> quantize_components;
 std::function<bool(std::uintptr_t,bool,bool,bool,std::string&)> flush_mesh_buffers;
 std::function<bool(const BatchObjectBorrowV96&,std::string&)> set_visual_null;
 std::function<bool(std::string&)> clear_render_lists;
 std::function<bool(BatchDriverFieldsV96&,std::string&)> driver_fields;
 //Only exceptional __aeabi_d2iz domain needs imported native conversion.
 std::function<bool(double,std::int32_t&,std::string&)> exceptional_d2iz;
};
//Native successor of ORIGINAL batch::BatchNodeCompiler at SAME Level158.
//No alternate Scene/World/mesh. Root34 is the actual supplied CBatchSceneNode.
class BatchNodeCompilerSourceV96 {
 BatchNativeServicesV96 services_;
 bool compiled0_{},failed_{},busy_{},destroying_{},destroyed_{},root_drop_complete_{},temp_drop_complete_{};
 std::vector<std::uintptr_t> objects10_;
 std::map<std::uintptr_t,std::uintptr_t> node_objects1c_;
 BatchMeshBorrowV96 temporary_mesh_;
 BatchRootBorrowV96 root34_,pending_root_; //Ctor failure journal; pending is not native34 publication.
 std::string failure_;
 bool reject(std::string&,const char*);
 bool current(std::string&);
 bool map_node(std::uintptr_t,const BatchNodeBorrowV96&,std::string&);
 bool visible_recursive(const BatchNodeBorrowV96&,bool,std::string&);
 bool no_batch_visible(const BatchNodeBorrowV96&,bool,bool&,std::string&);
 bool linked(std::uintptr_t,std::uintptr_t,std::string&);
 bool trim(std::string&);
public:
 explicit BatchNodeCompilerSourceV96(BatchNativeServicesV96 s):services_(std::move(s)){}
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 bool append_object(const BatchObjectBorrowV96&,std::string&);
 bool build_map(std::string&);
 bool compile(bool quantize,std::string&);
 bool finish_scene_attachment(std::string&);
 //Explicit original Free/D1. Failure retains SAME node/temporary native
 //reference prefix. Ordinary shared_ptr expiry is not a native D0 claim.
 bool destroy_source(std::string&);
 bool compiled0()const noexcept{return compiled0_;}
 const BatchRootBorrowV96& root34()const noexcept{return root34_;}
 const BatchNativeServicesV96& native_services()const noexcept{return services_;}
 const auto& source_objects10()const noexcept{return objects10_;}
 const auto& source_node_map1c()const noexcept{return node_objects1c_;}
};
bool release_level_batch_compiler_v96(const std::shared_ptr<CanonicalLevelContextV1>&,std::string&);
bool original_batch_eligible_v96(const BatchObjectBorrowV96&,const BatchNativeServicesV96&,bool&,std::string&);
bool original_batch_limit_v96(std::int32_t,const BatchNativeServicesV96&,std::int32_t&,std::string&);
}


