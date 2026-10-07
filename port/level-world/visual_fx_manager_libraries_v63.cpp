#include "visual_fx_manager_libraries_v63.hpp"
#include "application_services_owner_v5.hpp"
#include <stdexcept>
namespace dh2::fx {
VisualFxManagerLibrariesV63::VisualFxManagerLibrariesV63(data::EffectsTables::Borrow tables,VisualFxLibraryDebugV63 debug):
 tables_(std::move(tables)),debug_(std::move(debug)){
 if(!tables_||debug_.application.expired()||!debug_.invoke)throw std::invalid_argument("Required actual App/Arrays/Debug FX library services");
 // Stable native ABI views over the actual immutable authored arrays. Their
 // allocation is adapter storage, not source BuildLibraries or fake vectors.
 if(tables_.sets().size()>4096||tables_.dictionary().values.size()>4096)
  throw std::runtime_error("FX registration outside existing source-kernel native domain");
 registration_steps_.resize(tables_.sets().size());registration_sets_.resize(tables_.sets().size());
 for(std::size_t i=0;i<tables_.sets().size();++i){
  const auto& row=tables_.sets()[i];if(row.steps.size()>4096)throw std::runtime_error("FX steps outside native source registration domain");
  auto& steps=registration_steps_[i];steps.reserve(row.steps.size());
  for(const auto& step:row.steps)steps.push_back({step.file,step.redir});
  registration_sets_[i]={steps.data(),static_cast<std::int32_t>(steps.size()),0};
 }
 queue_storage_.resize(tables_.dictionary().values.size());
 pending10_={queue_storage_.data(),0,static_cast<std::uint32_t>(queue_storage_.size())};
 registration_table_={registration_sets_.data(),static_cast<std::uint32_t>(registration_sets_.size()),static_cast<std::uint32_t>(queue_storage_.size())};
}
bool VisualFxManagerLibrariesV63::belongs_to(const std::shared_ptr<application::ApplicationServicesOwnerV5>& app)const noexcept{
 const auto owner=debug_.application.lock();return owner&&app&&owner.get()==app.get()&&!owner.owner_before(app)&&!app.owner_before(owner);
}
std::uintptr_t VisualFxManagerLibrariesV63::application_identity()const noexcept{
 const auto owner=debug_.application.lock();return owner?owner->identity():0;
}
bool VisualFxManagerLibrariesV63::reject(const std::string& reason,std::string& error){
 failed_=true;if(failure_.empty())failure_=reason.empty()?"Original FX library provider failed":reason;error=failure_;return false;
}
bool VisualFxManagerLibrariesV63::query(std::uint32_t operation,const char* name,std::uint32_t& value,std::string& error){
 const auto app=debug_.application.lock();if(!app||!debug_.invoke){error="Actual App/Debug FX library service expired";return false;}
 return debug_.invoke(operation,name,value,error);
}
bool VisualFxManagerLibrariesV63::module(bool& enabled,std::string& error){
 std::uint32_t value{};
 if(!query(debug_load,nullptr,value,error)||!query(debug_module,"AnimatedFX",value,error))return false;
 enabled=value!=0;return true;
}
int VisualFxManagerLibrariesV63::preload(void* raw,std::uint32_t operation,const char* name,std::uint32_t* value){
 auto& owner=*static_cast<VisualFxManagerLibrariesV63*>(raw);
 if(!value||!owner.query(operation,name,*value,owner.leaf_error_))return -2;return 0;
}
bool VisualFxManagerLibrariesV63::register_impl(std::int32_t id,bool set,std::string& error){
 leaf_error_.clear();PreloadServices16 services{this,preload};
 const auto result=set?dh2_fx_register_set(&registration_table_,&pending10_,id,&services):dh2_fx_register_effect(&registration_table_,&pending10_,id,&services);
 if(result!=1){error=leaf_error_.empty()?"Original FX registration native bound/error "+std::to_string(result):leaf_error_;return false;}
 error.clear();return true;
}
bool VisualFxManagerLibrariesV63::register_effect_to_load(std::int32_t id,std::string& error){
 if(failed_){error=failure_;return false;}
 if(busy_){error="FX library registration reentered native mutation";return false;}
 struct Busy{bool& flag;Busy(bool& value):flag(value){flag=true;}~Busy(){flag=false;}} guard(busy_);
 try{return register_impl(id,false,error);}catch(const std::exception& e){return reject(e.what(),error);}
}
bool VisualFxManagerLibrariesV63::register_set_to_load(std::int32_t id,std::string& error){
 if(failed_){error=failure_;return false;}
 if(busy_){error="FX library registration reentered native mutation";return false;}
 struct Busy{bool& flag;Busy(bool& value):flag(value){flag=true;}~Busy(){flag=false;}} guard(busy_);
 try{return register_impl(id,true,error);}catch(const std::exception& e){return reject(e.what(),error);}
}
bool VisualFxManagerLibrariesV63::build_libraries(std::string& error){
 if(failed_){error=failure_;return false;}
 if(busy_)return reject("FX BuildLibraries reentered; source prefix retained",error);
 struct Busy{bool& flag;Busy(bool& value):flag(value){flag=true;}~Busy(){flag=false;}} guard(busy_);
 try{
  bool enabled{};if(!module(enabled,error))return reject(error,error);
  if(!enabled){error.clear();return true;} // actual GetModule false branch
  if(!dictionary28_.empty()){error.clear();return true;} // original28/2c gate
  byte4_=1; // source496bc4 BEFORE either builder
  for(const auto& file:tables_.dictionary().values){
   auto info=std::make_unique<AnimatedFxInfoV63>();info->file0=&file;
   dictionary28_.push_back(std::move(info)); // real empty active/free C1 members
  }
  if(!sets1c_.empty()){error.clear();return true;} // original1c/20 gate
  for(const auto& row:tables_.sets()){
   pending_set_=std::make_unique<AnimFxSetInfoV63>();pending_set_->authored0=&row;
   for(const auto& step:row.steps){
    if(step.file==-1)continue; // source496a34/-1 skips allocation AND registration
    auto native=std::make_unique<AnimFxStepV63>();native->redirect0=step.redir?1:0;native->file_or_set4=step.file;
    pending_set_->steps4.push_back(std::move(native));
    // Append occurs BEFORE force-cache registration; retain this pending row
    // and every queued source ID if a real provider/native bound later fails.
    if(row.force_cache&&!register_impl(step.file,step.redir!=0,error))return reject(error,error);
   }
   sets1c_.push_back(std::move(pending_set_));
  }
  error.clear();return true;
 }catch(const std::exception& e){return reject(e.what(),error);}catch(...){return reject("Original FX BuildLibraries native adapter threw",error);}
}
bool VisualFxManagerLibrariesV63::precache_libraries(std::string& error){
 if(failed_){error=failure_;return false;}
 if(busy_)return reject("FX PreCacheLibraries reentered",error);
 struct Busy{bool& flag;Busy(bool& value):flag(value){flag=true;}~Busy(){flag=false;}} guard(busy_);
 bool enabled{};if(!module(enabled,error))return reject(error,error);
 if(enabled)byte4_=1;error.clear();return true; // original _PreCacheAnimDict4933d8
}
bool VisualFxManagerLibrariesV63::flush_libraries_v88(const VisualFxLibraryReleaseV88& services,std::string& e){
 if(busy_||flush_failed_v88_){e=flush_failure_v88_.empty()?"FX library Flush cannot replay/reenter a reached prefix":flush_failure_v88_;return false;}
 if(!services.owner||!services.drop_finished||!services.animated_d0||!services.set_data_d1){e="Required actual FX pool deleting bodies";return false;}
 busy_=true;struct Busy{bool& b;~Busy(){b=false;}} busy{busy_};
 auto failed=[&]{flush_failed_v88_=true;flush_failure_v88_=e.empty()?"Actual FX library release failed":e;e=flush_failure_v88_;return false;};
 try{
  for(auto& fx:finished8_v88_)if(fx&&!services.drop_finished(fx,e))return failed();
  for(auto& info:dictionary28_){if(!info)continue;
   //Original rereads free vector bounds after each genuine D0.
   for(std::size_t i=0;i<info->free4.size();++i)if(info->free4[i]){
    if(!services.animated_d0(info->free4[i],e))return failed();info->free4[i].reset();
   }
   info->free4.clear();
   for(auto& fx:info->active10)if(fx){if(!services.animated_d0(fx,e))return failed();fx.reset();}
   info->active10.clear();
  }
  for(auto& info:sets1c_)if(info){for(auto& data:info->active10)if(data){if(!services.set_data_d1(data,e))return failed();data.reset();}info->active10.clear();}
  dictionary28_.clear();sets1c_.clear();finished8_v88_.clear();byte4_=0;
  //Explicit bounded-builder prefix retirement after the published arrays.
  //These really constructed native step allocations were never published.
  pending_set_.reset();failed_=false;failure_.clear();e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return failed();}
 catch(...){e="FX pool release provider threw; reached ownership retained";return failed();}
}

}
