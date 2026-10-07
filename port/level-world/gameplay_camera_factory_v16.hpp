#pragma once
#include "gameplay_camera_runtime_v11.hpp"
#include "gameobject_scene_root_registry_v1.hpp"
namespace dh2::camera {
struct CameraFactoryTypeV16 {std::uint32_t type;const char* name;};
const std::array<CameraFactoryTypeV16,16>& source_camera_factory_catalog_v16()noexcept;
struct CameraFactoryBackendV16 {
 std::shared_ptr<void> configured_backend;
 // Actual current driver viewport for onChangedSceneManager. This is backend
 // configuration (e.g. a landscape surface), not a guessed screenshot angle.
 std::function<bool(std::int32_t&,std::int32_t&,std::string&)> viewport;
};
class CameraProceduralNodeV16:public std::enable_shared_from_this<CameraProceduralNodeV16> {
 struct ParentLease;
 std::weak_ptr<world::GameObjectSceneRootRegistryV1> manager_;
 CameraFactoryBackendV16 backend_;
 scene::Scene graph_;
 std::uint32_t references_{1},flags11c_{0x60f},culling118_{};
 std::uintptr_t parentec_{},manager110_{};
 std::uint8_t local120_{1},parent121_{1};
 std::vector<std::shared_ptr<void>> animators_; // actual cam_ constructor empty
 CameraViewV11 view_;
 bool alive_{true},parent_attempted_{};
 std::string lifetime_error_;
 bool source_drop_leaf(std::string&);
 bool changed_manager(std::uintptr_t,std::string&);
public:
 CameraProceduralNodeV16(std::shared_ptr<world::GameObjectSceneRootRegistryV1>,CameraFactoryBackendV16);
 bool grab(std::string&);
 bool drop(std::string& e){return source_drop_leaf(e);}
 bool add_to_root(std::string&);
 bool remove_from_root(std::string&);
 bool notify_visibility(bool,std::string&);
 bool remove_animators(std::string&);
 bool set_data(float,float,float,float,std::string&);
 bool set_planes(float,float,std::string&);
 bool set_position(const PointV2&,std::string&);
 bool set_target(const PointV2&,std::string&);
 bool set_up(const PointV2&,std::string&);
 bool set_rotation(const std::array<float,4>&,std::string&);
 bool view(CameraViewV11&,std::string&);
 world::GameObjectSceneCameraBorrowV13 camera_borrow();
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 std::uint32_t source_references()const noexcept{return references_;}
 bool alive()const noexcept{return alive_;}
 std::uint32_t source_culling118()const noexcept{return culling118_;}
 std::uintptr_t source_parent_identity_v88()const noexcept{return parentec_;}
 const scene::Scene& graph()const noexcept{return graph_;}
};
// Actual DefaultSceneNodeFactory C1 type/name catalog and cam_ create branch.
// Keep one on the same SceneManager constructor graph as its first factory;
// other catalog constructors fail required rather than manufacture nodes.
class CameraDefaultFactoryV16 {
 std::weak_ptr<world::GameObjectSceneRootRegistryV1> manager14_;
 std::shared_ptr<void> file_system1c_;std::uintptr_t cursor18_{};
 CameraFactoryBackendV16 backend_;
public:
 CameraDefaultFactoryV16(std::shared_ptr<world::GameObjectSceneRootRegistryV1> manager,
                        std::shared_ptr<void> actual_file_system,std::uintptr_t actual_cursor,
                        CameraFactoryBackendV16 backend):manager14_(manager),file_system1c_(std::move(actual_file_system)),cursor18_(actual_cursor),backend_(std::move(backend)){}
 bool create(std::uint32_t type,std::uintptr_t parent,std::shared_ptr<CameraProceduralNodeV16>&,std::string&);
 std::uintptr_t source_cursor18()const noexcept{return cursor18_;}
};
}
