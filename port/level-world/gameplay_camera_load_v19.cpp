#include "gameplay_camera_load_v19.hpp"
namespace dh2::camera {
CameraRuntimeServicesV11 GameplayCameraLoadV19::runtime_services(){
 auto out=services_.runtime;
 out.attach_graph=[this](auto scene,std::string& e){
  std::weak_ptr<GameplayCameraRuntimeV11> weak=level128_;
  auto changed=[this,weak](std::uint32_t index,std::uintptr_t manager,std::string& error){
   auto runtime=weak.lock();if(!runtime){error="Released actual CameraLevel projection receiver";return false;}
   if(index!=0||runtime->scene()->cameras().size()!=1){error="Required additional actual camera-instance projection data owner";return false;}
   if(!manager){runtime->source_aspect_changed_v18(4.f/3.f);return true;}
   if(!services_.backend.configured_backend||!services_.backend.viewport){error="Required actual camera scene-manager driver viewport";return false;}std::int32_t width,height;if(!services_.backend.viewport(width,height,error))return false;runtime->source_aspect_changed_v18(static_cast<float>(width)/static_cast<float>(height));return true;
  };
  membership_=std::make_shared<GameplayCameraSceneMembershipV15>(scene,services_.manager,std::move(changed));return membership_->attach_graph(e);
 };
 out.retain_selected_v19=[this](auto scene,std::uint32_t index,std::string& e){if(!membership_||membership_->scene()!=scene){e="Required SAME selected camera scene membership";return false;}return membership_->retain_camera(index,camera_node_,e);};
 out.attach_animator=[this](auto scene,auto animator,std::string& e){if(!membership_||membership_->scene()!=scene||!camera_node_){e="Required SAME camera scene membership and retained selection";return false;}return membership_->attach_animator(std::move(animator),e);};
 out.detach=[this](auto scene,auto,std::string& e){if(!membership_||membership_->scene()!=scene){e="Required SAME camera detach membership";return false;}return membership_->detach_visual(e);};
 out.activate=[this](std::uintptr_t identity,auto,std::uint32_t index,std::string& e){
  if(!camera_node_||!membership_){e="Required actual selected camera node lifetime";return false;}std::uint32_t same;if(!camera_node_->camera_index(same,e)||same!=index){e="Camera instance identity changed before activation";return false;}
  auto& active=services_.application->active_camera();if(!base_registered_){CameraBaseBorrowV13 b;b.receiver=level128_;b.identity=identity;b.activated=source_camera_base_hook_v13;b.deactivated=source_camera_base_hook_v13;b.scene_camera=membership_->scene_camera_borrow(camera_node_);if(!active.register_receiver(std::move(b),e))return false;base_registered_=true;}
  return active.set_active(identity,*services_.manager,e);
 };
 out.is_active=[this](std::uintptr_t id,bool& active,std::string&){active=services_.application->active_camera().source_active()==id;return true;};
 out.release_active=[this](std::uintptr_t id,std::string& e){services_.application->active_camera().destroy_receiver(id);base_registered_=false;if(membership_&&!membership_->release_parent_camera_references(e))return false;if(camera_node_&&!camera_node_->drop(e))return false;camera_node_.reset();return true;};
 return out;
}
bool GameplayCameraLoadV19::load(const CameraLevelConfigV19& config,std::string& e){
 if(attempted_){e="Level camera load already attempted";return false;}attempted_=true;
 if(!services_.application||!services_.application->events14()||!services_.manager||!services_.actual_first_factory||!services_.actual_zoom_handler50){e="Required SAME Application services14/50 and scene factory/manager";return false;}
 auto overview=services_.overview;overview.application=services_.application;overview.events14=services_.application->events14();overview.manager=services_.manager;overview.actual_first_factory=services_.actual_first_factory;
 overview12c_=std::make_shared<GameplayCameraOverviewV18>(std::move(overview));if(!overview12c_->initialize(e))return false;
 if(!services_.application->active_camera().register_receiver(overview12c_->base_borrow(),e))return false;
 configured_node_=config.camera.node;level128_=std::make_shared<GameplayCameraRuntimeV11>(runtime_services());
 if(!level128_->load(config.camera,e)||!level128_->activate(e)||!level128_->play_idle(e))return false;
 if(!services_.local_character0){e="Required actual PlayerManager.GetLocalPlayer0 Character660";return false;}std::uintptr_t actor;if(!services_.local_character0(actor,e)||!level128_->set_target(actor,0,e))return false;
 if(!services_.actual_zoom_handler50->set_camera(level128_,e))return false;
 level128_->level()->fields().automatic_zoom8c=1;level128_->level()->fields().requested_zoom88=0;
 if(!config.skybox234.empty()){if(!services_.skybox){e="Required original non-empty LevelConfig skybox resource/mesh/root owner";return false;}if(!services_.skybox(config.skybox234,e))return false;}
 loaded_=true;return true;
}
bool GameplayCameraLoadV19::update(std::string& e){if(!loaded_){e="Required loaded actual Level camera";return false;}return level128_->update(e);}
bool GameplayCameraLoadV19::scene_phase(std::uint32_t stamp,std::string& e){if(!loaded_){e="Required loaded camera scene phase";return false;}return level128_->scene_phase(stamp,e);}
bool GameplayCameraLoadV19::view(CameraViewV11& out,std::string& e){if(!loaded_){e="Required loaded camera view";return false;}return level128_->view(out,e);}
bool GameplayCameraLoadV19::source_unbind_zoom_v19(std::string& e){if(!services_.actual_zoom_handler50){e="Required actual Application ZoomHandler50";return false;}return services_.actual_zoom_handler50->set_camera(nullptr,e);}
bool GameplayCameraLoadV19::source_clear_camera_roots_v19(std::string& e){if(membership_&&!membership_->detach_visual(e))return false;if(overview12c_&&overview12c_->node()&&!overview12c_->node()->remove_from_root(e))return false;return true;}
bool GameplayCameraLoadV19::source_clear_scene_active_v19(std::string& e){if(!services_.manager){e="Required SAME SceneManager active-camera clear";return false;}return services_.manager->set_active_camera_v13({},e);}
void GameplayCameraLoadV19::source_flush_animation_sets_v19()noexcept{if(services_.runtime.manager)services_.runtime.manager->source_flush_v17();}
bool GameplayCameraLoadV19::release_native_owners_v19(std::string& e){
 if(level128_&&!level128_->close(e))return false;
 // Failed loads can publish a visual/camera before CameraLevel.loaded becomes
 // true. Its historical close API does not invoke release_active in that
 // prefix, so discard the actual reached child references here once.
 if(membership_&&!membership_->release_parent_camera_references(e))return false;
 if(camera_node_&&!camera_node_->drop(e))return false;
 camera_node_.reset();
 if(overview12c_){if(!services_.application){e="Required actual Application for Overview discard";return false;}if(!overview12c_->close_base(services_.application->active_camera(),e))return false;}
 level128_.reset();overview12c_.reset();membership_.reset();loaded_=false;return true;
}
}
