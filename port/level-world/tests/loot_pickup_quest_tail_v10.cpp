#include "loot_pickup_quest_tail_v10.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
struct Fixture {
 std::vector<int> calls;int mode{},fail{};LootPickupQuestEventV10 copied;
 static bool player(void* p,std::uintptr_t c,bool& out,std::string& e){auto& f=*static_cast<Fixture*>(p);if(c!=123)throw std::runtime_error("identity");f.calls.push_back(1);out=f.mode!=1;if(f.fail==1){e="player failure";return false;}return true;}
 static bool registered(void* p,std::uintptr_t c,std::int32_t id,bool& out,std::string& e){auto& f=*static_cast<Fixture*>(p);if(c!=123||id!=456)throw std::runtime_error("source IDs");f.calls.push_back(2);out=f.mode>=3;if(f.fail==2){e="registered failure";return false;}return true;}
 static bool current(void* p,std::uintptr_t& out,std::string& e){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(3);out=f.mode==3?0:789;if(f.fail==3){e="GS failure";return false;}return true;}
 static bool constant(void* p,const char* r1,const char* r2,std::int32_t& out,std::string& e){auto& f=*static_cast<Fixture*>(p);if(std::string(r1)!="v2QuestObjectiveType"||std::string(r2)!="GatherLoot")throw std::runtime_error("literalorder");f.calls.push_back(4);out=37;if(f.fail==4){e="constant failure";return false;}return true;}
 static bool async(void* p,std::uintptr_t gs,const LootPickupQuestEventV10& event,std::string& e){auto& f=*static_cast<Fixture*>(p);if(gs!=789)throw std::runtime_error("GS identity");f.calls.push_back(5);f.copied=event;if(f.fail==5){e="async failure";return false;}return true;}
};
int main(){try{unsigned checks=0;auto check=[&](bool value){++checks;if(!value)throw std::runtime_error("check "+std::to_string(checks));};Fixture f;LootPickupQuestServicesV10 services{&f,Fixture::player,Fixture::registered,Fixture::current,Fixture::constant,Fixture::async};std::string e;
 check(loot_pickup_quest_tail_v10(0,456,{},e));
 for(int mode=1;mode<=4;++mode){f={};f.mode=mode;check(loot_pickup_quest_tail_v10(123,456,services,e));check(f.calls.size()==std::size_t(mode==4?5:mode));}
 check(f.copied.objective_type==37&&f.copied.character==123&&f.copied.item_id==456&&f.copied.network_id==-1&&f.copied.subject_id==-1&&!f.copied.flag0&&!f.copied.flag1);
 for(int failure=1;failure<=5;++failure){f={};f.mode=4;f.fail=failure;e.clear();check(!loot_pickup_quest_tail_v10(123,456,services,e)&&!e.empty());check(f.calls.size()==std::size_t(failure));}
 std::cout<<"PASS whole pickup quest tail gates/literal order/copied fields and required failure prefixes checks="<<checks<<'\n';return 0;
 }catch(const std::exception& ex){std::cerr<<ex.what();return 1;}}
