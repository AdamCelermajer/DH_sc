#include "loot_power_resources_v7.hpp"
#include <cstring>
#include <stdexcept>

namespace {
struct Reader {
 dh2::data::Bytes b;std::size_t at{};
 explicit Reader(dh2::data::Bytes x):b(x){
  auto p=reinterpret_cast<std::uintptr_t>(x.data);
  if(!p||x.size>8*1024*1024||x.size>UINTPTR_MAX-p)
   throw std::runtime_error("Invalid power creation byte span");
 }
 const std::uint8_t* take(std::size_t n){
  if(n>b.size-at)throw std::runtime_error("Truncated power creation cache");
  auto* p=b.data+at;at+=n;return p;
 }
 std::uint32_t word(){auto* p=take(4);return p[0]|std::uint32_t(p[1])<<8|
  std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;}
 std::int32_t integer(){auto w=word();std::int32_t x;std::memcpy(&x,&w,4);return x;}
 std::uint32_t count(){auto n=word();if(n>65536)throw std::runtime_error("Power creation count exceeds budget");return n;}
 std::int16_t small(){auto* p=take(2);std::uint16_t w=std::uint16_t(p[0])|std::uint16_t(p[1])<<8;
  std::int16_t x;std::memcpy(&x,&w,2);return x;}
 std::vector<std::string> strings(){auto n=count();std::vector<std::string> out;out.reserve(n);
  while(n--){auto k=count();auto* p=take(k);out.emplace_back(reinterpret_cast<const char*>(p),k);}return out;}
 void end(){if(at!=b.size)throw std::runtime_error("Unexpected power creation trailing bytes");}
};
void equal_schema(const std::vector<std::string>& a,std::initializer_list<const char*> expected){
 std::vector<std::string> b;for(auto* x:expected)b.emplace_back(x);
 if(a!=b)throw std::runtime_error("Power creation schema differs");
}
}
namespace dh2::data {
struct LootPowerResourcesV7::Snapshot {
 ItemPowerTablesV5::Borrow powers;
 std::vector<std::vector<LootPowerChoiceV7>> lists;
 std::vector<std::vector<std::int32_t>> monopolies;
 std::vector<std::vector<LootQuantityChoiceV7>> quantities;
 std::vector<std::string> list_names,monopoly_names,quantity_names;
};
bool LootPowerResourcesV7::load(const LootPowerInputsV7& input,ItemPowerTablesV5::Borrow powers,std::string& error){
 try{
  if(snapshot_&&snapshot_.use_count()!=1)throw std::runtime_error("Power creation resources have live borrowers");
  if(!powers)throw std::runtime_error("Power creation needs the genuine V5 authority");
  auto next=std::make_shared<Snapshot>();next->powers=std::move(powers);
  Reader records(input.powers),names(input.power_names),schema(input.power_schema);
  auto n=records.count();next->lists.reserve(n);
  for(unsigned i=0;i<n;++i){auto k=records.count();std::vector<LootPowerChoiceV7> row;row.reserve(k);
   while(k--){auto id=records.integer();std::int8_t probability;auto* p=records.take(1);
    std::memcpy(&probability,p,1);row.push_back({id,probability});}next->lists.push_back(std::move(row));}
  if(records.count()!=next->powers.rows().size())throw std::runtime_error("Power definition authority count differs");
  for(const auto& row:next->powers.rows()){
   ItemPowerDecoded48V5 value{};
   if(dh2_item_power_decode_v5(&value,input.powers.data+records.at,
        static_cast<std::uint32_t>(input.powers.size-records.at)))
    throw std::runtime_error("Invalid power definition in creation input");
   if(std::memcmp(&value.scalars,&row.scalars,sizeof(value.scalars))||value.count!=row.properties.size())
    throw std::runtime_error("Creation power data differs from its pinned authority");
   Reader entries({value.entries,std::size_t(value.count)*12});
   for(const auto& entry:row.properties)
    if(entries.integer()!=entry.type||entries.integer()!=entry.value||entries.integer()!=entry.extra)
     throw std::runtime_error("Creation power properties differ from pinned authority");
   records.take(value.consumed);
  }
  records.end();next->list_names=names.strings();auto definition_names=names.strings();names.end();
  if(next->list_names.size()!=next->lists.size()||definition_names!=next->powers.names())
   throw std::runtime_error("Creation power names differ");
  equal_schema(schema.strings(),{"Power","Prob"});equal_schema(schema.strings(),{"list_entries"});
  equal_schema(schema.strings(),{"Attr","Bonus","Flags"});
  for(unsigned i=0;i<43;++i)equal_schema(schema.strings(),{"Palette","SpecialEffect",
   "AttrBonusList","Description","GoldBonusMultiplier","SortingOrder","GoldBonus","AttrMonopoly"});
  schema.end();
  Reader monopoly(input.monopoly),monopoly_names(input.monopoly_names),monopoly_schema(input.monopoly_schema);
  n=monopoly.count();next->monopolies.reserve(n);
  while(n--){auto k=monopoly.count();std::vector<std::int32_t> row;row.reserve(k);
   while(k--)row.push_back(monopoly.integer());next->monopolies.push_back(std::move(row));}
  monopoly.end();next->monopoly_names=monopoly_names.strings();monopoly_names.end();
  equal_schema(monopoly_schema.strings(),{"list_entries"});monopoly_schema.end();
  if(next->monopoly_names.size()!=next->monopolies.size())throw std::runtime_error("Monopoly names differ");
  Reader quantities(input.quantities),loot_names(input.loot_names),loot_schema(input.loot_schema);
  n=quantities.count();next->quantities.reserve(n);
  while(n--){auto k=quantities.count();std::vector<LootQuantityChoiceV7> row;row.reserve(k);
   while(k--){auto quantity=quantities.small();auto probability=quantities.small();row.push_back({quantity,probability});}
   next->quantities.push_back(std::move(row));}
  quantities.end();for(unsigned i=0;i<7;++i)loot_names.strings();next->quantity_names=loot_names.strings();
  for(unsigned i=0;i<18;++i)loot_schema.strings();
  equal_schema(loot_schema.strings(),{"Num","Prob"});equal_schema(loot_schema.strings(),{"list_entries"});
  if(next->quantity_names.size()!=next->quantities.size())throw std::runtime_error("Quantity names differ");
  snapshot_=std::move(next);error.clear();return true;
 }catch(const std::exception& x){error=x.what();return false;}
}
const ItemPowerTablesV5::Borrow& LootPowerResourcesV7::Borrow::powers()const{if(!snapshot_)throw std::logic_error("Missing power creation resources");return snapshot_->powers;}
const std::vector<std::vector<LootPowerChoiceV7>>& LootPowerResourcesV7::Borrow::lists()const{if(!snapshot_)throw std::logic_error("Missing power creation resources");return snapshot_->lists;}
const std::vector<std::vector<std::int32_t>>& LootPowerResourcesV7::Borrow::monopolies()const{if(!snapshot_)throw std::logic_error("Missing power creation resources");return snapshot_->monopolies;}
const std::vector<std::vector<LootQuantityChoiceV7>>& LootPowerResourcesV7::Borrow::quantities()const{if(!snapshot_)throw std::logic_error("Missing power creation resources");return snapshot_->quantities;}
const std::vector<std::string>& LootPowerResourcesV7::Borrow::list_names()const{if(!snapshot_)throw std::logic_error("Missing power creation resources");return snapshot_->list_names;}
const std::vector<std::string>& LootPowerResourcesV7::Borrow::monopoly_names()const{if(!snapshot_)throw std::logic_error("Missing power creation resources");return snapshot_->monopoly_names;}
const std::vector<std::string>& LootPowerResourcesV7::Borrow::quantity_names()const{if(!snapshot_)throw std::logic_error("Missing power creation resources");return snapshot_->quantity_names;}
}
