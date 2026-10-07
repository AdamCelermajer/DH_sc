#include "fresh_inventory_owned_v4.hpp"
#include <cstring>
namespace dh2::data {namespace {
struct ReaderV50 {Bytes bytes;std::size_t at{};
 bool raw(std::size_t n,const std::uint8_t*& p){if(at>bytes.size||n>bytes.size-at)return false;p=bytes.data+at;at+=n;return true;}
 bool word(std::int32_t& value){const std::uint8_t* p;if(!raw(4,p))return false;const auto u=std::uint32_t(p[0])|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;std::memcpy(&value,&u,4);return true;}
 bool text(std::string& value){std::int32_t n;if(!word(n))return false;if(n<=0){value.clear();return true;}const std::uint8_t* p;if(!raw(std::uint32_t(n),p)||p[n-1])return false;value.assign(reinterpret_cast<const char*>(p),std::uint32_t(n)-1);return true;}
};
std::int32_t power_id_v50(const std::vector<std::string>& names,const std::string& name){for(unsigned i=0;i<names.size();++i)if(!std::strcmp(names[i].c_str(),name.c_str()))return std::int32_t(i);return -1;}
}
bool FreshInventoryOwnedV4::load_saved_section_v50(Bytes bytes,const std::vector<std::string>& powers,
 const OwnedInventoryServicesV4& services,InventoryLoadReceiptV1& receipt,std::string& error){
 if(loading_saved_v50_||!character_||(!bytes.data&&bytes.size)||!services.invoke||!mutation_allowed(error)){
  if(error.empty())error="Required nonreentrant SAME saved inventory receiver/services";return false;
 }
 loading_saved_v50_=true;struct End{bool& active;~End(){active=false;}}end{loading_saved_v50_};receipt={};ReaderV50 read{bytes};
 auto failure=[&](const char* e){receipt.consumed=read.at;if(error.empty())error=e;return false;};
 std::int32_t gold,selection,count;if(!read.word(gold)||!read.word(selection)||!read.word(count))return failure("Truncated source GEAR header");
 if(!set_gold(gold,services,error))return failure("Required source SetGold during GEAR load");
 selected_=std::uint8_t(selection);
 if(selected_>=2)return failure("Saved equipment byte outside native two-set domain");
 if(count<0)return failure("Saved unsigned item count outside bounded source domain");
 for(std::int32_t row=0;row<count;++row){
  std::string identifier;std::int32_t first,second,quantity,value,power_count;const std::uint8_t* identified;
  if(!read.text(identifier)||!read.word(first)||!read.word(second)||!read.word(quantity)||!read.word(value)||!read.raw(1,identified)||!read.word(power_count))return failure("Truncated source saved item");
  const auto id=item_id(table(),identifier);if(id<0){error="Missing actual saved ItemTable identifier "+identifier;return failure("");}
  std::unique_ptr<ItemInstanceV1> current;
  if(!create_item(id,std::uint32_t(quantity),current,services,error))return failure("Required source saved ItemInstance C1");
  current->value=value;OwnedInventoryResponseV4 response;
  if(!deliver(services,OwnedInventoryOperationV4::update_name,0x3fbc58,current.get(),nullptr,0,0,response,error))return failure("Required source SetValue name refresh");
  current->identified=std::uint8_t(*identified!=0);
  if(power_count<0)return failure("Saved unsigned power count outside bounded source domain");
  for(std::int32_t p=0;p<power_count;++p){std::string name;if(!read.text(name))return failure("Truncated saved power name");
   const auto power=power_id_v50(powers,name);
   if(!deliver(services,OwnedInventoryOperationV4::add_power,0x46a5f4,current.get(),nullptr,power,UINT32_MAX,response,error))return failure("Required source saved AddPower");
   ++receipt.powers_read;
  }
  ++receipt.items_read;std::int32_t index=-1;
  // Original46a60c/10 passes force=true, convertGold=true to3ff5d4.
  if(!add_item(current,true,true,index,services,error))return failure("Required source saved AddItemInstance");
  if(index>=0)++receipt.items_retained;
  const std::int32_t slots[]{first,second};
  for(unsigned set=0;set<2;++set){if(slots[set]==-1)continue;
   const auto previous=selected_;selected_=std::uint8_t(set);
   // Original46a63c and46a68c pass forced=true to400634.
   const bool equipped=equip_to_slot(std::uint32_t(slots[set]),std::uint32_t(index),true,services,error);
   selected_=previous;if(!equipped)return failure("Required original saved equipment restoration");
   ++receipt.equipment_deliveries;
  }
 }
 receipt.consumed=read.at;receipt.completed=true;error.clear();return true;
}
}
