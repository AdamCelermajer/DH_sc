#pragma once

#include "flow/menu_flow.hpp"
#include "native_host.hpp"
#include "creation/runtime_creation_flow_adapter_v1.hpp"
#include "../../platform_win32.hpp"
#include "../../renderer.hpp"

#include <memory>
#include <functional>
#include <algorithm>
#include <filesystem>
#include <cmath>
#include <limits>
#include <optional>
#include <string>

namespace dh2::world { struct CanonicalCharacterCandidateRecordV60; }

namespace dh::foundation::frontend {

enum class FrontendRuntimeOutcomeV1 {
    running,
    back,
    quit,
    gameplay_handoff_ready,
    generic_gameplay_started,
    source_operation_failed,
    host_failed
};

// A loan of the canonical record, which owns the native Character/Save/Profile
// graph. It deliberately carries no detached CharacterState or Save copy.
struct FrontendProfileLoanV1 {
    std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> record;

    bool valid() const noexcept { return static_cast<bool>(record); }
    bool same_as(const FrontendProfileLoanV1& other) const noexcept {
        return record && other.record && record.get() == other.record.get() &&
               !record.owner_before(other.record) && !other.record.owner_before(record);
    }
};

struct FrontendSourceStartReceiptV1 {
    FrontendProfileLoanV1 selected_profile;
    int selected_slot{-1};
    std::shared_ptr<const void> source_assign_receipt;
    std::shared_ptr<const void> source_start_receipt;
};

struct FrontendRuntimeResultV1 {
    FrontendRuntimeOutcomeV1 outcome{FrontendRuntimeOutcomeV1::running};
    std::string error;
    int selected_slot{-1};
    FrontendProfileLoanV1 selected_profile;
    // Present only when the injected native start owner returns a matching
    // receipt. A profile cache or Gear-ready prefix is not sufficient.
    std::optional<FrontendSourceStartReceiptV1> source_receipt;
    // Separate generic state/save evidence. It never satisfies the strict
    // native FrontendProfileLoan/InitPost gameplay_ready() contract.
    std::optional<creation::FrontendGenericStartReceiptV1> generic_start_receipt;

    bool gameplay_ready() const noexcept;
    bool generic_gameplay_ready() const noexcept {
        return outcome == FrontendRuntimeOutcomeV1::generic_gameplay_started &&
               generic_start_receipt &&
               generic_start_receipt->valid_for(generic_start_receipt->shared_state, selected_slot);
    }
};

struct FrontendRuntimeServicesV1 {
    // The same source-backed callbacks consumed by the authored Navigator.
    flow::Services navigation;
    // Borrow only the profile published by the successful native slot owner.
    std::function<FrontendProfileLoanV1(int)> borrow_selected_profile;
    // Borrow the source start receipt only after the native StartGame owner
    // actually succeeds. The receipt's selected profile must match the loan.
    std::function<std::optional<FrontendSourceStartReceiptV1>(int)>
        borrow_source_start_receipt;
    std::shared_ptr<creation::RuntimeCreationFlowAdapterV1> generic_creation;
};

struct FrontendRunConfigV1 {
    std::filesystem::path asset_root;
    std::filesystem::path capture_path;
    std::filesystem::path capture_directory;
    std::filesystem::path ui_assets;
    int frames{};
    int width{960};
    int height{540};
    int capture_every{6};
    // Fixed-step playback is opt-in for diagnostics. Production callers leave
    // this false and advance from Window::seconds().
    double fixed_step_seconds{0.016};
    bool fixed_step_enabled{};
    unsigned selected_class{};
    flow::SlotFact selected_slot{0, false};
    std::string initial_menu{"main"};
    std::string action_script;
    bool dump_layout{};
    bool verify_native_input{};
    bool verify_generic_creation{};
    // Drives the real Main/EnterName/SelectClass/StartGame contours against
    // the caller's isolated profile callbacks. Requires empty slot 0 and an
    // existing source-created Rogue in slot 1.
    bool verify_profile_slots{};
};

// Converts monotonic seconds to whole source milliseconds while preserving
// sub-millisecond elapsed time between frames. The first sample establishes
// a baseline; negative/non-finite deltas never run the source clock backward.
class FrontendElapsedClockV1 final {
public:
    void reset(double now_seconds) noexcept {
        initialized_ = std::isfinite(now_seconds);
        last_seconds_ = initialized_ ? now_seconds : 0.0;
        fractional_milliseconds_ = 0.0;
    }

    int advance(double now_seconds) noexcept {
        if (!std::isfinite(now_seconds)) return 0;
        if (!initialized_) {
            reset(now_seconds);
            return 0;
        }
        double delta = now_seconds - last_seconds_;
        if (!std::isfinite(delta) || delta <= 0.0) return 0;
        last_seconds_ = now_seconds;
        const double elapsed = delta * 1000.0 + fractional_milliseconds_;
        if (elapsed >= static_cast<double>(std::numeric_limits<int>::max())) {
            fractional_milliseconds_ = 0.0;
            return std::numeric_limits<int>::max();
        }
        const auto whole = static_cast<int>(std::floor(elapsed));
        fractional_milliseconds_ = elapsed - static_cast<double>(whole);
        return whole;
    }

