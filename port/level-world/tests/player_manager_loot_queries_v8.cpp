#include "../player_manager_loot_queries_v8.hpp"
#include <array>
#include <iostream>
#include <stdexcept>
using namespace dh2::player;
namespace {
unsigned checks{};
void check(bool v){++checks;if(!v)throw std::runtime_error("player class query contract");}
struct Fixture {
 std::int32_t count{4};std::array<std::uintptr_t,4> characters{{10,20,0,40}};
 std::array<std::int16_t,4> base{{263,290,325,263}};unsigned queries{};bool shrink{},missing{};
 static bool get(void* p,std::int32_t index,bool local,LootPlayerBorrowV8& out,std::string&){
  auto& f=*static_cast<Fixture*>(p);check(local);check(index>=0&&index<4);++f.queries;
  out={std::uintptr_t(index+1),&f.characters[std::size_t(index)],f.missing?nullptr:&f.base[std::size_t(index)]};
  if(f.shrink&&index==0)f.count=1;return true;
 }
};
}
int main(){try{
 Fixture f;LootPlayerManagerServicesV8 services{&f,Fixture::get};std::string error;int out{};
 check(player_warrior_count_v8(&f.count,services,out,error)&&out==2&&f.queries==4);
 check(player_mage_count_v8(&f.count,services,out,error)&&out==1);
 check(player_rogue_count_v8(&f.count,services,out,error)&&out==0); // NULL Character is skipped.
 f.characters[2]=30;check(player_rogue_count_v8(&f.count,services,out,error)&&out==1);
 f.shrink=true;f.queries=0;check(player_warrior_count_v8(&f.count,services,out,error)&&out==1&&f.queries==1);
 f.count=0;f.queries=0;check(player_class_count_v8(&f.count,263,{},out,error)&&out==0&&!f.queries);
 check(!player_class_count_v8(nullptr,263,services,out,error));
 f.count=4;f.shrink=false;f.missing=true;check(!player_warrior_count_v8(&f.count,services,out,error)&&out==0);
 f.missing=false;f.base[0]=-1;check(player_warrior_count_v8(&f.count,services,out,error)&&out==1);
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"actual_player_manager\":false}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
