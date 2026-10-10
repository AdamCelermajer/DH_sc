#include "runtime_audio_host_v1.hpp"
#include "../../../engine-audio/audio_source_command_v40.hpp"
#include "../../../engine-audio/audio_world_producer_v38.hpp"
#if defined(DH_PLATFORM_SDL2)
#include "linux_sdl2_source_session_control_v1.hpp"
#endif
#include <cmath>
#include <exception>
#include <utility>

namespace dh::foundation::audio {

struct RuntimeAudioHostV1::Context {
    RuntimeAudioHostV1* host{};
    AudioFilesystem files;
    RuntimeAudioHostProvidersV1 providers;
    std::shared_ptr<void> source_provider_lease;
    std::uintptr_t manager_identity{},gameplay_identity{};
    bool started{};
    std::string gate_error;
};

namespace {
#if defined(DH_PLATFORM_SDL2)
constexpr const char* native_audio_backend_name="SDL2";
constexpr const char* native_audio_clock_failure="No valid actual published SDL2 device-clock snapshot";
#else
constexpr const char* native_audio_backend_name="WinMM";
constexpr const char* native_audio_clock_failure="No valid actual published WinMM TIME_SAMPLES/QPC snapshot";
#endif

bool exact_asset_read(void* raw,const char* uri,
    std::shared_ptr<const std::vector<std::uint8_t>>& bytes,std::string& error) {
    auto* context=static_cast<RuntimeAudioHostV1::Context*>(raw);
    return context&&AudioFilesystem::read(
        &context->files,uri,bytes,error);
}

bool read_facts(RuntimeAudioHostV1::Context& context,
                RuntimeAudioHostFactsV1& facts,std::string& error) {
    facts={};
    if(!context.providers.snapshot) {
        error="Required caller gameplay/listener snapshot provider";
        return false;
    }
    try {
        return context.providers.snapshot(context.providers.context,facts,error);
    } catch(const std::exception& ex) {
        error=std::string("Gameplay/listener snapshot provider threw: ")+ex.what();
    } catch(...) {
        error="Gameplay/listener snapshot provider threw";
    }
    return false;
}
}

RuntimeAudioHostV1::RuntimeAudioHostV1(RuntimeAudioHostConfigV1 config) {
    context_=std::make_shared<Context>();
    context_->host=this;
    context_->files.root=std::move(config.exact_asset_root);
    context_->providers=config.providers;
    context_->source_provider_lease=std::move(config.provider_lease);
    context_->manager_identity=config.actual_manager_identity;
    context_->gameplay_identity=config.gameplay_context_identity;

    manager_general_=dh2::audio::original_vox_boot_general_v40();
    dh2::audio::original_vox_manager_general_init_v40(manager_general_);
    manager_general_initialized_=true;

    if(!context_->manager_identity||!context_->gameplay_identity||
       context_->files.root.empty()||!context_->source_provider_lease||
       !context_->providers.context||!context_->providers.snapshot) return;

    dh2::audio::AudioGameplaySourcesV40 sources;
    sources.context=context_.get();
    sources.gates={context_.get(),gates};
    sources.exact_assets={context_.get(),exact_asset_read,{}};
    sources.random=context_->providers.source_event_random;
    sources.source_command=source_command;
#if defined(DH_PLATFORM_SDL2)
    const auto session_control=dh2::audio::linux_sdl2_source_session_control_v1();
#else
    const auto session_control=dh2::audio::windows_source_session_control_v1();
#endif
    session_=std::make_unique<dh2::audio::AudioNativeSessionV42>(
        context_->manager_identity,sources,std::shared_ptr<void>(context_),lifecycle_,
        session_control);
}

RuntimeAudioHostV1::~RuntimeAudioHostV1() {
    if(session_) {
        std::string error;
        if(!shutdown(error)) {
            // AudioNativeSession deliberately terminates if destroyed before
            // checked close/join/drain. Leak the retained owner and its lease
            // rather than releasing output or borrowed source receivers.
            (void)session_.release();
        }
    }
}

bool RuntimeAudioHostV1::publish_window_activity(std::uint32_t owner,
    std::uint32_t sequence,bool resumed,bool focused,bool granted,bool destroyed) noexcept {
    return lifecycle_.publish_activity(owner,sequence,resumed,focused,granted,destroyed);
}

bool RuntimeAudioHostV1::start(std::string& error) {
    if(!session_) {error="Required actual manager, gameplay identity, asset root and provider lease";return false;}
    if(!session_->initialize(error))return false;
    context_->started=true;
    error.clear();return true;
}

bool RuntimeAudioHostV1::update(std::string& error) {
    if(!session_) {error=std::string("Required live same-session ")+native_audio_backend_name+" host";return false;}
    auto* runtime=session_->runtime_on_producer();
    if(!runtime||!runtime->source_data_initialized()) {
        error="Required initialized same AudioNativeSessionV42 runtime";return false;
    }
    runtime->pump_receipts();error.clear();return true;
}

bool RuntimeAudioHostV1::device_clock(RetainedFrameAudioClock& out,
                                      std::string& error) const {
    out={};
    if(!session_) {error=std::string("Required live same-session ")+native_audio_backend_name+" host";return false;}
    auto* runtime=session_->runtime_on_producer();
    dh2::audio::AudioDeviceClockV40 snapshot;
    if(!runtime||!runtime->clock().snapshot(snapshot)||!snapshot.ready||
       !snapshot.generation||snapshot.position<0||snapshot.monotonic_ns<=0) {
        error=native_audio_clock_failure;return false;
    }
    out={snapshot.generation,snapshot.position,snapshot.monotonic_ns,snapshot.ready};
    error.clear();return out.valid();
}

bool RuntimeAudioHostV1::submit_actual_play(
    const dh2::character::CombatSoundPlayV1& play,std::int64_t event_qpc,
    std::string& error) {
    if(!session_) {error=std::string("Required live same-session ")+native_audio_backend_name+" host";return false;}
    context_->gate_error.clear();
    if(!session_->submit_actual_play(play,event_qpc,error)) {
        if(!context_->gate_error.empty())error += "; "+context_->gate_error;
        return false;
    }
    error.clear();return true;
}

bool RuntimeAudioHostV1::submit_source_sound(ActorId subject,std::int32_t sound_id,
    const std::array<float,3>& position,std::int64_t event_qpc,std::string& error) {
    if(!session_) {error=std::string("Required live same-session ")+native_audio_backend_name+" host";return false;}
    auto* runtime=session_->runtime_on_producer();
    if(!runtime) {error="Required same AudioNativeSessionV42 producer";return false;}
    const auto request=dh2::audio::audio_world_request_v38(
        runtime->manager(),static_cast<std::uintptr_t>(subject),sound_id,position);
    return submit_actual_play(request,event_qpc,error);
}

bool RuntimeAudioHostV1::set_current_emitter_mix(float gain,float pitch,
                                                  std::string& error) {
    if(session_&&!session_->runtime_on_producer()) {
        error="Required same source audio producer for emitter DSP update";return false;
    }
    if(!std::isfinite(gain)||gain<0||!std::isfinite(pitch)||pitch<=0) {
        error="Required finite actual Vox emitter gain and positive pitch";return false;
    }
    current_gain_=gain;current_pitch_=pitch;error.clear();return true;
}

bool RuntimeAudioHostV1::set_manager_general(
    const dh2::audio::OriginalVoxGeneralV40& general,std::string& error) {
    if(session_&&!session_->runtime_on_producer()) {
        error="Required same source audio producer for manager general update";return false;
    }
    if(!std::isfinite(general.doppler_factor)||
       !std::isfinite(general.speed_over_doppler)||general.speed_over_doppler<=0) {
        error="Required finite current Vox manager general state";return false;
    }
    manager_general_=general;manager_general_initialized_=true;error.clear();return true;
}

bool RuntimeAudioHostV1::take_receipt(dh2::audio::AudioReceiptV34& out) {
    if(!session_)return false;
    auto* runtime=session_->runtime_on_producer();
    return runtime&&runtime->take_receipt(out);
}

std::int32_t RuntimeAudioHostV1::source_ordinal(const char* name) const noexcept {
    if(!session_||!name)return -1;
    auto* runtime=session_->runtime_on_producer();
    return runtime?runtime->bindings().source_id(name):-1;
}

bool RuntimeAudioHostV1::ready() const {
    return session_&&session_->ready_for_current_source();
}

bool RuntimeAudioHostV1::shutdown(std::string& error) {
    if(!session_) {error.clear();return true;}
    if(!session_->shutdown(error))return false;
    session_.reset();context_.reset();error.clear();return true;
}

std::unique_ptr<RuntimeCombatAudioV1> RuntimeAudioHostV1::make_combat_audio(
    CombatSession& combat,const dh2::character::CharacterCombatSoundTablesV2& tables,
    RuntimeCombatAudioServicesV1 services) {
    if(!session_||!session_->runtime_on_producer()||
       !session_->runtime_on_producer()->source_data_initialized())return {};
    return std::make_unique<RuntimeCombatAudioV1>(
        *session_,combat,tables,std::move(services));
}

int RuntimeAudioHostV1::gates(void* raw,
    const dh2::sound::VoxPlay3DRequestV2& request,
    dh2::sound::VoxPlay3DResponseV2& response) {
    auto* context=static_cast<Context*>(raw);
    if(!context) return -1;
    RuntimeAudioHostFactsV1 facts;std::string error;
    if(!read_facts(*context,facts,error)) {context->gate_error=std::move(error);return -1;}
    using Operation=dh2::sound::VoxPlay3DOperationV2;
    switch(request.operation) {
    case Operation::disabled:
        response.value=facts.audio_disabled?1:0;return 0;
    case Operation::current_level:
        response.identity=context->gameplay_identity;
        response.value=static_cast<std::int32_t>(facts.gameplay_active?
            RuntimeAudioHostLevelPhaseV1::gameplay_loaded:
            RuntimeAudioHostLevelPhaseV1::inactive);
        return 0;
    case Operation::online:
        response.value=facts.online?1:0;return 0;
    case Operation::network_muted:
        response.value=facts.network_muted?1:0;return 0;
    case Operation::platform_route:
        // This host is the modern native desktop route, not the source Android/JAVA path.
        response.value=0;return 0;
    case Operation::trace:
        response.value=facts.tracing_sfx?1:0;return 0;
    case Operation::platform_play:
        context->gate_error=std::string(native_audio_backend_name)+" host cannot satisfy original Android platform-play route";
        return -1;
    case Operation::sound_row: case Operation::bank_info:
    case Operation::emit: case Operation::native_play:
        context->gate_error="Source row/bank/emission operations belong to the same V42 runtime";
        return -1;
    }
    context->gate_error="Unrecognized source Vox operation";return -1;
}

bool RuntimeAudioHostV1::source_command(void* raw,
    const dh2::character::CombatSoundPlayV1& play,
    const dh2::audio::AudioSoundV34& sound,const dh2::audio::AudioGroupV34& group,
    dh2::audio::AudioCommandV34& command,std::string& error) {
    auto* context=static_cast<Context*>(raw);
    if(!context||!context->host||!context->started) {
        error="Required started same runtime audio host";return false;
    }
    RuntimeAudioHostFactsV1 facts;
    if(!read_facts(*context,facts,error))return false;
    if(!facts.gameplay_active) {error="Gameplay became inactive before source emitter command";return false;}
    if(!facts.world_epoch||facts.listener_world_epoch!=facts.world_epoch||
       !facts.listener_initialized) {
        error="Required current actual camera/listener snapshot for source world epoch";return false;
    }
    auto& host=*context->host;
    dh2::audio::AudioSourceCommandAuthorityV40 authority;
    authority.manager=context->manager_identity;
    authority.actual_current_level=context->gameplay_identity;
    authority.world_epoch=facts.world_epoch;
    authority.listener_world_epoch=facts.listener_world_epoch;
    authority.soundpack_initialized=true;
    authority.manager_general_initialized=host.manager_general_initialized_;
    authority.listener_initialized=facts.listener_initialized;
    authority.listener=facts.listener;
    authority.general=host.manager_general_;
    authority.actual_gain=host.current_gain_;
    authority.actual_pitch=host.current_pitch_;
    authority.actual_native_initial_state=facts.native_initial_state;
    return dh2::audio::audio_source_command_v40(authority,play,sound,group,command,error);
}

} // namespace dh::foundation::audio
