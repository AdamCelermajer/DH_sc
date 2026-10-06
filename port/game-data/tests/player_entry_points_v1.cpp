#include "../player_savegame_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>
using namespace dh2::data;
using Buffer=std::vector<std::uint8_t>;
void check(bool v,const char* m){if(!v)throw std::runtime_error(m);}
struct Reader {
 Buffer b;std::size_t at{};
 std::uint32_t word(){check(b.size()-at>=4,"fixture word");auto p=b.data()+at;at+=4;return std::uint32_t(p[0])|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;}
 Buffer bytes(std::size_t n){check(n<=b.size()-at,"fixture bytes");Buffer out(b.begin()+at,b.begin()+at+n);at+=n;return out;}
};
int main(int argc,char** argv){try{
 check(argc==2,"usage original-fixture");std::ifstream f(argv[1],std::ios::binary);check(bool(f),"fixture missing");Reader r{{std::istreambuf_iterator<char>(f),{}}};
 check(r.bytes(4)==Buffer({'E','P','S','1'}),"fixture signature");auto count=r.word();
 for(unsigned i=0;i<count;++i){auto entries=r.bytes(r.word()),spawn=r.bytes(r.word());auto expected_entries=r.bytes(12),expected_spawn=r.bytes(3);
  PlayerSavegameV1 owner;std::size_t used;std::string error;
  check(owner.load_entry_points({entries.data(),entries.size()},used,error)&&used==12,"entry reader cursor");
  for(unsigned j=0;j<3;++j){auto p=expected_entries.data()+4*j;auto v=std::uint32_t(p[0])|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;check(std::uint32_t(owner.location().entry_points[j])==v,"original entry mismatch");}
  check(owner.load_spawn_points({spawn.data(),spawn.size()},used,error)&&used==3,"spawn reader cursor");
  check(!std::memcmp(owner.location().use_spawn_point.data(),expected_spawn.data(),3),"original raw spawn mismatch");
 }
 check(r.at==r.b.size(),"fixture remainder");
 const std::uint8_t entries[]={7,0,0,0,9,0,0,0,11,0,0,0},spawn[]={2,128,255};
 for(std::size_t n=0;n<12;++n){PlayerSavegameV1 owner;std::size_t used=99;std::string error;
  check(!owner.load_entry_points({entries,n},used,error)&&used==(n/4)*4,"truncated entry cursor");
  for(unsigned i=0;i<3;++i)check(owner.location().entry_points[i]==(i<n/4?7+2*int(i):0),"entry reached stores");
 }
 for(std::size_t n=0;n<3;++n){PlayerSavegameV1 owner;std::size_t used=99;std::string error;
  check(!owner.load_spawn_points({spawn,n},used,error)&&used==n,"truncated spawn cursor");
  for(unsigned i=0;i<3;++i)check(owner.location().use_spawn_point[i]==(i<n?spawn[i]:0),"spawn reached stores");
 }
 std::cout<<"PASS "<<count<<" original LEPT/LUSP cases, signed/raw-byte boundaries, trailing bytes and 15 truncation prefixes\n";
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
