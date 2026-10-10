#include "boot_flow_v1.hpp"

#include <algorithm>

namespace dh::foundation::startup {

BootFlowV1::BootFlowV1(BootTiming timing, bool skip_boot) noexcept : timing_(timing) {
    if (skip_boot) {
        phase_ = BootPhase::complete;
    }
}

void BootFlowV1::enter(BootPhase next, double now_seconds) noexcept {
    phase_ = next;
    phase_start_ = now_seconds;
}

void BootFlowV1::update(double now_seconds, bool press) noexcept {
    if (now_seconds < last_now_) now_seconds = last_now_;  // clock never goes backwards
    last_now_ = now_seconds;
    switch (phase_) {
    case BootPhase::logo:
        // Logo is not skippable; it ends after its authored hold.
        if (now_seconds - phase_start_ >= logo_total_seconds()) enter(BootPhase::movie, now_seconds);
        break;
    case BootPhase::movie:
        if (press) enter(BootPhase::title, now_seconds);  // skip button
        break;
    case BootPhase::title:
        if (press) enter(BootPhase::complete, now_seconds);  // touch to continue
        break;
    case BootPhase::complete:
    case BootPhase::quit:
        break;
    }
}

void BootFlowV1::movie_finished(double now_seconds) noexcept {
    if (now_seconds < last_now_) now_seconds = last_now_;
    last_now_ = now_seconds;
    if (phase_ == BootPhase::movie) enter(BootPhase::title, now_seconds);
}

void BootFlowV1::request_quit() noexcept {
    phase_ = BootPhase::quit;
}

float BootFlowV1::logo_alpha(double now_seconds) const noexcept {
    if (phase_ != BootPhase::logo) return 0.0f;
    const double t = std::max(0.0, now_seconds - phase_start_);
    const double fade = std::max(timing_.logo_fade_seconds, 1e-9);
    if (t < fade) return static_cast<float>(t / fade);
    const double fadeOutStart = fade + timing_.logo_hold_seconds;
    if (t < fadeOutStart) return 1.0f;
    const double out = (t - fadeOutStart) / fade;
    return out >= 1.0 ? 0.0f : static_cast<float>(1.0 - out);
}

} // namespace dh::foundation::startup
