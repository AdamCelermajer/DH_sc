#pragma once
#include "../integration-v40/focus/audio_lifecycle_gate_v40.hpp"
#include "../audio_gameplay_runtime_v42.hpp"
#include "../integration-v40/focus/audio_native_session_v40.hpp"
#include "audio_session_control_factory_v42.hpp"
#include <condition_variable>
#include <chrono>
#include <memory>
#include <mutex>
#include <thread>
namespace dh2::audio {
// Exactly one actual producer and one dedicated control owner. Provider lease
// owns every borrowed World/GS/asset/RNG/source-command receiver until closure.
class AudioNativeSessionV42 {
    const std::thread::id producer_{std::this_thread::get_id()};
    const std::uintptr_t manager_;
    AudioGameplaySourcesV40 originals_;
    std::shared_ptr<void> provider_lease_;
    AudioSessionControlFactoryV42 control_factory_;
    AudioLifecycleGateV40& gate_;
    std::unique_ptr<AudioGameplayRuntimeV42> runtime_;
    std::thread worker_;
    mutable std::mutex mutex_;
    std::condition_variable changed_;
    AudioSessionControlOwnerV40* live_control_{};
    std::atomic<bool> close_completed_{false},control_joined_{false},accepting_{false};
    bool close_requested_{},close_result_known_{},startup_known_{},startup_ok_{},claimed_{},in_submit_{},initialization_attempted_{};
    std::atomic<std::uint32_t> epoch_{0};
    std::string worker_error_;
    using Factory=std::unique_ptr<AudioSessionControlOwnerV40>(*)(void*,AudioMixerV34&,AudioClockV40&,AudioLifecycleGateV40&);
#ifdef DH2_AUDIO_NATIVE_SESSION_FIXTURE
    Factory factory_{};void* factory_context_{};
#endif
    bool producer(std::string&)const;
    void control_thread();
    static bool output_ready(void*,std::string&);
    static bool source_command(void*,const dh2::character::CombatSoundPlayV1&,const AudioSoundV34&,const AudioGroupV34&,AudioCommandV34&,std::string&);
public:
    AudioNativeSessionV42(std::uintptr_t actual_manager,AudioGameplaySourcesV40,
                         std::shared_ptr<void> actual_provider_lease,AudioLifecycleGateV40&);
    AudioNativeSessionV42(std::uintptr_t actual_manager,AudioGameplaySourcesV40,
                         std::shared_ptr<void> actual_provider_lease,AudioLifecycleGateV40&,
                         AudioSessionControlFactoryV42);
#ifdef DH2_AUDIO_NATIVE_SESSION_FIXTURE
    // Explicit borrowed test driver only; absent from production public API.
    AudioNativeSessionV42(std::uintptr_t,AudioGameplaySourcesV40,std::shared_ptr<void>,AudioLifecycleGateV40&,Factory,void*);
#endif
    ~AudioNativeSessionV42();
    AudioNativeSessionV42(const AudioNativeSessionV42&)=delete;
    AudioNativeSessionV42& operator=(const AudioNativeSessionV42&)=delete;
    bool initialize(std::string&,std::chrono::milliseconds timeout=std::chrono::milliseconds(2000));
    bool submit_actual_play(const dh2::character::CombatSoundPlayV1&,std::int64_t actual_event_monotonic_ns,std::string&);
    bool ready_for_current_source()const;
    std::string control_error()const;
    // Thread-safe UI/native lifecycle request; no GL task or mixer command.
    void request_output_close();
    // Producer only: closes independently, joins, proves final drain, releases.
    // Failure retains the entire session, thread and provider lease. No retry.
    bool shutdown(std::string&,std::chrono::milliseconds timeout=std::chrono::milliseconds(2000));
    // Root diagnostics/receipt/preload/state scope only. Positive event
    // producers must bind submit_actual_play, never this borrowed runtime.
    AudioGameplayRuntimeV42* runtime_on_producer()noexcept;
};
}
