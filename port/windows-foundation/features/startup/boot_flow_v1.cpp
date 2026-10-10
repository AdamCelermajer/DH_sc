#include "boot_flow_v1.hpp"

namespace dh::foundation::startup {

BootFlowV1::BootFlowV1(bool skip_boot) noexcept {
    if (skip_boot) phase_ = BootPhase::complete;
}

void BootFlowV1::enter(BootPhase next, double now_seconds) noexcept {
    phase_ = next;
    phase_start_ = now_seconds;
}

void BootFlowV1::update(double now_seconds, bool press) noexcept {
    if (now_seconds < last_now_) now_seconds = last_now_;  // clock never goes backwards
    last_now_ = now_seconds;
    switch (phase_) {
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

} // namespace dh::foundation::startup
