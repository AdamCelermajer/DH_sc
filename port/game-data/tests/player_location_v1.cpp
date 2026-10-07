#include "../player_savegame_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>
using namespace dh2::data;
using Buffer=std::vector<std::uint8_t>;
void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
struct Reader{
 Buffer bytes;std::size_t at{};
 std::uint32_t word(){check(at+4<=bytes.size(),"fixture word");std::uint32_t v=0;for(unsigned i=0;i<4;++i)v|=std::uint32_t(bytes[at++])<<(8*i);return v;}
 Buffer take(std::size_t n){check(n<=bytes.size()-at,"fixture span");Buffer b(bytes.begin()+at,bytes.begin()+at+n);at+=n;return b;}
};
Buffer projection(const PlayerSavegameV1& saved){
 const auto& v=saved.location();Buffer out;
 auto put=[&](std::uint32_t x){for(unsigned i=0;i<4;++i)out.push_back(std::uint8_t(x>>(8*i)));};
 put(v.save_date);for(auto x:v.levels)put(std::uint32_t(x));for(auto x:v.seeds)put(std::uint32_t(x));
 for(auto x:v.current_acts)put(std::uint32_t(x));for(auto x:v.volatile_acts)put(std::uint32_t(x));return out;
}
int main(int argc,char** argv){try{
 check(argc==2,"usage fixture");std::ifstream f(argv[1],std::ios::binary);check(bool(f),"fixture missing");Reader r{{std::istreambuf_iterator<char>(f),{}}};
 check(r.take(4)==Buffer({'L','O','C','1'}),"signature");auto defaults=r.take(52);auto count=r.word();
 PlayerSavegameV1 blank;check(projection(blank)==defaults,"original constructor location defaults");
 for(unsigned i=0;i<count;++i){
  auto b=r.take(r.word()),expected=r.take(52);PlayerSavegameV1 saved;std::string error;std::size_t used=0;
  check(saved.load_location({b.data(),b.size()},used,error)&&used==40&&error.empty(),"LNAM byte consumption");
  check(projection(saved)==expected,"original location projection mismatch");
  if(i==0){
   // Truncation after date/level/seed must retain those writes and leave the
   // paired current-act stores untouched until their complete word arrives.
   for(unsigned n:{0u,3u,4u,8u,12u,15u}){
    PlayerSavegameV1 short_read;used=99;check(!short_read.load_location({b.data(),n},used,error),"truncated LNAM accepted");
    check(used==(n/4)*4,"truncated LNAM cursor");const auto& v=short_read.location();
    auto raw=[&](unsigned at){std::uint32_t x=0;for(unsigned k=0;k<4;++k)x|=std::uint32_t(b[at+k])<<(8*k);return x;};
    check(v.save_date==(n>=4?raw(0):0),"date prefix store");
    check(std::uint32_t(v.levels[0])==(n>=8?raw(4):0),"level prefix store");
    check(std::uint32_t(v.seeds[0])==(n>=12?raw(8):0),"seed prefix store");
    check(v.current_acts[0]==1&&v.volatile_acts[0]==1,"paired act store before complete input");
   }
  }
 }
 check(r.at==r.bytes.size(),"fixture trailing bytes");
 std::cout<<"PASS original location reader: "<<count<<" cases, constructor defaults, trailing input and six prefix truncations\n";
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
