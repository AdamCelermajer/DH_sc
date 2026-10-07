#include "source_settings_update_job_v102.hpp"
#include "owned_hud_settings_v1.hpp"
#include "application_save_files_owner_v61.hpp"
#include <thread>
#include <mutex>
#include <deque>
#include <exception>
namespace model_renderer {
namespace {
class SettingsUpdateJobV102:public std::enable_shared_from_this<SettingsUpdateJobV102> {
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application_;
 std::mutex error_mutex_;std::deque<std::string> worker_errors_;
public:
 //Only actual source stores: C1 byte8=1, job1 argument2/job2 argument3.
 const std::uint8_t source_byte8{1};const std::int32_t source_argument0c;
 SettingsUpdateJobV102(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,std::int32_t argument):application_(app),source_argument0c(argument){}
 bool belongs(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app)const{
  auto actual=application_.lock();return actual&&app&&actual.get()==app.get()&&!actual.owner_before(app)&&!app.owner_before(actual);
 }
 bool start2(std::string& error){
  auto self=shared_from_this();
  try{
   //Every Start2 launches the original one-shot worker; no fake busy gate or
   //shared profile queue substitutes for pthread_create's actual execution.
   std::thread([self]{
    std::string failure;
    try{
     auto app=self->application_.lock();
     if(!app)failure="Settings worker lost actual process Application";
     else if(!save_process_settings_v102(app,failure)&&failure.empty())failure="Actual settings worker save failed";
    }catch(const std::exception& ex){failure=ex.what();}
    if(!failure.empty()){std::lock_guard<std::mutex> lock(self->error_mutex_);self->worker_errors_.push_back(std::move(failure));}
   }).detach();
  }catch(const std::exception& ex){error=std::string("Actual settings Start2 thread creation failed: ")+ex.what();return false;}
  error.clear();return true;
 }
 bool take_error(std::string& error){std::lock_guard<std::mutex> lock(error_mutex_);
  if(worker_errors_.empty()){error.clear();return true;}error=std::move(worker_errors_.front());worker_errors_.pop_front();return false;
 }
};
std::mutex process_job_mutex;
std::shared_ptr<SettingsUpdateJobV102> process_job;
std::shared_ptr<SettingsUpdateJobV102> process_job1_v118;
bool borrow(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 std::shared_ptr<SettingsUpdateJobV102>& out,std::string& error,bool first=false){
 out.reset();std::lock_guard<std::mutex> lock(process_job_mutex);
 const auto& job=first?process_job1_v118:process_job;
 if(!job||!job->belongs(app)){error="Required reached SAME Application settings updateJob_thread C1";return false;}
 out=job;error.clear();return true;
}
}
bool enroll_process_settings_job_v102(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,std::string& error){
 if(!app){error="Required actual Application C1 for settings updateJob_thread";return false;}
 std::lock_guard<std::mutex> lock(process_job_mutex);
 if((process_job&&!process_job->belongs(app))||(process_job1_v118&&!process_job1_v118->belongs(app))){error="Different Application already owns settings updateJob_thread";return false;}
 //Original C1 order. Keep the first real allocation if the second fails;
 //never replace a previously produced job2 while extending old native backing.
 if(!process_job1_v118)process_job1_v118=std::make_shared<SettingsUpdateJobV102>(app,2);
 if(!process_job)process_job=std::make_shared<SettingsUpdateJobV102>(app,3);
 error.clear();return true;
}
bool start_process_settings_job_v102(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,std::string& error){
 std::shared_ptr<SettingsUpdateJobV102> job;if(!borrow(app,job,error))return false;return job->start2(error);
}
bool start_process_settings_job1_v118(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,std::string& error){
 std::shared_ptr<SettingsUpdateJobV102> job;if(!borrow(app,job,error,true))return false;return job->start2(error);
}
bool take_process_settings_job_error_v102(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,std::string& error){
 std::shared_ptr<SettingsUpdateJobV102> job1,job2;
 if(!borrow(app,job1,error,true)||!borrow(app,job2,error))return false;
 return job1->take_error(error)&&job2->take_error(error);
}
bool save_process_settings_v102(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,std::string& error){
 if(!app||!app->source_settings4c_v67()){error="Required actual App4c SavegameManager for settings worker";return false;}
 //threadfun3 fetches App4c when it EXECUTES, not a captured profile/snapshot.
 auto settings=app->source_settings4c_v67();
 if(!settings->source_save_settings_gate_v102()){error.clear();return true;}
 auto files=app->source_save_files_v61();
 if(!files||!files->belongs_to_application(app)){error="Required SAME process FileManager for saveSettings";return false;}
 //Serialization occurs only after real openSavefile/matching-job flush.
 //The synchronized byte buffer is transport data, not another settings owner.
 return files->write_settings_stream_v102([settings]{return settings->serialized();},error);
}
}
