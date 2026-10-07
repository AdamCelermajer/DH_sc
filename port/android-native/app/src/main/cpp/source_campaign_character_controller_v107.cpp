#include "source_campaign_character_controller_v107.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "model_renderer.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <application_services_owner_v5.hpp>
namespace model_renderer {
bool source_campaign_character_controller_update_v107(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;SourceCampaignCharacterBorrowV62 actual;
 if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world||
   !borrow_source_campaign_character_v62(world,id,actual,e)||!actual.character->actor)return false;
 auto& record=*actual.character;auto& actor=*record.actor;
 if(!actor.controller){e="Required source Character.Controller378";return false;}
 if(!record.player_controllers_v70)return actor.controller->source_default_update_v107(e);
 if(actor.controller->identity()!=record.player_controllers_v70->identity()||
   record.controllable374_v70.controller378!=actor.controller->identity()){e="Selected controller does not match SAME Mixed/Character378";return false;}
 auto input=candidate.application->source_input_manager_v60();if(!input){e="Required actual InputManager singleton for mixed Update";return false;}
 return record.player_controllers_v70->source_update_v107(*input,{},e);
}
}
