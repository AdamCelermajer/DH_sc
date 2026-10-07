// Include in source_campaign_runtime_v61.cpp beside stage9's source binder.
// Concrete SAME candidate/loading slot integration; actual process RES_PATH
// loan is passed from Main. Do not substitute private/profile files_directory.
bool bind_source_stage15_audio_v94(const std::shared_ptr<SourceWorldBorrowV61>& world,
 const std::shared_ptr<RendererSourceCandidateV55>& candidate,
 const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>& level,
 dh2::loader::EarlyLoadingDebugV46 debug,
 std::function<bool(std::string&,std::string&)> actual_res_path,
 dh2::loader::SourceLoadingInputsV43& loading,std::string& error){
 using namespace dh2;using namespace dh2::loader;
 if(!world||!world->owner||!world->application||!world->canonical_world||!world->design||!candidate||candidate->world.lock()!=world||candidate->canonical!=world->canonical_world||!candidate->preparation||candidate->preparation->level()!=level||!actual_res_path||loading.external.stage_body[15]){error="Required SAME Stage15 candidate/design/RES_PATH and empty source slot";return false;}
 auto tables=world->design->borrow();auto held=std::make_shared<decltype(tables)>(std::move(tables));
 if(!held->levels()){error="Required SAME actual Arrays.LevelList";return false;}
 const auto weak_world=std::weak_ptr<SourceWorldBorrowV61>(world);
 const auto weak_candidate=std::weak_ptr<RendererSourceCandidateV55>(candidate);
 const auto weak_level=std::weak_ptr<CanonicalLevelContextV1>(level);
 const auto weak_app=std::weak_ptr<application::ApplicationServicesOwnerV5>(world->application);
 auto current=[weak_world,weak_candidate,weak_level,weak_app](std::string& e){auto world=weak_world.lock();auto candidate=weak_candidate.lock();auto level=weak_level.lock();auto app=weak_app.lock();SourceCampaignCandidateBorrowV55 actual;
  if(!world||!candidate||!level||!app||world->application!=app||candidate->world.lock()!=world||candidate->canonical!=world->canonical_world||!borrow_source_campaign_candidate_runtime_v61(actual,e)||actual.actual_world!=world->owner||actual.level!=level||actual.application!=app||candidate->preparation->level()!=level||level->constructor_fields_v3().field130!=15){if(e.empty())e="Retired SAME actual Stage15 World/Level/candidate";return false;}return true;};
 Stage15LevelBorrowV94 actual;if(!borrow_stage15_level_v94(level,actual,error))return false;
 Stage15AudioServicesV94 services;services.debug=std::move(debug);
 services.sound_manager=[current](audio::AudioApplicationBorrowV42& out,std::string& e){return current(e)&&model_renderer::borrow_actual_application_audio_v42(out,e);};
 services.routing.owner=held;services.routing.levels=held->levels();services.routing.validate_delivery=current;
 services.routing.borrow_res_path=[current,borrow=std::move(actual_res_path)](std::string& out,std::string& e){return current(e)&&borrow(out,e);};
 auto body=stage15_audio_source_v94(std::move(actual),std::move(services));
 loading.external.stage_body[15]=[current,body=std::move(body)](std::string& e)mutable{if(!current(e))return LifecycleStepV36::failed;return body(e);};
 error.clear();return true;
}
