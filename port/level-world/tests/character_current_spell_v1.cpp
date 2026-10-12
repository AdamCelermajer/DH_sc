#include "../character_current_spell_v1.hpp"
#include "../../script-runtime/script_return_observer_v1.h"
#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
using namespace dh2::character::skills;
namespace {
unsigned checks{};
void check(bool v,const char* e){++checks;if(!v)throw std::runtime_error(e);}
std::uint32_t word(std::istream& f){std::uint32_t n{};check(bool(f.read(reinterpret_cast<char*>(&n),4)),"Gold read");return n;}
struct Provider {
 std::int32_t first{},second{},level{};unsigned count{},fail=99;
 std::vector<std::uint32_t> trace;
 static int invoke(void* p,const CurrentSpellRequest24V1* q,CurrentSpellResponse16V1* r){
  auto& s=*static_cast<Provider*>(p);check(q&&r&&!q->reserved&&q->character==0x100000001ull,"Actual current-spell owner/request");
  auto step=s.count++;s.trace.insert(s.trace.end(),{q->operation,q->id,static_cast<std::uint32_t>(q->difficulty),1});
  if(step==s.fail)return 1;
  r->value=q->operation==current_spell_selected_faery?(step==0?s.first:s.second):q->operation==current_spell_saved_level?s.level:12345678;
  return 0;
 }
};
struct First {unsigned calls{};float answer{};static int observe(void* p,const dh2_script_first_return_v1* v,char*,std::size_t){auto& s=*static_cast<First*>(p);check(v->count==1&&v->type==DH2_SCRIPT_NUMBER,"Actual Lua integer result projection");++s.calls;s.answer=v->number;return 0;}};
}
int main(int argc,char** argv){try{
 check(argc==2,"Usage: current_spell source-gold.bin");std::ifstream f(argv[1],std::ios::binary);check(word(f)==0x31505343,"Spell gold header");auto count=word(f);check(count==512,"Whole original gold count");unsigned requests=0;
 for(unsigned i=0;i<count;++i){Provider p;p.first=static_cast<std::int32_t>(word(f));p.second=static_cast<std::int32_t>(word(f));p.level=static_cast<std::int32_t>(word(f));auto expected=static_cast<std::int32_t>(word(f));std::vector<std::uint32_t> trace;for(unsigned j=0;j<16;++j)trace.push_back(word(f));
  CurrentSpellServices16V1 services{&p,Provider::invoke};std::int32_t answer=123;
  check(dh2_character_current_spell_level_v1(&answer,0x100000001ull,&services)==1&&answer==expected&&p.trace==trace,"Whole source result/service replay");requests+=p.count;
 }
 check(f.peek()==EOF,"Gold end");unsigned guards=0;
 for(unsigned stop=0;stop<4;++stop){Provider p;p.fail=stop;CurrentSpellServices16V1 s{&p,Provider::invoke};std::int32_t out=0x12345678;check(dh2_character_current_spell_level_v1(&out,0x100000001ull,&s)==-2&&out==0x12345678&&p.count==stop+1,"Required failure preserves source prefix");++guards;}
 Provider p;p.first=1;p.second=4;p.level=65535;CurrentSpellBindingsV1 bindings{0x100000001ull,{&p,Provider::invoke}};
 std::int32_t out=123;check(dh2_character_current_spell_level_v1(nullptr,bindings.character,&bindings.services)==-1&&p.count==0,"Null output rejects before services");++guards;
 check(dh2_character_current_spell_level_v1(&out,0,&bindings.services)==-1&&p.count==0,"Null Character rejects before services");++guards;
 check(dh2_character_current_spell_level_v1(&out,bindings.character,nullptr)==-1&&p.count==0,"Null services reject before effects");++guards;
 auto missing=bindings.services;missing.invoke=nullptr;check(dh2_character_current_spell_level_v1(&out,bindings.character,&missing)==-1&&p.count==0,"Missing invoke rejects before effects");++guards;
 std::uint32_t returned=12;char error[128]{};
 check(current_spell_info_v1(&bindings,nullptr,0,nullptr,0,&returned,error,sizeof(error))==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE&&returned==0&&p.count==4,"Return-storage failure retains all original query effects");++guards;
 p.count=0;p.trace.clear();
 auto* raw=dh2_script_vm_create(16u*1024u*1024u);check(raw,"Actual source VM");std::unique_ptr<dh2_script_vm,decltype(&dh2_script_vm_destroy)> vm(raw,dh2_script_vm_destroy);
 check(!dh2_script_vm_bind_source_values(raw,"GetCurrentSpellInfo",current_spell_info_v1,&bindings),"Bind actual recovered callback");
 const char source[]="function NoArgs()return GetCurrentSpellInfo()end function IgnoredArgs()return GetCurrentSpellInfo('ignored',{},42)end";
 check(!dh2_script_vm_load_source_file(raw,source,sizeof(source)-1),"Load actual source query callers");First first;
 check(!dh2_script_vm_call_first_source_v1(raw,"NoArgs",nullptr,0,First::observe,&first)&&first.answer==65535&&p.count==4,"Actual zero-argument Lua call");
 p.count=0;p.trace.clear();p.level=-1;
 check(!dh2_script_vm_call_first_source_v1(raw,"IgnoredArgs",nullptr,0,First::observe,&first)&&first.answer==-1&&p.count==4,"Source ignores provided Arguments");
 p.count=0;p.fail=1;auto prior=first.calls;
 check(dh2_script_vm_call_first_source_v1(raw,"NoArgs",nullptr,0,First::observe,&first)==DH2_SCRIPT_REQUIRED_FAILURE_STATUS&&p.count==2&&first.calls==prior,"Real VM required failure stops before result observer");++guards;
 std::cout<<"{\"validation\":\"PASS\",\"original_cases\":"<<count<<",\"ordered_services\":"<<requests<<",\"real_VM_queries\":2,\"guards\":"<<guards<<",\"checks\":"<<checks<<",\"full_player_initialization\":false}"<<std::endl;
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
