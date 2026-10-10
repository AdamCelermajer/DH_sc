#pragma once

#include "runtime_combat_audio_v1.hpp"
#include "feature_audio.hpp"
#include "windows_source_session_control_v1.hpp"
#include "../../../engine-audio/integration-v42/audio_native_session_v42.hpp"
#include "../../../engine-audio/integration-v40/focus/audio_lifecycle_gate_v40.hpp"
#include "../../../engine-audio/audio_source_command_v40.hpp"
#include <memory>

namespace dh::foundation::audio {

// Typed compatibility for the pure Play3D gate. The original compares the
// current Level phase at +0x130 with 38 decimal; frame-order evidence names
// the loaded gameplay phase 0x26. This adapter accepts modern gameplay-active
// state and does not claim a canonical native Level/GS pointer.
enum class RuntimeAudioHostLevelPhaseV1 : std::int32_t {
    inactive=0,
    gameplay_loaded=0x26
};

struct RuntimeAudioHostFactsV1 {
    bool audio_disabled{};
    bool gameplay_active{};
    bool online{};
    bool network_muted{};
    bool tracing_sfx{};

    // Caller-owned current camera/listener snapshot. It must belong to the
    // current source world epoch; the host does not derive it from a camera.
    bool listener_initialized{};
    std::uint64_t world_epoch{};
    std::uint64_t listener_world_epoch{};
    dh2::audio::VoxListenerAuthorityV38 listener{};
    std::int32_t native_initial_state{-1};
};

struct RuntimeAudioHostProvidersV1 {
    void* context{};
    bool (*snapshot)(void*,RuntimeAudioHostFactsV1&,std::string&){};
    // This RNG is only source SoundAutoGen event selection. It is caller-owned
    // (normally the same source random stream), never constructed by the host.
    dh2::audio::AudioRandomV34 source_event_random{};
};

struct RuntimeAudioHostConfigV1 {
    std::uintptr_t actual_manager_identity{};
    // Stable identity for the retained generic gameplay/provider context. This
    // is never advertised as an original native GS/Level object address.
    std::uintptr_t gameplay_context_identity{};
    std::string exact_asset_root;
    std::shared_ptr<void> provider_lease;
    RuntimeAudioHostProvidersV1 providers;
};

// One WinMM control, one AudioNativeSessionV42 and one recovered runtime.
// The caller publishes actual window state and supplies a current listener
// snapshot; source gameplay RNG remains outside this host.
class RuntimeAudioHostV1 {
public:
    struct Context;
private:
    std::shared_ptr<Context> context_;
    dh2::audio::AudioLifecycleGateV40 lifecycle_;
    std::unique_ptr<dh2::audio::AudioNativeSessionV42> session_;
    dh2::audio::OriginalVoxGeneralV40 manager_general_;
    float current_gain_{dh2::audio::original_fresh_emitter_gain_v40()};
    float current_pitch_{dh2::audio::original_fresh_emitter_pitch_v40()};
    bool manager_general_initialized_{};

    static int gates(void*,const dh2::sound::VoxPlay3DRequestV2&,
                     dh2::sound::VoxPlay3DResponseV2&);
    static bool source_command(void*,const dh2::character::CombatSoundPlayV1&,
        const dh2::audio::AudioSoundV34&,const dh2::audio::AudioGroupV34&,
        dh2::audio::AudioCommandV34&,std::string&);
public:
    explicit RuntimeAudioHostV1(RuntimeAudioHostConfigV1);
    ~RuntimeAudioHostV1();
    RuntimeAudioHostV1(const RuntimeAudioHostV1&)=delete;
    RuntimeAudioHostV1& operator=(const RuntimeAudioHostV1&)=delete;

    bool publish_window_activity(std::uint32_t owner,std::uint32_t sequence,
        bool resumed,bool window_focused,bool focus_granted,bool destroyed) noexcept;
    bool start(std::string& error);
    bool update(std::string& error);
    bool device_clock(RetainedFrameAudioClock&,std::string& error) const;
    bool submit_actual_play(const dh2::character::CombatSoundPlayV1&,
        std::int64_t event_qpc_ns,std::string& error);
    // Source-authored AnimTable/ItemTable sound ID through this exact manager,
    // provider gate and AudioNativeSessionV42 producer.
    bool submit_source_sound(ActorId subject,std::int32_t sound_id,
        const std::array<float,3>& position,std::int64_t event_qpc_ns,
        std::string& error);
    // Producer-thread source settings, initialized from the exact fresh
    // emitter defaults and updated only when the current source DSP changes.
    bool set_current_emitter_mix(float gain,float pitch,std::string& error);
    bool set_manager_general(const dh2::audio::OriginalVoxGeneralV40&,
        std::string& error);
    bool take_receipt(dh2::audio::AudioReceiptV34&);
    std::int32_t source_ordinal(const char* authored_name) const noexcept;
    bool ready() const;
    bool shutdown(std::string& error);

    // Caller retains/installs the returned adapter's observer on the same
    // CombatSession and supplies its actual gameplay RNG services.
    std::unique_ptr<RuntimeCombatAudioV1> make_combat_audio(
        CombatSession&,const dh2::character::CharacterCombatSoundTablesV2&,
        RuntimeCombatAudioServicesV1);
};

} // namespace dh::foundation::audio
