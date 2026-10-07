#include "loot_source_fields_v47.hpp"
#include <stdexcept>
#include <cstring>
#include <cstdio>
namespace dh2::character {
bool LootPlayerFieldAssociationV47::source_fresh_save_store_then_character_v60(
 std::shared_ptr<data::PlayerSavegameV1> save,std::string& e){
 if(!character_||save14e8_||save_||!save||save->character()){
  e="Required fresh Character14e8 NULL and just-constructed unbound Save C1";return false;
 }
 save_=std::move(save);save14e8_=reinterpret_cast<std::uintptr_t>(save_.get()); //3b36d8.
 save_->set_character(character_); // Tail SG_SetCharacter3bb754 after publication.
 e.clear();return true;
}
namespace {
bool require(bool value,const char* name,std::string& e){
 if(value)return true;e=std::string("Required actual loot V47 ")+name;return false;
}
}
LootPlayerFieldAssociationV47::LootPlayerFieldAssociationV47(std::uintptr_t id):character_(id){
 if(!id)throw std::invalid_argument("Required existing Character identity for source14e8 fields");
}
bool LootPlayerFieldAssociationV47::source_save_store_3b36d8(
 std::shared_ptr<data::PlayerSavegameV1> same,std::string& e){
 if(!require(same&&same->character()==character_,"constructed SAME Save C1/SetCharacter prefix",e))return false;
 if(save14e8_&&(!save_||save_.get()!=same.get())){
  e="Source14e8 already owns a different Save; no replacement/replay";return false;
 }
 save_=std::move(same);save14e8_=reinterpret_cast<std::uintptr_t>(save_.get());return true;
}
bool LootPlayerFieldAssociationV47::borrow_save(std::shared_ptr<void> lease,
 LootCharacterSaveBorrowV44& out,std::string& e)const{
 if(!require(bool(lease),"Character14e8 field lifetime",e))return false;
 LootCharacterSaveBorrowV44 next;next.character_lease=std::move(lease);
 if(save14e8_){
  if(!require(save_&&save_->character()==character_&&
     save14e8_==reinterpret_cast<std::uintptr_t>(save_.get()),"retained SAME Save slot identity",e))return false;
  next.save=save_.get();next.save_lease=save_;
 }
 out=std::move(next);return true;
}
bool loot_state_info_borrow_v47(CharacterStateOwner& owner,std::uintptr_t id,
 const std::int32_t*& out,std::string& e){
 auto& m=owner.machine();auto& f=owner.native_fsm();
 if(!require(id&&m.fsm==&f&&f.character==id&&f.state==&owner.state()&&
    !f.reserved&&!m.reserved&&f.current_present<=1&&m.state_count<=20&&
    (m.states||!m.state_count)&&m.current_index>=-1&&
    m.current_index<static_cast<std::int32_t>(m.state_count)&&
    ((m.current_index>=0)==bool(f.current_present)),"SAME registered CharStateMachine/StateInfo",e))return false;
 if(m.current_index<0){out=nullptr;return true;}
 const auto& info=m.states[m.current_index];
 if(!require(!info.reserved&&info.id==f.state->current,"current StateInfo id coherence",e))return false;
 out=&info.id;return true;
}
bool loot_character_is_player_v47(const data::AiTables& table,const data::PropertyView& properties,
 const std::string& name,bool& out,std::string& e){
 if(!require(properties.resolved!=nullptr,"GetCharAI cached property1",e))return false;
 const auto* ai=data::ai_props(table,properties.resolved[1]);
 if(!require(ai!=nullptr,"GetCharAI source fallback8 row",e))return false;
 const auto type=ai->type;
 out=type?type==1:std::strstr(name.c_str(),"PlayerCharacter")==name.c_str();return true;
}
LootPickupSourceLeavesV47::LootPickupSourceLeavesV47(LootPickupLeavesServicesV47 s):services_(std::move(s)){
 if(!services_.application_lease)throw std::invalid_argument("Required SAME pickup leaf Application lifetime");
}
bool LootPickupSourceLeavesV47::actor(std::uintptr_t id,LootPickupActorV23& out,std::string& e){
 if(!require(id&&bool(services_.actor),"pickup actor producer",e)||
    !services_.actor(services_.context,id,out,e))return false;
 return require(out.identity==id&&out.receiver_lease&&out.properties&&out.object_of_interest14a4,
  "SAME pickup actor/property/OOI lifetime",e);
}
bool LootPickupSourceLeavesV47::constant(const char* group,const char* key,std::int32_t& out,std::string& e){
 return world_loot_design_constant_v44(services_.design,group,key,out,e);
}
bool LootPickupSourceLeavesV47::route(const LootInteractRequestV8& q,LootInteractResponseV8& out,
 bool& handled,std::string& e){
 using O=LootInteractOperationV8;handled=true;
 switch(q.operation){
 case O::is_player:
 case O::is_local_player:{
  const auto callback=q.operation==O::is_player?services_.named_is_player:services_.is_local_player;
  if(!require(bool(callback),"source IsPlayer/IsLocalPlayer receiver",e))return false;
  bool value{};if(!callback(services_.context,q.character,value,e))return false;out.value=value;return true;
 }
 case O::online:{
  if(!require(bool(services_.online),"same online byte5 producer",e))return false;
  bool value{};if(!services_.online(services_.context,value,e))return false;out.value=value;return true;
 }
 case O::game_difficulty:{
  if(!require(bool(services_.save_fields),"Character14e8 source slot",e))return false;
  LootCharacterSaveBorrowV44 fields;const std::uintptr_t* slot{};
  if(!services_.save_fields(services_.context,q.character,fields,slot,e))return false;
  if(!require(fields.character_lease&&slot,"actual Save slot lifetime",e))return false;
  if(*slot&&(!fields.save||!fields.save_lease||reinterpret_cast<std::uintptr_t>(fields.save)!=*slot)){
   e="Source SG_GetGameDifficulty slot differs from retained Save";return false;
  }
  return player::character_game_difficulty_v29(
   {fields.character_lease,q.character,slot,services_.difficulty_global},out.value,e);
 }
 case O::tutorial_enabled:
  if(!require(services_.settings!=nullptr,"SAME Application+4c settings receiver",e))return false;
  // Original Interact3ed9dc reads byte2a; source tutorial block starts29.
  out.value=services_.settings->tutorials()[1]!=0;return true;
 case O::localized_text:{
  if(!require(services_.text&&q.key,"SAME StringManager source cache/key",e))return false;
  ui::LocalizationResult text;
  if(!services_.text->native_string(q.key,services_.text_environment.localization,text,e))return false;
  out.text=std::move(text.text);return true;
 }
 case O::transmute_preview:{
  LootPickupActorV23 picker;if(!actor(q.character,picker,e))return false;
  if(!require(q.item!=nullptr,"actual ItemInstance transmute preview",e))return false;
  std::int32_t multiplier{};if(!constant("CharacterDesign","TransmuteMultiplier",multiplier,e))return false;
  out.value=ui::character_menu_transmute_value_v1(q.item->value,picker.properties->resolved[197],multiplier);return true;
 }
 case O::transmute_format:{
  if(!require(q.item&&services_.item_text.invoke,"SAME ItemTextOwner/StringManager varargs",e))return false;
  char color[16]{};std::snprintf(color,sizeof color,"%x",unsigned(q.secondary)&0x00ffffffu);
  const data::ItemTextArgumentV5 arguments[3]{{0,0,color},{0,0,q.text},{0,q.argument,nullptr}};
  const data::ItemTextRequestV5 request{data::ItemTextOperationV5::parse_varargs,0,nullptr,nullptr,q.key,arguments,3};
  data::ItemTextResponseV5 response;
  return services_.item_text.invoke(services_.item_text.context,*q.item,request,response,out.text,e);
 }
 case O::transmute_index:{
  if(!require(bool(services_.bind_item_actions),"SAME Gear transmute source graph",e))return false;
  ui::CharacterMenuItemActionsGraphV1 graph;
  if(!services_.bind_item_actions(services_.context,q.character,graph,e))return false;
  // Source helper itself validates SAME property/inventory backing and
  // executes quantity/destruction/gold/trophy/Skin; no direct slot mutation.
  ui::CharacterMenuItemActionsV1 action(std::move(graph));std::int32_t value{};
  return action.transmute(static_cast<std::uint32_t>(q.argument),false,value,e);
 }
 case O::unlock_trophy:
  if(!require(q.key&&bool(services_.achievement),"named trophy source receiver",e))return false;
  return services_.achievement(services_.context,q.character,q.key,e);
 default:handled=false;return true;
 }
}
}
