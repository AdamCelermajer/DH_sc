#include "source_root_scopes.hpp"
#include "source_world_objects.hpp"
#include "content_paths.hpp"
#include "../level-world/canonical_point3d_globals_v1.hpp"
#include "../level-world/character_debug_stdio_v136.hpp"
#include <cstdio>
#include <set>
#include <stdexcept>

namespace dh::foundation {
struct SourceRootScopes::Impl {
    struct DebugFiles {
        std::unique_ptr<AssetCatalog> assets;
        static int open(void* raw,const char* name,std::uintptr_t* handle) {
            auto& self=*static_cast<DebugFiles*>(raw);
            if(!name||!handle||!self.assets)return 1;
            *handle=0;
            try {
                const auto path=self.assets->resolve(name);
                auto* file=std::fopen(path.string().c_str(),"rb");
                if(!file)return 1;
                *handle=reinterpret_cast<std::uintptr_t>(file);return 0;
            } catch(const std::invalid_argument&) { return 1; }
              catch(const std::exception&) {
                // Actual absence is allowed by the native Debug file leaf.
                // Existing unreadable/bad paths are errors, not invented defaults.
                std::error_code ec;
                const auto exists=std::filesystem::exists(self.assets->root()/name,ec);
                return ec||exists?1:0;
            }
        }
        static int close(void*,std::uintptr_t handle) {
            if(!handle)return 1;
            return std::fclose(reinterpret_cast<std::FILE*>(handle));
        }
    };
    struct LoadState { std::vector<std::uint8_t> bytes;bool released=false; };
    std::shared_ptr<SourceRootScopeContext> level=std::make_shared<SourceRootScopeContext>();
    std::shared_ptr<dh2::world::ModuleRuntimeGlobalsV1> globals=std::make_shared<dh2::world::ModuleRuntimeGlobalsV1>();
    std::shared_ptr<dh2::world::CanonicalPropertyMapV1> properties;
    std::shared_ptr<SourceLevelConfigSlot> current=std::make_shared<SourceLevelConfigSlot>();
    std::shared_ptr<DebugFiles> files=std::make_shared<DebugFiles>();
    std::shared_ptr<dh2::character::DebugSwitches> debug;
    std::shared_ptr<LoadState> loadState=std::make_shared<LoadState>();
    SourceLevelConfigNetworkBorrow network;
    SourceRootScopeOptions options;
    std::vector<ActorDefinition> definitions;
    std::unique_ptr<SourceLevelConfig> config;
    std::unique_ptr<SourceModuleConstruction> module;
    std::shared_ptr<dh2::world::CanonicalObjectManagerV1> manager;
    SourceLevelConstruction construction;
    bool attempted=false;
    std::function<bool(const char*,bool&,std::string&)> currentDebug;

