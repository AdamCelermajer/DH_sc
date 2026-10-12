from pathlib import Path
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/level-lightset-v53'
(b/'level_lightset_stage9_v53.hpp').write_text(r'''#pragma once
#include "lifecycle_v36.hpp"
#include "canonical_level_context_v1.hpp"
#include <canonical_level_config_module_v1.hpp>
namespace dh2::loader {
struct Stage9ConfigBorrowV53 {
 std::shared_ptr<void> level_owner,config_owner;
 std::uintptr_t* actual_config38{};std::uintptr_t config_identity{};
 const std::string* fixed2d0{};const std::string* regular2b8{};
};
bool borrow_stage9_config_v53(const std::shared_ptr<CanonicalLevelContextV1>&,
 const std::shared_ptr<world::CanonicalLevelConfigV1>&,Stage9ConfigBorrowV53&,std::string&);
struct Stage9ServicesV53 {
 std::shared_ptr<void> actual_device_owner;
 std::function<bool(bool&,std::string&)> is_fixed_pipeline;
 std::shared_ptr<void> actual_file_loader_owner;
 std::function<LifecycleStepV36(const std::string&,const char*,std::string&)> load_module_file_step;
 // Check actual current Level/config/device owner on each synchronous poll.
 std::function<bool(std::string&)> validate_current;
};
// Source _LoadLightSet3f459c: one of fixed/regular files, tag Module.
// Nonblocking poll adaptation; original loops synchronously until LoadFile!=0.
// This stores no alternate Level, manager, light-set Names or gameplay state.
class Stage9BodyV53 final {
 Stage9ConfigBorrowV53 fields_;Stage9ServicesV53 services_;
 bool selected_{},fixed_{},complete_{},failed_{},busy_{};std::string filename_,error_;
 LifecycleStepV36 fail(const std::string&,std::string&);
public:
 Stage9BodyV53(Stage9ConfigBorrowV53 f,Stage9ServicesV53 s):fields_(std::move(f)),services_(std::move(s)){}
 LifecycleStepV36 step(std::string&);
 const std::string& selected_filename()const noexcept{return filename_;}
};
}
''')
(b/'level_lightset_stage9_v53.cpp').write_text(r'''#include "level_lightset_stage9_v53.hpp"
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
 if(!services_.validate_current(e))return fail(e,e);
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
 if(!services_.validate_current(e))return fail(e,e);
 if(result==LifecycleStepV36::complete)complete_=true;
 if(result!=LifecycleStepV36::dependency_missing)e.clear();return result;
}
}
''')
(b/'canonical_lightset_file_v53.hpp').write_text(r'''#pragma once
#include "canonical_cached_file_v1.hpp"
#include "level_lightset_stage9_v53.hpp"
namespace dh2::loader {
struct CanonicalLightsetFileInputsV53 {
 assets::ZipAssetPackV1 archive;
 std::shared_ptr<CanonicalLevelContextV1> level;
 std::shared_ptr<void> candidate_owner;
 world::CanonicalObjectManagerV1* manager{};
 world::CanonicalClassServicesV1 classes;CanonicalFileSourceServicesV1 source;
 std::shared_ptr<void> actual_filesystem_owner;
 std::function<bool(const std::string&,std::string&,std::string&)> resolve_filename;
 std::function<bool(std::string&)> validate_current;
};
// Same parser/factory/registry producer as root/module load, one retained XML
// occurrence. Actual LightPoint dispatch is REQUIRED; never skip gametype19.
class CanonicalLightsetFileV53 final {
 CanonicalLightsetFileInputsV53 input_;std::unique_ptr<CanonicalCachedFileV1> file_;
 CanonicalLevelContextV1::ModuleLoadFields fields_;std::string raw_,uri_,error_;
 std::array<float,3> position_{};std::int32_t room_{};bool started_{},failed_{},busy_{};
public:
 explicit CanonicalLightsetFileV53(CanonicalLightsetFileInputsV53);
 LifecycleStepV36 step(const std::string&,const char*,std::string&);
 const CanonicalCachedFileV1* actual_file()const noexcept{return file_.get();}
 bool discard_after_owner_release(const std::function<bool(std::string&)>&,std::string&);
};
}
''')
(b/'canonical_lightset_file_v53.cpp').write_text(r'''#include "canonical_lightset_file_v53.hpp"
#include <algorithm>
namespace dh2::loader {
CanonicalLightsetFileV53::CanonicalLightsetFileV53(CanonicalLightsetFileInputsV53 in):input_(std::move(in)){if(input_.level)fields_=input_.level->module_load_fields();}
LifecycleStepV36 CanonicalLightsetFileV53::step(const std::string& raw,const char* tag,std::string& e){
 auto fail=[&](const std::string& n){if(!failed_){failed_=true;error_=n.empty()?"Actual light-set file producer failed":n;}e=error_;return LifecycleStepV36::failed;};
 if(failed_){e=error_;return LifecycleStepV36::failed;}if(busy_)return fail("Reentrant light-set canonical file delivery");
 busy_=true;struct Guard{bool& b;~Guard(){b=false;}} guard{busy_};
 if(!tag||std::string(tag)!="Module"||!input_.level||!input_.candidate_owner||!input_.manager||!fields_.level_owner||!fields_.module_offset160||!fields_.object_module_id18c)return fail("Required SAME Level/manager/candidate/Module LoadFile fields");
 if(!input_.validate_current||!input_.validate_current(e))return fail(e.empty()?"Required current actual light-set candidate validation":e);
 if(!started_){
  if(!input_.actual_filesystem_owner||!input_.resolve_filename)return fail("Required actual light-set filesystem route");
  std::string next;if(!input_.resolve_filename(raw,next,e))return fail(e);
  if(next.empty()||next.find('\0')!=std::string::npos)return fail("Invalid actual light-set resource identity");
  raw_=raw;uri_=std::move(next);position_={fields_.module_offset160[0],fields_.module_offset160[1],fields_.module_offset160[2]};room_=*fields_.object_module_id18c;
  file_=std::make_unique<CanonicalCachedFileV1>(input_.archive,*input_.manager,input_.classes,input_.source,CanonicalModuleContextV1{input_.candidate_owner,UINT32_MAX,room_,position_},ObjectEntryRouteV1::level);started_=true;
 }
 if(raw!=raw_||!std::equal(position_.begin(),position_.end(),fields_.module_offset160)||room_!=*fields_.object_module_id18c)return fail("Actual light-set occurrence/Level load fields changed");
 const auto result=file_->step(uri_,"Module");if(failed_)return fail(error_);
 if(result==LevelFileWalkStepV1::failed)return fail(file_->error());
 if(!input_.validate_current(e))return fail(e);
 e.clear();return result==LevelFileWalkStepV1::complete?LifecycleStepV36::complete:LifecycleStepV36::pending;
}
bool CanonicalLightsetFileV53::discard_after_owner_release(const std::function<bool(std::string&)>& release,std::string& e){if(!file_){e.clear();return true;}return file_->discard_after_owner_release(release,e);}
}
''')
print('Wrote isolated stage9 orchestration and SAME canonical Module file adapter')
