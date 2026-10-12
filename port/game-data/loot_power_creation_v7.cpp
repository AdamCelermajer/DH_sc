#include "loot_power_creation_v7.hpp"
#include <cstring>
#include <set>
#include <stdexcept>

namespace {
bool span(const void* p,std::size_t n,std::size_t alignment){auto x=reinterpret_cast<std::uintptr_t>(p);return p&&x%alignment==0&&n<=UINTPTR_MAX-x;}
bool overlap(const void* a,std::size_t n,const void* b,std::size_t k){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<y+k&&y<x+n;}
std::int32_t signed_word(std::uint32_t x){std::int32_t y;std::memcpy(&y,&x,4);return y;}
std::int32_t asr8(std::int32_t x){auto w=std::uint32_t(x);return signed_word((w>>8)|(x<0?0xff000000u:0));}
std::int32_t truncate(float x){if(x>=2147483648.0f)return INT32_MAX;if(x<=-2147483648.0f)return INT32_MIN;return static_cast<std::int32_t>(x);}
template<class Row>bool valid_selection(std::int32_t* out,dh2::data::LootRandom8V2* random,const Row* rows,std::uint32_t count){
 if(!span(out,4,4)||!span(random,sizeof(*random),4)||count>65536||overlap(out,4,random,sizeof(*random)))return false;
 if(count&&(!span(rows,std::size_t(count)*sizeof(Row),alignof(Row))||overlap(out,4,rows,std::size_t(count)*sizeof(Row))||overlap(random,sizeof(*random),rows,std::size_t(count)*sizeof(Row))))return false;
 return true;
}
}
extern "C" int dh2_loot_power_select_v7(std::uint32_t* out,dh2::data::LootRandom8V2* random,const dh2::data::LootPowerChoiceV7* rows,std::uint32_t count) noexcept {
 if(!valid_selection(reinterpret_cast<std::int32_t*>(out),random,rows,count))return -1;
 std::uint32_t total=0;for(unsigned i=0;i<count;++i)total+=std::uint32_t(std::int32_t(rows[i].probability));
 if(!count||!total)return -2;
 std::int32_t draw;if(dh2_loot_v2_random(random,signed_word(total),&draw))return -1;
 auto remainder=std::uint32_t(draw);
 for(unsigned i=0;i<count;++i){auto weight=std::uint32_t(std::int32_t(rows[i].probability));
  if(remainder<weight){*out=i;return 0;}remainder-=weight;}
 return -2;
}
extern "C" int dh2_loot_quantity_v7(std::int32_t* out,dh2::data::LootRandom8V2* random,const dh2::data::LootQuantityChoiceV7* rows,std::uint32_t count,std::int32_t bonus) noexcept {
 if(!valid_selection(out,random,rows,count))return -1;
 std::uint32_t total=0;for(unsigned i=0;i<count;++i)total+=std::uint32_t(std::int32_t(rows[i].probability));
 if(!count||!total)return -2;
 std::int32_t draw;if(dh2_loot_v2_random(random,signed_word(total),&draw))return -1;
 auto remainder=std::uint32_t(draw)+std::uint32_t(bonus);
 if(remainder>=total)remainder=total-1;
 for(unsigned i=0;i<count;++i){auto weight=std::uint32_t(std::int32_t(rows[i].probability));
  if(remainder<weight){*out=rows[i].quantity;return 0;}remainder-=weight;}
 return -2;
}
extern "C" int dh2_loot_item_value_v7(std::int32_t* out,dh2::data::LootRandom8V2* random,const dh2::data::ItemRecord164* record,const dh2::data::LootPowerGold8V7* powers,std::uint32_t count,std::int32_t bonus256) noexcept {
 if(!valid_selection(out,random,powers,count)||!span(record,sizeof(*record),4)||
    overlap(out,4,record,sizeof(*record))||overlap(random,sizeof(*random),record,sizeof(*record)))return -1;
 auto& w=record->words;
 if(w[22]!=13){auto value=std::uint32_t(w[27])*std::uint32_t(w[28]);
  for(unsigned i=0;i<count;++i)value+=std::uint32_t(powers[i].multiplier)*std::uint32_t(powers[i].bonus);
  *out=signed_word(value);return 0;}
 std::int32_t draw;if(dh2_loot_v2_random(random,signed_word(std::uint32_t(w[28])+1u-std::uint32_t(w[27])),&draw))return -1;
 const float amount=static_cast<float>(signed_word(std::uint32_t(draw)+std::uint32_t(w[27])));
 const float bonus=static_cast<float>(asr8(bonus256));
 const float shifted=bonus+100.0f;
 const float scale=shifted/100.0f;
 *out=truncate(amount*scale);return 0;
}
namespace dh2::data {
LootPowerCreationV7::LootPowerCreationV7(LootPowerResourcesV7::Borrow resources,LootRandom8V2& random):resources_(std::move(resources)),random_(&random){if(!resources_)throw std::invalid_argument("Missing power creation resources");}
bool LootPowerCreationV7::add_powers(const LootEntry32V2& entry,ItemInstanceV1& item,std::int32_t bonus256,std::int32_t requested,std::int32_t difficulty,const LootPowerServicesV7& services,std::string& error){
 error.clear();if(running_){error="Destructive power creation reentry unsupported";return false;}
 auto list_id=entry.words[1];if(list_id==-1)return true;
 const auto& lists=resources_.lists();if(list_id<0||std::size_t(list_id)>=lists.size()){error="Required invalid ItemPowerList Debug continuation";return false;}
 if(item.powers.size()>65536){error="Power vector exceeds native budget";return false;}
 struct Guard{bool& flag;explicit Guard(bool& x):flag(x){flag=true;}~Guard(){flag=false;}}guard(running_);
 if(requested==-1){auto quantity_id=entry.words[3];const auto& rows=resources_.quantities();
  if(quantity_id<0||std::size_t(quantity_id)>=rows.size()){error="Required invalid NumProbArray Debug continuation";return false;}
  const auto& row=rows[quantity_id];auto status=dh2_loot_quantity_v7(&requested,random_,row.data(),std::uint32_t(row.size()),asr8(bonus256));
  if(status){error="Required NumProbArray selection continuation";return false;}}
 auto call=[&](LootPowerOperationV7 op,std::uint32_t caller,const char* key,std::int32_t power){
  if(!services.invoke){error="Required native loot-power service missing";return false;}
  const auto before=item.powers.size();
  if(op==LootPowerOperationV7::add_power&&before>=65536){error="Power append exceeds native budget";return false;}
  std::int32_t result=0;if(!services.invoke(services.context,{op,caller,&item,key,power,difficulty},result,error)){
   if(error.empty())error="Required native loot-power service failed";return false;}
  if(op==LootPowerOperationV7::add_power&&item.powers.size()!=before+1){
   error="Required AddPower provider did not append one actual power";return false;}
  return true;};
 auto debug=[&](std::uint32_t load,std::uint32_t query,const char* key){return call(LootPowerOperationV7::debug_load,load,nullptr,0)&&call(LootPowerOperationV7::debug_query,query,key,0);};
 if(!debug(0x4033ac,0x4033c8,"isTracingItemInventory_Loot")||!debug(0x4033dc,0x4033f8,"isTracingItemInventory_Loot"))return false;
 const auto& list=lists[list_id];const auto target=std::uint32_t(requested);
 if(target>=list.size()){
  for(const auto& choice:list)if(!call(LootPowerOperationV7::add_power,0x4038a0,nullptr,choice.power))return false;
  return true;
 }
 std::set<std::int32_t> used;
 auto definition=[&](const LootPowerChoiceV7& choice)->const ItemPowerRecordV5*{
  const auto& rows=resources_.powers().rows();if(choice.power<0||std::size_t(choice.power)>=rows.size()){
   error="Required invalid selected power Debug continuation";return nullptr;}return &rows[choice.power];};
 auto conflict=[&](const ItemPowerRecordV5& row,bool& blocked){
  const auto& lists=resources_.monopolies();auto id=row.scalars.monopoly;
  if(id<0||std::size_t(id)>=lists.size()){error="Required invalid AttrMonopoly Debug continuation";return false;}
  blocked=false;for(auto type:lists[id])if(used.count(type)){blocked=true;break;}return true;};
 unsigned retries=0;
 while(target>item.powers.size()){
  std::uint32_t index;auto status=dh2_loot_power_select_v7(&index,random_,list.data(),std::uint32_t(list.size()));
  if(status){error="Required weighted ItemPowerList Debug continuation";return false;}
  const auto& choice=list[index];auto* row=definition(choice);if(!row)return false;bool blocked;
  if(!conflict(*row,blocked))return false;
  if(blocked){++retries;if(!debug(0x40367c,0x403698,"isTracingItemInventory_Loot"))return false;
   if(retries>9)break;continue;}
  if(!call(LootPowerOperationV7::add_power,0x4035c8,nullptr,choice.power))return false;
  for(const auto& property:row->properties)used.insert(property.type);
  retries=0;
 }
 if(target>item.powers.size()){
  for(const auto& choice:list){auto* row=definition(choice);if(!row)return false;bool blocked;
   if(!conflict(*row,blocked))return false;if(blocked)continue;
   if(!call(LootPowerOperationV7::add_power,0x4037b0,nullptr,choice.power))return false;
   if(target<=item.powers.size())break;
   for(const auto& property:row->properties)used.insert(property.type);
  }
 }
 return true;
}
bool loot_item_value_v7(ItemInstanceV1& instance,const ItemTable& items,ItemPowerTablesV5::Borrow powers,LootRandom8V2& random,std::int32_t bonus256,const ItemTextServicesV5& text,std::string& error){
 error.clear();auto* metadata=item(items,instance.id);if(!metadata||!powers||instance.powers.size()>65536){error="Loot value needs genuine item/power backing";return false;}
 std::vector<LootPowerGold8V7> values;
 if(metadata->record.words[22]!=13){values.reserve(instance.powers.size());for(auto id:instance.powers){
  if(id<0||std::size_t(id)>=powers.rows().size()){error="Required invalid power valuation continuation";return false;}
  const auto& row=powers.rows()[id].scalars;values.push_back({row.gold_multiplier,row.gold_bonus});}}
 std::int32_t value;
 if(dh2_loot_item_value_v7(&value,&random,&metadata->record,values.data(),std::uint32_t(values.size()),bonus256)){
  error="Invalid native loot valuation projection";return false;}
 instance.value=value;return item_update_name_v5(instance,text,error);
}
}
