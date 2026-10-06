#include "items.hpp"
#include <algorithm>
#include <cstring>
#include <limits>
#include <stdexcept>
namespace {
constexpr std::uint32_t byte_limit=8*1024*1024,count_limit=65536,string_limit=65536;
bool span(const void* p,std::size_t size,std::size_t alignment){
 const auto at=reinterpret_cast<std::uintptr_t>(p);
 return p&&at%alignment==0&&size<=std::numeric_limits<std::uintptr_t>::max()-at;
}
bool overlaps(const void* a,std::size_t an,const void* b,std::size_t bn){
 const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
 return x<y+bn&&y<x+an;
}
struct Cursor {
 const std::uint8_t* bytes;std::uint32_t size,at=0;
 bool take(std::uint32_t count,const std::uint8_t*& p){if(count>size-at)return false;p=bytes+at;at+=count;return true;}
 bool word(std::int32_t& out){const std::uint8_t* p;if(!take(4,p))return false;const auto v=std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);std::memcpy(&out,&v,4);return true;}
 bool text(std::int32_t& length,dh2::data::ItemTextSpan16& out){const std::uint8_t* p;if(!word(length)||length<0||std::uint32_t(length)>string_limit||!take(std::uint32_t(length),p))return false;out={p,std::uint32_t(length),0};return true;}
};
struct Reader {
 dh2::data::Bytes bytes;std::size_t at=0;
 explicit Reader(dh2::data::Bytes b):bytes(b){if(!span(b.data,b.size,1)||b.size<4||b.size>byte_limit)throw std::runtime_error("Item input size outside limit");}
 void skip(std::size_t size){if(size>bytes.size-at)throw std::runtime_error("Truncated item section");at+=size;}
 std::uint32_t word(){if(bytes.size-at<4)throw std::runtime_error("Truncated item word");const auto* p=bytes.data+at;at+=4;return p[0]|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
 std::uint32_t count(){const auto v=word();if(v>count_limit)throw std::runtime_error("Item section count outside limit");return v;}
 std::vector<std::string> strings(){const auto n=count();std::vector<std::string> out;out.reserve(n);for(std::uint32_t i=0;i<n;++i){const auto len=word();if(len>string_limit||len>bytes.size-at)throw std::runtime_error("Item identifier length outside limit");out.emplace_back(reinterpret_cast<const char*>(bytes.data+at),len);skip(len);}return out;}
};
const char* const base_fields[]={"IconName","PickUpType","DistType","SwooshSoundFX","SwooshFX","Stackable","WarriorEquipValue","MageEquipValue","RogueEquipValue","PaladdinEquipValue","BerserkerEquipValue","AssassinEquipValue","ArcherEquipValue","NecromancerEquipValue","IllusionistEquipValue"};
const char* const item_fields[]={"Name","Material","ModularModule","AudioVisualID","Type","BaseStat","BaseProp","BaseElement","EquipmentSlottingType","Value","GoldValueMultiplier","LevelRequirement","StrengthRequirement","DexterityRequirement","EnduranceRequirement","EnergyRequirement","ClassRequirement","Param1","Param2","Param3","Param4","Param5","Param6"};
}
extern "C" int dh2_item_decode_record(dh2::data::ItemRecord164* output,dh2::data::ItemTextSpan16* text,
                                      std::uint32_t* consumed,const std::uint8_t* bytes,std::uint32_t size){
 using namespace dh2::data;
 if(size>byte_limit||!span(bytes,size,1)||!span(output,sizeof(*output),4)||!span(text,2*sizeof(*text),8)||!span(consumed,4,4)||
    overlaps(output,sizeof(*output),bytes,size)||overlaps(text,2*sizeof(*text),bytes,size)||overlaps(consumed,4,bytes,size)||
    overlaps(output,sizeof(*output),text,2*sizeof(*text))||overlaps(output,sizeof(*output),consumed,4)||overlaps(text,2*sizeof(*text),consumed,4))return 1;
 Cursor c{bytes,size};ItemRecord164 next{};ItemTextSpan16 strings[2]{};
 if(!c.text(next.words[1],strings[0]))return 1;
 for(unsigned i=3;i<=6;++i)if(!c.word(next.words[i]))return 1;
 const std::uint8_t* boolean;if(!c.take(1,boolean))return 1;next.words[7]=boolean[0];
 for(unsigned i=8;i<=18;++i)if(!c.word(next.words[i]))return 1;
 if(!c.text(next.words[19],strings[1]))return 1;
 for(unsigned i=21;i<=40;++i)if(!c.word(next.words[i]))return 1;
 *output=next;text[0]=strings[0];text[1]=strings[1];*consumed=c.at;return 0;
}
namespace dh2::data {
bool load_items(Bytes records,Bytes names,Bytes fields,ItemTable& output,std::string& error){
 error.clear();try {
  Reader r(records),n(names),f(fields);ItemTable next;
  const auto priorities=r.count();for(std::uint32_t i=0;i<priorities;++i)r.skip(std::size_t(r.count())*4);
  r.skip(std::size_t(r.count())*2);
  const auto lists=r.count();for(std::uint32_t i=0;i<lists;++i)r.skip(std::size_t(r.count())*7);
  next.data_begin=r.at;
  const auto count=r.count();next.rows.reserve(count);
  for(std::uint32_t i=0;i<count;++i){Item row;ItemTextSpan16 text[2]{};std::uint32_t consumed=0;
   if(dh2_item_decode_record(&row.record,text,&consumed,records.data+r.at,std::uint32_t(records.size-r.at)))throw std::runtime_error("Malformed item record");
   row.icon_name.assign(reinterpret_cast<const char*>(text[0].data),text[0].size);row.name.assign(reinterpret_cast<const char*>(text[1].data),text[1].size);r.skip(consumed);next.rows.push_back(std::move(row));
  }
  for(unsigned i=0;i<3;++i)n.strings();
  next.names_begin=n.at;next.identifiers=n.strings();
  if(next.identifiers.size()!=next.rows.size())throw std::runtime_error("Item identifiers/records differ");
  const auto base=f.strings();next.fields=f.strings();
  if(base.size()!=15||next.fields.size()!=38)throw std::runtime_error("Item schema dimensions differ");
  for(unsigned i=0;i<15;++i)if(base[i]!=base_fields[i]||next.fields[i]!=base_fields[i])throw std::runtime_error("Item base schema differs");
  for(unsigned i=0;i<23;++i)if(next.fields[i+15]!=item_fields[i])throw std::runtime_error("Item schema differs");
  next.data_consumed=r.at;next.names_consumed=n.at;next.fields_consumed=f.at;output=std::move(next);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
const Item* item(const ItemTable& table,std::int32_t id)noexcept{return id>=0&&std::size_t(id)<table.rows.size()?&table.rows[id]:nullptr;}
std::int32_t item_id(const ItemTable& table,const std::string& identifier)noexcept{
 const auto it=std::find_if(table.identifiers.begin(),table.identifiers.end(),[&](const std::string& value){return std::strcmp(value.c_str(),identifier.c_str())==0;});
 return it==table.identifiers.end()||std::size_t(it-table.identifiers.begin())>=table.rows.size()?-1:std::int32_t(it-table.identifiers.begin());
}
std::int32_t item_type(const Item& row)noexcept{return row.record.words[22];}
float item_equip_value(const Item& row,std::uint32_t index)noexcept{if(index>=9)return 0;float out;std::memcpy(&out,row.record.words+8+index,4);return out;}
}
