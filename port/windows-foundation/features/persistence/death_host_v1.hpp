#pragma once

// I024 death host: the offline local-death presentation and revive orchestration.
// It owns the admitted DeathFlowTicketV1, the sprite742 death screen runtime and
// the sprite743 fade-from-black runtime. It does not own the window, the
// gameplay Session, the audio host or the menu stack: those are injected as
// callbacks by the host loop (main.cpp), so every consumer stays the real one.

#include "death_flow_v1.hpp"
#include "death_fade_from_black_v1.hpp"
#include "death_screen_runtime_v1.hpp"

#include <array>
#include <cstdint>
#include <string>

namespace dh::foundation::persistence::death_host {

// Source gate inputs. level_ready is Level type 38 in the READY lifecycle state
// (Level+304 == 38 in the export); player_manager_phase_active is the PlayerManager
// phase>0 fact. The port supplies both from the live Level owner; neither is
// inferred from HP.
struct AdmissionV1 {
    bool level_ready = false;
    bool player_manager_phase_active = false;
    bool local_character_assigned = false;
    bool local_character_dead = false;
    bool online = false;
};

// Host providers. Each callback is a real consumer owned by the host loop.
struct ServicesV1 {
    void* context = nullptr;
    // VoxSoundManager::SetSoundVolume(1, ...): muted=true at death, restored at revive.
    bool (*set_fx_muted)(void*, bool muted, std::string&) = nullptr;
    // Frame64 NativeScreenIsBlack. The host only records that the callback ran.
    bool (*native_screen_is_black)(void*, std::string&) = nullptr;
    // Level::LoadCheckpoint (selective world restore, same LevelSavegame/ObjectManager).
    bool (*load_level_checkpoint)(void*, std::string&) = nullptr;
    bool (*clear_character_buffs)(void*, std::string&) = nullptr;
    bool (*restore_revive_position)(void*, std::string&) = nullptr;
    bool (*revive_to_source_vitals)(void*, std::string&) = nullptr;
    bool (*set_character_idle)(void*, std::string&) = nullptr;
    bool (*ensure_minimum_potions)(void*, std::string&) = nullptr;
    // ReviveLocalPlayers' StopAllMusic(2)+PlayMusic(level,1000).
    bool (*restart_level_music)(void*, std::string&) = nullptr;
    // Frame88 NativePlayPreviousMusic (RestartMusic(0)), a distinct callback.
    bool (*native_play_previous_music)(void*, std::string&) = nullptr;
    // Frame32 of sprite743: native PopMenu of menu_FadeFromBlackScreen.
    bool (*pop_fade_from_black)(void*, std::string&) = nullptr;
};

enum class PhaseV1 : std::uint8_t { idle, death_screen, revive_pending, fade_from_black, done };

class DeathHostV1 {
    death_flow::DeathFlowTicketV1 flow_{};
    death_flow::DeathScreenRuntimeV1 screen_{};
    death_flow::OriginalFadeFromBlackRuntimeV1 fade_{};
    PhaseV1 phase_ = PhaseV1::idle;
    bool level_ready_ = false;
    bool fx_muted_ = false;
    bool screen_black_seen_ = false;
    bool revive_ran_ = false;
    bool failed_ = false;
    double accumulator_seconds_ = 0.0;
    std::string failure_;
    const ServicesV1* services_ = nullptr; // bound for the duration of one call only

public:
    // Source-gated start. Returns true when a new death flow was admitted on this
    // call; false with error set when the source gate rejects the observation.
    // A dead Character with an unready Level is not an error: the source does
    // nothing in that case, so error is empty and the result is false.
    bool observe(const AdmissionV1& admission, std::uint8_t quote_number,
                 const ServicesV1& services, std::string& error);

    // Advances the UI clock by real seconds (not gameplay seconds): each 1/30 s
    // authored tick runs one frame of the death screen or the fade.
    bool advance(double seconds, bool level_ready, const ServicesV1& services,
                 std::string& error);

    // Pointer release in window pixels. Returns true when Continue fired.
    // Any release while the modal is open is absorbed (the caller must not forward it).
    bool pointer_release(float window_x, float window_y, int window_width,
                         int window_height, const ServicesV1& services,
                         std::string& error);

    // PC adaptation: Space on its press edge is the fullscreen release.
    bool continue_key(const ServicesV1& services, std::string& error);

    // Modal capture: input and gameplay are absorbed while true.
    bool modal() const noexcept {
        return phase_ == PhaseV1::death_screen || phase_ == PhaseV1::revive_pending ||
               phase_ == PhaseV1::fade_from_black;
    }
    bool death_screen_visible() const noexcept { return phase_ == PhaseV1::death_screen; }
    bool fade_visible() const noexcept { return phase_ == PhaseV1::fade_from_black; }
    bool fx_muted() const noexcept { return fx_muted_; }
    bool failed() const noexcept { return failed_; }
    const std::string& failure() const noexcept { return failure_; }
    PhaseV1 phase() const noexcept { return phase_; }
    bool screen_black_seen() const noexcept { return screen_black_seen_; }

    // The frame the host currently shows. The caller draws it with the source renderers:
    // death_screen -> draw_original_death_screen_v1, fade_from_black -> draw_original_fade_from_black_v1.
    // Returns false (with empty error) when nothing is visible.
    bool visible_screen(death_flow::DeathScreenFrameV1& screen) const;
    bool visible_fade(death_flow::FadeFromBlackFrameV1& fade) const;

private:
    friend struct DeathHostBridge;
    bool run_ticks(double seconds, bool level_ready, const ServicesV1& services, std::string& error);
    bool tick_screen(bool level_ready, const ServicesV1& services, std::string& error);
    bool tick_fade(const ServicesV1& services, std::string& error);
    bool fail(const std::string& message, std::string& error);
};

} // namespace dh::foundation::persistence::death_host
