#include "character_world_ai_can_attack_v1.hpp"
#include <cassert>
#include <vector>
using namespace dh2::character;
struct Fixture {unsigned bits;std::vector<WorldAIAttackQueryV1> calls;};
static bool query(void* p,std::uintptr_t owner,std::uintptr_t target,WorldAIAttackQueryV1 q,std::int32_t& v,std::string&){
 assert(owner==7&&target==9);auto& f=*static_cast<Fixture*>(p);f.calls.push_back(q);v=(f.bits>>static_cast<unsigned>(q))&1;return true;
}
int main(){
 std::string error;bool result;
 assert(world_ai_can_attack_v1(7,0,0,{},result,error)&&!result);
 assert(!world_ai_can_attack_v1(7,9,0,{},result,error));
 for(unsigned b=0;b<32;++b){Fixture f{b,{}};WorldAIAttackServicesV1 s{&f,query};
  assert(world_ai_can_attack_v1(7,0,9,s,result,error));
  assert(result==bool((b&1)&&(((b&2)&&(b&4))||((b&8)&&(b&16)))));
  assert(f.calls.front()==WorldAIAttackQueryV1::IsEnemy);
  if(!(b&1))assert(f.calls.size()==1);
  else if((b&2)&&(b&4))assert(f.calls.size()==3);
  else assert(f.calls.back()==((b&8)?WorldAIAttackQueryV1::IsInRange:WorldAIAttackQueryV1::CharacterCanRangeAttack));
 }
}
