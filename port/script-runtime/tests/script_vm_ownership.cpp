#include "script_runtime.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
namespace {
unsigned checks=0;
void check(bool condition){++checks;if(!condition){std::cerr<<"failed check "<<checks<<'\n';std::abort();}}
dh2_script_value get(dh2_script_vm* vm,const char* name){
 dh2_script_value value{};check(dh2_script_vm_get_global(vm,name,&value)==0);return value;
}
void load(dh2_script_vm* vm,const char* code){check(dh2_script_vm_load(vm,code,std::strlen(code),"@ownership-audit")==0);}
int busy(void* opaque,const dh2_script_value*,uint32_t count,dh2_script_value*,uint32_t,uint32_t* results,char*,size_t){
 auto* vm=static_cast<dh2_script_vm*>(opaque);check(count==0);
 check(dh2_script_vm_open_source_library(vm,DH2_SCRIPT_LIBRARY_BASE)==-1);
 check(dh2_script_vm_open_source_libraries(vm)==-1);
 check(dh2_script_vm_stack_size(vm)==-1);*results=0;return 0;
}
}
int main(){
 check(!dh2_script_vm_create_empty(1));check(dh2_script_vm_stack_size(nullptr)==-1);
 check(dh2_script_vm_open_source_library(nullptr,0)==-1);
 check(dh2_script_vm_open_source_libraries(nullptr)==-1);
 auto* first=dh2_script_vm_create_empty(8*1024*1024);
 auto* second=dh2_script_vm_create_empty(8*1024*1024);
 auto* eager=dh2_script_vm_create(8*1024*1024);
 check(first&&second&&eager&&first!=second);check(dh2_script_vm_stack_size(first)==0);
 for(const char* name:{"_G","coroutine","math","table","string","Trace","StartTimer"})
  check(get(first,name).type==DH2_SCRIPT_NIL);
 check(dh2_script_vm_stack_size(first)==0);
 check(dh2_script_vm_open_source_library(first,4)==-1);
 check(dh2_script_vm_stack_size(first)==0);
 load(first,"before=7");check(get(first,"before").number==7);
 const char* names[]={"_G","math","table","string"};
 const int expected[]={2,3,4,5};
 for(uint32_t library=0;library<4;++library){
  check(dh2_script_vm_open_source_library(first,library)==0);
  check(dh2_script_vm_stack_size(first)==expected[library]);
  check(get(first,names[library]).type==DH2_SCRIPT_TABLE);
  check(dh2_script_vm_stack_size(first)==expected[library]);
  for(uint32_t pending=library+1;pending<4;++pending)check(get(first,names[pending]).type==DH2_SCRIPT_NIL);
 }
 check(get(first,"coroutine").type==DH2_SCRIPT_TABLE);
 check(dh2_script_vm_open_source_libraries(first)==0);check(dh2_script_vm_stack_size(first)==10);
 check(dh2_script_vm_open_source_libraries(second)==0);check(dh2_script_vm_stack_size(second)==5);
 check(dh2_script_vm_stack_size(eager)==0);
 for(const char* name:names)check(get(eager,name).type==DH2_SCRIPT_TABLE);
 load(first,"assert(math.abs(-3)==3); assert(string.upper('source')=='SOURCE'); local t={3,1}; table.sort(t); assert(t[1]==1); rounding=16777216+1; private=99; function CheckResult() return 3 end");
 check(get(first,"rounding").number==16777216.f);check(get(second,"private").type==DH2_SCRIPT_NIL);
 uint32_t returned=0;dh2_script_value result{};
 check(dh2_script_vm_call(first,"CheckResult",nullptr,0,&result,1,&returned)==0&&returned==1&&result.number==3);
 check(dh2_script_vm_stack_size(first)==10);
 check(dh2_script_vm_bind(first,"CheckBusy",busy,first)==0);
 load(first,"function Busy() CheckBusy() end");
 check(dh2_script_vm_call_discard_source(first,"Busy",nullptr,0)==0);
 check(dh2_script_vm_stack_size(first)==10);
 const char* failure="error('owned failure')";
 check(dh2_script_vm_load(first,failure,std::strlen(failure),"@failure")==-2);
 check(std::strstr(dh2_script_vm_error(first),"owned failure")!=nullptr);
 check(dh2_script_vm_stack_size(first)==10);
 dh2_script_vm_destroy(first);dh2_script_vm_destroy(second);dh2_script_vm_destroy(eager);
 auto* bounded=dh2_script_vm_create_empty(65536);check(bounded);
 bool protected_failure=false;unsigned opens=0;
 for(;opens<4096;++opens){
  const int before=dh2_script_vm_stack_size(bounded);
  const int status=dh2_script_vm_open_source_library(bounded,DH2_SCRIPT_LIBRARY_BASE);
  check(dh2_script_vm_memory(bounded)<=65536);
  if(status){check(status==-2);check(dh2_script_vm_stack_size(bounded)==before);check(*dh2_script_vm_error(bounded));protected_failure=true;break;}
  check(dh2_script_vm_stack_size(bounded)==before+2);
 }
 check(protected_failure&&opens>0);dh2_script_vm_destroy(bounded);
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks
          <<",\"source_library_stack_steps\":[2,3,4,5],\"repeated_stack_top\":10"
          <<",\"private_vms_verified\":true,\"eager_policy_preserved\":true"
          <<",\"allocation_failure_protected\":true,\"successful_bounded_opens\":"<<opens<<"}\n";
}
