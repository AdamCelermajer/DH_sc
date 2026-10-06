#include "../character_ranged_attack_v41.hpp"
#include <array>
#include <algorithm>
#include <vector>
#include <cstring>
#include <fstream>
#include <cassert>
#include <iostream>
using namespace dh2::character;
struct Fixture {
 std::array<unsigned,16> f{};AttackState64 state{};std::array<std::uintptr_t,2> entries{};AttackTargetList24 list{99,nullptr,0,0};std::vector<std::array<unsigned,4>> trace;int fail=-1;
 explicit Fixture(const std::array<unsigned,16>& input):f(input){state.owner=1;state.target=state.last_target=f[12]?2:0;state.owner_flags528=f[1];state.heading_active=f[5];state.last=f[4];state.continued=55;}
 static unsigned bits(float x){unsigned b;std::memcpy(&b,&x,4);return b;}
 static int invoke(void* raw,AttackState64& state,const RangedAttackRequestV41& q,RangedAttackResponseV41& out,std::string& error){auto& c=*static_cast<Fixture*>(raw);using O=RangedAttackOperationV41;unsigned peer=unsigned(q.other),a=0,b=0;
  if(q.operation==O::debug)a=q.name&&!std::strcmp(q.name,"isTracingChar_MeleePotentialTarget");
  if(q.operation==O::list_create)a=1;
  if(q.operation==O::melee_fallback)a=q.speculative;
  if(q.operation==O::frontal_angle)a=c.f[15];
  if(q.operation==O::list_search){a=bits(q.radius);b=bits(q.cone);}
  if(q.operation==O::list_frontal_sort||q.operation==O::list_search||q.operation==O::list_destroy)peer=0;
  c.trace.push_back({unsigned(q.operation),peer,a,b});if(int(q.operation)==c.fail){error="required fixture endpoint";return -1;}
  switch(q.operation){
  case O::owner_dead:out.word=c.f[0];break;
  case O::range_parameters:out.word=c.f[2];out.parameters[0]=10;std::memcpy(&out.parameters[1],&c.f[14],4);out.parameters[2]=4;break;
  case O::is_attacking:out.word=c.f[3];break;
  case O::list_create:out.list=&c.list;break;
  case O::can_attack_current:out.word=c.f[7];if(c.f[13]==1)state.heading_active=1;break;
  case O::target_dead:out.word=c.f[8];break;
  case O::owner_player:out.word=c.f[9];break;
  case O::frontal_angle:out.word=c.f[15];break;
  case O::list_search:c.entries[0]=3;c.list.entries=c.entries.data();c.list.count=c.f[6];break;
  case O::set_target:state.target=q.other;if(c.f[13]==2)c.entries[0]=4;break;
  default:break;
  }
  return 0;
 }
 int run(std::string& error){return character_ranged_attack_v41(state,f[11]?2:0,f[10],{this,invoke},error);}
};
int main(int argc,char** argv){assert(argc==2);std::ifstream in(argv[1],std::ios::binary);unsigned count{};in.read(reinterpret_cast<char*>(&count),4);assert(in&&count<=1000);unsigned calls=0;std::string error;
 for(unsigned i=0;i<count;++i){std::array<unsigned,19> row{};in.read(reinterpret_cast<char*>(row.data()),76);assert(in&&row[18]<=100);std::vector<std::array<unsigned,4>> want(row[18]);in.read(reinterpret_cast<char*>(want.data()),want.size()*16);assert(in);std::array<unsigned,16> f;std::copy_n(row.data(),16,f.data());Fixture c(f);const int result=c.run(error);
  if(result||c.state.continued!=row[16]||c.state.target!=row[17]||c.trace!=want){std::cerr<<"original ranged mismatch "<<i<<" status "<<result<<" continued "<<c.state.continued<<" target "<<c.state.target<<'\n';for(auto t:c.trace)std::cerr<<t[0]<<','<<t[1]<<','<<t[2]<<','<<t[3]<<' ';std::cerr<<"\nwant ";for(auto t:want)std::cerr<<t[0]<<','<<t[1]<<','<<t[2]<<','<<t[3]<<' ';std::cerr<<'\n';return 1;}calls+=want.size();
 }
 assert(in.peek()==std::char_traits<char>::eof());
 std::array<unsigned,16> f{};f[2]=1;f[6]=1;f[9]=1;f[14]=100;f[15]=90;
 unsigned failures=0;for(int op:{0,1,2,4,5,9,11,12,13,14,15}){Fixture c(f);c.fail=op;assert(c.run(error)<0&&!error.empty());++failures;}
 std::cout<<"ranged AI V41 original replay PASS "<<count<<" cases / "<<calls<<" ordered calls; "<<failures<<" required endpoint failures; projectile creation not claimed\n";
}
