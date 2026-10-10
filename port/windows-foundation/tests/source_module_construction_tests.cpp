#include "source_module_construction.hpp"
#include "source_level_config.hpp"
#include "source_level_construction.hpp"
#include "source_world_objects.hpp"
#include "asset_catalog.hpp"
#include "../level-world/canonical_point3d_globals_v1.hpp"
#include "../level-world/character_debug_stdio_v136.hpp"
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <set>
#include <stdexcept>
using namespace dh::foundation;
namespace {
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
struct DebugFiles {
    std::filesystem::path root;
    static int open(void* p,const char* name,std::uintptr_t* handle){
        auto path=static_cast<DebugFiles*>(p)->root/name;*handle=0;auto* file=std::fopen(path.string().c_str(),"rb");
        if(!file)return std::filesystem::exists(path)?1:0;*handle=reinterpret_cast<std::uintptr_t>(file);return 0;
    }
    static int close(void*,std::uintptr_t handle){return std::fclose(reinterpret_cast<std::FILE*>(handle));}
};
SourceLevelConstructionStep finish(SourceLevelConstruction& construction){
    for(unsigned i=0;i<100000;++i){const auto step=construction.tick();if(step!=SourceLevelConstructionStep::pending)return step;}
    throw std::runtime_error("source constructor polling failed to complete");
}
void run(const char* repo){
    const auto root=std::filesystem::path(repo)/"port/windows-foundation/assets";AssetCatalog assets(root);
    const std::string uri="original-cache/data/scene/001_swamp.mlx";
    std::vector<ActorDefinition> definitions;std::string error;
    check(load_actor_definitions(assets,uri,definitions,error),error);
    auto level=std::make_shared<int>(1);auto globals=std::make_shared<dh2::world::ModuleRuntimeGlobalsV1>();
    dh2::world::CanonicalPropertySourceServicesV1 propertySources;
    propertySources.position_rotation_default=&dh2::world::canonical_vec3_origin_v1();
    auto map=std::make_shared<dh2::world::CanonicalPropertyMapV1>(propertySources);
    auto slot=std::make_shared<SourceLevelConfigSlot>();auto filesOwner=std::make_shared<DebugFiles>();filesOwner->root=root;
    auto debug=std::shared_ptr<dh2::character::DebugSwitches>(dh2_character_debug_create(),dh2_character_debug_destroy);
    dh2::character::DebugFileServices24 files{filesOwner.get(),DebugFiles::open,DebugFiles::close};
    SourceLevelConfig config({level,map,slot,source_level_config_debug_switch(debug,filesOwner,files,
        dh2::character::debug_stdio_services_v136(nullptr)),{}, {}});
    auto conditionCache=std::make_shared<AssetCatalog>(root/"original-cache");
    dh2::world::NativeConditionCacheInputsV69 conditionInputs;conditionInputs.actual_cache=conditionCache;
    conditionInputs.read=[conditionCache](const char* name,bool& found,auto& bytes,std::string& e){
        found=std::filesystem::is_regular_file(conditionCache->root()/name);
        if(!found){bytes.clear();e.clear();return true;}
        try{bytes=conditionCache->read(name);e.clear();return true;}catch(const std::exception& ex){e=ex.what();return false;}
    };
    std::shared_ptr<const dh2::world::NativeConditionTableV69> conditionTable;
    check(dh2::world::read_native_condition_table_v69(conditionInputs,conditionTable,error),error);
    std::shared_ptr<dh2::world::NativeConditionRuntimeV69> conditions;
    check(dh2::world::NativeConditionRuntimeV69::create(conditionTable,{},conditions,error),error);
    check(!conditionTable->rows().empty(),"actual cache condition table has decoded rows");
    std::uint8_t online=0;SourceLevelConfigNetworkBorrow network{level,&online};
    auto applicationRandom=std::make_shared<dh2::world::ApplicationSpawnRandomOwnerV4>();
    auto spawnApplication=std::make_shared<SourceModuleSpawnApplication>();
    spawnApplication->applicationOwner=applicationRandom;spawnApplication->random=applicationRandom.get();spawnApplication->onlineByte5=&online;
    SourceModuleConstruction module({level,globals,config.lease(),globals.get(),map,config.services(),{}, {},conditions,spawnApplication});
    dh2::world::CanonicalObjectManagerServicesV1 managerServices;managerServices.context=&network;
    managerServices.assign_network_id=[](void* p,auto& object,auto& e){return source_level_config_network_leaf(*static_cast<SourceLevelConfigNetworkBorrow*>(p),object,e);};
    auto manager=std::make_shared<dh2::world::CanonicalObjectManagerV1>(managerServices);
    spawnApplication->manager=manager;
    std::int32_t room=-1;float offset[3]{};SourceLevelConstructionProviders providers;
    providers.levelOwner=providers.candidateOwner=level;providers.globalsOwner=globals;providers.classesOwner=module.lease();
    providers.levelModuleId18c=&room;providers.levelModuleOffset160=offset;providers.moduleGlobals=globals.get();providers.manager=manager;
    providers.classes=module.services();providers.moduleRecord=[&](auto id,auto& record,auto& e){return module.record(id,record,e);};
    providers.file.parse_result=[](bool ok,std::string& e){if(!ok)e="actual source parse failed";return ok;};
    providers.file.release_load_state=[](std::string&){return true;};
    // Host child-scope association uses the EXISTING importer identity format,
    // then returns the exact stored opaque scope. No suffix is parsed as an ID.
    providers.hostOccurrence=[&](const auto& document,std::uint32_t element,const auto& record,std::string& host,std::string& e){
        const auto* name=document.elements().at(element).attribute("name");if(!name){e="source Module has no name";return false;}
        std::set<std::string> scopes;const auto prefix="/"+*name+"#";
        for(const auto& d:definitions)if(d.moduleName.rfind(prefix,0)==0&&d.moduleName.find('/',1)==std::string::npos)scopes.insert(d.moduleName);
        if(scopes.size()!=1){e="actual source declaration needs unique explicit host child occurrence";return false;}
        host=*scopes.begin();check(record->receiver!=nullptr,"host association actual receiver missing");return true;
    };
    SourceLevelConstruction construction;
    check(construction.begin(providers,uri,assets.read(uri),0,error),error);
    const auto reached=finish(construction);
    if(reached!=SourceLevelConstructionStep::complete)std::cerr<<construction.status().gametype<<": "<<construction.status().error<<'\n';
    check(reached==SourceLevelConstructionStep::complete,"actual unfiltered cached root constructor traversal completes");
    check(construction.status().attempts==10&&construction.status().modules==9&&globals->next_module_id==9,"one actual config plus nine source Module constructors");
    check(slot->config!=nullptr,"same existing LevelConfig owner preserved");
    SourceWorldObjects objects;check(objects.load(definitions,error)&&construction.bind(objects,error),error);
    for(const auto& record:module.records()){
        check(record->receiver!=nullptr,"source record constructed");auto& receiver=*record->receiver;
        const auto& runtime=receiver.base().runtime();const auto id=receiver.module_id();
        check(id>=0&&id<9,"actual Module ID supported");
        for(unsigned axis=0;axis<3;++axis){
            check(runtime.controller.destination[axis]==runtime.subobjects.position[axis],"whole source SetPosition updates destination");
            check(runtime.subobjects.absolute_bounds[axis]==runtime.subobjects.position[axis]-100&&
                  runtime.subobjects.absolute_bounds[axis+3]==runtime.subobjects.position[axis]+100,"whole source SetPosition recomputes actual inherited AABB");
        }
        const auto* sourceName=receiver.base().string(0x30);check(sourceName!=nullptr,"source name produced");
        std::cout<<"actual Module "<<*sourceName<<" id="<<id<<" position="<<runtime.subobjects.position[0]<<','<<runtime.subobjects.position[1]<<','<<runtime.subobjects.position[2]<<'\n';
    }
    ActorId foundId=0;bool found=false;
    check(objects.named_object("_prim_TriggerZone_LizManIntro",module.records().at(1)->receiver->module_id(),foundId,found,error)&&found,
          "actual constructed source room admits original introductory trigger definition");
    check(objects.named_character("_prim_Monster_LizManIntro1",module.records().at(1)->receiver->module_id(),foundId,found,error)&&found,
          "actual constructed source room admits original scripted spawn definition");
    // Synthetic positive pointer tests the reached-backend boundary only: no
    // actual physical receiver/body is manufactured by the construction owner.
    auto& base=module.records().front()->receiver->base();*base.pointer(0x2dc)=123;
    const auto original=module.records().front()->receiver->canonical(module.records().front());
    const std::array<float,3> moved{12,13,14};const auto service=module.services();
    check(!service.set_position(service.context,original,moved,true,error)&&error.find("physical2dc")!=std::string::npos,
          "positive source physical pointer requires reached real backend instead of no-op");
    check(base.runtime().subobjects.position[0]==12&&base.runtime().controller.destination[0]==0,
          "source SetPosition failure preserves position/bounds prefix before destination");*base.pointer(0x2dc)=0;
    auto blockProviders=providers;
    SourceModuleConstruction noArena({level,globals,config.lease(),globals.get(),map,config.services(),{}, {},{}, {}});
    blockProviders.classes=noArena.services();blockProviders.classesOwner=noArena.lease();
    blockProviders.moduleRecord=[&](auto id,auto& record,auto& e){return noArena.record(id,record,e);};
    blockProviders.hostOccurrence=[](const auto&,auto,const auto&,std::string& host,std::string&){host="explicit-block-host-scope";return true;};
    const std::string blockXml="<Level><GameObject gametype='Block' name='block' position='1,2,3'/></Level>";
    SourceLevelConstruction block;
    check(block.begin(blockProviders,"synthetic-block-source.mlx",{blockXml.begin(),blockXml.end()},1,error),error);
    check(finish(block)==SourceLevelConstructionStep::complete&&block.modules().entries()[0].runtimeId==9&&globals->next_module_id==10,
          "Block alias executes same real constructor on same counter; no separate alias authority");
    check(block.modules().entries()[0].position==std::array<float,3>{1,2,3},"Block actual defaults/overrides/SetPosition stores");
    // InitPost is deliberately not invented by successful construction.
    const auto& first=module.records().front();auto owned=first->receiver->canonical(first);
    const auto services=module.services();
    for(const auto& actual:module.records()){
        auto object=actual->receiver->canonical(actual);
        std::uintptr_t character=1;check(object.as_character(object.context,character,error)&&character==0,
              "actual Module inherited AsChar returns NULL, never player shortcut");
        check(*actual->receiver->base().integer(0x270)==-1&&*actual->receiver->base().integer(0x274)==100,
              "actual cached/default Module source spawn fields start unevaluated/100");
        check(actual->receiver->base().string(0x90)->empty()&&actual->receiver->base().string(0xb4)->empty(),
              "cached/default Module embedded condition names are genuinely empty");
        check(!services.init_post(services.context,object,error)&&error=="Required original GameObject Device::IsHighPerformance provider",
              "actual condition and source probability default advance each Module to exact Device provider");
        check(*actual->receiver->base().integer(0x270)==-2,"real accepted source roll memoizes -2");
        check(module.clear_conditions(object.identity,error),"source NULL compiled clear leaf");
    }
    check(applicationRandom->channel(0).calls==9&&applicationRandom->channel(1).calls==0,
          "all nine actual Modules consume SAME application offline channel exactly once");
    check(manager->source_marked_for_deletion_v89().empty(),"default100 source probabilities delete no Module");
    auto& noArenaBase=noArena.records().front()->receiver->base();*noArenaBase.string(0x90)="required-named-condition";
    auto noArenaServices=noArena.services();auto noArenaObject=noArena.records().front()->receiver->canonical(noArena.records().front());
    check(!noArenaServices.init_post(noArenaServices.context,noArenaObject,error)&&error=="Required actual Arrays::Conditions snapshot",
          "named source field fails without genuine table instead of silent success");
    const dh2::world::NativeConditionRowV69* named=nullptr;
    for(const auto& row:conditionTable->rows()){
        for(int i=0;i<row.count4;++i)if(row.stubs8[i].op4!=4&&row.stubs8[i].op4!=5){named=&row;break;}
        if(named)break;
    }
    check(named!=nullptr,"actual cache contains condition requiring real runtime state");
    *base.string(0x90)=named->name;*base.string(0xb4)=named->name;
    check(!services.init_post(services.context,owned,error)&&error=="Required original GameObject Device::IsHighPerformance provider",
          "two actual named cache rows compile before exact next initialization failure");
    const auto firstCondition=*base.pointer(0xa8),secondCondition=*base.pointer(0xcc);
    check(firstCondition&&secondCondition&&firstCondition!=secondCondition,"actual ConditionList allocation identities, separate embedded receivers");
    bool truth=false;
    error.clear();
    check(!conditions->evaluate(firstCondition,truth,error)&&error.find("Required actual original condition service:")==0,
          "named original row evaluation refuses absent actual player/level transport");
    check(module.clear_conditions(owned.identity,error)&&!*base.pointer(0xa8)&&!*base.pointer(0xcc),
          "ordered positive clear destroys both actual lists and clears SAME source cells");
    check(!conditions->evaluate(firstCondition,truth,error),"cleared actual list no longer admitted by arena");
    check(module.clear_conditions(owned.identity,error),"repeat clear takes exact NULL leaves");
    *base.string(0x90)="not-in-actual-table";base.string(0xb4)->clear();
    check(!services.init_post(services.context,owned,error)&&error=="Required original GameObject Device::IsHighPerformance provider"&&!*base.pointer(0xa8),
          "genuine table name miss leaves source pointer unchanged");
    std::cout<<"Actual condition rows="<<conditionTable->rows().size()<<" positive Init/Clear row="<<named->name<<'\n';
    *base.string(0x90)="Invalid";
    check(!services.init_post(services.context,owned,error)&&error=="Required original GameObject Device::IsHighPerformance provider",
          "source Invalid-name no-call branch advances honestly");
    base.string(0x90)->clear();
    check(!module.clear_conditions(1,error)&&error.find("not retained")!=std::string::npos,
          "condition release rejects foreign receiver identity");
    check(!services.init_post(services.context,owned,error),"exact next initialization gap retained");
    std::cout<<"Next initialization provider: "<<error<<'\n';
    check(applicationRandom->channel(0).calls==9,"cached accepted rolls survive later missing Device service without rerolling");
    // Explicit failure-fixture mutation of SAME source fields, distinct from
    // the original root defaults verified above. Do not replay candidate InitPost.
    auto& rejected=module.records().at(2)->receiver->base();const auto rejectedIdentity=rejected.identity();
    *rejected.integer(0x270)=-1;*rejected.integer(0x274)=0;
    std::int32_t roll=0,probability=0;
    spawnApplication->manager.reset();error.clear();
    check(!module.check_spawn_probability(rejectedIdentity,roll,probability,error)&&
          error=="Required SAME actual Module ObjectManager MarkForDeletion",
          "failed probability reaches missing actual manager after genuine lifecycle prefix");
    check(applicationRandom->channel(0).calls==10&&*rejected.integer(0x270)>=0&&
          *rejected.byte(0x80)==0&&*rejected.byte(0x81)==1&&*rejected.byte(0x82)==0,
          "failed roll preserves cached270, visibility80, Delete disabled81 and continuation82 stores");
    spawnApplication->manager=manager;
    check(module.check_spawn_probability(rejectedIdentity,roll,probability,error)&&roll>=0&&probability==0&&
          applicationRandom->channel(0).calls==10&&manager->source_marked_for_deletion_v89().empty(),
          "source cached failed roll returns immediately; no invented retry of missed manager operation");
    *rejected.integer(0x270)=-1;
    check(module.check_spawn_probability(rejectedIdentity,roll,probability,error)&&roll>=0&&probability==0&&
          applicationRandom->channel(0).calls==11&&manager->source_marked_for_deletion_v89().size()==1&&
          manager->source_marked_for_deletion_v89().front()==rejectedIdentity,
          "fresh explicit failure fixture marks exact registered Module identity in actual source manager list");
    check(module.check_spawn_probability(rejectedIdentity,roll,probability,error)&&applicationRandom->channel(0).calls==11&&
          manager->source_marked_for_deletion_v89().size()==1,"cached failed result neither rerolls nor duplicates actual deletion list");
    *rejected.integer(0x270)=-1;spawnApplication->onlineByte5=nullptr;
    check(!module.check_spawn_probability(rejectedIdentity,roll,probability,error)&&
          error=="Required actual application online byte5 cell"&&*rejected.integer(0x270)==-1&&applicationRandom->channel(0).calls==11,
          "missing exact online cell fails before random/cache writes");
    spawnApplication->onlineByte5=&online;
    // A synthetic positive visual cell exercises refusal only; no visual
    // receiver is created or admitted by this fixture.
    *rejected.pointer(0x2d8)=123;*rejected.integer(0x270)=-1;*rejected.byte(0x81)=0;*rejected.byte(0x82)=0;
    check(!module.check_spawn_probability(rejectedIdentity,roll,probability,error)&&
          error=="Required SAME Module visual SyncVisibility"&&applicationRandom->channel(0).calls==12&&
          *rejected.byte(0x80)==0&&*rejected.byte(0x81)==0&&*rejected.byte(0x82)==0,
          "positive visual requires actual SyncVisibility and preserves failure before source Delete");
    *rejected.pointer(0x2d8)=0;
    auto wrongManager=std::make_shared<dh2::world::CanonicalObjectManagerV1>(managerServices);
    spawnApplication->manager=wrongManager;*rejected.integer(0x270)=-1;
    check(!module.check_spawn_probability(rejectedIdentity,roll,probability,error)&&
          error=="Required SAME manager registration for Module MarkForDeletion"&&wrongManager->source_marked_for_deletion_v89().empty(),
          "foreign manager cannot receive an actual Module deletion identity");
    spawnApplication->manager=manager;
    // Explicit source scalar fixtures select the original alternate channel;
    // ownerfc is an integer field, not a fabricated receiver identity.
    online=1;*rejected.integer(0x108)=0;*rejected.pointer(0xfc)=1;
    *rejected.integer(0x270)=-1;*rejected.integer(0x274)=100;
    check(module.check_spawn_probability(rejectedIdentity,roll,probability,error)&&roll==-2&&
          applicationRandom->channel(1).calls==1&&applicationRandom->channel(0).calls==13,
          "online network identity with nonzero actual owner scalar uses same application alternate channel");
    online=0;*rejected.integer(0x108)=-1;*rejected.pointer(0xfc)=0;
    std::cout<<"Shared application spawn calls="<<applicationRandom->channel(0).calls<<" alternate="<<applicationRandom->channel(1).calls<<'\n';
}
}
int main(int argc,char** argv){try{run(argc>1?argv[1]:".");std::cout<<"Actual-cache Module constructors/properties/positions/room receipts PASS (startup/parser boundaries declared)\n";return 0;}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
