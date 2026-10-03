#include "combat_result.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
using namespace dh2::data;
void check(bool b,const char* e){if(!b)throw std::runtime_error(e);}
std::uint32_t word(const std::vector<std::uint8_t>& data,std::size_t p){check(p<=data.size()&&data.size()-p>=4,"Reference truncated");return std::uint32_t(data[p])|(std::uint32_t(data[p+1])<<8)|(std::uint32_t(data[p+2])<<16)|(std::uint32_t(data[p+3])<<24);}
std::int32_t integer(const std::vector<std::uint8_t>& data,std::size_t p){auto bits=word(data,p);std::int32_t value;std::memcpy(&value,&bits,4);return value;}
int main(int argc,char** argv){try{
 if(argc!=2)return 2;
 std::ifstream file(argv[1],std::ios::binary);std::vector<std::uint8_t> data{std::istreambuf_iterator<char>(file),{}};auto count=word(data,0);check(count==9792&&data.size()==4+count*1920,"Reference dimensions differ");std::array<unsigned,9> outcomes{};
 for(unsigned i=0;i<count;++i){const auto p=4+i*1920;std::array<std::int32_t,224> ap,dp;for(unsigned j=0;j<224;++j){ap[j]=integer(data,p+j*4);dp[j]=integer(data,p+896+j*4);}auto view=[&](unsigned o,const std::int32_t* props){return CombatantView{props,integer(data,p+o),integer(data,p+o+4),word(data,p+o+8),word(data,p+o+12),word(data,p+o+16),integer(data,p+o+20),word(data,p+o+24)};};auto attacker=view(1792,ap.data()),defender=view(1820,dp.data());CombatRandom random{word(data,p+1864),word(data,p+1868)};CombatResultRequest request{&attacker,&defender,&random,word(data,p+1848),integer(data,p+1852),integer(data,p+1856),integer(data,p+1860)};CombatResult result;check(dh2_combat_result(&result,&request)==0,"Result calculation rejected");std::array<std::uint32_t,10> actual;static_assert(sizeof(result)==40);std::memcpy(actual.data(),&result,40);for(unsigned j=0;j<10;++j)check(actual[j]==word(data,p+1872+j*4),"Result differs from original instructions");check(random.seed==word(data,p+1912)&&random.calls==word(data,p+1916),"Result random state differs");for(unsigned bit=0;bit<9;++bit)outcomes[bit]+=bool(result.outcomes&(1u<<bit));}
 std::array<std::int32_t,224> props{};CombatantView actor{props.data()};CombatRandom random{123,456};CombatResult result;result.amount=1234;CombatResultRequest invalid{nullptr,&actor,&random,0xffffffff,-1,-1,1};check(dh2_combat_result(&result,&invalid)==1&&result.amount==1234&&random.seed==123&&random.calls==456,"Invalid actor changed output/RNG");invalid.attacker=&actor;invalid.random=nullptr;check(dh2_combat_result(&result,&invalid)==1&&result.amount==1234,"Invalid RNG changed output");
 check(dh2_combat_melee(&result,&actor,&actor,&random,2,0)==1&&result.amount==1234&&random.seed==123&&random.calls==456,"Invalid melee hand changed output/RNG");
 std::cout<<"{\"original_result_cases\":"<<count<<",\"result_words_per_case\":10,\"random_state_verified\":true,\"invalid_input_atomic\":true,\"outcomes_observed\":[";for(unsigned i=0;i<9;++i){if(i)std::cout<<',';std::cout<<outcomes[i];}std::cout<<"]}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
