#include "../character_hit_player_tail_v7.hpp"
#include <array>
#include <cassert>
#include <string>
#include <vector>
using namespace dh2::character;
struct Fixture {std::vector<std::string> calls;bool online=false;int difficulty=0,id=9;bool fail_job=false;};
int online(void* p,bool* out){auto& f=*static_cast<Fixture*>(p);f.calls.push_back("online");*out=f.online;return 0;}
int player(void* p,std::uintptr_t,bool* out){static_cast<Fixture*>(p)->calls.push_back("player");*out=true;return 0;}
int difficulty(void* p,std::uintptr_t,int* out){auto& f=*static_cast<Fixture*>(p);f.calls.push_back("difficulty");*out=f.difficulty;return 0;}
int script(void* p,const char* name,int* out){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(name);*out=f.id;return 0;}
int start(void* p,int id,int arg,bool flag){assert(id==9&&arg==-1&&!flag);static_cast<Fixture*>(p)->calls.push_back("start");return 0;}
int job(void* p){auto& f=*static_cast<Fixture*>(p);f.calls.push_back("job");return f.fail_job?1:0;}
int sound(void* p,const char* name,int* out){static_cast<Fixture*>(p)->calls.push_back(name);*out=-1;return 0;}
int play(void* p,int index,bool loop,int a,int b,bool flag){assert(index==-1&&!loop&&!a&&!b&&!flag);static_cast<Fixture*>(p)->calls.push_back("play");return 0;}
int main(){
 std::array<int,224> sheet{};dh2::data::PropertyView props{};props.resolved=sheet.data();sheet[38]=100;
 std::uint8_t low=1,tutorial=1;Fixture fixture;
 HitPlayerTailBorrowV7 actor{123,123,&props,&low,&tutorial};
 HitPlayerTailServicesV7 services{&fixture,online,player,difficulty,script,start,job,sound,play};
 sheet[36]=50;assert(hit_player_tail_v7(&actor,&services)==1);
 assert(!low&&!tutorial);
 assert((fixture.calls==std::vector<std::string>{"online","difficulty","cinematic_Tuto_potionUse","start","job","sfx_mc_low_hp","play"}));
 fixture.calls.clear();sheet[36]=74;assert(hit_player_tail_v7(&actor,&services)==1&&low==0);
 assert((fixture.calls==std::vector<std::string>{"player"}));
 low=0;sheet[36]=75;fixture.calls.clear();assert(hit_player_tail_v7(&actor,&services)==1&&low);
 low=1;tutorial=1;sheet[36]=1;fixture.fail_job=true;fixture.calls.clear();
 assert(hit_player_tail_v7(&actor,&services)==-2&&!tutorial&&low==1);
 assert(fixture.calls.back()=="job"); // Reached tutorial prefix; audio unreached.
 fixture.fail_job=false;fixture.calls.clear();actor.captured_main_player=456;
 assert(hit_player_tail_v7(&actor,&services)==1&&!low);
 assert(fixture.calls[2]=="sfx_friend_low_hp");
}
