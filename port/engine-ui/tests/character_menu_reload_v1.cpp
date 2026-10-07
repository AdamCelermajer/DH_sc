#include "../character_menu_reload_v1.hpp"
#include <vector>
#include <array>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>
using namespace dh2::ui;
void require(bool ok){if(!ok)throw std::runtime_error("Reload coordinator proof failed");}
std::uint32_t read(std::istream& in){std::uint32_t value;require(bool(in.read(reinterpret_cast<char*>(&value),4)));return value;}
struct Fixture {
 std::int32_t level;std::array<std::int32_t,3> classes{};unsigned index=0;
 std::vector<std::array<std::uint32_t,2>> events;int fail=-1;
 static int invoke(void* p,const MenuReloadRequest32V1* q,MenuReloadResponse16V1* r){
  auto& t=*static_cast<Fixture*>(p);require(q->subject==(q->service==9?0x100000002ULL:0x100000001ULL));
  if(q->service==9)require(q->path&&q->callback&&!std::strcmp(q->path,"_root.menu_CharacterMenu")&&!std::strcmp(q->callback,"IsSpecTime"));
  else require(!q->path&&!q->callback);
  t.events.push_back({q->service,q->argument});if(int(q->service)==t.fail)return -1;
  if(q->service==6)r->value=t.level;
  if(q->service==7){require(t.index<3);r->value=t.classes[t.index++];}
  if(q->service==8)r->identity=0x100000002ULL;
  return 0;
 }
};
int main(int argc,char** argv){try{
 require(argc==2);std::ifstream input(argv[1],std::ios::binary);require(read(input)==0x314d5243);auto cases=read(input);unsigned calls=0;
 for(unsigned i=0;i<cases;++i){
  Fixture f{};f.level=std::int32_t(read(input));for(auto& c:f.classes)c=std::int32_t(read(input));auto count=read(input);
  std::vector<std::array<std::uint32_t,2>> expected;while(count--)expected.push_back({read(input),read(input)});
  MenuReloadServices16V1 s{&f,Fixture::invoke};MenuReloadResult16V1 result{};
  require(dh2_character_menu_reload_v1(&result,0x100000001ULL,&s)==1&&result.phase==11&&f.events==expected&&result.calls==expected.size()&&result.specialization==expected.back()[1]);calls+=result.calls;
 }
 require(input.peek()==EOF);unsigned failures=0;
 for(int fail=0;fail<10;++fail){
  Fixture f{12,{0,0,0},0,{},fail};MenuReloadServices16V1 s{&f,Fixture::invoke};MenuReloadResult16V1 result{};
  require(dh2_character_menu_reload_v1(&result,0x100000001ULL,&s)==-2&&result.phase==unsigned(fail+1)&&f.events.back()[0]==unsigned(fail));++failures;
 }
 Fixture f{};MenuReloadServices16V1 s{&f,Fixture::invoke};MenuReloadResult16V1 out{77,88,99,0},before=out;
 require(dh2_character_menu_reload_v1(&out,0,&s)==-1&&!std::memcmp(&out,&before,sizeof(out))&&f.events.empty());
 require(dh2_character_menu_reload_v1(&out,1,nullptr)==-1&&!std::memcmp(&out,&before,sizeof(out)));
 std::cout<<"{\"validation\":\"PASS\",\"original_complete_cases\":"<<cases<<",\"ordered_component_boundaries\":"<<calls<<",\"required_failure_prefixes\":"<<failures<<",\"atomic_guards\":2,\"component_services_are_fixtures\":true,\"whole_menu_live\":false}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
