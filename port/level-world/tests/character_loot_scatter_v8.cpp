#include "../character_loot_scatter_v8.hpp"
#include <fstream>
#include <vector>
#include <cstring>
#include <iostream>
#include <stdexcept>
using namespace dh2::character;
bool normalize(void*,float[3],std::string&){return true;}
int main(int argc,char** argv){try{
 if(argc!=2)throw std::runtime_error("fixture path");std::ifstream f(argv[1],std::ios::binary);std::vector<char> bytes((std::istreambuf_iterator<char>(f)),{});std::size_t at{};
 auto read=[&](void* p,std::size_t n){if(n>bytes.size()-at)throw std::runtime_error("fixture span");std::memcpy(p,bytes.data()+at,n);at+=n;};
 std::uint32_t magic,count;read(&magic,4);read(&count,4);if(magic!=0x3843534c)throw std::runtime_error("magic");unsigned checks{};
 for(unsigned i=0;i<count;++i){dh2::data::LootRandom8V2 random,expected;std::uint32_t has;float source[3],killer[3],basis[3],output[3],gold[3];read(&random,8);read(&has,4);read(source,12);read(killer,12);read(basis,12);read(gold,12);read(&expected,8);std::string e;
  if(!character_loot_scatter_v8(random,source,has?killer:nullptr,output,{nullptr,normalize,basis},e)||std::memcmp(output,gold,12)||std::memcmp(&random,&expected,8))throw std::runtime_error("original scatter comparison "+std::to_string(i)+" "+e);++checks;
 }
 dh2::data::LootRandom8V2 random{19,7},before=random;float p[3]{},q[3]{1,0,0},out[3]{};std::string e;
 if(character_loot_scatter_v8(random,p,q,out,{},e)||std::memcmp(&before,&random,8))throw std::runtime_error("required normalization prefix");++checks;
 if(at!=bytes.size())throw std::runtime_error("fixture tail");std::cout<<"{\"validation\":\"PASS\",\"cases\":"<<count<<",\"checks\":"<<checks<<",\"normalization_and_basis_fixtures\":true}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
