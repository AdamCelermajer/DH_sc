#include "../character_update_startup.hpp"
#include <algorithm>
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
using Snapshot=std::array<std::uint32_t,6>;
using Event=std::array<std::uint32_t,11>;
using CanEvent=std::array<std::uint32_t,13>;
void require(bool value){if(!value)throw std::runtime_error("Character startup audit mismatch");}
struct Fixture {
 std::array<std::uint32_t,26> p{};std::uint32_t phase=0,state_calls=0,stats=0;
 CanUpdateOwner40 eligibility{};CanUpdateServices24 eligibility_services{};
 UpdateStartupQueue16 queue{};UpdateStartupOwner64 owner{};
 UpdateStartupPlayer24 player{};UpdateStartupServices24 services{};
 std::vector<Event> events;std::vector<CanEvent> can_events;
 int fail=-1;bool can_fail=false;std::uint32_t live_name_checks=0;
 Snapshot snapshot(){return {static_cast<std::uint32_t>(owner.resolved_hp),static_cast<std::uint32_t>(owner.active_ai),eligibility.interaction,stats,queue.count,owner.delayed_load};}
 static int can(void* context,CanUpdateOwner40* input,const CanUpdateRequest24* request,CanUpdateResponse16* response){
  auto& f=*static_cast<Fixture*>(context);require(input==&f.eligibility);
  require(request->operation==can_update_online||request->operation==can_update_dead||request->operation==can_update_respawn);
  f.can_events.push_back({request->operation,0,0,0,100,0,0,f.eligibility.interaction,1,0,0xa5,0,0xa5});
  if(f.can_fail)return 1;
  *response={0,request->operation==can_update_dead&&!f.p[6]?1u:0u,0};return 0;
 }
 static int invoke(void* context,UpdateStartupOwner64* input,const UpdateStartupRequest40* request,UpdateStartupResponse16* response){
  auto& f=*static_cast<Fixture*>(context);require(input==&f.owner&&request->owner==0x1234&&request->reserved==0);
  const auto op=request->operation;std::uint32_t name=0;
  if(request->name){
   if(!std::strcmp(request->name,"KillPlayerOne"))name=1;
   else if(!std::strcmp(request->name,"Give50Potions"))name=2;
   else if(!std::strcmp(request->name,"isABot"))name=3;
   else require(false);
   ++f.live_name_checks;
  }
  Event event{op,request->argument,request->argument2,static_cast<std::uint32_t>(request->subject),name};
  auto state=f.snapshot();std::copy(state.begin(),state.end(),event.begin()+5);f.events.push_back(event);
  if(static_cast<int>(op)==f.fail)return 1;
  std::uint32_t value=0;std::uintptr_t identity=0;
  if(op==update_debug_query){require(name!=0);value=f.p[name-1];}
  else if(op==update_get_player){
   const auto character=f.phase?(f.p[24]?0x7777u:0u):(f.p[4]?0x1234u:0x2222u);
   std::memcpy(&f.player.player_number,&f.p[23],4);f.player.character=character;f.player.bot_enabled=static_cast<std::uint8_t>(f.p[22]);identity=reinterpret_cast<std::uintptr_t>(&f.player);
  }else if(op==update_set_property){require(request->argument==36);std::memcpy(&f.owner.resolved_hp,&request->argument2,4);}
  else if(op==update_is_dead)value=f.p[5];
  else if(op==update_is_player)value=f.p[3];
  else if(op==update_get_state){
   value=f.p[7+std::min(f.state_calls,2u)];++f.state_calls;
   if(f.p[21]==1&&f.state_calls==2)f.eligibility.interaction=1;
  }else if(op==update_get_current_level)value=f.p[12];
  else if(op==update_load_and_init){value=f.p[15];if(f.p[21]==2)f.owner.active_ai=0xabc;}
  else if(op==update_is_monster||op==update_is_miniboss||op==update_is_boss)value=f.p[16+op-update_is_monster];
  else if(op==update_online)value=f.p[19];
  else if(op==update_real_time)value=0x80000000;
  else if(op==update_player_for_character){
   std::memcpy(&f.player.player_number,&f.p[23],4);f.player.character=0x1234;f.player.bot_enabled=static_cast<std::uint8_t>(f.p[22]);identity=reinterpret_cast<std::uintptr_t>(&f.player);
  }
  *response={identity,value,0};return 0;
 }
 void reset(const std::array<std::uint32_t,26>& params,std::uint32_t selected_phase){
  p=params;phase=selected_phase;stats=0xfffffffe;state_calls=0;events.clear();can_events.clear();fail=-1;can_fail=false;live_name_checks=0;
  eligibility={0x1234,nullptr,100,0x1350,0,0,static_cast<std::uint8_t>(p[11]),0,0};eligibility_services={this,can,63,0};queue={p[13],0,p[14]};
  std::int32_t hp;std::memcpy(&hp,&p[25],4);
  owner={&eligibility,&eligibility_services,p[10],0x4400,&stats,&queue,hp,static_cast<std::uint8_t>(p[20]),{},0};player={0x5500,0,0,0,{}};services={this,invoke,(1u<<19)-1,0};
 }
 int run(std::uint32_t& stage){return phase?dh2_character_update_after_controller(&owner,&services,&stage):dh2_character_update_startup(&owner,&services,&stage);}
};
std::uint32_t word(std::istream& input){std::uint32_t value;input.read(reinterpret_cast<char*>(&value),4);require(bool(input));return value;}
int main(int argc,char** argv){try{
 require(argc==2);std::ifstream input(argv[1],std::ios::binary);char magic[4];input.read(magic,4);require(!std::memcmp(magic,"CUS1",4));const auto count=word(input);
 Fixture fixture;std::uint32_t prefixes=0,cans=0,stops=0,names=0;
 for(std::uint32_t i=0;i<count;++i){
  std::array<std::uint32_t,26> p;for(auto& value:p)value=word(input);const auto phase=word(input),status=word(input),expected_stage=word(input);
  Snapshot state;for(auto& value:state)value=word(input);const auto ne=word(input),nc=word(input);
  std::vector<Event> events(ne);for(auto& event:events)for(auto& value:event)value=word(input);
  std::vector<CanEvent> can_events(nc);for(auto& event:can_events)for(auto& value:event)value=word(input);
  fixture.reset(p,phase);std::uint32_t stage=99;require(fixture.run(stage)==static_cast<int>(status));require(stage==expected_stage&&fixture.snapshot()==state&&fixture.events==events&&fixture.can_events==can_events);
  prefixes+=ne;cans+=nc;stops+=status!=0;names+=fixture.live_name_checks;
 }
 require(input.peek()==std::char_traits<char>::eof());std::uint32_t guards=0;
 std::array<std::uint32_t,26> ordinary{};ordinary[6]=1;ordinary[7]=ordinary[8]=ordinary[9]=3;
 auto failure=[&](int expected){std::uint32_t stage=99;require(fixture.run(stage)==expected&&stage==99);++guards;};
 for(int op:{0,1,2,3,10,11,12}){fixture.reset(ordinary,0);fixture.fail=op;failure(3);require(fixture.stats==0xfffffffe);}
 for(int op:{0,1,2,3,10,11,12}){fixture.reset(ordinary,0);fixture.services.available&=~(1u<<op);failure(2);}
 fixture.reset(ordinary,0);fixture.can_fail=true;failure(3);
 fixture.reset(ordinary,0);fixture.services.reserved=1;failure(1);
 fixture.reset(ordinary,0);fixture.owner.reserved[0]=1;failure(1);require(fixture.events.empty());
 fixture.reset(ordinary,0);fixture.owner.eligibility=reinterpret_cast<CanUpdateOwner40*>(reinterpret_cast<std::uintptr_t>(&fixture.eligibility)+1);failure(1);
 fixture.reset(ordinary,0);fixture.queue.reserved=1;failure(1);
 fixture.reset(ordinary,0);fixture.owner.application_updates=nullptr;failure(1);
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<count<<",\"ordered_prefix_services\":"<<prefixes<<",\"ordered_eligibility_services\":"<<cans<<",\"explicit_continuation_stops\":"<<stops<<",\"live_string_checks\":"<<names<<",\"malformed_and_delivery_checks\":"<<guards<<",\"mismatches\":0}\n";
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
