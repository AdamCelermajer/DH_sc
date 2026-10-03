#include "../character_body_config.hpp"
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::physical;
static void require(bool value,const char* message){if(!value)throw std::runtime_error(message);}
template<class T>static T read(std::ifstream& in){T result{};in.read(reinterpret_cast<char*>(&result),sizeof result);require(bool(in),"fixture truncated");return result;}
static bool floats_equal(const void* a,const void* b,std::size_t n){
 const auto* x=static_cast<const unsigned char*>(a);const auto* y=static_cast<const unsigned char*>(b);
 for(std::size_t i=0;i<n;i+=4){float p,q;std::memcpy(&p,x+i,4);std::memcpy(&q,y+i,4);if(std::memcmp(x+i,y+i,4)&&!(std::isnan(p)&&std::isnan(q)))return false;}return true;
}
static bool equal(const CharacterBodyConfig& expected,const CharacterBodyConfig& actual){
 const auto* x=reinterpret_cast<const unsigned char*>(&expected);const auto* y=reinterpret_cast<const unsigned char*>(&actual);
 return !std::memcmp(x,y,8)&&!std::memcmp(x+44,y+44,36)&&!std::memcmp(x+136,y+136,16)&&!std::memcmp(x+156,y+156,52)&&floats_equal(x+8,y+8,36)&&floats_equal(x+80,y+80,56)&&floats_equal(x+152,y+152,4);
}
int main(int argc,char** argv){try{
 require(argc==2,"character body config test requires original fixture");std::ifstream in(argv[1],std::ios::binary);require(read<std::uint32_t>(in)==0x31434243,"fixture magic");const auto count=read<std::uint32_t>(in);std::uint64_t requests=0;
 for(std::uint32_t i=0;i<count;++i){auto input=read<CharacterBodyInput>(in);const auto expected=read<CharacterBodyConfig>(in);CharacterBodyConfig actual{};require(dh2_character_body_config(&actual,&input)==0,"config rejection");if(!equal(expected,actual)){std::cerr<<"configuration fixture "<<i<<"\n";throw std::runtime_error("configuration differs");}requests+=actual.request_count;}
 require(in.peek()==std::char_traits<char>::eof(),"fixture suffix");
 CharacterBodyInput input{};input.owner=reinterpret_cast<void*>(0x1000000012345678ull);input.new_physical=reinterpret_cast<void*>(0x200000009abcdef0ull);input.character_type=4;CharacterBodyConfig out;std::memset(&out,0x5a,sizeof out);const auto before=out;
 require(dh2_character_body_config(nullptr,&input)==-1,"null output");require(dh2_character_body_config(&out,nullptr)==-1,"null input");input.reserved=1;require(dh2_character_body_config(&out,&input)==-1,"reserved input");input.reserved=0;input.is_player=2;require(dh2_character_body_config(&out,&input)==-1,"boolean input");input.is_player=0;input.new_physical=nullptr;require(dh2_character_body_config(&out,&input)==-1,"missing physical identity");require(!std::memcmp(&before,&out,sizeof out),"rejection mutated output");
 std::cout<<"{\"comparisons\":"<<count<<",\"service_requests\":"<<requests<<",\"malformed_checks\":5,\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
