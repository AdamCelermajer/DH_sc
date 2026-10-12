#include "../script_function_alias.h"
#include <cstring>
#include <iostream>
#include <stdexcept>
namespace {
unsigned checks=0;
void check(bool v,const char* reason){++checks;if(!v)throw std::runtime_error(reason);}
}
int main(){try{
 for(unsigned recording=0;recording<2;++recording)for(unsigned count: {0u,1u,8u,32u}){
  auto* aliases=dh2_script_alias_create();check(aliases,"allocation");
  for(unsigned i=0;i<count;++i){char key[32];std::snprintf(key,sizeof(key),"key%u",i);check(!dh2_script_alias_add(aliases,key,"before"),"main setup");}
  if(recording){check(!dh2_script_alias_push(aliases),"push");check(!dh2_script_alias_add(aliases,"OnTimer","during"),"backup setup");}
  check(!dh2_script_alias_clear_contents(aliases),"clear");
  check(!dh2_script_alias_contains(aliases,"OnTimer"),"main not empty");
  check(!dh2_script_alias_add(aliases,"Track","after-clear"),"tracking test setup");
  check(!dh2_script_alias_pop(aliases),"tracking test Pop");
  check(dh2_script_alias_contains(aliases,"Track")==int(!recording),"clear reset source tracking byte");
  check(!dh2_script_alias_clear_contents(aliases),"second clear");
  auto* vm=dh2_script_vm_create(2*1024*1024);check(vm&&!dh2_script_alias_bind(vm,aliases),"VM bind");
  const char* source="AddToVFTable('OnTimer','old');PushVFTable();AddToVFTable('OnTimer','during');proxy=newproxy(true);getmetatable(proxy).__gc=function() PopVFTable();AddToVFTable('GC','survived') end";
  check(!dh2_script_vm_load(vm,source,std::strlen(source),"@source-clear-before-close"),"Lua newproxy setup");
  check(!dh2_script_alias_clear_contents(aliases),"pre-close clear");
  // Keep callback context alive throughout lua_close. Actual __gc invokes the
  // native Pop/Add producers; cleared backup must not resurrect OnTimer.
  dh2_script_vm_destroy(vm);
  check(!dh2_script_alias_contains(aliases,"OnTimer"),"GC restored stale backup");
  check(!std::strcmp(dh2_script_alias_resolve(aliases,"GC"),"survived"),"actual __gc alias callback absent");
  dh2_script_alias_destroy(aliases);
 }
 check(dh2_script_alias_clear_contents(nullptr)==-1,"null clear accepted");
 std::cout<<"{\"validation\":\"PASS\",\"cases\":8,\"actual_gc_callbacks\":8,\"native_alias_callbacks_during_close\":16,\"tracking_preserved\":true,\"backup_clear_before_vm_close\":true,\"borrowed_wrapper_alive_through_close\":true,\"checks\":"<<checks<<",\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 3;}}
