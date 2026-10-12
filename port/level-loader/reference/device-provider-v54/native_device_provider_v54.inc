// Proposed inclusion outside native_app.cpp anonymous namespace, before existing
// borrow_actual_menu_device_v1 definition. Add actual_device_provider_v54.hpp,
// <EGL/egl.h>, <sys/syscall.h>, <unistd.h> includes.
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
bool model_renderer::borrow_actual_device_high_performance_v54(bool& out,std::string& error){
 return actual_device_provider_v54.high_performance(dh2::android_resources::budget_lease_v39(),
  current_gles_device_v54,[](dh2::ui::MenuDeviceFactsV1& f,std::string& e){return front_ui.menu_device_borrow_v4(f,e);},out,error);
}
// REQUIRED exact insertion points in NativeBridge.initialize:
// Before context_lost_v38: actual_device_provider_v54.context_lost();
// Immediately after begin_context_v38:
// std::string device_error;
// if(!actual_device_provider_v54.bind_current(dh2::android_resources::budget_lease_v39(),current_gles_device_v54,device_error))throw std::runtime_error(device_error);
// On any initialize catch/context teardown: actual_device_provider_v54.context_lost();
// Character qualified parent and existing Item initialization.device_high_performance
// should borrow the SAME exported function above. No shader/GL allocation, no error drain.
// Flags are actual current FrontUi owner cells (JNI Build string producers).
// Main must write actual multiplayer transitions to its authoritative flag cell;
// existing genuine zero default supports only untransitioned single-player state.
