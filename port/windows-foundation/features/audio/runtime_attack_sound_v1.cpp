#include "runtime_attack_sound_v1.hpp"
#include "audio_source_target_position_v1.hpp"
#include "../../../engine-audio/audio_animation_swoosh_v38.hpp"
#include "../../../game-data/items.hpp"
#include "../../playable_actor_world.hpp"
#include <exception>

namespace dh::foundation::audio {
namespace {
struct SwooshContext {
    const RuntimeAttackSoundSubmitV1* submit{};
    ActorId actor=invalid_actor_id;
    const dh2::data::ItemRecord164* main_item{};
    const dh2::data::ItemRecord164* off_item{};
    std::array<float,3> position{};
    std::int64_t qpc_ns{};
    std::string* playback_error{};
    bool playback_failed{};
};

int equipped_item(void* raw,std::int32_t kind,std::uintptr_t& result) {
    auto& context=*static_cast<SwooshContext*>(raw);
    const auto* item=kind==1?context.main_item:kind==2?context.off_item:nullptr;
    result=reinterpret_cast<std::uintptr_t>(item);
    return kind==1||kind==2?0:-1;
}

int item_effects(void*,std::uintptr_t raw_item,std::int32_t& sound,std::int32_t& fx) {
    sound=fx=-1;
    const auto* item=reinterpret_cast<const dh2::data::ItemRecord164*>(raw_item);
    if(!item)return 0;
    // Item::SwooshSoundFX and Item::SwooshFX are words 5 and 6 in
    // the recovered 164-byte scalar projection.
    sound=item->words[5];fx=item->words[6];return 0;
}

int submit_item_sound(void* raw,std::int32_t sound) {
    auto& context=*static_cast<SwooshContext*>(raw);
    std::string error;
    if(!context.submit||!(*context.submit)(context.actor,sound,context.position,
                                            context.qpc_ns,error)) {
        context.playback_failed=true;
        if(context.playback_error&&context.playback_error->empty())
            *context.playback_error=std::move(error);
    }
    // The recovered Play3D caller ignores the operation result. Preserve the
    // Swoosh fallback branch and report the reached failure diagnostically.
    return 0;
}

int observe_item_fx(void*,std::int32_t,bool) {
    // FX playback remains owned by the animation/effects feature. This audio
    // sidecar consumes the typed ItemRecord fields only to preserve the source
    // fallback-sound decision; it does not claim to render the effect.
    return 0;
}
}

RuntimeAttackSoundV1::RuntimeAttackSoundV1(RuntimeAttackSoundSubmitV1 submit,
    RuntimeAttackSoundReadyV1 ready,
    CombatSession& session,const dh2::data::AnimationTables& animations,
    const dh2::data::ItemTable& items,
    std::function<void(const RuntimeAttackSoundDiagnosticV1&)> diagnostic)
    :submit_(std::move(submit)),ready_(std::move(ready)),session_(session),
     animations_(animations),items_(items),
     diagnostic_(std::move(diagnostic)) {}

void RuntimeAttackSoundV1::report(const RuntimeAttackSoundDiagnosticV1& item) const noexcept {
    if(diagnostic_)try {diagnostic_(item);} catch(...) {}
}

CombatSessionStepObserver RuntimeAttackSoundV1::step_entry_observer() {
    return [this](const CombatSessionStepEntry& event) { dispatch(event); };
}

void RuntimeAttackSoundV1::dispatch(const CombatSessionStepEntry& event) {
    RuntimeAttackSoundDiagnosticV1 diagnostic;
    diagnostic.actor=event.actor;diagnostic.occurrence=event.occurrence;
    diagnostic.update_serial=event.update_serial;diagnostic.sequence_id=event.sequence_id;
    diagnostic.step=event.step;diagnostic.role=event.role;
    auto finish=[&](RuntimeAttackSoundStatusV1 status,std::string detail={}) {
        diagnostic.status=status;diagnostic.detail=std::move(detail);report(diagnostic);
    };

    const auto lease=event.binding_lease.lock();
    const auto current_lease=session_.actor_binding_lease().lock();
    if(!lease||!current_lease||lease.get()!=current_lease.get()||
       !event.actor||!event.occurrence||!event.update_serial) {
        finish(RuntimeAttackSoundStatusV1::required_owner_unavailable,
               "Required exact retained CombatSession actor lease/occurrence");return;
    }
    const OccurrenceKey key{reinterpret_cast<std::uintptr_t>(lease.get()),event.actor,event.occurrence};
    if(!delivered_.insert(key).second) {
        finish(RuntimeAttackSoundStatusV1::not_applicable,
               "Duplicate exact retained step occurrence suppressed");return;
    }
    if(!event.audio_clock||!event.audio_clock->valid()) {
        finish(RuntimeAttackSoundStatusV1::unavailable_clock,
               "No valid caller-paired WinMM TIME_SAMPLES/QPC sample for AnimTable step entry");return;
    }
    if(!ready_||!ready_()) {
        finish(RuntimeAttackSoundStatusV1::required_owner_unavailable,
               "Required focused same RuntimeAudioHost/V42 output epoch");return;
    }
    if(event.sequence_id<0||std::uint64_t(event.sequence_id)>=animations_.sequences.size()) {
        finish(RuntimeAttackSoundStatusV1::required_owner_unavailable,
               "Retained AnimTable sequence ID is outside actual AnimationTables");return;
    }
    const auto& sequence=animations_.sequences[std::size_t(event.sequence_id)];
    if(event.step>=sequence.steps.size()) {
        finish(RuntimeAttackSoundStatusV1::required_owner_unavailable,
               "Retained AnimTable step index is outside its actual sequence");return;
    }
    const auto& step=sequence.steps[event.step];diagnostic.sound_id=step.sound;
    const auto* actor=session_.actor(event.actor);
    auto* world=session_.world();
    const auto* traits=world?world->traits(event.actor):nullptr;
    if(!actor||!world||!traits) {
        finish(RuntimeAttackSoundStatusV1::required_owner_unavailable,
               "Required same-session actor state and equipment traits");return;
    }
    const auto position=audio_source_target_position_v1(*actor);
    const auto& clock=*event.audio_clock;
    std::string playback_error;
    bool playback_failed=false;
    if(step.swoosh) {
        const dh2::data::ItemRecord164* off_item=nullptr;
        for(const auto& equipped:actor->equipment) {
            if(equipped.slot!="off_hand")continue;
            if(off_item) {
                finish(RuntimeAttackSoundStatusV1::required_owner_unavailable,
                       "Ambiguous same-session off-hand ItemTable binding");return;
            }
            const auto row=dh2::data::item_id(items_,equipped.definition_id);
            const auto* item=dh2::data::item(items_,row);
            if(!item) {
                finish(RuntimeAttackSoundStatusV1::required_owner_unavailable,
                       "Same-session off-hand equipment has no original ItemTable record");return;
            }
            off_item=&item->record;
        }
        SwooshContext context{&submit_,event.actor,
            traits->main_item?&*traits->main_item:nullptr,off_item,position,
            clock.qpc_monotonic_ns,&playback_error,false};
        dh2::character::AnimationSwooshServicesV4 services{};
        services.context=&context;services.equipped=equipped_item;
        services.effects=item_effects;services.play_sound=submit_item_sound;
        services.play_fx=observe_item_fx;
        bool fallback_sound=false,fallback_fx=false;std::string gate_error;
        if(dh2::audio::audio_animation_swoosh_v38(step.anchor_fx,services,
                                                   fallback_sound,fallback_fx,gate_error)!=0) {
            finish(RuntimeAttackSoundStatusV1::required_owner_unavailable,
                   std::move(gate_error));return;
        }
        if(!fallback_sound) {
            finish(context.playback_failed?RuntimeAttackSoundStatusV1::playback_diagnostic:
                                           RuntimeAttackSoundStatusV1::dispatched,
                   std::move(playback_error));return;
        }
    }

    // This is the original non-Swoosh step call, or its exact fallback after
    // source equipment selection. The source caller ignores Play3D failure.
    if(!submit_||!submit_(event.actor,step.sound,position,
                          clock.qpc_monotonic_ns,playback_error))
        playback_failed=true;
    finish(playback_failed?RuntimeAttackSoundStatusV1::playback_diagnostic:
                           RuntimeAttackSoundStatusV1::dispatched,
           std::move(playback_error));
}

} // namespace dh::foundation::audio
