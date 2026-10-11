#include "death_host_v1.hpp"

#include <cmath>

namespace dh::foundation::persistence::death_host {

namespace {
constexpr double kAuthoredTickSeconds = 1.0 / 30.0;
}

// Routes the authored callbacks of the death screen and the revive chain to the
// host's ServicesV1 providers. The context is the bridge itself, so no global state.
struct DeathHostBridge {
    DeathHostV1* host = nullptr;
    const ServicesV1* services = nullptr;

    static bool missing(const char* what, std::string& error) {
        error = std::string(what) + " provider is not wired";
        return false;
    }
    static bool forward(bool (*provider)(void*, std::string&), void* context,
                        const char* what, std::string& error) {
        if (!provider) return missing(what, error);
        if (!provider(context, error)) {
            if (error.empty()) error = std::string(what) + " failed";
            return false;
        }
        error.clear();
        return true;
    }

    static DeathHostBridge& from(void* raw) { return *static_cast<DeathHostBridge*>(raw); }

    static bool screen_black(void* raw, std::string& error) {
        auto& b = from(raw);
        b.host->screen_black_seen_ = true;
        return forward(b.services->native_screen_is_black, b.services->context,
                       "Frame64 NativeScreenIsBlack", error);
    }
    static bool pop_death_screen(void* raw, std::string& error) {
        auto& b = from(raw);
        if (b.host->phase_ != PhaseV1::death_screen)
            return (error = "Death screen pop requires the active death screen", false);
        b.host->phase_ = PhaseV1::revive_pending; // the screen is covered until the fade starts
        error.clear();
        return true;
    }
    static bool push_fade_from_black(void* raw, std::string& error) {
        auto& b = from(raw);
        if (b.host->phase_ != PhaseV1::revive_pending)
            return (error = "Fade-from-black push requires the pending revive", false);
        if (!b.host->fade_.start(error)) return false;
        b.host->phase_ = PhaseV1::fade_from_black;
        error.clear();
        return true;
    }
    static bool load_level_checkpoint(void* raw, std::string& error) {
        auto& b = from(raw);
        if (!forward(b.services->load_level_checkpoint, b.services->context,
                     "Level::LoadCheckpoint", error))
            return false;
        // ReviveLocalPlayers: LoadCheckpoint, then VoxSoundManager::SetSoundVolume(1, VolumeFX).
        if (!forward_fx(b, false, error)) return false;
        return true;
    }
    static bool forward_fx(DeathHostBridge& b, bool muted, std::string& error) {
        if (!b.services->set_fx_muted) return missing("FX volume", error);
        if (!b.services->set_fx_muted(b.services->context, muted, error)) {
            if (error.empty()) error = "FX volume provider failed";
            return false;
        }
        b.host->fx_muted_ = muted;
        error.clear();
        return true;
    }
    static bool clear_buffs(void* raw, std::string& error) {
        auto& b = from(raw);
        return forward(b.services->clear_character_buffs, b.services->context, "Character buff owner", error);
    }
    static bool restore_position(void* raw, std::string& error) {
        auto& b = from(raw);
        return forward(b.services->restore_revive_position, b.services->context, "Source revive-position", error);
    }
    static bool revive_vitals(void* raw, std::string& error) {
        auto& b = from(raw);
        return forward(b.services->revive_to_source_vitals, b.services->context, "Character::Revive", error);
    }
    static bool idle(void* raw, std::string& error) {
        auto& b = from(raw);
        return forward(b.services->set_character_idle, b.services->context, "Character idle state", error);
    }
    static bool potions(void* raw, std::string& error) {
        auto& b = from(raw);
        return forward(b.services->ensure_minimum_potions, b.services->context, "CharacterDesign.MinimumPotionsOnDeath", error);
    }
    static bool level_music(void* raw, std::string& error) {
        auto& b = from(raw);
        return forward(b.services->restart_level_music, b.services->context, "ReviveLocalPlayers level music", error);
    }
    static bool previous_music(void* raw, std::string& error) {
        auto& b = from(raw);
        return forward(b.services->native_play_previous_music, b.services->context,
                       "Frame88 NativePlayPreviousMusic", error);
    }
    static bool goto_hide(void*, const char* label, std::string& error) {
        if (std::string(label) != "Hide")
            return (error = "Continue must invoke parent.gotoAndPlay(\"Hide\")", false);
        error.clear();
        return true;
    }
};

bool DeathHostV1::fail(const std::string& message, std::string& error) {
    failed_ = true;
    failure_ = message;
    error = message;
    // A failed flow must not hold the modal forever: the source flow cannot
    // continue, so the host releases input and reports the failure.
    phase_ = PhaseV1::idle;
    return false;
}

bool DeathHostV1::observe(const AdmissionV1& admission, std::uint8_t quote_number,
                          const ServicesV1& services, std::string& error) {
    error.clear();
    if (failed_) { error = failure_; return false; } // latched: report the original failure once, no re-wrapping
    if (phase_ != PhaseV1::idle) return false; // one flow per death
    if (!admission.local_character_assigned || !admission.local_character_dead) return false;
    if (admission.online)
        return (error = "Online death routes are not wired in the offline death host", false);
    if (!admission.level_ready || !admission.player_manager_phase_active) {
        error = "Death gate: Level READY or PlayerManager phase is not active";
        return false;
    }
    death_flow::DeathAdmissionV1 gate;
    gate.gameplay_level_type_38 = admission.level_ready;
    gate.local_character_assigned = admission.local_character_assigned;
    gate.character_dead = admission.local_character_dead;
    gate.player_manager_phase_active = admission.player_manager_phase_active;
    gate.online = false;
    flow_ = {};
    screen_ = death_flow::DeathScreenRuntimeV1{}; // each death is a fresh authored timeline
    fade_ = death_flow::OriginalFadeFromBlackRuntimeV1{};
    if (!death_flow::begin_death_flow_v1(gate, flow_, error)) return fail(error, error);
    if (!screen_.start(flow_, quote_number, error)) return fail(error, error);
    level_ready_ = admission.level_ready;
    screen_black_seen_ = false;
    accumulator_seconds_ = 0.0;
    // Source _HandleLocalDeaths offline branch: mute FX once, then push the screen once.
    if (!services.set_fx_muted) return fail("FX mute provider is not wired", error);
    if (!services.set_fx_muted(services.context, true, error)) {
        if (error.empty()) error = "FX mute provider failed";
        return fail(error, error);
    }
    fx_muted_ = true;
    phase_ = PhaseV1::death_screen;
    error.clear();
    return true;
}

bool DeathHostV1::tick_screen(bool level_ready, const ServicesV1& services, std::string& error) {
    DeathHostBridge bridge{this, &services};
    death_flow::DeathScreenRuntimeServicesV1 screen_services;
    screen_services.native_screen_is_black = &DeathHostBridge::screen_black;
    screen_services.screen_black_context = &bridge;
    screen_services.revive.pop_fade_to_black = &DeathHostBridge::pop_death_screen;
    screen_services.revive.push_fade_from_black = &DeathHostBridge::push_fade_from_black;
    screen_services.revive.load_level_checkpoint = &DeathHostBridge::load_level_checkpoint;
    screen_services.revive.clear_character_buffs = &DeathHostBridge::clear_buffs;
    screen_services.revive.restore_revive_position = &DeathHostBridge::restore_position;
    screen_services.revive.revive_to_source_vitals = &DeathHostBridge::revive_vitals;
    screen_services.revive.set_character_idle = &DeathHostBridge::idle;
    screen_services.revive.ensure_minimum_potions = &DeathHostBridge::potions;
    screen_services.revive.restart_level_music = &DeathHostBridge::level_music;
    screen_services.revive.context = &bridge;
    screen_services.native_play_previous_music = &DeathHostBridge::previous_music;
    screen_services.previous_music_context = &bridge;
    if (!screen_.advance(level_ready, screen_services, error)) return fail(error, error);
    if (screen_.complete() && phase_ != PhaseV1::fade_from_black)
        return fail("Revive completed without the source fade-from-black push", error);
    error.clear();
    return true;
}

bool DeathHostV1::tick_fade(const ServicesV1& services, std::string& error) {
    death_flow::FadeFromBlackFrameV1 frame;
    if (!fade_.advance(frame, error)) return fail(error, error);
    if (frame.pop_menu_callback) {
        if (!services.pop_fade_from_black)
            return fail("Frame32 NativePopMenu provider is not wired", error);
        std::string popError;
        const bool popped = services.pop_fade_from_black(services.context, popError);
        if (!fade_.confirm_native_pop(popped, error)) {
            return fail(popError.empty() ? error : popError, error);
        }
        phase_ = PhaseV1::idle; // the source screen stack is back to gameplay
    }
    error.clear();
    return true;
}

bool DeathHostV1::run_ticks(double seconds, bool level_ready, const ServicesV1& services, std::string& error) {
    if (!std::isfinite(seconds) || seconds < 0.0) return fail("Death host clock is invalid", error);
    if (phase_ == PhaseV1::idle) {
        accumulator_seconds_ = 0.0;
        error.clear();
        return true;
    }
    accumulator_seconds_ += seconds;
    // Bound the catch-up so a long stall cannot run more authored frames than the timeline has.
    int guard = 0;
    while (accumulator_seconds_ + 1e-9 >= kAuthoredTickSeconds && guard++ < 256) {
        accumulator_seconds_ -= kAuthoredTickSeconds;
        if (phase_ == PhaseV1::death_screen) {
            if (!tick_screen(level_ready, services, error)) return false;
        } else if (phase_ == PhaseV1::fade_from_black) {
            if (!tick_fade(services, error)) return false;
        } else {
            break;
        }
        if (phase_ == PhaseV1::idle) break;
    }
    error.clear();
    return true;
}

bool DeathHostV1::advance(double seconds, bool level_ready, const ServicesV1& services, std::string& error) {
    error.clear();
    if (failed_) { error = failure_; return false; } // latched: report the original failure once, no re-wrapping
    level_ready_ = level_ready;
    return run_ticks(seconds, level_ready, services, error);
}

bool DeathHostV1::pointer_release(float window_x, float window_y, int window_width,
                                  int window_height, const ServicesV1& services,
                                  std::string& error) {
    if (phase_ != PhaseV1::death_screen) {
        error = "Pointer release is absorbed by the death modal outside the death screen";
        return false;
    }
    float stage_x = 0.f, stage_y = 0.f;
    if (!death_flow::original_death_screen_to_stage_v1(window_x, window_y, window_width,
                                                      window_height, stage_x, stage_y)) {
        error = "Pointer release is in the letterbox outside the death stage";
        return false;
    }
    DeathHostBridge bridge{this, &services};
    death_flow::ContinueServicesV1 continue_services;
    continue_services.goto_parent_label = &DeathHostBridge::goto_hide;
    continue_services.context = &bridge;
    if (!screen_.on_release(stage_x, stage_y, continue_services, error)) return false;
    error.clear();
    return true;
}

bool DeathHostV1::continue_key(const ServicesV1& services, std::string& error) {
    if (phase_ != PhaseV1::death_screen) {
        error = "Continue key is absorbed by the death modal outside the death screen";
        return false;
    }
    DeathHostBridge bridge{this, &services};
    death_flow::ContinueServicesV1 continue_services;
    continue_services.goto_parent_label = &DeathHostBridge::goto_hide;
    continue_services.context = &bridge;
    // PC adaptation: the fullscreen release is the whole stage, so Space uses the stage centre.
    if (!screen_.on_release(240.f, 160.f, continue_services, error)) return false;
    error.clear();
    return true;
}

bool DeathHostV1::visible_screen(death_flow::DeathScreenFrameV1& screen) const {
    if (phase_ != PhaseV1::death_screen) return false;
    std::string error;
    return screen_.frame(screen, error);
}

bool DeathHostV1::visible_fade(death_flow::FadeFromBlackFrameV1& fade) const {
    if (phase_ != PhaseV1::fade_from_black) return false;
    std::string error;
    return fade_.frame(fade, error);
}

} // namespace dh::foundation::persistence::death_host
