#include "combat_text_design.hpp"
#include "../../content_paths.hpp"
#include "../../../engine-ui/localization.hpp"
#include "../../../script-runtime/script_constants.hpp"
#include <map>

namespace dh::foundation {
struct CombatTextDesign::Impl {
    std::unique_ptr<AssetCatalog> assets;
    CombatTextDesignQueries queries;
    std::unique_ptr<dh2_script_constants,decltype(&dh2_script_constants_destroy)> constants{dh2_script_constants_create(),dh2_script_constants_destroy};
    dh2::ui::Localization localization;
    std::map<std::uintptr_t,std::vector<std::uint8_t>> leases;
    std::map<std::int32_t,std::string> strings;
    std::uintptr_t next=1;
    std::string error;
    dh2::ui::LocalizationServices services(){
        dh2::ui::LocalizationServices s;s.context=this;
        s.open=[](void*p,const char*uri,bool&found,std::vector<std::uint8_t>&out,std::uintptr_t&lease,std::string&e){
            auto&x=*static_cast<Impl*>(p);found=false;lease=0;
            try{auto bytes=read_content(*x.assets,uri);lease=x.next++;x.leases.emplace(lease,bytes);out=std::move(bytes);found=true;e.clear();return true;}
            catch(const std::exception&ex){e=ex.what();return false;}
        };
        s.close=[](void*p,std::uintptr_t lease,std::string&e){auto&x=*static_cast<Impl*>(p);if(x.leases.erase(lease)!=1){e="Unknown combat localization resource lease";return false;}e.clear();return true;};
        s.debug=[](void*p,const char*key,std::string&e){return static_cast<Impl*>(p)->queries.debug(key,e);};
        s.constant=[](void*p,const char*g,const char*k,std::uint32_t&out,std::string&e){std::int32_t v=0;const auto r=dh2_script_constants_get(static_cast<Impl*>(p)->constants.get(),g,k,&v);if(r){e="Missing source localization constant";return false;}out=std::uint32_t(v);return true;};
        s.player_character=[](void*p,std::uintptr_t&out,std::string&e){return static_cast<Impl*>(p)->queries.player_character(out,e);};
        s.player_name=[](void*p,std::uintptr_t actor,std::string&out,std::string&e){return static_cast<Impl*>(p)->queries.player_name(actor,out,e);};
        return s;
    }
};
CombatTextDesign::CombatTextDesign():impl_(std::make_unique<Impl>()){}
CombatTextDesign::~CombatTextDesign()=default;
bool CombatTextDesign::load(const AssetCatalog&assets,CombatTextDesignQueries queries,std::string&error){
    if(!queries.debug||!queries.player_character||!queries.player_name){error="Combat localization requires actual Debug/current-player/name queries";return false;}
    try{auto next=std::make_unique<Impl>();if(!next->constants){error="Source combat design allocation failed";return false;}
        next->assets=std::make_unique<AssetCatalog>(assets.root());next->queries=std::move(queries);
        for(const char*name:{"common_text_pycst.bin","design_pycst.bin"}){
            auto raw=read_content(assets,std::string("data/pydata/")+name);dh2_script_constants_reload result{};
            if(dh2_script_constants_load(next->constants.get(),raw.data(),static_cast<std::uint32_t>(raw.size()),&result)!=0||result.consumed!=raw.size()){error="Original combat constants rejected";return false;}
        }
        auto records=read_content(assets,"data/pydata/common_text_pyarray.bin"),names=read_content(assets,"data/pydata/common_text_pyarraynames.bin"),schema=read_content(assets,"data/pydata/common_text_pystructnames.bin");
        if(!next->localization.load({records.data(),records.size()},{names.data(),names.size()},{schema.data(),schema.size()},error)||!next->localization.switch_pack(0,false,error))return false;
        impl_=std::move(next);error.clear();return true;
    }catch(const std::exception&e){error=e.what();return false;}
}
int CombatTextDesign::constant(const char*g,const char*k,std::int32_t*out)const{return dh2_script_constants_get(impl_->constants.get(),g,k,out);}
int CombatTextDesign::localized(std::int32_t id,const char**out){
    if(!out||!impl_->assets)return -1;std::string value;
    if(!impl_->localization.string_id(std::uint32_t(id),impl_->services(),value,impl_->error))return -1;
    auto&stored=impl_->strings[id];stored=std::move(value);*out=stored.c_str();return 0;
}
const std::string&CombatTextDesign::error()const noexcept{return impl_->error;}
} // namespace dh::foundation
