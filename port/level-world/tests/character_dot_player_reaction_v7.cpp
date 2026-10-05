#include "../character_dot_player_reaction_v7.hpp"
#include <cassert>
#include <initializer_list>
using namespace dh2;
struct Fixture {bool idle;int calls;int failure;};
int query(void* p,std::uintptr_t identity,bool* answer){
 auto& f=*static_cast<Fixture*>(p);assert(identity==0x1234);++f.calls;
 *answer=f.idle;return f.failure;
}
int main(){
 unsigned cases=0;
 for(unsigned idle=0;idle<2;++idle)for(unsigned bits=0;bits<512;++bits)
  for(int amount: {-256,0,256}) {
   Fixture f{idle!=0,0,0};character::DotPlayerReactionServicesV7 s{&f,query};
   data::CombatResult result{};result.amount=amount;result.outcomes=bits;
   unsigned expected=bits;
   if(idle&&(bits&0x16)==0)expected|=amount>0?0x10:2;
   assert(character::dot_player_reaction_v7(&result,0x1234,&s)==1);
   assert(result.outcomes==expected&&f.calls==1);++cases;
  }
 Fixture f{true,0,1};character::DotPlayerReactionServicesV7 s{&f,query};
 data::CombatResult result{};result.amount=256;result.outcomes=0x100;
 assert(character::dot_player_reaction_v7(&result,0x1234,&s)==-2);
 assert(result.outcomes==0x100&&f.calls==1);
 assert(cases==3072);
}
