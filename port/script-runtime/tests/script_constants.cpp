#include "../script_constants.hpp"
#include "../script_design_bindings.h"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
static std::vector<uint8_t> read(const char* path) {
  std::ifstream file(path,std::ios::binary);assert(file);
  return {std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>()};
}
static uint32_t word(const std::vector<uint8_t>& b,size_t& p) {
  assert(p+4<=b.size());uint32_t v=uint32_t(b[p])|(uint32_t(b[p+1])<<8)|(uint32_t(b[p+2])<<16)|(uint32_t(b[p+3])<<24);p+=4;return v;
}
static std::vector<uint8_t> span(const std::vector<uint8_t>& b,size_t& p) {
  uint32_t n=word(b,p);assert(n<=b.size()-p);std::vector<uint8_t> out(b.begin()+p,b.begin()+p+n);p+=n;return out;
}
static std::string text(const std::vector<uint8_t>& b,size_t& p){auto s=span(b,p);return {s.begin(),s.end()};}
static dh2_script_value global(dh2_script_vm* vm,const char* name){dh2_script_value v{};assert(!dh2_script_vm_get_global(vm,name,&v));return v;}
int main(int argc,char** argv) {
  assert(argc==2);auto corpus=read(argv[1]);size_t p=0;assert(word(corpus,p)==0x31545343);
  uint32_t loads=word(corpus,p),queries=0,assignments=0,stops=0;
  auto* table=dh2_script_constants_create();assert(table);
  for(uint32_t i=0;i<loads;++i) {
    auto data=span(corpus,p);int status=int(word(corpus,p));dh2_script_constants_reload expected{};
    expected.consumed=word(corpus,p);expected.assignments=word(corpus,p);expected.groups_complete=word(corpus,p);expected.source_name_stop=word(corpus,p);uint32_t count=word(corpus,p);
    dh2_script_constants_reload got{};assert(dh2_script_constants_load(table,data.data(),uint32_t(data.size()),&got)==status);assert(!std::memcmp(&expected,&got,sizeof(got)));
    assignments+=got.assignments;stops+=got.source_name_stop;
    for(uint32_t j=0;j<count;++j) {
      auto group=text(corpus,p),name=text(corpus,p);uint32_t expected_value=word(corpus,p);int32_t value=123;assert(!dh2_script_constants_get(table,group.c_str(),name.c_str(),&value));uint32_t bits;std::memcpy(&bits,&value,4);assert(bits==expected_value);++queries;
    }
  }
  assert(p==corpus.size());
  unsigned guards=0;dh2_script_constants_reload result{};uint8_t bad[]={1,0,0};
  assert(dh2_script_constants_load(table,bad,3,&result)==-1&&result.consumed==0);++guards;
  const uint32_t before=dh2_script_constants_size(table);
  assert(dh2_script_constants_load(table,nullptr,0,&result)==-1);++guards;
  int32_t value=0;assert(dh2_script_constants_get(table,nullptr,"x",&value)==-1);++guards;
  assert(dh2_script_constants_lookup(table,1,"AIStates","Attack",&value)==-1);++guards;
  assert(dh2_script_constants_size(table)==before);++guards;
  auto* vm=dh2_script_vm_create(8*1024*1024);assert(vm);dh2_script_design_bindings services{table,dh2_script_constants_lookup,0};assert(!dh2_script_design_bind(vm,&services));
  const char script[]="levelcap=GetPyCst('CharacterDesign','MaxLevelDVeryHard'); attack=GetPyCst('AIStates','Attack'); missing=GetPyCst('missing','missing'); wrongcase=GetPyCst('aistates','Attack'); old=GetPyCst('Proof','old'); new=GetPyCst('Proof','new'); duplicate=GetPyCst('Proof','duplicate'); ok,err=pcall(GetPyOID,'AIStates','Attack')";
  assert(!dh2_script_vm_load(vm,script,sizeof(script)-1,"actual-native-constants"));
  assert(global(vm,"levelcap").number==100&&global(vm,"attack").number==5&&global(vm,"missing").number==0&&global(vm,"wrongcase").number==0&&global(vm,"old").number==7&&global(vm,"new").number==-2147483648.0f&&global(vm,"duplicate").number==23&&global(vm,"ok").boolean==0&&global(vm,"err").type==4);
  dh2_script_vm_destroy(vm);dh2_script_constants_clear(table);assert(!dh2_script_constants_size(table));assert(!dh2_script_constants_get(table,"AIStates","Attack",&value)&&value==0);guards+=2;dh2_script_constants_destroy(table);
  std::cout<<"{\"validation\":\"PASS\",\"original_loads\":"<<loads<<",\"original_queries\":"<<queries<<",\"original_assignments\":"<<assignments<<",\"source_name_stops\":"<<stops<<",\"genuine_VM_checks\":9,\"native_boundary_checks\":"<<guards<<",\"mismatches\":0}\n";
}
