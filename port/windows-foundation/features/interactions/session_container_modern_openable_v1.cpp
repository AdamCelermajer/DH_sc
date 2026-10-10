#include "session_container_modern_openable_v1.hpp"
#include "world_object_container_state_v1.hpp"

#include <cmath>

namespace dh::foundation::interactions {
namespace {
bool fail(std::string& error,const char* message){error=message;return false;}
bool same_owner(const std::shared_ptr<const void>& a,
                const std::shared_ptr<const void>& b) noexcept {
    return a&&b&&!a.owner_before(b)&&!b.owner_before(a);
}
}

SessionContainerModernOpenableV1::SessionContainerModernOpenableV1(
    CombatSession& session,const ActorDefinition& definition,
    SessionContainerRetainedVisualV1& visual,
    std::shared_ptr<SessionContainerModernDropV1> drop,
    SessionContainerModernOpenablePolicyV1 policy)
    :session_(&session),definition_(&definition),visual_(&visual),
     drop_(std::move(drop)),policy_(std::move(policy)),
     lease_(session.actor_binding_lease().lock()),source_id_(definition.stableId),
     fields_(policy_.source_fields) {}

bool SessionContainerModernOpenableV1::create(
    CombatSession& session,const ActorDefinition& definition,
    const AssetCatalog& assets,
    SessionContainerRetainedVisualV1& visual,
    std::shared_ptr<SessionContainerModernDropV1> drop,
    SessionContainerModernOpenablePolicyV1 policy,
    std::shared_ptr<SessionContainerModernOpenableV1>& output,
    std::string& error) {
    output.reset();error.clear();
    if(!session.world()||!session.actor_binding_lease().lock())
        return fail(error,"Modern Openable requires a live same-session WorldObject lease");
    if(!definition.stableId||definition.stableId==invalid_actor_id||definition.name.empty())
        return fail(error,"Modern Openable requires exact authored ObjectId and source name");
    const auto desc=definition.properties.find("data_desc");
    if(desc==definition.properties.end()||desc->second.empty())
        return fail(error,"Modern Openable definition has no authored data_desc");
    if(policy.source_fields.data_desc!=desc->second)
        return fail(error,"Modern Openable requires complete source fields matching authored data_desc");
    auto lease=session.actor_binding_lease().lock();
    if(!same_owner(lease,visual.session_lease())||visual.session_identity()!=&session)
        return fail(error,"Modern Openable visual owner is not bound to this current Session lease");
    if(!drop)return fail(error,"Modern Openable requires the existing same-session drop owner");
    if(!policy.resolve_other_actor)
        return fail(error,"Modern Openable requires a typed same-session opener/actor resolver");
    if(policy.source.bind_timeline_callbacks||policy.source.play_animation||
       policy.source.scene_flags||policy.source.drop_loot_table)
        return fail(error,"Modern Openable policy must leave retained-visual/drop endpoints for the composer");
    auto* object=session.world()->find_object(definition.stableId);
    if(!object||object->id!=definition.stableId||object->name!=definition.name||
       object->visual.model.empty())
        return fail(error,"Modern Openable definition does not match a current authored WorldObject");
    for(float coordinate:object->transform.position)
        if(!std::isfinite(coordinate))return fail(error,"Modern Openable WorldObject position is not finite");

    // Modern enrollment owns the original ObjectBase admission gates. When a
    // caller supplies the explicit receipt and source table, derive the
    // receiver fields from the authored row and original constructor defaults;
    // never rerun CheckSpawnProbability from this interaction binder.
    if(policy.authored_table){
        const auto& admission=policy.prior_admission;
        if(admission.session_identity!=&session||
           !same_owner(admission.session_lease,lease)||!admission.admission_owner||
           admission.object_id!=definition.stableId||
           admission.definition_name!=definition.name||admission.data_desc!=desc->second)
            return fail(error,"Modern Openable requires a prior-admission receipt for this exact Session object/definition");
        dh2::world::OpenableContainerRowV1 row;std::int32_t row_id=-1;
        if(!policy.authored_table->resolve(desc->second,row_id,row,error))return false;
        if(row_id<0)return fail(error,"Admitted Openable data_desc has no authored Openable row");
        SourceContainerObjsFieldsV1 saved;
        if(!read_source_container_objs_v1(*object,saved,error))return false;
        dh2::world::OpenableContainerFieldsV1 fields;
        fields.state394=saved.state394;
        fields.data374=row_id;
        fields.data_desc=desc->second;
        // Exact original OpenableContainer constructor defaults.
        fields.key_name.clear();fields.death_reset=false;fields.key_consume=true;
        fields.key_qty=1;fields.key_id710=-1;fields.opener398=0;
        policy.source_fields=std::move(fields);
        auto table=policy.authored_table;
        policy.source.resolve_row=[table](const std::string& name,std::int32_t& id,
            dh2::world::OpenableContainerRowV1& resolved,std::string& e){
            return table->resolve(name,id,resolved,e);
        };
    }else if(policy.prior_admission.admission_owner){
        return fail(error,"Modern Openable admission receipt requires its original Openable table");
    }
    WorldObject* drop_object{};
    if(!drop->current_object(definition,drop_object,error)||drop_object!=object)
        return fail(error,"Modern Openable and drop owner do not resolve the identical current WorldObject");
    SourceContainerObjsFieldsV1 saved;
    if(!read_source_container_objs_v1(*object,saved,error))return false;
    try{
        auto binding=std::shared_ptr<SessionContainerModernOpenableV1>(
            new SessionContainerModernOpenableV1(session,definition,visual,
                                                  std::move(drop),std::move(policy)));
        binding->fields_.state394=saved.state394;
        // Restore may already have rebound this exact visual silently. Do not
        // bind a second retained visual into the Session in that case.
        if(!session.retained_object_visual_borrow(definition.stableId)&&
           !visual.bind_authored_object(definition,assets,error))return false;
        dh2::world::OpenableContainerServicesV1 source=binding->policy_.source;
        source.owner=std::const_pointer_cast<void>(binding->lease_);
        auto initial_drop=binding->drop_;
        SourceContainerLootServicesV1 loot;
        if(!initial_drop->services(loot,error))return false;
        SessionOpenableInteractionServicesV1 services;
        services.session_identity=&session;services.session_lease=binding->lease_;
        services.actor_context=binding.get();services.resolve_actor=&resolve_actor;
        services.source=std::move(source);services.interaction=binding->policy_.interaction;
        services.visual_context=&visual;
        services.bind_retained_visual=&SessionContainerRetainedVisualV1::bind_callbacks;
        services.play_retained_clip=&SessionContainerRetainedVisualV1::play_clip;
        services.retained_scene_flags=&SessionContainerRetainedVisualV1::scene_flags;
        services.loot=std::move(loot);
        binding->interaction_=SessionOpenableInteractionV1::create(
            std::move(services),error);
        if(!binding->interaction_)return false;
        output=std::move(binding);error.clear();return true;
    }catch(...){return fail(error,"Modern Openable binding allocation failed");}
}

bool SessionContainerModernOpenableV1::current(std::string& error)const{
    if(!session_||!definition_||!visual_||!interaction_||!lease_)
        return fail(error,"Modern Openable binding is incomplete");
    if(!same_owner(lease_,session_->actor_binding_lease().lock()))
        return fail(error,"Modern Openable binding has stale Session lease; recreate after restore");
    auto* object=session_->world()?session_->world()->find_object(source_id_):nullptr;
    if(!object||object->id!=source_id_||object->name!=definition_->name)
        return fail(error,"Modern Openable source is not the current same-world WorldObject");
    error.clear();return true;
}

bool SessionContainerModernOpenableV1::resolve_actor(
    void* raw,const void* identity,ActorId id,
    SessionContainerActorBorrowV1& output,std::string& error){
    auto* self=static_cast<SessionContainerModernOpenableV1*>(raw);
    output={};
    if(!self||identity!=self->session_||!self->current(error))
        return fail(error,"Modern Openable resolver received foreign/stale Session");
    if(id==self->source_id_){
        auto* object=self->session_->world()->find_object(id);
        if(!object)return fail(error,"Modern Openable source WorldObject is absent");
        output.session_identity=self->session_;output.actor_id=id;
        output.definition=const_cast<ActorDefinition*>(self->definition_);
        output.object=object;output.openable_state=&self->fields_;
        output.session_lease=self->lease_;
        output.binding_lifecycle=self->visual_->binding_lifecycle();
        error.clear();return true;
    }
    if(!self->policy_.resolve_other_actor(self->policy_.other_actor_context,
        identity,id,output,error))return false;
    if(output.session_identity!=identity||output.actor_id!=id||
       !same_owner(output.session_lease,self->lease_)||!output.definition||
       output.definition->stableId!=id||(!output.state&&!output.object)||
       !output.binding_lifecycle)
        return fail(error,"Modern Openable opener resolver returned a foreign or stale actor/object");
    if(output.state&&(output.state->id!=id||
       output.state->definition_id!=output.definition->name))
        return fail(error,"Modern Openable opener ActorState identity differs from its definition");
    if(output.object&&(output.object->id!=id||output.object->name!=output.definition->name))
        return fail(error,"Modern Openable opener WorldObject identity differs from its definition");
    error.clear();return true;
}

bool SessionContainerModernOpenableV1::init_post(std::string& error){
    if(!current(error))return false;
    return interaction_->init_post(source_id_,error);
}
bool SessionContainerModernOpenableV1::initialize_admitted(std::string& error){
    if(!current(error))return false;
    if(!policy_.authored_table||!policy_.prior_admission.admission_owner)
        return fail(error,"Modern Openable admitted initialization requires the exact source table and prior-admission receipt");
    return interaction_->restore_silently(source_id_,error);
}
bool SessionContainerModernOpenableV1::restore_silently(std::string& error){
    if(!current(error))return false;
    return interaction_->restore_silently(source_id_,error);
}
bool SessionContainerModernOpenableV1::interact(ActorId opener,std::string& error){
    if(!current(error))return false;
    return interaction_->interact(source_id_,opener,error);
}
bool SessionContainerModernOpenableV1::animation_event(
    const RetainedAnimationEvent& event,std::string& error){
    if(!current(error))return false;
    return interaction_->animation_event(source_id_,event,error);
}
bool SessionContainerModernOpenableV1::animation_finished(
    std::uint64_t generation,bool active,std::string& error){
    if(!current(error))return false;
    return interaction_->animation_finished(source_id_,generation,active,error);
}

} // namespace dh::foundation::interactions
