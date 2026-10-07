#include "../authored_fx_alignment_v39.hpp"
#include <fstream>
#include <cassert>
#include <cstring>
#include <iostream>
int main(int argc,char** argv){assert(argc==2);std::ifstream f(argv[1],std::ios::binary);unsigned count{};f.read(reinterpret_cast<char*>(&count),4);assert(f&&count<=1000);std::string error;
 for(unsigned i=0;i<count;++i){dh2::math::Matrix4f a{},b{},result{};unsigned char expected[65]{};f.read(reinterpret_cast<char*>(&a),68);f.read(reinterpret_cast<char*>(&b),68);f.read(reinterpret_cast<char*>(expected),65);assert(f);
  assert(dh2::fx::authored_fx_draw_world_v39(result,dh2::fx::AuthoredPositionSpaceV39::node_local,&a,&b,error));if(std::memcmp(&result,expected,65)){std::cerr<<"original matrix mismatch "<<i<<'\n';return 1;}
 }
 assert(f.peek()==std::char_traits<char>::eof());std::cout<<"original Matrix4f35e998 -> native alignment contract PASS "<<count<<" defined65-byte cases\n";
}
