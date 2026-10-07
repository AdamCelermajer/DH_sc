#include "../player_save_metadata_writer_v45.hpp"
#include "../player_save_inventory_writer_v45.hpp"
#include <cassert>
#include <fstream>
#include <algorithm>
#include <iostream>
using namespace dh2;
static std::vector<std::uint8_t> file(const char* name){std::ifstream f(std::string("port/android-native/app/src/main/assets/data/")+name,std::ios::binary);assert(f);return {std::istreambuf_iterator<char>(f),{}};}
int main(){unsigned checks=0;std::string e;auto a=file("character_properties_pyarray.bin"),b=file("character_properties_pyarraynames.bin"),c=file("character_properties_pystructnames.bin");data::CharacterTable characters;assert(data::load_characters({a.data(),a.size()},{b.data(),b.size()},{c.data(),c.size()},characters,e));
 auto saved=std::make_shared<data::PlayerSavegameV1>();saved->set_character(1);saved->set_player_class(0);auto authority=std::make_shared<data::PlayerSaveLoadOwnerV1>(saved);auto lease=std::make_shared<int>(1);
 level::PlayerSaveMetadataWriterV45 metadata(authority,{lease,&characters,[](std::int32_t& out,std::string&){out=2;return true;}});
 for(const char* tag:{"PCLS","PDFL","LNAM","LEPT","LUSP"}){
  level::SavegameStreamV2 stream;assert(metadata.write(tag,stream,e));data::PlayerSavegameV1 restored;std::size_t used{};data::Bytes bytes{stream.bytes().data(),stream.bytes().size()};
  if(std::string(tag)=="PCLS")assert(restored.load_class(bytes,characters.names,used,e)&&restored.class_id()==0);
  if(std::string(tag)=="PDFL"){int difficulty=-1;assert(restored.load_difficulty(bytes,&difficulty,[](void* p,int x,std::string&){*static_cast<int*>(p)=x;return true;},used,e)&&difficulty==2&&restored.unlocked_difficulty()==saved->unlocked_difficulty());}
  if(std::string(tag)=="LNAM")assert(restored.load_location(bytes,used,e)&&restored.location().current_acts==saved->location().current_acts);
  if(std::string(tag)=="LEPT")assert(restored.load_entry_points(bytes,used,e)&&restored.location().entry_points==saved->location().entry_points);
  if(std::string(tag)=="LUSP")assert(restored.load_spawn_points(bytes,used,e)&&restored.location().use_spawn_point==saved->location().use_spawn_point);
  assert(used==bytes.size);++checks;
 }
 a=file("loot_table_pyarray.bin");b=file("loot_table_pyarraynames.bin");c=file("loot_table_pystructnames.bin");data::LootTablesV2 tables;assert(tables.load({a.data(),a.size()},{b.data(),b.size()},{c.data(),c.size()},e));auto properties=std::make_shared<data::PropertyState>();data::LootRandom8V2 random{1,0};data::FreshInventoryOwnedV4 inventory(1,tables.borrow(),random,8,properties);
 const data::OwnedInventoryServicesV4 creation{nullptr,[](void*,auto&,const auto& q,auto& out,std::string&){out.value=0;if(q.operation==data::OwnedInventoryOperationV4::current_player)out.identity=1;return true;}}; // explicit construction-effect fixtures
 const int potion=data::item_id(inventory.table(),"Potion0");assert(potion>=0);std::unique_ptr<data::ItemInstanceV1> item;assert(inventory.create_item(potion,3,item,creation,e));int index{};assert(inventory.add_item(item,true,false,index,creation,e)&&inventory.items().size()==1);++checks;
 level::SavegameStreamV2 gear;assert(level::player_save_inventory_writer_v45(inventory,{},gear,e));data::ItemInventoryV1 restored(inventory.table());restored.set_character(1);restored.project_potion_capacity(8);data::InventoryLoadReceiptV1 receipt;
 const data::InventoryServicesV1 loading{nullptr,[](void*,auto&,const auto&,auto&,std::string&){return true;}}; // named deeper load effects, not live equip acceptance
 assert(restored.load_section({gear.bytes().data(),gear.bytes().size()},{},loading,receipt,e)&&receipt.completed&&receipt.consumed==gear.bytes().size());assert(restored.items().size()==1&&restored.items()[0]->item->id==potion&&restored.items()[0]->item->quantity==3);++checks;
 data::PropertyRules rules;auto view=data::property_view(rules,*properties);properties->saved[19]=512;const std::uint8_t observed194=1;level::SavegameStreamV2 props;assert(level::player_save_properties_writer_v45(view,&observed194,props,e));assert(props.size()==901);unsigned count{};assert(props.read_u32(count,e)&&count==224);for(unsigned i=0;i<224;++i){unsigned word{};assert(props.read_u32(word,e)&&word==unsigned(properties->saved[i]));}std::uint8_t last{};assert(props.read(&last,1,e)&&last==observed194);checks+=225;
 std::cout<<"Player named source metadata/inventory/PROP writers PASS "<<checks<<" checks; real cached item/class and existing readers; construction/load effect callbacks fixtures; Save194 producer required\n";
}
