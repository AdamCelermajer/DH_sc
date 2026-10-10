#include "combat_session.hpp"
#include "player_profile_properties.hpp"
#include <algorithm>
#include <cmath>
#include <set>
#include <stdexcept>
#include <limits>
#include "../level-world/actor_rotation.hpp"
#include "../level-world/navigation_heading.hpp"
#include "retained_sequence_playback.hpp"
#include "original_controller_commands.hpp"
#include "features/combo_chain/source_combo_chain.hpp"
#include "features/audio/retained_frame_audio_observer.hpp"
#include "../level-world/character_animation_ai.hpp"

namespace dh::foundation {
struct CombatSession::Impl {
    std::shared_ptr<CombatSessionLifetime> ownerLifetime=std::make_shared<CombatSessionLifetime>();
    struct ObjectEntry {
        CharacterVisual visual;
        std::unique_ptr<RetainedAnimationOwner> animation;
        CombatSessionObjectAnimationServices services;
        std::string model;
        std::uint64_t selection=0;
        std::uint32_t flags=0x60fu;
        bool selected=false;
    };
    std::map<ObjectId,std::shared_ptr<ObjectEntry>> objectEntries;
    std::vector<ObjectId> objectOrder;
    struct Entry {
        struct TargetProjection {
            ActorId lastTarget=invalid_actor_id;
            bool lastTargetKnown=false;
            ActorId objectOfInterest=invalid_actor_id;
            std::int32_t objectOfInterestType=-1;
            bool objectOfInterestKnown=false;
            std::uint64_t objectOfInterestSerial=0;
        } targetProjection;
        struct Locomotion {OriginalCombatPhase phase;float rate=1;std::int32_t repeats=0;std::int32_t originalState=-1;};
        std::map<std::string,Locomotion> locomotion;
        std::string locomotionSelected;
        CharacterVisual* visual=nullptr;
        AttackDefinition attack;
        std::string idle;
        std::string idlePath;
        double idleRate=1;
        PlayableActorTraits traits;
        CombatPoseBindings poses;
        // Source leaf metadata for admitted injury/death pose selections. These
        // seeded poses share the runtime visual owner but do not traverse the
        // retained attack hierarchy, so its boundary callback cannot see them.
        std::map<std::string,CombatSessionStepEntry> poseStepEntries;
        std::unique_ptr<OriginalAttackSequence> sequence;
        std::optional<OriginalCombatVisualPlan> sequencePlan;
        std::optional<OriginalAttackSelection> sequenceCallerSelection;
        bool retainedPhaseClock=false,sourceAction=false,animationOnly=false;
        bool receiveDamage=false;
        std::optional<bool> reactionMinimalRandoms;
        std::vector<OriginalCombatPhase> reactionChoices;
        std::unique_ptr<RetainedSequencePlayback> retained;
        std::vector<SourceCombatMarker> sourceEvents;
        const SourceCombatMarkerSink* sourceMarkerSink=nullptr;
        std::uint32_t sourceSerial=0;
        float sourceActorRate=1;
        std::string sourceSelectedClip;
        bool diagnosticAI=false;
        bool stateManaged=false,stateSequence=false,stateFrozen=false,attackProgram=true;
        bool permission=true,baseTargetable=true;
        std::optional<std::int32_t> lifecycleOriginalState;
        std::optional<std::int32_t> presentationInitialState;
        CombatSessionStateAnimationServices stateServices;
        std::optional<CombatSessionSourceSequencePolicy> sourceStatePolicy;
        bool dispatchingDeparture=false;
        std::uint32_t turnPositive=0;
        double rotationFractionMs=0;
        std::uint64_t rotationUpdateSerial=0;
        bool sourceCombo=false;
        bool sourceAttackStateSelection=false;
        dh2::character::AttackState64 sourceAttack{};
        std::uint64_t comboGeneration=0;
    };
    dh2::data::CombatRandom random{};
    std::unique_ptr<PlayableActorWorld> world;
    std::unique_ptr<CombatSystem> combat;
    std::unique_ptr<ActorCombatRuntime> runtime;
    ActorId player=invalid_actor_id;
    // A player target chosen through the public selection input remains sticky
    // across action teardown. This host-side intent is deliberately distinct
    // from ActorState::target_id, which combat finish/interrupt callbacks may
    // clear while releasing their own transient action state.
    ActorId stickyPlayerTarget=invalid_actor_id;
    std::map<ActorId,Entry> entries;
    std::vector<DamageEvent> events;
    std::vector<DamageEvent> pendingSourceHits;
    struct SourceHitBatch {
        std::uint64_t generation=0;
        std::set<std::pair<ActorId,std::uint32_t>> delivered;
    };
    std::map<std::pair<ActorId,std::string>,SourceHitBatch> sourceHitBatches;
    bool applyingSourceHit=false;
    SourceHitEffectHandler sourceHitEffectHandler;
    std::weak_ptr<const void> sourceHitEffectLease;
    bool sourceHitEffectRequired=false;
    bool deliveringSourceHitEffect=false;
    std::vector<PlayableCombatResolution> resolutions;
    std::vector<std::string> logs;
    std::set<ActorId> reactionGapLogged;
    bool detached=false;
    std::shared_ptr<const void> bindingLease=std::make_shared<const unsigned char>(0);
    OriginalSequencePolicies sequencePolicies;
    MotionHandler motionHandler;
    MotionPhaseHandler motionPhaseHandler;
    std::map<ActorId,std::vector<CombatSessionMotionSample>> pendingMotion;
    bool motionPhaseRequired=false,updating=false,motionPhaseDelivering=false;
    const AssetCatalog* assets=nullptr;
    CombatPermissionProvider permissionProvider;
    DiagnosticControllerAdmissionProvider controllerAdmissionProvider;
    ControllerNetworkModeProvider networkModeProvider;
    CombatSessionAnimationNotificationServices animationNotificationServices;
    std::function<bool(std::string&)> animationNotificationCheckpoint;
    std::weak_ptr<const void> animationNotificationLease;
    bool animationNotificationsReconstructible=false,animationNotificationsNeedRebind=false;
    bool checkingAnimationCheckpoint=false,restoreTeardown=false;
    ActorTransitionHandler actorTransitionHandler;
    std::function<bool(std::string&)> actorTransitionCheckpoint;
    std::weak_ptr<const void> actorTransitionLease;
    std::map<ActorId,CombatSessionActorTransition> pendingTransitions;
    std::uint64_t transitionOccurrence=0;
    bool transitionRequired=false,transitionDelivering=false,suppressRuntimeTransitions=false;
    std::string transitionFailure;
    std::vector<CombatSessionAnimationDispatch> animationDispatches;
    bool lifecycleRegistered=false;
    bool sourceCheckpointReadyAtDetach=false;
    AttackOwnerProvider attackOwnerProvider;
    std::vector<CombatSessionComboBoundary> comboBoundaries;
    RetainedFrameAudioObserver retainedFrameAudioObserver;
    std::vector<RetainedFrameAudioObserverDiagnostic> retainedFrameAudioDiagnostics;
    std::optional<RetainedFrameAudioClock> retainedFrameAudioClock;
    ActorDecisionProvider actorDecisionProvider;
    ActorDecisionProvider frameBeginProvider;
    std::uint64_t updateSerial=0;
    CombatSessionStepObserver stepObserver;
    std::vector<std::string> stepDiagnostics;
    std::uint64_t stepOccurrence=0;
    bool publish_source_state(ActorId id,std::int32_t state,std::string& error){
        const auto* old=world->combat_properties(id);const auto* traits=world->traits(id);
        if(!old||!traits){error="Source state publication requires current actor sheets";return false;}
        auto properties=*old;properties.facts.original_state=state;
        return world->update_combat_properties(id,std::move(properties),*traits,error);
    }
    bool source_checkpoint(std::string& error){
        if(updating||applyingSourceHit||deliveringSourceHitEffect||restoreTeardown||checkingAnimationCheckpoint||transitionDelivering||!pendingMotion.empty()){
            error="Actor motion phase is active or has unprocessed authored samples";return false;
        }
        if(!transitionFailure.empty()){error=transitionFailure;return false;}
        if(!pendingTransitions.empty()){error="Accepted actor transition has an unfinished reached prefix";return false;}
        if(sourceHitEffectRequired&&!sourceHitEffectHandler&&!detached){error="Required fresh source-hit effect binding after restore";return false;}
        if(runtime&&!runtime->validate_transition_checkpoint(error))return false;
        if(transitionRequired&&!actorTransitionHandler&&!detached){error="Required fresh actor transition binding after restore";return false;}
        if(actorTransitionHandler){
            const auto lease=actorTransitionLease.lock();
            if(!lease||!bindingLease||lease.owner_before(bindingLease)||bindingLease.owner_before(lease)||!actorTransitionCheckpoint){
                error="Actor transition checkpoint requires the current actor lease";return false;
            }
            struct Scope{bool& flag;~Scope(){flag=false;}} scope{checkingAnimationCheckpoint};checkingAnimationCheckpoint=true;
            try{const auto validator=actorTransitionCheckpoint;
                if(!validator(error)){if(error.empty())error="Actor transition checkpoint validator rejected";return false;}
            }catch(const std::exception& ex){error=ex.what();return false;}
             catch(...){error="Actor transition checkpoint validator threw";return false;}
        }
        try{for(const auto& pair:entries){const auto& entry=pair.second;
            if(entry.stateSequence&&entry.sourceStatePolicy){error="Active source skill/spell playback is not persisted by the checkpoint";return false;}
            if(entry.stateServices.checkpoint&&!entry.stateServices.checkpoint(error)){
                if(error.empty())error="Source program transient owner rejected checkpoint";return false;
            }
        }}catch(const std::exception& ex){error=ex.what();return false;}
          catch(...){error="Source program checkpoint policy threw";return false;}
        error.clear();return true;
    }
    bool animation_checkpoint(std::string& error){
        if(animationNotificationsNeedRebind&&!detached){
            error="Required fresh reconstructible animation notification binding after restore";return false;
        }
        if(!animationNotificationServices.notification&&!animationNotificationServices.state_event)return true;
        if(!animationNotificationsReconstructible||!animationNotificationCheckpoint){
            error="Animation providers are not persisted; checkpoint requires explicit reconstructible binding";return false;
        }
        const auto lease=animationNotificationLease.lock();
        if(!lease||!bindingLease||lease.owner_before(bindingLease)||bindingLease.owner_before(lease)||checkingAnimationCheckpoint){
            error="Animation checkpoint requires the same idle current actor lease";return false;
        }
        struct Scope{bool& flag;~Scope(){flag=false;}} scope{checkingAnimationCheckpoint};checkingAnimationCheckpoint=true;
        try{const auto validator=animationNotificationCheckpoint;
            if(!validator(error)){if(error.empty())error="Reconstructible animation checkpoint validator rejected";return false;}
            if(detached||!bindingLease||lease.owner_before(bindingLease)||bindingLease.owner_before(lease)){
                error="Animation checkpoint validator changed the actor binding";return false;
            }
            error.clear();return true;
        }catch(const std::exception& ex){error=ex.what();return false;}
          catch(...){error="Animation checkpoint validator threw";return false;}
    }
    void detach_bindings(bool checkpointReady){
        sourceCheckpointReadyAtDetach=checkpointReady;
        if(animationNotificationsReconstructible){
            animationNotificationsNeedRebind=true;animationNotificationServices={};
            animationNotificationCheckpoint={};animationNotificationLease.reset();animationNotificationsReconstructible=false;
        }
        actorTransitionHandler={};actorTransitionCheckpoint={};actorTransitionLease.reset();pendingTransitions.clear();
        sourceHitEffectHandler={};sourceHitEffectLease.reset();
        pendingMotion.clear();motionPhaseHandler={};
        runtime->clear();combat->clear();events.clear();resolutions.clear();comboBoundaries.clear();
        world->take_resolutions();pendingSourceHits.clear();sourceHitBatches.clear();
        objectEntries.clear();objectOrder.clear();bindingLease.reset();detached=true;
    }
    bool actor_transition(ActorId id,CombatRuntimeTransition receipt,std::string& error,
                          const std::function<bool(ActorId,std::string&)>* normal_finished=nullptr,
                          const CombatSessionSourceSequencePolicy* incoming_policy=nullptr){
        if(!actorTransitionHandler)return !transitionRequired||detached;
        if(detached||transitionDelivering||restoreTeardown||checkingAnimationCheckpoint||!entries.count(id)){
            error="Accepted actor transition needs idle current consumer delivery";return false;
        }
        const auto lease=actorTransitionLease.lock();
        if(!lease||!bindingLease||lease.owner_before(bindingLease)||bindingLease.owner_before(lease)){
            error="Accepted actor transition belongs to a stale actor lease";return false;
        }
        if(!transitionFailure.empty()){error=transitionFailure;return false;}
        const auto emit=[&](CombatSessionActorTransition event,CombatSessionTransitionStage stage){
            event.stage=stage;
            struct Scope{bool& flag;~Scope(){flag=false;}} scope{transitionDelivering};transitionDelivering=true;
            try{if(actorTransitionHandler(event,error))return true;
                if(error.empty())error="Accepted actor transition consumer rejected after its reached prefix";
            }catch(const std::exception& ex){error=ex.what();}
             catch(...){error="Accepted actor transition consumer threw after its reached prefix";}
            transitionFailure=error;return false;
        };
        if(receipt.stage==CombatRuntimeTransitionStage::before_change){
            if(pendingTransitions.count(id)||transitionOccurrence==UINT64_MAX){error="Actor transition occurrence is unfinished/exhausted";return false;}
            CombatSessionActorTransition event{id,receipt.from_state,receipt.to_state,receipt.generation,
                ++transitionOccurrence,updateSerial,receipt.cause,CombatSessionTransitionStage::blur,bindingLease};
            if(receipt.to_state==5&&receipt.cause==CombatRuntimeTransitionCause::attack){
                const auto& selection=entries.at(id).sequenceCallerSelection;
                if(selection){
                    if(selection->state=="Attack")event.source_attack_moving=true;
                    else if(selection->state=="AttackStatic")event.source_attack_moving=false;
                }
            }
            if(incoming_policy&&incoming_policy->original_state==10){
                event.source_other_actor=incoming_policy->source_other_actor;
                event.source_knockback_great=incoming_policy->source_knockback_great;
            }
            pendingTransitions.emplace(id,event);
            if(!emit(event,CombatSessionTransitionStage::blur))return false;
            if(entries.at(id).stateSequence&&entries.at(id).sourceStatePolicy){
                const bool retired=normal_finished
                    ? finish_source_state(id,receipt.to_state,*normal_finished,error)
                    : depart_source_state(id,receipt.to_state,error);
                if(!retired){
                    if(error.empty())error="Outgoing source program Post failed after its reached Blur";
                    transitionFailure=error;return false;
                }
            }else if(!publish_source_state(id,receipt.to_state,error))return false;
            if(!emit(event,CombatSessionTransitionStage::focus_prefix))return false;
            pendingTransitions.at(id).stage=CombatSessionTransitionStage::focus_prefix;
            return true;
        }
        const auto pending=pendingTransitions.find(id);
        if(pending==pendingTransitions.end()||pending->second.from_state!=receipt.from_state||
           pending->second.to_state!=receipt.to_state||pending->second.generation!=receipt.generation){
            error="Actor transition suffix has no matching admitted prefix";return false;
        }
        if(!emit(pending->second,CombatSessionTransitionStage::focus_suffix))return false;
        pendingTransitions.erase(id);return true;
    }
    bool dispatch_motion(ActorId id,Vec3 delta,bool enabled,std::string& error){
        if(!motionPhaseRequired&&!motionHandler)return true;
        auto* actor=world->find_actor(id);
        if(!actor){error="Authored motion actor unavailable";return false;}
        if(motionPhaseRequired){
            if(detached)return true; // Silent restored pose anchoring, never root replay.
            if(motionPhaseDelivering){error="Motion phase cannot reenter animation sampling";return false;}
            for(const float value:{delta.x,delta.y,delta.z})if(!std::isfinite(value)){
                error="Deferred authored motion sample is nonfinite";return false;
            }
            auto& samples=pendingMotion[id];
            if(samples.size()>=character_collection_limit){error="Authored motion samples exceed per-frame limit";return false;}
            samples.push_back({delta,enabled});return true;
        }
        return !motionHandler||motionHandler(*actor,delta,enabled,error);
    }
    bool deliver_motion_phase(double dt,std::string& error){
        if(!motionPhaseRequired)return true;
        if(!motionPhaseHandler){error="Required fresh same-session actor motion phase handler";return false;}
        // Retire all occurrences before callbacks. A failed later actor cannot
        // replay a reached position prefix or this frame's already sampled root.
        auto batch=std::move(pendingMotion);pendingMotion.clear();
        const auto handler=motionPhaseHandler;
        struct Scope{bool& value;~Scope(){value=false;}} scope{motionPhaseDelivering};
        motionPhaseDelivering=true;
        const std::vector<CombatSessionMotionSample> empty;
        try{for(const auto& entry:entries){
            auto* actor=world->find_actor(entry.first);
            if(!actor){error="Actor motion phase lost its registered current owner";return false;}
            const auto found=batch.find(entry.first);
            if(!handler(*actor,updateSerial,dt,found==batch.end()?empty:found->second,error)){
                if(error.empty())error="Actor motion phase consumer failed";return false;
            }
        }}catch(const std::exception& ex){error=ex.what();return false;}
          catch(...){error="Actor motion phase consumer threw";return false;}
        return true;
    }
    bool depart_source_state(ActorId id,std::int32_t next_state,std::string& error){
        auto& entry=entries.at(id);
        if(!entry.stateSequence||!entry.sourceStatePolicy)return true;
        const auto from=entry.sourceStatePolicy->original_state;
        if(from==10){auto* actor=world->find_actor(id);
            reset_actor_action(*actor,next_state==12?CharacterAction::dead:CharacterAction::idle);}
        const auto callback=entry.stateServices.departed;
        const auto checkpoint=entry.stateServices.checkpoint;
        // Commit the outgoing occurrence first. Failure in its Post cannot
        // replay the old Use marker or repeat the already-reached OnBlur.
        entry.retained->cancel();entry.stateManaged=entry.stateSequence=entry.stateFrozen=false;
        entry.sourceStatePolicy.reset();entry.sourceEvents.clear();entry.sourceSelectedClip.clear();
        entry.sourceAction=false;entry.stateServices={};entry.stateServices.checkpoint=checkpoint;
        struct Scope{bool& value;~Scope(){value=false;}} scope{entry.dispatchingDeparture};
        entry.dispatchingDeparture=true;
        try{
            if(callback&&!callback(id,from,next_state,error))return false;
            return publish_source_state(id,next_state,error);
        }
        catch(const std::exception& ex){error=ex.what();return false;}
        catch(...){error="Source departure consumer threw";return false;}
    }
    bool finish_source_state(ActorId id,std::int32_t next_state,
                             const std::function<bool(ActorId,std::string&)>& callback,std::string& error){
        auto& entry=entries.at(id);
        // The retained cursor has already closed normally. Retire its Use
        // occurrence without invoking the interruption/departed owner. The
        // normal Post still sees the outgoing World state until it succeeds.
        const auto checkpoint=entry.stateServices.checkpoint;
        if(entry.sourceStatePolicy&&entry.sourceStatePolicy->original_state==10){
            auto* actor=world->find_actor(id);
            reset_actor_action(*actor,next_state==12?CharacterAction::dead:CharacterAction::idle);
        }
        entry.stateManaged=entry.stateSequence=entry.stateFrozen=false;
        entry.sourceStatePolicy.reset();entry.sourceEvents.clear();entry.sourceSelectedClip.clear();
        entry.sourceAction=false;entry.stateServices={};entry.stateServices.checkpoint=checkpoint;
        struct Scope{bool& value;~Scope(){value=false;}} scope{entry.dispatchingDeparture};
        entry.dispatchingDeparture=true;
        try{
            if(callback&&!callback(id,error))return false;
            return publish_source_state(id,next_state,error);
        }catch(const std::exception& ex){error=ex.what();return false;}
         catch(...){error="Source normal completion consumer threw";return false;}
    }
    CombatPoseBindings pose_admission(ActorId id,CombatPoseBindings poses){
        poses.transition_state=[this,id](std::int32_t& state,std::string& error){
            state=-1;
            if(suppressRuntimeTransitions||detached||!actorTransitionHandler)return true;
            const auto* properties=world->combat_properties(id);
            if(!properties){error="Accepted transition source properties unavailable";return false;}
            state=properties->facts.original_state;return true;
        };
        poses.transition=[this,id](const CombatRuntimeTransition& receipt,std::string& error){
            return actor_transition(id,receipt,error);
        };
        poses.injury_transition=[this,id](const DamageEvent& result,bool& accepted,std::string& error){
            error.clear();accepted=true;const auto found=entries.find(id);
            if(found==entries.end()||!found->second.stateSequence||!found->second.sourceStatePolicy)return true;
            const auto policy=*found->second.sourceStatePolicy;
            // F_ApplyResult passes this original mask predicate to the direct
            // setter. Otherwise Cast has no 50010 registration, and Skill's
            // CSM_Interrupted reads the actual focus flags bit16.
            const bool direct=result.source_mask&&(*result.source_mask&0x18000000u);
            accepted=direct||(policy.original_state==6&&(policy.state_flags&0x10000u));
            return !accepted||actorTransitionHandler||depart_source_state(id,11,error);
        };
        const auto found=entries.find(id);
        if(found!=entries.end()&&found->second.reactionMinimalRandoms){
            poses.injury_choice=[this,id](const DamageEvent&,std::string& clip,bool& move,std::string& error){
                auto& entry=entries.at(id);std::uint32_t step=0;
                if(!*entry.reactionMinimalRandoms&&!world->random_uniform(static_cast<std::uint32_t>(entry.reactionChoices.size()),step,error))return false;
                if(step>=entry.reactionChoices.size()){error="Source injury selected a step outside its original root";return false;}
                const auto& phase=entry.reactionChoices[step];clip=phase.clipName;move=phase.moveGO!=0;return true;
            };
        }
        return poses;
    }
    bool dispatch_object(ObjectId id,const std::shared_ptr<ObjectEntry>& entry,
                         const RetainedAnimationFrame& frame,std::string& error){
        const auto selection=entry->selection;
        const auto& slot=entry->animation->pose().slots()[entry->animation->pose().current_slot()];
        const bool loop=slot.timeline.loop!=0;const auto clip=slot.clip_id;
        const auto services=entry->services;
        auto current=[&](){const auto found=objectEntries.find(id);
            return !detached&&world->find_object(id)&&found!=objectEntries.end()&&found->second==entry&&entry->selection==selection;
        };
        // Consume the reached completion before callbacks can change clips.
        const auto completion=entry->animation->take_completion();
        try{
            for(auto event:frame.events){
                if(!current())return true;
                if(event.clip_id!=clip)continue;
                event.generation=selection;
                if(services.event&&!services.event(id,event,error))return false;
            }
            if(completion.pending&&current()&&services.finished)
                return services.finished(id,selection,loop,error);
            return true;
        }catch(const std::exception& ex){error=ex.what();return false;}
         catch(...){error="Object animation consumer threw";return false;}
    }
    bool set_source_target(ActorId id,ActorId target,bool mode,std::string& error){
        auto found=entries.find(id);auto* actor=world?world->find_actor(id):nullptr;
        if(found==entries.end()||!actor){error="Source target setter requires the same live Session actor";return false;}
        if(target!=invalid_actor_id&&!world->find_actor(target)){error="Source target setter requires a target in the same Session";return false;}
        actor->target_id=target;
        auto& projection=found->second.targetProjection;
        if(!mode&&target!=invalid_actor_id&&projection.lastTargetKnown&&target!=projection.lastTarget)
            projection.lastTarget=target;
        if(found->second.sourceCombo){auto& ai=found->second.sourceAttack;ai.owner=id;ai.target=target;
            if(projection.lastTargetKnown)ai.last_target=projection.lastTarget;}
        if(id==player&&target==invalid_actor_id)stickyPlayerTarget=invalid_actor_id;
        error.clear();return true;
    }
    bool sync_source_last_target(ActorId id,std::string& error){
        auto found=entries.find(id);auto* actor=world?world->find_actor(id):nullptr;
        if(found==entries.end()||!actor){error="Source SyncLastTarget requires the same live Session actor";return false;}
        auto& projection=found->second.targetProjection;projection.lastTarget=actor->target_id;projection.lastTargetKnown=true;
        if(found->second.sourceCombo){found->second.sourceAttack.owner=id;found->second.sourceAttack.target=actor->target_id;found->second.sourceAttack.last_target=actor->target_id;}
        error.clear();return true;
    }
    bool combo_boundary(ActorId id,const RetainedSequenceBoundary& boundary,RetainedSequenceCursorDecision& decision,std::string& error){
        auto& entry=entries.at(id);auto* actor=world->find_actor(id);
        comboBoundaries.push_back({id,entry.comboGeneration,boundary.beginning,boundary.depth,boundary.step,boundary.count,
            entry.retained->phases().at(boundary.phase).source.resolvedPath});
        entry.sourceAttack.owner=id;entry.sourceAttack.target=actor->target_id;
        const auto* attack=entry.sequencePlan->sequence(entry.sequenceCallerSelection->state,entry.sequenceCallerSelection->variant);
        std::int32_t count=0;for(const auto& policy:sequencePolicies)if(policy.first<INT32_MAX)count=std::max(count,static_cast<std::int32_t>(policy.first)+1);
        combo::Boundary providers;
        providers.hasCombo=attack&&dh2_character_animation_has_combo(static_cast<std::int32_t>(attack->id),count,static_cast<std::int32_t>(attack->type));
        providers.lookAt=actor->target_id;
        providers.canRange=[this,id](){const auto* properties=world->combat_properties(id);const auto* traits=world->traits(id);
            return properties->sheets.resolved[32]!=-1||(traits->main_item&&(traits->main_item->words[22]==4||traits->main_item->words[22]==5));};
        providers.targetDead=[this](std::uintptr_t target){const auto* actor=world->find_actor(target);return !actor||!actor->alive();};
        std::string operationFailure;
        providers.onOperation=[this,id,&operationFailure](dh2::character::AttackState64& ai,const combo::Operation& operation){
            if(operation.service==dh2::character::attack_anim_clear_nonsticky_v1){
                if(id==player&&stickyPlayerTarget!=invalid_actor_id){
                    const auto* owner=world->find_actor(id);
                    const auto* target=world->find_actor(stickyPlayerTarget);
                    if(owner&&target&&world->eligible_target(*owner,*target))return true;
                    stickyPlayerTarget=invalid_actor_id;
                }
                // Player targetSelect has an explicit retained/sticky target
                // owner above. Other in-house actors remain nonsticky until
                // their source AI/OOI provider is bound.
                if(!set_source_target(id,invalid_actor_id,false,operationFailure)||
                   !sync_source_last_target(id,operationFailure))return false;
                return true;
            }
            if(operation.service==dh2::character::attack_anim_raise_v1){
                animationDispatches.push_back({id,operation.value,dh2::character::animation_state_event,0,{}});
                std::string failure;
                if(animationNotificationServices.state_event&&!animationNotificationServices.state_event(id,operation.value,nullptr,failure)){
                    operationFailure=failure.empty()?"Original combo state raise consumer failed":std::move(failure);return false;
                }
            }
            // Controller look/pre-attack modifier consumers remain explicit
            // host gaps; combo cursor/last/continued are the genuine kernels.
            (void)ai;return true;
        };
        combo::BoundaryEffects effects;
        if(!combo::retained_boundary(entry.sourceAttack,boundary,providers,effects,decision,error)){
            if(!operationFailure.empty())error=std::move(operationFailure);return false;
        }
        return true;
    }
    std::int32_t live_original_state(ActorId id)const{
        const auto& entry=entries.at(id);
        // P16 LIFECYCLE: with a bound transition handler the published World fact is canonical (Blur/Focus
        // publish the target before Focus), so the lifecycle overlay must not shadow it.
        if(actorTransitionHandler){const auto* properties=world->combat_properties(id);
            return properties?properties->facts.original_state:-1;}
        if(entry.lifecycleOriginalState)return *entry.lifecycleOriginalState;
        if(entry.stateSequence&&entry.sourceStatePolicy)return entry.sourceStatePolicy->original_state;
        if(entry.animationOnly&&!entry.receiveDamage){const auto* properties=world->combat_properties(id);
            return properties?properties->facts.original_state:-1;}
        const auto* actor=world->find_actor(id);
        switch(actor->action){case CharacterAction::idle:return 3;case CharacterAction::moving:return 4;
        case CharacterAction::attacking:return 5;case CharacterAction::dead:return 12;default:break;}
        const auto* properties=world->combat_properties(id);return properties?properties->facts.original_state:-1;
    }
    bool animation_flags(ActorId id,DiagnosticControllerAdmissionFacts& flags,std::string& error){
        // Explicit unbound diagnostic context; registered source owner is read fresh.
        flags={id,id,0,0,0,0};
        if(controllerAdmissionProvider&&!controllerAdmissionProvider(id,flags,error))return false;
        if(flags.owner_id!=id||!flags.controllable||flags.global_blocked>255||flags.local_locked>255||flags.forced>255){
            error="Same-owner animation controller projection invalid";return false;}
        return true;
    }
    bool numeric_animation_event(ActorId id,std::uint32_t event,std::uintptr_t payload,std::string& error){
        DiagnosticControllerAdmissionFacts flags;if(!animation_flags(id,flags,error))return false;
        struct Bridge{Impl* self;ActorId actor;std::string failure;
            static std::int32_t invoke(void* p,const dh2::character::AnimationEventRequest* request){
                using namespace dh2::character;auto& bridge=*static_cast<Bridge*>(p);auto& self=*bridge.self;
                if(!bridge.failure.empty())return 0;
                self.animationDispatches.push_back({bridge.actor,request->event,request->service,0,{}});
                if(request->service==animation_state_event){
                    if(self.animationNotificationServices.state_event&&!self.animationNotificationServices.state_event(
                        bridge.actor,request->event,nullptr,bridge.failure)&&bridge.failure.empty())bridge.failure="Numeric animation state consumer failed";
                    return 0;
                }
                std::int32_t value=request->service==animation_state_getter?self.live_original_state(bridge.actor):0;
                if(self.animationNotificationServices.notification&&!self.animationNotificationServices.notification(
                    bridge.actor,*request,value,bridge.failure)&&bridge.failure.empty())bridge.failure="Numeric animation consumer failed";
                // Unbound diagnostic begin/end consumers have no invented impact
                // or acceptance. End22/23 state forwarding remains unconditional.
                return value;
            }} bridge{this,id,{}};
        const dh2::character::AnimationEventFacts facts{event,unsigned(flags.global_blocked!=0),
            unsigned(flags.local_locked!=0),unsigned(flags.forced!=0),payload};
        const dh2::character::AnimationEventServices services{&bridge,Bridge::invoke};
        if(!original_controller_animation_event(facts,services,error))return false;
        if(!bridge.failure.empty()){error=std::move(bridge.failure);return false;}return true;
    }
    bool named_animation_event(ActorId id,const RetainedAnimationEvent& event,std::string& error){
        using namespace dh2::character;
        DiagnosticControllerAdmissionFacts flags;if(!animation_flags(id,flags,error))return false;
        auto* actor=world->find_actor(id);const auto* properties=world->combat_properties(id);
        // Native logical ownership tokens, not an ARM32 object/FSM overlay.
        AIEventOwner48 owner{id,flags.controllable,reinterpret_cast<std::uintptr_t>(actor),
            reinterpret_cast<std::uintptr_t>(properties),flags.forced,flags.local_locked,0,0};
        AIEventState64 state{reinterpret_cast<std::uintptr_t>(&entries.at(id)),&owner,nullptr,0,nullptr,
            0,0,flags.global_blocked,0,0,0};
        struct Bridge{Impl* self;ActorId id;const RetainedAnimationEvent* event;std::string failure;
            static std::int32_t invoke(void* p,AIEventState64*,const AIEventRequest40* request,std::uint32_t* result){
                auto& bridge=*static_cast<Bridge*>(p);auto& self=*bridge.self;auto& entry=self.entries.at(bridge.id);
                if(!bridge.failure.empty())return -1;
                const auto dispatchIndex=self.animationDispatches.size();
                self.animationDispatches.push_back({bridge.id,request->event,request->service,bridge.event->lag_ms,bridge.event->name});
                if(request->service==ai_event_helper&&request->operation==0x3d4434){
                    if(entry.stateManaged&&entry.stateServices.event&&!entry.stateServices.event(bridge.id,*bridge.event,bridge.failure))return -1;
                    const auto* traits=self.world->traits(bridge.id);const auto* properties=self.world->combat_properties(bridge.id);
                    const bool ranged=properties->sheets.resolved[32]!=-1||(traits->main_item&&
                        (traits->main_item->words[22]==4||traits->main_item->words[22]==5));
                    const auto kind=classify_original_named_animation_event(bridge.event->name,self.live_original_state(bridge.id),ranged);
                    if(entry.sourceAction&&(kind==OriginalNamedAnimationKind::mainhand||kind==OriginalNamedAnimationKind::offhand)){
                        if(entry.sourceSerial==UINT32_MAX){bridge.failure="Retained combat event stream exhausted";return -1;}
                        SourceCombatMarker marker{entry.sourceSerial++,bridge.event->name};
                        if(entry.sourceMarkerSink) {
                            if(!(*entry.sourceMarkerSink)(marker,bridge.failure))return -1;
                            self.animationDispatches[dispatchIndex].synchronous_source_hit=true;
                        } else entry.sourceEvents.push_back(std::move(marker));
                    }
                    // AIS_ANIM_EVENT common source exit0x3d45a4 returns1, even
                    // unrecognized names. Numeric controller gates do not apply.
                    *result=1;return 0;
                }
                if(request->service==ai_event_state_event){
                    if(self.animationNotificationServices.state_event&&!self.animationNotificationServices.state_event(
                        bridge.id,0x28,bridge.event,bridge.failure))return -1;
                    return 0;
                }
                bridge.failure="Unsupported named animation dispatcher service";return -1;
            }} bridge{this,id,&event,{}};
        const AIEventServices24 services{&bridge,Bridge::invoke,(1u<<ai_event_helper)|(1u<<ai_event_state_event),0};
        if(!route_original_named_animation_event(state,event,services,error)){
            if(!bridge.failure.empty())error=bridge.failure;return false;}return true;
    }
    void notify_step_entry(ActorId id,CombatSessionStepEntry event){
        if(detached||!stepObserver)return;
        event.actor=id;event.binding_lease=bindingLease;
        event.occurrence=++stepOccurrence;event.update_serial=updateSerial;
        event.audio_clock=retainedFrameAudioClock;
        const auto observer=stepObserver;
        try{observer(event);}
        catch(const std::exception& e){stepDiagnostics.push_back(e.what());}
        catch(...){stepDiagnostics.push_back("Step-entry presentation observer threw an unknown exception");}
    }
    RetainedSequenceServices retained_services(ActorId id){
        RetainedSequenceServices services;
        // Accepted finite incoming poses share the existing numeric animator
        // router. End34 follows the genuine seeded sequence completion; the
        // live state consumer decides Injury versus Dead effects. Full-action
        // stateServices cleanup remains in the separate closed callback.
        services.seeded_closed=[this,id](const dh2::timeline::Completion&,std::string& e){
            return numeric_animation_event(id,0x22,0,e);
        };
        services.frame=[this,id](std::size_t,const RetainedAnimationFrame& frame,std::string& e){
            const auto policy=entries.at(id).sourceStatePolicy;
            const auto current=[&](){const auto& entry=entries.at(id);return !policy||
                (entry.stateSequence&&entry.sourceStatePolicy&&entry.sourceStatePolicy->generation==policy->generation);};
            // Original slot Update delivers marker callbacks before transform
            // sampling and HandleDisplacement. Detached frame delivery cannot
            // prove all sampler reentry, but actor/PF displacement must not
            // precede the corresponding hit callback.
            for(std::size_t index=0;index<frame.events.size();++index){
                if(!current())return true;
                const auto& event=frame.events[index];
                if(retainedFrameAudioObserver){
                    const auto ordinal=static_cast<std::uint32_t>(index);
                    const auto* clock=retainedFrameAudioClock?&*retainedFrameAudioClock:nullptr;
                    RetainedFrameAudioObserverDiagnostic diagnostic;
                    if(!audio::dispatch_retained_frame_event_with_audio_observer(
                        [this,id,&event](std::string& error){return named_animation_event(id,event,error);},
                        retainedFrameAudioObserver,id,event,ordinal,clock,diagnostic,e))return false;
                    retainedFrameAudioDiagnostics.push_back(std::move(diagnostic));
                }else if(!named_animation_event(id,event,e))return false;
            }
            if(!current())return true;
            if(!dispatch_motion(id,frame.authored_motion,frame.move_go,e))return false;
            return true;
        };
        services.closed=[this,id](const dh2::timeline::Completion&,std::string&e){
            if(!numeric_animation_event(id,0x22,0,e))return false;
            auto& bound=entries.at(id);if(!bound.stateSequence)return true;
            if(bound.sourceStatePolicy&&actorTransitionHandler){
                const auto callback=bound.stateServices.finished;
                CombatRuntimeTransition receipt{id,bound.sourceStatePolicy->original_state,
                    bound.animationOnly&&!bound.receiveDamage?bound.presentationInitialState.value_or(3):
                        (world->find_actor(id)->alive()?3:12),bound.sourceStatePolicy->generation,
                    CombatRuntimeTransitionCause::completion};
                if(!actor_transition(id,receipt,e,&callback))return false;
                receipt.stage=CombatRuntimeTransitionStage::after_change;
                return actor_transition(id,receipt,e);
            }
            const auto callback=bound.stateServices.finished;bound.stateSequence=false;
            const bool generic=bound.sourceStatePolicy.has_value();
            if(bound.sourceStatePolicy&&bound.sourceStatePolicy->original_state==10){
                auto* actor=world->find_actor(id);
                reset_actor_action(*actor,actor->alive()?CharacterAction::idle:CharacterAction::dead);
            }
            if(bound.sourceStatePolicy){
                bound.stateManaged=bound.stateFrozen=false;bound.sourceStatePolicy.reset();
                bound.sourceSelectedClip.clear();bound.sourceEvents.clear();bound.sourceAction=false;
                const auto checkpoint=bound.stateServices.checkpoint;bound.stateServices={};bound.stateServices.checkpoint=checkpoint;
            }
            if(callback&&!callback(id,e))return false;
            return !generic||publish_source_state(id,
                bound.animationOnly&&!bound.receiveDamage?bound.presentationInitialState.value_or(3):live_original_state(id),e);};
        services.boundary=[this,id](const RetainedSequenceBoundary& boundary,RetainedSequenceCursorDecision& decision,std::string& error){
            if(boundary.beginning&&stepObserver){
                CombatSessionStepEntry event;
                event.sequence_id=boundary.sequenceId;event.depth=boundary.depth;event.step=boundary.step;
                event.container_path=boundary.containerPath;event.leaf_path=boundary.leafPath;
                notify_step_entry(id,std::move(event));
            }
            if(!entries.at(id).sourceCombo||!entries.at(id).sourceAction)return true;
            return combo_boundary(id,boundary,decision,error);
        };
        return services;
    }
    bool seed_source(Entry& entry,const OriginalCombatPhase& phase,bool loop,float rate,
                     RetainedSequencePlayback& owner,RetainedAnimationFrame& frame,std::string& error){
        const OriginalCombatSequencePlan* source=nullptr;
        for(const auto& sequence:entry.sequencePlan->sequences)for(const auto& p:sequence.phases)if(p.clipName==phase.clipName)source=&sequence;
        if(!source||source->loop< -1||source->loop>INT32_MAX||(loop&&!((source->type==0&&source->phases.size()==1)||source->type==2))){error="Seeded source repetition requires single-leaf type0 or explicitly pinned type2 selection metadata";return false;}
        if(loop&&source->loop==0){error="Infinite seeded playback cannot be invented for a finite source sequence";return false;}
        return owner.seed_sequence(*assets,phase.clipName,phase.resolvedPath,rate,static_cast<std::int32_t>(phase.blendOut),phase.moveGO!=0,
                                  loop?static_cast<std::int32_t>(source->loop):0,frame,error);
    }
    bool prepare_retained(Entry& entry,ActorId id,const OriginalAttackSelection& selection,std::string& error) {
        if(!entry.retainedPhaseClock)return true;
        entry.sourceActorRate=static_cast<float>(selection.actor_rate);
        entry.sourceEvents.clear();entry.sourceSerial=0;entry.sourceAction=false;
        auto root=[visual=entry.visual](const std::string& clip,std::int32_t time,std::array<float,3>& scratch,std::string& error){
            Vec3 point{scratch[0],scratch[1],scratch[2]};
            if(!visual->sample_source_root_translation(clip,time,point,error))return false;
            scratch={point.x,point.y,point.z};return true;
        };
        auto retained=std::make_unique<RetainedSequencePlayback>(*entry.visual,std::move(root));
        const auto services=retained_services(id);
        if(!retained->prepare(*assets,*entry.sequencePlan,sequencePolicies,selection,services,entry.attack.animation_clip_id,error))return false;
        const OriginalCombatPhase* idle=nullptr;
        for(const auto& sequence:entry.sequencePlan->sequences)for(const auto& phase:sequence.phases)if(phase.clipName==entry.idle)idle=&phase;
        if(!idle){error="Retained source idle metadata missing";return false;}
        RetainedAnimationFrame seed;
        if(!seed_source(entry,*idle,true,static_cast<float>(idle->speed)*entry.sourceActorRate,*retained,seed,error))return false;
        entry.sourceSelectedClip=idle->clipName;entry.retained=std::move(retained);entry.attackProgram=true;return true;
    }
    bool prepare_animation_only(Entry& entry,ActorId id,std::string& error){
        if(!entry.animationOnly||!entry.sequencePlan||!assets){error="Animation-only source plan is unavailable";return false;}
        const OriginalCombatPhase* idle=nullptr;
        for(const auto& sequence:entry.sequencePlan->sequences)for(const auto& phase:sequence.phases)
            if(phase.clipName==entry.idle&&phase.resolvedPath==entry.idlePath)idle=&phase;
        if(!idle){error="Animation-only authored Idle phase is absent from its source plan";return false;}
        auto root=[visual=entry.visual](const std::string& clip,std::int32_t time,std::array<float,3>& scratch,std::string& failure){
            Vec3 point{scratch[0],scratch[1],scratch[2]};
            if(!visual->sample_source_root_translation(clip,time,point,failure))return false;
            scratch={point.x,point.y,point.z};return true;
        };
        auto retained=std::make_unique<RetainedSequencePlayback>(*entry.visual,std::move(root));
        if(!retained->prepare_seeded(*assets,*entry.sequencePlan,*idle,retained_services(id),error))return false;
        entry.retained=std::move(retained);entry.sourceActorRate=1;entry.sourceEvents.clear();entry.sourceSerial=0;
        entry.sourceAction=false;entry.attackProgram=false;
        RetainedAnimationFrame frame;
        if(!seed_source(entry,*idle,true,static_cast<float>(idle->speed),*entry.retained,frame,error))return false;
        entry.sourceSelectedClip=idle->clipName;return true;
    }
    CombatVisualBinding retained_binding(Entry& entry) {
        auto validation=entry.sequence?entry.sequence->binding():combat_visual_binding(*entry.visual);
        CombatVisualBinding binding;
        binding.range=validation.range;binding.markers=validation.markers;
        binding.select=[this,&entry](const std::string& name,bool loop,std::string& error){
            const bool injury=name==entry.poses.react_clip_id||std::any_of(entry.reactionChoices.begin(),entry.reactionChoices.end(),[&](const auto& phase){return phase.clipName==name;});
            const bool incoming=entry.receiveDamage&&(injury||name==entry.poses.death_clip_id);
            if(entry.animationOnly&&name!=entry.idle&&!incoming){error="No-outgoing actor does not accept an unbound combat pose";return false;}
            entry.sourceEvents.clear();entry.sourceSerial=0;
            if(name==entry.attack.animation_clip_id) {if(loop){error="Retained finite action cannot loop";return false;}
                if(entry.sourceCombo){if(entry.comboGeneration==UINT64_MAX){error="Source combo action generation exhausted";return false;}++entry.comboGeneration;}
                if(!entry.attackProgram){auto selection=*entry.sequenceCallerSelection;selection.actor_rate=entry.sourceActorRate;if(!entry.retained->prepare_preserving(*assets,*entry.sequencePlan,sequencePolicies,selection,retained_services(world_id(entry)),entry.attack.animation_clip_id,error))return false;entry.attackProgram=true;}
                entry.stateManaged=entry.stateSequence=entry.stateFrozen=false;entry.sourceAction=true;entry.sourceSelectedClip=name;entry.locomotionSelected.clear();return entry.retained->begin(error);}
            const OriginalCombatPhase* selected=nullptr;
            for(const auto& sequence:entry.sequencePlan->sequences)for(const auto& phase:sequence.phases)if(phase.clipName==name)selected=&phase;
            if(!selected){error="Retained pose source metadata missing: "+name;return false;}
            entry.sourceAction=false;entry.sourceSelectedClip=name;entry.locomotionSelected.clear();RetainedAnimationFrame frame;
            return seed_source(entry,*selected,loop,static_cast<float>(selected->speed)*entry.sourceActorRate,*entry.retained,frame,error);
        };
        binding.update=[&entry](double seconds,std::string& error){return entry.sourceAction?entry.retained->advance(seconds,error):entry.retained->advance_seeded(seconds,error);};
        binding.source_clock=[](){return true;};
        binding.take_source_events=[&entry](std::vector<SourceCombatMarker>& events,std::string&){events=std::move(entry.sourceEvents);entry.sourceEvents.clear();return true;};
        binding.source_finished=[&entry](){return entry.sourceAction?entry.retained->finished():entry.retained->animation()->pose().current_ended();};
        binding.update_source=[&entry](double seconds,const SourceCombatMarkerSink& sink,std::string& error){
            // Borrow only during this synchronous source advancement. Restore
            // the prior sink even when a reached consumer rejects the frame.
            struct Scope {
                Entry& entry;const SourceCombatMarkerSink* previous;
                ~Scope(){entry.sourceMarkerSink=previous;}
            } scope{entry,entry.sourceMarkerSink};
            entry.sourceMarkerSink=&sink;
            auto seeded=std::move(entry.sourceEvents);entry.sourceEvents.clear();
            for(const auto& marker:seeded)if(!sink(marker,error))return false;
            return entry.sourceAction?entry.retained->advance(seconds,error):entry.retained->advance_seeded(seconds,error);
        };
        binding.hold_terminal=[&entry](const std::string& clip,std::string& error){
            std::int32_t start=0,end=0;SkeletalPose pose;
            return entry.visual->animation_range(clip,start,end,error)&&entry.visual->sample_local_pose(clip,end,pose,error)&&entry.visual->apply_local_pose(pose,error);
        };
        return binding;
    }
    ActorId world_id(const Entry& entry)const{for(const auto& pair:entries)if(&pair.second==&entry)return pair.first;return invalid_actor_id;}
    OriginalAttackSequenceServices sequence_services(CharacterVisual* visual,ActorId id) {
        return {combat_visual_binding(*visual),
            [visual](const std::string& name,std::string& error){return visual->restart(name,false,error);},
            [visual](){return visual->take_root_motion();},
            [this,id](Vec3 delta,bool enabled,std::string& error){
                return dispatch_motion(id,delta,enabled,error);
            }};
    }
    CombatVisualBinding pose_motion_binding(CombatVisualBinding binding,CharacterVisual* visual,ActorId id) {
        auto select=std::move(binding.select);
        binding.select=[this,id,select=std::move(select)](const std::string& clip,bool loop,std::string& error){
            const auto before=entries.find(id);
            if(before!=entries.end()&&before->second.stateSequence&&before->second.sourceStatePolicy){
                const auto& poses=before->second.poses;
                const bool injury=(!poses.react_clip_id.empty()&&clip==poses.react_clip_id)||
                    std::any_of(before->second.reactionChoices.begin(),before->second.reactionChoices.end(),[&](const auto& phase){return phase.clipName==clip;});
                if((!poses.death_clip_id.empty()&&clip==poses.death_clip_id)||injury){
                    if(!depart_source_state(id,clip==poses.death_clip_id?12:11,error))return false;
                }
            }
            if(!select||!select(clip,loop,error))return false;
            const auto actorEntry=entries.find(id);
            if(actorEntry!=entries.end()){
                const auto source=actorEntry->second.poseStepEntries.find(clip);
                if(source!=actorEntry->second.poseStepEntries.end())notify_step_entry(id,source->second);
            }
            return true;
        };
        binding.take_root_motion=[visual](){return visual->take_root_motion();};
        binding.apply_motion=[this,id](Vec3 delta,bool enabled,std::string& error){
            return dispatch_motion(id,delta,enabled,error);
        };
        return binding;
    }
    ActorId nearest(ActorId source)const {
        const auto* a=world->find_actor(source);if(!a)return invalid_actor_id;
        ActorId result=invalid_actor_id;float best=0;
        for(const auto& pair:world->actors())if(world->eligible_target(*a,pair.second)&&world->original_melee_in_range(source,pair.first)){
            float square=0;for(unsigned i=0;i<3;++i){const float d=a->transform.position[i]-pair.second.transform.position[i];square+=d*d;}
            if(result==invalid_actor_id||square<best){result=pair.first;best=square;}
        }
        return result;
    }
    bool turn(ActorId source,ActorId target,double dt,std::string& error){
        auto* a=world->find_actor(source);const auto* b=world->find_actor(target);
        if(!a||!b||!a->alive()||a->action==CharacterAction::hurt||
           !world->eligible_target(*a,*b)||!world->original_melee_in_range(source,target))return true;
        auto& entry=entries.at(source);
        // Admission and active-attack update can both request LookAt. They
        // share this frame's elapsed rotation budget rather than turning twice.
        if(updating&&dt>0&&entry.rotationUpdateSerial==updateSerial)return true;
        dh2::actor::RotationState rotation{{a->transform.rotation[0],a->transform.rotation[1],a->transform.rotation[2]},
                                          a->transform.rotation[2],entry.turnPositive,0};
        float direction[3];for(unsigned i=0;i<3;++i)direction[i]=b->transform.position[i]-a->transform.position[i];
        if(dh2_nav_look_towards(&rotation.heading_angle,direction)){error="Original target heading rejected";return false;}
        const double milliseconds=dt*1000+entry.rotationFractionMs;
        const auto integerMs=static_cast<std::uint32_t>(milliseconds);
        const auto* properties=world->combat_properties(source);
        const dh2::actor::RotationPolicy policy{original_speed_modifier(properties->sheets.resolved[47]),integerMs,1,1};
        std::uint32_t sync=0;
        if(dh2_actor_update_rotation(&rotation,&policy,&sync)){error="Original bounded turn kernel rejected";return false;}
        a->transform.rotation[2]=rotation.rotation[2];entry.turnPositive=rotation.turn_positive;
        entry.rotationFractionMs=milliseconds-integerMs;
        if(updating&&dt>0)entry.rotationUpdateSerial=updateSerial;
        return true;
    }
    bool facts(ActorId id,std::string& error){
        auto* actor=world->find_actor(id);const auto* old=world->combat_properties(id);
        const auto* currentTraits=world->traits(id);
        if(!actor||!old||!currentTraits)return false;
        // Animation-only Actors keep source-authored vital values verbatim;
        // combat/action state is not used to infer their pose or visibility.
        if(entries.at(id).animationOnly&&!entries.at(id).receiveDamage&&!entries.at(id).sourceStatePolicy)return true;
        auto properties=*old;
        if(entries.at(id).lifecycleOriginalState&&!actorTransitionHandler)properties.facts.original_state=*entries.at(id).lifecycleOriginalState;
        else if(entries.at(id).stateSequence&&entries.at(id).sourceStatePolicy)
            properties.facts.original_state=entries.at(id).sourceStatePolicy->original_state;
        else if(actorTransitionHandler){
            // Accepted transitions own this existing World fact on the opted-in
            // path. A late action projection cannot erase outgoing source state.
        }
        else switch(actor->action){
        case CharacterAction::idle:properties.facts.original_state=3;break;
        case CharacterAction::moving:properties.facts.original_state=4;break;
        case CharacterAction::attacking:properties.facts.original_state=5;break;
        case CharacterAction::hurt:
            if(!entries.at(id).poses.react_clip_id.empty()){properties.facts.original_state=11;break;}
            return true;
        case CharacterAction::dead:properties.facts.original_state=12;break;
        case CharacterAction::knocked_back:properties.facts.original_state=10;break;
        default:
            if(reactionGapLogged.insert(id).second)logs.push_back("Unresolved original reaction/cast state producer for actor "+std::to_string(id));
            return true;
        }
        // Equipment changes publish traits into the shared world. Updating action
        // facts must preserve those current traits, not the initialization cache.
        auto traits=*currentTraits;
        traits.targetable=entries.at(id).traits.targetable;
        return world->update_combat_properties(id,std::move(properties),traits,error);
    }
    bool select_source_attack_for_request(ActorId id,std::string& error){
        auto& entry=entries.at(id);if(!entry.sourceAttackStateSelection)return true;
        if(entry.stateSequence||entry.stateManaged){error="Source attack cannot replace a managed state before departure";return false;}
        auto selection=*entry.sequenceCallerSelection;
        selection.state=live_original_state(id)==4?"Attack":"AttackStatic";
        const auto* properties=world->combat_properties(id);
        volatile float rate=static_cast<float>(selection.actor_rate)*original_speed_modifier(properties->sheets.resolved[48]);
        if(!std::isfinite(rate)||rate<=0){error="Live source attack speed is invalid";return false;}
        if(selection.state==entry.sequenceCallerSelection->state&&rate==entry.sourceActorRate&&entry.attackProgram)return true;
        auto effective=selection;effective.actor_rate=rate;
        auto prepared=std::make_unique<OriginalAttackSequence>();
        if(!prepared->prepare(*entry.sequencePlan,sequencePolicies,effective,sequence_services(entry.visual,id),entry.attack.animation_clip_id,error))return false;
        const auto validation=prepared->binding();const auto* markers=validation.markers(entry.attack.animation_clip_id,error);
        if(!markers)return false;
        for(const auto& binding:entry.attack.damage_markers)
            if(std::none_of(markers->markers().begin(),markers->markers().end(),[&](const auto& marker){return marker.name==binding.marker_name;})){
                error="Selected source attack root has no configured hand event";return false;
            }
        if(!entry.retained->prepare_preserving(*assets,*entry.sequencePlan,sequencePolicies,effective,
            retained_services(id),entry.attack.animation_clip_id,error))return false;
        // Runtime range/marker callbacks capture this sequence object's address.
        // Replace its prepared contents, never the owned object behind them.
        *entry.sequence=std::move(*prepared);entry.sequenceCallerSelection=selection;
        entry.sourceActorRate=rate;entry.attackProgram=true;
        return true;
    }
    bool cycle_player_target(ActorId& selected,std::string& error){
        auto* actor=world->find_actor(player);
        // Explicit input cycling starts at the nearest eligible character.
        // Subsequent commands cycle deterministically, including distance ties.
        std::vector<std::pair<double,ActorId>> ordered;
        for(const auto& candidate:world->actors())if(world->eligible_target(*actor,candidate.second)){
            double distance=0;for(unsigned axis=0;axis<3;++axis){
                const double delta=double(candidate.second.transform.position[axis])-actor->transform.position[axis];
                distance+=delta*delta;
            }
            ordered.push_back({distance,candidate.first});
        }
        std::sort(ordered.begin(),ordered.end());std::vector<ActorId> eligible;eligible.reserve(ordered.size());
        for(const auto& candidate:ordered)eligible.push_back(candidate.second);
        auto current=std::find(eligible.begin(),eligible.end(),actor->target_id);
        const auto target=eligible.empty()?invalid_actor_id:current==eligible.end()||++current==eligible.end()?eligible.front():*current;
        if(!set_source_target(player,target,false,error))return false;
        if(target==invalid_actor_id&&!sync_source_last_target(player,error))return false;
        stickyPlayerTarget=target;selected=target;return true;
    }
    bool request(ActorId source,ActorId target,std::string& error){
        if(transitionDelivering||restoreTeardown||checkingAnimationCheckpoint){error="Attack command cannot reenter actor transition/checkpoint/teardown";return false;}
        if(entries.at(source).animationOnly){error="Animation-only actor cannot enter combat";return false;}
        auto* a=world->find_actor(source);auto* b=world->find_actor(target);
        const auto& entry=entries.at(source);
        if(!a||!a->alive()||!entry.permission||runtime->owns_pose(source)||combat->cooldown_remaining(source)>0||
           (a->action!=CharacterAction::idle&&a->action!=CharacterAction::moving))return true;
        // Source AI admission/search/OOI run before this C354 consumer. The
        // Idle3/Move4 FSM independently rejects owner +0x528 bit1.
        if(entry.sourceCombo&&(entry.sourceAttack.owner_flags528&2u))return true;
        if(target==invalid_actor_id){
            if(!entry.sourceCombo||entry.attack.geometry!=AttackGeometry::melee_radius)return true;
        }else if(!b||!entries.at(target).permission||!world->eligible_target(*a,*b)||
                 !world->original_melee_in_range(source,target))return true;
        if(!select_source_attack_for_request(source,error)||!runtime->begin(source,target,entries.at(source).attack,error))return false;
        return facts(source,error);
    }
    bool diagnostic_attack_body(ActorId source,ActorId requested,double dt,std::string& error){
        if(entries.at(source).animationOnly){error="Animation-only actor cannot enter combat";return false;}
        auto* a=world->find_actor(source);if(!a)return true;
        auto* target=world->find_actor(requested);
        if(!target||!world->eligible_target(*a,*target))requested=nearest(source);
        a->target_id=requested;
        if(!turn(source,requested,dt,error))return false;
        return request(source,requested,error);
    }
    // Accepted departure of a retained source combo swing (CSAttack::OnBlur):
    // cancel the same retained cursor, start the AttackDelay cooldown once
    // (CombatSystem::interrupt), and clear continuation so the next swing starts
    // from a fresh command. Stale or non-combo swings are no-ops.
    bool depart_source_attack(ActorId id,std::string& error){
        auto& entry=entries.at(id);
        if(!entry.sourceCombo||!entry.sourceAction)return true;
        // CombatSystem::interrupt (also reached through runtime->interrupt) clears
        // target_id unconditionally, which the source departure does not do
        // (CSAttack::OnBlur only starts the AttackDelay timer). Keep the target
        // across the interrupts, then apply the source nonsticky rule
        // (CharAI::_ClearNonStickyTarget) exactly as the completion path does.
        const auto* actor=world->find_actor(id);const ActorId target=actor?actor->target_id:invalid_actor_id;
        entry.retained->cancel();entry.sourceAction=false;entry.sourceEvents.clear();
        entry.sourceAttack.continued=0;entry.sourceAttack.last=0;
        if(runtime->owns_pose(id)&&!runtime->interrupt(id,error))return false;
        combat->interrupt(id);
        if(id==player&&stickyPlayerTarget!=invalid_actor_id){
            if(auto* after=world->find_actor(id))after->target_id=target;
            return true;
        }
        if(!set_source_target(id,invalid_actor_id,false,error)||!sync_source_last_target(id,error))return false;
        return true;
    }
    bool command_request(ActorId source,ActorId requested,double dt,std::string& error){
        if(entries.at(source).stateSequence&&entries.at(source).sourceStatePolicy){error.clear();return true;}
        const auto found=entries.find(source);
        if(found==entries.end()){error="Attack command requires a bound live session actor";return false;}
        if(found->second.animationOnly){error="Animation-only actor rejects combat commands";return false;}
        if(entries.at(source).sourceCombo)return combo_command_request(source,requested,dt,error);
        if(!controllerAdmissionProvider)return diagnostic_attack_body(source,requested,dt,error);
        DiagnosticControllerAdmissionFacts admission;
        if(!controllerAdmissionProvider(source,admission,error))return false;
        if(admission.owner_id!=source||admission.controllable==0){error="Diagnostic controller admission requires the same actor/controllable owner";return false;}
        dh2::character::ControllerAttackState32 controller{admission.controllable,source,
            admission.global_blocked,admission.local_locked,admission.forced,admission.network_enabled};
        dh2::character::AttackState64 state{};state.owner=source;
        if(const auto* actor=world->find_actor(source))state.target=actor->target_id;
        struct Bridge {
            Impl* session;ActorId source;double dt;std::string failure;
            static void invoke(void* p,dh2::character::AttackState64*,dh2::character::ControllerAttackState32* controller,
                const dh2::character::AttackRequest32* request,dh2::character::AttackResponse16* output){
                using namespace dh2::character;auto& self=*static_cast<Bridge*>(p);
                if(!self.failure.empty())return;
                if(request->service==attack_network_mode){
                    bool online=false;
                    if(!self.session->networkModeProvider||!self.session->networkModeProvider(self.source,online,self.failure)){
                        if(self.failure.empty())self.failure="Actual controller network-mode query unavailable";return;}
                    if(online&&controller->network_enabled){self.failure="Diagnostic session has no original online speculative attack/packet backend";return;}
                    output->word=online?1u:0u;return;
                }
                if(request->service==attack_controllable_dispatch){
                    if(request->subject!=controller->controllable||controller->character!=self.source){self.failure="Diagnostic attack controllable ownership changed";return;}
                    if(!self.session->diagnostic_attack_body(self.source,request->payload,self.dt,self.failure)&&self.failure.empty())
                        self.failure="Diagnostic attack body failed";
                    return;
                }
                self.failure="Diagnostic session cannot supply original attack service "+std::to_string(request->service);
            }
        } bridge{this,source,dt,{}};
        const dh2::character::AttackServices16 services{&bridge,Bridge::invoke};
        const auto result=original_controller_attack(controller,&state,requested,services,error);
        if(!bridge.failure.empty()){error=std::move(bridge.failure);return false;}
        return result.admission!=OriginalCommandAdmission::failed;
    }
    bool combo_command_request(ActorId source,ActorId requested,double dt,std::string& error){
        using namespace dh2::character;
        auto& entry=entries.at(source);auto* actor=world->find_actor(source);
        entry.sourceAttack.owner=source;entry.sourceAttack.target=actor->target_id;
        entry.sourceAttack.last_target=entry.targetProjection.lastTargetKnown?entry.targetProjection.lastTarget:invalid_actor_id;
        entry.sourceAttack.object_of_interest=invalid_actor_id;entry.sourceAttack.object_of_interest_type=-1;
        entry.targetProjection.objectOfInterest=invalid_actor_id;entry.targetProjection.objectOfInterestType=-1;
        entry.targetProjection.objectOfInterestKnown=false;
        AttackOwnerFacts owner;
        if(attackOwnerProvider&&!attackOwnerProvider(source,owner,error))return false;
        if(owner.heading_active>255||owner.object_of_interest_type< -128||owner.object_of_interest_type>127){error="Live source attack owner fields invalid";return false;}
        entry.sourceAttack.owner_flags528=owner.flags528;entry.sourceAttack.heading_active=owner.heading_active;
        entry.sourceAttack.object_of_interest=owner.object_of_interest;entry.sourceAttack.object_of_interest_type=owner.object_of_interest_type;
        if(attackOwnerProvider){entry.targetProjection.objectOfInterest=owner.object_of_interest;
            entry.targetProjection.objectOfInterestType=owner.object_of_interest_type;entry.targetProjection.objectOfInterestKnown=true;
            entry.targetProjection.objectOfInterestSerial=updateSerial;}
        DiagnosticControllerAdmissionFacts admission;if(!animation_flags(source,admission,error))return false;
        ControllerAttackState32 controller{admission.controllable,source,admission.global_blocked,admission.local_locked,admission.forced,admission.network_enabled};
        struct Bridge {
            Impl* self;ActorId source;double dt;std::vector<std::uintptr_t> candidates;AttackTargetList24 list{};std::string failure;
            static void invoke(void* context,AttackState64* ai,ControllerAttackState32* controller,const AttackRequest32* request,AttackResponse16* output){
                auto& b=*static_cast<Bridge*>(context);*output={};if(!b.failure.empty())return;
                auto& s=*b.self;auto& entry=s.entries.at(b.source);auto* actor=s.world->find_actor(b.source);
                const auto* traits=s.world->traits(b.source);const auto* props=s.world->combat_properties(b.source);
                switch(request->service){
                case attack_owner_dead:output->word=!actor->alive()||!entry.permission;break;
                case attack_owner_ranged:output->word=props->sheets.resolved[32]!=-1||(traits->main_item&&(traits->main_item->words[22]==4||traits->main_item->words[22]==5));break;
                case attack_is_attacking:output->word=actor->action==CharacterAction::attacking&&entry.sourceAction&&entry.retained->active();break;
                case attack_diagnostic:break; // Explicit diagnostic tracing policy.
                case attack_list_create:b.candidates.clear();b.list={reinterpret_cast<std::uintptr_t>(&b),nullptr,0,0};output->identity=reinterpret_cast<std::uintptr_t>(&b.list);break;
                case attack_list_reset_sort:break;
                case attack_list_search:{
                    std::vector<std::pair<float,ActorId>> eligible;
                    for(const auto& pair:s.world->actors())if(s.world->eligible_target(*actor,pair.second)&&s.world->original_melee_in_range(b.source,pair.first)){
                        float distance=0;for(unsigned i=0;i<3;++i){const float d=actor->transform.position[i]-pair.second.transform.position[i];distance+=d*d;}eligible.push_back({distance,pair.first});
                    }
                    std::sort(eligible.begin(),eligible.end());for(const auto& pair:eligible)b.candidates.push_back(pair.second);
                    b.list.entries=b.candidates.empty()?nullptr:b.candidates.data();b.list.count=static_cast<std::uint32_t>(b.candidates.size());b.list.cursor=0;break;
                }
                case attack_list_pop:if(b.list.cursor<b.list.count)++b.list.cursor;break;
                case attack_list_destroy:break;
                case attack_set_target:
                    if(!s.set_source_target(b.source,request->payload,request->argument0!=0,b.failure))return;
                    break;
                case attack_sync_last_target:
                    if(!s.sync_source_last_target(b.source,b.failure))return;
                    break;
                case attack_can_attack_current:{const auto* target=s.world->find_actor(ai->target);output->word=target&&s.world->eligible_target(*actor,*target)&&s.world->original_melee_in_range(b.source,ai->target);break;}
                case attack_target_dead:{const auto* target=s.world->find_actor(request->payload);output->word=!target||!target->alive();break;}
                case attack_owner_player:output->word=traits->is_player;break;
                case attack_current_in_melee:output->word=s.world->original_melee_in_range(b.source,ai->target);break;
                case attack_set_attack_state:
                    if(!s.turn(b.source,ai->target,b.dt,b.failure)||!s.request(b.source,ai->target,b.failure))return;
                    break;
                case attack_controllable_dispatch:{
                    if(!controller||request->subject!=controller->controllable){b.failure="Source combo controllable ownership changed";return;}
                    const AttackServices16 services{&b,invoke};
                    if(dh2_character_ai_melee_attack(ai,request->payload,0,&services))b.failure="Source melee combo command failed";
                    break;
                }
                case attack_network_mode:{bool online=false;if(s.networkModeProvider&&!s.networkModeProvider(b.source,online,b.failure))return;
                    if(online){b.failure="Source combo online attack backend is unbound";return;}break;}
                case attack_frontal_angle:b.failure="Actual source frontal-angle/search-heading provider unavailable";break;
                case attack_range_redirect:b.failure="Original ranged action redirect is not bound for source combo";break;
                default:b.failure="Original source combo service unavailable: "+std::to_string(request->service);break;
                }
            }
        } bridge{this,source,dt,{},{},{}};
        const AttackServices16 services{&bridge,Bridge::invoke};
        if(!combo::controller_command(controller,entry.sourceAttack,true,requested,services,error))return false;
        if(!bridge.failure.empty()){error=std::move(bridge.failure);return false;}
        return true;
    }
};
CombatSession::CombatSession()=default;
CombatSession::~CombatSession(){if(impl_&&impl_->ownerLifetime)impl_->ownerLifetime->alive_=false;}
void CombatSession::set_motion_handler(MotionHandler handler){
    if(impl_){if(impl_->transitionDelivering||impl_->restoreTeardown||impl_->checkingAnimationCheckpoint)
        throw std::logic_error("Cannot change motion consumer during actor transition/checkpoint/teardown");
        impl_->motionHandler=std::move(handler);}
}
bool CombatSession::set_motion_phase_handler(MotionPhaseHandler handler,std::string& error){
    // P16 LIFECYCLE: samples queued outside an update (a spawn's begin/select runs between frames) stay queued and
    // are delivered by the handler bound at the next update. The per-frame rebinding in main.cpp relies on this.
    error.clear();if(!impl_||impl_->detached||impl_->updating||impl_->restoreTeardown||impl_->checkingAnimationCheckpoint||impl_->transitionDelivering||!handler){
        error="Motion phase binding requires idle current Session and handler";return false;
    }
    impl_->motionPhaseHandler=std::move(handler);impl_->motionPhaseRequired=true;return true;
}
bool CombatSession::clear_motion_phase_handler(std::string& error){
    error.clear();if(!impl_||impl_->detached||impl_->updating||impl_->restoreTeardown||impl_->checkingAnimationCheckpoint||impl_->transitionDelivering||!impl_->pendingMotion.empty()){
        error="Motion phase removal requires idle current Session without pending samples";return false;
    }
    impl_->motionPhaseHandler={};impl_->motionPhaseRequired=false;return true;
}
bool CombatSession::bind_reconstructible_actor_transition_handler(ActorTransitionHandler handler,
    std::function<bool(std::string&)> validator,std::string& error){
    error.clear();
    if(!impl_||impl_->detached||impl_->updating||impl_->restoreTeardown||impl_->checkingAnimationCheckpoint||
       impl_->transitionDelivering||impl_->applyingSourceHit||!impl_->pendingTransitions.empty()||
       !impl_->pendingMotion.empty()||!handler||!validator||!impl_->bindingLease||!impl_->transitionFailure.empty()){
        error="Actor transition binding requires idle current Session, consumer and validator";return false;
    }
    auto& s=*impl_;const auto lease=s.bindingLease;
    struct Scope{bool& flag;~Scope(){flag=false;}} scope{s.checkingAnimationCheckpoint};s.checkingAnimationCheckpoint=true;
    try{
        if(!validator(error)){if(error.empty())error="Actor transition binding validator rejected";return false;}
        s.actorTransitionHandler=std::move(handler);s.actorTransitionCheckpoint=std::move(validator);
        s.actorTransitionLease=lease;s.transitionRequired=true;error.clear();return true;
    }catch(const std::exception& ex){error=ex.what();return false;}
     catch(...){error="Actor transition binding validator threw";return false;}
}
namespace {
const OriginalCombatPhase& choose(const OriginalCombatVisualPlan& plan,const CombatSessionChoice& choice){
    const auto* result=plan.phase(choice.state,choice.variant,choice.leafPath);
    if(!result||!result->has_visual()||!std::isfinite(result->speed)||result->speed<=0)
        throw std::runtime_error("Explicit source phase unavailable/nonvisual/invalid rate: "+plan.profileId+"/"+choice.state);
    return *result;
}
}
bool CombatSession::initialize(const AssetCatalog& assets,const OriginalPropertyDatabase& database,
 const OriginalMeleeBindings& bindings,const CombatSessionConfig& config,CharacterVisual& playerVisual,
 ActorPopulation& population,Vec3 playerPosition,const ActorCustomization& playerCustomization,std::string& error){
    error.clear();
    if(impl_&&(impl_->updating||impl_->restoreTeardown||impl_->checkingAnimationCheckpoint||impl_->transitionDelivering)){
        error="Cannot replace Session during actor frame, checkpoint validation or restore teardown";return false;
    }
    try {
        if(config.diagnosticRngSeed.has_value()==config.initialRandomState.has_value()||config.playerId==invalid_actor_id||config.playerProfileId.empty()||config.tableRoot.empty())
            throw std::runtime_error("Combat session requires exactly one initial RNG state or diagnostic seed, player ID/profile, and source table root");
        const auto playerProfile=config.profiles.find(config.playerProfileId);
        if(playerProfile==config.profiles.end())throw std::runtime_error("Player combat profile has no explicit source choices");
        for(float v:{playerPosition.x,playerPosition.y,playerPosition.z})if(!std::isfinite(v))throw std::runtime_error("Player position is nonfinite");
        auto next=std::make_unique<Impl>();
        if(impl_)next->ownerLifetime=impl_->ownerLifetime;
        next->random=config.initialRandomState?*config.initialRandomState:dh2::data::CombatRandom{*config.diagnosticRngSeed,0};
        next->player=config.playerId;
        next->assets=&assets;
        dh2::data::AiTables tables;
        if(!load_original_ai_tables(assets,config.tableRoot,tables,error))return false;
        next->world=std::make_unique<PlayableActorWorld>(std::move(tables),next->random);
        next->combat=std::make_unique<CombatSystem>(*next->world);
        next->runtime=std::make_unique<ActorCombatRuntime>(*next->combat);
        OriginalSequencePolicies sequencePolicies;
        if(std::any_of(config.profiles.begin(),config.profiles.end(),[](const auto& p){return bool(p.second.sequenceAction)||p.second.animationOnly;})&&
           !original_sequence_policies(bindings,sequencePolicies,error))return false;
        for(const auto& pair:config.profiles){const auto& policy=pair.second;
            if(!policy.sourceAttackBank&&!policy.sourceAttackPolicies.empty())
                throw std::runtime_error("Source attack policies require their explicit bank");
            if(policy.sourceAttackBank&&policy.sourceAttackPolicies.empty())
                throw std::runtime_error("Source attack bank requires original Type/Loop policies");
            for(const auto& row:policy.sourceAttackPolicies){const auto& value=row.second;
                if(row.first<0||value.id!=row.first||value.type<0||value.type>2||value.loop< -1||value.loop>INT32_MAX)
                    throw std::runtime_error("Invalid source attack sequence policy");
                const auto inserted=sequencePolicies.emplace(row.first,value);
                if(!inserted.second&&(inserted.first->second.type!=value.type||inserted.first->second.loop!=value.loop||
                   (!inserted.first->second.name.empty()&&!value.name.empty()&&inserted.first->second.name!=value.name)))
                    throw std::runtime_error("Source attack sequence policy conflicts with original metadata");
            }
        }
        next->sequencePolicies=sequencePolicies;
        dh2::data::ItemTable items;
        // Definitions are not instances: two identical equipped daggers/rings
        // must contribute twice. Main/off selectors claim distinct occurrences.
        auto equipment=config.equippedItemIds;
        if(equipment.size()>character_collection_limit)throw std::runtime_error("Equipped occurrence count exceeds limit");
        std::optional<std::size_t> mainIndex,offIndex;
        auto hand=[&](const std::string& name,std::optional<std::size_t> excluded)->std::optional<std::size_t>{
            if(name.empty())return std::nullopt;
            for(std::size_t i=0;i<equipment.size();++i)if(equipment[i]==name&&(!excluded||i!=*excluded))return i;
            if(equipment.size()==character_collection_limit)throw std::runtime_error("Equipped hand occurrence exceeds limit");
            equipment.push_back(name);return equipment.size()-1;
        };
        mainIndex=hand(config.mainItemId,std::nullopt);
        offIndex=hand(config.offItemId,mainIndex);
        std::vector<OriginalEquippedItem> equipped;
        std::optional<dh2::data::ItemRecord164> main,off;
        if(!equipment.empty()){
            const auto read=[&](const char* file){return assets.read(std::filesystem::path(config.tableRoot)/file);};
            const auto data=read("loot_table_pyarray.bin"),names=read("loot_table_pyarraynames.bin"),fields=read("loot_table_pystructnames.bin");
            if(!dh2::data::load_items({data.data(),data.size()},{names.data(),names.size()},{fields.data(),fields.size()},items,error))return false;
            for(std::size_t occurrence=0;occurrence<equipment.size();++occurrence){const auto& id=equipment[occurrence];const auto index=dh2::data::item_id(items,id);const auto* item=dh2::data::item(items,index);
                if(!item)throw std::runtime_error("Equipped ID must be exact original ItemTable name: "+id);
                equipped.push_back({item->record,offIndex&&occurrence==*offIndex,{}});
                if(mainIndex&&occurrence==*mainIndex)main=item->record;if(offIndex&&occurrence==*offIndex)off=item->record;
            }
            next->logs.push_back("Equipment base records bound; generated instance powers remain a host gap");
        }
        struct Staged {ActorId id;CharacterVisual visual;CharacterVisual* destination;CombatPoseBindings poses;Impl::Entry entry;
                       std::optional<OriginalCombatVisualPlan> sequencePlan;std::optional<OriginalAttackSelection> sequenceSelection;};
        std::vector<Staged> staged;staged.reserve(population.actors().size()+1);
        const auto prepare=[&](ActorId id,const std::string& profileId,const CombatSessionProfile& policy,
                               CharacterVisual& destination,const ActorCustomization& customization,
                               Vec3 position,float heading,bool isPlayer,const ActorDefinition* definition){
            const auto* source=bindings.find_actor(profileId);
            if(!source||source->propertyRow<0||static_cast<std::size_t>(source->propertyRow)>=database.characters.names.size())
                throw std::runtime_error("Source actor property row unavailable: "+profileId);
            OriginalCombatVisualPlan plan;
            if(!build_original_combat_visual_plan(assets,bindings,profileId,customization,"actor-"+std::to_string(id),plan,error))throw std::runtime_error(error);
            if(policy.sourceAttackBank){const auto& bank=*policy.sourceAttackBank;
                if(!policy.sequenceAction||policy.animationOnly||bank.profileId!=profileId||
                   bank.config.model_path!=plan.config.model_path||bank.sequences.empty()||
                   !bank.sequence(policy.sequenceAction->state,policy.sequenceAction->variant))
                    throw std::runtime_error("Source attack bank requires matching profile/model and explicit outgoing root");
                std::set<std::pair<std::string,std::size_t>> bankKeys;
                for(const auto& sequence:bank.sequences){
                    if((sequence.state!="Attack"&&sequence.state!="AttackStatic")||
                       !bankKeys.emplace(sequence.state,sequence.variant).second||sequence.phases.empty())
                        throw std::runtime_error("Source attack bank may replace only explicit Attack/AttackStatic roots");
                    const auto sourcePolicy=policy.sourceAttackPolicies.find(sequence.id);
                    if(sourcePolicy==policy.sourceAttackPolicies.end()||sourcePolicy->second.type!=sequence.type||sourcePolicy->second.loop!=sequence.loop)
                        throw std::runtime_error("Source attack bank root disagrees with its original Type/Loop policy");
                    plan.sequences.erase(std::remove_if(plan.sequences.begin(),plan.sequences.end(),[&](const auto& existing){
                        return existing.state==sequence.state&&existing.variant==sequence.variant;
                    }),plan.sequences.end());
                    plan.sequences.push_back(sequence);
                    if(std::find(plan.stateNames.begin(),plan.stateNames.end(),sequence.state)==plan.stateNames.end())plan.stateNames.push_back(sequence.state);
                }
                for(const auto& clip:bank.config.clips){
                    if(clip.first.empty()||clip.second.empty())throw std::runtime_error("Source attack bank has an empty clip alias/path");
                    const auto existing=std::find_if(plan.config.clips.begin(),plan.config.clips.end(),[&](const auto& item){return item.first==clip.first;});
                    if(existing!=plan.config.clips.end()){
                        if(existing->second!=clip.second)throw std::runtime_error("Source attack bank conflicts with actor clip alias");
                    }else plan.config.clips.push_back(clip);
                }
                for(const auto& rate:bank.clipRates)plan.clipRates.insert_or_assign(rate.first,rate.second);
            }
            const auto* action=(policy.sequenceAction||policy.animationOnly)?nullptr:&choose(plan,policy.action);const auto& idle=choose(plan,policy.initialIdle);
            if(policy.animationOnly&&(policy.sequenceAction||policy.sourceCombo||
               !policy.damageMarkerNames.empty()||policy.sourceMeleeHandMarkers||policy.diagnosticAIEnabled||
               (!policy.receiveDamage&&(policy.reaction||policy.death||policy.propertyOptions.refill_vitals))))
                throw std::runtime_error("No-outgoing profile cannot declare attack policy; presentation-only actors cannot declare hit/vital policy");
            if(policy.receiveDamage&&(!policy.animationOnly||!policy.reaction||!policy.death))
                throw std::runtime_error("Incoming-only capability requires no-outgoing profile and explicit source Injury/Died choices");
            if(policy.reactionMinimalRandoms&&!policy.reaction)
                throw std::runtime_error("Source reaction choice requires its actual Injury root");
            if(policy.retainedPhaseClock&&!policy.sequenceAction&&!policy.animationOnly)throw std::runtime_error("Retained phase clock requires an explicit source sequence");
            if(policy.sourceCombo&&(!policy.retainedPhaseClock||!policy.sequenceAction||!policy.sequenceAction->group_path.empty()))
                throw std::runtime_error("Source combo requires retained full-root sequence selection");
            if(policy.sourceAttackStateSelection&&(!isPlayer||!policy.sourceAttackBank||!policy.sequenceAction||
               !policy.retainedPhaseClock||!policy.sequenceAction->group_path.empty()||!policy.sequenceAction->choices.empty()||
               policy.sequenceAction->variant!=0||
               !policy.sourceAttackBank->sequence("Attack",0)||!policy.sourceAttackBank->sequence("AttackStatic",0)))
                throw std::runtime_error("Source attack focus selection requires both explicit player full roots without unresolved choices");
            CharacterVisualConfig visualConfig=plan.config;
            const auto appendSourceClips=[&](const std::vector<std::pair<std::string,std::string>>& clips){
                for(const auto& extra:clips){
                    if(extra.first.empty()||extra.second.empty())throw std::runtime_error("Source animation bank requires explicit alias and resource path");
                    const auto existing=std::find_if(visualConfig.clips.begin(),visualConfig.clips.end(),[&](const auto& clip){return clip.first==extra.first;});
                    if(existing!=visualConfig.clips.end()){
                        if(existing->second!=extra.second)throw std::runtime_error("Source animation bank alias conflicts with existing actor clip: "+extra.first);
                    }else visualConfig.clips.push_back(extra);
                }
            };
            if(!isPlayer&&(!policy.motionRoot.empty()||policy.animationOnly)) {
                // Animation-only actors use the same recovered GetAnimRoot
                // selection as other source actors, without a dummy attack.
                visualConfig.motion_node_id=policy.motionRoot.empty()?"auto":policy.motionRoot;
                visualConfig.consume_root_motion=true;
            }
            if(isPlayer){
                const auto& host=config.playerVisualConfig;
                if(host.model_path.empty())throw std::runtime_error("Player visual config must preserve the host locomotion bank/model");
                visualConfig=host;
                if(visualConfig.clips.empty())for(unsigned i=0;i<3;++i)if(!host.animation_paths[i].empty())
                    visualConfig.clips.emplace_back(i==0?"idle":i==1?"walk":"attack",host.animation_paths[i]);
                appendSourceClips(plan.config.clips);
                visualConfig.skin_id_contains=customization.skin_id_contains;
                visualConfig.controller_ids=customization.controller_ids;
                visualConfig.use_authored_modular_defaults=customization.use_authored_modular_defaults;
                visualConfig.expected_controller_count=customization.expected_controller_count;
                visualConfig.include_static_instances=customization.include_static_instances;
                visualConfig.allow_missing_animation_targets=customization.allow_missing_animation_targets;
            }
            appendSourceClips(policy.sourceAnimationClips);
            staged.emplace_back();auto& stage=staged.back();stage.id=id;stage.destination=&destination;
            stage.poses.source_injury_gate_enabled=true;
            stage.entry.retainedPhaseClock=policy.retainedPhaseClock||policy.animationOnly;
            stage.entry.animationOnly=policy.animationOnly;
            stage.entry.receiveDamage=policy.receiveDamage;
            stage.entry.reactionMinimalRandoms=policy.reactionMinimalRandoms;
            stage.entry.sourceCombo=policy.sourceCombo;stage.entry.sourceAttack.owner=id;stage.entry.sourceAttack.object_of_interest_type=-1;
            if(policy.sourceCombo){stage.entry.targetProjection.lastTarget=invalid_actor_id;stage.entry.targetProjection.lastTargetKnown=true;}
            stage.entry.sourceAttackStateSelection=policy.sourceAttackStateSelection;
            if(!stage.visual.load(assets,visualConfig,error)||!stage.visual.select(idle.clipName,true,error))throw std::runtime_error(error);
            const std::string actionAlias="actor-"+std::to_string(id)+"/source-sequence";
            CombatVisualBinding actionVisual=combat_visual_binding(stage.visual);
            if(policy.sequenceAction){
                stage.sequencePlan=plan;stage.sequenceSelection=*policy.sequenceAction;
                stage.entry.sequenceCallerSelection=*policy.sequenceAction;
                stage.entry.sequence=std::make_unique<OriginalAttackSequence>();
                stage.poses.clip_rates[actionAlias]=1;
            }else if(action)stage.poses.clip_rates[action->clipName]=action->speed;
            if(policy.animationOnly)stage.sequencePlan=plan;
            for(const auto* pair:{&policy.reaction,&policy.death})if(*pair&&(!policy.animationOnly||policy.receiveDamage)){
                const auto& phase=choose(plan,**pair);stage.poses.clip_rates[phase.clipName]=phase.speed;
                // Publish only the selected leaf's own AnimTable container.
                // This diagnostic leaf selection does not execute ancestor
                // groups and must not invent their Sound/Swoosh/FX entries.
                const auto* sequence=plan.sequence((**pair).state,(**pair).variant);
                if(!sequence||phase.sourcePath.empty()||
                   phase.sourcePath.size()>UINT32_MAX||phase.sourcePath.back()>UINT32_MAX||
                   phase.ancestors.size()+1<phase.sourcePath.size())
                    throw std::runtime_error("Reaction/death source step metadata is unavailable");
                CombatSessionStepEntry step;
                step.depth=static_cast<std::uint32_t>(phase.sourcePath.size()-1);
                step.step=static_cast<std::uint32_t>(phase.sourcePath.back());
                step.sequence_id=step.depth?phase.ancestors[step.depth-1].animationId:sequence->id;
                step.leaf_path=phase.sourcePath;
                step.container_path.assign(phase.sourcePath.begin(),phase.sourcePath.end()-1);
                step.role=pair==&policy.reaction?CombatSessionStepRole::hurt:CombatSessionStepRole::death;
                stage.entry.poseStepEntries.emplace(phase.clipName,std::move(step));
                if(pair==&policy.reaction){stage.poses.react_clip_id=phase.clipName;stage.poses.react_move_go=phase.moveGO!=0;}
                else{stage.poses.death_clip_id=phase.clipName;stage.poses.death_move_go=phase.moveGO!=0;}
                if(pair==&policy.reaction&&policy.reactionMinimalRandoms){
                    if(sequence->type!=2||sequence->loop!=0||sequence->phases.empty()||sequence->phases.size()>INT32_MAX)
                        throw std::runtime_error("Source reaction chooser requires finite Type2 Injured root");
                    for(std::size_t i=0;i<sequence->phases.size();++i){const auto& choice=sequence->phases[i];
                        if(choice.sourcePath!=std::vector<std::size_t>{i}||!choice.has_visual()||!std::isfinite(choice.speed)||choice.speed<=0)
                            throw std::runtime_error("Source reaction chooser requires authored flat Type2 leaf order");
                        stage.entry.reactionChoices.push_back(choice);stage.poses.clip_rates[choice.clipName]=choice.speed;
                        auto choiceStep=step;choiceStep.step=static_cast<std::uint32_t>(i);choiceStep.leaf_path={i};
                        stage.entry.poseStepEntries.insert_or_assign(choice.clipName,std::move(choiceStep));
                    }
                }
            }
            OriginalCombatFacts facts;facts.original_state=policy.originalCombatState;
            // Presentation-only Actors retain authored vital sentinels. Their
            // explicit prepared Idle is nevertheless a real initial state;
            // publish it once without inferring Dead from projected zero HP.
            if(policy.animationOnly&&!policy.receiveDamage&&facts.original_state==-1&&policy.initialIdle.state=="Idle")
                facts.original_state=3;
            if(policy.animationOnly&&!policy.receiveDamage&&facts.original_state!=-1)
                stage.entry.presentationInitialState=facts.original_state;
            if(isPlayer&&!original_combat_equipment_facts(main?&*main:nullptr,off?&*off:nullptr,facts,error))throw std::runtime_error(error);
            OriginalCombatProperties props;
            if(!build_original_combat_properties(database,database.characters.names[source->propertyRow],policy.propertyOptions,
                 isPlayer?equipped:std::vector<OriginalEquippedItem>{},facts,props,error))throw std::runtime_error(error);
            if(isPlayer&&config.selectedPlayerProfile){
                OriginalCombatProperties projected;
                if(!project_player_profile_properties(database,*config.selectedPlayerProfile,props,projected,error))
                    throw std::runtime_error("Selected profile before player binding: "+error);
                props=std::move(projected);
            }
            if(stage.entry.sequence){
                const float originalRate=original_speed_modifier(props.sheets.resolved[48]);
                volatile float effectiveRate=static_cast<float>(policy.sequenceAction->actor_rate)*originalRate;
                stage.sequenceSelection->actor_rate=effectiveRate;
                const OriginalAttackSequenceServices services{actionVisual,[visual=&stage.visual](const std::string& name,std::string& e){return visual->restart(name,false,e);}};
                if(!stage.entry.sequence->prepare(plan,sequencePolicies,*stage.sequenceSelection,services,actionAlias,error))throw std::runtime_error(error);
                actionVisual=stage.entry.sequence->binding();
                next->logs.push_back("Original PROPS_AttackSpeed48 actor="+std::to_string(id)+" multiplier="+std::to_string(originalRate)+" caller_factor="+std::to_string(policy.sequenceAction->actor_rate));
            }
            ActorState actor;actor.id=id;actor.definition_id=definition?definition->sourceId:profileId;
            if(isPlayer&&config.selectedPlayerProfile)actor.persistent_character_id=config.selectedPlayerProfile->id;
            const auto classId=props.sheets.resolved[26];if(classId>=0&&std::size_t(classId)<database.classes.names.size())actor.class_id=database.classes.names[classId];
            if(isPlayer){
                std::map<std::string,unsigned> slotOccurrences,definitionOccurrences;
                for(std::size_t occurrence=0;occurrence<equipment.size();++occurrence){const auto& name=equipment[occurrence];const auto* item=dh2::data::item(items,dh2::data::item_id(items,name));
                    std::string slot=mainIndex&&occurrence==*mainIndex?"main_hand":offIndex&&occurrence==*offIndex?"off_hand":"source-type/"+std::to_string(item->record.words[22]);
                    const auto repeatedSlot=slotOccurrences[slot]++;if(repeatedSlot)slot+="/"+std::to_string(repeatedSlot);
                    std::string instance="equipped/"+name;const auto repeatedDefinition=definitionOccurrences[name]++;if(repeatedDefinition)instance+="/"+std::to_string(repeatedDefinition);
                    actor.equipment.push_back({std::move(slot),name,std::move(instance)});
                }
            }
            actor.faction_id=props.sheets.resolved[0];actor.transform.position={position.x,position.y,position.z};actor.transform.rotation[2]=heading;
            actor.health=original_signed256(props.sheets.resolved[36]);actor.max_health=original_signed256(props.sheets.resolved[38]);
            actor.resource=original_signed256(props.sheets.resolved[41]);actor.max_resource=original_signed256(props.sheets.resolved[43]);
            if(policy.animationOnly){
                // ActorState is a nonnegative gameplay projection; source -1
                // vital sentinels remain untouched in the original property
                // sheet and project to zero without refilling/adding HP.
                actor.health=std::max(0.0f,actor.health);actor.max_health=std::max(0.0f,actor.max_health);
                actor.resource=std::max(0.0f,actor.resource);actor.max_resource=std::max(0.0f,actor.max_resource);
            }
            actor.action=actor.alive()?CharacterAction::idle:CharacterAction::dead;
            stage.entry.traits={isPlayer,!policy.animationOnly||policy.receiveDamage,isPlayer?main:std::nullopt};
            stage.entry.baseTargetable=stage.entry.traits.targetable;
            if(policy.animationOnly){
                if(!next->world->bind_actor(actor,props,stage.entry.traits,error))throw std::runtime_error(error);
            }else{
                auto& attack=stage.entry.attack;attack.id="actor-"+std::to_string(id)+(policy.sequenceAction?"/source-group":"/diagnostic-leaf");attack.animation_clip_id=policy.sequenceAction?actionAlias:action->clipName;
                const auto sourceId="actor-"+std::to_string(id)+"/melee";
                std::vector<std::pair<std::string,OriginalMeleeSource>> handSources;
                if(policy.damageMarkerNames.empty())throw std::runtime_error("Source damage marker choice required: "+profileId);
                const auto* markers=actionVisual.markers(attack.animation_clip_id,error);if(!markers)throw std::runtime_error(error);
                for(const auto& marker:policy.damageMarkerNames){
                    if(std::none_of(markers->markers().begin(),markers->markers().end(),[&](const AnimationMarker& m){return m.name==marker;}))
                        throw std::runtime_error("Selected source phase has no damage marker: "+profileId+"/"+marker);
                    std::string markerSource=sourceId;
                    if(policy.sourceMeleeHandMarkers){
                        const auto kind=classify_original_named_animation_event(marker,5,false);
                        if(kind!=OriginalNamedAnimationKind::mainhand&&kind!=OriginalNamedAnimationKind::offhand)
                            throw std::runtime_error("Source melee hand binding requires original main/offhand event");
                        const bool left=kind==OriginalNamedAnimationKind::offhand;
                        markerSource+=left?"/offhand":"/mainhand";
                        handSources.push_back({markerSource,{left,policy.alternate}});
                    }
                    attack.damage_markers.push_back({marker,markerSource});
                }
                actor.attack_ids.push_back(attack.id);
                if(!next->world->bind_actor(actor,props,stage.entry.traits,error))throw std::runtime_error(error);
                if(policy.sourceMeleeHandMarkers){
                    for(const auto& hand:handSources)if(!next->world->bind_source(hand.first,hand.second,error))throw std::runtime_error(error);
                }else if(!next->world->bind_source(sourceId,{policy.offHand,policy.alternate},error))throw std::runtime_error(error);
                attack.maximum_range=next->world->melee_reach(id);
                const auto* ai=dh2::data::ai_props(next->world->factions(),props.sheets.resolved[1]);
                if(!ai||ai->attack_delay<0)throw std::runtime_error("Original AI AttackDelay unavailable/negative");
                attack.cooldown_seconds=ai->attack_delay*0.001;attack.cooldown_timing=CooldownTiming::attack_departure;
                if(!validate_attack_definition(attack,error))throw std::runtime_error(error);
            }
            stage.entry.idle=idle.clipName;stage.entry.idlePath=idle.resolvedPath;stage.entry.idleRate=idle.speed;
            if(!isPlayer&&policy.diagnosticAIEnabled&&!policy.animationOnly){
                const auto gate=definition->properties.find("ai_state");
                stage.entry.diagnosticAI=!policy.requiredAIState.empty()&&gate!=definition->properties.end()&&gate->second==policy.requiredAIState;
                next->logs.push_back("Diagnostic stationary AI "+std::string(stage.entry.diagnosticAI?"enabled":"gated")+" actor="+std::to_string(id)+" authored ai_state="+(gate==definition->properties.end()?"<absent>":gate->second));
            }
        };
        prepare(config.playerId,config.playerProfileId,playerProfile->second,playerVisual,playerCustomization,playerPosition,0,true,nullptr);
        for(auto& populationActor:population.actors()){
            const auto policy=config.profiles.find(populationActor.profileId);if(policy==config.profiles.end())continue;
            const auto& matrix=populationActor.definition.placement;
            prepare(populationActor.definition.stableId,populationActor.profileId,policy->second,populationActor.visual,
                    policy->second.customization,{matrix[12],matrix[13],matrix[14]},std::atan2(matrix[1],matrix[0]),false,&populationActor.definition);
        }
        // Validate runtime bindings against staged visuals before publishing.
        for(auto& stage:staged)if(!next->runtime->bind(*next->world->find_actor(stage.id),
             stage.entry.sequence?stage.entry.sequence->binding():combat_visual_binding(stage.visual),stage.poses,error))throw std::runtime_error(error);
        next->runtime->clear();
        for(auto& stage:staged){*stage.destination=std::move(stage.visual);stage.entry.visual=stage.destination;
            stage.entry.poses=stage.poses;
            if(stage.entry.sequence){
                const auto services=next->sequence_services(stage.destination,stage.id);
                if(!stage.entry.sequence->prepare(*stage.sequencePlan,sequencePolicies,*stage.sequenceSelection,services,stage.entry.attack.animation_clip_id,error))throw std::runtime_error(error);
                stage.entry.sequencePlan=std::move(stage.sequencePlan);
            }
            else if(stage.entry.animationOnly)stage.entry.sequencePlan=std::move(stage.sequencePlan);
            auto& entry=next->entries.emplace(stage.id,std::move(stage.entry)).first->second;
            if(entry.animationOnly){if(!next->prepare_animation_only(entry,stage.id,error))throw std::runtime_error(error);}
            else if(entry.retainedPhaseClock&&!next->prepare_retained(entry,stage.id,*stage.sequenceSelection,error))throw std::runtime_error(error);
            const auto visualBinding=next->pose_motion_binding(
                entry.retainedPhaseClock?next->retained_binding(entry):entry.sequence?entry.sequence->binding():combat_visual_binding(*stage.destination),
                stage.destination,stage.id);
            if(!next->runtime->bind(*next->world->find_actor(stage.id),visualBinding,next->pose_admission(stage.id,stage.poses),error))throw std::runtime_error(error);
        }
        if(std::any_of(next->entries.begin(),next->entries.end(),[](const auto& p){return bool(p.second.sequence);}))
            next->logs.push_back("Explicit original source phase groups execute pre/strike/recovery; outer combo chaining remains a caller gap");
        for(const auto& entry:next->entries)if(entry.second.retainedPhaseClock)
            next->logs.push_back("Retained source animation clock actor="+std::to_string(entry.first)+" uses original two-slot blend, events, dynamic root/default library, completion and replay cadence; explicit bound player locomotion shares this owner; native callback reentry remains a gap");
        for(const auto& entry:next->entries)if(entry.second.animationOnly&&!entry.second.receiveDamage)
            next->logs.push_back("Animation-only source actor="+std::to_string(entry.first)+" retains exact property vitals/visual and authored Idle; it is untargetable, has no attack IDs, and visual advancement is independent of combat alive/visibility state");
        for(const auto& entry:next->entries)if(entry.second.receiveDamage)
            next->logs.push_back("Incoming-only source actor="+std::to_string(entry.first)+" is a targetable shared damage receiver with original Injury/Died poses; outgoing attack IDs/commands remain disabled");
        for(const auto& entry:next->entries)if(entry.second.sourceCombo)
            next->logs.push_back("Source combo actor="+std::to_string(entry.first)+" uses same-owner continued/last/index and actual hierarchy hooks; unbound flags528/heading/OOI and target-search policies are diagnostic; frontal-heading/ranged/network backends reject when reached; preattack/look/sticky producers remain explicit gaps");
        if(std::any_of(next->entries.begin(),next->entries.end(),[](const auto& p){return bool(p.second.sequence)&&!p.second.retainedPhaseClock;}))
            next->logs.push_back("Diagnostic virtual timeline configured for some actors; source frame cadence and retained blending require explicit retained phase clock");
        if(std::any_of(next->entries.begin(),next->entries.end(),[](const auto& p){return !p.second.sequence&&!p.second.animationOnly;}))
            next->logs.push_back("Diagnostic single source leaf compatibility configured for some actors; full groups are opt-in");
        next->logs.push_back("Full original DOT/status/leech/FX/quest outcomes retained for host; only direct health application is active");
        next->logs.push_back("Target turning uses original heading/rotation kernels and resolved rotation modifier; attack-specific rotation lock/snap flags remain a host gap");
        for(const auto& entry:next->entries)if(!next->facts(entry.first,error))throw std::runtime_error(error);
        impl_=std::move(next);return true;
    }catch(const std::exception& ex){error=ex.what();return false;}
}
bool CombatSession::update(double dt,const InputActions& input,Vec3 position,float facing,std::string& error,
                           const RetainedFrameAudioClock* audio_clock){
    error.clear();if(!impl_||impl_->detached){error="Combat session is not initialized or is detached for restore";return false;}
    if(!std::isfinite(dt)||dt<0||dt*1000>double(std::numeric_limits<std::uint32_t>::max())-1||!std::isfinite(position.x)||!std::isfinite(position.y)||!std::isfinite(position.z)||!std::isfinite(facing)){
        error="Combat session frame inputs are invalid";return false;
    }
    auto& s=*impl_;
    if(s.updating||s.restoreTeardown||s.checkingAnimationCheckpoint||s.transitionDelivering||(s.motionPhaseRequired&&!s.motionPhaseHandler)){
        error="Actor frame cannot reenter or run without its required fresh motion phase";return false;
    }
    if(s.animationNotificationsNeedRebind){error="Required fresh reconstructible animation notification binding after restore";return false;}
    if(s.transitionRequired&&!s.actorTransitionHandler){error="Required fresh actor transition binding after restore";return false;}
    if(!s.transitionFailure.empty()||!s.pendingTransitions.empty()||!s.runtime->validate_transition_checkpoint(error)){
        if(error.empty())error=s.transitionFailure.empty()?"Actor transition has unfinished reached prefix":s.transitionFailure;return false;
    }
    if(s.updateSerial==UINT64_MAX){error="Combat frame serial exhausted";return false;}
    struct ActorFrameScope {
        Impl& session;
        ~ActorFrameScope(){session.pendingMotion.clear();session.updating=false;}
    } frameScope{s};s.updating=true;++s.updateSerial;
    s.events.clear();s.resolutions.clear();s.animationDispatches.clear();s.comboBoundaries.clear();s.retainedFrameAudioDiagnostics.clear();
    s.retainedFrameAudioClock.reset();if(audio_clock&&audio_clock->valid())s.retainedFrameAudioClock=*audio_clock;
    struct ClockReset {std::optional<RetainedFrameAudioClock>& value;~ClockReset(){value.reset();}} clockReset{s.retainedFrameAudioClock};
    if(s.frameBeginProvider&&!s.frameBeginProvider(*this,dt,error))return false;
    auto* player=s.world->find_actor(s.player);
    // A clear published between session frames belongs to its source owner
    // (for example an explicit skill ClearTarget). Do not resurrect it from
    // this convenience record. Finish/combo clears happen synchronously below
    // and are repaired before the frame is published to presentation.
    if(player->target_id==invalid_actor_id&&!input.targetSelect)
        s.stickyPlayerTarget=invalid_actor_id;
    else if(player->target_id!=invalid_actor_id&&s.stickyPlayerTarget!=invalid_actor_id&&
            player->target_id!=s.stickyPlayerTarget)
        s.stickyPlayerTarget=invalid_actor_id;
    if(!refresh_actor_combat_permissions(error))return false;
    player->transform.position={position.x,position.y,position.z};player->transform.rotation[2]=facing;
    if(!s.runtime->owns_pose(s.player)&&!s.entries.at(s.player).stateSequence&&player->alive()){
        const auto action=(input.move2D.x!=0||input.move2D.y!=0)?CharacterAction::moving:CharacterAction::idle;
        // A paused host supplies neutral input with zero elapsed time. Keep the
        // already admitted physical locomotion state/pose; neutral is not Idle
        // admission while the gameplay clock is frozen.
        if(!s.actorTransitionHandler||dt>0){
            if(s.actorTransitionHandler&&player->action!=action){error="Physical locomotion must be admitted through the current authored locomotion selection";return false;}
            player->action=action;
        }
    }
    if(input.targetSelect){ActorId selected{};if(!s.cycle_player_target(selected,error))return false;}
    // A fresh attack command owns its target resolution only AFTER admission.
    // Ordinary selection housekeeping remains independent when not attacking.
    if(!input.attack)if(const auto* target=s.world->find_actor(player->target_id);!target||!s.world->eligible_target(*player,*target)){
        if(!s.set_source_target(s.player,invalid_actor_id,true,error))return false;
    }
    if(input.attack){if(!s.command_request(s.player,player->target_id,dt,error))return false;}
    // Source Space release raises event 50001, and Character::CSM_StoppedAttacking
    // leaves Attack only when Character+1090 (CharAI+122) is set. That byte is
    // (step!=0 && final step) in _OnAnimStepBegin_Attack, i.e. AttackState64::finisher
    // here; AttackState64::last (CharAI+121) also covers the pre step and gates input.
    if(!input.attack&&player->action==CharacterAction::attacking){
        const auto& playerEntry=s.entries.at(s.player);
        if(playerEntry.sourceCombo&&playerEntry.sourceAction&&playerEntry.sourceAttack.finisher&&
           !s.depart_source_attack(s.player,error))return false;
    }
    if(player->action==CharacterAction::attacking&&!s.turn(s.player,player->target_id,dt,error))return false;
    if(s.actorDecisionProvider&&!s.actorDecisionProvider(*this,dt,error))return false;
    for(const auto& entry:s.entries){
        if(entry.first==s.player||!entry.second.diagnosticAI)continue;
        if(s.actorDecisionProvider)continue;
        auto* a=s.world->find_actor(entry.first);
        if(!s.command_request(entry.first,a->target_id,dt,error))return false;
    }
    for(const auto& entry:s.entries)if(!s.facts(entry.first,error))return false;
    std::set<ActorId> ownedBeforeUpdate;
    for(const auto& entry:s.entries)if(s.runtime->owns_pose(entry.first))ownedBeforeUpdate.insert(entry.first);
    const bool playerActionWasActive=player->action==CharacterAction::attacking||ownedBeforeUpdate.count(s.player)!=0;
    if(!s.runtime->update(dt,s.events,error))return false;
    if(playerActionWasActive&&!s.runtime->owns_pose(s.player)&&player->action!=CharacterAction::attacking&&
       s.stickyPlayerTarget!=invalid_actor_id){
        const auto* target=s.world->find_actor(s.stickyPlayerTarget);
        if(target&&s.world->eligible_target(*player,*target)){if(!s.set_source_target(s.player,s.stickyPlayerTarget,false,error))return false;}
        else s.stickyPlayerTarget=invalid_actor_id;
    }
    for(auto& entry:s.entries){
        if(s.runtime->owns_pose(entry.first))continue;
        const auto* a=s.world->find_actor(entry.first);if(!a->alive()&&!entry.second.animationOnly)continue;
        if(entry.second.stateManaged){
            if(entry.second.stateSequence){if(!entry.second.retained->advance(ownedBeforeUpdate.count(entry.first)?0:dt,error))return false;}
            else if(entry.second.retained->seeded()){if(!entry.second.retained->advance_seeded(ownedBeforeUpdate.count(entry.first)?0:dt,error))return false;}
            continue;
        }
        if(entry.second.retainedPhaseClock) {
            auto binding=s.retained_binding(entry.second);
            if(!entry.second.locomotionSelected.empty()) {
                // Host selection already seeded this exact retained resource.
                // Combat may have owned/released the interval; avoid reusing it.
                if(!entry.second.retained->advance_seeded(ownedBeforeUpdate.count(entry.first)?0:dt,error))return false;
                continue;
            }
            if(entry.second.sourceSelectedClip!=entry.second.idle&&!binding.select(entry.second.idle,true,error))return false;
            if(!entry.second.retained->advance_seeded(ownedBeforeUpdate.count(entry.first)?0:dt,error))return false;
            continue;
        }
        if(entry.first==s.player)continue; // legacy host retains its own locomotion
        if(entry.second.visual->animation_name()!=entry.second.idlePath&&
           !entry.second.visual->select(entry.second.idle,true,error))return false;
        // A released action already consumed this interval's visual clock.
        // Start idle at its first frame and advance it on the next update.
        if(!entry.second.visual->update(ownedBeforeUpdate.count(entry.first)?0:dt*entry.second.idleRate,error))return false;
    }
    if(!s.deliver_motion_phase(dt,error))return false;
    // Snapshot registration order; an object added by a callback starts its
    // frame advancement on the next update. Pin entries across consumer removal.
    const auto objects=s.objectOrder;
    for(const auto id:objects){
        const auto found=s.objectEntries.find(id);if(found==s.objectEntries.end())continue;
        const auto entry=found->second;const auto* object=s.world->find_object(id);
        if(!object)continue;
        if(object->visual.model!=entry->model){error="Neutral object model changed without visual rebind";return false;}
        if(!entry->selected||!(entry->flags&0x200u)||((entry->flags&0x400u)&&!object->visual.visible))continue;
        RetainedAnimationFrame frame;if(!entry->animation->advance(dt,frame,error)||!s.dispatch_object(id,entry,frame,error))return false;
    }
    s.events.insert(s.events.end(),s.pendingSourceHits.begin(),s.pendingSourceHits.end());
    s.pendingSourceHits.clear();
    s.resolutions=s.world->take_resolutions();return true;
}
void CombatSession::set_retained_frame_audio_observer(RetainedFrameAudioObserver observer){
    if(impl_)impl_->retainedFrameAudioObserver=std::move(observer);
}
void CombatSession::set_step_entry_observer(CombatSessionStepObserver observer){if(impl_)impl_->stepObserver=std::move(observer);}
void CombatSession::clear_step_entry_observer(){if(impl_)impl_->stepObserver={};}
const std::vector<std::string>& CombatSession::step_entry_diagnostics()const noexcept{
    static const std::vector<std::string> empty;
    return impl_?impl_->stepDiagnostics:empty;
}
void CombatSession::set_actor_decision_provider(ActorDecisionProvider provider){if(impl_)impl_->actorDecisionProvider=std::move(provider);}
void CombatSession::set_frame_begin_provider(ActorDecisionProvider provider){if(impl_)impl_->frameBeginProvider=std::move(provider);}
void CombatSession::clear_frame_begin_provider(){if(impl_)impl_->frameBeginProvider={};}
std::uint64_t CombatSession::update_serial()const noexcept{return impl_&&!impl_->detached?impl_->updateSerial:0;}
void CombatSession::clear_actor_decision_provider(){if(impl_)impl_->actorDecisionProvider={};}
bool CombatSession::select_next_player_target(ActorId& selected,std::string& error){
    error.clear();
    if(!impl_||impl_->detached||!impl_->world->find_actor(impl_->player)){
        error="Target command requires a current bound player";return false;
    }
    auto& s=*impl_;
    if(s.transitionDelivering||s.motionPhaseDelivering||s.restoreTeardown||s.checkingAnimationCheckpoint||
       !s.transitionFailure.empty()||!s.pendingTransitions.empty()){
        error="Target command cannot replace intent during transition/motion/checkpoint/teardown";return false;
    }
    if(!refresh_actor_combat_permissions(error))return false;
    return s.cycle_player_target(selected,error);
}
bool CombatSession::request_actor_attack(ActorId actor,ActorId target,double dt,std::string& error){
    error.clear();
    if(!impl_||impl_->detached||!impl_->entries.count(actor)){
        error="Attack command requires a bound live session actor";return false;
    }
    if(!std::isfinite(dt)||dt<0){error="Attack command interval must be finite and nonnegative";return false;}
    if(impl_->transitionDelivering||impl_->checkingAnimationCheckpoint||impl_->restoreTeardown){error="Attack command cannot reenter transition/checkpoint/teardown";return false;}
    return impl_->command_request(actor,target,dt,error);
}
bool CombatSession::bind_source_hit_effect_handler(SourceHitEffectHandler handler,std::string& error){
    error.clear();
    if(!impl_||impl_->detached||!handler||impl_->updating||impl_->applyingSourceHit||impl_->deliveringSourceHitEffect||
       impl_->transitionDelivering||impl_->checkingAnimationCheckpoint||impl_->restoreTeardown){
        error="Source-hit effects require an idle attached Session and actual handler";return false;
    }
    auto& s=*impl_;s.sourceHitEffectHandler=std::move(handler);
    s.sourceHitEffectLease=s.bindingLease;s.sourceHitEffectRequired=true;
    s.runtime->set_hit_effect_observer([this](const DamageEvent& hit,std::uint64_t occurrence,
        CombatRuntimeHitEffectStage stage,std::string& detail){
        auto& current=*impl_;const auto lease=current.sourceHitEffectLease.lock();
        if(current.detached||!lease||!current.bindingLease||
           lease.owner_before(current.bindingLease)||current.bindingLease.owner_before(lease)||
           !current.sourceHitEffectHandler){detail="Source-hit effect handler lacks its current actor lease";return false;}
        if(current.deliveringSourceHitEffect){detail="Source-hit effect handler cannot reenter";return false;}
        struct Scope{bool& active;~Scope(){active=false;}} scope{current.deliveringSourceHitEffect};
        current.deliveringSourceHitEffect=true;
        const auto handler=current.sourceHitEffectHandler;
        return handler(*this,hit,occurrence,stage,detail);
    });
    return true;
}
bool CombatSession::apply_source_result(const CombatSessionSourceHit& hit,
    DamageEvent& receipt,std::string& error){
    receipt={};error.clear();
    if(!impl_||impl_->detached||hit.binding_lease.expired()||
       hit.binding_lease.owner_before(impl_->bindingLease)||impl_->bindingLease.owner_before(hit.binding_lease)||
       !hit.generation||hit.source_id.empty()||hit.source_id.size()>character_text_limit||
       hit.marker_name.empty()||hit.marker_name.size()>character_text_limit||
       !impl_->entries.count(hit.attacker)||!impl_->entries.count(hit.target)){
        error="Source hit requires current Session actors/lease and an authored occurrence";return false;
    }
    auto& s=*impl_;
    if(s.applyingSourceHit||s.deliveringSourceHitEffect||s.transitionDelivering||s.checkingAnimationCheckpoint||s.restoreTeardown){error="Source hit calculation/application cannot reenter";return false;}
    const bool unfinishedBlur=std::any_of(s.pendingTransitions.begin(),s.pendingTransitions.end(),
        [](const auto& pair){return pair.second.stage!=CombatSessionTransitionStage::focus_prefix;});
    if(!s.transitionFailure.empty()||unfinishedBlur||!s.runtime->validate_transition_checkpoint(error)){
        if(error.empty())error="Source hit requires a completed actor transition";return false;
    }
    const auto batchKey=std::make_pair(hit.attacker,hit.source_id);
    const auto delivery=std::make_pair(hit.target,hit.event_index);
    const auto old=s.sourceHitBatches.find(batchKey);
    if(old!=s.sourceHitBatches.end()){
        if(hit.generation<old->second.generation)return true;
        if(hit.generation==old->second.generation&&old->second.delivered.count(delivery))return true;
        if(hit.generation==old->second.generation&&old->second.delivered.size()>=character_collection_limit){
            error="Source hit occurrence exceeds delivery limit";return false;
        }
    }
    if(old==s.sourceHitBatches.end()&&s.sourceHitBatches.size()>=character_collection_limit){
        error="Source hit producer count exceeds limit";return false;
    }
    struct ApplyingReset {bool& value;~ApplyingReset(){value=false;}} reset{s.applyingSourceHit};
    s.applyingSourceHit=true;
    OriginalMeleeResolution result;
    if(!s.world->resolve_source_result(hit.source_id,hit.attacker,hit.target,hit.marker_name,
        hit.mask,hit.category,hit.element,hit.direct_amount,result,error,hit.attacker_formula_sheet))return false;
    // Consume the source occurrence before lifecycle work. If a later visual
    // callback fails, its reached damage/RNG prefix must not be applied again.
    auto& batch=s.sourceHitBatches[batchKey];
    if(batch.generation!=hit.generation){batch.generation=hit.generation;batch.delivered.clear();}
    batch.delivered.insert(delivery);
    const bool applied=s.runtime->apply_calculated_hit(hit.attacker,hit.target,hit.source_id,
        hit.marker_name,result.damage,result.original.outcomes,result.original.mask,receipt,error);
    if(receipt.applied)s.pendingSourceHits.push_back(receipt);
    return applied;
}
void CombatSession::clear_retained_frame_audio_observer(){if(impl_)impl_->retainedFrameAudioObserver={};}
const std::vector<RetainedFrameAudioObserverDiagnostic>& CombatSession::retained_frame_audio_diagnostics()const noexcept{
    static const std::vector<RetainedFrameAudioObserverDiagnostic> empty;
    return impl_?impl_->retainedFrameAudioDiagnostics:empty;
}
bool CombatSession::resolve_source_result_only(const CombatSessionSourceHit& hit,
    CombatSessionSourceCalculation& receipt,std::string& error){
    receipt={};error.clear();
    if(!impl_||impl_->detached||hit.binding_lease.expired()||
       hit.binding_lease.owner_before(impl_->bindingLease)||impl_->bindingLease.owner_before(hit.binding_lease)||
       !hit.generation||hit.source_id.empty()||hit.source_id.size()>character_text_limit||
       hit.marker_name.empty()||hit.marker_name.size()>character_text_limit||
       !impl_->entries.count(hit.attacker)||!impl_->entries.count(hit.target)){
        error="Source calculation requires current Session actors/lease and an authored occurrence";return false;
    }
    auto& s=*impl_;if(s.applyingSourceHit||s.deliveringSourceHitEffect||s.transitionDelivering||s.checkingAnimationCheckpoint||s.restoreTeardown){error="Source calculation cannot reenter";return false;}
    const auto batchKey=std::make_pair(hit.attacker,hit.source_id);
    const auto delivery=std::make_pair(hit.target,hit.event_index);
    const auto old=s.sourceHitBatches.find(batchKey);
    if(old!=s.sourceHitBatches.end()){
        if(hit.generation<old->second.generation)return true;
        if(hit.generation==old->second.generation&&old->second.delivered.count(delivery))return true;
        if(hit.generation==old->second.generation&&old->second.delivered.size()>=character_collection_limit){error="Source calculation occurrence exceeds delivery limit";return false;}
    }
    if(old==s.sourceHitBatches.end()&&s.sourceHitBatches.size()>=character_collection_limit){error="Source calculation producer count exceeds limit";return false;}
    struct Reset{bool& value;~Reset(){value=false;}} reset{s.applyingSourceHit};s.applyingSourceHit=true;
    OriginalMeleeResolution result;
    if(!s.world->calculate_source_result(hit.source_id,hit.attacker,hit.target,hit.marker_name,
        hit.mask,hit.category,hit.element,hit.direct_amount,result,error,hit.attacker_formula_sheet))return false;
    auto& batch=s.sourceHitBatches[batchKey];
    if(batch.generation!=hit.generation){batch.generation=hit.generation;batch.delivered.clear();}
    batch.delivered.insert(delivery);receipt.result=result;receipt.calculated=true;return true;
}
void CombatSession::detach_for_restore(){
    if(!impl_||impl_->detached)return;
    auto& s=*impl_;
    if(s.updating||s.deliveringSourceHitEffect||s.restoreTeardown||s.checkingAnimationCheckpoint||s.transitionDelivering)
        throw std::logic_error("Cannot detach Session during actor frame, checkpoint validation or restore teardown");
    // Validate generic timers while their current lease still exists. A
    // detached rebind must not re-query an already-invalidated feature owner.
    std::string sourceCheckpointError;
    const bool ready=s.source_checkpoint(sourceCheckpointError)&&
        (!s.animationNotificationsReconstructible||s.animation_checkpoint(sourceCheckpointError));
    // Release still-valid old bindings before world.replace_actors invalidates
    // their pointers. An empty runtime can safely survive either restore result.
    s.detach_bindings(ready);
}
bool CombatSession::detach_for_restore(const std::function<bool(std::string&)>& teardown,std::string& error){
    error.clear();
    if(!impl_||impl_->detached||impl_->updating||impl_->restoreTeardown||
       impl_->checkingAnimationCheckpoint||impl_->transitionDelivering||impl_->applyingSourceHit||!teardown){
        error="Admitted restore teardown requires idle attached Session and teardown callback";return false;
    }
    if(!validate_lifecycle_checkpoint(error))return false;
    auto& s=*impl_;const auto lease=s.bindingLease;
    struct Scope{bool& flag;~Scope(){flag=false;}} scope{s.restoreTeardown};s.restoreTeardown=true;
    try{
        if(!teardown(error)){if(error.empty())error="External restore teardown rejected after its reached prefix";return false;}
        if(s.detached||!s.bindingLease||lease.owner_before(s.bindingLease)||s.bindingLease.owner_before(lease)){
            error="External restore teardown changed the actor binding";return false;
        }
        // Admission was checked while the live external resources still existed.
        // Their intentional removal must not be revalidated as a saved-state mismatch.
        s.detach_bindings(true);error.clear();return true;
    }catch(const std::exception& ex){error=ex.what();return false;}
      catch(...){error="External restore teardown threw after its reached prefix";return false;}
}
bool CombatSession::rebind_after_restore(std::string& error){
    error.clear();
    if(!validate_lifecycle_checkpoint(error))return false;
    if(!impl_||!impl_->detached){error="Combat restore rebind requires a detached initialized session";return false;}
    auto& s=*impl_;
    if(s.world->actors().size()!=s.entries.size()){error="Restored combat actor roster differs from visual bindings";return false;}
    for(const auto& entry:s.entries){
        const auto* actor=s.world->find_actor(entry.first);
        if(!actor||!s.world->traits(entry.first)||!validate_actor_state(*actor,error)){
            if(error.empty())error="Restored actor/traits unavailable for visual binding";return false;
        }
    }
    const auto fail=[&](){s.runtime->clear();s.combat->clear();s.events.clear();s.resolutions.clear();return false;};
    for(auto& entry:s.entries){
        auto* actor=s.world->find_actor(entry.first);
        entry.second.traits=*s.world->traits(entry.first);
        if(!entry.second.animationOnly)entry.second.attack.maximum_range=s.world->melee_reach(entry.first);
        entry.second.turnPositive=0;entry.second.rotationFractionMs=0;entry.second.rotationUpdateSerial=0;
        if(entry.second.sourceCombo){entry.second.sourceAttack={};entry.second.sourceAttack.owner=entry.first;entry.second.sourceAttack.object_of_interest_type=-1;
            entry.second.targetProjection.lastTarget=invalid_actor_id;entry.second.targetProjection.lastTargetKnown=true;
            entry.second.targetProjection.objectOfInterest=invalid_actor_id;entry.second.targetProjection.objectOfInterestType=-1;
            entry.second.targetProjection.objectOfInterestKnown=false;}
        entry.second.locomotionSelected.clear();
        if(entry.second.animationOnly)actor->action=actor->alive()?CharacterAction::idle:CharacterAction::dead;
        else reset_actor_action(*actor,CharacterAction::idle);
        actor->target_id=invalid_actor_id;
        entry.second.poses.restore_dead_terminal=!actor->alive();
        if(entry.second.sequence){
            auto selection=*entry.second.sequenceCallerSelection;
            const auto* properties=s.world->combat_properties(entry.first);
            volatile float effectiveRate=static_cast<float>(selection.actor_rate)*original_speed_modifier(properties->sheets.resolved[48]);
            selection.actor_rate=effectiveRate;
            const auto services=s.sequence_services(entry.second.visual,entry.first);
            if(!entry.second.sequence->prepare(*entry.second.sequencePlan,s.sequencePolicies,selection,services,entry.second.attack.animation_clip_id,error))return fail();
            if(entry.second.retainedPhaseClock&&!s.prepare_retained(entry.second,entry.first,selection,error))return fail();
        }
        if(entry.second.animationOnly&&!entry.second.receiveDamage&&entry.second.presentationInitialState&&
           !s.publish_source_state(entry.first,*entry.second.presentationInitialState,error))return fail();
        if(entry.second.animationOnly&&!s.prepare_animation_only(entry.second,entry.first,error))return fail();
        if(!entry.second.retainedPhaseClock&&(!entry.second.visual->select(entry.second.idle,true,error)||
           !entry.second.visual->update(0,error)))return fail();
        const auto visualBinding=s.pose_motion_binding(
            entry.second.retainedPhaseClock?s.retained_binding(entry.second):entry.second.sequence?entry.second.sequence->binding():combat_visual_binding(*entry.second.visual),
            entry.second.visual,entry.first);
        if(!s.runtime->bind(*actor,visualBinding,s.pose_admission(entry.first,entry.second.poses),error))return fail();
    }
    // Restart configured dead poses at time zero; no action/marker history or
    // cooldown survives a restore. Source registrations and exact RNG remain.
    if(!s.runtime->update(0,s.events,error))return fail();
    for(const auto& entry:s.entries)if(!s.facts(entry.first,error))return fail();
    s.events.clear();s.resolutions.clear();s.world->take_resolutions();
    // The quiescent witness admitted this restore. The host reconstructs its
    // generic coordinator on the new lease; retain unrelated observers/policy.
    for(auto& pair:s.entries)if(pair.second.stateServices.checkpoint){
        pair.second.stateServices={};pair.second.sourceStatePolicy.reset();
        pair.second.stateManaged=pair.second.stateSequence=pair.second.stateFrozen=false;
    }
    s.bindingLease=std::make_shared<const unsigned char>(0);s.detached=false;
    s.logs.push_back("Restore rebind reset targets/action cursors/cooldowns; exact restored RNG and source registrations retained");
    return true;
}
PlayableActorWorld* CombatSession::world()noexcept{return impl_?impl_->world.get():nullptr;}
std::weak_ptr<const void> CombatSession::actor_binding_lease()const noexcept{
    return impl_&&!impl_->detached?impl_->bindingLease:std::weak_ptr<const void>{};
}
std::weak_ptr<const CombatSessionLifetime> CombatSession::lifetime_lease()const noexcept{
    return impl_?impl_->ownerLifetime:std::weak_ptr<const CombatSessionLifetime>{};
}
bool CombatSession::bind_object_visual(ObjectId id,const AssetCatalog& assets,std::string& error){
    error.clear();try{
    if(!impl_||impl_->detached||!impl_->world->find_object(id)||impl_->objectEntries.count(id)){
        error="Object visual requires an unbound neutral object in the current Session";return false;
    }
    const auto* object=impl_->world->find_object(id);
    auto entry=std::make_shared<Impl::ObjectEntry>();entry->model=object->visual.model;
    if(!entry->visual.load_embedded_scene(assets,entry->model,error))return false;
    // Game-object parts animate in their authored model scene; no artificial
    // Character motion root or world displacement is extracted.
    entry->animation=std::make_unique<RetainedAnimationOwner>(entry->visual,
        [](const std::string&,std::int32_t,std::array<float,3>& point,std::string&){point={0,0,0};return true;});
    const auto bytes=assets.read(entry->model);std::vector<EmbeddedSceneClip> clips;
    if(!decode_embedded_scene_clips(bytes.data(),bytes.size(),clips,error))return false;
    for(const auto& clip:clips)if(!entry->animation->bind_events(clip.name,bytes.data(),bytes.size(),error))return false;
    impl_->objectEntries.emplace(id,std::move(entry));impl_->objectOrder.push_back(id);return true;
    }catch(const std::exception& ex){error=ex.what();return false;}
}
bool CombatSession::unbind_object_visual(ObjectId id){
    if(!impl_||impl_->detached||!impl_->objectEntries.erase(id))return false;
    auto& order=impl_->objectOrder;order.erase(std::remove(order.begin(),order.end(),id),order.end());return true;
}
CharacterVisual* CombatSession::retained_object_visual_borrow(ObjectId id)noexcept{
    if(!impl_||impl_->detached||!impl_->world->find_object(id))return nullptr;
    const auto found=impl_->objectEntries.find(id);
    return found==impl_->objectEntries.end()||impl_->world->find_object(id)->visual.model!=found->second->model?nullptr:&found->second->visual;
}
const CharacterVisual* CombatSession::retained_object_visual_borrow(ObjectId id)const noexcept{
    return const_cast<CombatSession*>(this)->retained_object_visual_borrow(id);
}
bool CombatSession::bind_object_animation_services(ObjectId id,CombatSessionObjectAnimationServices services,std::string& error){
    error.clear();if(!retained_object_visual_borrow(id)){error="Object callbacks require current Session visual";return false;}
    impl_->objectEntries.at(id)->services=std::move(services);return true;
}
bool CombatSession::play_object_clip(ObjectId id,const std::string& clip,bool loop,bool& accepted,std::string& error){
    accepted=false;error.clear();if(!retained_object_visual_borrow(id)){error="Object clip requires current Session visual";return false;}
    const auto entry=impl_->objectEntries.at(id);std::int32_t start{},end{};
    if(!entry->visual.animation_range(clip,start,end,error)){error.clear();return true;}
    if(entry->selection==UINT64_MAX){error="Object animation selection exhausted";return false;}
    RetainedAnimationFrame frame;if(!entry->animation->select(clip,loop,1,0,false,frame,error))return false;
    ++entry->selection;entry->selected=true;entry->flags|=0x200u;accepted=true;
    return impl_->dispatch_object(id,entry,frame,error);
}
bool CombatSession::restore_object_pose(ObjectId id,const std::string& clip,bool loop,std::string& error){
    error.clear();if(!retained_object_visual_borrow(id)){error="Object restore pose requires current Session visual";return false;}
    const auto entry=impl_->objectEntries.at(id);std::int32_t start{},end{};
    if(!entry->visual.animation_range(clip,start,end,error))return false;
    if(entry->selection==UINT64_MAX){error="Object animation selection exhausted";return false;}
    RetainedAnimationFrame ignored;if(!entry->animation->select(clip,loop,1,0,false,ignored,error))return false;
    entry->animation->take_completion();++entry->selection;entry->selected=true;entry->flags|=0x200u;return true;
}
bool CombatSession::set_object_scene_flags(ObjectId id,std::uint32_t clear,std::uint32_t set,std::string& error){
    error.clear();if(!retained_object_visual_borrow(id)){error="Object scene flags require current Session visual";return false;}
    auto& flags=impl_->objectEntries.at(id)->flags;flags=(flags&~clear)|set;return true;
}
const PlayableActorWorld* CombatSession::world()const noexcept{return impl_?impl_->world.get():nullptr;}
const dh2::data::AiTables* CombatSession::original_ai_tables()const noexcept{return impl_&&impl_->world?&impl_->world->factions():nullptr;}
std::int32_t CombatSession::original_actor_state(ActorId id)const noexcept{return impl_&&impl_->entries.count(id)?impl_->live_original_state(id):-1;}
bool CombatSession::borrow_rotation_turn(ActorId id,std::uint32_t*& out,std::string& error){
    if(!impl_||impl_->detached||!impl_->entries.count(id)||!impl_->world->find_actor(id)){error="Rotation turn borrow requires current shared actor/session";return false;}
    out=&impl_->entries.at(id).turnPositive;error.clear();return true;
}
ActorState* CombatSession::actor(ActorId id)noexcept{return impl_?impl_->world->find_actor(id):nullptr;}
const ActorState* CombatSession::actor(ActorId id)const noexcept{return impl_?impl_->world->find_actor(id):nullptr;}
ActorId CombatSession::player_id()const noexcept{return impl_?impl_->player:invalid_actor_id;}
bool CombatSession::owns_pose(ActorId id)const noexcept{if(!impl_||impl_->detached)return false;const auto entry=impl_->entries.find(id);return impl_->runtime->owns_pose(id)||(entry!=impl_->entries.end()&&entry->second.stateManaged&&(entry->second.stateSequence||entry->second.stateFrozen||!entry->second.retained->seeded()));}
bool CombatSession::owns_population_pose(std::uint64_t id)const noexcept{return impl_&&id!=impl_->player&&impl_->entries.count(id);}
const ActorState* CombatSession::selectedactor()const noexcept{const auto* p=actor(player_id());return p?actor(p->target_id):nullptr;}
const std::vector<DamageEvent>& CombatSession::events()const noexcept{static const std::vector<DamageEvent> empty;return impl_?impl_->events:empty;}
const std::vector<PlayableCombatResolution>& CombatSession::resolutions()const noexcept{static const std::vector<PlayableCombatResolution> empty;return impl_?impl_->resolutions:empty;}
const std::vector<std::string>& CombatSession::logs()const noexcept{static const std::vector<std::string> empty;return impl_?impl_->logs:empty;}
const OriginalAttackSequence* CombatSession::attack_sequence(ActorId id)const noexcept{
    if(!impl_)return nullptr;const auto entry=impl_->entries.find(id);return entry==impl_->entries.end()?nullptr:entry->second.sequence.get();
}
bool CombatSession::bind_player_locomotion(const std::string& alias,const CombatSessionChoice& choice,
    double actor_rate,bool loop,std::string& error){
    return bind_actor_locomotion(player_id(),alias,choice,actor_rate,loop,error);
}
bool CombatSession::bind_actor_locomotion(ActorId id,const std::string& alias,const CombatSessionChoice& choice,
    double actor_rate,bool loop,std::string& error){
    if(!impl_||impl_->detached||!impl_->entries.count(id)||!impl_->entries.at(id).sequencePlan){error="Actor locomotion requires a bound initialized source plan";return false;}
    return bind_actor_locomotion_from_bank(id,alias,*impl_->entries.at(id).sequencePlan,choice,actor_rate,loop,error);
}
bool CombatSession::bind_actor_locomotion_from_bank(ActorId id,const std::string& alias,
    const OriginalCombatVisualPlan& bank,const CombatSessionChoice& choice,
    double actor_rate,bool loop,std::string& error){
    error.clear();if(!impl_||impl_->detached||!impl_->entries.count(id)){error="Actor locomotion requires a bound initialized session actor";return false;}
    auto& entry=impl_->entries.at(id);
    if(!entry.retainedPhaseClock||!entry.retained||!entry.sequencePlan){error="Player locomotion requires retained source phase ownership";return false;}
    if(alias.empty()||!std::isfinite(actor_rate)||actor_rate<=0||actor_rate>std::numeric_limits<float>::max()){error="Explicit player locomotion alias/rate is invalid";return false;}
    try{
        const auto& selected=choose(bank,choice);
        const auto* sequence=bank.sequence(choice.state,choice.variant);
        if(!sequence||sequence->loop< -1||sequence->loop>INT32_MAX||!((sequence->type==0&&sequence->phases.size()==1)||(sequence->type==2&&!choice.leafPath.empty()))){error="Player locomotion requires source single-leaf type0 or explicitly pinned type2 choice";return false;}
        if(loop&&sequence->loop==0){error="Player locomotion repeat cannot be fabricated for finite source state";return false;}
        if(selected.blendOut<INT32_MIN||selected.blendOut>INT32_MAX){error="Player locomotion BlendOut exceeds native width";return false;}
        std::int32_t start=0,end=0;if(!entry.visual->animation_range(selected.clipName,start,end,error))return false;
        volatile float rate=static_cast<float>(actor_rate)*static_cast<float>(selected.speed);
        if(!std::isfinite(rate)||rate<=0){error="Player locomotion effective rate is invalid";return false;}
        // Updating future locomotion metadata does not select or advance a
        // pose. Equipment may change while an attack is paused in the menu;
        // select_actor_locomotion still protects the active combat owner.
        if(entry.locomotionSelected==alias)entry.locomotionSelected.clear();
        const auto state=choice.state=="Idle"?3:(choice.state=="Walk"||choice.state=="Run")?4:-1;
        entry.locomotion[alias]={selected,rate,loop?static_cast<std::int32_t>(sequence->loop):0,state};return true;
    }catch(const std::exception& failure){error=failure.what();return false;}
}
bool CombatSession::select_player_locomotion(const std::string& alias,std::string& error){
    return select_actor_locomotion(player_id(),alias,error);
}
bool CombatSession::select_actor_locomotion(ActorId id,const std::string& alias,std::string& error){
    error.clear();if(!impl_||impl_->detached||!impl_->entries.count(id)){error="Actor locomotion requires a bound initialized session actor";return false;}
    if(impl_->transitionDelivering||impl_->checkingAnimationCheckpoint||impl_->restoreTeardown){error="Locomotion cannot reenter actor transition/checkpoint/teardown";return false;}
    auto& entry=impl_->entries.at(id);const auto policy=entry.locomotion.find(alias);
    if(!entry.retainedPhaseClock||!entry.retained||policy==entry.locomotion.end()){error="Explicit player locomotion alias is not bound: "+alias;return false;}
    const auto* actor=impl_->world->find_actor(id);
    if(!actor||!actor->alive()||impl_->runtime->owns_pose(id)||entry.stateSequence){error="Combat/death/source sequence owns actor pose; locomotion selection is unavailable";return false;}
    if(entry.locomotionSelected==alias&&entry.sourceSelectedClip==policy->second.phase.clipName&&!entry.sourceAction)return true;
    auto& s=*impl_;const auto& phase=policy->second.phase;RetainedAnimationFrame frame;
    CombatRuntimeTransition receipt;
    if(s.actorTransitionHandler){
        const auto* properties=s.world->combat_properties(id);
        if(policy->second.originalState<0||!properties){error="Physical locomotion needs an explicit supported original Idle/Walk/Run state";return false;}
        receipt={id,properties->facts.original_state,policy->second.originalState,0,CombatRuntimeTransitionCause::locomotion};
        // Walk/Run alias changes are Move.UpdateType, not a new state admission.
        if(receipt.from_state==receipt.to_state)receipt.from_state=-1;
        if(receipt.from_state!=-1&&!s.actor_transition(id,receipt,error))return false;
    }
    if(!entry.retained->seed_sequence(*impl_->assets,phase.clipName,phase.resolvedPath,policy->second.rate,
        static_cast<std::int32_t>(phase.blendOut),phase.moveGO!=0,policy->second.repeats,frame,error))return false;
    entry.sourceAction=false;entry.sourceEvents.clear();entry.sourceSerial=0;entry.sourceSelectedClip=phase.clipName;entry.locomotionSelected=alias;
    if(s.actorTransitionHandler){
        reset_actor_action(*s.world->find_actor(id),policy->second.originalState==4?CharacterAction::moving:CharacterAction::idle);
        if(receipt.from_state!=-1){receipt.stage=CombatRuntimeTransitionStage::after_change;if(!s.actor_transition(id,receipt,error))return false;}
    }
    return true;
}
bool CombatSession::uses_retained_player_locomotion()const noexcept{
    if(!impl_||impl_->detached)return false;const auto entry=impl_->entries.find(impl_->player);return entry!=impl_->entries.end()&&entry->second.retainedPhaseClock&&entry->second.retained&&!entry->second.locomotion.empty();
}
bool CombatSession::has_player_locomotion(const std::string& alias)const noexcept{
    if(!impl_||impl_->detached)return false;const auto entry=impl_->entries.find(impl_->player);return entry!=impl_->entries.end()&&entry->second.retainedPhaseClock&&entry->second.locomotion.count(alias);
}
const RetainedPosePlayback* CombatSession::retained_player_pose()const noexcept{
    return retained_actor_pose(player_id());
}
const RetainedPosePlayback* CombatSession::retained_actor_pose(ActorId id)const noexcept{
    if(!impl_||impl_->detached)return nullptr;const auto entry=impl_->entries.find(id);return entry!=impl_->entries.end()&&entry->second.retained?&entry->second.retained->animation()->pose():nullptr;
}
const CharacterVisual* CombatSession::retained_actor_visual_borrow(ActorId id)const noexcept{
    if(!impl_||impl_->detached)return nullptr;
    const auto entry=impl_->entries.find(id);
    return entry==impl_->entries.end()?nullptr:entry->second.visual;
}
bool CombatSession::with_locomotion_preview_pose(ActorId id,const std::string& alias,double seconds,
        const LocomotionPreviewDrawV1& draw,std::string& error){
    error.clear();
    if(!impl_||impl_->detached||!impl_->entries.count(id)){error="Locomotion preview requires a bound initialized session actor";return false;}
    if(!draw){error="Locomotion preview draw callback is required";return false;}
    if(!std::isfinite(seconds)||seconds<0){error="Locomotion preview clock must be finite and nonnegative";return false;}
    auto& entry=impl_->entries.at(id);
    const auto policy=entry.locomotion.find(alias);
    if(!entry.retainedPhaseClock||!entry.retained||!entry.visual||policy==entry.locomotion.end()){
        error="Explicit locomotion alias is not bound: "+alias;return false;
    }
    CharacterVisual& visual=*entry.visual;
    LocomotionPreviewPoseV1 preview;
    preview.alias=alias;preview.clip=policy->second.phase.clipName;
    std::int32_t start=0,end=0;
    if(!visual.animation_range(preview.clip,start,end,error))return false;
    // The preview owns its clock: authored source time from clip start, looped
    // over the clip range, independent of the actor's current action clock.
    const double span=std::max(1.0,static_cast<double>(end)-static_cast<double>(start));
    preview.source_ms=start+static_cast<std::int32_t>(std::fmod(seconds*1000.0,span));
    SkeletalPose live,presented;
    if(!visual.current_local_pose(live,error))return false;
    if(!visual.sample_local_pose(preview.clip,preview.source_ms,presented,error))return false;
    if(!visual.apply_local_pose(presented,error))return false;
    bool drawn=false;
    try{drawn=draw(preview,error);}
    catch(const std::exception& failure){error=std::string("Locomotion preview draw threw: ")+failure.what();drawn=false;}
    catch(...){error="Locomotion preview draw threw an unknown exception";drawn=false;}
    // Restore the exact live pose through the same publication path combat uses,
    // so the next combat update or the closed page sees the actor it left behind.
    std::string restoreError;
    if(!visual.apply_local_pose(live,restoreError)){
        error=drawn?"Locomotion preview restore failed: "+restoreError
                   :error+" (restore also failed: "+restoreError+")";
        return false;
    }
    if(!drawn){if(error.empty())error="Locomotion preview draw rejected";return false;}
    error.clear();return true;
}
void CombatSession::set_actor_combat_permission_provider(CombatPermissionProvider provider){if(impl_){impl_->permissionProvider=std::move(provider);if(impl_->permissionProvider)impl_->lifecycleRegistered=true;}}
void CombatSession::set_diagnostic_controller_admission_provider(DiagnosticControllerAdmissionProvider provider,
    ControllerNetworkModeProvider network){if(impl_){impl_->controllerAdmissionProvider=std::move(provider);impl_->networkModeProvider=std::move(network);}}
void CombatSession::clear_diagnostic_controller_admission_provider(){if(impl_){impl_->controllerAdmissionProvider={};impl_->networkModeProvider={};}}
bool CombatSession::uses_diagnostic_controller_admission()const noexcept{return impl_&&bool(impl_->controllerAdmissionProvider);}
void CombatSession::set_animation_notification_services(CombatSessionAnimationNotificationServices services){
    if(!impl_)return;
    auto& s=*impl_;
    if(s.restoreTeardown||s.checkingAnimationCheckpoint||s.transitionDelivering)
        throw std::logic_error("Cannot replace animation notifications during checkpoint validation or restore teardown");
    s.animationNotificationServices=std::move(services);s.animationNotificationCheckpoint={};
    s.animationNotificationLease.reset();s.animationNotificationsReconstructible=false;
}
void CombatSession::clear_animation_notification_services(){
    if(!impl_)return;
    auto& s=*impl_;
    if(s.restoreTeardown||s.checkingAnimationCheckpoint||s.transitionDelivering)
        throw std::logic_error("Cannot clear animation notifications during checkpoint validation or restore teardown");
    s.animationNotificationServices={};s.animationNotificationCheckpoint={};
    s.animationNotificationLease.reset();s.animationNotificationsReconstructible=false;
}
bool CombatSession::bind_reconstructible_animation_notifications(
    CombatSessionAnimationNotificationServices services,std::function<bool(std::string&)> validator,std::string& error){
    error.clear();
    if(!impl_||impl_->detached||impl_->updating||impl_->restoreTeardown||impl_->checkingAnimationCheckpoint||impl_->transitionDelivering||
       impl_->applyingSourceHit||!impl_->pendingMotion.empty()||!impl_->bindingLease||
       (!services.notification&&!services.state_event)||!validator){
        error="Reconstructible notifications require idle current Session, callbacks and checkpoint validator";return false;
    }
    auto& s=*impl_;const auto lease=s.bindingLease;
    struct Scope{bool& flag;~Scope(){flag=false;}} scope{s.checkingAnimationCheckpoint};s.checkingAnimationCheckpoint=true;
    try{
        if(!validator(error)){if(error.empty())error="Reconstructible animation binding validator rejected";return false;}
        if(s.detached||!s.bindingLease||lease.owner_before(s.bindingLease)||s.bindingLease.owner_before(lease)){
            error="Animation binding validator changed the actor binding";return false;
        }
        s.animationNotificationServices=std::move(services);s.animationNotificationCheckpoint=std::move(validator);
        s.animationNotificationLease=lease;s.animationNotificationsReconstructible=true;
        s.animationNotificationsNeedRebind=false;error.clear();return true;
    }catch(const std::exception& ex){error=ex.what();return false;}
      catch(...){error="Reconstructible animation binding validator threw";return false;}
}
bool CombatSession::deliver_original_animation_notification(ActorId id,std::uint32_t event,std::uintptr_t payload,std::string& error){
    if(!impl_||impl_->detached||!impl_->entries.count(id)){error="Animation notification actor unavailable";return false;}
    if(impl_->restoreTeardown||impl_->checkingAnimationCheckpoint||impl_->transitionDelivering||impl_->animationNotificationsNeedRebind){
        error="Animation notification requires fresh binding outside checkpoint validation/restore teardown";return false;
    }
    return impl_->numeric_animation_event(id,event,payload,error);
}
const std::vector<CombatSessionAnimationDispatch>& CombatSession::original_animation_dispatches()const noexcept{
    static const std::vector<CombatSessionAnimationDispatch> empty;return impl_?impl_->animationDispatches:empty;
}
bool CombatSession::refresh_actor_combat_permissions(std::string& error){
    error.clear();if(!impl_||impl_->detached){error="Actor permissions require attached initialized session";return false;}
    auto& s=*impl_;try{for(auto& pair:s.entries){auto& entry=pair.second;bool allowed=!s.permissionProvider||s.permissionProvider(pair.first);allowed=allowed&&!entry.lifecycleOriginalState;
        entry.permission=allowed;entry.traits.targetable=entry.baseTargetable&&allowed;
        if(!allowed){if(s.runtime->owns_pose(pair.first)&&!s.runtime->interrupt(pair.first,error))return false;s.combat->interrupt(pair.first);entry.sourceEvents.clear();s.world->find_actor(pair.first)->target_id=invalid_actor_id;}
        if(!s.facts(pair.first,error))return false;
    }for(const auto& pair:s.entries){auto* actor=s.world->find_actor(pair.first);const auto target=s.entries.find(actor->target_id);if(target!=s.entries.end()&&!target->second.permission)actor->target_id=invalid_actor_id;}
    }catch(const std::exception& failure){error=failure.what();return false;}return true;
}
bool CombatSession::set_actor_original_state(ActorId id,std::int32_t state,std::string& error){
    if(!impl_||impl_->detached||!impl_->entries.count(id)){error="Original lifecycle state actor is unavailable";return false;}
    if(state!=0&&state!=1&&state!=2&&state!=3&&state!=17){error="Session lifecycle state must be original0/1/2/3/17";return false;} // P16 DESPAWN: 2 = Despawn
    auto& entry=impl_->entries.at(id);
    // P16 LIFECYCLE: under a bound handler the target was already published by its admitted transition (idempotent).
    if(impl_->actorTransitionHandler&&!impl_->publish_source_state(id,state,error))return false;
    entry.lifecycleOriginalState=state==3?std::optional<std::int32_t>{}:state;impl_->lifecycleRegistered=true;return refresh_actor_combat_permissions(error);
}
bool CombatSession::select_actor_state_leaf(ActorId id,const CombatSessionChoice& choice,double actor_rate,bool frozen,
    CombatSessionStateAnimationServices services,std::string& error,std::int32_t lifecycle_to_state){
    if(!impl_||impl_->detached||!impl_->entries.count(id)){error="State-animation actor is unavailable";return false;}
    auto& s=*impl_;auto& entry=s.entries.at(id);if(!entry.retained||!entry.sequencePlan){error="State animation requires explicit retained source profile";return false;}
    if(s.actorTransitionHandler&&lifecycle_to_state<0){error="Legacy lifecycle leaf selection has no admitted physical transition recipe";return false;}
    if(entry.dispatchingDeparture||(entry.stateSequence&&entry.sourceStatePolicy)){error="State leaf replacement requires accepted source departure";return false;}
    if(!std::isfinite(actor_rate)||actor_rate<=0||actor_rate>std::numeric_limits<float>::max()){error="State animation caller rate is invalid";return false;}
    // P16 LIFECYCLE: admitted legacy transition = Blur -> publish -> Focus prefix, selection, Focus suffix.
    CombatRuntimeTransition receipt;
    if(s.actorTransitionHandler){
        receipt={id,s.live_original_state(id),lifecycle_to_state,0,CombatRuntimeTransitionCause::source_program};
        if(!s.actor_transition(id,receipt,error))return false;
    }
    try{const auto& selected=choose(*entry.sequencePlan,choice);volatile float rate=static_cast<float>(actor_rate)*static_cast<float>(selected.speed);if(!std::isfinite(rate)){error="State animation effective rate overflow";return false;}
        const auto* sequence=entry.sequencePlan->sequence(choice.state,choice.variant);RetainedAnimationFrame frame;
        s.runtime->interrupt(id);s.combat->interrupt(id);
        if(frozen){if(!entry.retained->seed_sequence(*s.assets,selected.clipName,selected.resolvedPath,0,static_cast<int>(selected.blendOut),selected.moveGO!=0,0,frame,error))return false;}
        else if(!s.seed_source(entry,selected,sequence&&sequence->loop!=0,rate,*entry.retained,frame,error))return false;
        entry.stateManaged=true;entry.stateSequence=false;entry.stateFrozen=frozen;entry.stateServices=std::move(services);entry.sourceAction=false;entry.sourceSelectedClip=selected.clipName;entry.locomotionSelected.clear();entry.sourceEvents.clear();s.lifecycleRegistered=true;error.clear();
    }catch(const std::exception& failure){error=failure.what();return false;}
    if(s.actorTransitionHandler){receipt.stage=CombatRuntimeTransitionStage::after_change;return s.actor_transition(id,receipt,error);}
    return true;
}
bool CombatSession::play_actor_state_sequence(ActorId id,const OriginalAttackSelection& selection,
    CombatSessionStateAnimationServices services,std::string& error,std::int32_t lifecycle_to_state){
    if(!impl_||impl_->detached||!impl_->entries.count(id)){error="State-sequence actor is unavailable";return false;}
    const auto& entry=impl_->entries.at(id);
    if(!entry.sequencePlan){error="Whole state sequence needs retained profile";return false;}
    CombatSessionSourceSequencePolicy policy;policy.lifecycle_to_state=lifecycle_to_state;
    return play_actor_source_sequence(id,*entry.sequencePlan,impl_->sequencePolicies,selection,std::move(services),policy,error);
}
bool CombatSession::play_actor_source_sequence(ActorId id,const OriginalCombatVisualPlan& plan,
    const OriginalSequencePolicies& policies,const OriginalAttackSelection& selection,
    CombatSessionStateAnimationServices services,std::string& error){
    return play_actor_source_sequence(id,plan,policies,selection,std::move(services),{},error);
}
bool CombatSession::play_actor_source_sequence(ActorId id,const OriginalCombatVisualPlan& plan,
    const OriginalSequencePolicies& policies,const OriginalAttackSelection& selection,
    CombatSessionStateAnimationServices services,const CombatSessionSourceSequencePolicy& policy,std::string& error){
    if(!impl_||impl_->detached||!impl_->entries.count(id)){error="Source-sequence actor is unavailable";return false;}
    auto& s=*impl_;auto& entry=s.entries.at(id);
    const bool generic=policy.original_state==6||policy.original_state==7||policy.original_state==10;
    if(!generic&&(policy.original_state!=-1||policy.state_flags||policy.generation)){
        error="Source sequence policy must describe Skill6/Cast7/KnockedBack10 or legacy lifecycle playback";return false;
    }
    if(generic&&(!policy.generation||!services.departed||!services.checkpoint)){
        error="Generic source sequence needs occurrence, departure and transient checkpoint policies";return false;
    }
    if(policy.original_state==10){
        const auto* actor=s.world->find_actor(id);const auto* traits=s.world->traits(id);
        const auto* other=s.world->find_actor(policy.source_other_actor);
        const auto state=s.live_original_state(id);
        if(!actor||!actor->alive()||!traits||!traits->is_player||!other||
           policy.state_flags!=0x2341u||(!policy.source_direct_transition&&state!=3&&state!=11)){
            error="KnockedBack10 requires its live player/attacker, source flags and admitted current state";return false;
        }
    }
    if(entry.dispatchingDeparture||(entry.stateSequence&&entry.sourceStatePolicy)){
        error="Active generic source sequence requires an accepted departure before replacement";return false;
    }
    if(!entry.retained||!services.finished){error="Whole source sequence needs retained owner and completion provider";return false;}
    // Validate supplied hierarchy, rates and existing visual clips before
    // interrupting a live action. No animation owner or clock is allocated.
    OriginalAttackSequence validation;
    OriginalAttackSequenceServices validationServices;
    validationServices.visual=combat_visual_binding(*entry.visual);
    validationServices.restart_clip=[](const std::string&,std::string&){return true;};
    const auto alias="actor-"+std::to_string(id)+"/state-sequence";
    if(!validation.prepare(plan,policies,selection,std::move(validationServices),alias,error))return false;
    if(s.transitionDelivering||s.restoreTeardown||s.checkingAnimationCheckpoint){error="Source program cannot reenter transition/checkpoint/teardown";return false;}
    CombatRuntimeTransition receipt;
    if(s.actorTransitionHandler){
        // P16 LIFECYCLE: a legacy OriginalActorLifecycle program is admitted only with its explicit target state.
        if(!generic&&policy.lifecycle_to_state!=1&&policy.lifecycle_to_state!=2&&policy.lifecycle_to_state!=3&&policy.lifecycle_to_state!=17){
            error="Legacy lifecycle source program has no admitted physical transition recipe";return false;
        }
        const auto to=generic?policy.original_state:policy.lifecycle_to_state;
        receipt={id,s.world->combat_properties(id)->facts.original_state,to,generic?policy.generation:0,CombatRuntimeTransitionCause::source_program};
        if(!s.actor_transition(id,receipt,error,nullptr,&policy))return false;
    }
    {struct Scope{bool& flag;~Scope(){flag=false;}} scope{s.suppressRuntimeTransitions};s.suppressRuntimeTransitions=true;
        if(s.runtime->owns_pose(id)&&!s.runtime->interrupt(id,error))return false;
        s.combat->interrupt(id);
        // Interrupted source combo swing: drop continuation/last so the next swing starts fresh.
        entry.sourceAttack.continued=0;entry.sourceAttack.last=0;
    }
    if(!entry.retained->prepare_preserving(*s.assets,plan,policies,selection,s.retained_services(id),alias,error))return false;
    entry.attackProgram=false;entry.stateManaged=true;entry.stateSequence=true;entry.stateFrozen=false;entry.stateServices=std::move(services);entry.sourceAction=false;entry.sourceEvents.clear();entry.locomotionSelected.clear();
    entry.sourceStatePolicy=generic?std::optional<CombatSessionSourceSequencePolicy>{policy}:std::nullopt;
    if(!generic)s.lifecycleRegistered=true;
    if(policy.original_state==10)reset_actor_action(*s.world->find_actor(id),CharacterAction::knocked_back);
    if(generic&&!s.facts(id,error))return false;
    if(!entry.retained->begin(error))return false;
    if(s.actorTransitionHandler){receipt.stage=CombatRuntimeTransitionStage::after_change;return s.actor_transition(id,receipt,error);}
    return true;
}
bool CombatSession::cancel_actor_source_sequence(ActorId id,std::weak_ptr<const void> lease,
    std::uint64_t generation,std::int32_t next_state,bool& departed,std::string& error){
    departed=false;error.clear();
    if(!impl_||impl_->detached||!impl_->entries.count(id)||lease.expired()||
       lease.owner_before(impl_->bindingLease)||impl_->bindingLease.owner_before(lease)||
       !generation||next_state<0||next_state>18){
        error="Source departure needs current Session actor/lease, occurrence and destination";return false;
    }
    auto& entry=impl_->entries.at(id);
    if(!entry.stateSequence||!entry.sourceStatePolicy||entry.sourceStatePolicy->generation!=generation)return true;
    if(entry.dispatchingDeparture){error="Source departure cannot reenter";return false;}
    if(impl_->actorTransitionHandler){
        if(next_state!=3&&next_state!=12){error="Incoming physical state must use its accepted Runtime admission";return false;}
        CombatRuntimeTransition receipt{id,entry.sourceStatePolicy->original_state,next_state,generation,CombatRuntimeTransitionCause::interruption};
        departed=true;if(!impl_->actor_transition(id,receipt,error))return false;
        receipt.stage=CombatRuntimeTransitionStage::after_change;return impl_->actor_transition(id,receipt,error);
    }
    departed=true;return impl_->depart_source_state(id,next_state,error);
}
bool CombatSession::freeze_actor_state_animation(ActorId id,std::string& error){
    if(!impl_||impl_->detached||!impl_->entries.count(id)){error="Frozen state-animation actor unavailable";return false;}auto& entry=impl_->entries.at(id);
    if(!entry.retained||!entry.stateManaged){error="Explicit state animation must be selected before freeze";return false;}
    if(!entry.retained->set_source_rate(0,error))return false;entry.stateFrozen=true;impl_->lifecycleRegistered=true;return true;
}
bool CombatSession::validate_lifecycle_checkpoint(std::string& error)const{
    // Presentation observes an already-applied gameplay event. Its live output
    // and device clock are transient host state; registering it does not add
    // campaign/controller state to the gameplay checkpoint.
    if(impl_&&(impl_->lifecycleRegistered||impl_->permissionProvider||impl_->controllerAdmissionProvider||impl_->networkModeProvider)){error="Campaign lifecycle/controller providers are not persisted; checkpoint/restore requires explicit serialization or clearing transient services";return false;}
    if(impl_){
        if(impl_->detached){if(!impl_->sourceCheckpointReadyAtDetach){error="Detached source program had no quiescent checkpoint admission";return false;}}
        else if(!impl_->source_checkpoint(error)||!impl_->animation_checkpoint(error))return false;
        if(impl_->detached&&(impl_->animationNotificationServices.notification||impl_->animationNotificationServices.state_event)){
            error="Detached volatile animation providers are not persisted";return false;
        }
    }
    error.clear();return true;
}
bool CombatSession::clear_lifecycle_services(std::string& error){
    if(!impl_){error.clear();return true;}auto& s=*impl_;
    // This legacy cleanup is not an admitted generic Skill/Cast exit and must
    // not erase Post or outstanding cooldowns to bypass checkpoint admission.
    if(s.detached){if(!s.sourceCheckpointReadyAtDetach){error="Cannot clear an unpersisted detached source program";return false;}}
    else if(!s.source_checkpoint(error))return false;
    s.permissionProvider={};s.retainedFrameAudioObserver={};s.retainedFrameAudioDiagnostics.clear();s.stepObserver={};s.stepDiagnostics.clear();s.lifecycleRegistered=false;s.sourceCheckpointReadyAtDetach=true;
    for(auto& pair:s.entries){auto& e=pair.second;if(e.stateManaged&&e.retained){e.retained->cancel();e.sourceSelectedClip.clear();e.attackProgram=false;}e.stateManaged=e.stateSequence=e.stateFrozen=false;e.stateServices={};e.sourceStatePolicy.reset();e.lifecycleOriginalState.reset();e.permission=true;e.traits.targetable=e.baseTargetable;}
    if(s.detached){error.clear();return true;}return refresh_actor_combat_permissions(error);
}
void CombatSession::set_attack_owner_provider(AttackOwnerProvider provider){if(impl_)impl_->attackOwnerProvider=std::move(provider);}
const dh2::character::AttackState64* CombatSession::source_attack_state(ActorId id)const noexcept{
    if(!impl_||impl_->detached)return nullptr;const auto found=impl_->entries.find(id);
    return found==impl_->entries.end()||!found->second.sourceCombo?nullptr:&found->second.sourceAttack;
}
CombatSessionTargetPresentation CombatSession::target_presentation_state(ActorId id)const noexcept{
    CombatSessionTargetPresentation result;
    if(!impl_||impl_->detached)return result;
    const auto* actor=impl_->world->find_actor(id);if(!actor)return result;
    result.current=actor->target_id;
    const auto found=impl_->entries.find(id);if(found==impl_->entries.end())return result;
    const auto& source=found->second.targetProjection;
    result.last_target=source.lastTarget;result.last_target_known=source.lastTargetKnown;
    result.object_of_interest=source.objectOfInterest;result.object_of_interest_type=source.objectOfInterestType;
    result.object_of_interest_known=source.objectOfInterestKnown&&source.objectOfInterestSerial==impl_->updateSerial;return result;
}
bool CombatSession::set_source_target(ActorId owner,ActorId target,bool mode,std::string& error){
    error.clear();if(!impl_||impl_->detached){error="Source target update requires an attached Session";return false;}
    return impl_->set_source_target(owner,target,mode,error);
}
bool CombatSession::sync_source_last_target(ActorId owner,std::string& error){
    error.clear();if(!impl_||impl_->detached){error="Source last-target sync requires an attached Session";return false;}
    return impl_->sync_source_last_target(owner,error);
}
bool CombatSession::publish_source_object_interest(ActorId owner,ActorId target,std::int32_t interaction_type,std::string& error){
    error.clear();if(!impl_||impl_->detached){error="Source OOI publication requires an attached Session";return false;}
    if(interaction_type< -128||interaction_type>127){error="Source OOI interaction type is outside signed-byte range";return false;}
    auto found=impl_->entries.find(owner);if(found==impl_->entries.end()||!impl_->world->find_actor(owner)||
       (target!=invalid_actor_id&&!impl_->world->find_actor(target))){error="Source OOI publication requires same-Session actor identities";return false;}
    auto& projection=found->second.targetProjection;projection.objectOfInterest=target;
    projection.objectOfInterestType=interaction_type;projection.objectOfInterestKnown=true;
    projection.objectOfInterestSerial=impl_->updateSerial;
    if(found->second.sourceCombo){found->second.sourceAttack.object_of_interest=target;
        found->second.sourceAttack.object_of_interest_type=interaction_type;}
    return true;
}
const std::vector<CombatSessionComboBoundary>& CombatSession::combo_boundaries()const noexcept{
    static const std::vector<CombatSessionComboBoundary> empty;return impl_?impl_->comboBoundaries:empty;
}
}


