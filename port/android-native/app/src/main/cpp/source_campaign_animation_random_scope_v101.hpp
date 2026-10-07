#pragma once
#include "canonical_character_candidate_v60.hpp"
namespace model_renderer {
// The animation selector borrows a native projection of the shared Random.
// Publish it before reentrant callbacks, then retain every nested consumption.
class CampaignAnimationRandomScopeV101 {
 dh2::world::CanonicalCharacterCandidateRecordV60& record_;
 dh2::data::AnimationRandom* projection_;
public:
 explicit CampaignAnimationRandomScopeV101(dh2::world::CanonicalCharacterCandidateRecordV60& r)
  :record_(r),projection_(r.animation_random_inflight_v101){
  if(projection_&&record_.services.random){record_.services.random->seed=projection_->seed;record_.services.random->calls=projection_->calls;}
 }
 ~CampaignAnimationRandomScopeV101(){
  if(projection_&&record_.services.random){projection_->seed=record_.services.random->seed;projection_->calls=record_.services.random->calls;}
 }
 CampaignAnimationRandomScopeV101(const CampaignAnimationRandomScopeV101&)=delete;
 CampaignAnimationRandomScopeV101& operator=(const CampaignAnimationRandomScopeV101&)=delete;
};
}
