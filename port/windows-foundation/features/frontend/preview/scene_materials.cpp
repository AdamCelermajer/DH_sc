#include "scene_materials.hpp"
#include "../../../source_material_pass.hpp"
#include "../../../actor_lighting.hpp"
namespace dh::foundation::frontend {
bool apply_preview_scene_materials(const std::vector<std::uint8_t>& bytes,OriginalScene& scene,std::string& error){
    dh2::resources::BresView view{};
    if(dh2_bres_open(&view,bytes.data(),bytes.size())!=dh2::resources::BresError::ok){error="Preview scene material BRES rejected";return false;}
    if(scene.materials.size()!=scene.mesh.ranges.size()){error="Preview material/range count differs";return false;}
    auto ranges=scene.mesh.ranges;
    for(std::size_t i=0;i<ranges.size();++i){
        CommonMaterialPass pass;
        const auto result=resolve_common_material_pass(view,scene.materials[i].id,pass,error);
        if(result==CommonMaterialPassResult::invalid)return false;
        if(result!=CommonMaterialPassResult::applied)continue;
        auto& material=ranges[i].material;material.sourcePass=pass.state;
        const auto lighting=classifySourceVertexLighting(pass.vertexShader,pass.vertexDefines);
        if(lighting==SourceVertexLighting::CommonUnlit)material.lightingEnabled=false;
        // CommonLit requires the actual source light/material uniform owner.
        // Keep the host's lighting boundary explicit instead of inventing it.
    }
    scene.mesh.ranges=std::move(ranges);error.clear();return true;
}
}
