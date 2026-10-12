#include "source_campaign_area_transition_v114.hpp"
#include <utility>
namespace model_renderer {
namespace {
dh2::loader::AreaTransitionStepV114 step(SourceCampaignRetirementResultV88 value){
 using A=dh2::loader::AreaTransitionStepV114;using R=SourceCampaignRetirementResultV88;
 switch(value){case R::pending:return A::pending;case R::complete:return A::complete;case R::failed:return A::failed;}return A::failed;
}
}
bool bind_source_campaign_transition_retirement_v114(SourceCampaignRetirementServicesV88 retirement,
 dh2::loader::AreaTransitionServicesV114& output,std::string& e){
 if(!retirement.owner||!retirement.quiesce||!retirement.retire_world||!output.start_native||!output.native_completion||
    output.begin_retirement||output.drain_retirement||output.retry_retirement){
  e="Require genuine V88 retirement and once-only native start/completion providers";return false;
 }
 auto prepared=output;if(!prepared.owner)prepared.owner=retirement.owner;
 prepared.begin_retirement=[retirement=std::move(retirement)](const auto& request,std::string& error){
  if(!request){error="Required actual confirmed area transition request";return false;}
  return begin_source_campaign_retirement_v88(retirement,error);
 };
 prepared.drain_retirement=[](std::string& error){return step(drain_source_campaign_retirement_v88(error));};
 prepared.retry_retirement=[](std::string& error){return retry_source_campaign_retirement_v114(error);};
 output=std::move(prepared);e.clear();return true;
}
}