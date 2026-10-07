#pragma once
#include "assigned_loadfile_kernel_v65.hpp"
#include "level_root_filename_route_v52.hpp"
namespace dh2::loader {
struct FilenameSourceLeavesV65 {
 std::shared_ptr<void> owner;
 std::function<bool(const std::string&,bool&,std::string&)> is_using_uncompiled_data;
 // Same actual ordered CFS open boundary as V52. Successful found=true MUST
 // also lend genuine IStreamBase handle/owner; canonical URI is diagnostic.
 std::function<bool(const std::string&,bool&,std::string&,SourceIStreamBorrowV65&,std::string&)> open_resource;
 std::function<LifecycleStepV36(SourceIStreamBorrowV65&,std::string&)> close_source;
 AssignedCopyLeavesV65 copy;
};
class FilenameRootRouteV65 {
 FilenameSourceLeavesV65 leaves_;
 std::unique_ptr<LevelRootFilenameResolverV52> resolver_;
 SourceIStreamBorrowV65 opened_;
 std::shared_ptr<void> expected_;
 std::weak_ptr<CanonicalLevelContextV1> level_seen_;
 std::uintptr_t expected_identity_{};
 std::string name_,canonical_,error_;
 enum class Phase{fresh,close_source,read,complete,failed};Phase phase_{Phase::fresh};
 bool busy_{},close_reentered_{};AssignedRootRouteV64 assigned_;
 LifecycleStepV36 fail(std::string& e){phase_=Phase::failed;if(error_.empty())error_=e.empty()?"Actual filename source prefix failed":e;e=error_;return LifecycleStepV36::failed;}
 bool close_done_{},close_owner_invalid_{};
 // Close can yield after consuming native handle metadata. Keep the SAME
 // independent owner until genuine completion; never restore a retired handle.
 std::shared_ptr<void> close_pin_;std::uintptr_t close_identity_{};
 bool same_close_owner()const noexcept{
  return !opened_.actual_owner||(opened_.actual_owner.get()==close_pin_.get()&&
   !opened_.actual_owner.owner_before(close_pin_)&&!close_pin_.owner_before(opened_.actual_owner));
 }
 LifecycleStepV36 close_source_prefix(std::string& e){
  if(close_owner_invalid_){e="Filename source close ownership was replaced";return LifecycleStepV36::failed;}
  if(close_done_)return LifecycleStepV36::complete;
  if(!close_pin_){
   if(!opened_.actual_owner&&!opened_.identity){close_done_=true;return LifecycleStepV36::complete;}
   if(!opened_.actual_owner){e="Source close requires genuine retained IStream owner";return LifecycleStepV36::failed;}
   close_pin_=opened_.actual_owner;close_identity_=opened_.identity;
  }
  if(!same_close_owner()||(opened_.identity&&opened_.identity!=close_identity_)){
   close_owner_invalid_=true;e="Filename source close receiver/owner was replaced";return LifecycleStepV36::failed;
  }
  if(!leaves_.close_source){e="Required actual filesystem close leaf";return LifecycleStepV36::dependency_missing;}
  if(!opened_.actual_owner)opened_.actual_owner=close_pin_;
  try{
   const auto result=leaves_.close_source(opened_,e);
   // Record completed native work before recursion/error checks; explicit
   // cancellation retry must never invoke a completed native close again.
   if(result==LifecycleStepV36::complete)close_done_=true;
   if(!same_close_owner()||(opened_.identity&&opened_.identity!=close_identity_)){
    close_owner_invalid_=true;if(e.empty())e="Filename source close replaced the genuine owner/handle";return LifecycleStepV36::failed;
   }
   if(!opened_.actual_owner)opened_.actual_owner=close_pin_;
   if(close_reentered_||phase_==Phase::failed){
    if(e.empty())e="Filename source close recursively failed";return LifecycleStepV36::failed;
   }
   if(result!=LifecycleStepV36::complete)return result;
   opened_={};close_pin_.reset();close_identity_=0;return result;
  }catch(const std::exception& ex){
   if(!opened_.actual_owner)opened_.actual_owner=close_pin_;
   if(e.empty())e=ex.what();return LifecycleStepV36::failed;
  }catch(...){
   if(!opened_.actual_owner)opened_.actual_owner=close_pin_;
   if(e.empty())e="Filename source close provider threw";return LifecycleStepV36::failed;
  }
 }
public:
 explicit FilenameRootRouteV65(FilenameSourceLeavesV65 leaves):leaves_(std::move(leaves)){
  LevelRootFilenameServicesV52 route;route.actual_filesystem_owner=leaves_.owner;
  route.is_using_uncompiled_data=leaves_.is_using_uncompiled_data;
  route.open_resource=[this](const std::string& name,bool& found,std::string& canonical,std::string& e){
   if(!leaves_.open_resource){e="Required genuine filename IStreamBase open leaf";return false;}
   SourceIStreamBorrowV65 next;const bool ok=leaves_.open_resource(name,found,canonical,next,e);
   if(next.actual_owner||next.identity)opened_=std::move(next); // retain reached prefix even on failure
   if(!ok)return false;
   if(found&&(!opened_.actual_owner||!opened_.identity)){e="Filename open did not retain actual source handle";return false;}
   if(!found&&opened_.actual_owner){e="Missing resource returned a live source handle";return false;}return true;
  };
  resolver_=std::make_unique<LevelRootFilenameResolverV52>(std::move(route));
 }
 FilenameRootRouteV65(const FilenameRootRouteV65&)=delete;
 bool owns_assigned_prefix()const noexcept{return expected_identity_!=0;}
 bool accepts_owned140(const CanonicalLevelContextV1& level,std::string& e)const{
  if(level_seen_.lock().get()!=&level){e="Filename occurrence changed retained Level owner";return false;}
  const auto actual=level.constructor_fields_v3().field140;
  const auto& owning=level.assigned_source_owner_slot_v65();
  if(phase_==Phase::complete&&!actual&&!owning)return true;
  if(!expected_||actual!=expected_identity_||!owning||owning.get()!=expected_.get()||owning.owner_before(expected_)||expected_.owner_before(owning)){e="Filename occurrence assigned140 cleared/replaced prematurely";return false;}return true;
 }
 // Used by cancellation before native candidate teardown. Actual handle is
 // retained on close failure/pending; no parser/copy replay or fake close.
 LifecycleStepV36 close_before_unload(const std::shared_ptr<CanonicalLevelContextV1>& level,std::string& e){
  if(phase_!=Phase::fresh&&level_seen_.lock()!=level){e="Cannot close another Level filename occurrence";return LifecycleStepV36::failed;}
  if(busy_){close_reentered_=true;e="Filename source close reentered";return LifecycleStepV36::failed;}
  if(close_owner_invalid_){e="Cannot unload after filename source ownership replacement";return LifecycleStepV36::failed;}
  if(close_done_){opened_={};close_pin_.reset();close_identity_=0;return LifecycleStepV36::complete;}
  busy_=true;struct Guard{bool& flag;~Guard(){flag=false;}} guard{busy_};
  close_reentered_=false;
  // A source error is sticky for loading, but cancellation may retry its SAME
  // close prefix. Only new recursion during this delivery is a cleanup failure.
  const auto saved_phase=phase_;if(phase_==Phase::failed)phase_=Phase::close_source;
  const auto result=close_source_prefix(e);
  if(saved_phase==Phase::failed)phase_=saved_phase;
  return result;
 }
 // Capture only SAME assigned ownership metadata before explicit unload;
 // this handles copied/close-pending prefixes before the first Read call.
 bool capture_release_prefix_before_unload(const std::shared_ptr<CanonicalLevelContextV1>& level,const AssignedRootServicesV64& services,std::string& e){
  if(busy_){e="Cannot capture filename release prefix during delivery";return false;}
  if(phase_!=Phase::fresh&&level_seen_.lock()!=level){e="Filename cleanup belongs to another Level";return false;}
  if(expected_identity_&&!accepts_owned140(*level,e))return false;
  return assigned_.capture_release_prefix_before_unload(level,services,e);
 }
 bool release_after_unload(const std::shared_ptr<CanonicalLevelContextV1>& level,const AssignedRootServicesV64& services,std::string& e){
  if(phase_!=Phase::fresh&&level_seen_.lock()!=level){e="Cannot release another Level filename occurrence";return false;}
  if(!assigned_.release_after_unload(level,services,e))return false;
  expected_.reset();phase_=Phase::failed;return true;
 }
 template<class Consume>
 LifecycleStepV36 step(const std::shared_ptr<CanonicalLevelContextV1>& level,const std::string& name,const AssignedRootServicesV64& services,Consume consume,std::string& e){
  if(busy_){e="Filename load source reentered";return fail(e);}
  if(phase_==Phase::failed){e=error_;return LifecycleStepV36::failed;}
  if(!level){e="Required SAME retained filename Level";return fail(e);}
  if(phase_!=Phase::fresh&&level_seen_.lock()!=level){e="Filename controller belongs to another retained Level";return fail(e);}
  if(phase_!=Phase::fresh&&name!=name_){e="Filename occurrence changed original source name";return fail(e);}
  if(phase_==Phase::complete)return LifecycleStepV36::complete;
  busy_=true;struct Guard{bool& flag;~Guard(){flag=false;}} guard{busy_};
  try{
   if(phase_==Phase::fresh){
    if(level->constructor_fields_v3().field140||level->assigned_source_owner_slot_v65()){e="Fresh filename source cannot adopt a foreign assigned receiver";return fail(e);}
    name_=name;level_seen_=level;
    if(!resolver_->resolve(name,canonical_,nullptr,e)||phase_==Phase::failed)return fail(e);
    if(!copy_stream_to_level140_v65(*level,opened_,leaves_.copy,e)||phase_==Phase::failed)return fail(e);
    expected_=level->assigned_source_owner_slot_v65();expected_identity_=level->constructor_fields_v3().field140;phase_=Phase::close_source;
   }
   if(!accepts_owned140(*level,e))return fail(e);
   if(phase_==Phase::close_source){
    close_reentered_=false;const auto result=close_source_prefix(e);if(phase_==Phase::failed)return fail(e);
    if(result==LifecycleStepV36::pending)return result;
    if(result!=LifecycleStepV36::complete)return fail(e);
    phase_=Phase::read;
    return LifecycleStepV36::pending; // Original filename prefix returns false; Read is NEXT call.
   }
   const auto result=assigned_.step(level,name_,services,std::move(consume),e);
   if(result==LifecycleStepV36::complete){phase_=Phase::complete;expected_.reset();return result;}
   if(result!=LifecycleStepV36::pending)return fail(e);return result;
  }catch(const std::exception& ex){if(e.empty())e=ex.what();return fail(e);}
 }
};
}