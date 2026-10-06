#pragma once
#include <hud_startup_callbacks.hpp>
#include <functional>
#include <memory>
#include <string>
namespace dh2::loader {
struct DevicePipelineBorrowServicesV1 {
    std::shared_ptr<void> actual_device_owner;
    // Required actual owner projection, e.g. root's borrow_actual_menu_device_v1
    // mapped to original exclusions and configured driver capability word.
    std::function<bool(ui::HudDevicePipeline16&,std::string&)> borrow_actual;
};
// Does not initialize hardware, menus or a second device singleton. Native
// fixture callers must label their producer; production requires the actual
// initialized device owner. Failure preserves the caller's predicate output.
bool device_high_performance_from_borrow_v1(const DevicePipelineBorrowServicesV1&,
    bool& out,std::string&);
}
