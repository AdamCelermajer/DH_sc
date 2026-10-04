#include "../script_design_bindings.h"
#include "../../game-data/data.hpp"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

static std::vector<unsigned char> read_file(const std::string& path) {
  std::ifstream input(path,std::ios::binary);assert(input);
  return {std::istreambuf_iterator<char>(input),std::istreambuf_iterator<char>()};
}
static unsigned word(const std::vector<unsigned char>& bytes,size_t& cursor) {
  assert(cursor+4<=bytes.size());unsigned value;std::memcpy(&value,bytes.data()+cursor,4);
  cursor+=4;return value;
}
static unsigned bits(float value) {unsigned result;std::memcpy(&result,&value,4);return result;}
static int signed_word(unsigned value) {int result;std::memcpy(&result,&value,4);return result;}
struct Lookup {
  int value=0;unsigned calls=0;bool corpus=true,fail=false;
  const dh2::data::CharacterTable* characters=nullptr;
};
static int lookup(void* context,unsigned kind,const char* group,const char* name,int* out) {
  auto& source=*static_cast<Lookup*>(context);++source.calls;
  if(source.fail)return 1;
  if(source.corpus){assert(std::strcmp(group,"CharacterProperties")==0&&std::strcmp(name,"SkillTree")==0);*out=source.value;return 0;}
  // Actual decoded Character field names are borrowed here. Registration and
  // other Application design tables remain explicit service responsibilities.
  if(kind==1&&std::strcmp(group,"CharacterProperties")==0) {
    const auto& fields=source.characters->fields;
    auto found=std::find(fields.begin(),fields.end(),name);
    *out=found==fields.end()?-1:static_cast<int>(found-fields.begin());
  } else *out=kind==1?-1:0;
  return 0;
}
static dh2_script_value global(dh2_script_vm* vm,const char* name) {
  dh2_script_value value{};assert(dh2_script_vm_get_global(vm,name,&value)==0);return value;
}
static void load(dh2_script_vm* vm,const std::string& text) {
  assert(dh2_script_vm_load(vm,text.data(),text.size(),"design-audit")==0);
}
int main(int argc,char** argv) {
  assert(argc==3);auto gold=read_file(argv[1]);size_t cursor=0;
  assert(word(gold,cursor)==0x314e4744);const unsigned count=word(gold,cursor);
  Lookup state;dh2_script_design_bindings source{&state,lookup,0};
  dh2_script_function callbacks[]={dh2_script_design_get_constant,dh2_script_design_get_struct,dh2_script_design_get_oid};
  unsigned lookup_calls=0,guards=0;
  for(unsigned i=0;i<count;++i) {
    const auto op=word(gold,cursor),arity=word(gold,cursor),first=word(gold,cursor),second=word(gold,cursor);
    state.value=signed_word(word(gold,cursor));const auto expected_count=word(gold,cursor),expected_word=word(gold,cursor);
    assert(op<3&&arity<=33);std::vector<dh2_script_value> values(arity);
    for(unsigned j=0;j<arity;++j){values[j].type=j==0?first:j==1?second:5;values[j].text=j==0?"CharacterProperties":"SkillTree";}
    state.calls=0;dh2_script_value result{};unsigned returned=0xdeadbeef;char error[128]{};
    assert(callbacks[op](&source,values.data(),arity,&result,1,&returned,error,sizeof error)==0);
    assert(returned==expected_count&&state.calls==expected_count);
    if(returned)assert(result.type==3&&bits(result.number)==expected_word);
    lookup_calls+=state.calls;
  }
  assert(cursor==gold.size()&&count==1200&&lookup_calls==60);
  dh2_script_value names[2]{};names[0].type=names[1].type=4;names[0].text="CharacterProperties";names[1].text="SkillTree";
  for(auto callback:callbacks) {
    unsigned returned=0xdeadbeef;char error[128]{};dh2_script_value result{};state.calls=0;
    assert(callback(nullptr,names,2,&result,1,&returned,error,sizeof error)==1&&returned==0&&state.calls==0);++guards;
    assert(callback(&source,names,2,&result,0,&returned,error,sizeof error)==1&&returned==0&&state.calls==0);++guards;
    state.fail=true;assert(callback(&source,names,2,&result,1,&returned,error,sizeof error)==1&&returned==0&&state.calls==1);++guards;state.fail=false;
  }
  const std::string assets=argv[2];auto records=read_file(assets+"/character_properties_pyarray.bin"),
    rows=read_file(assets+"/character_properties_pyarraynames.bin"),fields=read_file(assets+"/character_properties_pystructnames.bin");
  dh2::data::CharacterTable table;std::string error;
  auto view=[](const auto& b){return dh2::data::Bytes{b.data(),b.size()};};
  assert(dh2::data::load_characters(view(records),view(rows),view(fields),table,error));
  assert(table.fields.size()==224&&table.rows.size()==448);
  state.corpus=false;state.characters=&table;
  auto* vm=dh2_script_vm_create(8*1024*1024);assert(vm);
  assert(dh2_script_design_bind(vm,&source)==0);
  unsigned vm_checks=0;
  for(unsigned i=0;i<table.fields.size();++i) {
    load(vm,"a=GetPyStruct('CharacterProperties','"+table.fields[i]+"'); b=GetPyOID('CharacterProperties','"+table.fields[i]+"')");
    assert(global(vm,"a").number==static_cast<float>(i)&&global(vm,"b").number==static_cast<float>(i));vm_checks+=2;
  }
  load(vm,"missing=GetPyStruct('CharacterProperties','absent'); wrongcase=GetPyOID('characterproperties','SkillTree'); cst=GetPyCst('missing','missing'); guard0=select('#',GetPyStruct()); guard1=select('#',GetPyOID('CharacterProperties')); guard2=select('#',GetPyCst(4,'a')); side=0; value=GetPyStruct('CharacterProperties','SkillTree',setmetatable({}, {__index=function(t,k) side=side+1 return nil end})); count=select('#',GetPyStruct('CharacterProperties','SkillTree'))");
  assert(global(vm,"missing").number==-1&&global(vm,"wrongcase").number==-1&&global(vm,"cst").number==0);
  assert(global(vm,"guard0").number==0&&global(vm,"guard1").number==0&&global(vm,"guard2").number==0);
  assert(global(vm,"side").number==1&&global(vm,"count").number==1);vm_checks+=8;
  state.fail=true;load(vm,"ok,message=pcall(GetPyStruct,'CharacterProperties','SkillTree')");
  assert(global(vm,"ok").boolean==0&&global(vm,"message").type==4);++vm_checks;state.fail=false;
  dh2_script_vm_destroy(vm);
  std::cout<<"{\"validation\":\"PASS\",\"original_gold_cases\":"<<count<<",\"ordered_lookup_calls\":"<<lookup_calls
    <<",\"actual_VM_checks\":"<<vm_checks<<",\"native_boundary_checks\":"<<guards
    <<",\"actual_character_fields\":224,\"full_manager_lookup_proved\":false,\"mismatches\":0}\n";
}
