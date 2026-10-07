#pragma once
#include "level_update_loading_v46.hpp"
#include "level_source_loading_v43.hpp"
#include "retained_level_module_graph_v1.hpp"
namespace dh2::loader {
struct SourceLoadProcessPrefixV46 {
 std::shared_ptr<void> actual_provider_owner;
 // Original3f69b0..3f6a24 complete per-call prefix: optional fast-travel
 // unlock/world-map byte12c, DebugSwitches.load/GetSwitch exact
 // IsDisplayLoadingStepName, optional StepName/MenuDebug.SetText, _DEBUG_OUT.
 // Receives actual state word; no copied state or alternative current global.
 std::function<bool(std::uint32_t,std::string&)> before_dispatch;
 // Existing bounded dispatcher deliberately skips debug-only stages3/16/19.
 // Their actual GetInstance/GetSwitch(isTracingLevel_Loading) must run here.
 std::function<bool(std::uint32_t,std::string&)> debug_only_stage;
};
// Bounded native response adapter: original per-call prefix, ONE authentic
// stage step, original scalar tail and required main/menu publish callback.
// Source loops yield between steps; not an exact synchronous whole Init call.
class LevelSourceLoadingUpdateV46 final {
 std::shared_ptr<CanonicalLevelContextV1> level_;
 std::shared_ptr<SourceLoadingV43> source_;
 std::unique_ptr<LevelLoadingUpdateV46<CanonicalLevelContextV1>> update_;
 bool cancel_requested_{};std::string cancel_error_;
public:
 static bool create(std::shared_ptr<CanonicalLevelContextV1> level,SourceLoadingInputsV43 in,SourceLoadProcessPrefixV46 prefix,std::unique_ptr<LevelSourceLoadingUpdateV46>& out,std::string& error){
  LifecycleBorrowV36 actual;if(!borrow_lifecycle_fields_v36(level,actual,error))return false;
  if(!in.preparation||in.preparation->level().get()!=level.get()||in.preparation->level().owner_before(level)||level.owner_before(in.preparation->level())){error="Level.Update source preparation requires SAME actual C1 owner";return false;}
  std::unique_ptr<SourceLoadingV43> driver;if(!SourceLoadingV43::create(std::move(in),driver,error))return false;
  auto source=std::shared_ptr<SourceLoadingV43>(std::move(driver));
  using Bridge=LevelLoadingUpdateV46<CanonicalLevelContextV1>;
  // Raw pointer is an internal observer only. Bridge owns this slot through
  // its callback; the slot owns no Bridge and introduces no self-cycle.
  auto bridge_slot=std::make_shared<Bridge*>(nullptr);
  LevelLoadProcessProviderV46 body;body.actual_level_owner=actual.actual_level_owner;body.identity=actual.identity;
  body.load_process=[source,actual,bridge_slot,prefix=std::move(prefix)](std::string& e){
   if(!prefix.actual_provider_owner||!prefix.before_dispatch){e="Required original Level._LoadProcess per-call debug/fast-travel prefix3f69b0..3f6a24";return false;}
   if(!prefix.before_dispatch(*actual.fields.state130,e))return false;
   if(*bridge_slot&&(*bridge_slot)->failure_latched()){e="Level.Update prefix reentered; dispatcher not invoked";return false;}
   const auto phase=*actual.fields.state130;
   if(phase==3||phase==16||phase==19){if(!prefix.debug_only_stage){e="Required original debug-only loading stage GetInstance/GetSwitch(isTracingLevel_Loading)";return false;}if(!prefix.debug_only_stage(phase,e))return false;}
   if(*bridge_slot&&(*bridge_slot)->failure_latched()){e="Level.Update debug stage reentered; dispatcher not invoked";return false;}
   const auto result=source->tick();
   if(result==LifecycleStatusV36::failed){e=source->diagnostics().error;return false;}
   if(result==LifecycleStatusV36::cancelled||result==LifecycleStatusV36::cancelling){e="Original Level.Update loading branch does not perform cancellation";return false;}
   e.clear();return true;
  };
  std::unique_ptr<LevelLoadingUpdateV46<CanonicalLevelContextV1>> update;if(!LevelLoadingUpdateV46<CanonicalLevelContextV1>::create(level,std::move(body),update,error))return false;
  *bridge_slot=update.get();
  auto next=std::make_unique<LevelSourceLoadingUpdateV46>();next->level_=std::move(level);next->source_=std::move(source);next->update_=std::move(update);out=std::move(next);error.clear();return true;
 }
 void request_cancel()noexcept{cancel_requested_=true;source_->request_cancel();}
 bool cancellation_requested()const noexcept{return cancel_requested_;}
 LifecycleStatusV36 drain_cancel(std::string& error){
  if(!cancel_requested_){error="Request loader cancellation before draining teardown";return LifecycleStatusV36::failed;}
  const auto status=source_->tick();
  if(status==LifecycleStatusV36::failed){cancel_error_=source_->diagnostics().error;error=cancel_error_;return status;}
  if(status!=LifecycleStatusV36::cancelling&&status!=LifecycleStatusV36::cancelled){
   cancel_error_="Cancellation drain returned a non-teardown lifecycle state";error=cancel_error_;return LifecycleStatusV36::failed;
  }
  cancel_error_.clear();error.clear();return status;
 }
 LevelLoadingUpdateResultV46 update(const std::shared_ptr<CanonicalLevelContextV1>& level,bool force){
  if(cancel_requested_){cancel_error_="Loading/gameplay Update stopped for loader cancellation";return LevelLoadingUpdateResultV46::failed;}
  return update_->update(level,force);
 }
 const std::string& error()const noexcept{return cancel_error_.empty()?update_->error():cancel_error_;}
 const std::string& required_service()const noexcept{return update_->required_service();}
 const LifecycleDiagnosticsV36& diagnostics()const noexcept{return source_->diagnostics();}
};
}
