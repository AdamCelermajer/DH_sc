#include "session_actor_transition_v1.hpp"

#include "../enemy_ai/runtime_enemy_navigation_v1.hpp"
#include "../../original_actor_motion_flags.hpp"

#include <exception>

namespace dh::foundation::physics {
namespace {
bool same_owner(const std::weak_ptr<const void>& left,
                const std::weak_ptr<const void>& right) noexcept {
    return !left.expired() && !right.expired() &&
           !left.owner_before(right) && !right.owner_before(left);
}
bool same_receipt(const CombatSessionActorTransition& a,
                  const CombatSessionActorTransition& b) noexcept {
    return a.actor==b.actor&&a.from_state==b.from_state&&a.to_state==b.to_state&&
           a.generation==b.generation&&a.occurrence==b.occurrence&&
           a.update_serial==b.update_serial&&a.cause==b.cause&&
           same_owner(a.binding_lease,b.binding_lease)&&
           a.source_attack_moving==b.source_attack_moving&&
           a.source_other_actor==b.source_other_actor&&
           a.source_knockback_great==b.source_knockback_great;
}
bool fail(std::string& error,const char* message){error=message;return false;}
}

SessionActorTransitionConsumerV1::SessionActorTransitionConsumerV1(
    SessionActorTransitionConsumerConfigV1 config):config_(std::move(config)){}

bool SessionActorTransitionConsumerV1::bind(CombatSession& session,std::string& error){
    error.clear();
    if(session_||!config_.bodies||!config_.navigation)
        return fail(error,"Transition consumer requires one Session, actual actor bodies, and modern route owner");
    auto self=weak_from_this().lock();
    if(!self)return fail(error,"Transition consumer must be shared-owned before binding");
    actor_lease_=session.actor_binding_lease();
    lifetime_lease_=session.lifetime_lease();
    if(actor_lease_.expired()||lifetime_lease_.expired())
        return fail(error,"Transition consumer requires a live Session actor/lifetime lease");
    session_=&session;
    if(!session.bind_reconstructible_actor_transition_handler(
        [self,&session](const CombatSessionActorTransition& event,std::string& detail){
            return self->consume(session,event,detail);
        },
        [self,&session](std::string& detail){
            auto life=self->lifetime_lease_.lock();
            if(!life||!life->alive()||self->session_!=&session||
               !same_owner(self->actor_lease_,session.actor_binding_lease())||
               !self->pending_.empty()||self->delivering_){
                detail="Transition consumer checkpoint requires its same live Session and no pending receipt";
                return false;
            }
            detail.clear();return true;
        },error)){
        session_=nullptr;actor_lease_.reset();lifetime_lease_.reset();return false;
    }
    return true;
}

bool SessionActorTransitionConsumerV1::validate_current(
    CombatSession& session,const CombatSessionActorTransition& event,
    std::int32_t expected_state,ActorState*& actor,std::string& error)const{
    auto life=lifetime_lease_.lock();
    if(&session!=session_||!life||!life->alive()||
       !same_owner(actor_lease_,session.actor_binding_lease())||
       !same_owner(actor_lease_,event.binding_lease))
        return fail(error,"Transition receipt is not from this live Session/current actor binding");
    if(event.actor==invalid_actor_id||event.occurrence==0||
       event.update_serial!=session.update_serial())
        return fail(error,"Transition receipt actor/frame/occurrence is stale or malformed");
    auto* world=session.world();
    if(!world||(actor=world->find_actor(event.actor))==nullptr||
       actor!=session.actor(event.actor)||actor->id!=event.actor)
        return fail(error,"Transition receipt no longer resolves to the same live Session actor");
    const auto* properties=world->combat_properties(event.actor);
    if(!properties||properties->facts.original_state!=expected_state||
       session.original_actor_state(event.actor)!=expected_state)
        return fail(error,"Transition stage does not match the current canonical source-state binding");
    error.clear();return true;
}

bool SessionActorTransitionConsumerV1::body_present(
    CombatSession& session,ActorId id,bool& present,std::string& error){
    present=false;
    if(!config_.bodies||session.actor(id)!=session.world()->find_actor(id))
        return fail(error,"Physical body lookup requires the same current Session actor");
    auto* actor=session.actor(id);
    OriginalTriggerActorBorrow owner;
    if(!config_.bodies->actor_borrow(id,owner,error))return false;
    OriginalActorSubobjectsBorrow borrow;
    if(!config_.bodies->subobjects_borrow(id,borrow,error))return false;
    if(!owner.receiver||!owner.position160||owner.position160!=actor->transform.position.data()||
       owner.identity==0||!borrow.owner_lease||!same_owner(
           std::weak_ptr<const void>(owner.receiver),
           std::weak_ptr<const void>(borrow.owner_lease))||
       owner.identity!=borrow.identity||!owner.physical2dc||
       owner.physical2dc!=borrow.physical2dc||!borrow.physical2dc){
        return fail(error,"Physical body query returned a stale or mismatched source owner");
    }
    present=*borrow.physical2dc!=0&&borrow.native&&borrow.native->body;
    error.clear();return true;
}

bool SessionActorTransitionConsumerV1::set_body_pinned(
    CombatSession& session,ActorId id,bool pinned,std::string& error){
    bool present=false;if(!body_present(session,id,present,error))return false;
    if(!present){error.clear();return true;}
    const auto* before=config_.bodies->physical(id);
    if(!before||before->actor_identity()==0)
        return fail(error,"Pin/Unpin requires the same registered actual source body");
    if(!config_.bodies->set_pinned(id,pinned,error))return false;
    bool still_present=false;if(!body_present(session,id,still_present,error))return false;
    if(!still_present||config_.bodies->physical(id)!=before)
        return fail(error,"Source body identity changed during Pin/Unpin delivery");
    error.clear();return true;
}

bool SessionActorTransitionConsumerV1::blur(
    CombatSession& session,const CombatSessionActorTransition& event,
    ActorState& actor,std::string& error){
    switch(event.from_state){
    case 3:
        if(!config_.source.clear_idle_suppressed538)
            return fail(error,"Reached Idle Blur requires actual Character+0x538 setter");
        if(!config_.source.clear_idle_suppressed538(actor.id,error))return false;
        break;
    case 4:{
        auto* pf=config_.bodies->navigation(actor.id);
        if(!pf)return fail(error,"Move Blur requires this actor's actual PF object");
        if(!config_.navigation->release_route(session,actor.id,error))return false;
        bool position_from_physics=false;
        if(!config_.source.stop_after_route_drop)
            return fail(error,"Reached Move Blur requires actual GameObject.Stop field/IsUpdatingPositionFromPhysics provider");
        // PlayableActorBodies owns this PF object mutably; its public borrow is
        // const because most navigation clients are observers. The transition
        // provider is the admitted source Stop writer for this live receipt.
        if(!config_.source.stop_after_route_drop(actor,
              *const_cast<dh2::navigation::NavigationObject*>(pf),position_from_physics,error))return false;
        bool present=false;if(!body_present(session,actor.id,present,error))return false;
        if(present&&position_from_physics){
            bool stopped=false;
            if(!config_.bodies->stop_physical(actor.id,
                [&session](ActorId id){return session.actor(id);},stopped,error))return false;
        }
        // CSMove::OnBlur reacquires +0x2dc only after GameObject::Stop.
        return set_body_pinned(session,actor.id,true,error);
    }
    case 5:{
        if(!config_.source.attack_blur_delay_timer)
            return fail(error,"Reached Attack Blur requires the actual delay/timer2a provider");
        if(!config_.source.attack_blur_delay_timer(event,error))return false;
        return set_body_pinned(session,actor.id,true,error);
    }
    case 6:{
        auto* pf=config_.bodies->navigation(actor.id);
        if(!pf)return fail(error,"Skill Blur requires this actor's actual PF object");
        if(!config_.navigation->release_route(session,actor.id,error))return false;
        bool position_from_physics=false;
        if(!config_.source.stop_after_route_drop)
            return fail(error,"Reached Skill Blur requires actual GameObject.Stop field/IsUpdatingPositionFromPhysics provider");
        if(!config_.source.stop_after_route_drop(actor,
              *const_cast<dh2::navigation::NavigationObject*>(pf),position_from_physics,error))return false;
        bool present=false;if(!body_present(session,actor.id,present,error))return false;
        if(present&&position_from_physics){
            bool stopped=false;
            if(!config_.bodies->stop_physical(actor.id,
                [&session](ActorId id){return session.actor(id);},stopped,error))return false;
        }
        bool timer_started=false;
        if(!config_.source.skill_blur_timer10_event48)
            return fail(error,"Reached Skill Blur requires actual gate528/timer10/event0x30 owner");
        if(!config_.source.skill_blur_timer10_event48(event,timer_started,error))return false;
        if(timer_started){error.clear();return true;}
        return set_body_pinned(session,actor.id,true,error);
    }
    case 7:error.clear();return true;
    case 2:
        // P16 DESPAWN. CSDespawn::OnBlur deletes summoned actors and sends others to Limbus; the pool slot and the Limbus
        // transition are owned by DespawnAfterDeathV1 and OriginalActorLifecycle, so the physics consumer has no effect here.
        error.clear();return true;
    case 0:
        // P16 DESPAWN. CSLimbus::OnBlur: SetPosition/SetRotation to the authored anchor, Revive, and the limbus group
        // bookkeeping (SM_SetLimbusState). These are the lifecycle's own effects; no body is pinned here.
        error.clear();return true;
    case 1:
    case 17:
        // P16 LIFECYCLE. CSPreSpawn::OnBlur (Revive, EnableCollisions) and CSSpawn::OnBlur
        // (InitPhysicalObject unless flags520 bit 0x2000) are the lifecycle's own native effects. OriginalActorLifecycle
        // runs them in source order around this admitted transition (init before, filter enable before). Neither
        // state pins the body, so the consumer only validates the receipt here.
        error.clear();return true;
    case 10:{
        if(!config_.source.knockback_controller_lock)
            return fail(error,"KnockBack Blur requires the actual controller lock owner");
        if(!config_.source.knockback_controller_lock(actor.id,false,error))return false;
        bool present=false;if(!body_present(session,actor.id,present,error))return false;
        if(present&&!config_.bodies->reset_source_physical_filter(actor.id,
            [&session](ActorId id){return session.actor(id);},error))return false;
        return set_body_pinned(session,actor.id,true,error);
    }
    case 11:error.clear();return true;
    case 12:{
        // Controller unlock is a lifecycle tail owned elsewhere. The physical
        // projection restores the body's source filter only when it exists.
        bool present=false;if(!body_present(session,actor.id,present,error))return false;
        if(present){
            if(!config_.source.dead_blur_reset_filter)
                return fail(error,"Reached Dead Blur body requires actual resetFilter owner");
            if(!config_.source.dead_blur_reset_filter(actor.id,error))return false;
        }
        break;
    }
    default:return fail(error,"Source Blur state is outside the verified 3/4/5/6/7/10/11/12 consumer boundary");
    }
    error.clear();return true;
}

bool SessionActorTransitionConsumerV1::focus_prefix(
    CombatSession&,const CombatSessionActorTransition& event,
    ActorState& actor,std::string& error){
    switch(event.to_state){
    case 3:{
        if(!config_.source.read_idle_suppressed538)
            return fail(error,"Reached Idle Focus requires actual Character+0x538 getter");
        bool suppressed=false;if(!config_.source.read_idle_suppressed538(actor.id,suppressed,error))return false;
        if(!suppressed)actor.source_flags520=0x2380u;
        error.clear();return true;
    }
    case 4:{
        OriginalMotionPrefixResult result;
        std::uint8_t suppressed=0;
        if(!original_motion_focus_prefix(OriginalMotionFocusPrefix::move,
             actor.source_flags520,actor.source_movement_type,suppressed,result,error))return false;
        error.clear();return true;
    }
    case 5:
        actor.source_flags520=0x2341u;
        if(!config_.source.attack_focus_delay_gate)
            return fail(error,"Reached Attack Focus requires actual GetAttackDelay/gate528 provider");
        return config_.source.attack_focus_delay_gate(event,error);
    case 6:{
        actor.source_flags520=0x6341u;
        bool moving_byte=false;
        if(!config_.source.skill_focus_moving_byte)
            return fail(error,"Reached Skill Focus requires current SkillTable moving-byte provider");
        if(!config_.source.skill_focus_moving_byte(event,moving_byte,error))return false;
        if(!config_.source.skill_focus_gate528)
            return fail(error,"Reached Skill Focus requires actual Character+0x528 gate owner");
        return config_.source.skill_focus_gate528(event,moving_byte,error);
    }
    case 7:
        actor.source_flags520=0x6301u;
        error.clear();return true;
    case 2:
        // P16 DESPAWN. CSDespawn::OnFocus writes flags328 = 512 (Despawn clip selection is the lifecycle's effect).
        actor.source_flags520=0x200u;
        error.clear();return true;
    case 0:
        // P16 DESPAWN. CSLimbus::OnFocus clears flags328 (0) and hides the actor; the respawn timer and AI_ClearAllAggro
        // are the lifecycle's effects (OriginalActorLifecycle::change for state 0).
        actor.source_flags520=0u;
        error.clear();return true;
    case 1:
        // CSSpawn::OnFocus writes flags 577 (0x241); OriginalActorLifecycle re-applies carried 0x2000 after selection.
        actor.source_flags520=0x241u;
        error.clear();return true;
    case 17:
        // CSPreSpawn::OnFocus writes flags 4864 (0x1300). Its body removal and DisableCollisions run after selection
        // in OriginalActorLifecycle (source SetPhysicalObject(nullptr) and DisableCollisions order).
        actor.source_flags520=0x1300u;
        error.clear();return true;
    case 10:{
        if(!event.source_knockback_great||event.source_other_actor==invalid_actor_id)
            return fail(error,"KnockBack Focus requires the actual variant and attacker");
        if(!config_.source.knockback_read_gate528||!config_.source.knockback_write_gate528)
            return fail(error,"KnockBack Focus requires the actual embedded gate528 owner");
        std::uint32_t gate=0;
        if(!config_.source.knockback_read_gate528(actor.id,gate,error))return false;
        // SM_SetKnockBackState writes this before OnFocus/SM_SetAnim.
        gate=*event.source_knockback_great?0x18u:(gate&~0x18u);
        if(!config_.source.knockback_write_gate528(actor.id,gate,error))return false;
        actor.source_flags520=0x2341u;
        error.clear();return true;
    }
    case 11:
        actor.source_flags520=0x2b41u;
        error.clear();return true;
    case 12:{
        actor.source_flags520=0x241u;
        if(!config_.source.source_is_player)
            return fail(error,"Reached Dead Focus after flags241 requires actual source IsPlayer getter");
        bool player=false;if(!config_.source.source_is_player(actor.id,player,error))return false;
        if(player)actor.source_flags520=*actor.source_flags520|0x2000u;
        error.clear();return true;
    }
    default:return fail(error,"Source Focus state is outside the verified 3/4/5/6/7/10/11/12 consumer boundary");
    }
}

bool SessionActorTransitionConsumerV1::focus_suffix(
    CombatSession& session,const CombatSessionActorTransition& event,
    ActorState& actor,std::string& error){
    switch(event.to_state){
    case 3:error.clear();return true;
    case 4:
        // Move Focus unpins only after UpdateType/animation publication.
        return set_body_pinned(session,actor.id,false,error);
    case 5:{
        if(!event.source_attack_moving)
            return fail(error,"Attack Focus suffix requires the current prepared source Attack/AttackStatic branch fact");
        // CombatSession supplies the actual prepared root choice. Moving
        // Attack unpins; AttackStatic pins. No predecessor/action/clip guess.
        return set_body_pinned(session,actor.id,!*event.source_attack_moving,error);
    }
    case 6:
        // CSSkill.Focus unpins only after its actual retained source selection
        // and animation publication have completed.
        return set_body_pinned(session,actor.id,false,error);
    case 7:error.clear();return true;
    case 2:
    case 0:
    case 1:
    case 17:
        error.clear();return true; // P16 LIFECYCLE/DESPAWN: see focus_prefix; no pin change for these states.
    case 10:{
        std::uint32_t gate=0;
        if(!config_.source.knockback_read_gate528||!config_.source.knockback_write_gate528||
           !config_.source.knockback_controller_lock||!config_.source.knockback_look_at_cancel_sneaking)
            return fail(error,"KnockBack suffix requires actual gate/controller/look/sneak owners");
        if(!config_.source.knockback_read_gate528(actor.id,gate,error))return false;
        if(gate&8u){
            bool present=false;if(!body_present(session,actor.id,present,error))return false;
            if(present&&!config_.bodies->set_source_physical_filter(actor.id,
                [&session](ActorId id){return session.actor(id);},0,0x51c,3,false,error))return false;
        }
        gate=(gate&0x10u)?0x20u:(gate&~0x20u);
        if(!config_.source.knockback_write_gate528(actor.id,gate,error)||
           !config_.source.knockback_controller_lock(actor.id,true,error)||
           !config_.source.knockback_look_at_cancel_sneaking(event,error))return false;
        return set_body_pinned(session,actor.id,false,error);
    }
    case 11:error.clear();return true;
    case 12:{
        bool present=false;if(!body_present(session,actor.id,present,error))return false;
        if(present){
            if(!config_.source.dead_focus_physical_filter)
                return fail(error,"Dead Focus with a live body requires its actual source physical-filter provider");
            if(!config_.source.dead_focus_physical_filter(actor.id,error))return false;
        }
        error.clear();return true;
    }
    default:return fail(error,"Source Focus suffix is outside the verified 3/4/5/6/7/10/11/12 consumer boundary");
    }
}

bool SessionActorTransitionConsumerV1::consume(
    CombatSession& session,const CombatSessionActorTransition& event,std::string& error){
    error.clear();
    if(delivering_)return fail(error,"Actor transition consumer cannot reenter itself");
    struct Scope{bool& active;explicit Scope(bool& value):active(value){active=true;}~Scope(){active=false;}} scope(delivering_);
    ActorState* actor=nullptr;
    if(event.stage==CombatSessionTransitionStage::blur){
        if(pending_.count(event.actor))return fail(error,"Duplicate/out-of-order Blur for pending source transition");
        if(!validate_current(session,event,event.from_state,actor,error)||
           !blur(session,event,*actor,error))return false;
        pending_.emplace(event.actor,Pending{event,CombatSessionTransitionStage::focus_prefix});
        error.clear();return true;
    }
    const auto found=pending_.find(event.actor);
    if(found==pending_.end()||found->second.next!=event.stage||!same_receipt(found->second.receipt,event))
        return fail(error,"Focus receipt has no matching same-binding Blur/prefix occurrence");
    if(!validate_current(session,event,event.to_state,actor,error))return false;
    if(event.stage==CombatSessionTransitionStage::focus_prefix){
        if(!focus_prefix(session,event,*actor,error))return false;
        found->second.next=CombatSessionTransitionStage::focus_suffix;
        error.clear();return true;
    }
    if(!focus_suffix(session,event,*actor,error))return false;
    pending_.erase(found);error.clear();return true;
}

std::shared_ptr<SessionActorTransitionConsumerV1> make_session_actor_transition_consumer_v1(
    SessionActorTransitionConsumerConfigV1 config){
    return std::make_shared<SessionActorTransitionConsumerV1>(std::move(config));
}

} // namespace dh::foundation::physics
