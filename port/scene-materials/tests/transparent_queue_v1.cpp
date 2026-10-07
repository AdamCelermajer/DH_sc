#include "transparent_queue_v1.hpp"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
using namespace dh2::scene;
int main(int argc,char** argv){
 assert(argc==2);std::ifstream input(argv[1],std::ios::binary);assert(input);
 auto word=[&](){std::uint32_t w{};input.read(reinterpret_cast<char*>(&w),4);assert(input);return w;};
 auto floating=[](std::uint32_t bits){float f{};std::memcpy(&f,&bits,4);return f;};
 auto signed_word=[](std::uint32_t bits){std::int32_t w{};std::memcpy(&w,&bits,4);return w;};
 auto owner=std::make_shared<int>(1);std::string error;const auto count=word();
 for(unsigned i=0;i<count;++i){
  std::array<std::uint32_t,15> w{};for(auto& value:w)value=word();
  TransparentEntryV1 a{w[0],w[1],w[2],signed_word(w[3]),floating(w[4]),owner,owner};
  TransparentEntryV1 b{w[5],w[6],w[7],signed_word(w[8]),floating(w[9]),owner,owner};
  TransparentQueueServicesV1 s;
  s.material_equal_3537b0=[&](auto x,auto y,bool& result,auto&){assert(x==a.material&&y==b.material);result=w[10]!=0;return true;};
  s.material_less_3537e4=[&](auto x,auto y,bool& result,auto&){assert(x==a.material&&y==b.material);result=w[11]!=0;return true;};
  s.node_suborder_20=[&](auto node,auto part,std::int32_t& result,auto&){assert((node==a.node&&part==a.part)||(node==b.node&&part==b.part));result=signed_word(node==a.node?w[12]:w[13]);return true;};
  bool result{};assert(transparent_less_v1(a,b,s,result,error)&&result==(w[14]!=0));
 }
 unsigned calls=0;TransparentQueueServicesV1 s;
 s.node_priority_d8=[&](auto,std::int32_t& value,auto&){assert(calls++==0);value=-2;return true;};
 s.node_position_38=[&](auto,auto& value,auto&){assert(calls++==1);value={4,6,8};return true;};
 s.node_distance_bias_d0=[&](auto,float& value,auto&){assert(calls++==2);value=2;return true;};
 TransparentEntryV1 a{0x100000010ull,0,0,0,0,owner,{}};
 assert(transparent_entry_v1(a,{1,2,3},nullptr,0x7fffffff,s,error)&&a.priority==-2&&a.distance==52&&calls==3);
 TransparentQueueV1 q;std::array<float,3> supplied{1,2,3};s.node_distance_bias_d0=[](auto,float& value,auto&){value=1;return true;};
 assert(q.append(a,{1,2,3},&supplied,4,s,error)&&q.entries().size()==1&&q.entries()[0].distance==1);
 q.clear();assert(q.entries().empty());
 TransparentEntryV1 b=a;a.material=1;b.material=2;a.priority=b.priority=0;a.distance=b.distance=0;
 bool out=true;assert(!transparent_less_v1(a,b,{},out,error)&&out&&error.find("material equality")!=std::string::npos);
 std::cout<<"Transparent comparator original gold PASS cases="<<count<<"; entry publication/actual-provider failure/native ownership PASS\n";
}
