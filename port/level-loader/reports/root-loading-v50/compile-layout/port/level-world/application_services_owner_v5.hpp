#pragma once
#include "event_manager_owner_v12.hpp"
#include "gameplay_camera_active_v13.hpp"
namespace dh2::application {
// Retained native Application identity and currently reconstructed service
// fields. This is not a claim that the whole Application ctor/PostInit/Shutdown
// is implemented. C1 zeros pointer14; the first PostInit service allocation
// constructs the distinct heap EventManager and publishes that SAME pointer.
class ApplicationServicesOwnerV5 {
 std::shared_ptr<events::EventManagerOwnerV12> events14_;
 camera::GameplayCameraActiveV13 active_camera_; // ONE process source global
 bool event_publication_attempted_{};
 // Native integration lifetime for the retained renderer's sole camera
 // service context. This is not a recovered Application byte-offset field.
 // SceneManager/factory/Zoom/animation services survive individual Worlds
 // and GL contexts; the adapter must not hold a strong Application cycle.
 std::shared_ptr<void> native_camera_services_v20_;
public:
 ApplicationServicesOwnerV5()=default;
 ApplicationServicesOwnerV5(const ApplicationServicesOwnerV5&)=delete;
 ApplicationServicesOwnerV5& operator=(const ApplicationServicesOwnerV5&)=delete;
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 bool post_init_events_v5(std::string&);
 const std::shared_ptr<events::EventManagerOwnerV12>& events14()const noexcept{return events14_;}
 camera::GameplayCameraActiveV13& active_camera()noexcept{return active_camera_;}
 const std::shared_ptr<void>& native_camera_services_v20()const noexcept{return native_camera_services_v20_;}
 bool publish_native_camera_services_v20(std::shared_ptr<void> owner,std::string& error){
  if(!owner){error="Required retained native camera services owner";return false;}
  if(native_camera_services_v20_&&native_camera_services_v20_.get()!=owner.get()){
   error="Application camera services already published";return false;
  }
  native_camera_services_v20_=std::move(owner);return true;
 }
};
}
