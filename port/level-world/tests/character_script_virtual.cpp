#include "character_script_virtual.hpp"
#include <fstream>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <dlfcn.h>
using namespace dh2::character;
namespace {
unsigned checks=0;
void check(bool value){++checks;if(!value)throw std::runtime_error("script virtual check "+std::to_string(checks));}
uint32_t word(std::istream& in){uint32_t v;in.read(reinterpret_cast<char*>(&v),4);check(bool(in));return v;}
dh2_script_value get(dh2_script_vm* vm,const char* name){dh2_script_value v{};check(!dh2_script_vm_get_global(vm,name,&v));return v;}
}
int main(int argc,char** argv){try{
 check(argc==2);std::ifstream gold(argv[1],std::ios::binary);char magic[4];gold.read(magic,4);check(!std::memcmp(magic,"VSC1",4));check(word(gold)==4100);
 const char* names[]={"OnTargetHit","OnTargetMissed","OnUpdate","OnFriendSpotted","OnTargetOutOfRange","OnTargetInRangedRange","OnTargetInCloseRange","OnTargetInMeleeRange","OnMasterOutOfRange","OnMasterInRangedRange","OnMasterInCloseRange","OnMasterInMeleeRange"};
 auto* aliases=dh2_script_alias_create();check(aliases);auto* vm=dh2_script_vm_create_empty(4*1024*1024);check(vm);check(!dh2_script_vm_open_source_libraries(vm));
 for(unsigned n=0;n<4100;++n){auto external=word(gold),mask=word(gold),expected=word(gold);check(!dh2_script_alias_clear_contents(aliases));for(unsigned i=0;i<(external?12u:2u);++i)if(mask&(1u<<i))check(!dh2_script_alias_add(aliases,names[i],"MissingTarget"));
  uint32_t flags=0xf0f0f0f0;check(!dh2_character_script_init_vcb(&flags,aliases,external));check(flags==expected);
 }
 check(get(vm,"MissingTarget").type==DH2_SCRIPT_NIL);check(!dh2_script_alias_clear_contents(aliases));
 const char* initial[]={"OnInit","OnInitPost","OnInitFinal","OnTerminate"};const char* replacements[]={"InitAlias","PostAlias","FinalAlias","TerminateAlias"};
 for(unsigned i=0;i<4;++i)check(!dh2_script_alias_add(aliases,initial[i],replacements[i]));
 const char* lua="calls=0;projections=0;local object=setmetatable({}, {__index=function(t,k) assert(k=='_this');projections=projections+1;return nil end}); function InitAlias()calls=calls+1;return object end;function PostAlias()calls=calls+2;return object end;function FinalAlias()calls=calls+4;return object end;function TerminateAlias()calls=calls+8;return object end";
 check(!dh2_script_vm_load(vm,lua,std::strlen(lua),"@genuine-external-initial"));const ScriptTimerCall16 session{vm,aliases};
 for(unsigned kind=0;kind<6;++kind)for(unsigned slot=8;slot<=20;slot+=4)check(!dh2_character_script_initial_virtual(&session,kind,slot));
 check(get(vm,"calls").number==15&&get(vm,"projections").number==4&&dh2_script_vm_stack_size(vm)==5);
 const char* replace="function InitAlias()calls=calls+100 end";check(!dh2_script_vm_load(vm,replace,std::strlen(replace),"@fresh-global"));check(!dh2_character_script_initial_virtual(&session,script_external,8));check(get(vm,"calls").number==115);
 check(!dh2_script_alias_add(aliases,"OnInit","Absent"));check(dh2_character_script_initial_virtual(&session,script_external,8)==-2);check(dh2_script_vm_stack_size(vm)==5);
 const ScriptTimerCall16 empty{};for(unsigned kind:{0u,1u,2u,3u,5u})check(!dh2_character_script_initial_virtual(&empty,kind,8));
 uint32_t guard=0xa5a5a5a5;check(dh2_character_script_init_vcb(&guard,nullptr,1)==-1&&guard==0xa5a5a5a5);check(dh2_character_script_init_vcb(&guard,aliases,2)==-1&&guard==0xa5a5a5a5);check(dh2_character_script_init_vcb(nullptr,aliases,1)==-1);check(dh2_character_script_initial_virtual(nullptr,4,8)==-1);check(dh2_character_script_initial_virtual(&session,6,8)==-1);check(dh2_character_script_initial_virtual(&session,4,9)==-1);check(dh2_character_script_initial_virtual(&empty,4,8)==-1);
 Dl_info module{},runtime{},alias{};check(dladdr(reinterpret_cast<void*>(&dh2_character_script_init_vcb),&module));check(dladdr(reinterpret_cast<void*>(&dh2_script_vm_create_empty),&runtime));check(dladdr(reinterpret_cast<void*>(&dh2_script_alias_contains),&alias));
 dh2_script_vm_destroy(vm);dh2_script_alias_destroy(aliases);
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"original_vcb_cases\":4100,\"initial_virtual_cases\":24,\"actual_lua_calls\":5,\"actual_return_table_projections\":4,\"source_empty_initial_methods\":20,\"missing_aliased_global_protected_error\":true,\"flags_from_alias_membership_without_global\":true,\"library\":\""<<module.dli_fname<<"\",\"runtime_library\":\""<<runtime.dli_fname<<"\",\"alias_library\":\""<<alias.dli_fname<<"\"}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
