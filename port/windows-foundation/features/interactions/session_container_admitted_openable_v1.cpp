#include "session_container_admitted_openable_v1.hpp"

namespace dh::foundation::interactions {
namespace {
bool fail(std::string& error,const char* message){error=message;return false;}
bool same_owner(const std::shared_ptr<const void>& a,
                const std::shared_ptr<const void>& b)noexcept{
    return a&&b&&!a.owner_before(b)&&!b.owner_before(a);
}
}

bool admit_and_bind_session_openable_v1(
    CombatSession& session,SessionSourceObjectAdmissionRequestV1 request,
    const AssetCatalog& assets,SessionContainerRetainedVisualV1& visual,
    std::shared_ptr<SessionContainerModernDropV1> drop,
    SessionContainerModernOpenablePolicyV1 policy,
    SessionContainerAdmittedOpenableResultV1& output,std::string& error){
    output={};error.clear();
    if(!request.definition)
        return fail(error,"Admitted Openable composition requires its exact authored definition");

    // Own the identity strings used across callbacks, while retaining the
    // caller's durable level-definition reference for the returned binder.
    // The API contract requires that definition to remain immutable/alive.
    const ActorDefinition* const definition=request.definition;
    const auto desc=definition->properties.find("data_desc");
    const ActorId source_id=definition->stableId;
    const std::string source_name=definition->name;
    const std::string source_gametype=definition->gametype;
    const std::string source_data_desc=desc==definition->properties.end()?std::string{}:desc->second;
    if(source_gametype!="OpenableContainer"||source_id==invalid_actor_id||
       source_name.empty()||source_data_desc.empty())
        return fail(error,"Admitted Openable composition requires an exact authored Openable definition/data_desc");
    if(!session.world()||!session.actor_binding_lease().lock())
        return fail(error,"Admitted Openable composition requires a live current Session");
    auto initial_world=session.world();
    auto initial_lease=session.actor_binding_lease().lock();
    if(visual.session_identity()!=&session||!same_owner(visual.session_lease(),initial_lease))
        return fail(error,"Admitted Openable visual owner is not bound to this current Session lease");
    if(session.retained_object_visual_borrow(source_id))
        return fail(error,"Unpublished Openable candidate already has retained visual state");
    if(!drop)
        return fail(error,"Admitted Openable composition requires the existing same-session world-item drop owner");
    if(!policy.authored_table)
        return fail(error,"Admitted Openable composition requires the original Openable table");
    if(!policy.resolve_other_actor)
        return fail(error,"Admitted Openable composition requires a typed same-session opener resolver");
    if(policy.source.bind_timeline_callbacks||policy.source.play_animation||
       policy.source.scene_flags||policy.source.drop_loot_table)
        return fail(error,"Admitted Openable source endpoints must remain owned by the retained visual/drop composer");
    policy.prior_admission={};
    policy.source_fields.data_desc=source_data_desc;

    // Ask the existing drop owner for its exact same-session store/RNG bridge
    // before admission. This is a side-effect-free provider readiness check;
    // it does not draw, select, or publish any loot.
    SourceContainerLootServicesV1 loot_services;
    if(!drop->services(loot_services,error))return false;
    if(!loot_services.tables||!loot_services.with_gameplay_rng||
       !loot_services.drop_item_with_rng)
        return fail(error,"Same-session Openable drop owner returned incomplete source loot endpoints");

    const auto rollback=[&](const char* reason){
        if(session.world()==initial_world&&
           same_owner(initial_lease,session.actor_binding_lease().lock())){
            session.unbind_object_visual(source_id);
            initial_world->remove_object(source_id);
        }
        output={};return fail(error,reason);
    };

    if(!admit_session_source_object_v1(session,std::move(request),output.admission,error)){
        output={};return false;
    }
    if(!output.admission.admitted){
        if(session.world()!=initial_world||
           !same_owner(initial_lease,session.actor_binding_lease().lock())){
            output={};return fail(error,"Session binding changed after a rejected source candidate");
        }
        error.clear();return true;
    }
    const auto desc_after=definition->properties.find("data_desc");
    if(definition->stableId!=source_id||definition->name!=source_name||
       definition->gametype!=source_gametype||desc_after==definition->properties.end()||
       desc_after->second!=source_data_desc)
        return rollback("Authored source definition identity changed during admission callbacks");
    if(session.world()!=initial_world||
       !same_owner(initial_lease,session.actor_binding_lease().lock()))
        return rollback("Session binding changed after source admission");

    policy.prior_admission=output.admission.prior_admission;
    if(!SessionContainerModernOpenableV1::create(session,*definition,assets,visual,
        std::move(drop),std::move(policy),output.openable,error)){
        const auto reason=error.empty()?"Row-backed Openable binder creation failed after admission":error;
        return rollback(reason.c_str());
    }
    if(!output.openable||!output.openable->initialize_admitted(error)){
        const auto reason=error.empty()?"Row-backed Openable initialization failed after admission":error;
        return rollback(reason.c_str());
    }
    if(session.world()!=initial_world||
       !same_owner(initial_lease,session.actor_binding_lease().lock()))
        return rollback("Session binding changed during admitted Openable initialization");
    error.clear();return true;
}

} // namespace dh::foundation::interactions
