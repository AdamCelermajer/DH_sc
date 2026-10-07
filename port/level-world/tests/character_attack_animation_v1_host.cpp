#include "character_attack_animation_v1.hpp"
#include <array>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cstring>
using namespace dh2::character;
using Words=std::array<std::uint32_t,8>;
struct Case {
 std::array<std::uint32_t,16> input{};
 std::vector<Words> trace;
 static int call(void* p,AttackState64& a,const AttackAnimationRequestV1& q,std::uint32_t& out){
  auto& c=*static_cast<Case*>(p);const auto& in=c.input;out=0;
  if(q.service==0)out=in[2];
  if(q.service==1)out=in[3];
  if(q.service==3&&in[13])a.last=in[14];
  if(q.service==5)out=in[10];
  if(q.service==6)a.target=0;
  if(q.service==8)out=in[11];
  if(q.service==9){out=in[12];if(in[15])a.target=0;}
  c.trace.push_back({q.service,q.value,std::uint32_t(q.subject),a.continued,a.last,a.finisher,
       std::uint32_t(a.index),std::uint32_t(bool(a.target))});return 0;
 }
};
template<class T>void load(std::ifstream& f,T& value){
 f.read(reinterpret_cast<char*>(&value),sizeof value);if(!f)throw std::runtime_error("truncated original reference");
}
int main(int argc,char** argv){
 if(argc!=2)return 2;
 std::ifstream f(argv[1],std::ios::binary);std::uint32_t magic,count;load(f,magic);load(f,count);
 if(magic!=0x31414143)return 3;
 std::size_t callbacks=0;
 for(std::uint32_t i=0;i<count;++i){
  Case c;load(f,c.input);std::array<std::uint32_t,6> expected;load(f,expected);
  std::vector<Words> trace(expected[5]);for(auto& row:trace)load(f,row);
  const auto& in=c.input;
  AttackState64 a{0x100000001ull,in[8],0,0,0,0,in[4],in[5],0,
       std::int32_t(in[9]),in[6],0};std::memcpy(&a.index,&in[7],4);
  const std::uint32_t phase=in[1];const std::uintptr_t look=2;
  const AttackAnimationBorrowV1 b{&a,&phase,&look};const AttackAnimationServicesV1 s{&c,Case::call};
  const int r=in[0]?dh2_character_attack_animation_end_v1(&b,&s):dh2_character_attack_animation_begin_v1(&b,&s);
  const std::array<std::uint32_t,5> actual{a.continued,a.last,a.finisher,std::uint32_t(a.index),std::uint32_t(bool(a.target))};
  if(r!=1||!std::equal(actual.begin(),actual.end(),expected.begin())||c.trace!=trace)
   throw std::runtime_error("original comparison mismatch "+std::to_string(i));
  callbacks+=trace.size();
 }
 if(f.peek()!=EOF)throw std::runtime_error("reference trailing bytes");
 // Entry guards execute no callback; reached failure retains first source store.
 AttackState64 a{};std::uint32_t phase=0;std::uintptr_t target=0;Case c;
 AttackAnimationBorrowV1 b{&a,&phase,&target};AttackAnimationServicesV1 s{&c,Case::call};
 if(dh2_character_attack_animation_begin_v1(nullptr,&s)!=-1||dh2_character_attack_animation_end_v1(&b,nullptr)!=-1||!c.trace.empty())return 4;
 struct Failure {
  static int call(void*,AttackState64&,const AttackAnimationRequestV1& q,std::uint32_t& out){
   out=q.service==0?7:9;return q.service==attack_anim_raise_v1?-1:0;
  }
 };
 s.invoke=Failure::call;
 if(dh2_character_attack_animation_begin_v1(&b,&s)!=-1||a.index!=7)return 5;
 std::cout<<"PASS original cases "<<count<<" ordered callbacks "<<callbacks<<" guards/failure prefix 3\n";
}
