#include "level_source_loading_v43.hpp"
#include "retained_level_module_graph_v1.hpp"
#include "stage_loader_v38_stage10.hpp"
#include "stage_loader_v39_file_counter.hpp"
#include "stage7_cancel_cleanup.hpp"
namespace dh2::loader {
struct SourceLoadingV43::Impl {
 SourceLoadingInputsV43 inputs;
 std::unique_ptr<Stage7BodyV38> root;
 std::unique_ptr<Stage10BodyV38<world::CanonicalObjectManagerV1>> objects;
 std::unique_ptr<LifecycleV36> lifecycle;
 LifecycleDiagnosticsV36 terminal;
 AssignedRootRouteV64 assigned_root;RootSourceModeV64 root_source_mode{RootSourceModeV64::none};
 bool filename_resolved{},unload_completed{};std::string source_name,resolved_name;
 Impl(SourceLoadingInputsV43 in,LifecycleBorrowV36 actual,Stage7FieldsV38 fields,FileCounterBorrowV39 file):inputs(std::move(in)){
  auto root_services=std::move(inputs.procedural);
  root_services.root_file_step=[this](const std::string& name,const char* tag,std::string& e){
   if(!tag||std::string(tag)!="Level"){e="Require original root Level tag";return LifecycleStepV36::failed;}
   const auto& level=inputs.preparation->level();
   const bool own_filename140=root_source_mode==RootSourceModeV64::filename&&inputs.native_filename_source&&inputs.native_filename_source->owns_assigned_prefix();
   if(own_filename140){if(!inputs.native_filename_source->accepts_owned140(*level,e))return LifecycleStepV36::failed;}
   else if(!select_root_source_mode_v64(level->constructor_fields_v3().field140,root_source_mode,e))return LifecycleStepV36::failed;
   if(root_source_mode==RootSourceModeV64::assigned){
    return assigned_root.step(level,name,inputs.assigned_source,[this](auto document,auto owner,auto source,auto& error){
     const auto result=inputs.preparation->load_root_document_step_v64(std::move(document),std::move(owner),std::move(source));
     if(result==LevelFileWalkStepV1::failed)error=inputs.preparation->status().error;return result;
    },e);
   }
   if(inputs.native_filename_source){
    return inputs.native_filename_source->step(level,name,inputs.assigned_source,[this](auto document,auto owner,auto source,auto& error){
     const auto result=inputs.preparation->load_root_document_step_v64(std::move(document),std::move(owner),std::move(source));
     if(result==LevelFileWalkStepV1::failed)error=inputs.preparation->status().error;return result;
    },e);
   }
   if(!filename_resolved){
    if(!inputs.filename_provider||!inputs.resolve_filename){e="Required actual filesystem filename resolver";return LifecycleStepV36::dependency_missing;}
    std::string next;if(!inputs.resolve_filename(name,next,e))return LifecycleStepV36::failed;
    if(next.empty()||next.find('\0')!=std::string::npos){e="Filename resolver returned invalid resource identity";return LifecycleStepV36::failed;}
    source_name=name;resolved_name=std::move(next);filename_resolved=true;
   }else if(source_name!=name){e="Actual root filename changed after source occurrence began";return LifecycleStepV36::failed;}
   const auto result=inputs.preparation->load_root_step_v38(resolved_name);
   if(result==LevelFileWalkStepV1::failed){e=inputs.preparation->status().error;return LifecycleStepV36::failed;}
   return result==LevelFileWalkStepV1::complete?LifecycleStepV36::complete:LifecycleStepV36::pending;
  };
  root_services.increment_actual_file13c=actual_file_counter_increment_v39(std::move(file));
  root=std::make_unique<Stage7BodyV38>(std::move(fields),inputs.globals,std::move(root_services));
  auto object_services=std::move(inputs.object_services);
  object_services.load_module=[this](std::uintptr_t identity,std::string& e){
   return inputs.preparation->load_source_module_v38(identity,inputs.module_xml,e)?LifecycleStepV36::complete:LifecycleStepV36::failed;
  };
  object_services.init_post=[this](const world::CanonicalObjectBorrowV1& object,std::string& e){return inputs.preparation->source_object_init_post_v38(object,e);};
  objects=std::make_unique<Stage10BodyV38<world::CanonicalObjectManagerV1>>(actual,inputs.manager,*inputs.manager,std::move(object_services),std::move(inputs.stage10_trace));
  auto services=std::move(inputs.external);
  services.stage_body[7]=[this](std::string& e){const auto r=root->step();if(r==LifecycleStepV36::failed)e=root->error();return r;};
  services.after_source_increment[7]=[this](std::string& e){return root->after_source_increment(e);};
  services.stage_body[10]=[this](std::string& e){const auto r=objects->step();if(r==LifecycleStepV36::failed)e=objects->error();return r;};
  // Module handles use the same native CFS authority but their own source
  // occurrence. Close/capture them before ANY actual candidate unload.
  if(services.cancel_and_unload&&inputs.preparation->module_files()->native_source_bound()){
   auto original=std::move(services.cancel_and_unload);
   services.cancel_and_unload=[this,original=std::move(original)](std::string& e){
    const auto close=inputs.preparation->module_files()->close_native_source_before_unload(e);
    if(close!=LifecycleStepV36::complete)return close;
    return original(e);
   };
  }
  // Filename source handle is independent of the copied Level140 receiver.
  // Close it before actual teardown, retaining it while close yields/fails.
  if(services.cancel_and_unload&&inputs.native_filename_source){auto original=std::move(services.cancel_and_unload);
   services.cancel_and_unload=[this,original=std::move(original)](std::string& e){
    const auto close=inputs.native_filename_source->close_before_unload(inputs.preparation->level(),e);
    if(close!=LifecycleStepV36::complete)return close;return original(e);
   };
  }
  services.cancel_and_unload=compose_assigned_cancel_v64(assigned_root,unload_completed,
   [this]{return inputs.preparation->level();},inputs.assigned_source,std::move(services.cancel_and_unload));
  // Drop filename route's own captured parser/Level aliases only after genuine
  // external unload and assigned cleanup succeeded, even if Main retains route.
  if(services.cancel_and_unload&&inputs.native_filename_source){auto finish=std::move(services.cancel_and_unload);
   services.cancel_and_unload=[this,finish=std::move(finish)](std::string& e){
    const auto result=finish(e);if(result!=LifecycleStepV36::complete)return result;
    if(!inputs.native_filename_source->release_after_unload(inputs.preparation->level(),inputs.assigned_source,e))return LifecycleStepV36::failed;
    return LifecycleStepV36::complete;
   };
  }
  services.cancel_and_unload=wrap_stage7_cancel_v64(*root,std::move(services.cancel_and_unload));
  auto pins=std::move(inputs.resource_pins);pins.push_back(inputs.preparation);pins.push_back(inputs.manager);
  lifecycle=std::make_unique<LifecycleV36>(actual.fields,std::move(actual.actual_level_owner),std::move(pins),std::move(services));
 }
 LifecycleStatusV36 tick(){
  if(!lifecycle)return terminal.status;
  const auto status=lifecycle->tick();
  if(status==LifecycleStatusV36::cancelled){
   terminal=lifecycle->diagnostics();lifecycle.reset();objects.reset();root.reset();inputs={};
   assigned_root=AssignedRootRouteV64{};root_source_mode=RootSourceModeV64::none;
   source_name.clear();resolved_name.clear();
  }
  return status;
 }
};
SourceLoadingV43::SourceLoadingV43(std::unique_ptr<Impl> in):impl_(std::move(in)){}
SourceLoadingV43::~SourceLoadingV43()=default;
bool SourceLoadingV43::create(SourceLoadingInputsV43 in,std::unique_ptr<SourceLoadingV43>& out,std::string& e){
 if(!in.preparation||!in.manager||in.manager.get()!=&in.preparation->manager()){e="Require SAME retained preparation and actual manager lease";return false;}
 if(in.preparation->status().stage==RetainedModulePreparationStageV1::failed||in.manager->source_init_phase7c_v38()!=0){e="Require fresh source dispatcher with actual manager phase0";return false;}
 if(in.external.stage_body[7]||in.external.stage_body[10]||in.external.after_source_increment[7]||in.procedural.root_file_step||in.procedural.increment_actual_file13c||in.object_services.load_module||in.object_services.init_post){e="Source7/10 bindings must have one authoritative provider";return false;}
 LifecycleBorrowV36 actual;Stage7FieldsV38 root;FileCounterBorrowV39 file;
 const auto& level=in.preparation->level();
 if(!borrow_lifecycle_fields_v36(level,actual,e)||!borrow_stage7_fields_v38(level,root,e)||!borrow_file_counter_v39(level,file,e))return false;
 if(*actual.fields.state130>38){e="Invalid actual source loading state";return false;}
 auto next=std::unique_ptr<SourceLoadingV43>(new SourceLoadingV43(std::make_unique<Impl>(std::move(in),std::move(actual),std::move(root),std::move(file))));
 out=std::move(next);e.clear();return true;
}
LifecycleStatusV36 SourceLoadingV43::tick(){return impl_->tick();}
void SourceLoadingV43::request_cancel()noexcept{if(impl_->lifecycle)impl_->lifecycle->request_cancel();}
const LifecycleDiagnosticsV36& SourceLoadingV43::diagnostics()const noexcept{return impl_->lifecycle?impl_->lifecycle->diagnostics():impl_->terminal;}
}
