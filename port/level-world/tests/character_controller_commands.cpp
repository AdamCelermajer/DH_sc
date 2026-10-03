#include "character_controller_commands.hpp"
#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
namespace {
constexpr std::uintptr_t owner=0x100000011ull,target=0x200000033ull;
struct Input60 {std::uint32_t command,present,blocked,locked,forced,network,remote,cache,cached;float actual[3],cached_position[3];};
static_assert(sizeof(Input60)==60);
void require(bool v){if(!v)throw std::runtime_error("controller command audit mismatch");}
template<class T>T read(std::ifstream& f){T v{};require(bool(f.read(reinterpret_cast<char*>(&v),sizeof(v))));return v;}
struct Context {
 Input60 input{};std::vector<CharacterControlRequest32> calls;int failure=-1;
 static int invoke(void* p,const CharacterControlRequest32* req,CharacterControlResponse16* response){
  auto& c=*static_cast<Context*>(p);require(req&&response);auto copy=*req;
  require(copy.subject==owner||copy.subject==target);copy.subject=copy.subject==owner?1:2;c.calls.push_back(copy);
  if(req->service==control_is_remotely_updated)response->word=c.input.network!=0xffffffff?1:c.input.remote;
  else if(req->service==control_target_position)std::memcpy(response->position,c.input.cache&&c.input.cached?c.input.cached_position:c.input.actual,12);
  return req->service==static_cast<std::uint32_t>(c.failure)?-1:1;
 }
};
ControllerCommandState32 state(const Input60& i){return {0x300000055ull,owner,i.blocked,i.locked,i.forced,0};}
}
int main(int argc,char** argv){try{
 require(argc==2);std::ifstream file(argv[1],std::ios::binary);require(bool(file));require(read<std::array<char,4>>(file)==std::array<char,4>{'C','C','D','1'});
 const auto cases=read<std::uint32_t>(file);std::uint64_t requests=0;
 for(std::uint32_t i=0;i<cases;++i){
  Context c;c.input=read<Input60>(file);const auto count=read<std::uint32_t>(file);require(count<=4);
  std::vector<CharacterControlRequest32> expected(count);for(auto& r:expected)r=read<CharacterControlRequest32>(file);
  auto s=state(c.input);CharacterControlServices16 services{&c,Context::invoke};
  require(dh2_character_controller_character(&s,c.input.command,c.input.present?target:0,&services)==1);
  require(c.calls.size()==expected.size());for(std::size_t j=0;j<expected.size();++j)require(std::memcmp(&c.calls[j],&expected[j],32)==0);requests+=count;
 }
 require(file.peek()==std::char_traits<char>::eof());
 Context c;c.input={1,1,0,0,0,0xffffffff,0,1,1,{7,-11,13},{-17,19,-23}};
 CharacterControlServices16 services{&c,Context::invoke};auto s=state(c.input);std::uint32_t guards=0;
 auto reject=[&](const ControllerCommandState32* q,std::uint32_t command,const CharacterControlServices16* svc){c.calls.clear();require(dh2_character_controller_character(q,command,target,svc)==-1&&c.calls.empty());++guards;};
 reject(nullptr,1,&services);reject(&s,1,nullptr);reject(&s,3,&services);
 auto malformed=s;malformed.reserved=1;reject(&malformed,1,&services);malformed=s;malformed.controller=0;reject(&malformed,1,&services);malformed=s;malformed.owner=0;reject(&malformed,1,&services);
 for(unsigned i=0;i<3;++i){malformed=s;if(i==0)malformed.global_blocked=256;else if(i==1)malformed.locked=256;else malformed.forced=256;reject(&malformed,1,&services);}
 CharacterControlServices16 missing{&c,nullptr};reject(&s,1,&missing);
 // Source gate exits before owner/service use, including a missing callback.
 malformed=s;malformed.global_blocked=255;malformed.owner=0;c.calls.clear();require(dh2_character_controller_character(&malformed,1,target,&missing)==1&&c.calls.empty());
 std::uint32_t failure_checks=0;
 for(std::uint32_t command=0;command<3;++command){c.failure=-1;c.calls.clear();require(dh2_character_controller_character(&s,command,target,&services)==1);const auto expected=c.calls;
  for(std::size_t j=0;j<expected.size();++j){c.failure=expected[j].service;c.calls.clear();require(dh2_character_controller_character(&s,command,target,&services)==-1);require(c.calls.size()==j+1);for(std::size_t k=0;k<=j;++k)require(std::memcmp(&c.calls[k],&expected[k],32)==0);++failure_checks;}
 }
 std::cout<<"{\"validation\":\"PASS\",\"original_corpus_cases\":"<<cases<<",\"ordered_service_requests\":"<<requests<<",\"native_guards\":"<<guards<<",\"service_failure_prefix_checks\":"<<failure_checks<<",\"identities_above_4gib\":true,\"mismatches\":0}\n";
 return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
