#include "device_pipeline_borrow_v1.hpp"
namespace dh2::loader {
bool device_high_performance_from_borrow_v1(const DevicePipelineBorrowServicesV1& s,bool& out,std::string& e){
    if(!s.actual_device_owner||!s.borrow_actual){e="Required actual device capability owner/borrow";return false;}
    ui::HudDevicePipeline16 actual{};if(!s.borrow_actual(actual,e))return false;
    const auto result=dh2_hud_device_pipeline(&actual);
    if(result<0){e="Actual device source predicate rejected its projection";return false;}
    out=result!=0;return true;
}
}
