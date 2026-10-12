#include "item_inventory_v1.hpp"
#include "player_savegame_v1.hpp"
#include <cstring>
namespace dh2::data {
namespace {
struct Reader {Bytes b;std::size_t at{};bool raw(std::size_t n,const std::uint8_t*& p){if(at>b.size||n>b.size-at)return false;p=b.data+at;at+=n;return true;}
 bool word(std::int32_t& x){const std::uint8_t* p;if(!raw(4,p))return false;auto u=std::uint32_t(p[0])|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;std::memcpy(&x,&u,4);return true;}
 bool text(std::string& x){std::int32_t n;if(!word(n))return false;if(n<=0){x.clear();return true;}const std::uint8_t* p;if(!raw(std::size_t(n),p)||p[n-1])return false;x.assign(reinterpret_cast<const char*>(p),std::size_t(n)-1);return true;}
};
std::int32_t lookup(const std::vector<std::string>& a,const std::string& name){for(std::size_t i=0;i<a.size();++i)if(!std::strcmp(a[i].c_str(),name.c_str()))return static_cast<std::int32_t>(i);return -1;}
std::int32_t wrapping_add(std::int32_t a,std::int32_t b){auto u=std::uint32_t(a)+std::uint32_t(b);std::int32_t x;std::memcpy(&x,&u,4);return x;}
}
std::int32_t ItemInstanceV1::signed_quantity()const noexcept{return dh2_inventory_v1_quantity(quantity);}
ItemInventoryV1::ItemInventoryV1(const ItemTable& table):table_(std::make_shared<const ItemTable>(table)){}
bool ItemInventoryV1::deliver(const InventoryServicesV1& s,InventoryOperationV1 op,ItemInstanceV1* item,std::int32_t arg,std::uint32_t index,std::uint32_t set,InventoryResponseV1& response,std::string& e){
 if(!s.invoke){e="required original inventory service unavailable";return false;}response={};return s.invoke(s.context,*this,{op,item,arg,index,set},response,e);
}
bool ItemInventoryV1::load_section(Bytes bytes,const std::vector<std::string>& powers,const InventoryServicesV1& s,InventoryLoadReceiptV1& receipt,std::string& e){
 if(load_active_||!character_||(!bytes.data&&bytes.size)||!s.invoke){e="invalid inventory load owner/service or unsupported reentry";return false;}
 load_active_=true;struct Guard{bool& active;~Guard(){active=false;}}guard{load_active_};receipt={};Reader r{bytes};InventoryResponseV1 response;
 auto failure=[&](const char* message){receipt.consumed=r.at;if(e.empty())e=message;return false;};
 std::int32_t raw_gold,selection,count;if(!r.word(raw_gold)||!r.word(selection)||!r.word(count))return failure("truncated inventory header");
 if(raw_gold<0){e="negative gold requires original Debug policy";return failure("");}
 gold_=raw_gold>gold_limit_?gold_limit_:raw_gold;
 if(!deliver(s,InventoryOperationV1::gold_notifications,nullptr,raw_gold,0,0,response,e))return failure("required gold effects failed");
 selected_equipment_=static_cast<std::int8_t>(static_cast<std::uint8_t>(selection));
 if(count<0){e="unsigned inventory item count exceeds safe native span";return failure("");}
 for(std::int32_t n=0;n<count;++n){std::string identifier;std::int32_t first,second,quantity,value,power_count;const std::uint8_t* identified;
  if(!r.text(identifier)||!r.word(first)||!r.word(second)||!r.word(quantity)||!r.word(value)||!r.raw(1,identified)||!r.word(power_count))return failure("truncated inventory item");
  auto id=lookup(table_->identifiers,identifier);auto record=item(*table_,id);if(!record){e="missing original ItemTable identifier: "+identifier;return failure("");}
  auto instance=std::make_unique<ItemInstanceV1>();instance->id=id;instance->quantity=static_cast<std::uint16_t>(quantity);auto* current=instance.get();
  for(auto op:{InventoryOperationV1::update_name,InventoryOperationV1::update_stats,InventoryOperationV1::update_requirements})if(!deliver(s,op,current,0,0,0,response,e))return failure("required ItemInstance initialization failed");
  current->value=value;if(!deliver(s,InventoryOperationV1::update_name,current,0,0,0,response,e))return failure("required SetValue name refresh failed");current->identified=std::uint8_t(*identified!=0);
  if(power_count<0){e="unsigned power count exceeds safe native span";return failure("");}
  for(std::int32_t p=0;p<power_count;++p){std::string name;if(!r.text(name))return failure("truncated power name");auto power=lookup(powers,name);
   if(!deliver(s,InventoryOperationV1::add_power,current,power,0,0,response,e))return failure("required ItemInstance.AddPower failed");++receipt.powers_read;
  }
  ++receipt.items_read;std::uint32_t index=UINT32_MAX;auto type=item_type(*record);
  if((potion_capacity_==0&&type==14)||type==13){
   if(type==13&&!(potion_capacity_==0&&type==14)){auto next=wrapping_add(gold_,current->value);if(next<0){e="wrapped AddGold requires original Debug policy";return failure("");}gold_=next>gold_limit_?gold_limit_:next;
    if(!deliver(s,InventoryOperationV1::gold_notifications,nullptr,next,0,0,response,e))return failure("required AddGold effects failed");}
   if(!deliver(s,InventoryOperationV1::destroy_item,current,0,0,0,response,e))return failure("required source item destruction failed");
  }else{
   if(!potion_&&type==14)potion_=current;
   auto slot=std::make_unique<ItemSlotV1>();slot->item=std::move(instance);items_.push_back(std::move(slot));index=static_cast<std::uint32_t>(items_.size()-1);++receipt.items_retained;
   if(!deliver(s,InventoryOperationV1::inventory_full,current,0,index,0,response,e))return failure("required IsInventoryFull failed");
   if(response.flag&&!deliver(s,InventoryOperationV1::full_notifications,current,0,index,0,response,e))return failure("required inventory-full effects failed");
  }
  for(std::uint32_t set=0;set<2;++set){auto selected=set?second:first;if(selected==-1)continue;
   auto old=selected_equipment_;selected_equipment_=static_cast<std::int8_t>(set);
   if(!deliver(s,InventoryOperationV1::equip_item,index<items_.size()?items_[index]->item.get():nullptr,selected,index,set,response,e)){selected_equipment_=old;return failure("required equipment restore failed");}
   selected_equipment_=old;++receipt.equipment_deliveries;
  }
 }
 receipt.consumed=r.at;receipt.completed=true;e.clear();return true;
}
bool ItemInventoryV1::record_delivered_equipment(std::uint32_t set,std::uint32_t slot,std::uint32_t index,std::string& e){if(set>=2||slot>=9||index>=items_.size()){e="invalid delivered equipment identity";return false;}
 auto* value=items_[index].get();equipment_[set][slot]=value;value->equipment_set=static_cast<std::int8_t>(set);value->equipment_slot=static_cast<std::int8_t>(slot);e.clear();return true;}
std::int32_t ItemInventoryV1::num_potions()const noexcept{return potion_?potion_->signed_quantity():0;}
std::int32_t ItemInventoryV1::current_equipment(std::int32_t x)const noexcept{return dh2_inventory_v1_current_equipment(static_cast<std::uint8_t>(selected_equipment_),x);}
void ItemInventoryV1::swap_equipment()noexcept{selected_equipment_=static_cast<std::int8_t>(dh2_inventory_v1_swap_equipment(static_cast<std::uint8_t>(selected_equipment_)));}
bool ItemInventoryV1::set_potion_quantity(std::int32_t quantity,const InventoryServicesV1& s,std::string& e){
 if(load_active_||!character_){e="invalid potion owner or unsupported reentry";return false;}
 if(quantity<0){e="negative quantity requires original Debug policy";return false;}
 load_active_=true;struct Guard{bool& a;~Guard(){a=false;}}guard{load_active_};InventoryResponseV1 response;
 if(potion_){if(quantity){potion_->quantity=static_cast<std::uint16_t>(quantity);e.clear();return true;}
  std::size_t index=0;while(index<items_.size()&&items_[index]->item.get()!=potion_)++index;
  if(index==items_.size()){e="invalid owned potion identity";return false;}for(auto& set:equipment_)for(auto ptr:set)if(ptr==items_[index].get()){e="source potion removal would leave equipped dangling identity";return false;}
  auto target=potion_;potion_=nullptr; // Original clears before destructor.
  if(!deliver(s,InventoryOperationV1::destroy_item,target,0,0,0,response,e))return false;
  items_.erase(items_.begin()+static_cast<std::ptrdiff_t>(index));e.clear();return true;
 }
 auto id=item_id(*table_,"Potion0");if(!item(*table_,id)){e="actual Potion0 ItemTable row unavailable";return false;}
 auto instance=std::make_unique<ItemInstanceV1>();instance->id=id;instance->quantity=static_cast<std::uint16_t>(quantity);auto* current=instance.get();
 for(auto op:{InventoryOperationV1::update_name,InventoryOperationV1::update_stats,InventoryOperationV1::update_requirements})if(!deliver(s,op,current,0,0,0,response,e))return false;
 auto type=item_type(*item(*table_,id));if(potion_capacity_==0&&type==14)return deliver(s,InventoryOperationV1::destroy_item,current,0,0,0,response,e);
 if(type==14)potion_=current;auto slot=std::make_unique<ItemSlotV1>();slot->item=std::move(instance);items_.push_back(std::move(slot));auto index=static_cast<std::uint32_t>(items_.size()-1);
 if(!deliver(s,InventoryOperationV1::inventory_full,current,0,index,0,response,e))return false;
 if(response.flag&&!deliver(s,InventoryOperationV1::full_notifications,current,0,index,0,response,e))return false;e.clear();return true;
}
bool ItemInventoryV1::remove_one_potion(const InventoryServicesV1& s,std::string& e){if(load_active_){e="unsupported potion reentry";return false;}if(!potion_){e.clear();return true;}auto n=potion_->signed_quantity();return set_potion_quantity(n>1?n-1:0,s,e);}
}
