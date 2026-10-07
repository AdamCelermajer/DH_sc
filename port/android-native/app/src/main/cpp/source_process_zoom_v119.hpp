#pragma once
#include <memory>
#include <string>
namespace dh2::application {class ApplicationServicesOwnerV5;}
namespace dh2::camera {class GameplayCameraZoomV17;}
namespace model_renderer {
bool borrow_source_process_zoom_v119(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 std::shared_ptr<dh2::camera::GameplayCameraZoomV17>&,std::string&);
}
