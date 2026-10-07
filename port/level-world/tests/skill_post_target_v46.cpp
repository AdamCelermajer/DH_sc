#include "../character_target_bindings.hpp"
#include <cassert>
#include <fstream>
#include <iterator>
#include <string>
using namespace dh2::character;
static void load(dh2_script_vm* vm,const std::string& s){assert(!dh2_script_vm_load_source_file(vm,s.data(),s.size()));}
static int debug(void*,TargetState48*,const TargetRequest24* q,std::uint32_t* out){
 if(q->service!=target_debug_load&&q->service!=target_debug_query)return -1;
 *out=0;return 0;
}
int main(int argc,char** argv){assert(argc==2);unsigned checks=0;
 for(const char* name:{"bashdown","charge","ground_slam"})for(bool in_range:{false,true}){
  TargetOwner16 owner{100,9,0,0};TargetState48 target{200,&owner,22,22,22,1,1,0,0,0};
  TargetBindings48 binding{&target,{nullptr,debug},nullptr,{0,0}};
  auto* vm=dh2_script_vm_create_empty(8*1024*1024);assert(vm&&!dh2_script_vm_open_source_libraries(vm));
  assert(!dh2_character_target_bind(vm,&binding));
  // Fixtures only expose registration and a range result. The actual authored
  // Post function and native scoped ClearTarget execute unmodified.
  load(vm,std::string("function GetPyOID() return 0 end; function RegisterSkill(a,b,c,d,e) actual_post=e end; function TargetInMeleeRange() return ")+(in_range?"true":"false")+" end");
  const std::string path=std::string(argv[1])+"/prince_warrior_"+name+".luac";
  std::ifstream input(path,std::ios::binary);assert(input);const std::string source{std::istreambuf_iterator<char>(input),{}};load(vm,source);
  std::uint32_t returned=99;assert(!dh2_script_vm_call(vm,"actual_post",nullptr,0,nullptr,0,&returned)&&!returned);
  const bool retain=std::string(name)=="charge"&&in_range;
  assert(target.target==(retain?22u:0u)&&target.last_target==(retain?22u:0u)&&target.candidate==(retain?22u:0u));
  assert(owner.word14d0==(retain?9u:0u)&&!binding.scope);
  dh2_script_vm_destroy(vm);++checks;
 }
 assert(checks==6);
}
