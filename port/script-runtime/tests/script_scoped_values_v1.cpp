#include "../script_runtime.h"
#include <cassert>
#include <cstring>
#include <iostream>
struct Fixture{dh2_script_vm* vm;dh2_script_callback_scope saved{};unsigned calls{};};
static int scoped(void* p,const dh2_script_callback_scope* s,const dh2_script_value* args,uint32_t count,
 dh2_script_value* out,uint32_t capacity,uint32_t* returned,char*,size_t){
 auto& f=*static_cast<Fixture*>(p);assert(dh2_script_callback_scope_valid(s)&&count==2&&capacity==16);
 assert(args[0].type==DH2_SCRIPT_NUMBER&&args[1].type==DH2_SCRIPT_BOOLEAN);f.saved=*s;++f.calls;
 assert(dh2_script_vm_call_discard_source(f.vm,"Leaf",nullptr,0)==-1);
 assert(!dh2_script_callback_call_discard_source(s,"Leaf",nullptr,0));assert(dh2_script_callback_scope_valid(s));
 out[0]={};out[0].type=DH2_SCRIPT_NUMBER;out[0].number=args[0].number+7;
 out[1]={};out[1].type=DH2_SCRIPT_BOOLEAN;out[1].boolean=args[1].boolean;*returned=2;return 0;
}
static int required(void*,const dh2_script_callback_scope*,const dh2_script_value*,uint32_t,
 dh2_script_value*,uint32_t,uint32_t*,char*,size_t){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
int main(){Fixture f{dh2_script_vm_create(1024*1024)};assert(f.vm);
 assert(!dh2_script_vm_bind_source_scoped_values(f.vm,"Attack",scoped,&f));
 assert(!dh2_script_vm_bind_source_scoped_values(f.vm,"Missing",required,&f));
 const char* code="calls=0;function Leaf() calls=calls+1 end;function Run() amount,flag=Attack(10,true);assert(amount==17 and flag==true);pcall(Missing);after=1 end";
 assert(!dh2_script_vm_load(f.vm,code,std::strlen(code),"scoped-values"));
 const auto top=dh2_script_vm_stack_size(f.vm);assert(dh2_script_vm_call_source_status_objects(f.vm,"Run",nullptr,0)==DH2_SCRIPT_REQUIRED_FAILURE_STATUS);
 assert(f.calls==1&&!dh2_script_callback_scope_valid(&f.saved)&&dh2_script_vm_stack_size(f.vm)==top);
 dh2_script_value value{};assert(!dh2_script_vm_get_global(f.vm,"amount",&value)&&value.number==17);
 assert(!dh2_script_vm_get_global(f.vm,"after",&value)&&value.number==1);assert(dh2_script_vm_required_failure_epoch(f.vm)==1);
 dh2_script_vm_destroy(f.vm);std::cout<<"PASS scoped F_Attack-like returns, nested callback, busy guard, expired capability, caught required failure\n";
}
