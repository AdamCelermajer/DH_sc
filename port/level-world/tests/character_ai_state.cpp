#include "../character_ai_state.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
using namespace dh2::character;
struct Fixture {std::array<std::uint32_t,27> words;std::vector<std::array<std::uint32_t,3>> trace;};
constexpr std::uintptr_t owners[2]={0xabcdef0100002000ULL,0xabcdef0100004000ULL},target=0xbcdef01200006000ULL;
std::uint32_t query(void* context,AttackQueryState16* state,const AttackQueryRequest24* request){
 auto& f=*static_cast<Fixture*>(context);const auto k=request->service;
 assert(k<5&&request->reserved==0&&request->target==target);
 const std::uint32_t owner=request->owner==owners[0]?0:1;assert(request->owner==owners[owner]);
 f.trace.push_back({k,owner,2});
 if(f.words[7]&(1U<<k)){state->owner=owners[owner^1];state->target=owners[1];}
 return f.words[2+k];
}
int main(int argc,char** argv){
 assert(argc==2);std::ifstream in(argv[1],std::ios::binary);std::array<std::uint32_t,3> header{};in.read(reinterpret_cast<char*>(header.data()),12);assert(in&&header[0]==0x31535141&&header[2]==108);
 std::uint64_t callbacks=0;
 for(std::uint32_t i=0;i<header[1];++i){
  Fixture f{};in.read(reinterpret_cast<char*>(f.words.data()),108);assert(in);
  AttackQueryState16 state{owners[0],f.words[1]?target:0};AttackQueryServices16 services{&f,query};std::uint32_t result=0xdeadbeef;
  assert(dh2_character_ai_can_attack(&state,f.words[0]?target:0,&services,&result)==0&&result==f.words[8]);
  assert(state.owner==owners[f.words[9]]&&(state.target==0?0:state.target==target?2:3)==f.words[10]);
  assert(f.trace.size()==f.words[11]);
  for(std::size_t n=0;n<f.trace.size();++n)for(unsigned j=0;j<3;++j)assert(f.trace[n][j]==f.words[12+n*3+j]);
  callbacks+=f.trace.size();
 }
 assert(in.peek()==std::char_traits<char>::eof());
 Fixture f{};AttackQueryState16 state{owners[0],target};AttackQueryServices16 services{&f,query};std::uint32_t result=0x12345678;const auto before=state;
 assert(dh2_character_ai_can_attack(nullptr,0,&services,&result)==1);
 assert(dh2_character_ai_can_attack(&state,0,nullptr,&result)==1);
 assert(dh2_character_ai_can_attack(&state,0,&services,nullptr)==1);
 assert(dh2_character_ai_can_attack(&state,0,&services,reinterpret_cast<std::uint32_t*>(&state))==1);
 AttackQueryServices16 missing{&f,nullptr};assert(dh2_character_ai_can_attack(&state,0,&missing,&result)==1);
 assert(result==0x12345678&&state.owner==before.owner&&state.target==before.target&&f.trace.empty());
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<header[1]<<",\"ordered_queries\":"<<callbacks<<",\"atomic_guards\":5,\"mismatches\":0}\n";
}
