#include "source_level_config.hpp"
#include "source_level_construction.hpp"
#include "asset_catalog.hpp"
#include "original_camera_config.hpp"
#include "../level-world/character_debug_stdio_v136.hpp"
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
namespace {
void check(bool ok,const std::string& message){if(!ok)throw std::runtime_error(message);}
struct DebugFiles {
    std::filesystem::path root;
    static int open(void* p,const char* name,std::uintptr_t* handle){
        const auto path=static_cast<DebugFiles*>(p)->root/name;*handle=0;
        auto* file=std::fopen(path.string().c_str(),"rb");
        if(!file)return std::filesystem::exists(path)?1:0;
        *handle=reinterpret_cast<std::uintptr_t>(file);return 0;
    }
    static int close(void*,std::uintptr_t handle){return std::fclose(reinterpret_cast<std::FILE*>(handle));}
};
SourceLevelConstructionStep finish(SourceLevelConstruction& walk){
    for(unsigned i=0;i<100000;++i){const auto step=walk.tick();if(step!=SourceLevelConstructionStep::pending)return step;}
    throw std::runtime_error("walk test failed to terminate");
}
void run(const char* repo){
    const auto assetPath=std::filesystem::path(repo)/"port/windows-foundation/assets";
    AssetCatalog assets(assetPath);auto fileOwner=std::make_shared<DebugFiles>();fileOwner->root=assetPath;
    auto debug=std::shared_ptr<dh2::character::DebugSwitches>(dh2_character_debug_create(),dh2_character_debug_destroy);
    dh2::character::DebugFileServices24 files{fileOwner.get(),DebugFiles::open,DebugFiles::close};
    auto actualDebug=source_level_config_debug_switch(debug,fileOwner,files,dh2::character::debug_stdio_services_v136(nullptr));
    unsigned debugCalls=0;
    auto slot=std::make_shared<SourceLevelConfigSlot>();auto levelLease=std::make_shared<int>(1);
    auto map=std::make_shared<dh2::world::CanonicalPropertyMapV1>(dh2::world::CanonicalPropertySourceServicesV1{});
    SourceLevelConfig provider({levelLease,map,slot,[&](const char* key,bool& out,std::string& e){++debugCalls;return actualDebug(key,out,e);},{},{}});
    dh2::world::ModuleRuntimeGlobalsV1 globals;std::int32_t room=-1;float offset[3]{};
    // Explicit offline runtime mode boundary. The recovered leaf reads this
    // value and takes343274; it is not an unconditional successful callback.
    std::uint8_t online=0;SourceLevelConfigNetworkBorrow network{levelLease,&online};
    auto inputs=[&]{SourceLevelConstructionProviders p;p.levelOwner=p.candidateOwner=p.globalsOwner=levelLease;
        p.classesOwner=provider.lease();p.levelModuleId18c=&room;p.levelModuleOffset160=offset;p.moduleGlobals=&globals;
        dh2::world::CanonicalObjectManagerServicesV1 managerServices;managerServices.context=&network;
        managerServices.assign_network_id=[](void* c,auto& object,auto& e){return source_level_config_network_leaf(*static_cast<SourceLevelConfigNetworkBorrow*>(c),object,e);};
        p.manager=std::make_shared<dh2::world::CanonicalObjectManagerV1>(managerServices);
        p.classes=provider.services();p.file.parse_result=[](bool parsed,std::string& e){if(!parsed)e="parser failure";return parsed;};
        p.file.release_load_state=[](std::string&){return true;};return p;};
    SourceLevelConstruction walk;std::string error;
    const auto bytes=assets.read("original-cache/data/scene/001_swamp.mlx");
    check(walk.begin(inputs(),"original-cache/data/scene/001_swamp.mlx",bytes,0,error),error);
    const auto reached=finish(walk);std::cout<<"Reached "<<walk.status().gametype<<": "<<walk.status().error<<'\n';
    check(reached==SourceLevelConstructionStep::failed&&walk.status().gametype=="Module"&&walk.status().attempts==2,
          "actual original LevelConfig completes before honest next Module provider gap");
    check(walk.status().error.find("next source constructor: Module")!=std::string::npos,"next missing callback identifies actual Module class");
    check(slot->config&&debugCalls==4,"actual InitPost makes four original Debug queries and publishes SAME config field");
    SourceLevelSettings settings;check(provider.settings(settings,error),error);
    check(settings.cameraFile=="data/3D/camera/CameraTests.bdae"&&settings.cameraName=="PlayerCamera_Default"&&settings.cameraAnimationSet=="Default","source descriptor/InitPost camera defaults");
    check(settings.cameraNear==900&&settings.cameraFar==4200,"actual selected XML plane overrides preserved");
    check(settings.fogStart==4000&&settings.fogEnd==5800&&settings.fogColorRaw==std::array<float,3>{1,14,0},"actual fog fields exported without guessed normalization");
    check(settings.fogDirectionMask==std::array<float,3>{0,0,1}&&settings.ambient[0]==0.5f/255.0f,"source defaults and original ambient conversion preserved");
    check(globals.next_module_id==0,"missing Module constructor does not fabricate counter advancement");
    dh2::world::CanonicalObjectBorrowV1 unused;
    online=1;check(!source_level_config_network_leaf(network,unused,error),"positive online path requires actual distinct provider");online=0;
    check(!source_level_config_network_leaf({},unused,error),"missing actual Online owner cannot masquerade as offline");
    auto extractedSlot=std::make_shared<SourceLevelConfigSlot>();auto extractedInputs=inputs();
    SourceLevelSettings extracted;
    check(load_original_level_config(assets,"original-cache/data/scene/001_swamp.mlx",
        {levelLease,map,extractedSlot,actualDebug,{},{}},extractedInputs.manager,extracted,error),error);
    check(extractedSlot->config&&extracted.cameraNear==900&&extracted.cameraFar==4200&&extracted.fogColorRaw==settings.fogColorRaw,
          "bounded standalone helper publishes actual source config/settings without executing Module declarations");
    check(globals.next_module_id==0,"standalone config extraction makes no module-ID claim or counter change");
    OriginalGameplayCamera existingCamera;
    check(existingCamera.load(assets,"original-cache/data/scene/001_swamp.mlx",
        std::filesystem::path(repo)/".local-inputs/windows-camera-assets",error),error);
    check(extracted.cameraNear==existingCamera.config().nearPlane&&extracted.cameraFar==existingCamera.config().farPlane&&
          extracted.cameraFile==existingCamera.config().file&&extracted.cameraName==existingCamera.config().node&&
          extracted.cameraAnimationSet==existingCamera.config().animationSet,
          "canonical source LevelConfig settings match existing OriginalGameplayCamera configuration");
    SourceLevelConfig missingDebug({levelLease,map,std::make_shared<SourceLevelConfigSlot>(),{},{},{}});
    auto failedInputs=inputs();failedInputs.classes=missingDebug.services();failedInputs.classesOwner=missingDebug.lease();
    SourceLevelConstruction failure;check(failure.begin(failedInputs,"same-original.mlx",bytes,1,error),error);
    check(finish(failure)==SourceLevelConstructionStep::failed&&failure.status().gametype=="LevelConfig"&&failure.status().factoryPrefix==dh2::world::CanonicalFactoryStageV1::overrides,
          "missing actual Debug fails at reached InitPost after properties/overrides");
    std::cout<<"Next actual-cache callback: "<<walk.status().error<<'\n';
}
}
int main(int argc,char** argv){try{run(argc>1?argv[1]:".");std::cout<<"Actual-cache LevelConfig source composition PASS (Level/parser ownership boundaries declared)\n";return 0;}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
