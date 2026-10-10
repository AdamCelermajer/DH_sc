#include "source_level_config.hpp"
#include "asset_catalog.hpp"
#include "content_paths.hpp"
#include "../level-loader/canonical_source_adapter_v1.hpp"
#include <cstring>
#include <map>
#include <stdexcept>
#include <utility>

namespace dh::foundation {
bool source_level_config_network_leaf(const SourceLevelConfigNetworkBorrow& source,
    dh2::world::CanonicalObjectBorrowV1&,std::string& error){
    if(!source.applicationOwner||!source.onlineByte5){error="Required actual Application Online.byte5 owner";return false;}
    if(!*source.onlineByte5){error.clear();return true;} // Literal original offline return343274.
    error="Required positive online AssignObjectNetworkId provider";return false;
}
std::function<bool(const char*,bool&,std::string&)> source_level_config_debug_switch(
    std::shared_ptr<dh2::character::DebugSwitches> debug,std::shared_ptr<void> filesOwner,
    dh2::character::DebugFileServices24 files,
    std::optional<dh2::character::DebugExistingFileServicesV136> streams){
    return [debug=std::move(debug),filesOwner=std::move(filesOwner),files,streams](const char* key,bool& value,std::string& error){
        if(!debug||!filesOwner||!key){error="Required actual runtime Debug owner/files/query key";return false;}
        const auto loaded=streams?dh2_character_debug_load_stream_v136(debug.get(),&files,&*streams):dh2_character_debug_load(debug.get(),&files);
        std::uint32_t raw=0;
        if(loaded!=1||dh2_character_debug_get(&raw,debug.get(),key,&files)!=1){error="Actual runtime Debug load/query failed";return false;}
        value=raw!=0;error.clear();return true;
    };
}
struct SourceLevelConfig::Impl : std::enable_shared_from_this<Impl> {
    SourceLevelConfigProviders providers;
    std::map<std::uintptr_t,std::shared_ptr<dh2::world::CanonicalLevelConfigV1>> configs;
    explicit Impl(SourceLevelConfigProviders p):providers(std::move(p)){}
    std::shared_ptr<dh2::world::CanonicalLevelConfigV1> find(const dh2::world::CanonicalObjectBorrowV1& object){
        const auto i=configs.find(object.identity);return i==configs.end()?nullptr:i->second;
    }
    static bool construct(void* context,const dh2::world::CanonicalFactoryEntryV1& factory,
        const dh2::world::CanonicalSourceObjectRequestV1& request,dh2::world::CanonicalObjectBorrowV1& object,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(!factory.name||std::strcmp(factory.name,"LevelConfig")){
            if(self.providers.remaining.construct&&self.providers.remainingOwner)
                return self.providers.remaining.construct(self.providers.remaining.context,factory,request,object,error);
            error=std::string("Required actual next source constructor: ")+(factory.name?factory.name:"<null>");return false;
        }
        if(factory.original_address!=0x340ca8){error="LevelConfig original catalog constructor address differs";return false;}
        if(!self.providers.levelOwner){error="LevelConfig constructor requires SAME current-Level owner";return false;}
        dh2::world::LevelConfigServicesV1 services;services.owner=self.providers.levelOwner;
        services.debug_switch=self.providers.debugSwitch;
        const auto weak=self.weak_from_this();
        services.set_level_config=[weak](std::uintptr_t identity,std::string& e){
            const auto state=weak.lock();if(!state||!state->providers.current){e="Required SAME in-house current-Level config field";return false;}
            const auto i=state->configs.find(identity);if(i==state->configs.end()){e="LevelConfig publication addressed another source receiver";return false;}
            // Concrete field publication; no music/UI/World side effects are
            // claimed as original Level::SetLevelConfig3f150c completion.
            state->providers.current->config=i->second;e.clear();return true;
        };
        auto config=std::make_shared<dh2::world::CanonicalLevelConfigV1>(self.providers.levelOwner,std::move(services));
        object=config->canonical(config);self.configs.emplace(config->identity(),std::move(config));error.clear();return true;
    }
    template<class F> static bool property(void* context,const dh2::world::CanonicalObjectBorrowV1& object,std::string& error,F operation){
        auto& self=*static_cast<Impl*>(context);auto config=self.find(object);
        if(!config){error="LevelConfig property callback addressed unowned receiver";return false;}
        if(!self.providers.properties){error="Required SAME actual CanonicalPropertyMapV1";return false;}
        auto actor=config->properties();return operation(*self.providers.properties,actor,error);
    }
    static bool init_properties(void* c,const dh2::world::CanonicalObjectBorrowV1& o,std::string& e){
        auto& s=*static_cast<Impl*>(c);if(!s.find(o)){
            if(s.providers.remaining.init_properties&&s.providers.remainingOwner)return s.providers.remaining.init_properties(s.providers.remaining.context,o,e);
            e="Required next source InitProperties provider";return false;}
        return property(c,o,e,[](auto& map,auto& actor,auto& error){return map.init_properties(actor,error);});
    }
    static bool set_template(void* c,const dh2::world::CanonicalObjectBorrowV1& o,const char* name,std::string& e){
        auto& s=*static_cast<Impl*>(c);if(!s.find(o)){
            if(s.providers.remaining.set_template&&s.providers.remainingOwner)return s.providers.remaining.set_template(s.providers.remaining.context,o,name,e);
            e="Required next source SetTemplate provider";return false;}
        return property(c,o,e,[name](auto& map,auto& actor,auto& error){return map.set_template(actor,name,error);});
    }
    static bool defaults(void* c,const dh2::world::CanonicalObjectBorrowV1& o,std::string& e){
        auto& s=*static_cast<Impl*>(c);if(!s.find(o)){
            if(s.providers.remaining.load_defaults&&s.providers.remainingOwner)return s.providers.remaining.load_defaults(s.providers.remaining.context,o,e);
            e="Required next source LoadDefaults provider";return false;}
        return property(c,o,e,[](auto& map,auto& actor,auto& error){return map.load_defaults(actor,error);});
    }
    static bool overrides(void* c,const dh2::world::CanonicalObjectBorrowV1& o,const dh2::world::CanonicalSourceObjectRequestV1& q,std::string& e){
        auto& s=*static_cast<Impl*>(c);if(!s.find(o)){
            if(s.providers.remaining.load_overrides&&s.providers.remainingOwner)return s.providers.remaining.load_overrides(s.providers.remaining.context,o,q,e);
            e="Required next source LoadOverrides provider";return false;}
        return property(c,o,e,[&q](auto& map,auto& actor,auto& error){return map.load_overrides(actor,q,error);});
    }
    static bool init_post(void* c,const dh2::world::CanonicalObjectBorrowV1& o,std::string& e){
        auto& s=*static_cast<Impl*>(c);if(auto config=s.find(o))return config->init_post(e);
        if(s.providers.remaining.init_post&&s.providers.remainingOwner)return s.providers.remaining.init_post(s.providers.remaining.context,o,e);
        e="Required next source InitPost provider";return false;
    }
    static bool is_game(void* c,const dh2::world::CanonicalObjectBorrowV1& o,bool& game,std::string& e){
        auto& s=*static_cast<Impl*>(c);if(s.find(o)){game=false;e.clear();return true;} // Original virtual type fact.
        if(s.providers.remaining.is_game_object&&s.providers.remainingOwner)return s.providers.remaining.is_game_object(s.providers.remaining.context,o,game,e);
        e="Required next source IsGameObject provider";return false;
    }
    static bool position(void* c,const dh2::world::CanonicalObjectBorrowV1& o,std::array<float,3>& p,std::string& e){
        auto& s=*static_cast<Impl*>(c);if(s.providers.remaining.position&&s.providers.remainingOwner)return s.providers.remaining.position(s.providers.remaining.context,o,p,e);
        e="Required reached next source position provider";return false;
    }
    static bool set_position(void* c,const dh2::world::CanonicalObjectBorrowV1& o,const std::array<float,3>& p,bool update,std::string& e){
        auto& s=*static_cast<Impl*>(c);if(s.providers.remaining.set_position&&s.providers.remainingOwner)return s.providers.remaining.set_position(s.providers.remaining.context,o,p,update,e);
        e="Required reached next source SetPosition provider";return false;
    }
    static bool unknown(void* c,const char* type,std::string& e){
        auto& s=*static_cast<Impl*>(c);if(s.providers.remaining.unknown_type_debug&&s.providers.remainingOwner)return s.providers.remaining.unknown_type_debug(s.providers.remaining.context,type,e);
        e=std::string("Required reached unknown-type Debug provider: ")+type;return false;
    }
};
SourceLevelConfig::SourceLevelConfig(SourceLevelConfigProviders p):impl_(std::make_shared<Impl>(std::move(p))){}
dh2::world::CanonicalClassServicesV1 SourceLevelConfig::services() const noexcept{
    return {impl_.get(),Impl::construct,Impl::init_properties,Impl::set_template,Impl::defaults,Impl::overrides,Impl::init_post,Impl::is_game,Impl::position,Impl::set_position,Impl::unknown};
}
std::shared_ptr<void> SourceLevelConfig::lease() const noexcept{return impl_;}
bool SourceLevelConfig::settings(SourceLevelSettings& out,std::string& error) const{
    if(!impl_->providers.current||!impl_->providers.current->config){error="Current source LevelConfig has not been published";return false;}
    const auto& config=*impl_->providers.current->config;SourceLevelSettings settings;
    auto vector=[&](unsigned offset){const auto* v=config.vector(offset);if(!v)throw std::runtime_error("Missing produced LevelConfig vector");return *v;};
    auto integer=[&](unsigned offset){const auto* v=config.integer(offset);if(!v)throw std::runtime_error("Missing produced LevelConfig integer");return *v;};
    auto text=[&](unsigned offset){const auto* v=config.string(offset);if(!v)throw std::runtime_error("Missing produced LevelConfig string");return *v;};
    try{
        settings.ambient=vector(0x1cc);settings.fogColorRaw=vector(0x1e0);settings.fogDirectionMask=vector(0x1f8);settings.clearColorRaw=vector(0x1ec);
        settings.fogStart=integer(0x1d8);settings.fogEnd=integer(0x1dc);settings.cameraNear=integer(0x294);settings.cameraFar=integer(0x298);
        settings.cameraFile=text(0x24c);settings.cameraAnimationSet=text(0x264);settings.cameraName=text(0x27c);
        settings.lightSet=text(0x2b8);settings.fixedLightSet=text(0x2d0);settings.skybox=text(0x234);
        out=std::move(settings);error.clear();return true;
    }catch(const std::exception& ex){error=ex.what();return false;}
}
bool load_original_level_config(const AssetCatalog& assets,const std::string& uri,
    SourceLevelConfigProviders providers,const std::shared_ptr<dh2::world::CanonicalObjectManagerV1>& manager,
    SourceLevelSettings& settings,std::string& error){
    try {
        if(!manager||!providers.levelOwner||!providers.current){error="Standalone LevelConfig requires SAME manager/Level/current config owners";return false;}
        dh2::loader::XmlDocumentV1 document;
        if(!document.capture_level_buffer(uri,read_content(assets,uri),error))return false;
        const auto source=document.borrow();if(!source.parsed()){error=source.diagnostic().message;return false;}
        std::uint32_t selected=UINT32_MAX;
        for(const auto& root:source.top_level_nodes()){
            if(root.kind!=1||root.value!="Level"||root.element==UINT32_MAX)continue;
            for(const auto element:source.elements().at(root.element).children){
                const auto* type=source.elements().at(element).attribute("gametype");
                if(!type||*type!="LevelConfig")continue;
                if(selected!=UINT32_MAX){error="Standalone LevelConfig helper requires one unambiguous declaration";return false;}
                selected=element;
            }
        }
        if(selected==UINT32_MAX){error="Selected original Level resource has no LevelConfig declaration";return false;}
        const auto current=providers.current;
        SourceLevelConfig config(std::move(providers));
        dh2::loader::CanonicalSourceBindingV1 binding;
        dh2::loader::CanonicalModuleContextV1 context{config.lease()};
        if(!dh2::loader::prepare_canonical_source_binding_v1(source,selected,
            dh2::loader::ObjectEntryRouteV1::level,{},context,binding,error))return false;
        dh2::loader::CanonicalBoundSourceAttemptV1 attempt(std::move(binding));
        if(!attempt.execute(*manager,config.services(),error))return false;
        const auto* factory=attempt.factory_attempt();
        if(!factory||factory->stage()!=dh2::world::CanonicalFactoryStageV1::complete||!current->config){
            error="Selected LevelConfig source factory did not complete/publish";return false;}
        const auto* object=manager->object(factory->handle().key);
        if(!object||object->identity!=current->config->identity()){
            error="Published current LevelConfig differs from SAME source manager receiver";return false;}
        return config.settings(settings,error);
    }catch(const std::exception& ex){error=ex.what();return false;}
}
} // namespace dh::foundation
