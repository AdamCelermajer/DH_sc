#include "level_lightset_stage9_v53.hpp"
namespace dh2::loader {
bool borrow_stage9_config_v53(const std::shared_ptr<CanonicalLevelContextV1>& level,const std::shared_ptr<world::CanonicalLevelConfigV1>& config,Stage9ConfigBorrowV53& out,std::string& e){
 if(!level||!config){e="Required SAME actual Level/LevelConfig owner";return false;}
 auto f=level->config_fields();auto fixed=config->string(0x2d0);auto regular=config->string(0x2b8);
 if(!f.level_owner||!f.config38||*f.config38!=config->identity()||!fixed||!regular){e="Required published SAME Level.config38/actual config light filenames";return false;}
 out={f.level_owner,config,f.config38,config->identity(),fixed,regular};e.clear();return true;
}
LifecycleStepV36 Stage9BodyV53::fail(const std::string& message,std::string& e){if(!failed_){failed_=true;error_=message.empty()?"Actual source light-set service failed":message;}e=error_;return LifecycleStepV36::failed;}
LifecycleStepV36 Stage9BodyV53::step(std::string& e){
 if(failed_){e=error_;return LifecycleStepV36::failed;}
 if(busy_)return fail("Stage9 reentrant source delivery",e);
 busy_=true;struct Guard{bool& b;~Guard(){b=false;}} guard{busy_};
 if(!fields_.level_owner||!fields_.config_owner||!fields_.actual_config38||!fields_.config_identity||*fields_.actual_config38!=fields_.config_identity||!fields_.fixed2d0||!fields_.regular2b8)return fail("Required current actual Level.config38/retained config",e);
 if(!services_.validate_current)return fail("Required current Level/device validation",e);
 if(!services_.validate_current(e))return fail(e,e);if(failed_){e=error_;return LifecycleStepV36::failed;}
 if(complete_){e.clear();return LifecycleStepV36::complete;}
 if(!selected_){
  if(!services_.actual_device_owner||!services_.is_fixed_pipeline)return fail("Required actual Device.IsFixedPipeline381668",e);
  if(!services_.is_fixed_pipeline(fixed_,e))return fail(e,e);
  if(failed_){e=error_;return LifecycleStepV36::failed;}
  filename_=fixed_?*fields_.fixed2d0:*fields_.regular2b8;selected_=true;
 }else if(filename_!=(fixed_?*fields_.fixed2d0:*fields_.regular2b8))return fail("Actual selected light-set filename changed during occurrence",e);
 if(filename_.empty()){complete_=true;e.clear();return LifecycleStepV36::complete;}
 if(filename_.find('\0')!=std::string::npos)return fail("Source light-set CString contains embedded NUL",e);
 if(!services_.actual_file_loader_owner||!services_.load_module_file_step)return fail("Required actual LoadFile Module continuation",e);
 auto result=services_.load_module_file_step(filename_,"Module",e);
 if(failed_){e=error_;return LifecycleStepV36::failed;}
 if(result==LifecycleStepV36::failed)return fail(e,e);
 if(*fields_.actual_config38!=fields_.config_identity||filename_!=(fixed_?*fields_.fixed2d0:*fields_.regular2b8))return fail("Actual config changed during light-set delivery",e);
 if(!services_.validate_current(e))return fail(e,e);if(failed_){e=error_;return LifecycleStepV36::failed;}
 if(result==LifecycleStepV36::complete)complete_=true;
 if(result!=LifecycleStepV36::dependency_missing)e.clear();return result;
}
}
