#include "source_material_pass.hpp"
#include "../scene-materials/effect_render_pass_v4.hpp"
#include <cstring>
#include <sstream>
#include <stdexcept>
namespace dh::foundation {
namespace {
struct Reader {
    const dh2::resources::BresView& view;
    std::uint32_t word(std::size_t p) const {
        if(p>view.size||view.size-p<4)throw std::runtime_error("COMMON material field outside BRES");
        std::uint32_t result;std::memcpy(&result,view.bytes+p,4);return result;
    }
    std::string text(std::uint32_t p) const {
        if(!p)return {};
        if(p>=view.size)throw std::runtime_error("COMMON material string outside BRES");
        auto end=std::memchr(view.bytes+p,0,view.size-p);
        if(!end)throw std::runtime_error("COMMON material unterminated string");
        return {reinterpret_cast<const char*>(view.bytes+p),static_cast<std::size_t>(static_cast<const std::uint8_t*>(end)-(view.bytes+p))};
    }
};
bool alpha_test(const std::string& source) {
    std::istringstream lines(source);std::string line;
    while(std::getline(lines,line)){std::istringstream words(line);std::string directive,name;words>>directive>>name;
        if(directive=="#define"&&name=="ALPHATEST")return true;}
    return false;
}
}
CommonMaterialPassResult resolve_common_material_pass(const dh2::resources::BresView& view,
    const std::string& id,CommonMaterialPass& out,std::string& error) {
    error.clear();try {
        Reader r{view};std::size_t material=0;bool found=false;
        for(unsigned i=0;i<dh2_bres_library_count(&view,dh2::resources::Library::material);++i){
            auto row=dh2_bres_library_item(&view,dh2::resources::Library::material,i)-view.bytes;
            if(r.text(r.word(row))==id){material=row;found=true;break;}}
        if(!found)throw std::runtime_error("COMMON material ID not found");
        if(r.word(material+8))return CommonMaterialPassResult::notCommon;
        const auto uri=r.text(r.word(material+12));if(uri.empty()||uri[0]!='#')return CommonMaterialPassResult::notCommon;
        std::size_t effect=0;found=false;
        for(unsigned i=0;i<dh2_bres_library_count(&view,dh2::resources::Library::effect);++i){
            auto row=dh2_bres_library_item(&view,dh2::resources::Library::effect,i)-view.bytes;
            if(r.text(r.word(row))==uri.substr(1)){effect=row;found=true;break;}}
        if(!found)return CommonMaterialPassResult::notCommon;
        const auto count=r.word(effect+32),base=r.word(effect+36);
        if(!count)return CommonMaterialPassResult::notCommon;
        // Native CMaterial::allocate initializes technique byte zero. Serialized
        // type20 parameters can replace it with an actual declared technique ID.
        std::string selected=r.text(r.word(base));
        for(unsigned i=0;i<r.word(material+16);++i){auto p=r.word(material+20)+24*i;
            if(r.word(p+8)!=20)continue;
            auto candidate=r.text(r.word(r.word(p+20)+4));
            for(unsigned j=0;j<count;++j)if(r.text(r.word(base+12*j))==candidate){selected=candidate;break;}}
        // Identify the selected shader before admitting richer pass state.
        bool common=false;
        for(unsigned j=0;j<count;++j){auto row=base+12*j;if(r.text(r.word(row))!=selected)continue;
            if(r.word(row+4)!=1)return CommonMaterialPassResult::notCommon;
            auto pass=r.word(row+8);
            common=r.text(r.word(pass+4))=="ProfileCOMMON_emul_VS.glsl"&&r.text(r.word(pass+16))=="ProfileCOMMON_emul_FS.glsl";
            break;}
        if(!common)return CommonMaterialPassResult::notCommon;
        dh2::scene::EffectRenderPassV4 pass;
        if(!dh2::scene::effect_render_pass_v4(view,id.c_str(),selected.c_str(),pass,error))return CommonMaterialPassResult::invalid;
        // The 1.1 backend supports additive blend equation directly; admitting a
        // different equation would require an actual extension entry point.
        if(pass.blend_equation!=0x8006){error="COMMON pass requires unsupported blend equation";return CommonMaterialPassResult::invalid;}
        CommonMaterialPass result;result.technique=selected;result.vertexShader=pass.vertex_file;result.fragmentShader=pass.fragment_file;
        result.vertexDefines=pass.vertex_defines;result.fragmentDefines=pass.fragment_defines;
        result.state={pass.blend_src,pass.blend_dst,pass.depth_function,pass.cull_face,pass.front_face,
                      pass.blend,pass.depth,pass.depth_write,pass.cull,alpha_test(pass.fragment_defines)};
        out=std::move(result);return CommonMaterialPassResult::applied;
    }catch(const std::exception& ex){error=ex.what();return CommonMaterialPassResult::invalid;}
}
}
