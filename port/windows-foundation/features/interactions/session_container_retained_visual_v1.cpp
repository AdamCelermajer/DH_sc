#include "session_container_retained_visual_v1.hpp"
#include "world_object_container_state_v1.hpp"

#include <cmath>

namespace dh::foundation::interactions {
namespace {
bool fail(std::string& error,const char* message){error=message;return false;}
bool same_owner(const std::shared_ptr<const void>& a,
                const std::shared_ptr<const void>& b)noexcept{
    return a&&b&&!a.owner_before(b)&&!b.owner_before(a);
}
}

SessionContainerRetainedVisualV1::SessionContainerRetainedVisualV1(
    CombatSession& session):session_(&session){
    lease_=session.actor_binding_lease().lock();
    if(lease_)lifecycle_=1;
}

bool SessionContainerRetainedVisualV1::refresh_after_restore(std::string& error){
    if(!session_)return fail(error,"Container retained visual has no CombatSession");
    auto next=session_->actor_binding_lease().lock();
    if(!next)return fail(error,"Container retained visual requires current Session actor/object lease");
    if(lease_&&same_owner(lease_,next))return fail(error,"Container retained visual lease was not replaced");
    lease_=std::move(next);
    if(lifecycle_==UINT64_MAX)return fail(error,"Container visual lifecycle exhausted");
    ++lifecycle_;error.clear();return true;
}

bool SessionContainerRetainedVisualV1::current(
    const void* identity,ActorId id,std::string& error,bool require_visual)const{
    if(!session_||identity!=session_||!lease_)
        return fail(error,"Container visual callback has foreign Session identity or expired lease");
    auto current_lease=session_->actor_binding_lease().lock();
    if(!same_owner(lease_,current_lease))
        return fail(error,"Container visual callback has stale Session lease");
    if(id==invalid_actor_id||!session_->world()||!session_->world()->find_object(id))
        return fail(error,"Container visual callback requires same-world ObjectId");
    if(require_visual&&!session_->retained_object_visual_borrow(id))
        return fail(error,"Container visual callback requires retained ObjectId visual");
    error.clear();return true;
}

bool SessionContainerRetainedVisualV1::bind_object(
    ActorId id,const AssetCatalog& assets,std::string& error){
    if(!current(session_,id,error,false))return false;
    return session_->bind_object_visual(id,assets,error);
}

bool SessionContainerRetainedVisualV1::bind_authored_object(
    const ActorDefinition& definition,const AssetCatalog& assets,std::string& error){
    if(!definition.stableId||definition.stableId==invalid_actor_id||definition.name.empty())
        return fail(error,"Authored container binding requires a stable source ObjectId/name");
    if(!current(session_,definition.stableId,error,false))return false;
    const auto* object=session_->world()->find_object(definition.stableId);
    if(!object||object->id!=definition.stableId||object->name!=definition.name||
       object->visual.model.empty())
        return fail(error,"Current same-world object does not match authored container ID/name/model");
    for(const auto coordinate:object->transform.position)
        if(!std::isfinite(coordinate))return fail(error,"Authored container transform is not finite");
    return session_->bind_object_visual(definition.stableId,assets,error);
}

bool SessionContainerRetainedVisualV1::restore_authored_object(
    const ActorDefinition& definition,const AssetCatalog& assets,std::string& error){
    if(!bind_authored_object(definition,assets,error))return false;
    const auto* object=session_->world()->find_object(definition.stableId);
    SourceContainerObjsFieldsV1 fields;
    if(!object||!read_source_container_objs_v1(*object,fields,error))return false;
    const auto pose=source_container_restore_visual_v1(fields.state394);
    if(pose==SourceContainerRestoreVisualV1::none){error.clear();return true;}
    return session_->restore_object_pose(definition.stableId,
        pose==SourceContainerRestoreVisualV1::idle?"idle":"idleactive",false,error);
}

bool SessionContainerRetainedVisualV1::restore_pose(
    ActorId id,const std::string& clip,bool loop,std::string& error){
    if(!current(session_,id,error))return false;
    return session_->restore_object_pose(id,clip,loop,error);
}

bool SessionContainerRetainedVisualV1::bind_callbacks(
    void* opaque,const void* identity,ActorId id,
    SessionContainerEventCallbackV1 event,
    SessionContainerCompletionCallbackV1 finished,std::string& error){
    auto* self=static_cast<SessionContainerRetainedVisualV1*>(opaque);
    if(!self||!event||!finished||!self->current(identity,id,error))
        return fail(error,"Container retained visual requires both same-session event and completion callbacks");
    CombatSessionObjectAnimationServices callbacks;
    callbacks.event=[self,identity,id,event=std::move(event)](
        ObjectId source,const RetainedAnimationEvent& value,std::string& e){
        if(source!=id)return fail(e,"Retained container event ObjectId differs from its same-session binding");
        return self->current(identity,id,e)&&event(source,value,e);
    };
    callbacks.finished=[self,identity,id,finished=std::move(finished)](
        ObjectId source,std::uint64_t generation,bool loop,std::string& e){
        if(source!=id)return fail(e,"Retained container completion ObjectId differs from its same-session binding");
        return self->current(identity,id,e)&&finished(source,generation,loop,e);
    };
    return self->session_->bind_object_animation_services(id,std::move(callbacks),error);
}

bool SessionContainerRetainedVisualV1::play_clip(
    void* opaque,const void* identity,ActorId id,const char* clip,
    bool& accepted,std::string& error){
    accepted=false;
    auto* self=static_cast<SessionContainerRetainedVisualV1*>(opaque);
    if(!self||!clip||!self->current(identity,id,error))
        return fail(error,"Container retained clip requires same Session, ObjectId, and clip");
    return self->session_->play_object_clip(id,clip,false,accepted,error);
}

bool SessionContainerRetainedVisualV1::scene_flags(
    void* opaque,const void* identity,ActorId id,std::uint32_t clear,
    std::uint32_t set,std::string& error){
    auto* self=static_cast<SessionContainerRetainedVisualV1*>(opaque);
    if(!self||!self->current(identity,id,error))
        return fail(error,"Container scene flags require same Session ObjectId");
    return self->session_->set_object_scene_flags(id,clear,set,error);
}

} // namespace dh::foundation::interactions
