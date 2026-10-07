#pragma once
#include "scene.hpp"
#include <memory>
#include <functional>
#include <map>
namespace dh2::world {
struct NativeLightV113;
class NativeSceneLightNodeV113;
struct NativeBatchMaterialValuesV113;
struct NativeMaterialLightsV113 {
 std::array<std::shared_ptr<NativeLightV113>,4> lights;
 std::array<std::uint8_t,4> source_assignment_v113{}; //native receipt of reached positive SetParameter; preserves earlier ApplySettings stores
 std::shared_ptr<const NativeBatchMaterialValuesV113> source_creation_v113;
 NativeMaterialLightsV113()=default;
 NativeMaterialLightsV113(const NativeMaterialLightsV113&)=delete;
 ~NativeMaterialLightsV113();
 bool assign(unsigned,std::shared_ptr<NativeLightV113>,std::string&);
};
class SceneManagerMapOwnerV2;struct RetainedVisualNodeV91;
struct RetainedMeshDataV91 {
 struct BatchBacklinkV112 {std::uintptr_t batch18{};std::size_t segment1c{};std::weak_ptr<void> owner;};
 //Per-source primitive CMeshBuffer30 native adaptation. C1 has no backlink;
 //actual compiler sort writes18/1c only for a retained dynamic buffer.
 std::map<std::uint32_t,BatchBacklinkV112> batch_backlinks_v112;
 using Point=std::array<float,3>;
 std::vector<Point> source_positions,positions;
 std::vector<Point> source_normals_v113,normals_v113;
 std::vector<std::array<std::uint32_t,3>> triangles;
 std::array<float,6> bounds{};
 static bool create(const resources::BresView&,std::uint32_t geometry,std::shared_ptr<RetainedMeshDataV91>&,std::string&);
};
struct RetainedMeshNodeV91 {
 std::map<std::uint32_t,std::shared_ptr<NativeMaterialLightsV113>> material_lights_v113;
 std::function<bool(const std::string&,std::shared_ptr<NativeLightV113>&,std::string&)> local_light_v113;
 std::shared_ptr<scene::InstanceStorageV91> fields;
 std::shared_ptr<RetainedMeshDataV91> mesh;
 std::shared_ptr<void> source_resource_v93;resources::BresView source_image_v93{};
 std::weak_ptr<RetainedVisualNodeV91> parent;std::weak_ptr<SceneManagerMapOwnerV2> map_parent_v93;
 std::function<void()> hierarchy_changed_v93;
 bool native_destroyed_v106{};
 void destroy_native_storage_v106()noexcept;
 void source_set_visible(bool visible)noexcept{auto& state=fields->mesh_fields;state.visibility.local120=visible?1:0;
  if(visible&&state.visibility.parent121&&!fields->detached)state.flags|=1u;else state.flags&=~1u;}
 void source_hide()noexcept{source_set_visible(false);}
};
class RetainedTriangleSelectorV91 {
 std::shared_ptr<RetainedMeshDataV91> mesh_;
public:
 explicit RetainedTriangleSelectorV91(std::shared_ptr<RetainedMeshDataV91> m):mesh_(std::move(m)){}
 const std::shared_ptr<RetainedMeshDataV91>& mesh()const noexcept{return mesh_;}
 bool line(const float* start,const float* end,bool& hit,std::array<float,3>& point,std::string&)const;
};
struct RetainedVisualNodeV91 {
 std::uint32_t index{};
 std::shared_ptr<scene::NodeStorageV91> fields;
 std::weak_ptr<RetainedVisualNodeV91> parent;
 std::vector<std::shared_ptr<RetainedVisualNodeV91>> children;
 std::vector<std::shared_ptr<RetainedMeshNodeV91>> meshes;
 std::vector<std::shared_ptr<NativeSceneLightNodeV113>> lights_v113; //actual instance-light children, one native parent reference each
 std::shared_ptr<RetainedTriangleSelectorV91> selector;
 // Native reference domains: parent membership and explicit source grabs.
 // Diagnostic scene/view shared_ptr copies are not counted as engine grabs.
 bool native_parent_owned_v106{},native_destroyed_v106{};std::uint32_t native_external_grabs_v106{};
 bool grab_native_v106(std::shared_ptr<RetainedVisualNodeV91>&,const std::shared_ptr<RetainedVisualNodeV91>&,std::string&);
 bool validate_native_d1_v106(std::string&)const;
 bool drop_parent_native_v106(std::string&);
 void drop_external_native_v106()noexcept;
 void destroy_native_v106()noexcept;
 void collect_meshes(std::vector<std::shared_ptr<RetainedMeshNodeV91>>&)const;
};
}
