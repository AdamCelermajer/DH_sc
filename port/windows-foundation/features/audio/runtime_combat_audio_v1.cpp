#include "runtime_combat_audio_v1.hpp"
#include "../../retained_animation_owner.hpp"
#include "../../../engine-audio/audio_named_animation_sound_v38.hpp"
#include "../../../level-world/character_combat_sound_v1.hpp"
#include <algorithm>
#include <climits>
#include <cstring>
#include <exception>

namespace dh::foundation::audio {
namespace {
RuntimeCombatAudioStatusV1 classify_reached_play_failure(const std::string& error) {
    // These are failures after an initialized source operation has been
    // reached. The recovered native callers ignore Play's return; retain the
    // evidence without turning it into a combat/session failure.
    if(error.rfind("Unavailable original audio asset: ",0)==0||
       error=="Audio voice observation capacity; drain producer receipts"||
       error=="Audio source command queue full"||
       error=="Source audio token exhaustion")
        return RuntimeCombatAudioStatusV1::playback_diagnostic;
    return RuntimeCombatAudioStatusV1::required_owner_unavailable;
}

bool frame_event_time(const RetainedAnimationEvent& event,
                      const RetainedFrameAudioClock& clock,
                      std::int64_t& out,std::string& error) {
    const auto lag=std::int64_t(event.lag_ms)*1000000LL;
    const auto frame=clock.qpc_monotonic_ns;
    if(!clock.valid()||frame<=0||(lag>=0&&frame<=lag)||
       (lag<0&&frame>INT64_MAX+lag)) {
        error="Required valid caller-paired source output QPC and marker lag";
        return false;
    }
    out=frame-lag;error.clear();return true;
}
}

bool runtime_combat_audio_char_sound_id_v1(const CombatSession& combat,ActorId actor,
    std::int32_t& out,std::string& error) {
    const auto* world=combat.world();
    const auto* properties=world?world->combat_properties(actor):nullptr;
    if(!properties) {
        error="Required same-session original combat properties for CharSounds field 8";
        return false;
    }
    out=properties->sheets.resolved[8];
    error.clear();
    return true;
}

RuntimeCombatAudioV1::RuntimeCombatAudioV1(dh2::audio::AudioNativeSessionV42& audio,
    CombatSession& combat,const dh2::character::CharacterCombatSoundTablesV2& tables,
    RuntimeCombatAudioServicesV1 services)
    :audio_(audio),combat_(combat),tables_(tables),services_(std::move(services)),
     actor_lease_(combat.actor_binding_lease()) {}

bool RuntimeCombatAudioV1::current_owners(std::string& error)const {
    if(actor_lease_.expired()) {error="Required current CombatSession actor lease";return false;}
    auto* runtime=audio_.runtime_on_producer();
    if(!runtime||!runtime->source_data_initialized()) {
        error="Required initialized same AudioNativeSessionV42 producer/runtime";return false;
    }
    if(!audio_.ready_for_current_source()) {
        error="Required focused same AudioNativeSessionV42 output epoch";return false;
    }
    error.clear();return true;
}

void RuntimeCombatAudioV1::report(const RuntimeCombatAudioDiagnosticV1& diagnostic)const {
    if(services_.diagnostic) {
        try {services_.diagnostic(diagnostic);} catch(...) { /* diagnostics never veto source callbacks */ }
    }
}

RetainedFrameAudioObserver RuntimeCombatAudioV1::retained_event_observer() {
    return [this](ActorId actor,const RetainedAnimationEvent& event,std::uint32_t ordinal,
                  const RetainedFrameAudioClock* clock,std::string& detail) {
        RuntimeCombatAudioDiagnosticV1 diagnostic;
        diagnostic.actor=actor;diagnostic.event_index=ordinal;diagnostic.event_name=event.name;
        auto finish=[&](RuntimeCombatAudioStatusV1 status,std::string message={}) {
            diagnostic.status=status;diagnostic.detail=std::move(message);report(diagnostic);
            detail=diagnostic.detail;
            switch(status) {
            case RuntimeCombatAudioStatusV1::dispatched: return RetainedFrameAudioObserverStatus::dispatched;
            case RuntimeCombatAudioStatusV1::unavailable_clock: return RetainedFrameAudioObserverStatus::unavailable_clock;
            case RuntimeCombatAudioStatusV1::required_owner_unavailable: return RetainedFrameAudioObserverStatus::required_owner_unavailable;
            case RuntimeCombatAudioStatusV1::playback_diagnostic: return RetainedFrameAudioObserverStatus::playback_diagnostic;
            case RuntimeCombatAudioStatusV1::not_applicable:
            case RuntimeCombatAudioStatusV1::source_name_miss: return RetainedFrameAudioObserverStatus::not_applicable;
            }
            return RetainedFrameAudioObserverStatus::not_applicable;
        };
        if(!clock||!clock->valid())
            return finish(RuntimeCombatAudioStatusV1::unavailable_clock,
                          "No valid caller-supplied WinMM TIME_SAMPLES/QPC pair for retained event");
        const auto lease=combat_.actor_binding_lease().lock();
        if(!lease)return finish(RuntimeCombatAudioStatusV1::required_owner_unavailable,
                                "Required retained CombatSession binding lease");
        const OccurrenceKey occurrence{reinterpret_cast<std::uintptr_t>(lease.get()),
            event.generation,actor,event.clip_id,event.slot,event.wall_timestamp_ms,
            event.lag_ms,event.name,ordinal};
        if(delivered_occurrences_.count(occurrence))
            return finish(RuntimeCombatAudioStatusV1::not_applicable,
                          "Duplicate exact retained occurrence suppressed");
        delivered_occurrences_.insert(occurrence);delivered_order_.push_back(occurrence);
        if(delivered_order_.size()>4096){delivered_occurrences_.erase(delivered_order_.front());delivered_order_.pop_front();}
        const auto& damage_events=combat_.events();
        const auto* live_world=combat_.world();
        if(!live_world)return finish(RuntimeCombatAudioStatusV1::required_owner_unavailable,
                                     "Required same-session pending combat resolution owner");
        const auto& pending=live_world->pending_resolutions();
        const auto update_serial=combat_.update_serial();
        if(update_serial==0)return finish(RuntimeCombatAudioStatusV1::required_owner_unavailable,
                                          "Required live CombatSession update serial");
        const UpdateKey update_key{reinterpret_cast<std::uintptr_t>(lease.get()),update_serial};
        if(!has_update_||update_key!=current_update_){
            current_update_=update_key;has_update_=true;
            consumed_resolution_indices_.clear();consumed_damage_indices_.clear();
        }
        std::size_t resolution_index=pending.size(),damage_index=damage_events.size();
        for(std::size_t i=0;i<pending.size();++i){
            const auto& resolution=pending[i];
            if(consumed_resolution_indices_.count(i)||resolution.attacker!=actor||
               resolution.marker_name!=event.name)continue;
            for(std::size_t j=0;j<damage_events.size();++j){
                const auto& damage=damage_events[j];
                if(consumed_damage_indices_.count(j)||!damage.applied||
                   damage.attacker!=resolution.attacker||damage.target!=resolution.victim||
                   damage.marker_name!=resolution.marker_name)continue;
                resolution_index=i;damage_index=j;break;
            }
            if(resolution_index<pending.size())break;
        }
        if(resolution_index<pending.size()){
            // Copy every hit fact while the world queue is valid. The exact
            // target transform is captured before the retained motion callback.
            const auto resolution=pending[resolution_index];
            const auto damage=damage_events[damage_index];
            const auto* target_state=combat_.actor(resolution.victim);
            if(!target_state)return finish(RuntimeCombatAudioStatusV1::required_owner_unavailable,
                "Required same-session hit target before retained motion");
            const std::array<float,3> target_position=target_state->transform.position;
            consumed_resolution_indices_.insert(resolution_index);
            consumed_damage_indices_.insert(damage_index);
            const auto hit=dispatch_hit(resolution,damage,event,ordinal,target_position,*clock);
            diagnostic.target=hit.target;diagnostic.source_id=hit.source_id;
            return finish(hit.status,std::move(hit.detail));
        }
        if(event.name.rfind("sfx_",0)!=0)return finish(RuntimeCombatAudioStatusV1::not_applicable);
        std::string error;
        if(!current_owners(error))
            return finish(RuntimeCombatAudioStatusV1::required_owner_unavailable,std::move(error));
        const auto actual=static_cast<std::uintptr_t>(actor);
        if(!actual)return finish(RuntimeCombatAudioStatusV1::required_owner_unavailable,
                                 "Required stable nonzero retained actor identity");
        std::int64_t event_ns{};
        if(!frame_event_time(event,*clock,event_ns,error))
            return finish(RuntimeCombatAudioStatusV1::unavailable_clock,std::move(error));
        auto* runtime=audio_.runtime_on_producer();
        if(!runtime) return finish(RuntimeCombatAudioStatusV1::required_owner_unavailable,
                                   "Required same audio producer thread");
        struct Context {RuntimeCombatAudioV1* self;ActorId actor;std::uintptr_t actual;std::int64_t event_ns;std::string* error;};
        Context context{this,actor,actual,event_ns,&error};
        dh2::audio::AudioNamedAnimationSoundServicesV38 source;
    source.context=&context;source.actual_character=actual;
        source.manager=[](void* raw,std::uintptr_t& identity) {
            auto& c=*static_cast<Context*>(raw);std::string e;
            if(!c.self->current_owners(e)){*c.error=std::move(e);return -1;}
            auto* r=c.self->audio_.runtime_on_producer();if(!r){*c.error="Required same source runtime";return -1;}
            identity=r->manager();return identity?0:-1;
        };
        source.target_position=[](void* raw,std::uintptr_t character,std::array<float,3>& position) {
            auto& c=*static_cast<Context*>(raw);
            const auto* actor=c.self->combat_.actor(c.actor);
            if(!actor) {*c.error="Required current retained source actor position";return -1;}
            if(character!=c.actual) {*c.error="Retained marker Character identity changed";return -1;}
            position=actor->transform.position;
            return 0;
        };
        source.play=[](void* raw,const dh2::character::CombatSoundPlayV1& play) {
            auto& c=*static_cast<Context*>(raw);
            if(!c.self->audio_.submit_actual_play(play,c.event_ns,*c.error))return -1;
            return 0;
        };
        dh2::audio::AudioNamedAnimationSoundResultV38 named;
        try {
            const auto status=dh2::audio::audio_named_animation_sound_v38(
                event.name.c_str()+4,runtime->bindings(),source,named);
            diagnostic.source_id=named.source_id;
            if(status) {
                const auto classification=classify_reached_play_failure(error);
                return finish(classification,error.empty()?
                    (named.required?named.required:"Original named sound owner failed"):std::move(error));
            }
            return finish(named.source_id<0?RuntimeCombatAudioStatusV1::source_name_miss:
                          RuntimeCombatAudioStatusV1::dispatched);
        } catch(const std::exception& ex) {
            return finish(RuntimeCombatAudioStatusV1::required_owner_unavailable,ex.what());
        } catch(...) {
            return finish(RuntimeCombatAudioStatusV1::required_owner_unavailable,
                          "Original named sound provider threw");
        }
    };
}

RuntimeCombatAudioDiagnosticV1 RuntimeCombatAudioV1::dispatch_hit(
    const PlayableCombatResolution& resolution,const DamageEvent& event,
    const RetainedAnimationEvent& retained_event,std::uint32_t event_index,
    const std::array<float,3>& target_position,const RetainedFrameAudioClock& clock) {
    RuntimeCombatAudioDiagnosticV1 diagnostic;
    diagnostic.actor=resolution.attacker;diagnostic.target=resolution.victim;
    diagnostic.event_index=event_index;diagnostic.event_name=retained_event.name;
    auto finish=[&](RuntimeCombatAudioStatusV1 status,std::string detail={}) {
        diagnostic.status=status;diagnostic.detail=std::move(detail);return diagnostic;
    };
    if(!clock.valid())return finish(RuntimeCombatAudioStatusV1::unavailable_clock,
        "No valid caller-supplied WinMM TIME_SAMPLES/QPC pair for retained combat resolution");
    std::string error;
    if(!current_owners(error))return finish(RuntimeCombatAudioStatusV1::required_owner_unavailable,std::move(error));
    if(!event.applied||event.attacker!=resolution.attacker||event.target!=resolution.victim||
       event.marker_name!=resolution.marker_name)
        return finish(RuntimeCombatAudioStatusV1::required_owner_unavailable,
            "No matching applied CombatSession damage event for retained source resolution");
    if(!services_.minimal_randoms||!services_.random)
        return finish(RuntimeCombatAudioStatusV1::required_owner_unavailable,
                      "Required source MP_MinimalRandoms/shared-Random providers");
    const auto* attacker_state=combat_.actor(resolution.attacker);
    const auto* target_state=combat_.actor(resolution.victim);
    if(!attacker_state||!target_state)
        return finish(RuntimeCombatAudioStatusV1::required_owner_unavailable,
                      "Required current same-session hit actors");
    const auto attacker=static_cast<std::uintptr_t>(resolution.attacker);
    const auto target=static_cast<std::uintptr_t>(resolution.victim);
    std::int32_t attacker_sound_id{},target_sound_id{};
    try {
        const bool have_attacker=services_.cached_char_sound_id?
            services_.cached_char_sound_id(resolution.attacker,attacker_sound_id,error):
            runtime_combat_audio_char_sound_id_v1(combat_,resolution.attacker,attacker_sound_id,error);
        if(!have_attacker)
            return finish(RuntimeCombatAudioStatusV1::required_owner_unavailable,
                error.empty()?"Required source attacker CharSounds ID":std::move(error));
        const bool have_target=services_.cached_char_sound_id?
            services_.cached_char_sound_id(resolution.victim,target_sound_id,error):
            runtime_combat_audio_char_sound_id_v1(combat_,resolution.victim,target_sound_id,error);
        if(!have_target)
            return finish(RuntimeCombatAudioStatusV1::required_owner_unavailable,
                error.empty()?"Required source target CharSounds ID":std::move(error));
    } catch(const std::exception& ex) {
        return finish(RuntimeCombatAudioStatusV1::required_owner_unavailable,ex.what());
    } catch(...) {
        return finish(RuntimeCombatAudioStatusV1::required_owner_unavailable,
                      "Original CharSounds row provider threw");
    }
    if(!attacker||!target)
        return finish(RuntimeCombatAudioStatusV1::required_owner_unavailable,
                      "Required stable nonzero hit ActorId identities");
    std::int64_t event_ns{};
    if(!frame_event_time(retained_event,clock,event_ns,error))
        return finish(RuntimeCombatAudioStatusV1::unavailable_clock,std::move(error));

    struct Context {
        RuntimeCombatAudioV1* self;ActorId attacker_id,target_id;
        std::uintptr_t attacker,target;std::int32_t attacker_sound_id,target_sound_id;bool target_died;
        std::array<float,3> target_position;
        std::int32_t source_id=-1;
        std::int64_t event_ns;RuntimeCombatAudioStatusV1 play_status=RuntimeCombatAudioStatusV1::dispatched;
        std::string* error;
    } context{this,resolution.attacker,resolution.victim,attacker,target,
              attacker_sound_id,target_sound_id,event.target_died,target_position,-1,event_ns,
              RuntimeCombatAudioStatusV1::dispatched,&error};
    dh2::character::CombatSoundServicesV1 source;
    source.context=&context;
    source.row=[](void* raw,std::uintptr_t identity,const dh2::character::CombatSoundRowV1** row) {
        auto& c=*static_cast<Context*>(raw);ActorId id=invalid_actor_id;std::int32_t sound_id=-1;
        if(identity==c.attacker){id=c.attacker_id;sound_id=c.attacker_sound_id;}
        else if(identity==c.target){id=c.target_id;sound_id=c.target_sound_id;}
        else {*c.error="CombatSound row requested for an unrelated Character identity";return -1;}
        if(!c.self->combat_.actor(id)||!(*row=c.self->tables_.get(sound_id))) {
            *c.error="Required current CharSounds table row";return -1;
        }
        return 0;
    };
    source.minimal_randoms=[](void* raw,std::uint32_t* value) {
        auto& c=*static_cast<Context*>(raw);
        if(!c.self->services_.minimal_randoms||!c.self->services_.minimal_randoms(*value,*c.error))return -1;
        return 0;
    };
    source.dead=[](void* raw,std::uintptr_t identity,bool* dead) {
        auto& c=*static_cast<Context*>(raw);
        if(identity!=c.target){*c.error="CombatSound dead query target identity mismatch";return -1;}
        if(!c.self->combat_.actor(c.target_id)){*c.error="Required current same-session hit target";return -1;}
        // This is the exact target state after this individual hit's HitFor,
        // before any later same-frame hits can change it.
        *dead=c.target_died;return 0;
    };
    source.manager=[](void* raw,std::uintptr_t* manager) {
        auto& c=*static_cast<Context*>(raw);std::string detail;
        if(!c.self->current_owners(detail)) {*c.error=std::move(detail);return -1;}
        auto* runtime=c.self->audio_.runtime_on_producer();
        if(!runtime){*c.error="Required same producer audio runtime";return -1;}
        *manager=runtime->manager();return *manager?0:-1;
    };
    source.random=[](void* raw,std::uint32_t count,std::uint32_t* index) {
        auto& c=*static_cast<Context*>(raw);
        if(!c.self->services_.random||!c.self->services_.random(count,*index,*c.error))return -1;
        if(*index>=count){*c.error="Original CharSounds Random returned an out-of-range index";return -1;}
        return 0;
    };
    source.position=[](void* raw,std::uintptr_t identity,std::array<float,3>* position) {
        auto& c=*static_cast<Context*>(raw);
        if(identity!=c.target){*c.error="CombatSound position target identity mismatch";return -1;}
        const auto* actor=c.self->combat_.actor(c.target_id);
        if(!actor){*c.error="Required current same-session hit target position";return -1;}
        *position=c.target_position;
        return 0;
    };
    source.play=[](void* raw,const dh2::character::CombatSoundPlayV1* play) {
        auto& c=*static_cast<Context*>(raw);
        if(!play||play->manager==0){*c.error="Required same source Play manager/request";return -1;}
        c.source_id=play->sound_id;
        if(!c.self->audio_.submit_actual_play(*play,c.event_ns,*c.error)) {
            c.play_status=classify_reached_play_failure(*c.error);return -1;
        }
        return 0;
    };
    try {
        dh2::character::CombatSoundOutputV1 output;
        const auto result=dh2::character::character_combat_sound_v1(
            &output,&resolution.melee.original,attacker,target,true,&source);
        diagnostic.source_id=context.source_id;
        if(result) {
            if(context.play_status==RuntimeCombatAudioStatusV1::playback_diagnostic)
                return finish(context.play_status,error.empty()?"Original ignored hit Play failed":std::move(error));
            return finish(RuntimeCombatAudioStatusV1::required_owner_unavailable,
                error.empty()?"Required original CharSounds hit operation owner":std::move(error));
        }
        return finish(output.played?RuntimeCombatAudioStatusV1::dispatched:
                      RuntimeCombatAudioStatusV1::not_applicable);
    } catch(const std::exception& ex) {
        return finish(RuntimeCombatAudioStatusV1::required_owner_unavailable,ex.what());
    } catch(...) {
        return finish(RuntimeCombatAudioStatusV1::required_owner_unavailable,
                      "Original CharSounds provider threw");
    }
}

} // namespace dh::foundation::audio
