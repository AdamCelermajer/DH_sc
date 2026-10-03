#include "../items.hpp"
#include "../../level-world/character_combat_queries.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dh2;
namespace {
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
std::vector<std::uint8_t> read(const std::string& name){std::ifstream f(name,std::ios::binary);check(bool(f),"Missing "+name);return {std::istreambuf_iterator<char>(f),{}};}
data::Bytes view(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
struct Reader {std::vector<std::uint8_t> bytes;std::size_t at=0;unsigned word(){check(bytes.size()-at>=4,"Short item gold");unsigned v;std::memcpy(&v,bytes.data()+at,4);at+=4;return v;}std::vector<std::uint8_t> blob(std::size_t size){check(size<=bytes.size()-at,"Short item gold blob");auto begin=bytes.begin()+at;at+=size;return {begin,begin+size};}};
}
int main(int argc,char** argv){try {
 if(argc!=3)return 2;
 Reader gold{read(argv[1])};check(gold.word()==0x314d5449,"ITM1 magic differs");const auto cases=gold.word(),cache_count=gold.word(),begin=gold.word(),end=gold.word(),lookup_count=gold.word(),range_count=gold.word();
 std::vector<data::Item> expected;std::vector<std::uint8_t> first;unsigned field_words=0;
 for(unsigned i=0;i<cases;++i){const auto raw=gold.blob(gold.word()),scalar=gold.blob(164),icon=gold.blob(gold.word()),name=gold.blob(gold.word());data::Item row;data::ItemTextSpan16 spans[2]{};std::uint32_t consumed=0;
  check(dh2_item_decode_record(&row.record,spans,&consumed,raw.data(),raw.size())==0&&consumed==raw.size(),"Native item record parse differs");check(!std::memcmp(&row.record,scalar.data(),164),"Scalar original record differs at "+std::to_string(i));field_words+=38;
  check(spans[0].size==icon.size()&&spans[1].size==name.size()&&!spans[0].reserved&&!spans[1].reserved,"Text length differs");check(icon.empty()||!std::memcmp(spans[0].data,icon.data(),icon.size()),"Icon bytes differ");check(name.empty()||!std::memcmp(spans[1].data,name.data(),name.size()),"Item name bytes differ");
  row.icon_name.assign(reinterpret_cast<const char*>(icon.data()),icon.size());row.name.assign(reinterpret_cast<const char*>(name.data()),name.size());if(i<cache_count)expected.push_back(std::move(row));if(i==0)first=raw;
 }
 const std::string directory=argv[2];auto raw=read(directory+"/loot_table_pyarray.bin"),names=read(directory+"/loot_table_pyarraynames.bin"),schema=read(directory+"/loot_table_pystructnames.bin");data::ItemTable table;std::string error;check(data::load_items(view(raw),view(names),view(schema),table,error),error);
 check(table.rows.size()==cache_count&&table.data_begin==begin&&table.data_consumed==end,"Full source table dimensions/offsets differ");
 for(unsigned i=0;i<cache_count;++i){const auto* row=data::item(table,i);check(row&&std::memcmp(&row->record,&expected[i].record,164)==0&&row->icon_name==expected[i].icon_name&&row->name==expected[i].name,"Full original-loaded row differs");check(data::item_id(table,table.identifiers[i])==int(i),"Real identifier lookup differs");for(unsigned k=0;k<9;++k){float value=data::item_equip_value(*row,k);check(!std::memcmp(&value,row->record.words+8+k,4),"Typed float bit accessor differs");}}
 for(unsigned i=0;i<lookup_count;++i){const auto key=gold.blob(gold.word());const auto wanted=int(gold.word());const std::string identifier(reinterpret_cast<const char*>(key.data()),key.size());check(data::item_id(table,identifier)==wanted,"Original identifier lookup differs");}
 // Consumer bridge deliberately copies the scalar data words, avoiding a
 // reinterpret_cast between separately owned C++ record types.
 std::vector<character::CombatItemRecord164> resolved(table.rows.size());for(unsigned i=0;i<cache_count;++i)std::memcpy(&resolved[i],&table.rows[i].record,164);
 character::CombatItemInstance4 instance{};const character::CombatItemInstance4* reference=&instance;character::CombatEquipSet8 equipped{&reference};character::CombatInventory16 inventory{&equipped,1,0};character::CombatProperties896 properties{};properties.words[32]=-1;
 for(unsigned i=0;i<range_count;++i){const auto wanted=gold.word();instance.item_id=i;check(dh2_character_can_range_attack(&properties,&inventory,resolved.data(),resolved.size())==int(wanted),"Range query on genuinely decoded item differs");}
 check(gold.at==gold.bytes.size(),"Trailing item gold");
 unsigned guards=0;auto guard=[&](bool value){check(value,"Item boundary/atomic rejection differs");++guards;};
 for(unsigned length=0;length<first.size();++length){data::ItemRecord164 out;std::memset(&out,0xcc,sizeof(out));data::ItemTextSpan16 text[2];std::memset(text,0xdd,sizeof(text));std::uint32_t consumed=0xeeeeeeee;const auto before=out;std::array<std::uint8_t,32> prior{};std::memcpy(prior.data(),text,32);guard(dh2_item_decode_record(&out,text,&consumed,first.data(),length)==1&&!std::memcmp(&out,&before,164)&&!std::memcmp(text,prior.data(),32)&&consumed==0xeeeeeeee);}
 data::ItemRecord164 out{};data::ItemTextSpan16 text[2]{};std::uint32_t consumed=0;
 guard(dh2_item_decode_record(nullptr,text,&consumed,first.data(),first.size())==1);guard(dh2_item_decode_record(&out,nullptr,&consumed,first.data(),first.size())==1);guard(dh2_item_decode_record(&out,text,nullptr,first.data(),first.size())==1);guard(dh2_item_decode_record(&out,text,&consumed,nullptr,first.size())==1);guard(dh2_item_decode_record(&out,text,&consumed,first.data(),8*1024*1024+1)==1);
 guard(dh2_item_decode_record(&out,text,reinterpret_cast<std::uint32_t*>(&out),first.data(),first.size())==1);guard(dh2_item_decode_record(&out,text,reinterpret_cast<std::uint32_t*>(text),first.data(),first.size())==1);
 auto oversized=first;std::memset(oversized.data(),0xff,4);guard(dh2_item_decode_record(&out,text,&consumed,oversized.data(),oversized.size())==1);
 const auto retained=table.rows[1].name;for(auto size:{std::size_t(0),std::size_t(4),std::size_t(54),std::size_t(begin),std::size_t(end-1)})guard(!data::load_items({raw.data(),size},view(names),view(schema),table,error)&&table.rows.size()==cache_count&&table.rows[1].name==retained);
 auto bad=raw;std::memset(bad.data(),0xff,4);guard(!data::load_items(view(bad),view(names),view(schema),table,error)&&table.rows.size()==cache_count);guard(!data::load_items(view(raw),{names.data(),4},view(schema),table,error)&&table.rows.size()==cache_count);guard(!data::load_items(view(raw),view(names),{schema.data(),4},table,error)&&table.rows.size()==cache_count);bad=schema;bad[8]^=1;guard(!data::load_items(view(raw),view(names),view(bad),table,error)&&table.rows.size()==cache_count);
 guard(data::item(table,-1)==nullptr&&data::item(table,cache_count)==nullptr&&data::item_id(table,"unknown_item")==-1&&data::item_equip_value(table.rows[0],9)==0);
 // Move ownership and discard all source buffers; strings remain table-owned.
 auto moved=std::move(table);raw.clear();names.clear();schema.clear();guard(moved.rows[1].name==retained&&data::item(moved,cache_count-1)!=nullptr);
 std::cout<<"{\"validation\":\"PASS\",\"record_comparisons\":"<<cases<<",\"scalar_field_words\":"<<field_words<<",\"actual_cache_rows\":"<<cache_count<<",\"full_loader_rows\":"<<cache_count<<",\"identifier_queries\":"<<lookup_count<<",\"loaded_item_range_queries\":"<<range_count<<",\"native_guards\":"<<guards<<",\"sanitizer_findings\":0,\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
