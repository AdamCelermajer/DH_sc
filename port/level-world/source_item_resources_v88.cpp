#include "source_item_resources_v88.hpp"
#include <stdexcept>
#include <cstring>
namespace dh2::character {namespace {
data::Bytes bytes(const std::vector<std::uint8_t>& value){return {value.data(),value.size()};}
struct TailReader {
 data::Bytes bytes;std::size_t at{};
 std::uint32_t word(){if(at>bytes.size||bytes.size-at<4)throw std::runtime_error("Truncated original Item Arrays suffix");
  const auto* p=bytes.data+at;at+=4;const auto n=p[0]|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;
  return n;}
 std::uint32_t count(){const auto n=word();if(n>65536)throw std::runtime_error("Original Item Arrays count exceeds native resource budget at "+std::to_string(at-4)+": "+std::to_string(n));return n;}
 std::int32_t integer(){const auto value=word();std::int32_t out;std::memcpy(&out,&value,4);return out;}
 std::vector<std::string> strings(){const auto n=count();std::vector<std::string> out;out.reserve(n);for(unsigned i=0;i<n;++i){const auto size=count();if(size>bytes.size-at)throw std::runtime_error("Truncated original Item Arrays string");out.emplace_back(reinterpret_cast<const char*>(bytes.data+at),size);skip(size);}return out;}
 void skip(std::size_t size){if(at>bytes.size||size>bytes.size-at)throw std::runtime_error("Truncated original Item Arrays records");at+=size;}
};
}
bool SourceItemResourcesV88::read(const char* path,std::vector<std::uint8_t>& out,std::string& e){
 bool found{};if(!inputs_.asset||!inputs_.asset(path,found,out,e))return false;
 if(!found){e=std::string("Required actual source Item cache ")+path;return false;}
 if(out.size()>8u*1024u*1024u){e="Source Item cache exceeds existing native codec budget";return false;}return true;
}
bool SourceItemResourcesV88::load(std::string& e){
 if(attempted_){e="Source Item resource initialization cannot replay a reached prefix";return false;}attempted_=true;
 if(!inputs_.owner||!inputs_.localization_owner||!inputs_.loot||!inputs_.design||!inputs_.localization||!inputs_.design.characters()){
  e="Required SAME campaign Arrays/GameDesign/StringManager source Item resources";return false;}
 std::vector<std::uint8_t> power_records,power_names,power_schema,monopoly_records,monopoly_names,monopoly_schema,
  loot_records,loot_names,loot_schema,av_records,av_names,av_schema;
 auto table=[this,&e](const char* name,auto& records,auto& names,auto& schema){const auto path=std::string("data/pydata/")+name;
  return read((path+"_pyarray.bin").c_str(),records,e)&&read((path+"_pyarraynames.bin").c_str(),names,e)&&read((path+"_pystructnames.bin").c_str(),schema,e);};
 if(!table("item_powers",power_records,power_names,power_schema)||!table("item_powers_monopoly",monopoly_records,monopoly_names,monopoly_schema)||
  !table("loot_table",loot_records,loot_names,loot_schema)||!table("loot_audiovisual",av_records,av_names,av_schema))return false;
 try{
  if(!definitions_.load(bytes(power_records),bytes(power_names),bytes(power_schema),e))return false;
  //The proven six-table decoder stops immediately before source table6,
  //Merchant20 (read4ff604): buy4/sell8 then condition4/merchandise8 pairs.
  //Struct schema order differs from table order: ItemList/Point structs do
  //not imply additional serialized arrays at this cursor.
  TailReader suffix{bytes(loot_records),inputs_.loot.consumed()};
  auto count=suffix.count();std::vector<SourceMerchantRowV114> merchants;merchants.reserve(count);
  for(unsigned i=0;i<count;++i){SourceMerchantRowV114 row;row.buy4=suffix.integer();row.sell8=suffix.integer();const auto n=suffix.count();row.entries.reserve(n);
   for(unsigned j=0;j<n;++j)row.entries.push_back({suffix.integer(),suffix.integer()});merchants.push_back(std::move(row));}
  //Source table7 is the Num(int16)/Prob(int16) list array consumed by the
  //existing LootPowerResourcesV7 quantity reader.
  const auto start=suffix.at;count=suffix.count();while(count--){const auto entries=suffix.count();suffix.skip(std::size_t(entries)*4);}
  const data::Bytes quantities{loot_records.data()+start,suffix.at-start};
  if(suffix.at!=loot_records.size())throw std::runtime_error("Unexpected original Item Arrays suffix bytes");
  TailReader merchant_schema{bytes(loot_schema)};for(unsigned i=0;i<22;++i)merchant_schema.strings();
  if(merchant_schema.strings()!=std::vector<std::string>{"ConditionId","Merchandise"}||merchant_schema.strings()!=std::vector<std::string>{"BuyMultiplier","SellMultiplier","MerchandiseList"})throw std::runtime_error("Actual Merchant source schema differs");
  TailReader merchant_names{bytes(loot_names)};for(unsigned i=0;i<6;++i)merchant_names.strings();auto names=merchant_names.strings();
  if(names.size()!=merchants.size())throw std::runtime_error("Actual Merchant source names/count differs");
  const data::LootPowerInputsV7 input{bytes(power_records),bytes(power_names),bytes(power_schema),
   bytes(monopoly_records),bytes(monopoly_names),bytes(monopoly_schema),quantities,bytes(loot_names),bytes(loot_schema)};
  if(!powers_.load(input,definitions_.borrow(),e)||!audiovisual_.load(bytes(av_records),bytes(av_names),bytes(av_schema),e))return false;
  presentation_=std::make_shared<data::ItemPresentationOwnerV5>(definitions_.borrow());
  text_=std::make_unique<ui::ItemTextOwnerV5>(inputs_.loot.items(),*inputs_.design.characters(),*inputs_.localization,inputs_.text_environment);
  merchants_v114_=std::move(merchants);merchant_names_v114_=std::move(names);ready_=true;e.clear();return true;
 }catch(const std::exception& failure){e=failure.what();return false;}
}
}
