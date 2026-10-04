#include "../player_savegame_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::data;using B=std::vector<std::uint8_t>;
void check(bool x){if(!x)throw std::runtime_error("original metadata mismatch");}
struct R{B b;std::size_t at{};std::uint32_t word(){check(at+4<=b.size());std::uint32_t n=0;for(int i=0;i<4;++i)n|=std::uint32_t(b[at++])<<(8*i);return n;}B bytes(std::size_t n){check(n<=b.size()-at);B v(b.begin()+at,b.begin()+at+n);at+=n;return v;}B block(){return bytes(word());}};
B file(const char* p){std::ifstream f(p,std::ios::binary);check(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char** argv){try{check(argc==3);R names{file(argv[2])};std::vector<std::string> keys;auto count=names.word();while(count--){auto b=names.block();keys.emplace_back(b.begin(),b.end());}check(names.at==names.b.size());R r{file(argv[1])};check(r.bytes(4)==B({'M','S','G','1'}));auto cases=r.word();std::string error;unsigned checks=0;
 for(unsigned i=0;i<cases;++i){PlayerSavegameV1 p;std::size_t used=0;auto name=r.block();auto wantname=r.block();check(p.load_name({name.data(),name.size()},used,error)&&used==name.size()&&p.name()==std::string(wantname.begin(),wantname.end()));++checks;
 auto level=r.bytes(4);auto wantlevel=r.word();check(p.load_level({level.data(),level.size()},used,error)&&used==4&&std::uint32_t(p.level())==wantlevel);++checks;auto cl=r.block();auto wantclass=r.word();check(p.load_class({cl.data(),cl.size()},keys,used,error)&&used==cl.size()&&std::uint32_t(p.class_id())==wantclass);++checks;
 }
 check(r.at==r.b.size());std::cout<<"{\"validation\":\"PASS\",\"original_cases\":"<<cases<<",\"reader_comparisons\":"<<checks<<",\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
