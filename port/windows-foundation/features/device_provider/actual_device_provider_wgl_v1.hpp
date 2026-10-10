#pragma once

#include "../../../engine-resources/resource_budget_v37.hpp"
#include "../../../engine-ui/swf_menu_device_v1.hpp"

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace dh::foundation::device {

// Identity of the context that is actually current on this Windows thread.
// It is a witness only; it does not own the WGL context or the selected source
// graphics driver.
struct CurrentWglContextV1 {
    std::uintptr_t device_context{};
    std::uintptr_t rendering_context{};
    std::uint32_t thread_id{};
};

bool current_wgl_context_v1(CurrentWglContextV1&, std::string& error);

// In-house GameObject visual-load quality policy. This is intentionally a
// separate port policy from the recovered original Device driver-enum query.
class GameObjectVisualQualityWglV1 final {
public:
    using Current = std::function<bool(CurrentWglContextV1&, std::string&)>;
    using RendererPolicy = std::function<bool(const CurrentWglContextV1&, bool&, std::string&)>;

    bool bind_current(const Current&, std::string& error);
    void context_lost() noexcept;
    bool high_performance(const Current&, const RendererPolicy&,
                          bool& out, std::string& error) const;

private:
    CurrentWglContextV1 bound_{};
    bool bound_ready_{};
};

// The source driver type remains an operation on the real selected source
// driver. A WGL context alone cannot establish the original virtual +0x5c
// capability word.
class ActualDeviceProviderWglV1 final {
public:
    using Current = std::function<bool(CurrentWglContextV1&, std::string&)>;
    using Facts = std::function<bool(dh2::ui::MenuDeviceFactsV1&, std::string&)>;
    using BudgetLease = std::function<std::shared_ptr<dh2::resources::ContextResourceBudgetV37>()>;
    using DriverType = std::function<bool(const CurrentWglContextV1&, std::uint32_t&, std::string&)>;

    void context_lost() noexcept;
    bool bind_current(const BudgetLease&, const Current&, std::string& error);
    bool high_performance(const BudgetLease&, const Current&, const Facts&,
                          const DriverType&, bool& out, std::string& error) const;

private:
    std::weak_ptr<dh2::resources::ContextResourceBudgetV37> budget_;
    CurrentWglContextV1 bound_{};
    std::uint64_t generation_{};
};

} // namespace dh::foundation::device
