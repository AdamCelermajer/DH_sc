#include "session_admitted_destructible_v1.hpp"
#include "world_object_container_state_v1.hpp"

#include <algorithm>

namespace dh::foundation::interactions {
namespace {
bool fail(std::string& error,const char* message){error=message;return false;}
bool same_owner(const std::shared_ptr<const void>& a,
                const std::shared_ptr<const void>& b)noexcept{
    return a&&b&&!a.owner_before(b)&&!b.owner_before(a);
}

bool validate_services(const SessionDestructibleInteractionServicesV1& s,
    const dh2::world::DestructibleContainerRowV16& row,std::string& error){
    if(!s.session_identity||!s.session_lease||!s.actor_context||!s.resolve_actor||
       !s.table||!s.visual_context||!s.visual_asset||!s.has_visual||
       !s.load_object_script||!s.has_script||!s.raise_destroy_quest||
       !s.play_sound_3d||!s.source_on_interact||!s.as_character||
       !s.increment_stat||!s.get_stat||!s.is_local_player||!s.trophy_id||
       !s.unlock_trophy)
        return fail(error,"Authored Destructible requires complete same-session source policy endpoints");
    if(row.visual()!=-1&&(!s.animation_count||!s.bind_retained_visual||
       !s.play_retained_clip||!s.retained_scene_flags))
        return fail(error,"Authored visual Destructible requires retained timeline/count/scene endpoints");
    if(!row.keep_physics20&&!s.detach_physical)
        return fail(error,"Authored Destructible KeepPhysics=false row requires source physical detach endpoint");
    if(!row.script30.empty()&&!s.script_call)
        return fail(error,"Authored scripted Destructible requires source script-call endpoint");
    if(!s.loot.owner||!s.loot.tables||!s.loot.powers||!s.loot.entry.invoke||
       (!s.loot.gameplay_rng&&!s.loot.with_gameplay_rng)||
       (!s.loot.drop_item&&!s.loot.drop_item_with_rng))
        return fail(error,"Authored Destructible requires the existing source loot RNG and drop-store endpoints");
    error.clear();return true;
}
}

SessionAdmittedDestructibleV1::SessionAdmittedDestructibleV1(
    CombatSession& session,ActorId source,std::shared_ptr<const void> lease,
    std::shared_ptr<const void> lifetime,
    std::shared_ptr<SessionContainerRetainedVisualV1> visual,
    std::shared_ptr<const void> providers_owner,
    std::shared_ptr<SessionDestructibleInteractionV1> interaction)
    :session_(&session),source_id_(source),session_lease_(std::move(lease)),
     session_lifetime_lease_(std::move(lifetime)),visual_(std::move(visual)),
     providers_owner_(std::move(providers_owner)),interaction_(std::move(interaction)){}

bool SessionAdmittedDestructibleV1::bind(CombatSession& session,
    const ActorDefinition& definition,
    const SessionSourceObjectAdmissionReceiptV1& admission,
    const AssetCatalog& assets,
    std::shared_ptr<SessionContainerRetainedVisualV1> visual,
    SessionDestructibleInteractionServicesV1 services,
    SessionAdmittedDestructibleProvidersV1 providers,
    std::shared_ptr<SessionAdmittedDestructibleV1>& output,
    std::string& error){
    output.reset();error.clear();
    const auto lease=session.actor_binding_lease().lock();
    const auto lifetime=session.lifetime_lease().lock();
    if(!session.world()||!lease||!lifetime||!visual||!providers.owner||
       !providers.validate_visual_row)
        return fail(error,"Authored Destructible binder requires current Session, retained visual and source providers");
    if(definition.gametype!="DestructibleContainer"||
       definition.stableId==invalid_actor_id||definition.name.empty())
        return fail(error,"Authored Destructible binder requires exact DestructibleContainer definition");
    const auto desc=definition.properties.find("data_desc");
    if(desc==definition.properties.end()||desc->second.empty())
        return fail(error,"Authored Destructible definition has no source data_desc");
    if(!admission.admitted||!admission.online_provider_evaluated||
       admission.cached_roll270!=-2||!admission.prior_admission.admission_owner)
        return fail(error,"Authored Destructible binder requires successful prior source-admission receipt");
    const auto& prior=admission.prior_admission;
    if(prior.session_identity!=&session||!same_owner(prior.session_lease,lease)||
       prior.object_id!=definition.stableId||prior.definition_name!=definition.name||
       prior.data_desc!=desc->second)
        return fail(error,"Destructible admission receipt differs from exact current Session/ObjectId/definition/data_desc");
    if(services.session_identity!=&session||!same_owner(services.session_lease,lease)||
       visual->session_identity()!=&session||!same_owner(visual->session_lease(),lease))
        return fail(error,"Authored Destructible providers/retained visual do not share the current Session lease");
    if(!services.table)
        return fail(error,"Authored Destructible requires the original source table");
    if(services.table->data_id(desc->second)<0)
        return fail(error,"Authored Destructible data_desc is absent from the source table");
    const auto row_id=services.table->data_id(desc->second);
    if(std::count(services.table->names().begin(),services.table->names().end(),desc->second)!=1)
        return fail(error,"Authored Destructible data_desc is ambiguous in the source row-name table");
    const auto* row=services.table->row(row_id);
    if(!row)return fail(error,"Authored Destructible source row is unavailable");
    if(!validate_services(services,*row,error))return false;
    auto* object=session.world()->find_object(definition.stableId);
    if(!object||object->id!=definition.stableId||object->name!=definition.name)
        return fail(error,"Admitted Destructible has no exact current same-world WorldObject");
    SourceContainerObjsFieldsV1 saved;
    if(!read_source_container_objs_v1(*object,saved,error))
        return fail(error,"Admitted Destructible WorldObject has no valid source GameSave component");
    SessionContainerActorBorrowV1 borrow;
    if(!services.resolve_actor(services.actor_context,&session,definition.stableId,borrow,error))return false;
    if(borrow.session_identity!=&session||borrow.actor_id!=definition.stableId||
       !same_owner(borrow.session_lease,lease)||borrow.definition!=&definition||
       borrow.object!=object||!borrow.destructible_state||
       borrow.destructible_state->data_desc!=desc->second||
       borrow.binding_lifecycle==0)
        return fail(error,"Destructible provider did not resolve exact admitted WorldObject/state/definition/lifecycle");
    borrow.destructible_state->state394=saved.state394;
    if(!providers.validate_visual_row(providers.context,definition,*row,*object,error))
        return fail(error,"Destructible row visual differs from the exact admitted source WorldObject");

    const bool had_visual=session.retained_object_visual_borrow(definition.stableId)!=nullptr;
    if(!visual->bind_authored_object(definition,assets,error))return false;
    auto interaction=SessionDestructibleInteractionV1::create(std::move(services),error);
    if(!interaction){
        if(!had_visual)session.unbind_object_visual(definition.stableId);
        return false;
    }
    if(!interaction->initialize(definition.stableId,error)){
        if(!had_visual)session.unbind_object_visual(definition.stableId);
        return false;
    }
    if(session.world()==nullptr||!same_owner(lease,session.actor_binding_lease().lock())){
        if(!had_visual)session.unbind_object_visual(definition.stableId);
        return fail(error,"Session lease changed while binding authored Destructible callbacks");
    }
    output=std::shared_ptr<SessionAdmittedDestructibleV1>(
        new SessionAdmittedDestructibleV1(session,definition.stableId,lease,lifetime,
            std::move(visual),std::move(providers.owner),std::move(interaction)));
    error.clear();return true;
}

bool SessionAdmittedDestructibleV1::current(std::string& error)const{
    if(!session_||!visual_||!providers_owner_||!interaction_||!session_lease_||
       !session_lifetime_lease_)
        return fail(error,"Admitted Destructible binding lost a retained owner");
    if(!same_owner(session_lease_,session_->actor_binding_lease().lock()))
        return fail(error,"Admitted Destructible binding belongs to a stale Session lease");
    const auto lifetime=session_->lifetime_lease().lock();
    if(!lifetime||lifetime.get()!=session_lifetime_lease_.get())
        return fail(error,"Admitted Destructible binding belongs to a stale Session lifetime");
    if(visual_->session_identity()!=session_||!same_owner(session_lease_,visual_->session_lease()))
        return fail(error,"Admitted Destructible lost its same-session retained visual");
    error.clear();return true;
}

bool SessionAdmittedDestructibleV1::interact(ActorId opener,std::string& error){
    return current(error)&&interaction_->interact(source_id_,opener,error);
}
bool SessionAdmittedDestructibleV1::animation_event(const RetainedAnimationEvent& event,
    std::string& error){return current(error)&&interaction_->animation_event(source_id_,event,error);}
bool SessionAdmittedDestructibleV1::animation_finished(std::uint64_t generation,
    bool active,std::string& error){
    return current(error)&&interaction_->animation_finished(source_id_,generation,active,error);
}

} // namespace dh::foundation::interactions
