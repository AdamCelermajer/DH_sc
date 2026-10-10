#include "loot_creation_v8.hpp"
#include <cstring>
namespace dh2::data {
bool LootCreationV8::add(std::int32_t table,std::int32_t value_bonus,std::int32_t power_bonus,std::int32_t fixed,bool bypass,bool all,const LootCreationServicesV8& s,std::string& e){
 e.clear();if(running_){e="Unsupported destructive AddLoot reentry";return false;}running_=true;struct Reset{bool& b;~Reset(){b=false;}}reset{running_};
 if(pending_){e="Required cleanup of retained failed AddLoot item before retry";return false;}
 if(!tables_){e="Required actual loot tables";return false;}
 if(table<0||std::size_t(table)>=tables_.loots().size())return true;
 auto debug=[&](std::uint32_t caller,const char* key,std::int32_t& value){if(!s.entry.invoke){e="Required original AddLoot Debug provider";return false;}LootEntryRequestV8 q{LootEntryOperationV8::debug_load,caller,nullptr};if(!s.entry.invoke(s.entry.context,q,value,e))return false;q.operation=LootEntryOperationV8::debug_query;q.key=key;return s.entry.invoke(s.entry.context,q,value,e);};
 std::int32_t minimal{};if(!debug(0x4040fc,"MP_MinimalRandoms",minimal))return false;if(minimal)return true;
 LootEntrySelectionV8 trace(random_,s.entry);
 for(auto c:{0x404144u,0x404174u})if(!trace.trace(c,e))return false;
 LootTableSelectionV8 select(tables_,powers_,random_,s.entry);std::vector<const LootEntry32V2*> selected;if(!select.select(table,selected,e))return false;
 if(selected.empty()){for(auto c:{0x4041dcu,0x40420cu})if(!trace.trace(c,e))return false;return true;}
 for(auto c:{0x404248u,0x404278u})if(!trace.trace(c,e))return false;
 LootItemSelectionV8 expand(tables_,random_,s.entry);std::vector<LootItemInfoV8> infos;if(!expand.expand(selected,all,infos,e))return false;
 if(infos.empty()){for(auto c:{0x404558u,0x404588u,0x4041dcu,0x40420cu})if(!trace.trace(c,e))return false;return true;}
 for(auto c:{0x4042e0u,0x404310u})if(!trace.trace(c,e))return false;
 LootPowerCreationV7 power(powers_,random_);
 auto query=[&](LootCreationOperationV8 op,std::uint32_t caller,LootCreationResponseV8& out){if(!s.query){e="Required original AddLoot application/player provider";return false;}return s.query(s.context,{op,caller},out,e);};
 for(auto& info:infos){auto type=info.item->record.words[4];std::int32_t duplicates=1;
  if(type==2||type==3){LootCreationResponseV8 count;if(!query(LootCreationOperationV8::player_count,0x4043a8,count))return false;duplicates=count.value;}
  for(std::int32_t i=0;i<duplicates;++i){LootCreationResponseV8 level;if(!query(LootCreationOperationV8::current_level,0x4043cc,level))return false;std::int32_t difficulty{};
   if(level.present){if(!query(LootCreationOperationV8::current_level,0x4043dc,level))return false;if(!level.present){e="Required still-live source current level after second query";return false;}difficulty=level.value;}
   auto id=std::int32_t(info.id);const auto& items=tables_.items();
   if(id<0||std::size_t(id)>=items.rows.size()){e="Required actual signed AddLoot item identity";return false;}
   std::int32_t selected_id;if(!select_loot_item_variant_v8(items,id,difficulty,bypass,selected_id,e))return false;
   id=selected_id;info.id=std::int16_t(id);info.item=&items.rows[std::size_t(id)];
   if(!s.create){e="Required original ItemInstance constructor provider";return false;}
   if(!s.create(s.context,id,pending_,e))return false;
   if(!pending_||pending_->id!=id){e="Source ItemInstance constructor did not produce actual requested item";return false;}
   std::int8_t quantity;std::memcpy(&quantity,&info.quantity,1);if(quantity==-2){quantity=99;info.quantity=99;}
   if(quantity<0){e="Required original negative SetQty assertion continuation";return false;}
   pending_->quantity=std::uint16_t(quantity);
   if(!power.add_powers(*info.entry,*pending_,power_bonus,fixed,difficulty,s.power,e)||!loot_item_value_v7(*pending_,items,definitions_,random_,value_bonus,s.text,e))return false;
   if(!s.store){e="Required actual source loot destination inventory";return false;}
   if(!s.store(s.context,pending_,e))return false;
   if(pending_){e="Source AddItemInstance reported success without consuming actual item";return false;}
  }
 }return true;
}
}
