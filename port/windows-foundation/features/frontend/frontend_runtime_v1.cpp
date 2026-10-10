#include "frontend_runtime_v1.hpp"

#include <algorithm>
#include <utility>

namespace dh::foundation::frontend {

bool FrontendRuntimeV1::attach(Window& window, Renderer& renderer,
                               FrontendRuntimeServicesV1 services,
                               std::string& error) {
    if (opened_) {
        error = "Frontend runtime already attached";
        return false;
    }
    window_ = &window;
    renderer_ = &renderer;
    services_ = std::move(services);
    if (services_.generic_creation)
        services_.navigation = services_.generic_creation->bind_navigation(std::move(services_.navigation));
    navigator_ = std::make_unique<flow::Navigator>(services_.navigation);
    if (window.focused() && !bind_input_if_focused(true, error)) {
        navigator_.reset();
        window_ = nullptr;
        renderer_ = nullptr;
        return false;
    }
    opened_ = true;
    input_focus_.observe(window.focused());
    error.clear();
    return true;
}

bool FrontendRuntimeV1::poll() {
    if (!opened_ || !window_) return false;
    window_->poll();
    update_input_focus();
    return !window_->should_close();
}

bool FrontendRuntimeV1::bind_input_if_focused(bool initially_fatal,
                                               std::string& error) {
    if (!window_ || !input_focus_.should_bind_host(window_->focused())) return true;
    if (!input_.attach_focused_window(error)) {
        input_error_ = error;
        return !initially_fatal;
    }
    input_focus_.host_bound();
    input_error_.clear();
    error.clear();
    return true;
}

void FrontendRuntimeV1::update_input_focus() {
    if (!opened_ || !window_) return;
    const bool focused = window_->focused();
    std::string error;
    bind_input_if_focused(false, error);
    input_focus_.observe(focused);
}

std::vector<HostEvent> FrontendRuntimeV1::take_input_events() {
    if (!opened_ || !window_) return {};
    update_input_focus();
    const bool focused = window_->focused();
    std::vector<HostEvent> events;
    if (input_focus_.host_ready()) {
        events = input_.take_events();
    }
    const bool focusLost = input_focus_.take_focus_lost();
    if (!focused && input_focus_.host_ready()) {
        // Drop events that arrived while inactive. In particular, an up event
        // must not complete a pointer or key capture begun before focus loss.
        events.clear();
    }
    return frontend_filter_host_events_v1(std::move(events),
        input_focus_.host_ready(), focused, focusLost);
}

void FrontendRuntimeV1::present() {
    if (opened_ && window_) window_->swap();
}

FrontendRuntimeResultV1 FrontendRuntimeV1::result(
    FrontendRuntimeOutcomeV1 outcome, std::string error) const {
    FrontendRuntimeResultV1 result;
    result.outcome = outcome;
    result.error = std::move(error);
    if (!opened_ || !navigator_) {
        result.outcome = FrontendRuntimeOutcomeV1::host_failed;
        if (result.error.empty()) result.error = "Frontend runtime is not attached";
        return result;
    }
    result.selected_slot = navigator_->current_slot();

    if (outcome == FrontendRuntimeOutcomeV1::generic_gameplay_started) {
        if (!navigator_->start_delivered()) {
            result.outcome = FrontendRuntimeOutcomeV1::source_operation_failed;
            result.error = "Navigator has not delivered a successful StartGame";
            return result;
        }
        if (!services_.generic_creation || result.selected_slot < 0) {
            result.outcome = FrontendRuntimeOutcomeV1::host_failed;
            result.error = "Generic selected-state/save identity service unavailable";
            return result;
        }
        result.generic_start_receipt = services_.generic_creation->start_receipt();
        if (!result.generic_gameplay_ready()) {
            result.generic_start_receipt.reset();
            result.outcome = FrontendRuntimeOutcomeV1::host_failed;
            result.error = "Generic start receipt does not match the exact shared CharacterState and selected save slot";
            return result;
        }
        return result;
    }

    if (outcome == FrontendRuntimeOutcomeV1::gameplay_handoff_ready) {
        if (!navigator_->start_delivered()) {
            result.outcome = FrontendRuntimeOutcomeV1::source_operation_failed;
            result.error = "Navigator has not delivered a successful native StartGame";
            return result;
        }
        if (result.selected_slot < 0 || !services_.borrow_selected_profile ||
            !services_.borrow_source_start_receipt) {
            result.outcome = FrontendRuntimeOutcomeV1::host_failed;
            result.error = "Native selected-profile/start-receipt providers unavailable";
            return result;
        }
        result.selected_profile = services_.borrow_selected_profile(result.selected_slot);
        const auto receipt = services_.borrow_source_start_receipt(result.selected_slot);
        result.source_receipt = receipt;
        if (!result.selected_profile.valid() || !result.gameplay_ready()) {
            result.selected_profile = {};
            result.source_receipt.reset();
            result.outcome = FrontendRuntimeOutcomeV1::host_failed;
            result.error = "Native start receipt/profile mismatch or canonical InitPost incomplete";
            return result;
        }
    } else if (result.selected_slot >= 0 && services_.borrow_selected_profile) {
        result.selected_profile = services_.borrow_selected_profile(result.selected_slot);
    }
    return result;
}

} // namespace dh::foundation::frontend
