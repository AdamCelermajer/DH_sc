#pragma once
#include "gameplay_skybox_v24.hpp"
#include "../scene-materials/effect_render_pass_v4.hpp"
namespace dh2::camera {
struct SkyboxTechniqueV25 {
 std::string name;
 scene::EffectRenderPassV4 authored;
 // Mutable source pass+4 authority; authored.pass is immutable load input.
 std::uint32_t flags4{};std::uint8_t dirty30{};
};
// Concrete CPU material/pass counterpart built from SAME real BRES material
// and all authored GLES2 techniques. Program compilation/bind remains Root's
// actual ShaderProgramCollection/GPU service, not a successful no-op here.
class SkyboxMaterialV25:public std::enable_shared_from_this<SkyboxMaterialV25> {
 std::vector<SkyboxTechniqueV25> techniques_;
 std::uint32_t material_{};
 bool ready_{};
public:
 bool initialize(const SkyboxResourceV24&,std::uint32_t,std::string&);
 bool constructor_first_pass_borrow(SkyboxMaterialRendererBorrowV24&,std::string&);
 bool state(std::uint32_t actual_technique,scene::EffectRenderPassV4&,std::string&)const;
 const std::vector<SkyboxTechniqueV25>& techniques()const noexcept{return techniques_;}
 std::uint32_t material_index()const noexcept{return material_;}
};
// Called by the actual same material factory; one table for this actual
// material. No inferred current/fog/lighting technique is selected here.
bool construct_skybox_material_v25(const std::shared_ptr<SkyboxResourceV24>&,std::uint32_t,
 std::shared_ptr<SkyboxMaterialV25>&,SkyboxMaterialRendererBorrowV24&,std::string&);
class GameplaySkyboxPipelineV25 {
 std::map<std::pair<const SkyboxResourceV24*,std::uint32_t>,std::weak_ptr<SkyboxMaterialV25>> materials_;
 std::unique_ptr<GameplaySkyboxV24> loader_;
public:
 explicit GameplaySkyboxPipelineV25(SkyboxLoadServicesV24 actual);
 bool add(const std::string&file,std::string&e){return loader_->add(file,e);}
 bool release(std::string&e){return loader_->release(e);}
 const std::shared_ptr<SkyboxNodeV24>& current()const noexcept{return loader_->current();}
 bool material(const SkyboxResourceV24&,std::uint32_t,std::shared_ptr<SkyboxMaterialV25>&,std::string&)const;
};
}
