#pragma once
#include "native_batch_resources_v110.hpp"
#include "../engine-resources/resource_budget_v37.hpp"
#include "../engine-resources/cpu_vector_capacity_v41.hpp"
#include <array>
#include "../scene-materials/effect_render_pass_v4.hpp"
#include "native_batch_material_values_v113.hpp"
namespace dh2::world {
struct RetainedMeshNodeV91;
struct RetainedMeshDataV91;
struct NativeMaterialLightsV113;
struct NativeBatchVertexV111 {float position[3]{},uv[2]{},color[4]{1,1,1,1};};
struct NativeBatchAttributeV111 {
 std::uint32_t source_type{},components{};
 resources::CpuVectorCapacityV41 quantized_charge;
 std::vector<std::array<float,4>> values;
 std::array<float,4> quantized_scale{},quantized_offset{};
 std::vector<std::array<std::int16_t,4>> quantized_components;
};
struct NativeBatchPartV111 {
 scene::Material material;
 scene::EffectRenderPassV4 pass_v112;
 std::shared_ptr<const NativeBatchMaterialValuesV113> material_values_v113;
 std::shared_ptr<NativeMaterialLightsV113> material_lights_v113;
 resources::CpuVectorCapacityV41 vertices_charge,indices_charge;
 std::array<resources::CpuVectorCapacityV41,18> attribute_charges;
 std::array<NativeBatchAttributeV111,18> attributes;
 resources::CpuVectorCapacityV41 uv_charge,normal_charge;
 std::vector<NativeBatchVertexV111> vertices;
 std::vector<std::uint16_t> indices;
 std::size_t segment{};
 std::uintptr_t source_node{};
 std::weak_ptr<RetainedMeshNodeV91> source_mesh_v111;
 std::uint32_t source_primitive_v111{};
 bool dynamic_v112{},skinned_v112{};
 bool native_retired_v113{}; //actual source Clean retires backend delivery, never source visibility.
 std::uintptr_t animation_identity_v113{};
 std::shared_ptr<RetainedMeshDataV91> source_buffer_v112;
 std::shared_ptr<scene::InstanceStorageV91> source_fields_v112;
 resources::CpuVectorCapacityV41 source_ids_charge_v112;
 std::vector<std::uint32_t> source_ids_v112;
 resources::CpuVectorCapacityV41 rest_normal_charge_v113;
 std::vector<std::array<float,3>> rest_normals_v113;
 std::array<float,6> bounds{};
 std::array<float,2> uv_scale{},uv_offset{};
 std::vector<std::array<std::int16_t,2>> quantized_uv;
 std::vector<std::array<std::int16_t,3>> quantized_normal;
};
// NativeBatchVertexV111 carries an explicit white default color. The source
// compiler leaves attributes[2] absent when the mesh has no color stream, but
// shader variants may still consume color. Only treat a genuinely absent
// color stream as this authored transport default; quantized or malformed
// streams must continue through the regular validation path.
inline bool native_batch_default_color_v113(const NativeBatchPartV111& part) noexcept {
 const auto& color=part.attributes[2];
 return color.source_type==0&&color.components==0&&color.values.empty()&&color.quantized_components.empty();
}
struct NativeBatchGpuServicesV111 {
 std::shared_ptr<void> owner;
 std::function<bool(const std::shared_ptr<NativeBatchMeshV110>&,std::string&)> upload;
 std::function<bool(std::uintptr_t,std::string&)> release;
 std::function<bool(std::uintptr_t,std::string&)> invalidate;
};
class NativeBatchCompiledV111 {
 bool released_{},quantized_{},uploaded_{};
public:
 std::shared_ptr<resources::ContextResourceBudgetV37> budget;
 std::weak_ptr<NativeBatchNodeV110> root;
 std::vector<NativeBatchPartV111> parts;
 NativeBatchGpuServicesV111 gpu;
 std::function<bool(std::size_t,bool&,std::string&)> visible;
 std::uint64_t context_generation{};
 std::map<std::uintptr_t,BatchAnimationBorrowV112> animations_v112;
 std::map<std::uintptr_t,std::vector<BatchAnimationBorrowV112>> retiring_animations_v113;
 std::uint64_t pose_revision_v112{};
 std::uint32_t solid_batches_v112{};
 std::uintptr_t mesh_identity_v112{};
 std::function<bool(std::string&)> validate_current_v112;
 bool update_segment_content_v112(std::size_t,std::string&);
 bool animate_v112(std::uint32_t,std::string&);
 bool retire_object_v113(NativeBatchMeshV110&,std::uintptr_t,std::string&);
 void finish_object_retirement_v113(std::uintptr_t object)noexcept{retiring_animations_v113.erase(object);}
 bool quantize(bool positions,bool normal,std::string&);
 bool flush(const std::shared_ptr<NativeBatchMeshV110>&,bool,bool,bool,std::string&);
 bool release(std::uintptr_t,std::string&);
 bool quantized()const noexcept{return quantized_;}
};
struct NativeBatchCompileServicesV111 {
 std::shared_ptr<void> owner;
 std::shared_ptr<resources::ContextResourceBudgetV37> budget;
 std::int32_t* vertices22c{};std::int32_t* indices230{};
 std::function<bool(std::string&)> current;
 std::function<bool(std::uintptr_t&,std::string&)> get_rendered;
 std::function<bool(std::uintptr_t,std::string&)> set_rendered;
 NativeBatchGpuServicesV111 gpu;
 std::function<bool(std::uintptr_t,bool&,std::string&)> game_object_visible;
 std::function<bool(const resources::BresView&,const scene::Material&,scene::EffectRenderPassV4&,std::string&)> material_pass_v112;
 std::function<bool(const resources::BresView&,const scene::Material&,NativeBatchMaterialValuesV113&,std::string&)> material_values_v113;
 std::function<bool(const std::shared_ptr<RetainedMeshNodeV91>&,std::uint32_t,
  std::shared_ptr<const NativeBatchMaterialValuesV113>,std::shared_ptr<NativeMaterialLightsV113>&,std::string&)> material_lights_v113;
};
//Actual58ff10/58fd10 native compile transport consumes real V109 child loans.
//Dynamic ranges retain source buffers/animation graph and their live updates.
bool native_compile_scene_v111(const std::vector<loader::BatchNodeBorrowV96>&,
 const std::shared_ptr<NativeBatchMeshV110>&,const NativeBatchCompileServicesV111&,
 const loader::BatchLinkedCallbackV96&,std::string&);
inline bool resolve_material_binding_v111(const std::vector<scene::InstanceMaterialBindingV1>& bindings,
 const std::string& symbol,scene::Material& out,std::string& e){
 if(symbol.empty()){e="Empty source primitive material symbol";return false;}
 const scene::InstanceMaterialBindingV1* match=nullptr;
 for(const auto& binding:bindings)if(binding.symbol==symbol){
  if(match){e="Duplicate source material-symbol binding";return false;}
  match=&binding;
 }
 if(!match){e="Missing source material-symbol binding";return false;}
 if(match->target.id.empty()){e="Missing source material target ID";return false;}
 out=match->target;e.clear();return true;
}
}