    bool load(const AssetCatalog& assets,const std::string& uri,
              const std::vector<ActorDefinition>& sourceDefinitions,
              const SourceRootScopeOptions& supplied,std::string& error) {
        if(attempted){error="Source root scope owner cannot replay construction";return false;}
        attempted=true;options=supplied;definitions=sourceDefinitions;
        try {
            loadState->bytes=read_content(assets,uri);
            dh2::world::CanonicalPropertySourceServicesV1 sourceProperties;
            sourceProperties.position_rotation_default=&dh2::world::canonical_vec3_origin_v1();
            properties=std::make_shared<dh2::world::CanonicalPropertyMapV1>(sourceProperties);
            auto debugSwitch=options.debugSwitch;
            if(debugSwitch) {
                if(!options.debugOwner)throw std::runtime_error("Caller Debug provider requires retained owner");
            } else {
                files->assets=std::make_unique<AssetCatalog>(assets.root());
                debug=std::shared_ptr<dh2::character::DebugSwitches>(dh2_character_debug_create(),dh2_character_debug_destroy);
                if(!debug)throw std::runtime_error("Native Debug settings allocation failed");
                dh2::character::DebugFileServices24 sourceFiles{files.get(),DebugFiles::open,DebugFiles::close};
                debugSwitch=source_level_config_debug_switch(debug,files,sourceFiles,
                    dh2::character::debug_stdio_services_v136(nullptr));
            }
            currentDebug=debugSwitch;
            config=std::make_unique<SourceLevelConfig>(SourceLevelConfigProviders{level,properties,current,std::move(debugSwitch),{}, {}});
            SourceModuleConstructionProviders moduleProviders;
            moduleProviders.candidateOwner=level;moduleProviders.globalsOwner=globals;
            moduleProviders.previousOwner=config->lease();moduleProviders.globals=globals.get();
            moduleProviders.properties=properties;moduleProviders.previous=config->services();
            module=std::make_unique<SourceModuleConstruction>(std::move(moduleProviders));
            auto managerServices=options.managerServices;
            const bool customManager=managerServices.context||managerServices.local_player||managerServices.highest_threat||
                managerServices.missing_name_debug||managerServices.destroy_duplicate||managerServices.assign_network_id||managerServices.published;
            if(customManager) {
                if(!options.managerServicesOwner||!managerServices.assign_network_id)
                    throw std::runtime_error("Caller manager providers require retained owner and actual network assignment leaf");
            } else {
                network={level,&level->onlineByte5};managerServices.context=&network;
                managerServices.assign_network_id=[](void* raw,auto& object,auto& e) {
                    return source_level_config_network_leaf(*static_cast<SourceLevelConfigNetworkBorrow*>(raw),object,e);
                };
            }
            manager=std::make_shared<dh2::world::CanonicalObjectManagerV1>(managerServices);
            SourceLevelConstructionProviders p;
            p.levelOwner=p.candidateOwner=level;p.globalsOwner=globals;p.classesOwner=module->lease();
            p.levelModuleId18c=&level->moduleId;p.levelModuleOffset160=level->moduleOffset.data();
            p.moduleGlobals=globals.get();p.manager=manager;p.classes=module->services();
            p.moduleRecord=[this](auto id,auto& record,auto& e){return module->record(id,record,e);};
            p.file=options.file;
            const bool customFile=bool(p.file.parse_result)||bool(p.file.release_load_state)||bool(p.file.before_element)||bool(p.file.observe_walk);
            if(customFile&&!options.fileOwner)throw std::runtime_error("Caller source file providers require retained owner");
            if(!p.file.parse_result)p.file.parse_result=[](bool parsed,std::string& e){if(!parsed)e="Host root XML parser rejected original resource";return parsed;};
            if(!p.file.release_load_state) {
                const auto state=loadState;
                p.file.release_load_state=[state](std::string& e) {
                    if(state->released){e="Host root load-state already released";return false;}
                    std::vector<std::uint8_t>().swap(state->bytes);
                    state->released=true;e.clear();return true;
                };
            }
            p.hostOccurrence=[this](const auto& document,std::uint32_t element,const auto& record,std::string& host,std::string& e) {
                if(!record||!record->receiver){e="Actual source Module record missing";return false;}
                const auto explicitHost=options.hostOccurrenceByElement.find(element);
                if(explicitHost!=options.hostOccurrenceByElement.end()) {
                    host=explicitHost->second;
                    if(host.empty()){e="Explicit source occurrence is empty";return false;}
                    bool found=false;for(const auto& definition:definitions)if(definition.moduleName==host)found=true;
                    if(!found){e="Explicit source occurrence not retained in host definitions";return false;}
                    return true;
                }
                const auto* name=document.elements().at(element).attribute("name");
                if(!name){e="Source Module lacks declaration name";return false;}
                const auto prefix="/"+*name+"#";std::set<std::string> scopes;
                for(const auto& definition:definitions)
                    if(definition.moduleName.rfind(prefix,0)==0&&definition.moduleName.find('/',1)==std::string::npos)
                        scopes.insert(definition.moduleName);
                if(scopes.size()!=1){e="Source Module requires explicit unique declaration occurrence association";return false;}
                host=*scopes.begin();return true;
            };
            if(!construction.begin(std::move(p),uri,loadState->bytes,options.fileOccurrence,error))return false;
            for(unsigned steps=0;steps<1000000;++steps) {
                const auto status=construction.tick();
                if(status==SourceLevelConstructionStep::complete){error.clear();return true;}
                if(status==SourceLevelConstructionStep::failed){error=construction.status().error;return false;}
            }
            error="Source root constructor traversal exceeded host polling budget";return false;
        }catch(const std::exception& ex){error=ex.what();return false;}
    }
};

SourceRootScopes::SourceRootScopes():impl_(std::make_unique<Impl>()){}
SourceRootScopes::~SourceRootScopes()=default;
SourceRootScopes::SourceRootScopes(SourceRootScopes&&) noexcept=default;
SourceRootScopes& SourceRootScopes::operator=(SourceRootScopes&&) noexcept=default;
bool SourceRootScopes::load(const AssetCatalog& assets,const std::string& uri,const std::vector<ActorDefinition>& definitions,
                           std::string& error,const SourceRootScopeOptions& options){return impl_->load(assets,uri,definitions,options,error);}
bool SourceRootScopes::build(const AssetCatalog& assets,const std::string& uri,const std::vector<ActorDefinition>& definitions,
                            SourceWorldObjects& objects,std::string& error,const SourceRootScopeOptions& options){return load(assets,uri,definitions,error,options)&&bind(objects,error);}
bool SourceRootScopes::bind(SourceWorldObjects& objects,std::string& error)const{return impl_->construction.bind(objects,error);}
const SourceModuleConstructionTrace& SourceRootScopes::modules()const noexcept{return impl_->construction.modules();}
const SourceLevelConstructionStatus& SourceRootScopes::status()const noexcept{return impl_->construction.status();}
const std::vector<std::shared_ptr<dh2::world::CanonicalModuleRecordV2>>& SourceRootScopes::module_records()const noexcept{
    static const std::vector<std::shared_ptr<dh2::world::CanonicalModuleRecordV2>> empty;
    return impl_->module?impl_->module->records():empty;
}
std::size_t SourceRootScopes::count()const noexcept{return modules().entries().size();}
const SourceRootScopeContext& SourceRootScopes::context()const noexcept{return *impl_->level;}
bool SourceRootScopes::debug_load(std::string& error)const{
    if(!impl_||!impl_->debug||!impl_->files||!impl_->files->assets){
        error="Source root standalone Debug load capability unavailable";return false;
    }
    const dh2::character::DebugFileServices24 files{impl_->files.get(),Impl::DebugFiles::open,Impl::DebugFiles::close};
    const auto streams=dh2::character::debug_stdio_services_v136(nullptr);
    if(dh2_character_debug_load_stream_v136(impl_->debug.get(),&files,&streams)!=1){
        error="Actual runtime Debug standalone load failed";return false;
    }
    error.clear();return true;
}
bool SourceRootScopes::debug_switch(const char* key,bool& value,std::string& error)const{
    if(!impl_->currentDebug){error="Source root Debug owner unavailable";return false;}
    return impl_->currentDebug(key,value,error);
}
} // namespace dh::foundation
