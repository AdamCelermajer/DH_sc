#include "../character_can_update.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
using Snapshot=std::array<std::uint32_t,10>;
using Event=std::array<std::uint32_t,13>;
void require(bool b){if(!b)throw std::runtime_error("CanUpdate audit mismatch");}
struct Fixture {
 std::array<std::uint32_t,12> p{};
 CanUpdateNode8 nodes[2]{};
 CanUpdateVisual8 visuals[2]{};
 CanUpdateOwner40 owner{};
 CanUpdateServices24 services{};
 std::vector<Event> events;
 int depth=0,fail=-1,bad=-1;
 Snapshot snapshot(){return {owner.visual==nullptr?0u:owner.visual==&visuals[0]?1u:2u,
  static_cast<std::uint32_t>(owner.player_link),owner.enabled,owner.force_update,owner.interaction,
  visuals[0].node==&nodes[0]?1u:2u,nodes[0].culling,nodes[0].animate_enabled,nodes[1].culling,nodes[1].animate_enabled};}
 static int invoke(void* context,CanUpdateOwner40* owner,const CanUpdateRequest24* request,CanUpdateResponse16* response){
  auto& f=*static_cast<Fixture*>(context);require(owner==&f.owner);
  const auto op=request->operation;require(op<6&&request->owner==0x1234);
  require(request->argument==(op==2?1u:0u)&&request->subject==(op==3?0x1350u:0u));
  Event event{op,request->argument,static_cast<std::uint32_t>(request->subject)};
  auto state=f.snapshot();std::copy(state.begin(),state.end(),event.begin()+3);f.events.push_back(event);
  if(static_cast<int>(op)==f.fail)return 1;
  const auto mode=f.p[11];
  if(op==0&&mode==1)owner->visual=&f.visuals[1];
  if(op==2&&mode==2)owner->player_link=200;
  if(op==2&&mode==3)f.nodes[0].culling=0;
  if(op==2&&mode==4)owner->visual=&f.visuals[1];
  if(op==3&&mode==5)owner->interaction=1;
  if(op==4&&mode==6)owner->enabled=1;
  if(op==4&&mode==7)f.visuals[0].node=&f.nodes[1];
  if(op==1&&mode==9)owner->enabled=0;
  const std::uint32_t values[]{f.p[1],f.p[2],f.p[3]?100u:200u,f.p[6],f.p[8],f.p[9]};
  auto value=values[op];if(f.depth&&op==2)value=100;if(f.depth&&op==4)value=0;
  *response={op==2?value:0,op==2?0:value,0};
  if(op==3&&mode==8&&!f.depth){
   f.depth=1;owner->visual=&f.visuals[1];owner->player_link=100;
   std::uint32_t nested=99;require(dh2_character_can_update(owner,&f.services,&nested)==0&&nested==1);response->word=1;
  }
  if(static_cast<int>(op)==f.bad){
   if(op==0)response->word=256;
   else if(op==2)owner->visual=reinterpret_cast<CanUpdateVisual8*>(reinterpret_cast<std::uintptr_t>(&f.visuals[0])+1);
   else if(op==4)f.visuals[0].node=reinterpret_cast<CanUpdateNode8*>(reinterpret_cast<std::uintptr_t>(&f.nodes[0])+1);
  }
  return 0;
 }
 void reset(const std::array<std::uint32_t,12>& input){
  p=input;depth=0;fail=-1;bad=-1;events.clear();
  nodes[0]={p[4],0xa5,{}};nodes[1]={0,0xa5,{}};
  visuals[0]={&nodes[0]};visuals[1]={&nodes[1]};
  owner={0x1234,p[0]?&visuals[0]:nullptr,100,0x1350,static_cast<std::uint8_t>(p[10]),static_cast<std::uint8_t>(p[5]),static_cast<std::uint8_t>(p[7]),0,0};
  services={this,invoke,63,0};
 }
};
std::uint32_t word(std::istream& stream){std::uint32_t v;stream.read(reinterpret_cast<char*>(&v),4);require(bool(stream));return v;}
int main(int argc,char** argv){try{
 require(argc==2);std::ifstream input(argv[1],std::ios::binary);char magic[4];input.read(magic,4);require(std::memcmp(magic,"CUF1",4)==0);
 const auto count=word(input);std::uint32_t total_events=0;Fixture fixture;
 for(std::uint32_t i=0;i<count;++i){
  std::array<std::uint32_t,12> p;for(auto& x:p)x=word(input);
  auto expected=word(input);Snapshot state;for(auto& x:state)x=word(input);
  auto n=word(input);std::vector<Event> events(n);for(auto& e:events)for(auto& x:e)x=word(input);
  fixture.reset(p);std::uint32_t accepted=0xa5;require(dh2_character_can_update(&fixture.owner,&fixture.services,&accepted)==0);
  require(accepted==expected&&fixture.snapshot()==state&&fixture.events==events);total_events+=n;
 }
 require(input.peek()==std::char_traits<char>::eof());std::uint32_t guards=0;
 const std::array<std::uint32_t,12> base{1,1,0,0,1,0,1,0,1,0,0,0};
 auto check=[&](int expected,const CanUpdateServices24* service){std::uint32_t out=0xa5;require(dh2_character_can_update(&fixture.owner,service,&out)==expected&&out==0xa5);++guards;};
 for(int op=0;op<6;++op){fixture.reset(base);fixture.fail=op;check(3,&fixture.services);require(fixture.nodes[0].animate_enabled==0);}
 for(int op=0;op<6;++op){fixture.reset(base);fixture.services.available&=~(1u<<op);check(2,&fixture.services);}
 fixture.reset(base);check(2,nullptr);
 fixture.reset(base);fixture.services.invoke=nullptr;check(2,&fixture.services);
 fixture.reset(base);fixture.services.reserved=1;check(1,&fixture.services);
 fixture.reset(base);fixture.owner.reserved=1;check(1,&fixture.services);require(fixture.nodes[0].animate_enabled==0xa5);
 fixture.reset(base);fixture.owner.visual=reinterpret_cast<CanUpdateVisual8*>(reinterpret_cast<std::uintptr_t>(&fixture.visuals[0])+1);check(1,&fixture.services);
 fixture.reset(base);fixture.visuals[0].node=reinterpret_cast<CanUpdateNode8*>(reinterpret_cast<std::uintptr_t>(&fixture.nodes[0])+1);check(1,&fixture.services);
 for(int op:{0,2,4}){fixture.reset(base);fixture.owner.enabled=1;fixture.bad=op;check(1,&fixture.services);}
 fixture.reset(base);std::uint32_t accepted=0xa5;require(dh2_character_can_update(nullptr,&fixture.services,&accepted)==1&&accepted==0xa5);++guards;
 require(dh2_character_can_update(&fixture.owner,&fixture.services,reinterpret_cast<std::uint32_t*>(&fixture.owner))==1);++guards;
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<count<<",\"ordered_services\":"<<total_events<<",\"malformed_and_delivery_checks\":"<<guards<<",\"mismatches\":0}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
