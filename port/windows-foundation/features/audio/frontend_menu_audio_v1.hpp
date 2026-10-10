#pragma once

#include "../../../engine-audio/integration-v42/audio_native_session_v42.hpp"
#include "../../../engine-audio/audio_bank_v34.hpp"
#include "music_voice_v1.hpp"
#include <memory>
#include <string>

namespace dh::foundation::audio {

enum class FrontendMenuAudioStatusV1 {
    submitted,
    unknown_source_name,
    skipped_without_focus,
    missing_original_asset,
    rejected
};

struct FrontendMenuAudioReceiptV1 {
    FrontendMenuAudioStatusV1 status{FrontendMenuAudioStatusV1::rejected};
    std::int32_t source_ordinal{-1};
    std::int32_t xml_sound_uid{-1};
    std::uint64_t token{};
    std::string detail;
};

enum class FrontendMusicStatusV1 {
    started,
    resumed,
    switched,
    unknown_source_name,
    skipped_without_focus,
    missing_original_asset,
    rejected
};

struct FrontendMusicReceiptV1 {
    FrontendMusicStatusV1 status{FrontendMusicStatusV1::rejected};
    std::int32_t source_ordinal{-1};
    std::int32_t xml_sound_uid{-1};
    std::string detail;
};

// Narrow source-authored frontend action recovered from menu_MainMenu's
// btn_MENU_SINGLE_PLAYER.onRelease. The source calls NativePlaySoundFX
// immediately after its FirstTimeClicked gate and before the slot branch.
struct FrontendAuthoredMenuSoundV1 {
    const char* menu{};
    const char* button{};
    const char* action{};
    const char* sound_name{};
    std::int32_t source_ordinal{-1};
    std::int32_t xml_sound_uid{-1};
};

bool resolve_frontend_authored_menu_sound_v1(const char* menu,const char* button,
    const char* action,FrontendAuthoredMenuSoundV1& sound);
// Samples the platform's real QueryPerformanceCounter-backed monotonic clock.
bool frontend_menu_event_qpc_ns_v1(std::int64_t& actual_event_qpc_ns,
    std::string& error);

// Owns one real WinMM/V42 menu output and its device/QPC clock. It is tied to
// the caller's actual frontend window activity. Call shutdown on the same
// producer thread before handing control to a different audio owner.
class FrontendMenuAudioSessionV1 final {
public:
    struct Context;
private:
    std::shared_ptr<Context> context_;
    std::unique_ptr<dh2::audio::AudioNativeSessionV42> session_;
    std::uint32_t activity_sequence_{};
    bool focused_{},minimized_{},started_{};
    MusicVoiceV1 music_;

    bool publish_activity(bool focused,bool minimized,std::string& error);
public:
    FrontendMenuAudioSessionV1();
    ~FrontendMenuAudioSessionV1();
    FrontendMenuAudioSessionV1(const FrontendMenuAudioSessionV1&)=delete;
    FrontendMenuAudioSessionV1& operator=(const FrontendMenuAudioSessionV1&)=delete;

    bool start(const std::string& exact_asset_root,bool actual_window_focused,
        bool actual_window_minimized,std::string& error);
    bool window_activity(bool actual_window_focused,bool actual_window_minimized,
        std::string& error);
    bool ready() const;
    // This is the complete NativePlaySoundFX -> PlayMenu -> Play path for a
    // source-authored plain menu sound. The name and event QPC time come from
    // the actual authored action; no private clock or ordinal guess is used.
    // Audio failures are returned as a receipt and do not veto menu actions.
    bool play_menu_sound(const char* exact_authored_name,
        std::int64_t actual_event_qpc_ns,FrontendMenuAudioReceiptV1&,
        std::string& error);
    // Production caller path: strict authored menu/button/action lookup and
    // ordinal/UID guard, rather than a blanket response to every mouse click.
    bool play_authored_menu_action(const char* menu,const char* button,
        const char* action,std::int64_t actual_event_qpc_ns,
        FrontendMenuAudioReceiptV1&,std::string& error);
    void pump_receipts();
    bool take_receipt(dh2::audio::AudioReceiptV34&);
    // B064: original NativePlayMusic -> VoxSoundManager::PlayMusic(id,loop,force=0,fade) for a frontend
    // screen's authored track name (e.g. "TitleMusic"). Same voice semantics as the level music
    // (music_voice_v1): the track already playing resumes, another one is faded out first. Non-vetoing:
    // missing sample / no focused output are receipts, not failures.
    bool play_music(const char* track_name,int fade_ms,FrontendMusicReceiptV1&,std::string& error);
    bool stop_music(int fade_ms,std::string& error);
    std::int32_t music_ordinal() const noexcept{return music_.ordinal;}
    // Producer-pump diagnostic: is the owned music voice still alive (looping)?
    bool music_playing(std::string& error);
    bool shutdown(std::string& error);
};

} // namespace dh::foundation::audio
