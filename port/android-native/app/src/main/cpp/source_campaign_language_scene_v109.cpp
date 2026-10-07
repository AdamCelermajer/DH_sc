#include "source_campaign_language_scene_v109.hpp"
#include "model_renderer.hpp"
#include "source_campaign_retirement_v88.hpp"
#include <canonical_object_manager_v1.hpp>
namespace model_renderer {
bool borrow_source_campaign_language_scene_v109(const std::shared_ptr<void>& world,
 SourceCampaignLanguageSceneV109& out,std::string& error){
 out={};SourceCampaignCandidateBorrowV55 current;
 if(!world||source_campaign_retirement_requested_v88()){
  error="Required current campaign for language refresh";return false;
 }
 if(!borrow_source_campaign_candidate_v55(current,error))return false;
 if(current.actual_world!=world||current.actual_world.owner_before(world)||
    world.owner_before(current.actual_world)||!current.objects||
    current.objects->source_lifecycle_failed_v1()){
  error="Campaign localization manager retired or replaced";return false;
 }
 out.manager=current.objects;out.scene=&current.objects->source_language_scene_v109();
 error.clear();return true;
}
}
