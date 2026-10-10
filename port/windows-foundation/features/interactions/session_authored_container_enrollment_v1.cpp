#include "session_authored_container_enrollment_v1.hpp"

#include <algorithm>
#include <cmath>

namespace dh::foundation::interactions {
namespace {
bool fail(std::string& error,const char* message){error=message;return false;}
bool same_owner(const std::shared_ptr<const void>& a,
                const std::shared_ptr<const void>& b)noexcept{
    return a&&b&&!a.owner_before(b)&&!b.owner_before(a);
}
}

bool enroll_session_authored_container_v1(
    CombatSession& session,SessionSourceObjectAdmissionRequestV1 request,
    const AssetCatalog& assets,
    std::shared_ptr<SessionContainerRetainedVisualV1> visual,
    std::shared_ptr<SessionContainerModernDropV1> drop,
    SessionContainerModernOpenablePolicyV1 openable_policy,
    SessionDestructibleInteractionServicesV1 destructible_services,
    SessionAdmittedDestructibleProvidersV1 destructible_providers,
    SessionAuthoredContainerEnrollmentResultV1& output,std::string& error){
    output={};error.clear();
    if(!request.definition||!visual)
        return fail(error,"Authored container enrollment requires its exact definition and retained visual owner");
    const auto* const definition=request.definition;
    const auto id=definition->stableId;
    const auto name=definition->name;
    const auto gametype=definition->gametype;
    const auto desc=definition->properties.find("data_desc");
    const auto data_desc=desc==definition->properties.end()?std::string{}:desc->second;
    if(id==invalid_actor_id||name.empty()||data_desc.empty()||
       (gametype!="OpenableContainer"&&gametype!="DestructibleContainer"))
        return fail(error,"Authored container enrollment requires a decoded OpenableContainer or DestructibleContainer data_desc declaration");
    const auto initial_lease=session.actor_binding_lease().lock();
    auto* const initial_world=session.world();
    if(!initial_lease||!initial_world||visual->session_identity()!=&session||
       !same_owner(initial_lease,visual->session_lease()))
        return fail(error,"Authored container enrollment requires the retained visual owner for this live Session binding");
    if(request.definition!=definition||request.candidate.id!=id||
       request.candidate.name!=name)
        return fail(error,"Authored source candidate identity differs from its decoded Level declaration");
    for(std::size_t axis=0;axis<3;++axis){
        const auto expected=definition->placement[12+axis];
        const auto actual=request.candidate.transform.position[axis];
        if(!std::isfinite(expected)||!std::isfinite(actual)||actual!=expected)
            return fail(error,"Authored source candidate position differs from the decoded Module/GameObject placement");
    }
    if(session.retained_object_visual_borrow(id))
        return fail(error,"Unpublished authored container candidate already has a retained visual");

    if(gametype=="OpenableContainer"){
        if(!drop)return fail(error,"Authored Openable enrollment requires the existing same-session drop/store owner");
        SessionContainerAdmittedOpenableResultV1 admitted;
        if(!admit_and_bind_session_openable_v1(session,std::move(request),assets,
            *visual,std::move(drop),std::move(openable_policy),admitted,error))
            return false;
        output.admission=std::move(admitted.admission);
        output.openable=std::move(admitted.openable);
        if(output.admission.admitted&&!output.openable){
            output={};return fail(error,"Accepted authored Openable candidate did not retain its interaction owner");
        }
        error.clear();return true;
    }

    if(destructible_services.session_identity!=&session||
       !same_owner(destructible_services.session_lease,initial_lease)||
       !destructible_providers.owner||!destructible_providers.validate_visual_row)
        return fail(error,"Authored Destructible enrollment requires typed providers retained by this Session");
    // Preflight the exact row and provider graph before consuming spawn RNG.
    // SessionAdmittedDestructibleV1 repeats these checks after admission.
    if(!destructible_services.table||
       destructible_services.table->data_id(data_desc)<0)
        return fail(error,"Authored Destructible enrollment requires its exact original table row before admission");
    const auto row_id=destructible_services.table->data_id(data_desc);
    if(std::count(destructible_services.table->names().begin(),
                  destructible_services.table->names().end(),data_desc)!=1||
       !destructible_services.table->row(row_id))
        return fail(error,"Authored Destructible data_desc must map to one original source row before admission");
    const auto* row=destructible_services.table->row(row_id);
    const auto& services=destructible_services;
    if(services.session_identity!=&session||!same_owner(services.session_lease,initial_lease)||
       !services.actor_context||!services.resolve_actor||!services.visual_context||
       !services.visual_asset||!services.has_visual||!services.load_object_script||
       !services.has_script||!services.raise_destroy_quest||!services.play_sound_3d||
       !services.source_on_interact||!services.as_character||!services.increment_stat||
       !services.get_stat||!services.is_local_player||!services.trophy_id||
       !services.unlock_trophy||!services.loot.owner||!services.loot.tables||
       !services.loot.powers||!services.loot.entry.invoke||
       (!services.loot.gameplay_rng&&!services.loot.with_gameplay_rng)||
       (!services.loot.drop_item&&!services.loot.drop_item_with_rng))
        return fail(error,"Authored Destructible enrollment requires complete same-session source policy and loot/store endpoints before admission");
    if(row->visual()!=-1&&(!services.animation_count||!services.bind_retained_visual||
       !services.play_retained_clip||!services.retained_scene_flags))
        return fail(error,"Authored Destructible visual row requires retained animation endpoints before admission");
    if(!row->keep_physics20&&!services.detach_physical)
        return fail(error,"Authored Destructible KeepPhysics=false row requires source physical detach before admission");
    if(!row->script30.empty()&&!services.script_call)
        return fail(error,"Authored scripted Destructible row requires source script-call endpoint before admission");
    if(!destructible_providers.validate_visual_row(destructible_providers.context,
        *definition,*row,request.candidate,error))
        return fail(error,"Staged Destructible model does not match its exact authored source row before admission");

    if(!admit_session_source_object_v1(session,std::move(request),output.admission,error))
        return false;
    if(!output.admission.admitted){error.clear();return true;}
    const auto rollback=[&](const char* why){
        if(session.world()==initial_world&&
           same_owner(initial_lease,session.actor_binding_lease().lock())){
            session.unbind_object_visual(id);
            auto* object=initial_world->find_object(id);
            if(object&&object->name==name)initial_world->remove_object(id);
        }
        output={};return fail(error,why);
    };
    const auto desc_after=definition->properties.find("data_desc");
    if(definition->stableId!=id||definition->name!=name||
       definition->gametype!=gametype||desc_after==definition->properties.end()||
       desc_after->second!=data_desc||session.world()!=initial_world||
       !same_owner(initial_lease,session.actor_binding_lease().lock()))
        return rollback("Session binding or authored declaration changed after source admission");
    if(!SessionAdmittedDestructibleV1::bind(session,*definition,output.admission,
        assets,visual,std::move(destructible_services),
        std::move(destructible_providers),output.destructible,error)){
        const auto reason=error.empty()?"Admitted Destructible binding failed":error;
        return rollback(reason.c_str());
    }
    if(session.world()!=initial_world||
       !same_owner(initial_lease,session.actor_binding_lease().lock()))
        return rollback("Session binding changed during authored Destructible enrollment");
    error.clear();return true;
}

} // namespace dh::foundation::interactions
