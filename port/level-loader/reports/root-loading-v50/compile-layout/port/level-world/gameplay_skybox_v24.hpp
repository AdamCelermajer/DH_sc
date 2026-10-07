#pragma once
#include "gameobject_scene_root_registry_v1.hpp"
#include "../engine-textures/textures.hpp"
namespace dh2::camera {
struct SkyboxTextureV24 {std::string uri;std::vector<std::uint8_t> bytes,rgba;std::uint32_t width{},height{};};
struct SkyboxBufferV24 {
 assets::Primitive primitive{};
 std::uint32_t material{};
 std::vector<std::array<float,3>> positions;
 std::vector<std::array<float,2>> uv;
 std::vector<std::uint32_t> indices;
};
struct SkyboxResourceV24 {
 std::string file;
 std::vector<std::uint8_t> bytes;
 resources::BresView bres{};
 scene::Scene scene;
 std::uint32_t source_mesh_instance{};
 assets::Mesh mesh{};
 std::vector<SkyboxBufferV24> buffers;
 std::vector<std::shared_ptr<SkyboxTextureV24>> textures;
};
struct SkyboxMaterialRendererBorrowV24 {
 std::shared_ptr<void> owner;
 // SAME original selected RenderPass flags+4 and dirty byte+30. Bit20 is
 // depth-write in the independently verified original GL state projection.
 std::uint32_t* flags4{};std::uint8_t* dirty30{};
};
struct SkyboxLoadServicesV24 {
 std::shared_ptr<void> actual_resource_owner;
 std::shared_ptr<world::GameObjectSceneRootRegistryV1> same_roots;
 std::function<bool(const std::string&,std::vector<std::uint8_t>&,bool&,std::string&)> read;
 std::function<bool(const std::shared_ptr<SkyboxResourceV24>&,std::uint32_t,SkyboxMaterialRendererBorrowV24&,std::string&)> material_renderer;
};
class SkyboxNodeV24;
struct SkyboxQueueEntryV24 {
 std::shared_ptr<SkyboxNodeV24> node;
 std::uint32_t buffer_parameter{}; // source index+1, never zero
 std::int32_t pass{2},priority{INT32_MAX};std::uint32_t flags{};
 std::uint32_t material{};
};
struct SkyboxRenderServicesV24 {
 bool actual_driver_present{},actual_active_camera_present{};
 std::function<bool(std::array<float,3>&,std::string&)> active_camera_position;
 std::function<bool(float&,std::string&)> active_camera_near;
 // Original IMesh.setTransform -> actual driver state1 world matrix.
 std::function<bool(const std::array<float,16>&,std::string&)> set_world_transform;
 std::function<bool(const SkyboxResourceV24&,const SkyboxBufferV24&,std::string&)> set_material;
 std::function<bool(std::uint32_t,std::string&)> set_shadow_cascade;
 std::function<bool(const SkyboxResourceV24&,const SkyboxBufferV24&,std::string&)> draw_buffer;
};
class SkyboxNodeV24:public std::enable_shared_from_this<SkyboxNodeV24> {
 struct ParentLease;
 std::shared_ptr<SkyboxResourceV24> resource_;
 std::vector<SkyboxMaterialRendererBorrowV24> materials_;
 std::weak_ptr<world::GameObjectSceneRootRegistryV1> roots_;
 std::uint32_t references_{1},flags11c_{0x60f},culling118_{};
 std::uintptr_t parentec_{},manager110_{};
 std::uint8_t local120_{1},parent121_{1},enabled138_{1};
 std::array<float,16> transform_{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
 bool alive_{true};
public:
 SkyboxNodeV24(std::shared_ptr<SkyboxResourceV24>,std::vector<SkyboxMaterialRendererBorrowV24>,std::shared_ptr<world::GameObjectSceneRootRegistryV1>);
 bool construct_tail(std::string&);
 bool attach(std::string&);
 bool detach(std::string&);
 bool grab(std::string&);
 bool drop(std::string&);
 bool register_buffers(bool actual_driver_present,const std::function<bool(const SkyboxQueueEntryV24&,std::string&)>&,std::string&);
 bool render(std::uint32_t source_parameter,const SkyboxRenderServicesV24&,std::string&);
 const std::shared_ptr<SkyboxResourceV24>& resource()const noexcept{return resource_;}
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 std::uint32_t source_references()const noexcept{return references_;}
 std::uint32_t source_flags11c()const noexcept{return flags11c_;}
 std::uint32_t source_culling118()const noexcept{return culling118_;}
 static constexpr std::uint32_t source_type(){return 0x5f796b73u;}
 std::uint8_t& source_enabled138()noexcept{return enabled138_;}
 const std::array<float,16>& source_transform24()const noexcept{return transform_;}
};
class GameplaySkyboxV24 {
 SkyboxLoadServicesV24 services_;
 std::shared_ptr<SkyboxNodeV24> skybox438_;
 std::shared_ptr<SkyboxResourceV24> reached_resource_;
 bool prepare(const std::string&,std::shared_ptr<SkyboxResourceV24>&,bool&,std::string&);
public:
 explicit GameplaySkyboxV24(SkyboxLoadServicesV24 s):services_(std::move(s)){}
 // Original AddSkyBoxSceneNode(file,NULL): empty/actual resource miss keeps
 // the current skybox; positive replacement removes old before new C1.
 bool add(const std::string&,std::string&);
 bool release(std::string&);
 const std::shared_ptr<SkyboxNodeV24>& current()const noexcept{return skybox438_;}
};
}
extern "C" void dh2_skybox_transform_v24(float*,const float*,const float*);
extern "C" void dh2_skybox_clear_depth_write_v24(std::uint32_t*,std::uint8_t*);
