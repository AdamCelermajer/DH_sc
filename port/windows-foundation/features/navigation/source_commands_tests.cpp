#include "source_commands.hpp"
#include "../../source_module_floors.hpp"
#include "../../source_root_scopes.hpp"
#include "../../source_world_objects.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
namespace {
void check(bool ok,const std::string& error){if(!ok)throw std::runtime_error(error);}
}
int main(int argc,char** argv){try{
    check(argc==2,"Actual original asset root required");AssetCatalog assets(argv[1]);std::string error;
    const std::string manifest="original-cache/data/scene/001_swamp.mlx";
    std::vector<ActorDefinition> definitions;check(load_actor_definitions(assets,manifest,definitions,error),error);
    SourceWorldObjects objects;SourceRootScopes scopes;
    check(objects.load(definitions,error)&&scopes.load(assets,manifest,definitions,error)&&scopes.bind(objects,error),error);
    auto world=std::make_shared<dh2::floors::World>();SourceModuleFloors floors(world);
    for(const auto& trace:scopes.modules().entries()){
        SourceModuleFloorRequest request;request.trace=trace;request.pose_phase=SourceModulePosePhase::constructor_degrees;
        request.admitted=true; // Explicit asset fixture admission, not campaign success.
        for(const auto& record:scopes.module_records())if(record->receiver->base().identity()==trace.receiver.identity)request.record=record;
        std::shared_ptr<SourceModuleFloorRoom> room;check(floors.load(assets,request,room,error),error);
    }
    check(floors.post_load(error),error);
    check(!world->records.empty(),"No actual original floor records");
    const auto& t=world->records.front()->triangles.front();
    std::array<float,3> position{};for(unsigned k=0;k<3;++k)position[k]=(t.points[0][k]+t.points[1][k]+t.points[2][k])/3;
    // Native fixture owns one receiver; API borrows it and never allocates a
    // second navigation object. Runtime wrapper borrows bodies.navigation(id).
    dh2::navigation::NavigationObject pf{};check(!dh2_nav_object_defaults(&pf),"Defaults failed");
    const dh2::navigation::ObjectInitRequest init{&world->collision_world,&pf,99,{position[0],position[1],position[2]},0,0,0};
    check(!dh2_nav_init_object(&init)&&pf.motion.floor!=UINT32_MAX,"Actual source floor center init failed");
    const std::size_t capacity=std::size_t(world->graph.node_count)+1;
    std::vector<dh2::navigation::PathSegment> segments(capacity);std::vector<unsigned> ids(capacity);
    dh2::navigation::PathObject path{};path.segments=segments.data();path.capacity=capacity;
    dh2::navigation::PathController controller{};OriginalActorPathBindings actual;actual.controller_lease=world;actual.controller=&controller;actual.path=&path;
    dh2::navigation::RouteResult result{};result.search.path=ids.data();result.search.path_capacity=capacity;
    features::SourcePathCommandBindings bindings;bindings.floor_lease=world;bindings.floors=world.get();bindings.controller=&actual;bindings.result=&result;
    unsigned policies=0,published=0;bindings.source_search_limit=world->graph.node_count+1;
    bindings.search_enabled=[&](auto& enabled,auto&){++policies;enabled=1;return true;};
    bindings.publish_command=[&](const auto&,auto&){++published;return true;}; // Explicit publication fixture.
    const Vec3 target{position[0],position[1],position[2]};
    check(features::find_source_destination_for_pf(pf,target,bindings,error),error);
    check(policies==1&&published==1&&path.route.flags==pf.motion.flags&&path.route.radius==pf.radius,"Sole PF source/policy publication differs");
    check(result.found&&path.count,"Actual source floor direct route not found");
    bindings.search_enabled=[&](auto&,auto& e){++policies;e="deliberate reached source Debug failure";return false;};
    const Vec3 next{position[0]+1,position[1],position[2]};
    check(!features::find_source_destination_for_pf(pf,next,bindings,error)&&path.count==0&&path.target[0]==next.x&&published==1,"Reached search failure did not preserve DropPath/store-target prefix");
    features::SourceDecorObstacleBinding decor;decor.owner_lease=world;decor.identity=123;dh2::navigation::NavigationObject nullPf{};check(!dh2_nav_object_defaults(&nullPf),"Null defaults failed");decor.actual_pf=&nullPf;
    check(features::update_source_decor_obstacle(decor,error),error);
    nullPf.user=123;check(!features::update_source_decor_obstacle(decor,error),"Unbound reached decor world silently succeeded");
    std::array<dh2::navigation::ObstacleEntry,8> entries{};std::array<unsigned,8> keys{};
    dh2::navigation::ObstacleRegistry registry{entries.data(),0,8,keys.data(),0,8};
    std::array<float,6> bounds{position[0]-10,position[1]-20,position[2],position[0]+10,position[1]+20,position[2]+30};
    decor.actual_pf=&pf;decor.identity=99;decor.floor_lease=decor.obstacle_lease=world;
    decor.floors=world.get();decor.obstacles=&registry;decor.absolute12c=bounds.data();
    check(features::update_source_decor_obstacle(decor,error),error);
    check(pf.radius==20&&registry.count==0,"Generic source Decor fabricated a PF obstacle or wrong bounds radius");
    std::cout<<"PASS actualSourceModules="<<floors.rooms().size()<<" realFloorRoute=true solePFProjection=true reachedPolicyFailurePrefix=true decorNullPrefix=true missingDecorServicesRejected=true\n";
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
