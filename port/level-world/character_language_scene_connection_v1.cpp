#include "character_language_scene_connection_v1.hpp"
#include "../engine-ui/settings_language_scene_v2.hpp"
#include <stdexcept>
namespace dh2::character {
CharacterLanguageSceneConnectionV1::CharacterLanguageSceneConnectionV1(CharacterLanguageSceneServicesV1 services):services_(std::move(services)){if(!services_.owner||!services_.scene||!services_.actors||!services_.ai_types)throw std::invalid_argument("Language refresh requires actual complete World scene and AI types");}
bool CharacterLanguageSceneConnectionV1::inventory(std::uintptr_t id){
 player::PlayerEquipmentRenderOwnerV1* gear=nullptr;
 if(!services_.gear||!services_.gear(id,gear,error_))return false;
 if(!gear){if(!services_.other_inventory){error_="Required real merchant/non-player inventory localization unavailable";return false;}return services_.other_inventory(id,error_);}
 const auto* inventory=gear->inventory();if(!inventory||inventory->character()!=id){error_="Language refresh requires SAME Character Gear inventory";return false;}
 data::LootTablesV2::Borrow loot;data::ItemPowerTablesV5::Borrow tables;data::ItemTextServicesV5 text;data::LootRandom8V2* rng=nullptr;
 if(!gear->loot_sources_v8(loot,tables,text,rng,error_))return false;
 const auto count=inventory->items().size();
 for(std::size_t i=0;i<count;++i){if(i>=inventory->items().size()||!inventory->items()[i]||!inventory->items()[i]->item){error_="Original inventory item lifetime lost during language refresh";return false;}
  ui::ItemLanguageReceiptV1 receipt;if(!ui::item_update_localization_v1(*inventory->items()[i]->item,text,services_.powers,receipt,error_))return false;
 }
 return true;
}
int CharacterLanguageSceneConnectionV1::invoke(void* context,const ui::SettingsSceneRequest16V1* request,std::uint32_t* result){
 auto& self=*static_cast<CharacterLanguageSceneConnectionV1*>(context);if(!request||!result)return -1;*result=0;
 switch(request->operation){
 case ui::SettingsSceneOperationV1::is_player:
 case ui::SettingsSceneOperationV1::is_merchant:{
  skills::WorldTargetActorBorrowV1 actor{};if(self.services_.actors->actor(request->identity,&actor)){self.error_="Actual language Character absent from SAME World";return -1;}
  std::int32_t value=0;
  if(request->operation==ui::SettingsSceneOperationV1::is_player){
   if(skills::dh2_character_skill_target_query_v6(&value,target_providers::is_player,actor.character,nullptr,self.services_.ai_types,self.services_.target_services)){self.error_="Source IsPlayer delivery failed";return -1;}
  }else {
   if(skills::dh2_character_skill_target_query_v6(&value,target_providers::char_type,actor.character,nullptr,self.services_.ai_types,self.services_.target_services)){self.error_="Required actual GetCharType AI row unavailable";return -1;}value=value==7;
  }
  *result=value!=0;return 0;
 }
 case ui::SettingsSceneOperationV1::refresh_inventory:return self.inventory(request->identity)?0:-1;
 case ui::SettingsSceneOperationV1::is_game_object:{bool value=false;if(!self.services_.is_game_object||!self.services_.is_game_object(request->identity,value,self.error_))return -1;*result=value;return 0;}
 case ui::SettingsSceneOperationV1::refresh_item:return self.services_.item_object&&self.services_.item_object(request->identity,self.error_)?0:-1;
 }
 return -1;
}
bool CharacterLanguageSceneConnectionV1::refresh(std::string& error){error_.clear();ui::SettingsSceneServices16V1 callbacks{this,invoke};const auto status=dh2_settings_v2_refresh_language_scene(services_.scene,&callbacks);if(status){if(error_.empty())error_="Original complete language scene failed status="+std::to_string(status);error=error_;return false;}error.clear();return true;}
}
