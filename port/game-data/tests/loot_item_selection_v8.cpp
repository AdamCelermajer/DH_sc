#define main table_selection_main
#include "loot_table_selection_v8.cpp"
#undef main
#include "../loot_item_selection_v8.hpp"
int main(int argc,char** argv){try{
 check(argc==3);std::string cache=argv[2],error;
 auto records=file(cache+"/loot_table_pyarray.bin"),names=file(cache+"/loot_table_pyarraynames.bin"),schema=file(cache+"/loot_table_pystructnames.bin");
 LootTablesV2 tables;check(tables.load(span(records),span(names),span(schema),error),error);
 auto gold=file(argv[1]);Reader r{gold};check(r.block(4)==Raw({'L','I','S','8'}));auto n=r.u();unsigned emitted{};
 for(unsigned i=0;i<n;++i){LootRandom8V2 random{r.u(),r.u()};bool give_all=r.u()!=0;auto count=r.u();std::vector<LootEntry32V2> entries(count);std::vector<const LootEntry32V2*> pointers;
  for(auto& entry:entries){auto bytes=r.block(32);std::memcpy(entry.words,bytes.data(),32);pointers.push_back(&entry);}auto expected=r.block(r.u());Services services;
  LootItemSelectionV8 selector(tables.borrow(),random,{&services,Services::invoke});std::vector<LootItemInfoV8> output;
  check(selector.expand(pointers,give_all,output,error),error);Raw actual;word(actual,random.seed);word(actual,random.calls);word(actual,unsigned(output.size()));
  for(auto& item:output){word(actual,unsigned(int(item.id)));word(actual,unsigned(item.entry-entries.data()));word(actual,item.quantity);check(item.item==&tables.borrow().items().rows[std::size_t(item.id)]);}
  word(actual,unsigned(services.calls.size()));for(auto c:services.calls)word(actual,c);
  if(actual!=expected){std::cerr<<"case "<<i<<" expansion transcript mismatch\n";return 1;}emitted+=unsigned(output.size());check(true);
 }check(r.at==gold.size());
 LootRandom8V2 random{7,3};LootItemSelectionV8 missing(tables.borrow(),random,{});std::vector<LootItemInfoV8> output;LootEntry32V2 entry{};entry.words[0]=0;
 check(!missing.expand({&entry},true,output,error));check(output.empty());
 std::cout<<"{\"validation\":\"PASS\",\"original_cases\":"<<n<<",\"actual_cache_items_selected\":"<<emitted<<",\"checks\":"<<checks<<",\"full_AddLoot_creation\":false}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
