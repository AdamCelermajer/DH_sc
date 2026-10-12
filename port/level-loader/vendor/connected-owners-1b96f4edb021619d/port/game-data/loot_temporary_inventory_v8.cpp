#include "loot_temporary_inventory_v8.hpp"
namespace dh2::data {
bool LootTemporaryInventoryV8::create(std::int32_t id,std::unique_ptr<ItemInstanceV1>& out,const ItemTextServicesV5& text,std::string& e){
 e.clear();if(running_||transferred_slot_||out||!tables_||id<0||std::size_t(id)>=tables_.items().rows.size()){e="Malformed source temporary inventory ItemInstance creation";return false;}
 running_=true;struct Reset{bool& b;~Reset(){b=false;}}reset{running_};
 out=std::make_unique<ItemInstanceV1>();out->id=id;out->quantity=1;
 return item_update_name_v5(*out,text,e)&&item_update_stats_v5(*out,text,e)&&item_update_requirements_v5(*out,text,e);
}
bool LootTemporaryInventoryV8::store(std::unique_ptr<ItemInstanceV1>& incoming,const LootEntryServicesV8& s,void* context,bool(*notify)(void*,LootTemporaryInventoryV8&,ItemInstanceV1&,std::string&),std::string& e){
 e.clear();if(running_||transferred_slot_||!incoming||!tables_||incoming->id<0||std::size_t(incoming->id)>=tables_.items().rows.size()){e="Malformed source temporary inventory forced insertion";return false;}
 running_=true;struct Reset{bool& b;~Reset(){b=false;}}reset{running_};
 auto* item=incoming.get();if(!potion_&&item_type(tables_.items().rows[std::size_t(item->id)])==14)potion_=item;
 auto slot=std::make_unique<ItemSlotV1>();slot->item=std::move(incoming);items_.push_back(std::move(slot));
 if(!s.invoke){e="Required original InventoryFull Debug after actual loot storage";return false;}
 std::int32_t infinite{};LootEntryRequestV8 q{LootEntryOperationV8::debug_load,0x3fe364,nullptr};
 if(!s.invoke(s.context,q,infinite,e))return false;
 q.operation=LootEntryOperationV8::debug_query;q.caller=0x3fe3a8;q.key="InfiniteInventory";
 if(!s.invoke(s.context,q,infinite,e))return false;
 if(!infinite&&items_.size()>99){if(!notify){e="Required original NULL-character inventory full-notification tail";return false;}return notify(context,*this,*item,e);}
 return true;
}
bool LootTemporaryInventoryV8::release_spawned(ItemInstanceV1& item,std::unique_ptr<ItemInstanceV1>& out,std::string& e){
 e.clear();if(running_||out||transferred_slot_){e="Malformed source spawned-item ownership transfer or retained failed transfer prefix";return false;}
 for(auto it=items_.begin();it!=items_.end();++it)if((*it)->item.get()==&item){out=std::move((*it)->item);if(potion_==&item)potion_=nullptr;items_.erase(it);return true;}
 e="Spawned item absent from actual temporary inventory";return false;
}
ItemInstanceV1* LootTemporaryInventoryV8::peek(std::uint32_t index)const noexcept{
 if(index>=items_.size())return nullptr;auto* slot=items_[index].get();return slot==transferred_slot_?transferred_item_:slot->item.get();
}
bool LootTemporaryInventoryV8::transfer(std::uint32_t index,bool force,bool convert_gold,void* context,bool(*add)(void*,std::unique_ptr<ItemInstanceV1>&,bool,bool,std::int32_t&,std::string&),std::int32_t& result,std::string& e){
 e.clear();if(running_||transferred_slot_||index>=items_.size()){e="Unsupported transfer reentry, retained failure prefix or original index assertion";return false;}
 auto* slot=items_[index].get();auto* item=slot->item.get();if(!item){e="Required actual source transfer item";return false;}
 if(item->signed_quantity()==0){result=0;return true;}
 if(item->signed_quantity()<0){e="Required original negative source transfer quantity continuation";return false;}
 if(!add){e="Required actual destination AddItemInstance";return false;}
 running_=true;struct Reset{bool& b;~Reset(){b=false;}}reset{running_};transferred_slot_=slot;transferred_item_=item;if(potion_==item)potion_=nullptr;
 if(!add(context,slot->item,force,convert_gold,result,e))return false;
 if(slot->item){e="Destination AddItemInstance reported success without consuming actual transferred item";return false;}
 // No read of item after AddItemInstance: gold conversion/stack merge can
 // genuinely destroy it. The source item ID needed by async quest delivery
 // must be captured by the caller before invoking this transfer.
 transferred_slot_=nullptr;transferred_item_=nullptr;items_.erase(items_.begin()+index);return true;
}
}
