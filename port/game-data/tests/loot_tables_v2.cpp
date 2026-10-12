#ifdef NDEBUG
#undef NDEBUG
#endif
#include "../loot_tables_v2.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
using namespace dh2::data;
static unsigned checks;
static void ck(bool b){++checks;if(!b)throw std::runtime_error("Loot check "+std::to_string(checks));}
using Raw=std::vector<std::uint8_t>;
static Raw file(const std::string& p){std::ifstream f(p,std::ios::binary);ck(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
static std::uint32_t word(const Raw& b,std::size_t& at){ck(b.size()-at>=4);std::uint32_t n;std::memcpy(&n,b.data()+at,4);at+=4;return n;}
int main(int argc,char** argv){try{ck(argc==3);auto gold=file(argv[1]);std::size_t at=0;ck(word(gold,at)==0x3256544c);auto count=word(gold,at);unsigned guards=0;
 for(unsigned i=0;i<count;++i){auto n=word(gold,at);ck(n<=gold.size()-at);LootRow64V2 row{};ck(!dh2_loot_v2_decode(&row,gold.data()+at,n));ck(row.consumed==n);for(auto s:{row.random_entries,row.fixed_entries,row.sub_loots})ck(!s.reserved&&s.bytes>=gold.data()+at&&s.bytes<=gold.data()+at+n);auto old=row;ck(dh2_loot_v2_decode(&row,gold.data()+at,n-1)==-1&&!std::memcmp(&row,&old,sizeof row));++guards;at+=n;}
 auto ops=word(gold,at);for(unsigned i=0;i<ops;++i){LootRandom8V2 r{word(gold,at),word(gold,at)};auto bound=word(gold,at);std::int32_t b;std::memcpy(&b,&bound,4);auto expected_seed=word(gold,at),expected_calls=word(gold,at),expected_value=word(gold,at);std::int32_t value;ck(!dh2_loot_v2_random(&r,b,&value)&&r.seed==expected_seed&&r.calls==expected_calls&&std::uint32_t(value)==expected_value);}ck(at==gold.size());
 auto records=file(std::string(argv[2])+"/loot_table_pyarray.bin"),names=file(std::string(argv[2])+"/loot_table_pyarraynames.bin"),schema=file(std::string(argv[2])+"/loot_table_pystructnames.bin");LootTablesV2 owner;std::string error;if(!owner.load({records.data(),records.size()},{names.data(),names.size()},{schema.data(),schema.size()},error))throw std::runtime_error(error);auto b=owner.borrow();ck(b.loots().size()==339&&b.item_lists().size()==186&&b.items().rows.size()==1322&&b.consumed()==282872);auto& starter=b.loots().at(165);ck(b.loot_names()[165]=="KnightStartingLoot"&&starter.roll_type==0&&starter.num_random_item_probs==20&&starter.random_entries.empty()&&starter.fixed_entries.size()==5&&starter.sub_loots.empty());unsigned expected[]={1079,1073,1076,664,925},qty[]={1,1,1,1,5};for(unsigned i=0;i<5;++i){auto& list=b.item_lists().at(starter.fixed_entries[i].words[0]);ck(list.size()==1&&list[0].item==std::int32_t(expected[i])&&list[0].probability==1&&list[0].quantity==qty[i]);}
 ck(!owner.load({records.data(),records.size()},{names.data(),names.size()},{schema.data(),schema.size()},error)&&error=="LootTable snapshot has live borrowers");++guards;std::fill(records.begin(),records.end(),0);ck(b.items().identifiers[1079]=="StartingSuit");b={};ck(!owner.load({records.data(),records.size()},{names.data(),names.size()},{schema.data(),schema.size()},error)&&owner.borrow().loots().size()==339);++guards;
 LootRandom8V2 r{123,0};auto old=r;ck(dh2_loot_v2_random(&r,1,reinterpret_cast<std::int32_t*>(&r))==-1&&!std::memcmp(&r,&old,sizeof r));++guards;
 std::cout<<"{\"validation\":\"PASS\",\"original_Loot_records\":339,\"synthetic_records\":128,\"original_random_operations\":"<<ops<<",\"actual_ItemTable_rows\":1322,\"actual_ItemLists\":186,\"starter_authored_items\":5,\"guards\":"<<guards<<",\"checks\":"<<checks<<",\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
