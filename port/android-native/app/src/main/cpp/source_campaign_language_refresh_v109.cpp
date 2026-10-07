#include "source_campaign_language_refresh_v109.hpp"
#include "source_campaign_language_scene_v109.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_items_v88.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "model_renderer.hpp"
#include "canonical_receiver_transport_v1.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "world_loot_gameplay_v23.hpp"
#include "source_item_resources_v88.hpp"
#include "settings_language_scene_v2.hpp"
#include "item_language_localization_v1.hpp"
namespace model_renderer {
namespace {
struct LanguageDeliveryV109 {
 std::shared_ptr<void> world;
 SourceCampaignLanguageSceneV109 scene;
 std::string error;
 bool item(dh2::data::ItemInstanceV1& item){
  std::shared_ptr<dh2::character::SourceItemResourcesV88> resources;
  if(!borrow_source_campaign_item_resources_v88(world,resources,error)||!resources||!resources->ready()||!resources->presentation())return false;
  auto presentation=resources->presentation();auto text=resources->text();
  dh2::ui::ItemLanguagePowerServicesV1 powers;powers.owner=resources;
  powers.clear=[presentation](auto& item,auto& e){if(!presentation->forget(item,e))return false;item.powers.clear();return true;};
  powers.add=[presentation,text](auto& item,auto id,auto mode,auto& e){return presentation->add_power(item,id,mode,text,e);};
  dh2::ui::ItemLanguageReceiptV1 receipt;return dh2::ui::item_update_localization_v1(item,text,powers,receipt,error);
 }
 static int invoke(void* raw,const dh2::ui::SettingsSceneRequest16V1* request,std::uint32_t* result){
  auto& s=*static_cast<LanguageDeliveryV109*>(raw);if(!request||!result)return -1;*result=0;
  // Borrow again at each native delivery; earlier prefix stores stay visible
  // if an actual callback retires/replaces the campaign.
  SourceCampaignLanguageSceneV109 now;if(!borrow_source_campaign_language_scene_v109(s.world,now,s.error)||now.manager!=s.scene.manager||now.scene!=s.scene.scene)return -1;
  using O=dh2::ui::SettingsSceneOperationV1;
  if(request->operation==O::is_player||request->operation==O::is_merchant||request->operation==O::refresh_inventory){
   SourceCampaignCharacterBorrowV62 b;if(!borrow_source_campaign_character_v62(s.world,request->identity,b,s.error))return -1;auto r=b.character;
   if(request->operation==O::is_player){bool value{};if(!r->is_player(value,s.error))return -1;*result=value;return 0;}
   if(request->operation==O::is_merchant){if(!r->properties||!r->design.ai()){s.error="Required source merchant AI row";return -1;}const auto* row=dh2::data::ai_props(*r->design.ai(),r->properties->resolved[1]);if(!row){s.error="Missing actual merchant AI row";return -1;}*result=row->type==7;return 0;}
   if(!r->inventory37c){s.error="Required SAME Character inventory37c for language refresh";return -1;}
   const auto count=r->inventory37c->items().size();for(std::size_t i=0;i<count;++i){const auto& items=r->inventory37c->items();
    if(i>=items.size()||!items[i]||!items[i]->item){s.error="Original language inventory changed during traversal";return -1;}if(!s.item(*items[i]->item))return -1;
   }return 0;
  }
  SourceCampaignCandidateBorrowV55 candidate;if(!borrow_source_campaign_candidate_v55(candidate,s.error)||candidate.actual_world!=s.world)return -1;
  if(request->operation==O::is_game_object){
   auto transport=candidate.receiver_transport_v69.lock();if(!transport){s.error="Required actual class receiver for IsGameObject";return -1;}
   const dh2::world::CanonicalObjectBorrowV1* object{};std::int32_t key{};bool found=false;
   for(bool at=s.scene.manager->source_ordered_begin_v38(key,object);at;at=s.scene.manager->source_ordered_next_v38(key,key,object))if(object&&object->identity==request->identity){found=true;break;}
   if(!found){s.error="Language object left actual canonical manager";return -1;}
   const auto loan=*object;auto services=transport->services();bool value{};
   if(!services.is_game_object||!services.is_game_object(services.context,loan,value,s.error))return -1;*result=value;return 0;
  }
  if(request->operation==O::refresh_item){std::shared_ptr<SourceWorldBorrowV61> world;
   if(!borrow_source_campaign_condition_world_v70(candidate,world,s.error)||!world->prepared_items_v88||!world->prepared_items_v88->items()){s.error="Required SAME source Item manager for localization";return -1;}
   auto* receiver=world->prepared_items_v88->items()->pool().receiver(request->identity);
   if(!receiver){s.error="Required actual ItemObject localization receiver";return -1;}
   auto* item=receiver->inventory().peek(0);return !item||s.item(*item)?0:-1; //Genuine native nullable inventory0 branch.
  }
  s.error="Unowned native localization operation";return -1;
 }
};
}
bool refresh_source_campaign_language_v109(const std::shared_ptr<void>& world,std::string& error){
 LanguageDeliveryV109 delivery;delivery.world=world;if(!borrow_source_campaign_language_scene_v109(world,delivery.scene,error))return false;
 const dh2::ui::SettingsSceneServices16V1 services{&delivery,LanguageDeliveryV109::invoke};
 const auto status=dh2_settings_v2_refresh_language_scene(delivery.scene.scene,&services);
 if(status){error=delivery.error.empty()?"Original campaign language traversal failed: "+std::to_string(status):delivery.error;return false;}error.clear();return true;
}
}
