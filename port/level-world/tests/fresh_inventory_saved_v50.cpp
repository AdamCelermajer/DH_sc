#include "../player_save_inventory_writer_v45.hpp"
#include <fstream>
#include <iostream>
#include <cassert>
using namespace dh2;
static std::vector<std::uint8_t> file(const char* name){std::ifstream f(std::string("port/android-native/app/src/main/assets/data/")+name,std::ios::binary);assert(f);return {std::istreambuf_iterator<char>(f),{}};}
struct Effects {unsigned calls{},fail_at{};std::vector<std::uint32_t> operations;};
static bool effect(void* raw,data::FreshInventoryOwnedV4& inv,const data::OwnedInventoryRequestV4& q,data::OwnedInventoryResponseV4& out,std::string& e){auto& f=*static_cast<Effects*>(raw);f.operations.push_back(std::uint32_t(q.operation));if(++f.calls==f.fail_at){e="Required fixture continuation";return false;}out.value=0;if(q.operation==data::OwnedInventoryOperationV4::current_player)out.identity=inv.character();if(q.operation==data::OwnedInventoryOperationV4::add_power&&q.argument>=0)q.item->powers.push_back(q.argument);return true;}
int main(){std::string e;unsigned checks{};auto a=file("loot_table_pyarray.bin"),b=file("loot_table_pyarraynames.bin"),c=file("loot_table_pystructnames.bin");data::LootTablesV2 tables;assert(tables.load({a.data(),a.size()},{b.data(),b.size()},{c.data(),c.size()},e));data::LootRandom8V2 random{77,0};auto p=std::make_shared<data::PropertyState>();p->resolved[188]=100*256;
 Effects effects;data::OwnedInventoryServicesV4 services{&effects,effect};data::FreshInventoryOwnedV4 source(1,tables.borrow(),random,8,p);int weapon=-1;
 for(unsigned i=0;i<source.table().rows.size();++i){const auto* record=data::item(source.table(),int(i));if(record&&record->record.words[26]==1){weapon=int(i);break;}}assert(weapon>=0);
 std::unique_ptr<data::ItemInstanceV1> item;int index{};assert(source.create_item(weapon,1,item,services,e)&&source.add_item(item,true,true,index,services,e)&&source.equip_to_slot(1,index,true,services,e));source.swap_equipment();assert(source.equip_to_slot(1,index,true,services,e));source.swap_equipment();
 assert(source.create_item(data::item_id(source.table(),"Potion0"),3,item,services,e)&&source.add_item(item,true,true,index,services,e));assert(source.set_gold(137,services,e));level::SavegameStreamV2 frame;assert(level::player_save_inventory_writer_v45(source,{},frame,e));
 auto restored_properties=std::make_shared<data::PropertyState>();restored_properties->resolved[188]=100*256;data::FreshInventoryOwnedV4 restored(1,tables.borrow(),random,8,restored_properties);const auto* same=&restored;const auto rng=random;
 Effects restored_effects;data::OwnedInventoryServicesV4 restore{&restored_effects,effect};data::InventoryLoadReceiptV1 receipt;assert(restored.load_saved_section_v50({frame.bytes().data(),frame.bytes().size()},{},restore,receipt,e)&&receipt.completed&&receipt.consumed==frame.size());++checks;
 assert(&restored==same&&restored.properties()==restored_properties&&restored.gold()==137&&restored.num_potions()==3&&restored.current_equipment()==source.current_equipment());checks+=5;
 assert(restored.items().size()==source.items().size());++checks;for(unsigned i=0;i<source.items().size();++i){assert(restored.items()[i]->item->id==source.items()[i]->item->id&&restored.items()[i]->item->quantity==source.items()[i]->item->quantity&&restored.items()[i]->slots==source.items()[i]->slots);checks+=3;}
 for(unsigned set=0;set<2;++set)assert(restored.equipment()[set][1]==restored.items()[0].get());checks+=2;assert(random.seed==rng.seed&&random.calls==rng.calls);++checks;
 // Truncation/effect failure cannot publish an empty success or replay grants.
 data::FreshInventoryOwnedV4 truncated(1,tables.borrow(),random,8,p);assert(!truncated.load_saved_section_v50({frame.bytes().data(),frame.bytes().size()-1},{},services,receipt,e)&&!receipt.completed&&receipt.items_read>0);++checks;
 Effects failed;failed.fail_at=2;data::OwnedInventoryServicesV4 fail{&failed,effect};data::FreshInventoryOwnedV4 prefix(1,tables.borrow(),random,8,p);assert(!prefix.load_saved_section_v50({frame.bytes().data(),frame.bytes().size()},{},fail,receipt,e)&&prefix.gold()==137&&!receipt.completed&&prefix.items().empty());++checks;
 // Present empty GEAR is malformed; absent GEAR is decided by the profile
 // provider BEFORE this method and never converted to empty successful bytes.
 data::FreshInventoryOwnedV4 empty(1,tables.borrow(),random,8,p);assert(!empty.load_saved_section_v50({nullptr,0},{},services,receipt,e)&&empty.items().empty());++checks;
 std::cout<<"SAME V4 source GEAR restore actual item/dual equipment/potions/gold/failure/RNG PASS "<<checks<<"; effects explicit fixtures, no live visual/quest publication claimed\n";
}
