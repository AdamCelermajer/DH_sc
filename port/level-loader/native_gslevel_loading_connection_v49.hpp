#pragma once
#include "native_gslevel_frame_v45.hpp"
#include "level_source_loading_update_v46.hpp"
namespace dh2::loader {
// Native integration policy, not another original engine body: attach the
// checked loading-only Update to the actual GS frame's SAME C1. The original
// phase38/forced gameplay body remains an explicit main-owned continuation.
// Sibling owner only; no GS -> connection -> GS or callback -> connection cycle.
class NativeGSLevelLoadingConnectionV49 final {
 std::shared_ptr<LevelSourceLoadingUpdateV46> loading_;
 std::unique_ptr<NativeGSLevelFrameV45> frame_;
public:
 using GameplayUpdate=std::function<bool(const std::shared_ptr<CanonicalLevelContextV1>&,bool,std::string&)>;
 static bool create(std::shared_ptr<NativeGSLevelRuntimeV27> runtime,
  GSApplicationBorrowV44 application,GSLevelUpdateServicesV44<CanonicalLevelContextV1> services,
  SourceLoadingInputsV43 inputs,SourceLoadProcessPrefixV46 prefix,GameplayUpdate gameplay,
  std::unique_ptr<NativeGSLevelLoadingConnectionV49>& out,std::string& error){
  if(!runtime){error="Required actual constructed GS runtime for loading connection";return false;}
  if(services.level_update||services.borrow_level||services.level_load){error="GS loading connection requires sole actual Level.Load/Update/borrow providers";return false;}
  GSLevelFieldsV2<CanonicalLevelContextV1>* fields{};
  if(!runtime->frame_fields_v45(fields,error))return false;
  if(!fields||!fields->level34){error="Required actual GS Level34 receiver";return false;}
  auto level=fields->level34;
  std::unique_ptr<LevelSourceLoadingUpdateV46> loading;
  if(!LevelSourceLoadingUpdateV46::create(level,std::move(inputs),std::move(prefix),loading,error))return false;
  auto retained=std::shared_ptr<LevelSourceLoadingUpdateV46>(std::move(loading));
  services.level_update=[retained,gameplay=std::move(gameplay)](const auto& receiver,bool force,std::string& e){
   const auto result=retained->update(receiver,force);
   if(result==LevelLoadingUpdateResultV46::failed){e=retained->error();return false;}
   if(result==LevelLoadingUpdateResultV46::main_gameplay_required){
    if(!gameplay){e="Required main original Level.Update gameplay3f8350 continuation";return false;}
    return gameplay(receiver,force,e);
   }
   e.clear();return true;
  };
  std::unique_ptr<NativeGSLevelFrameV45> frame;
  if(!NativeGSLevelFrameV45::create(std::move(runtime),std::move(application),std::move(services),frame,error))return false;
  auto next=std::make_unique<NativeGSLevelLoadingConnectionV49>();
  next->loading_=std::move(retained);next->frame_=std::move(frame);
  out=std::move(next);error.clear();return true;
 }
 // Drain the pinned actual source independently of GS/menu/gameplay frames.
 // Keep this connection on cleanup failure; an explicit request enables retry.
 void request_cancel()noexcept{loading_->request_cancel();}
 bool cancellation_requested()const noexcept{return loading_->cancellation_requested();}
 LifecycleStatusV36 drain_cancel(std::string& error){return loading_->drain_cancel(error);}
 GSLevelUpdateResultV44 tick(){
  if(cancellation_requested())return GSLevelUpdateResultV44::failed;
  return frame_->tick();
 }
 const std::string& error()const noexcept{return cancellation_requested()?loading_->error():frame_->error();}
 const std::string& required_service()const noexcept{return frame_->required_service();}
 const LifecycleDiagnosticsV36& loading_diagnostics()const noexcept{return loading_->diagnostics();}
};
}
