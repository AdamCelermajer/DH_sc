#include "../character_ai_attack.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
using namespace dh2::character;
using Trace=std::array<std::uint32_t,17>;
constexpr std::uintptr_t identities[]={0,0xabcdef0100001000ULL,0xabcdef0100004000ULL,0xabcdef0100006000ULL,0xabcdef0100008000ULL,0xabcdef0100009000ULL,0xabcdef0100001374ULL};
struct List {AttackTargetList24 view{};std::array<std::uintptr_t,4> entries{};};
struct Fixture {
 std::vector<std::uint32_t> words;std::vector<Trace> trace;
 std::vector<std::unique_ptr<List>> lists;AttackServices16 services{};bool mutated=false;
 int invalid_list=0;
 std::uint32_t identity(std::uintptr_t value){
  for(unsigned i=0;i<7;++i)if(value==identities[i])return i;
  for(const auto& list:lists)if(value==reinterpret_cast<std::uintptr_t>(&list->view))return 7;
  assert(false);return 0;
 }
 std::array<std::uint32_t,12> state(const AttackState64& s){return {identity(s.target),identity(s.last_target),s.continued,s.seeking,static_cast<std::uint32_t>(s.index),s.last,s.finisher,s.owner_flags528,s.heading_active,static_cast<std::uint32_t>(s.object_of_interest_type),identity(s.object_of_interest),identity(s.owner)};}
 void mutate(AttackState64* s,std::uint32_t op){
  if(mutated||op!=words[29])return;
  mutated=true;
  switch(words[30]){
   case 1:s->heading_active=1-s->heading_active;break;
   case 2:s->target=identities[4];break;
   case 3:s->target=0;break;
   case 4:s->owner_flags528=1;break;
   case 5:s->last=255;break;
   case 6:s->index=static_cast<std::int32_t>(0xdeadbeef);s->finisher=77;s->last=19;s->seeking=3;s->continued=6;break;
   case 7:assert(dh2_character_ai_melee_attack(s,identities[4],1,&services)==0);break;
   case 8:s->last_target=identities[5];break;
   default:break;
  }
 }
};
void invoke(void* context,AttackState64* state,ControllerAttackState32*,const AttackRequest32* request,AttackResponse16* response){
 auto& f=*static_cast<Fixture*>(context);const auto op=request->service;assert(op<=attack_controllable_dispatch&&request->reserved==0);
 Trace row{op,request->argument0,request->argument1,f.identity(request->subject),f.identity(request->payload)};
 const auto snapshot=f.state(*state);for(unsigned i=0;i<12;++i)row[5+i]=snapshot[i];f.trace.push_back(row);f.mutate(state,op);
 switch(op){
  case attack_owner_dead:response->word=f.words[0];break;
  case attack_owner_ranged:response->word=f.words[2];break;
  case attack_is_attacking:response->word=f.words[3];break;
  case attack_diagnostic:response->word=f.words[11];break;
  case attack_frontal_angle:response->word=f.words[28];break;
  case attack_can_attack_current:response->word=f.words[7];break;
  case attack_target_dead:response->word=f.words[9];break;
  case attack_owner_player:response->word=f.words[10];break;
  case attack_current_in_melee:response->word=f.words[8];break;
  case attack_network_mode:response->word=f.words[20];break;
  case attack_list_create:{
   auto list=std::make_unique<List>();list->view.token=reinterpret_cast<std::uintptr_t>(&list->view);list->view.entries=list->entries.data();
   if(f.invalid_list==1)list->view.count=65537;
   response->identity=list->view.token;f.lists.push_back(std::move(list));break;
  }
  case attack_list_search:{
   auto* list=reinterpret_cast<AttackTargetList24*>(request->payload);list->count=f.words[6];list->cursor=0;
   auto* entries=const_cast<std::uintptr_t*>(list->entries);for(unsigned i=0;i<list->count;++i)entries[i]=identities[3];break;
  }
  case attack_list_pop:{auto* list=reinterpret_cast<AttackTargetList24*>(request->payload);if(f.invalid_list!=2)++list->cursor;break;}
  case attack_set_target:state->target=request->payload;break;
  case attack_sync_last_target:state->last_target=state->target;break;
  case attack_controllable_dispatch:assert(dh2_character_ai_melee_attack(state,request->payload,0,&f.services)==0);break;
  default:break;
 }
}
AttackState64 initial(const std::vector<std::uint32_t>& w){return {identities[1],w[14]?identities[2]:0,w[23]?identities[5]:0,identities[4],w[1],w[5],w[24],w[4],static_cast<std::int32_t>(w[26]),w[15]?8:-1,w[25],w[27]};}
int main(int argc,char** argv){
 assert(argc==2);std::ifstream in(argv[1],std::ios::binary);std::array<std::uint32_t,3> header{};in.read(reinterpret_cast<char*>(header.data()),12);assert(in&&header[0]==0x31414d43&&header[2]==17);
 std::uint64_t callbacks=0,commands=0,nested=0;
 for(std::uint32_t i=0;i<header[1];++i){
  std::uint32_t size=0;in.read(reinterpret_cast<char*>(&size),4);assert(in&&size>=45&&size<=65536);Fixture f;f.words.resize(size);in.read(reinterpret_cast<char*>(f.words.data()),size*4);assert(in&&size==45+f.words[44]*17);f.services={&f,invoke};
  auto state=initial(f.words);ControllerAttackState32 controller{identities[6],f.words[22]?identities[1]:0,f.words[18],f.words[17],f.words[19],f.words[21]};
  const auto requested=f.words[13]?identities[2]:0;
  const int status=f.words[16]?dh2_character_cmd_attack(&controller,&state,requested,&f.services):dh2_character_ai_melee_attack(&state,requested,f.words[12],&f.services);
  assert(status==0&&f.trace.size()==f.words[44]);const auto snapshot=f.state(state);for(unsigned j=0;j<12;++j)assert(snapshot[j]==f.words[32+j]);
  for(std::size_t j=0;j<f.trace.size();++j)for(unsigned k=0;k<17;++k)assert(f.trace[j][k]==f.words[45+j*17+k]);
  callbacks+=f.trace.size();commands+=f.words[16];nested+=f.words[30]==7&&f.mutated;
 }
 assert(in.peek()==std::char_traits<char>::eof());
 Fixture guard;guard.words.resize(32);guard.words[29]=0xffffffff;guard.services={&guard,invoke};auto state=initial(guard.words);const auto snapshot=guard.state(state);ControllerAttackState32 controller{identities[6],identities[1],0,0,0,0};AttackServices16 absent{&guard,nullptr};
 assert(dh2_character_ai_melee_attack(nullptr,0,0,&guard.services)==1);
 assert(dh2_character_ai_melee_attack(&state,0,0,nullptr)==1);
 assert(dh2_character_ai_melee_attack(&state,0,0,&absent)==1);
 assert(dh2_character_ai_melee_attack(&state,0,256,&guard.services)==1);
 assert(dh2_character_cmd_attack(nullptr,&state,0,&guard.services)==1);
 assert(dh2_character_cmd_attack(&controller,&state,0,nullptr)==1);
 assert(dh2_character_cmd_attack(&controller,&state,0,&absent)==1);
 controller.locked=256;assert(dh2_character_cmd_attack(&controller,&state,0,&guard.services)==1);
 assert(guard.trace.empty()&&guard.state(state)==snapshot);
 guard.invalid_list=1;assert(dh2_character_ai_melee_attack(&state,0,0,&guard.services)==2);guard.lists.clear();guard.trace.clear();guard.invalid_list=2;guard.words[6]=guard.words[10]=guard.words[11]=1;state=initial(guard.words);
 assert(dh2_character_ai_melee_attack(&state,0,0,&guard.services)==2);assert(guard.trace.back()[0]==attack_list_destroy);
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<header[1]<<",\"ordered_services\":"<<callbacks<<",\"controller_cases\":"<<commands<<",\"executed_nested_callbacks\":"<<nested<<",\"atomic_guards\":8,\"invalid_provider_checks\":2,\"mismatches\":0}\n";
}
