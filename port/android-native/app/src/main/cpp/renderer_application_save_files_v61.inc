// Include with application_save_files_owner_v61.hpp after the actual renderer
// App service borrow declarations. This helper never allocates per-profile or
// per-World save queues. Cache/archive asset transport remains the existing
// RendererLevelFilesV25 owner supplied to the selected-profile inputs.
[[maybe_unused]] static bool prepare_application_save_files_v61(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& application,
 const std::string& actual_private_directory,
 std::shared_ptr<dh2::application::ApplicationSaveFilesOwnerV61>& out,std::string& e){
 return dh2::application::ApplicationSaveFilesOwnerV61::acquire(application,actual_private_directory,out,e);
}
template<class ProfileInputs> static bool bind_profile_file_inputs_v61(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& application,
 ProfileInputs& input,std::string& e){
 if(!input.selected){e="Required actual selected profile/private directory before file binding";return false;}
 std::shared_ptr<dh2::application::ApplicationSaveFilesOwnerV61> actual;
 if(!prepare_application_save_files_v61(application,input.selected->private_directory,actual,e))return false;
 if((input.private_files&&(input.private_files.get()!=actual->files().get()||
     input.private_files.owner_before(actual->files())||actual->files().owner_before(input.private_files)))||
    (input.application_jobs&&(input.application_jobs.get()!=actual->jobs().get()||
     input.application_jobs.owner_before(actual->jobs())||actual->jobs().owner_before(input.application_jobs)))){
  e="Selected-profile inputs contain a different Application FileManager/jobs";return false;
 }
 input.private_files=actual->files();input.application_jobs=actual->jobs();e.clear();return true;
}
// Root's application frame invokes this once, independently of a current
// World/Level, to advance the original global save queue. Before FileManager
// construction the caller may explicitly skip this native transport; an
// invoked missing owner never masquerades as completed source UpdateJobs.
[[maybe_unused]] static bool update_application_save_jobs_v61(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& application,
 bool& progressed,std::string& e){
 progressed=false;if(!application){e="Required actual Application save-job frame";return false;}
 const auto& owner=application->source_save_files_v61();
 if(!owner){e="Required published original Application save-job owner";return false;}
 return owner->update(progressed,e);
}
