#define main loot_table_fixture_main
#include "loot_table_selection_v8.cpp"
#undef main
#include "../loot_temporary_inventory_v8.hpp"
struct Cleanup {unsigned count{},fail_at{};std::vector<int> ids;};
static bool debug(void*,const LootEntryRequestV8&,std::int32_t& value,std::string&){value=0;return true;}
static bool destroy(void* p,ItemInstanceV1& item,std::string& e){auto& c=*static_cast<Cleanup*>(p);c.ids.push_back(item.id);if(++c.count==c.fail_at){e="Explicit source item destructor owner failure";return false;}return true;}
int main(int argc,char** argv){try{check(argc==2);std::string error;auto b=file(std::string(argv[1])+"/loot_table_pyarray.bin"),n=file(std::string(argv[1])+"/loot_table_pyarraynames.bin"),s=file(std::string(argv[1])+"/loot_table_pystructnames.bin");LootTablesV2 tables;check(tables.load(span(b),span(n),span(s),error),error);
 for(unsigned fail:{0u,2u}){LootTemporaryInventoryV8 inventory(tables.borrow());for(int id:{0,1,2}){auto item=std::make_unique<ItemInstanceV1>();item->id=id;item->quantity=1;check(inventory.store(item,{nullptr,debug},nullptr,nullptr,error),error);check(!item);}
  Cleanup c;c.fail_at=fail;const bool result=inventory.remove_all_owned_v2(&c,destroy,error);
  if(!fail){check(result&&inventory.items().empty()&&inventory.potion()==nullptr&&c.ids==std::vector<int>({0,1,2}));check(inventory.remove_all_owned_v2(&c,destroy,error)&&c.count==3);}
  else{check(!result&&c.ids==std::vector<int>({0,1}));check(inventory.items().size()==3&&!inventory.items()[0]&&inventory.peek(1)&&inventory.peek(2));check(!inventory.remove_all_owned_v2(&c,destroy,error)&&c.count==2);auto item=std::make_unique<ItemInstanceV1>();item->id=0;check(!inventory.store(item,{nullptr,debug},nullptr,nullptr,error)&&item);}
 }
 std::cout<<"Actual NULL-character RemoveAll(true) owner PASS ordered deletion/failure prefix/no-retry; Debug/destructor endpoint fixtures explicit checks="<<checks<<'\n';return 0;
 }catch(const std::exception& e){std::cerr<<"check "<<checks<<" "<<e.what()<<'\n';return 1;}}
