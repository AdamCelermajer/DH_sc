#include "actual_device_provider_wgl_v1.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <GL/gl.h>

#include "../../../engine-ui/hud_startup_callbacks.hpp"

namespace dh::foundation::device {

bool current_wgl_context_v1(CurrentWglContextV1& out, std::string& error) {
    const HGLRC context = wglGetCurrentContext();
    const HDC dc = wglGetCurrentDC();
    const DWORD thread = GetCurrentThreadId();
    if (!context || !dc || thread == 0) {
        error = "Required current actual WGL context, device context, and thread";
        return false;
    }
    out = {reinterpret_cast<std::uintptr_t>(dc),
           reinterpret_cast<std::uintptr_t>(context),
           static_cast<std::uint32_t>(thread)};
    error.clear();
    return true;
}

bool GameObjectVisualQualityWglV1::bind_current(const Current& current,
                                                std::string& error) {
    context_lost();
    if (!current) {
        error = "Required current WGL context provider for GameObject quality policy";
        return false;
    }
    CurrentWglContextV1 context;
    if (!current(context, error)) return false;
    if (!context.device_context || !context.rendering_context || !context.thread_id) {
        error = "Required live current WGL context for GameObject quality policy";
        return false;
    }
    bound_ = context;
    bound_ready_ = true;
    error.clear();
    return true;
}

void GameObjectVisualQualityWglV1::context_lost() noexcept {
    bound_ = {};
    bound_ready_ = false;
}

bool GameObjectVisualQualityWglV1::high_performance(
        const Current& current, const RendererPolicy& policy, bool& out,
        std::string& error) const {
    if (!bound_ready_ || !current || !policy) {
        error = "Required bound WGL context and actual Renderer/App quality policy";
        return false;
    }
    CurrentWglContextV1 now;
    if (!current(now, error)) return false;
    if (now.device_context != bound_.device_context ||
        now.rendering_context != bound_.rendering_context ||
        now.thread_id != bound_.thread_id) {
        error = "Required same current WGL context for GameObject quality policy";
        return false;
    }
    bool selected_quality{};
    if (!policy(now, selected_quality, error)) return false;
    out = selected_quality;
    error.clear();
    return true;
}

void ActualDeviceProviderWglV1::context_lost() noexcept {
    budget_.reset();
    bound_ = {};
    generation_ = 0;
}

bool ActualDeviceProviderWglV1::bind_current(const BudgetLease& lease,
                                              const Current& current,
                                              std::string& error) {
    context_lost();
    if (!lease || !current) {
        error = "Required actual WGL resource-budget lease and current-context provider";
        return false;
    }
    const auto budget = lease();
    if (!budget) {
        error = "Required actual WGL resource-budget lease";
        return false;
    }
    const auto before = budget->context_state_v41();
    CurrentWglContextV1 context;
    if (!current(context, error)) return false;
    const auto after = budget->context_state_v41();
    if (!before.ready || !after.ready || !before.generation ||
        before.generation != after.generation || !context.device_context ||
        !context.rendering_context || !context.thread_id) {
        error = "Required live selected WGL context and ready resource-budget generation";
        return false;
    }
    budget_ = budget;
    bound_ = context;
    generation_ = after.generation;
    error.clear();
    return true;
}

bool ActualDeviceProviderWglV1::high_performance(
        const BudgetLease& lease, const Current& current, const Facts& facts,
        const DriverType& driver_type, bool& out, std::string& error) const {
    const auto bound_budget = budget_.lock();
    if (!bound_budget || !lease || !current || !facts) {
        error = "Required same retained actual WGL Device owner and source facts";
        return false;
    }
    const auto actual_budget = lease();
    if (!actual_budget || actual_budget != bound_budget) {
        error = "Required same retained actual WGL resource-budget owner";
        return false;
    }
    const auto before = bound_budget->context_state_v41();
    CurrentWglContextV1 now;
    if (!current(now, error)) return false;
    const auto after = bound_budget->context_state_v41();
    if (!before.ready || !after.ready || before.generation != generation_ ||
        after.generation != generation_ || now.device_context != bound_.device_context ||
        now.rendering_context != bound_.rendering_context || now.thread_id != bound_.thread_id) {
        error = "Required current bound WGL context and resource-budget generation";
        return false;
    }

    dh2::ui::MenuDeviceFactsV1 device_facts{};
    if (!facts(device_facts, error)) return false;
    std::uint32_t type = 0;
    // Original Device::IsHighPerformance short-circuits on these exact source
    // fields before invoking the selected driver's virtual +0x5c operation.
    if (!device_facts.sharp && !device_facts.htc && !device_facts.multiplayer_mode) {
        if (!driver_type) {
            error = "Required selected source WGL driver capability provider";
            return false;
        }
        if (!driver_type(now, type, error)) return false;
    }
    const dh2::ui::HudDevicePipeline16 source{{device_facts.sharp,
        device_facts.htc, device_facts.multiplayer_mode}, type};
    const int result = dh2_hud_device_pipeline(&source);
    if (result < 0) {
        error = "Actual source Device pipeline rejected WGL facts";
        return false;
    }
    out = result != 0;
    error.clear();
    return true;
}

} // namespace dh::foundation::device
