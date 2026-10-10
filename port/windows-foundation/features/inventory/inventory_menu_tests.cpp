#include "inventory_menu.hpp"
#include <cassert>
#include <fstream>
#include <iterator>
#include <iostream>
using namespace dh::foundation;
static std::vector<std::uint8_t> read(const std::string& path){std::ifstream f(path,std::ios::binary);assert(f);return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char**argv){assert(argc==2);const std::string root=argv[1];auto r=read(root+"/loot_table_pyarray.bin"),n=read(root+"/loot_table_pyarraynames.bin"),f=read(root+"/loot_table_pystructnames.bin");dh2::data::ItemTable table;std::string error;assert(dh2::data::load_items({r.data(),r.size()},{n.data(),n.size()},{f.data(),f.size()},table,error));
auto owner=make_default_character();owner.inventory.push_back({"owned",table.identifiers[664],1});owner.equipment.push_back({"slot1","owned"});inventory::MenuPresenter menu(owner,table);inventory::MenuBindings bindings;
bindings.symbol=[](const std::string& key,std::string& out,std::string&){assert(key=="GLOBAL_EMPTY");out="SOURCE_EMPTY_PROVIDER";return true;};
bindings.item_name=[](const inventory::Row& row,std::string& out,std::string&){assert(row.instance_id=="owned");out="SOURCE_ITEM_PROVIDER";return true;};
bindings.potions=[](std::uint32_t count,std::string& out,std::string&){assert(count==0);out="SOURCE_POTION_PROVIDER";return true;};
character_menu::Frame frame;frame.art.batches=character_menu::original_menu_art(character_menu::Tab::equipment).batches;
assert(menu.content(bindings,frame,error));assert(frame.text.size()==10);unsigned equipped=0;for(const auto& text:frame.text)if(text.value=="SOURCE_ITEM_PROVIDER")++equipped;assert(equipped==1);
const auto& slots=inventory::original_inventory_slots();assert(slots.size()==10);assert(slots[0].icons[0].shape_id!=slots[1].icons[0].shape_id);assert(slots[5].icons[0].shape_id==slots[6].icons[0].shape_id);
for(const auto& slot:slots){assert(!slot.hit_contour.empty()&&slot.fields.size()==1);const auto&a=slot.hit_contour[0];const auto&b=slot.hit_contour[1];const auto&c=slot.hit_contour[2];const float x=(a.x+b.x+c.x)/3,y=(a.y+b.y+c.y)/3;assert(menu.hit_slot(x,y)==int(slot.source_slot));assert(menu.release_slot(x,y,error));assert(menu.requested_slot()==int(slot.source_slot));for(const auto& fill:slot.color_fills)assert(!fill.empty());}
assert(menu.hit_slot(-1000,-1000)==-1);bindings.item_color=[](const inventory::Row&,unsigned& color,std::string&){color=3;return true;};assert(menu.content(bindings,frame,error));bool purple=false;for(const auto& batch:frame.art.batches)for(const auto& expected:slots[1].color_fills[3])if(batch.role==expected.role&&batch.shape_id==expected.shape_id)purple=true;assert(purple);
const auto old=frame.art.batches.size();bindings.item_color=[](const inventory::Row&,unsigned& color,std::string&){color=9;return true;};assert(!menu.content(bindings,frame,error)&&frame.art.batches.size()==old);
std::cout<<"inventory menu10 actual sourcecategoryicons/hitzones/textfields passed; localization providers mocked explicitly\n";
}
