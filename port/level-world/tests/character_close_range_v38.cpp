#include "../character_close_range_v38.hpp"
#include "../character_target_update.hpp"
#include <cassert>
#include <cstring>
#include <limits>
#include <vector>
#include <iostream>
#include <fstream>
using namespace dh2::character;
struct F {
 std::vector<CloseRangeOperationV38> calls;std::uintptr_t expected=22;
 std::int32_t kind=0,interaction=8,range=1,minimum=10,tracing=0;float p[3]{3,4,0};int fail=-1;
 static int invoke(void* raw,const CloseRangeRequestV38& q,CloseRangeResponseV38& r,std::string& error){
  auto& f=*static_cast<F*>(raw);f.calls.push_back(q.operation);
  if(int(f.calls.size())==f.fail){error="fixture missing source endpoint";return -1;}
  using O=CloseRangeOperationV38;
  if(q.operation==O::resolve_character){assert(q.subject==f.expected);r.character=q.subject;}
  else if(q.operation==O::object_kind)r.word=f.kind;
  else if(q.operation==O::interaction_type){assert(q.other==11);r.word=f.interaction;}
  else if(q.operation==O::interaction_range){assert(q.subject==11&&q.other==f.expected);r.word=17;}
  else if(q.operation==O::range_parameters){assert(q.subject==11);r.word=f.range;r.limits[0]=f.minimum;}
  else if(q.operation==O::position){if(q.subject!=11)std::memcpy(r.position,f.p,12);}
  else if(q.operation==O::debug_query){assert(q.name);r.word=f.tracing;}
  return 0;
 }
 int run(int& out,std::string& e,std::uintptr_t explicit_id=22,std::uintptr_t current=33){return character_close_range_v38(out,11,explicit_id,current,{this,invoke},e);}
};
int main(int argc,char** argv){unsigned checks=0;std::string error;int out=99;F f;
 assert(character_close_range_v38(out,11,0,0,{},error)==0&&out==0);++checks;
 assert(f.run(out,error)==0&&out==1&&f.calls.size()==8);++checks;
 f.calls.clear();f.expected=33;assert(!f.run(out,error,0,33)&&f.calls.size()==8);++checks;
 f.expected=22;f.calls.clear();f.p[0]=6;f.p[1]=8;assert(!f.run(out,error)&&out==0);++checks; // equality is NOT close
 f.calls.clear();f.p[0]=0;f.p[1]=0;f.p[2]=11;assert(!f.run(out,error)&&out==0);++checks; // 3D, no flat radius
 f.calls.clear();f.range=0;assert(!f.run(out,error)&&out==0&&f.calls.size()==4);++checks;
 f.range=1;f.calls.clear();f.kind=7;assert(!f.run(out,error)&&out==17&&f.calls.size()==3);++checks;
 f.kind=0;f.calls.clear();f.interaction=2;assert(!f.run(out,error)&&out==17&&f.calls.size()==4);++checks;
 f.interaction=8;f.calls.clear();f.tracing=1;assert(!f.run(out,error)&&f.calls.size()==10);++checks;
 for(int n=1;n<=10;++n){F failed;failed.tracing=1;failed.fail=n;out=77;assert(failed.run(out,error)<0&&out==77&&error=="fixture missing source endpoint"&&failed.calls.size()==unsigned(n));++checks;}
 const float origin[3]{};float point[3]{1,0,0};
 assert(character_close_distance_v38(origin,point,65536)==0);++checks; // source signed32 MUL wrap
 assert(character_close_distance_v38(origin,point,-2)==1);++checks;
 point[0]=std::numeric_limits<float>::quiet_NaN();assert(character_close_distance_v38(origin,point,10)==0);++checks;
 assert(character_close_distance_v38(nullptr,point,10)<0);++checks;
 if(argc==2){std::ifstream gold(argv[1],std::ios::binary);std::uint32_t count{};gold.read(reinterpret_cast<char*>(&count),4);assert(gold&&count<=10000);for(unsigned i=0;i<count;++i){float p[6];std::int32_t m;std::uint32_t expected;gold.read(reinterpret_cast<char*>(p),24);gold.read(reinterpret_cast<char*>(&m),4);gold.read(reinterpret_cast<char*>(&expected),4);assert(gold&&character_close_distance_v38(p,p+3,m)==int(expected));++checks;}assert(gold.peek()==std::char_traits<char>::eof());}
 std::cout<<"close-range V38 source order/3D/strict-boundary/failure prefix PASS "<<checks<<" checks; endpoint facts are fixtures\n";
}
