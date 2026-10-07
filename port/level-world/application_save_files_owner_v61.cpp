#include "application_save_files_owner_v61.hpp"
#include "application_services_owner_v5.hpp"
#include <stdexcept>
namespace dh2::application {
namespace {
template<class A,class B> bool same_owner(const std::shared_ptr<A>& a,const std::shared_ptr<B>& b)noexcept{
 return a&&b&&static_cast<const void*>(a.get())==static_cast<const void*>(b.get())&&!a.owner_before(b)&&!b.owner_before(a);
}
struct EndCall {bool& active;~EndCall(){active=false;}};
}
ApplicationSaveFilesOwnerV61::ApplicationSaveFilesOwnerV61(const std::shared_ptr<ApplicationServicesOwnerV5>& app):application_(app){}
bool ApplicationSaveFilesOwnerV61::belongs_to_application(const std::shared_ptr<ApplicationServicesOwnerV5>& app)const noexcept{
 return same_owner(application_.lock(),app);
}
bool ApplicationSaveFilesOwnerV61::matches_directory(const std::string& directory)const noexcept{
 return files_&&files_->directory_v59()==directory;
}
bool ApplicationServicesOwnerV5::publish_source_save_files_v61(std::shared_ptr<ApplicationSaveFilesOwnerV61> owner,std::string& e){
 if(!owner){e="Required actual Application private files/jobs owner";return false;}
 const auto app=owner->application_.lock();
 if(app.get()!=this||(source_save_files_v61_&&!same_owner(source_save_files_v61_,owner))){
  e="Application private files/jobs belong to a different retained owner";return false;
 }
 source_save_files_v61_=std::move(owner);e.clear();return true;
}
bool ApplicationSaveFilesOwnerV61::acquire(const std::shared_ptr<ApplicationServicesOwnerV5>& app,
 const std::string& directory,std::shared_ptr<ApplicationSaveFilesOwnerV61>& out,std::string& e,
 std::shared_ptr<level::PrivateSaveFileTransportV45> old_files,std::shared_ptr<level::SavegameJobsOwnerV2> old_jobs){
 if(!app||directory.empty()||directory.front()!='/'||directory.find('\0')!=std::string::npos){
  e="Required actual Application and Android getFilesDir path";return false;
 }
 if(bool(old_files)!=bool(old_jobs)){e="Existing private FileManager/jobs must be adopted together, never replaced";return false;}
 if(const auto& current=app->source_save_files_v61()){
  out=current;
  if(!current->belongs_to_application(app)||!current->matches_directory(directory)||
     (old_files&&(!same_owner(current->files_,old_files)||!same_owner(current->jobs_,old_jobs)))){
   e="Application private directory/FileManager/job authority changed";return false;
  }
  if(!same_owner(current->jobs_->file_storage_v59(),current->files_)){
   e="Application jobs no longer retain SAME private FileManager";return false;
  }
  e.clear();return true; // No new queue, job replay, file read or reset.
 }
 if(old_files&&(!old_jobs||old_files->directory_v59()!=directory||!same_owner(old_jobs->file_storage_v59(),old_files))){
  e="Existing jobs/private directory do not belong to SAME FileManager";return false;
 }
 auto owner=std::shared_ptr<ApplicationSaveFilesOwnerV61>(new ApplicationSaveFilesOwnerV61(app));
 if(old_files){owner->files_=std::move(old_files);owner->jobs_=std::move(old_jobs);}
 else{
  // The existing transport performs actual bounded POSIX read/write/backup/
  // close. The existing source job constructor starts with an empty list;
  // no fake file/Save/Gear/profile receiver is produced by this binding.
  owner->files_=std::make_shared<level::PrivateSaveFileTransportV45>(directory,private_save_budget);
  owner->jobs_=std::make_shared<level::SavegameJobsOwnerV2>(owner->files_->services());
 }
 out=owner; // Preserve the actual constructed pair if publication fails.
 return app->publish_source_save_files_v61(std::move(owner),e);
}
bool ApplicationSaveFilesOwnerV61::admit(std::string& e)const{
 const auto app=application_.lock();
 if(!app||app->source_save_files_v61().get()!=this||!files_||!jobs_||!same_owner(jobs_->file_storage_v59(),files_)){
  e="Required published SAME Application private files/jobs";return false;
 }
 if(native_call_active_||jobs_->busy_v61()){e="Application source save jobs busy; flush/read completion unavailable";return false;}
 if(jobs_->failed()){e="Application source save job failure prefix retained; no replay/replacement";return false;}
 e.clear();return true;
}
bool ApplicationSaveFilesOwnerV61::valid_name(const char* name,std::string& e){
 if(!name)return true;const std::string text=name;
 if(text.empty()||text=="."||text==".."||text.find_first_of("/\\")!=std::string::npos){e="Invalid actual private save filename";return false;}
 return true;
}
bool ApplicationSaveFilesOwnerV61::read_save(const std::string& name,bool& found,std::vector<std::uint8_t>& bytes,std::string& e){
 std::lock_guard<std::recursive_mutex> lock(native_io_mutex_v102_);
 found=false;bytes.clear();
 if(name.find('\0')!=std::string::npos){e="Invalid actual private save filename";return false;}
 if(!valid_name(name.c_str(),e)||!admit(e))return false;
 native_call_active_=true;EndCall end{native_call_active_};
 if(!jobs_->flush(name.c_str(),e))return false;
 const auto io=files_->services();return io.read_file(io.context,name,found,bytes,e);
}
bool ApplicationSaveFilesOwnerV61::update(bool& progressed,std::string& e){
 std::lock_guard<std::recursive_mutex> lock(native_io_mutex_v102_);
 progressed=false;if(!admit(e))return false;
 native_call_active_=true;EndCall end{native_call_active_};return jobs_->update(progressed,e);
}
bool ApplicationSaveFilesOwnerV61::flush(const char* name,std::string& e){
 std::lock_guard<std::recursive_mutex> lock(native_io_mutex_v102_);
 if(!valid_name(name,e)||!admit(e))return false;
 native_call_active_=true;EndCall end{native_call_active_};return jobs_->flush(name,e);
}
bool ApplicationSaveFilesOwnerV61::write_settings_stream_v102(
 const std::function<std::vector<std::uint8_t>()>& serialize,std::string& e){
 std::lock_guard<std::recursive_mutex> lock(native_io_mutex_v102_);
 if(!serialize||!admit(e)){if(e.empty())e="Required actual settings stream serializer";return false;}
 native_call_active_=true;EndCall end{native_call_active_};
 constexpr const char* name="dh2_settings.savegame";
 if(!jobs_->flush(name,e))return false;
 const auto io=files_->services();void* stream{};
 if(!io.open_write||!io.write||!io.close){e="Required actual settings FileManager stream operations";return false;}
 if(!io.open_write(io.context,name,stream,e))return false;
 if(!stream){e.clear();return true;} //46cb8c: genuine observed NULL returns.
 bool ok=false;
 try{
  const auto bytes=serialize();std::uint64_t written{};
  ok=io.write(io.context,stream,{bytes.data(),bytes.size()},written,e);
  if(ok&&written!=bytes.size()){e="Short raw original settings stream write";ok=false;}
 }catch(const std::exception& ex){e=ex.what();}
 std::string close_error;const bool closed=io.close(io.context,stream,close_error);
 if(!closed){if(e.empty())e=close_error;ok=false;}
 return ok;
}
}
