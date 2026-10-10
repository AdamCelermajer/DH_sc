#include "source_module_trace.hpp"
#include "source_world_objects.hpp"
#include <iostream>
#include <limits>
#include <stdexcept>
#ifdef DH_FOUNDATION_SOURCE_MODULE_CTOR_TEST
#include "../level-world/canonical_level_module_bindings_v2.hpp"
#endif
using namespace dh::foundation;
namespace {
void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
// Fixture receiver fields test transport/binding only. They do not prove a
// real ModuleC1 ran or that a particular level's XML ordinal equals its ID.
struct Fixture {std::int32_t id;std::array<float,3> position;};
SourceModuleReceiverBorrow borrow(const std::shared_ptr<Fixture>& f){
    return {f,reinterpret_cast<std::uintptr_t>(f.get()),[f](auto& id,auto& p,auto& e){id=f->id;p=f->position;e.clear();return true;}};
}
ActorDefinition object(std::uint64_t id,const char* occurrence){
    ActorDefinition d;d.stableId=id;d.name="shared_name";d.gametype="Dummy";d.moduleName=occurrence;
    d.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};return d;
}
void run(){
    SourceWorldObjects world;std::string error;
    check(world.load({object(1,"/module#900"),object(2,"/module#2")},error),"fixture world loads");
    auto a=std::make_shared<Fixture>(Fixture{41,{10,20,30}});
    auto b=std::make_shared<Fixture>(Fixture{58,{40,50,60}});
    SourceModuleConstructionTrace trace;
    SourceModuleOccurrence first{"/module#900","data/scene/selected.mlx",4,17};
    check(!borrow_source_module_record({}).receiver,"null retained source record cannot fabricate receiver");
    check(!trace.observe_record(first,{},std::make_shared<int>(1),error)&&trace.entries().empty(),"missing retained record/declaration fails without changing trace");
    check(trace.observe(first,borrow(a),error),"record actual receiver ID unrelated to host ordinal");
    check(trace.entries()[0].runtimeId==41,"trace does not parse occurrence suffix or allocate IDs");
    check(trace.observe(first,borrow(a),error)&&trace.entries().size()==1,"same exact receipt can be refreshed idempotently");
    SourceModuleOccurrence second{"/module#2","data/scene/selected.mlx",4,18};
    check(trace.observe(second,borrow(b),error),"repeated source URI/element distinguished by retained file occurrence");
    check(trace.bind(world,error),"observed actual IDs bind world object scopes");
    ActorId id=0;bool found=false;
    check(world.named_object("shared_name",41,id,found,error)&&found&&id==1,"first host occurrence resolves in actual source room41");
    check(world.named_object("shared_name",58,id,found,error)&&found&&id==2,"second host occurrence resolves in actual source room58");
    a->position={7,8,9};check(trace.refresh(error)&&trace.entries()[0].position[0]==7,"mutable placement refresh uses same retained receiver");
    a->id=42;check(!trace.refresh(error)&&trace.entries()[0].runtimeId==41,"source runtime ID mutation rejected");
    check(!trace.bind(world,error),"changed original identity cannot be rebound");a->id=41;
    auto conflict=std::make_shared<Fixture>(Fixture{41,{}});
    check(!trace.observe({"/other#0","different.mlx",8,21},borrow(conflict),error),"duplicate actual runtime ID across different receivers rejected");
    conflict->id=99;check(!trace.observe(first,borrow(conflict),error),"same source occurrence cannot be assigned another receiver");
    check(!trace.observe({"","source",0,0},borrow(conflict),error),"missing host identity rejected");
    check(!trace.observe({"unknown","source",0,0},SourceModuleReceiverBorrow{},error),"missing constructed source receiver rejected");
    conflict->position[0]=std::numeric_limits<float>::infinity();
    check(!trace.observe({"unknown","source",0,0},borrow(conflict),error),"nonfinite source placement rejected");
    conflict->position={};conflict->id=-1;
    check(!trace.observe({"unknown","source",0,0},borrow(conflict),error),"unsupported negative runtime ID rejected");
    SourceWorldObjects unbound;check(unbound.load({object(1,"/module#900")},error),"atomic fixture loads");
    SourceModuleConstructionTrace partial;check(partial.observe(first,borrow(a),error),"known source receipt");
    b->id=58;check(partial.observe({"unknown","other.mlx",0,0},borrow(b),error),"other source receipt");
    check(!partial.bind(unbound,error),"unknown host occurrence rejects whole bind");
    check(!unbound.named_object("shared_name",41,id,found,error),"failed batch does not partially bind known earlier occurrence");
}
#ifdef DH_FOUNDATION_SOURCE_MODULE_CTOR_TEST
void sourceRecordTests(){
    // Actual recovered ModuleC1 + GameObject base constructor execute here.
    // Global reset value, declaration lease and uncalled init services are
    // explicit boundaries; no full Level/property/InitPost traversal is claimed.
    dh2::world::ModuleRuntimeGlobalsV1 globals;
    auto candidate=std::make_shared<int>(1);
    auto declarationA=std::make_shared<int>(2),declarationB=std::make_shared<int>(3);
    auto a=std::make_shared<dh2::world::CanonicalModuleRecordV2>();
    auto b=std::make_shared<dh2::world::CanonicalModuleRecordV2>();
    a->declaration=declarationA;b->declaration=declarationB;
    a->receiver=std::make_unique<dh2::world::CanonicalModuleV1>(candidate,a->runtime,globals,
        dh2::world::GameObjectInitializationServicesV1{},dh2::world::ModuleInitServicesV1{});
    b->receiver=std::make_unique<dh2::world::CanonicalModuleV1>(candidate,b->runtime,globals,
        dh2::world::GameObjectInitializationServicesV1{},dh2::world::ModuleInitServicesV1{});
    check(a->receiver->module_id()==0&&b->receiver->module_id()==1&&globals.next_module_id==2,
          "actual recovered Module constructors assign and advance SAME source counter");
    SourceModuleConstructionTrace trace;std::string error;
    SourceModuleOccurrence occurrence{"/actual#900","selected.mlx",8,0};
    check(trace.observe_record(occurrence,a,declarationA,error)&&trace.entries()[0].runtimeId==0,
          "positive retained record bridge reads actual constructed receiver ID");
    check(!trace.observe_record({"/wrong#1","selected.mlx",9,0},b,declarationA,error),
          "different actual source declaration lease rejected");
    check(trace.observe_record({"/actual#9","selected.mlx",9,0},b,declarationB,error),
          "second actual source record bridge succeeds with SAME declaration");
    a->runtime.subobjects.position[0]=22;
    check(trace.refresh(error)&&trace.entries()[0].position[0]==22,
          "actual retained receiver position160 refresh uses alias owner");
    std::weak_ptr<dh2::world::CanonicalModuleRecordV2> lifetime=a;a.reset();
    check(!lifetime.expired()&&trace.refresh(error),"trace alias retains unique Module receiver through actual record");
    SourceWorldObjects world;check(world.load({object(1,"/actual#900"),object(2,"/actual#9")},error)&&trace.bind(world,error),
          "actual constructor IDs bind host occurrences without suffix-derived mapping");
    ActorId id=0;bool found=false;
    check(world.named_object("shared_name",0,id,found,error)&&found&&id==1,
          "live typed record ID0 scopes host name lookup");
    check(world.named_object("shared_name",1,id,found,error)&&found&&id==2,
          "live typed record ID1 scopes host name lookup");
}
#endif
}
int main(){try{run();std::cout<<"Source module trace transport/binding PASS (receiver inputs declared fixtures)\n";
#ifdef DH_FOUNDATION_SOURCE_MODULE_CTOR_TEST
sourceRecordTests();std::cout<<"Actual recovered ModuleC1 retained-record bridge PASS (Level/declaration/init boundaries declared)\n";
#endif
return 0;}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
