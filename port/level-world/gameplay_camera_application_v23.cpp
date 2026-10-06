#include "gameplay_camera_application_v23.hpp"
namespace dh2::camera {
bool GameplayCameraApplicationV23::initialize(CameraApplicationBindingsV23 s,std::string&e){
 if(attempted_){e="Actual Application camera services already attempted";return false;}attempted_=true;
 if(!s.application||!s.application->events14()||!s.actual_roots||!s.actual_dictionary||!s.actual_dictionary_lease||!s.actual_file_system||!s.backend.configured_backend||!s.backend.viewport||!s.actual_lg_devices||!s.read||!s.debug_after_add){e="Required actual Application camera/SceneManager/dictionary/filesystem/driver services";return false;}
 // An App-owned dictionary alias would create App->context->App ownership.
 if(!s.actual_dictionary_lease.owner_before(s.application)&&!s.application.owner_before(s.actual_dictionary_lease)){e="Dictionary lease must not cycle into its owning Application camera slot";return false;}
 application_=s.application;events14_=s.application->events14();dictionary_lease_=std::move(s.actual_dictionary_lease);dictionary_=s.actual_dictionary;file_system_=std::move(s.actual_file_system);roots_=std::move(s.actual_roots);backend_=std::move(s.backend);
 factory_=std::make_shared<CameraDefaultFactoryV16>(roots_,file_system_,s.actual_cursor,backend_);
 CameraAnimationManagerServicesV10 ms;
 // Actual App identity alias with real heap events14 lifetime; weak guards
 // prevent use after App release. It does not retain App through its own slot.
 ms.application_lease=std::shared_ptr<void>(events14_,s.application.get());ms.dictionary=dictionary_;
 const auto weak=application_;
 ms.actual_lg_devices=[weak,callback=std::move(s.actual_lg_devices)](auto&v,auto&error){if(weak.expired()){error="Released actual Application device producer";return false;}return callback(v,error);};
 ms.read=[weak,callback=std::move(s.read)](const auto&name,auto&bytes,auto&error){if(weak.expired()){error="Released actual Application resource producer";return false;}return callback(name,bytes,error);};
 resource_read_=ms.read;
 ms.debug_after_add=[weak,callback=std::move(s.debug_after_add)](auto&error){if(weak.expired()){error="Released actual Application Debug producer";return false;}return callback(error);};
 animations_=std::make_shared<GameplayCameraAnimationManagerV10>(std::move(ms));
 s.zoom.backend=backend_;zoom_=std::make_shared<GameplayCameraZoomV17>(std::move(s.zoom));
 if(!zoom_->initialize(events14_,e))return false;ready_=true;return true;
}
bool GameplayCameraApplicationV23::load(const CameraLevelConfigV19&config,CameraWorldBindingsV23 bindings,std::shared_ptr<CameraWorldSessionV23>&slot,std::string&e){
 auto app=application_.lock();if(!ready_||!app){e="Required initialized SAME Application camera services";return false;}auto previous=world_.lock();if((previous&&previous->camera)||slot){e="Actual prior World camera prefix requires ordered release before replacement";return false;}
 if(!bindings.world_lease||!bindings.actual_tables){e="Required actual World/table owner";return false;}
 auto session=std::make_shared<CameraWorldSessionV23>();session->bindings=std::move(bindings);auto&wb=session->bindings;CameraLoadServicesV19 s;s.application=std::move(app);s.manager=roots_;s.actual_first_factory=factory_;s.actual_zoom_handler50=zoom_;s.backend=backend_;s.overview=wb.overview;s.local_character0=wb.local_character0;s.skybox=wb.positive_skybox;
 s.runtime.world_lease=wb.world_lease;s.runtime.tables=wb.actual_tables;s.runtime.design=wb.design;s.runtime.manager=animations_;
 // Canonical resources belong to the SAME Application manager. Reuse its
 // supplied real reader through the retained service instead of World copies.
 s.runtime.read=resource_read_;
 s.runtime.actor_services=wb.actors;
 std::weak_ptr<CameraWorldSessionV23> weak_session=session;
 s.runtime.actor_services.handle_centering=[weak_session](auto&p,auto&error){auto actual=weak_session.lock();if(!actual){error="Released actual World camera layout services";return false;}return source_camera_centering_v14(p,actual->bindings.layout,error);};
 session->camera=std::make_unique<GameplayCameraLoadV19>(std::move(s));slot=session;world_=session;return session->camera->load(config,e);
}
bool GameplayCameraApplicationV23::update(std::string&e){auto w=world_.lock();if(!loaded()||application_.expired()||!w){e="Required SAME loaded Application/World camera";return false;}auto* level=w->camera->level()->level();if(!level){e="Required actual CameraLevel fields";return false;}if(!source_camera_autozoom_v14(level->fields().automatic_zoom8c,w->bindings.design,w->bindings.layout,e))return false;return w->camera->update(e);}
bool GameplayCameraApplicationV23::scene_phase(std::uint32_t stamp,std::string&e){auto w=world_.lock();if(!loaded()||application_.expired()||!w){e="Required actual camera scene phase owner";return false;}return w->camera->scene_phase(stamp,e);}
bool GameplayCameraApplicationV23::view(CameraViewV11&out,std::string&e){auto w=world_.lock();if(!loaded()||application_.expired()||!w){e="Required actual submitted camera view";return false;}return w->camera->view(out,e);}
bool GameplayCameraApplicationV23::picking(CameraPickingViewV20 driver,GameplayCameraPickingV20&out,std::string&e){if(!view(driver.camera,e))return false;return out.bind(std::move(driver),e);}
bool GameplayCameraApplicationV23::begin_world_release(std::string&e){auto w=world_.lock();if(!w||!w->camera)return true;return w->camera->source_unbind_zoom_v19(e)&&w->camera->source_clear_camera_roots_v19(e)&&w->camera->source_clear_scene_active_v19(e);}
void GameplayCameraApplicationV23::flush_animation_sets()noexcept{auto w=world_.lock();if(w&&w->camera)w->camera->source_flush_animation_sets_v19();}
bool GameplayCameraApplicationV23::release_world_owners(std::string&e){auto w=world_.lock();if(!w||!w->camera)return true;if(!w->camera->release_native_owners_v19(e))return false;w->camera.reset();w->bindings={};world_.reset();return true;}
bool GameplayCameraApplicationV23::close_application(std::string&e){auto w=world_.lock();if(w&&w->camera){e="Required ordered actual World camera teardown before Application shutdown";return false;}if(zoom_&&!zoom_->close(e))return false;zoom_.reset();animations_.reset();factory_.reset();roots_.reset();dictionary_lease_.reset();dictionary_=nullptr;file_system_.reset();events14_.reset();application_.reset();resource_read_={};ready_=false;return true;}
}
