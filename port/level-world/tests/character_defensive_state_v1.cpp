#include "character_defensive_state_v1.hpp"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>
using namespace dh2::character;
struct Fixture {
 int table{},stanced{},stance{},animation{};std::vector<unsigned char> trace;
 static Fixture& get(void* p){return *static_cast<Fixture*>(p);}
 static int player(void* p,std::uintptr_t,bool* out){get(p).trace.push_back(1);*out=false;return 0;}
 static int table_id(void* p,std::uintptr_t,int* out){get(p).trace.push_back(2);*out=get(p).table;return 0;}
 static int anim(void* p,int table,DefensiveAnimationV1,bool* found,int* out){*found=table>=0&&table<2;*out=get(p).animation;return 0;}
 static int constant(void* p,const char* key,const char* group,int* out){assert(!std::strcmp(key,"AnimStancedAnim")&&!std::strcmp(group,"SL__LIST_IPHONE"));get(p).trace.push_back(3);*out=get(p).stanced;return 0;}
 static int stance_id(void* p,std::uintptr_t,int* out){get(p).trace.push_back(4);*out=get(p).stance;return 0;}
 static int event(void* p,int event,std::uintptr_t payload){assert(event==50010&&payload==0x12345678);get(p).trace.push_back(5);return 0;}
 static int transition(void* p,int state,int event,std::uintptr_t payload){assert(state==11&&event==50010&&payload==0x12345678);get(p).trace.push_back(6);return 0;}
};
int main(int argc,char** argv){
 assert(argc==2);std::ifstream f(argv[1],std::ios::binary);assert(f);unsigned count{};f.read(reinterpret_cast<char*>(&count),4);assert(count==2880);
 for(unsigned i=0;i<count;++i){
  std::uint32_t row[11]{};unsigned char trace[8]{};f.read(reinterpret_cast<char*>(row),44);f.read(reinterpret_cast<char*>(trace),8);assert(f);
  Fixture fixture;fixture.table=int(row[2]);fixture.stanced=int(row[3]);fixture.stance=int(row[4]);fixture.animation=int(row[6]);
  float gate{};std::memcpy(&gate,&row[1],4);State state{};std::memcpy(&state.animation_override,&row[7],4);
  PlayerInjureBorrowV7 borrow{1,&state,&gate};PlayerInjureServicesV7 common{};common.context=&fixture;common.is_player=Fixture::player;common.animation_table=Fixture::table_id;common.constant=Fixture::constant;common.stance=Fixture::stance_id;common.event=Fixture::event;common.transition=Fixture::transition;
  const DefensiveStateServicesV1 services{&common,Fixture::anim};assert(character_defensive_state_v1(&borrow,0x12345678,row[5]!=0,DefensiveAnimationV1(row[0]),&services)==1);
  std::uint32_t actual{};std::memcpy(&actual,&gate,4);assert(actual==row[8]);assert(state.animation_override==int(row[9]));assert(fixture.trace==std::vector<unsigned char>(trace,trace+row[10]));
 }
 std::puts("PASS 2880 whole original defensive reaction cases: gate, authored animation, stance, event order and overflow");
}
