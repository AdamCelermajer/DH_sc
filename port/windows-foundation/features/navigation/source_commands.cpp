#include "source_commands.hpp"
#include <algorithm>
#include <cmath>

namespace dh::foundation::features {
bool find_source_destination(PlayableActorBodies& bodies,ActorId id,Vec3 destination,
                             SourcePathCommandBindings& bindings,std::string& error){
    const auto* pf=bodies.navigation(id);
    if(!pf){error="Destination actor has no SAME PF receiver";return false;}
    return find_source_destination_for_pf(*pf,destination,bindings,error);
}
bool find_source_destination_for_pf(const dh2::navigation::NavigationObject& actual,Vec3 destination,
                                    SourcePathCommandBindings& bindings,std::string& error){
    const auto* pf=&actual;
    auto* source=bindings.controller;
    if(!pf||!pf->user||!bindings.floor_lease||!bindings.floors||!bindings.floors->sewn||
       !source||!source->controller_lease||!source->controller||!source->path||
       !bindings.result||!bindings.search_enabled||!bindings.publish_command){
        error="FindPath requires SAME initialized actor PF, sewn floor graph, actual controller/path/result and source command/search providers";return false;
    }
    for(float value:{destination.x,destination.y,destination.z})if(!std::isfinite(value)){
        error="Nonfinite source path destination";return false;
    }
    auto& path=*source->path;
    // Only bounded path coordinator state is projected. PFObject remains the
    // sole motion/radius/flags authority and is never copied into a new owner.
    path.route.flags=pf->motion.flags;path.route.radius=pf->radius;
    std::copy_n(pf->motion.position,3,path.position);
    struct Context {SourcePathCommandBindings* bindings;std::string* error;};
    Context context{&bindings,&error};
    const dh2::navigation::FindSourceServicesV1 services{&context,[](void* raw,std::uint32_t* enabled){
        auto& c=*static_cast<Context*>(raw);return c.bindings->search_enabled(*enabled,*c.error)?0:-1;
    }};
    dh2::navigation::FindRequest request{&bindings.floors->route_world,&bindings.floors->collision_world,
        &path,bindings.result,&bindings.floors->route_workspace,{destination.x,destination.y,destination.z},
        bindings.source_search_limit,0,0};
    const int status=dh2_nav_find_path_source_v1(&request,&services);
    if(status){if(error.empty())error="Original FindPath service/capacity failure: "+std::to_string(status);return false;}
    // found=false is the real search result, not successful arrival. Command
    // owner decides its original failed-path state transition/publication.
    if(!bindings.publish_command(*bindings.result,error))return false;
    error.clear();return true;
}
bool update_source_chase(PlayableActorBodies& bodies,ActorId id,OriginalActorPathBindings& controller,
                         dh2::navigation::ControllerResult& result,std::string& error){
    return bodies.update_path(id,controller,result,error);
}
bool update_source_decor_obstacle(const SourceDecorObstacleBinding& source,std::string& error){
    if(!source.owner_lease||!source.identity||!source.actual_pf){error="Decor PF producer requires SAME actual owner/PF receiver";return false;}
    // Preserve constructor user0 early-return before requiring world services.
    if(!source.actual_pf->user){dh2::navigation::ProducerRequest request{};request.object=source.actual_pf;
        if(dh2_nav_update_game_object(&request)){error="Decor constructor-null PF prefix failed";return false;}error.clear();return true;}
    if(source.actual_pf->user!=source.identity||!source.floor_lease||!source.obstacle_lease||!source.floors||
       !source.floors->sewn||!source.obstacles||!source.absolute12c){error="Decor PF update requires SAME initialized owner, actual floors/registry/bounds";return false;}
    dh2::navigation::ProducerFields fields{};fields.type=source.source_class;
    fields.physical_present=source.actual_physical&&source.actual_physical->body;
    fields.physical_radius=fields.physical_present?source.actual_physical->radius:0;
    fields.minimum[0]=source.absolute12c[0];fields.minimum[1]=source.absolute12c[1];
    fields.maximum[0]=source.absolute12c[3];fields.maximum[1]=source.absolute12c[4];
    const dh2::navigation::ProducerRequest request{&source.floors->collision_world,source.obstacles,
        source.actual_pf,source.identity,&fields};
    const auto status=dh2_nav_update_game_object(&request);
    if(status){error="Actual decor PF producer rejected services/capacity: "+std::to_string(status);return false;}
    error.clear();return true;
}
}
