#define main previous_selection_main
#include "loot_table_selection_v8.cpp"
#undef main
#include "../loot_temporary_inventory_v8.hpp"
#include "../fresh_inventory_owned_v4.hpp"
namespace {
struct TransferFixture {
 LootTemporaryInventoryV8* source{};std::vector<std::unique_ptr<ItemInstanceV1>> destination;
 std::vector<std::int32_t> order;std::size_t original_count{};int after_fail{-1};
 static bool debug(void*,const LootEntryRequestV8&,std::int32_t& out,std::string&){out=0;return true;}
 static bool add(void* raw,std::unique_ptr<ItemInstanceV1>& item,bool force,bool convert,std::int32_t& out,std::string&){auto& f=*static_cast<TransferFixture*>(raw);check(!force&&convert&&item);check(f.source->items().size()==f.original_count);f.order.push_back(100+item->id);out=static_cast<std::int32_t>(f.destination.size());f.destination.push_back(std::move(item));return true;}
 static bool after(void* raw,std::int32_t id,std::string& e){auto& f=*static_cast<TransferFixture*>(raw);check(f.source->items().size()==f.original_count);f.order.push_back(200+id);if(id==f.after_fail){e="actual reached quest receiver failure fixture";return false;}return true;}
 static bool gold(void* raw,std::int32_t amount,std::string&){auto& f=*static_cast<TransferFixture*>(raw);check(amount==0&&f.source->items().empty());f.order.push_back(300);return true;}
};
struct SamePlayerFixture {
 FreshInventoryOwnedV4& player;
 unsigned gold_notifications{};
 static bool effects(void* raw,FreshInventoryOwnedV4& inventory,const OwnedInventoryRequestV4& q,OwnedInventoryResponseV4& out,std::string& e){
  if(q.operation==OwnedInventoryOperationV4::debug_load||q.operation==OwnedInventoryOperationV4::debug_query){out.value=0;return true;}
  if(q.operation==OwnedInventoryOperationV4::gold_notifications){auto& f=*static_cast<SamePlayerFixture*>(raw);check(&inventory==&f.player&&q.source_caller==0x3fdfd8&&inventory.gold()==0);++f.gold_notifications;return true;} // Explicit observed gold presentation/quest receiver fixture.
  e="Unexpected required actual inventory continuation";return false;
 }
 static bool add(void* raw,std::unique_ptr<ItemInstanceV1>& item,bool force,bool convert,std::int32_t& out,std::string& e){auto& f=*static_cast<SamePlayerFixture*>(raw);check(!force&&convert);return f.player.add_item(item,force,convert,out,{raw,effects,nullptr},e);}
 static bool after(void*,std::int32_t,std::string&){return true;} // Explicit empty destination quest-set receiver fixture.
 static bool gold(void* raw,std::int32_t amount,std::string& e){check(amount==0);return static_cast<SamePlayerFixture*>(raw)->player.add_gold(amount,{raw,effects,nullptr},e);}
};
}
int main(int argc,char** argv){try{
 check(argc==2);std::string e;LootTablesV2 tables;const std::string path=argv[1];
 auto b=file(path+"/loot_table_pyarray.bin"),n=file(path+"/loot_table_pyarraynames.bin"),s=file(path+"/loot_table_pystructnames.bin");check(tables.load(span(b),span(n),span(s),e),e);
 for(unsigned count=0;count<=3;++count){LootTemporaryInventoryV8 source(tables.borrow());TransferFixture f;f.source=&source;f.original_count=count;std::vector<ItemInstanceV1*> identities;
  for(unsigned i=0;i<count;++i){auto item=std::make_unique<ItemInstanceV1>();item->id=int(i);item->quantity=1;identities.push_back(item.get());check(source.store(item,{nullptr,TransferFixture::debug},nullptr,nullptr,e),e);}
  check(source.transfer_all_source_v10(false,true,&f,TransferFixture::add,TransferFixture::after,TransferFixture::gold,e),e);check(source.items().empty()&&f.destination.size()==count);
  std::vector<int> expected;for(unsigned i=0;i<count;++i){expected.push_back(100+i);expected.push_back(200+i);check(f.destination[i].get()==identities[i]);}expected.push_back(300);check(f.order==expected);
 }
 LootTemporaryInventoryV8 failed(tables.borrow());TransferFixture f;f.source=&failed;f.original_count=3;f.after_fail=1;
 for(int i=0;i<3;++i){auto item=std::make_unique<ItemInstanceV1>();item->id=i;item->quantity=1;check(failed.store(item,{nullptr,TransferFixture::debug},nullptr,nullptr,e),e);}
 check(!failed.transfer_all_source_v10(false,true,&f,TransferFixture::add,TransferFixture::after,TransferFixture::gold,e));check(e=="actual reached quest receiver failure fixture");check(f.destination.size()==2&&failed.items().size()==3&&!failed.items()[0]&&failed.items()[1]&&!failed.items()[1]->item&&failed.items()[2]->item);
 const auto before=f.order;check(!failed.transfer_all_source_v10(false,true,&f,TransferFixture::add,TransferFixture::after,TransferFixture::gold,e)&&f.order==before);
 // A real retained player inventory destination, not a mock item object or
 // cloned temporary inventory. Equipment/property/RNG remain its one graph.
 auto properties=std::make_shared<PropertyState>();LootRandom8V2 random{123,0};FreshInventoryOwnedV4 player(0x12345678,tables.borrow(),random,5,properties);SamePlayerFixture live{player};LootTemporaryInventoryV8 dropped(tables.borrow());
 int row=-1;for(std::size_t i=0;i<tables.borrow().items().rows.size();++i){const auto type=item_type(tables.borrow().items().rows[i]);if(type!=13&&type!=14){row=int(i);break;}}check(row>=0);
 auto actual=std::make_unique<ItemInstanceV1>();actual->id=row;actual->quantity=1;auto* same=actual.get();check(dropped.store(actual,{nullptr,TransferFixture::debug},nullptr,nullptr,e),e);check(dropped.transfer_all_source_v10(false,true,&live,SamePlayerFixture::add,SamePlayerFixture::after,SamePlayerFixture::gold,e),e);check(dropped.items().empty()&&player.items().size()==1&&player.items()[0]->item.get()==same&&player.character()==0x12345678&&player.properties()==properties&&player.gold()==0&&random.seed==123&&live.gold_notifications==1);
 std::cout<<"PASS whole NULL-character TransferInventoryTo source slot/quest/gold order, original item identity and failed destination/quest prefix; destination/quest callbacks explicitfixtures checks="<<checks<<'\n';return 0;
 }catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
