#include "canonical_lightset_file_v53.hpp"
#include <algorithm>
namespace dh2::loader {
CanonicalLightsetFileV53::CanonicalLightsetFileV53(CanonicalLightsetFileInputsV53 in):input_(std::move(in)){if(input_.level)fields_=input_.level->module_load_fields();}
LifecycleStepV36 CanonicalLightsetFileV53::step(const std::string& raw,const char* tag,std::string& e){
 auto fail=[&](const std::string& n){if(!failed_){failed_=true;error_=n.empty()?"Actual light-set file producer failed":n;}e=error_;return LifecycleStepV36::failed;};
 if(failed_){e=error_;return LifecycleStepV36::failed;}if(busy_)return fail("Reentrant light-set canonical file delivery");
 busy_=true;struct Guard{bool& b;~Guard(){b=false;}} guard{busy_};
 if(!tag||std::string(tag)!="Module"||!input_.level||!input_.candidate_owner||!input_.manager||!fields_.level_owner||!fields_.module_offset160||!fields_.object_module_id18c)return fail("Required SAME Level/manager/candidate/Module LoadFile fields");
 if(!input_.validate_current||!input_.validate_current(e))return fail(e.empty()?"Required current actual light-set candidate validation":e);
 if(failed_)return fail(error_);
 // This adapter implements resource-open/buffer traversal only. Do not resolve
 // a filename when original LoadFile already owns an assigned stream140.
 if(input_.level->constructor_fields_v3().field140)return fail("Required actual assigned Level140 stream continuation");
 if(!started_){
  if(!input_.actual_filesystem_owner||!input_.resolve_filename)return fail("Required actual light-set filesystem route");
  std::string next;if(!input_.resolve_filename(raw,next,e))return fail(e);if(failed_)return fail(error_);
  if(next.empty()||next.find('\0')!=std::string::npos)return fail("Invalid actual light-set resource identity");
  raw_=raw;uri_=std::move(next);position_={fields_.module_offset160[0],fields_.module_offset160[1],fields_.module_offset160[2]};room_=*fields_.object_module_id18c;
  file_=std::make_unique<CanonicalCachedFileV1>(input_.archive,*input_.manager,input_.classes,input_.source,CanonicalModuleContextV1{input_.candidate_owner,UINT32_MAX,room_,position_},ObjectEntryRouteV1::level);started_=true;
 }
 if(raw!=raw_||!std::equal(position_.begin(),position_.end(),fields_.module_offset160)||room_!=*fields_.object_module_id18c)return fail("Actual light-set occurrence/Level load fields changed");
 const auto result=file_->step(uri_,"Module");if(failed_)return fail(error_);
 if(result==LevelFileWalkStepV1::failed)return fail(file_->error());
 if(!input_.validate_current(e))return fail(e);if(failed_)return fail(error_);
 e.clear();return result==LevelFileWalkStepV1::complete?LifecycleStepV36::complete:LifecycleStepV36::pending;
}
bool CanonicalLightsetFileV53::discard_after_owner_release(const std::function<bool(std::string&)>& release,std::string& e){if(!file_){e.clear();return true;}return file_->discard_after_owner_release(release,e);}
}
