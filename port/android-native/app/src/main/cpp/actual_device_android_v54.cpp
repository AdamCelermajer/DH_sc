#include "actual_device_android_v54.hpp"
#include "actual_device_provider_v54.hpp"
#include "native_resource_budget_v38.hpp"
#include <EGL/egl.h>
#include <sys/syscall.h>
#include <unistd.h>
namespace model_renderer {bool borrow_actual_menu_device_v1(dh2::ui::MenuDeviceFactsV1&,std::string&);}
namespace {
dh2::world::ActualDeviceProviderV54 actual_device_provider_v54;
bool current_gles_device_v54(dh2::world::CurrentGlesContextV54& out,std::string& error){
 const auto context=eglGetCurrentContext();const auto display=eglGetCurrentDisplay();
 EGLint version=0;
 if(context==EGL_NO_CONTEXT||display==EGL_NO_DISPLAY||eglQueryAPI()!=EGL_OPENGL_ES_API||
    eglQueryContext(display,context,EGL_CONTEXT_CLIENT_VERSION,&version)!=EGL_TRUE){
  error="Required current actual Android GLES context";return false;
 }
 dh2::world::CurrentGlesContextV54 result;
 result.context=reinterpret_cast<std::uintptr_t>(context);
 result.display=reinterpret_cast<std::uintptr_t>(display);
 // Thread token is only a same-call-thread witness, not source driver state.
 const auto tid=syscall(SYS_gettid);if(tid<=0){error="Required actual GL thread identity";return false;}
 result.thread=static_cast<std::uintptr_t>(tid);
 result.client_version=version;result.opengl_es_api=true;out=result;error.clear();return true;
}
// Proposed exported model_renderer declaration in model_renderer.hpp.
}
bool model_renderer::borrow_actual_device_high_performance_v54(bool& out,std::string& error){
 return actual_device_provider_v54.high_performance(dh2::android_resources::budget_lease_v39(),
  current_gles_device_v54,[](dh2::ui::MenuDeviceFactsV1& f,std::string& e){return model_renderer::borrow_actual_menu_device_v1(f,e);},out,error);
}
bool model_renderer::bind_actual_device_context_v54(std::string& error){
 return actual_device_provider_v54.bind_current(dh2::android_resources::budget_lease_v39(),current_gles_device_v54,error);
}
bool model_renderer::borrow_actual_device_driver_type_v55(std::int32_t& out,std::string& error){
 std::uint32_t type{};
 if(!actual_device_provider_v54.renderer_type(dh2::android_resources::budget_lease_v39(),current_gles_device_v54,type,error))return false;
 out=static_cast<std::int32_t>(type);error.clear();return true;
}
void model_renderer::invalidate_actual_device_context_v54() noexcept {actual_device_provider_v54.context_lost();}
