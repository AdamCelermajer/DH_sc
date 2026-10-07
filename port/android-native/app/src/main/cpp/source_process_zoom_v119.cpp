#include "source_process_zoom_v119.hpp"
#include "application_services_owner_v5.hpp"
#include "gameplay_camera_zoom_v17.hpp"
#include "actual_device_android_v54.hpp"
#include "model_renderer.hpp"
namespace model_renderer {
bool borrow_source_process_zoom_v119(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 std::shared_ptr<dh2::camera::GameplayCameraZoomV17>& out,std::string& e){
 out.reset();if(!app||!app->events14()){e="Required actual process App14 before ZoomHandler.GetInstance";return false;}
 if(app->source_zoom_v119()){
  out=app->source_zoom_v119();
  if(!out->source_initialized_v119()){e="Retained failed source ZoomHandler C1 prefix requires genuine teardown";return false;}
  e.clear();return true;
 }
 struct Backend{std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> app;};
 auto backend=std::make_shared<Backend>();backend->app=app;
 dh2::camera::CameraZoomServicesV17 services;services.backend.configured_backend=backend;
 services.backend.viewport=[backend](auto& width,auto& height,auto& e){
  if(backend->app.expired()){e="Retired actual process ZoomHandler viewport provider";return false;}
  std::int32_t driver{};
  return borrow_actual_device_driver_type_v55(driver,e)&&borrow_actual_camera_viewport_v20(width,height,e);
 };
 auto zoom=std::make_shared<dh2::camera::GameplayCameraZoomV17>(std::move(services));
 out=zoom;
 if(!app->publish_source_zoom_v119(zoom,e)||!zoom->initialize(app->events14(),e))return false;
 e.clear();return true;
}
}
