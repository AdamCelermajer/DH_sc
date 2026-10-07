#pragma once
#include "native_gslevel_loading_connection_v49.hpp"
#include "source_load_process_prefix_v50.hpp"
#include "stage_loader_v46_physical.hpp"

namespace dh2::loader {
// Root adoption policy, not a second loading engine. Retain as a SIBLING of
// the actual GS on WorldScriptContext. Application/provider leases must not
// strongly retain the World containing this connection; weak World callbacks
// and aliasing manager leases keep ownership acyclic.
struct NativeRootLoadingInputsV50 {
 std::shared_ptr<NativeGSLevelRuntimeV27> gs;
 std::shared_ptr<NativeGSLevelGlobalsV27> globals;
 GSApplicationBorrowV44 application;
 GSLevelUpdateServicesV44<CanonicalLevelContextV1> frame;
 SourceLoadingInputsV43 source;
 world::CanonicalObjectManagerV1::FreshSourceBorrowV50 manager_freshness;
 LoadPrefixServicesV50 prefix;
 Stage0ApplicationBorrowV46 stage0_application;
 Stage0ServicesV46 stage0;
 std::shared_ptr<physical::NativeWorld> physical_world;
 EarlyLoadingDebugV46 early_debug;
 NativeGSLevelLoadingConnectionV49::GameplayUpdate gameplay;
};
class NativeRootLoadingConnectionV50 final {
 std::shared_ptr<NativeGSLevelRuntimeV27> gs_;
 std::shared_ptr<NativeGSLevelGlobalsV27> globals_;
 std::shared_ptr<CanonicalLevelContextV1> level_;
 std::unique_ptr<NativeGSLevelLoadingConnectionV49> connected_;
 bool failed_{};
 std::string lifetime_error_;
 bool current_lifetime(){
  GSLevelFieldsV2<CanonicalLevelContextV1>* actual{};
  if(!gs_->frame_fields_v45(actual,lifetime_error_) || !actual ||
     actual->level34.get()!=level_.get() || globals_->s_level.get()!=level_.get() ||
     globals_->s_level.owner_before(level_) || level_.owner_before(globals_->s_level)){
   if(lifetime_error_.empty())lifetime_error_="Root loading source GS/current Level lifetime changed";
   failed_=true;return false;
  }
  return true;
 }
public:
 NativeRootLoadingConnectionV50(const NativeRootLoadingConnectionV50&)=delete;
 NativeRootLoadingConnectionV50& operator=(const NativeRootLoadingConnectionV50&)=delete;
 NativeRootLoadingConnectionV50()=default;
 static bool create(NativeRootLoadingInputsV50 in,
   std::unique_ptr<NativeRootLoadingConnectionV50>& out,std::string& error) {
  if(!in.gs || !in.globals || !in.gs->owns_globals_v50(in.globals)){
   error="Required actual root GS and SAME sole global storage owner";return false;
  }
  GSLevelFieldsV2<CanonicalLevelContextV1>* gs_fields{};
  if(!in.gs->frame_fields_v45(gs_fields,error))return false;
  const auto level=gs_fields->level34;
  CanonicalCurrentLevelBorrowV1 current;
  if(!in.gs->current(current,error))return false;
  auto same=[&](const auto& a,const auto& b){return a && b && a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
  if(!level || !same(current.level(),level) || !same(in.globals->s_level,level)){
   error="Root loading requires SAME constructed GS34/published C1/global owner";return false;
  }
  if(!in.source.preparation || !same(in.source.preparation->level(),level) ||
     !in.source.manager || in.source.manager.get()!=&in.source.preparation->manager()){
   error="Root loading requires SAME C1/preparation/canonical registry";return false;
  }
  if(!in.manager_freshness.same_fresh_producer(in.source.manager)){
   error="Root loading requires authentic fresh source manager C1 producer, not demo phase0";return false;
  }
  const auto& candidate=in.source.preparation->candidate_owner_v50();
  if(!candidate || candidate.owner_before(in.source.manager) || in.source.manager.owner_before(candidate)){
   error="Root manager lease must alias its SAME authentic preparation candidate owner";return false;
  }
  if(in.source.external.stage_body[0] || in.source.external.stage_body[5] || in.prefix.current_level){
   error="Root loading Stage0/Stage5/current-Level must have one authoritative provider";return false;
  }
  LifecycleBorrowV36 fields;
  Stage0LevelBorrowV46 stage0_fields;
  if(!borrow_lifecycle_fields_v36(level,fields,error) ||
     !borrow_stage0_level_v46(level,stage0_fields,error))return false;
  // Read the same global on EVERY source call. This captures the process
  // global owner, never GS/World/this; it introduces no owner self-cycle.
  in.prefix.current_level=[globals=in.globals](LoadPrefixLevelV50& out,std::string& e){
   CanonicalCurrentLevelBorrowV1 actual;
   if(!borrow_current_canonical_level_v1(globals->borrow(globals),actual,e))return false;
   if(!actual){out={};e.clear();return true;}
   CanonicalLevelContextV1::LoadingFieldsV26 f;
   if(!actual.level()->loading_fields_v26(f,e))return false;
   out={actual.level(),actual.identity(),f.state130};e.clear();return true;
  };
  auto prefix=std::make_shared<SourceLoadProcessPrefixV50>(
    LoadPrefixLevelV50{fields.actual_level_owner,fields.identity,fields.fields.state130},std::move(in.prefix));
  SourceLoadProcessPrefixV46 prefix_bridge;
  prefix_bridge.actual_provider_owner=prefix;
  prefix_bridge.before_dispatch=[prefix](std::uint32_t phase,std::string& e){return prefix->before_dispatch(phase,e);};
  prefix_bridge.debug_only_stage=[prefix](std::uint32_t phase,std::string& e){return prefix->debug_only_stage(phase,e);};
  in.source.external.stage_body[0]=stage0_body_v46(std::move(stage0_fields),std::move(in.stage0_application),std::move(in.stage0));
  in.source.external.stage_body[5]=stage5_physical_body_v46(fields,std::move(in.physical_world),std::move(in.early_debug));
  std::unique_ptr<NativeGSLevelLoadingConnectionV49> connection;
  if(!NativeGSLevelLoadingConnectionV49::create(in.gs,std::move(in.application),
       std::move(in.frame),std::move(in.source),std::move(prefix_bridge),
       std::move(in.gameplay),connection,error))return false;
  auto next=std::make_unique<NativeRootLoadingConnectionV50>();
  next->gs_=std::move(in.gs);next->globals_=std::move(in.globals);
  next->level_=level;next->connected_=std::move(connection);
  out=std::move(next);error.clear();return true;
 }
 // Cleanup remains reachable after an original frame or publication failure.
 void request_cancel()noexcept{connected_->request_cancel();}
 bool cancellation_requested()const noexcept{return connected_->cancellation_requested();}
 LifecycleStatusV36 drain_cancel(std::string& error){return connected_->drain_cancel(error);}
 GSLevelUpdateResultV44 tick(){
  if(cancellation_requested()||failed_)return GSLevelUpdateResultV44::failed;
  if(!current_lifetime())return GSLevelUpdateResultV44::failed;
  const auto result=connected_->tick();
  // Preserve a source failure's first diagnostic. Otherwise reject callback
  // teardown/publication changes before the caller renders a stale Level.
  if(result==GSLevelUpdateResultV44::failed)return result;
  if(!current_lifetime())return GSLevelUpdateResultV44::failed;
  return result;
 }
 const std::string& error()const noexcept{return cancellation_requested()?connected_->error():(failed_?lifetime_error_:connected_->error());}
 const std::string& required_service()const noexcept{
  static const std::string lifetime="SAME root GS/current Level lifetime";
  return failed_?lifetime:connected_->required_service();
 }
 const LifecycleDiagnosticsV36& diagnostics()const noexcept{return connected_->loading_diagnostics();}
 const std::shared_ptr<CanonicalLevelContextV1>& level()const noexcept{return level_;}
};
}
