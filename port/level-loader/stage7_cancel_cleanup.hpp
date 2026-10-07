#pragma once
#include "stage_loader_v38_root_file.hpp"
namespace dh2::loader {
// root and callback references are owned by the stable SourceLoading Impl.
// No callback may outlive it. Required cleanup runs before the genuine native
// unload/source-release composition, which retains its own partial prefix.
// Successful steps are cached inside root/the existing unload composer, so a
// failed release retry cannot regenerate, destroy a completed temp, or replay
// completed world unload. Missing real unload never reports cancelled.
inline std::function<LifecycleStepV36(std::string&)> wrap_stage7_cancel_v64(
 Stage7BodyV38& root,std::function<LifecycleStepV36(std::string&)> original){
 if(!original)return {};
 return [&root,original=std::move(original)](std::string& error){
  const auto ready=root.prepare_cancel(error);
  if(ready!=LifecycleStepV36::complete)return ready;
  const auto result=original(error);
  if(result==LifecycleStepV36::complete)root.finish_cancel();
  return result;
 };
}
}
