#include "../transparent_sort_v2.hpp"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
int main(int argc,char** argv){
 assert(argc==2);std::ifstream in(argv[1],std::ios::binary);assert(in);
 auto word=[&](){std::uint32_t w;in.read(reinterpret_cast<char*>(&w),4);assert(in);return w;};
 const auto cases=word();
 for(std::uint32_t c=0;c<cases;++c){
  const auto count=word();std::vector<dh2::scene::TransparentEntryV1> input(count),expected(count);
  for(auto* entries:{&input,&expected})for(auto& entry:*entries){
   entry.node=word();entry.part=word();entry.material=word();
   const auto priority=word(),distance=word();std::memcpy(&entry.priority,&priority,4);std::memcpy(&entry.distance,&distance,4);
  }
  std::string error;assert(dh2::scene::transparent_sort_v2(input,{},error));
  for(std::size_t i=0;i<count;++i){const auto& a=input[i];const auto& b=expected[i];assert(a.node==b.node&&a.part==b.part&&a.material==b.material&&a.priority==b.priority&&std::memcmp(&a.distance,&b.distance,4)==0);}
 }
 // Genuine required services reject instead of supplying guessed tie order.
 std::vector<dh2::scene::TransparentEntryV1> missing(2);missing[0].material=1;missing[1].material=2;
 std::string error;assert(!dh2::scene::transparent_sort_v2(missing,{},error));assert(!error.empty());
 std::cout<<"PASS "<<cases<<" complete original transparent heapsort cases + required material service rejection\n";
}
