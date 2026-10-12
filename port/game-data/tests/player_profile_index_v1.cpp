#include "../player_profile_index_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::data;using B=std::vector<std::uint8_t>;
void check(bool x){if(!x)throw std::runtime_error("profile index mismatch");}
struct R{B b;std::size_t at{};std::uint32_t word(){check(at+4<=b.size());std::uint32_t x=0;for(int i=0;i<4;++i)x|=std::uint32_t(b[at++])<<(8*i);return x;}B bytes(std::size_t n){check(n<=b.size()-at);B x(b.begin()+at,b.begin()+at+n);at+=n;return x;}};
int main(int argc,char** argv){try{check(argc==2);std::ifstream f(argv[1],std::ios::binary);check(bool(f));R r{{std::istreambuf_iterator<char>(f),{}}};check(r.bytes(4)==B({'P','I','X','1'}));auto count=r.word();std::string error;unsigned checks=0,guards=0;B nonempty;
 for(unsigned i=0;i<count;++i){auto blob=r.bytes(r.word());R expected{r.bytes(r.word())};PlayerProfileIndexV1 profile;check(profile.load({blob.data(),blob.size()},error));auto view=profile.borrow();check(view&&view.bytes()==blob);auto rows=expected.word();while(rows--){auto key=expected.bytes(expected.word());std::string name(key.begin(),key.end());auto offset=expected.word(),size=expected.word();auto section=view.section(name.c_str());check(section&&section->offset==offset&&section->size==size);auto payload=view.payload(name.c_str());check(payload.size==size&&payload.data==view.bytes().data()+offset);++checks;}
  check(expected.at==expected.b.size()&&!view.section("missing"));check(!profile.load({blob.data(),blob.size()},error));++guards;if(blob.size()>20&&nonempty.empty())nonempty=blob;
 }
 check(r.at==r.b.size());PlayerProfileIndexV1 profile;std::uint8_t good[4]{};check(profile.load({good,4},error));for(std::size_t i=0;i<nonempty.size();++i){check(!profile.load({nonempty.data(),i},error));check(profile.borrow().bytes()==B(4,0));++guards;}
 std::uint8_t corrupt[4]={255,255,255,255};check(!profile.load({corrupt,4},error));++guards;PlayerProfileIndexV1::Borrow retained;{PlayerProfileIndexV1 owned;check(owned.load({nonempty.data(),nonempty.size()},error));retained=owned.borrow();}check(retained.bytes()==nonempty);++guards;
 std::cout<<"{\"validation\":\"PASS\",\"original_cases\":"<<count<<",\"section_checks\":"<<checks<<",\"guards\":"<<guards<<",\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
