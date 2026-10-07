#include "../npc_inventory_owner_v1.hpp"
#include "../owned_equipment_queries_v4.hpp"
#include <cassert>
#include <fstream>
#include <iterator>
#include <iostream>
using namespace dh2;
static std::vector<std::uint8_t> read(const char* path){std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error(std::string("Actual cache unavailable: ")+path);return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char** argv){try{
 if(argc!=4)return 2;auto records=read(argv[1]),names=read(argv[2]),schema=read(argv[3]);
 data::LootTablesV2 tables;std::string error;
 if(!tables.load({records.data(),records.size()},{names.data(),names.size()},{schema.data(),schema.size()},error))throw std::runtime_error(error);
 assert(tables.borrow().items().rows.size()==1322);
 data::LootRandom8V2 random{0xD22026u,17};auto first=std::make_shared<data::PropertyState>(),second=std::make_shared<data::PropertyState>();
 character::NpcInventoryOwnerV1 a(0x100000006ull,tables.borrow(),random,first),b(0x100000007ull,tables.borrow(),random,second);
 assert(random.seed==0xD22026u&&random.calls==17);
 for(auto* owner:{&a,&b}){const auto& actual=owner->inventory();assert(actual.items().empty()&&!actual.potion()&&actual.num_potions()==0&&actual.gold()==0&&actual.current_equipment()==0);
  for(const auto& set:actual.equipment())for(auto* slot:set)assert(!slot);
  const auto query=owner->attack_borrow();assert(query.owned==&actual&&!query.projection&&query.resolved==actual.properties()->resolved.data());
  // Regression: the scrolling combat text consumer used to require the old
  // null-set projection and reject this genuine owned NPC inventory.
  player::EquipmentQueries12V1 equipment{7,8,9};
  assert(player::owned_equipment_queries_v4(equipment,*query.owned,actual.character(),query.resolved,error));
  assert(equipment.main_category==-1&&equipment.off_category==-1&&equipment.flags==0);
  const auto before=equipment;
  assert(!player::owned_equipment_queries_v4(equipment,*query.owned,actual.character()+1,query.resolved,error));
  assert(equipment.main_category==before.main_category&&equipment.off_category==before.off_category&&equipment.flags==before.flags);
  assert(&actual.table()==&tables.borrow().items());bool two{};assert(actual.has_two_hander(false,two,error)&&!two);
 }
 assert(a.inventory().properties()==first&&b.inventory().properties()==second&&a.inventory().character()!=b.inventory().character());
 a.inventory().swap_equipment();assert(a.inventory().current_equipment()==1&&b.inventory().current_equipment()==0);
 player::EquipmentQueries12V1 swapped{};
 assert(player::owned_equipment_queries_v4(swapped,a.inventory(),a.inventory().character(),first->resolved.data(),error)&&swapped.flags==0);
 bool rejected=false;try{character::NpcInventoryOwnerV1 malformed(0,{},random,first);}catch(const std::invalid_argument&){rejected=true;}assert(rejected);
 assert(random.calls==17);
 std::cout<<"PASS actual1322 ItemTable lease; same NPC props/identity; owned-equipment consumer supports both selected sets and rejects different Character authority without output mutation; source two9null sets and selected0; RNG untouched; no player Gear; malformed required table rejected.\n";
 return 0;
}catch(const std::exception& x){std::cerr<<x.what()<<'\n';return 1;}}
