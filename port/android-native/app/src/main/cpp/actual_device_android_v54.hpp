#pragma once
#include <string>
#include <cstdint>
namespace model_renderer {
// Called on the actual GL thread immediately after begin_context_v38.
bool bind_actual_device_context_v54(std::string&);
void invalidate_actual_device_context_v54() noexcept;
bool borrow_actual_device_high_performance_v54(bool&,std::string&);
bool borrow_actual_device_driver_type_v55(std::int32_t&,std::string&);
}