    double fractional_milliseconds() const noexcept { return fractional_milliseconds_; }

private:
    bool initialized_{};
    double last_seconds_{};
    double fractional_milliseconds_{};
};

// Platform-independent policy for binding physical input to a caller-owned
// window. The host may be bound only after actual focus, and focus loss is a
// single edge that the interaction owner can use to cancel captured input.
class FrontendInputFocusGateV1 final {
public:
    bool should_bind_host(bool actually_focused) const noexcept {
        return actually_focused && !host_ready_;
    }
    void host_bound() noexcept { host_ready_ = true; }
    bool observe(bool actually_focused) noexcept {
        const bool lost = input_active_ && !actually_focused;
        focus_lost_pending_ = focus_lost_pending_ || lost;
        input_active_ = host_ready_ && actually_focused;
        return lost;
    }
    bool take_focus_lost() noexcept {
        const bool result = focus_lost_pending_;
        focus_lost_pending_ = false;
        return result;
    }
    bool host_ready() const noexcept { return host_ready_; }
    bool input_enabled(bool actually_focused) const noexcept {
        return host_ready_ && actually_focused;
    }

private:
    bool host_ready_{};
    bool input_active_{};
    bool focus_lost_pending_{};
};

inline std::vector<HostEvent> frontend_filter_host_events_v1(
    std::vector<HostEvent> events, bool host_ready, bool actually_focused,
    bool focus_lost_edge) {
    const bool event_lost_focus = std::any_of(events.begin(), events.end(),
        [](const HostEvent& event) { return event.kind == HostEvent::Kind::focus_lost; });
    if (focus_lost_edge || event_lost_focus) {
        HostEvent lost;
        lost.kind = HostEvent::Kind::focus_lost;
        return {std::move(lost)};
    }
    if (!host_ready || !actually_focused) return {};
    return events;
}

inline void frontend_rebase_after_blocking_load_v1(
    const FrontendRunConfigV1& config, FrontendElapsedClockV1& clock,
    double now_seconds) noexcept {
    if (!config.fixed_step_enabled) clock.reset(now_seconds);
}

inline int frontend_frame_elapsed_milliseconds_v1(
    const FrontendRunConfigV1& config, FrontendElapsedClockV1& clock,
    double now_seconds) noexcept {
    if (config.fixed_step_enabled) {
        const double milliseconds = config.fixed_step_seconds * 1000.0;
        if (!std::isfinite(milliseconds) || milliseconds <= 0.0 ||
            milliseconds >= static_cast<double>(std::numeric_limits<int>::max())) return 0;
        return static_cast<int>(std::lround(milliseconds));
    }
    return clock.advance(now_seconds);
}

// Full native menu entry: loads source art, handles NativeHostInput actions,
// advances class previews, composes source presentation and draws on the
// caller's current Window/Renderer context. The result is a source-service
// outcome, not a promise that the caller performed an actual host swapchain.
FrontendRuntimeResultV1 run_frontend_v1(
    Window&, Renderer&, const FrontendRunConfigV1&,
    FrontendRuntimeServicesV1);

// Binds a caller-owned Window/Renderer pair, the event host attached to that
// Window, and a Navigator configured with injected native flow services. This
// reuses the caller's HWND/WGL context; it never creates a second host window.
class FrontendRuntimeV1 final {
public:
    FrontendRuntimeV1() = default;
    FrontendRuntimeV1(const FrontendRuntimeV1&) = delete;
    FrontendRuntimeV1& operator=(const FrontendRuntimeV1&) = delete;

    bool attach(Window&, Renderer&, FrontendRuntimeServicesV1,
                std::string& error);
    bool poll();
    // Physical events are exposed only while the borrowed Window has actual
    // host focus. A transition out of focus produces one focus_lost event so
    // the interaction owner can cancel captures and pressed keys.
    std::vector<HostEvent> take_input_events();
    void present();
    FrontendRuntimeResultV1 result(FrontendRuntimeOutcomeV1,
                                   std::string error = {}) const;

    Window& window() noexcept { return *window_; }
    Renderer& renderer() noexcept { return *renderer_; }
    NativeHostInput& input_host() noexcept { return input_; }
    bool input_host_ready() const noexcept { return input_focus_.host_ready(); }
    bool input_focused() const noexcept {
        return opened_ && window_ && input_focus_.input_enabled(window_->focused());
    }
    const std::string& input_error() const noexcept { return input_error_; }
    flow::Navigator& navigator() noexcept { return *navigator_; }
    bool opened() const noexcept { return opened_; }

private:
    Window* window_{};
    Renderer* renderer_{};
    NativeHostInput input_;
    FrontendRuntimeServicesV1 services_;
    std::unique_ptr<flow::Navigator> navigator_;
    bool opened_{};
    FrontendInputFocusGateV1 input_focus_;
    std::string input_error_;

    bool bind_input_if_focused(bool initially_fatal, std::string& error);
    void update_input_focus();
};

} // namespace dh::foundation::frontend
