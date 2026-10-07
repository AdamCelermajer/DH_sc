#pragma once
#include "native_root_loading_connection_v50.hpp"

namespace dh2::loader {
struct NativeRootGSBootstrapInputsV55 {
 std::shared_ptr<NativeGSLevelGlobalsV27> globals;
 LevelSourceRequestV1 request;
 GSLevelArgumentsV2 arguments;
 LevelConstructorApplicationV4 application;
 GSLevelServicesV2<CanonicalLevelContextV1> constructor;
 std::function<bool(std::uint32_t&,std::string&)> online_state;
 // Bind actual native Loading callbacks before the synchronous source GS
 // constructor reaches onProgress. This is not a new UI or readiness owner.
 std::function<bool(ui::LoadingMenuStateServicesV1,std::string&)> bind_loading_callbacks;
 // Borrow the fresh source candidate/factory/Scene/PF graph after its actual
 // C1 is complete. The callback must not own this sibling or another GS.
 std::function<bool(const std::shared_ptr<NativeGSLevelRuntimeV27>&,
   const std::shared_ptr<CanonicalLevelContextV1>&,NativeRootLoadingInputsV50&,std::string&)> connect_candidate;
};
// Root start-game composition. One instance belongs to one fresh source-Level
// candidate; retain it before begin() so a failed original constructor prefix
// remains inspectable. Replaces the direct Crypt startup call; it neither
// clones the Crypt mutable world nor performs source unload on an old Level.
class NativeRootGSBootstrapV55 final {
 std::shared_ptr<NativeGSLevelRuntimeV27> gs_;
 std::unique_ptr<NativeRootLoadingConnectionV50> loading_;
 bool attempted_{},complete_{},failed_{};
 std::string error_;
public:
 NativeRootGSBootstrapV55()=default;
 NativeRootGSBootstrapV55(const NativeRootGSBootstrapV55&)=delete;
 NativeRootGSBootstrapV55& operator=(const NativeRootGSBootstrapV55&)=delete;
 bool begin(NativeRootGSBootstrapInputsV55 in,std::string& error){
  if(attempted_){error=error_.empty()?"Root source GS bootstrap already attempted":error_;return false;}
  if(!in.globals||in.globals->s_level||!in.application.owner||
     in.request.identity.empty()||in.request.definition.empty()||in.arguments.name18.empty()||
     in.request.seed!=in.arguments.word20||!in.bind_loading_callbacks||!in.connect_candidate){
   error="Required fresh source candidate, empty sole GS global, authored name/seed and actual providers";return false;
  }
  attempted_=true;
  auto reject=[&](const char* why){if(error.empty())error=why;error_=error;failed_=true;return false;};
  try {
   gs_=std::make_shared<NativeGSLevelRuntimeV27>(in.globals);
   if(!gs_->prepare_loading(in.online_state,error))return reject("Original GS Loading binding failed");
   if(!in.bind_loading_callbacks(gs_->loading_services(),error))return reject("Actual Loading callback registration failed");
   if(!gs_->construct(std::move(in.request),std::move(in.arguments),std::move(in.application),
         std::move(in.constructor),std::move(in.online_state),error))
    return reject("Original GS constructor failed; completed prefix retained");
   const auto& level=gs_->fields().level34;
   if(!level||!gs_->connection()||!gs_->connection()->level_connection()->complete())
    return reject("Original GS did not retain its actual completed C1");
   NativeRootLoadingInputsV50 loading;
   if(!in.connect_candidate(gs_,level,loading,error))return reject("Actual source candidate graph connection failed");
   // The caller supplies graph/platform bodies; source GS/C1/global authority
   // belongs to this exact bootstrap and cannot be replaced by the callback.
   if((loading.gs&&loading.gs!=gs_)||(loading.globals&&loading.globals!=in.globals))
    return reject("Candidate attempted to replace the source GS/global owner");
   loading.gs=gs_;loading.globals=std::move(in.globals);
   if(!NativeRootLoadingConnectionV50::create(std::move(loading),loading_,error))
    return reject("Actual source GS loading attachment failed");
   complete_=true;error_.clear();error.clear();return true;
  }catch(const std::exception& e){error=e.what();return reject("Root source bootstrap provider exception");}
  catch(...){return reject("Root source bootstrap unknown provider exception");}
 }
 GSLevelUpdateResultV44 tick(){
  if(failed_||!complete_||!loading_)return GSLevelUpdateResultV44::failed;
  const auto result=loading_->tick();
  if(result==GSLevelUpdateResultV44::failed){failed_=true;error_=loading_->error();}
  return result;
 }
 const std::string& error()const noexcept{return error_;}
 const auto& gs()const noexcept{return gs_;}
 const auto& loading()const noexcept{return loading_;}
 bool attached()const noexcept{return complete_;}
 bool adopt_into(std::shared_ptr<NativeGSLevelRuntimeV27>& actual_gs,
   std::unique_ptr<NativeRootLoadingConnectionV50>& actual_loading,std::string& error){
  if(!complete_||failed_||!gs_||!loading_||actual_loading||
     (actual_gs&&(actual_gs.get()!=gs_.get()||actual_gs.owner_before(gs_)||gs_.owner_before(actual_gs)))){
   error="Require completed source bootstrap and empty SAME candidate loading slot";return false;
  }
  actual_gs=gs_;actual_loading=std::move(loading_);error.clear();return true;
 }
};
}
