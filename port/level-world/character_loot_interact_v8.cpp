#include "character_loot_interact_v8.hpp"
#include <cstdio>
namespace dh2::character {
bool CharacterLootInteractV8::call(LootInteractOperationV8 op,std::uintptr_t character,data::ItemInstanceV1* item,const char* key,const char* text,std::int32_t argument,bool flag,LootInteractResponseV8& out,std::string& e,std::int32_t secondary){
 if(!services_.invoke){e="Required original ItemObject Interact receiver at "+std::to_string(std::uint32_t(op));return false;}out={};return services_.invoke(services_.context,{op,object_,character,&inventory_,item,key,text,argument,flag,secondary},out,e);
}
bool CharacterLootInteractV8::interact(std::uintptr_t user,std::string& e){
 e.clear();if(running_){e="Unsupported destructive ItemObject Interact reentry";return false;}running_=true;struct Reset{bool& b;~Reset(){b=false;}}reset{running_};
 if(inventory_.items().empty())return true;
 using O=LootInteractOperationV8;LootInteractResponseV8 r;
 if(!call(O::character_cast,user,nullptr,nullptr,nullptr,0,false,r,e))return false;auto actor=r.character;if(!actor.identity)return true;auto character=actor.identity;
 if(fields_.owner3bc&&fields_.owner3bc!=character)return true;
 if(fields_.player_id3c0!=-1){if(!call(O::player_id,character,nullptr,nullptr,nullptr,0,false,r,e))return false;if(r.value==fields_.player_id3c0&&fields_.lock3b8>0)return true;}
 if(!call(O::is_player,character,nullptr,nullptr,nullptr,0,false,r,e))return false;if(!r.value)return true;
 auto* item=inventory_.items().front()->item.get();auto* row=item?inventory_.metadata(*item):nullptr;if(!row){e="Required actual Interact GetItem(0)/metadata";return false;}
 if(!call(O::saved_option,character,item,"AutoTransmute",nullptr,0,false,r,e))return false;auto option=r.value;
 const auto type=row->record.words[22];const bool equippable=row->record.words[26]!=-1;
 bool transmute=equippable&&std::uint32_t(item->powers.size())<std::uint32_t(option);
 if(equippable&&!transmute){if(!call(O::inventory_full,character,item,nullptr,nullptr,0,false,r,e))return false;if(r.value){if(!call(O::localized_text,character,item,"GAMEPLAYMENUS_INVENTORY_FULL",nullptr,0,false,r,e))return false;auto text=r.text;if(!call(O::show_text,character,item,nullptr,text.c_str(),0,false,r,e))return false;return true;}}
 if(!transmute&&type==14){if(!call(O::num_potions,character,item,nullptr,nullptr,0,false,r,e))return false;auto count=r.value;if(!call(O::potion_capacity,character,item,nullptr,nullptr,0,false,r,e))return false;if(count>=r.value)return true;}
 if(transmute){
  if(!call(O::localized_text,character,item,"ITEMS_AUTO_TRANSMUTE",nullptr,0,false,r,e))return false;auto format=r.text;
  if(!call(O::font_color,character,item,nullptr,nullptr,0,false,r,e))return false;auto color=r.value;
  if(!call(O::transmute_preview,character,item,nullptr,nullptr,0,true,r,e))return false;auto value=r.value;
  if(!call(O::transmute_format,character,item,format.c_str(),item->name.c_str(),value,false,r,e,color&0x00ffffff))return false;auto text=r.text;
  if(!call(O::show_text,character,item,nullptr,text.c_str(),0,false,r,e))return false;
  if(!call(O::transfer_one,character,item,nullptr,nullptr,0,true,r,e))return false;auto index=r.value;
  if(!call(O::transmute_index,character,nullptr,nullptr,nullptr,index,false,r,e))return false;
 }else{
  if(!call(O::font_color,character,item,nullptr,nullptr,0,false,r,e))return false;char color[16];std::snprintf(color,sizeof(color),"%x",unsigned(r.value));auto text=std::string("<font color=\"#")+color+"\">"+item->name+"</font>";
  if(!call(O::local_player_character,character,item,nullptr,nullptr,0,true,r,e))return false;if(r.identity==character&&!call(O::show_text,character,item,nullptr,text.c_str(),0,false,r,e))return false;
  if(type!=13){if(!call(O::online,character,item,nullptr,nullptr,0,false,r,e))return false;if(!r.value){if(!call(O::game_difficulty,character,item,nullptr,nullptr,0,false,r,e))return false;if(!r.value){if(!call(O::tutorial_enabled,character,item,nullptr,nullptr,0,false,r,e))return false;if(r.value){if(!call(O::tutorial_id,character,item,"cinematic_Tuto_menuInvSheet",nullptr,0,true,r,e))return false;if(r.value!=-1&&!call(O::start_tutorial,character,item,nullptr,nullptr,r.value,false,r,e))return false;}}}}
  if(!services_.debug.invoke){e="Required original isTracingItemPickedUp Debug";return false;}std::int32_t ignored{};data::LootEntryRequestV8 q{data::LootEntryOperationV8::debug_load,0x3ed454,nullptr};if(!services_.debug.invoke(services_.debug.context,q,ignored,e))return false;q.operation=data::LootEntryOperationV8::debug_query;q.caller=0x3ed474;q.key="isTracingItemPickedUp";if(!services_.debug.invoke(services_.debug.context,q,ignored,e))return false;
  if(!call(O::transfer_all,character,item,nullptr,nullptr,0,true,r,e))return false;
  if(!actor.properties||dh2_property_add(actor.properties,223,256)){e="Required same live picked-up count property223";return false;}std::int32_t count{};if(dh2_property_resolve(actor.properties,223,&count)){e="Required source picked-up count property read";return false;}
  if((count>>8)>=300){if(!call(O::is_player,character,nullptr,nullptr,nullptr,0,false,r,e))return false;if(r.value){if(!call(O::is_local_player,character,nullptr,nullptr,nullptr,0,false,r,e))return false;if(r.value&&!call(O::unlock_trophy,character,nullptr,"picked_up_300_drops",nullptr,0,false,r,e))return false;}}
 }
 if(!inventory_.items().empty())return true;
 if(!call(O::online,character,nullptr,nullptr,nullptr,0,false,r,e))return false;if(r.value){if(!call(O::controller_networked,character,nullptr,nullptr,nullptr,0,false,r,e))return false;if(r.value&&!call(O::message,character,nullptr,"CMsgControllerAction",nullptr,5,true,r,e))return false;}
 if(!call(O::pickup_sound,character,nullptr,nullptr,nullptr,fields_.audio_pickup3b6,true,r,e))return false;
 if(fields_.tooltip3c8){if(!call(O::tooltip_destroy,character,nullptr,nullptr,nullptr,0,false,r,e))return false;fields_.tooltip3c8=0;}
 if(!call(O::hide_glow,character,nullptr,nullptr,nullptr,0,false,r,e))return false;
 if(!actor.current_target14a4){e="Required same Character target14a4 borrow";return false;}if(*actor.current_target14a4==object_)*actor.current_target14a4=0;
 if(!call(O::loot_fx,character,nullptr,"loot_orb_fx",nullptr,0,false,r,e)||!call(O::despawn,character,nullptr,nullptr,nullptr,0,false,r,e)||!call(O::player_id,character,nullptr,nullptr,nullptr,0,false,r,e))return false;
 return call(O::increment_stat,character,nullptr,nullptr,nullptr,r.value,false,r,e,4);
}
}
