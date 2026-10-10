#include "source_campaign_character_clean_v107.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "source_campaign_fx_v77.hpp"
#include "model_renderer.hpp"
#include <character_clean_v123.hpp>
namespace model_renderer {
bool source_campaign_character_clean_v107(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& e){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,e)||!actual.character->actor)return false;
 auto& r=*actual.character;
 dh2::world::CharacterCleanServicesV123 services;
 services.retire_draws=[&](auto& error){return retire_source_campaign_batch_object_v113(world,id,error);};
 services.detach_fx_anchors=[&](auto& error){return retire_source_campaign_fx_anchor_v117(world,id,error);};
 services.borrow_fx_owner=[&](auto& error){return borrow_source_campaign_fx_v77(world,r.target_fx_pin_v70,r.target_fx_manager_v70,error);};
 return dh2::world::character_clean_v123(r,services,e);
}
}
