#include "effects_material_binding.hpp"
#include "../../frame_perf.hpp" // B062 probes
#include "../../content_paths.hpp"
#include "../../source_material_pass.hpp"
#include "../../actor_lighting.hpp"
#include "../../../scene-materials/effect_render_pass_v4.hpp"
#include <algorithm>

namespace dh::foundation::effects {
OriginalEffectMaterialBinding::~OriginalEffectMaterialBinding() { clear(); }
void OriginalEffectMaterialBinding::clear() {
    if(services_.release)for(const auto& texture:textures_)services_.release(texture.second);
    textures_.clear();
    resolved_.clear();
}
bool OriginalEffectMaterialBinding::bind(const EffectDrawSource& source,
    const dh2::scene::Material& original,Material& output,std::string& error) {
    error.clear();CommonMaterialPass pass;
    CommonMaterialPassResult result;{DH_PROBE("fxmat.common_pass");result=resolve_common_material_pass(source.image,original.id,pass,error);}
    if(result!=CommonMaterialPassResult::applied) {
        if(error.empty())error="Required original FX shader backend: "+original.id;
        return false;
    }
    dh2::scene::EffectRenderPassV4 rich;
    {DH_PROBE("fxmat.rich_pass");if(!dh2::scene::effect_render_pass_v4(source.image,original.id.c_str(),pass.technique.c_str(),rich,error))return false;}
    if(rich.stencil||rich.sample_coverage||rich.polygon_offset) {
        error="Required original FX stencil/coverage/offset renderer state";return false;
    }
    if(classifySourceVertexLighting(pass.vertexShader,pass.vertexDefines)!=SourceVertexLighting::CommonUnlit) {
        error="Required original FX lit vertex shader/light-set evaluation";return false;
    }
    if(!original.alpha_map.empty()&&original.alpha_map!=original.diffuse) {
        error="Required separate original FX alpha-map shader binding";return false;
    }
    Material material;std::copy_n(original.color,4,material.color.begin());
    material.sourcePass=pass.state;material.alphaReference=original.alpha_ref;
    material.doubleSided=!pass.state.cull;material.additive=original.additive;
    material.transparent=pass.state.blend;material.lightingEnabled=false;
    if(!original.diffuse.empty()) {
        if(!services_.upload||!services_.release) {
            error="Required original FX texture/context upload and release";return false;
        }
        try {
            std::filesystem::path path;
            {DH_PROBE("fxmat.resolve_path");
                auto known=resolved_.find({original.diffuse,source.resource_uri});
                if(known==resolved_.end())known=resolved_.emplace(std::make_pair(original.diffuse,source.resource_uri),resolve_content_path(assets_,original.diffuse,source.resource_uri)).first;
                path=known->second;}
            auto cached=textures_.find(path);
            if(cached!=textures_.end())material.texture=cached->second;
            else {
                DH_PROBE("fxmat.texture_load_upload");
                TextureImage image;if(!load_texture(path,image,error))return false;
                std::uint32_t texture=0;if(!services_.upload(image,texture,error)||!texture) {
                    if(texture)services_.release(texture);
                    if(error.empty())error="Original FX texture upload failed";return false;
                }
                try { textures_.emplace(path,texture); }
                catch(...) { services_.release(texture);throw; }
                material.texture=texture;
            }
        } catch(const std::exception& e) { error=e.what();return false; }
    }
    output=material;return true;
}
}
