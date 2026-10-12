#include "../script_return_observer_v3.h"
#include <cstdio>
#include <cstring>
#include <stdexcept>
namespace {
unsigned checks=0;
void check(bool b,const char* m){++checks;if(!b)throw std::runtime_error(m);}
struct Context {dh2_script_vm* vm;dh2_script_first_return_v1 result{};unsigned observations{},busy{};int reject{};char text[32]{};
 static int observe(void* p,const dh2_script_first_return_v1* v,char*,std::size_t){auto& t=*static_cast<Context*>(p);t.result=*v;++t.observations;check(dh2_script_vm_stack_size(t.vm)==-1,"Busy stack");check(dh2_script_vm_call_indexed_source_v3(t.vm,"Returns",nullptr,0,0,observe,p)==-1,"Indexed reentry rejected");++t.busy;if(v->text){check(v->text_bytes<sizeof(t.text),"Bounded text");std::memcpy(t.text,v->text,v->text_bytes);t.text[v->text_bytes]=0;}return t.reject;}
};
int identity(void*,const dh2_script_value*,unsigned,dh2_script_value* o,unsigned,unsigned* n,char*,std::size_t){*o={};o->type=DH2_SCRIPT_IDENTITY;o->identity=0xabcde00111223344ull;*n=1;return 0;}
int required(void*,const dh2_script_value*,unsigned,dh2_script_value*,unsigned,unsigned*,char*,std::size_t){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
}
int main(){try{
 auto* vm=dh2_script_vm_create(8u*1024u*1024u);check(vm,"VM");Context c{vm};check(!dh2_script_vm_bind_source_values(vm,"Identity",identity,nullptr)&&!dh2_script_vm_bind_source_values(vm,"Required",required,nullptr),"Bindings");const char* code=R"(
calls=0;projections=0
function Returns()calls=calls+1;return false,true,17.75,"a\000b",Identity(),setmetatable({}, {__index=function(t,k)projections=projections+1;return Identity()end}),function()end,nil end
function NoReturns()calls=calls+1 end
function Many(n)calls=calls+1;local t={};for i=1,n do t[i]=setmetatable({}, {__index=function()projections=projections+1;return Identity()end})end;return unpack(t)end
function LaterError()calls=calls+1;return 7,setmetatable({}, {__index=function()error("later projection")end})end
function SourceError()error("source error")end
function UnsupportedError()error({})end
function CaughtRequired()pcall(Required);return false,true end
)";check(!dh2_script_vm_load_source_file(vm,code,std::strlen(code)),"Load");auto top=dh2_script_vm_stack_size(vm);unsigned source_calls=0,projected=0;
 for(unsigned round=0;round<64;++round)for(unsigned index=0;index<10;++index){
  check(!dh2_script_vm_call_indexed_source_v3(vm,"Returns",nullptr,0,index,Context::observe,&c),"Indexed delivery");++source_calls;++projected;check(c.result.count==8,"Full arity");check(dh2_script_vm_stack_size(vm)==top,"Source stack retained");
  if(index<2)check(c.result.type==1&&c.result.boolean==index,"Separate Check usable/active");
  else if(index==2)check(c.result.type==3&&c.result.number==17.75f,"Raw float3");
  else if(index==3)check(c.result.type==4&&c.result.text_bytes==1&&!std::strcmp(c.text,"a"),"First NUL");
  else if(index==4||index==5)check(c.result.type==(index==4?2u:7u)&&c.result.identity==0xabcde00111223344ull,"Native identity/full table projection");
  else check(!c.result.type&&!c.result.identity,"Absent/function/nil");
 }
 for(unsigned n:{1u,2u,17u,257u})for(unsigned i:{0u,1u,n-1,n,0xffffffffu}){dh2_script_value a{};a.type=3;a.number=float(n);check(!dh2_script_vm_call_indexed_source_v3(vm,"Many",&a,1,i,Context::observe,&c),"All table projections");++source_calls;projected+=n;check(c.result.count==n&&c.result.type==(i<n?7u:0u),"Selected table cardinality");}
 dh2_script_value v{};check(!dh2_script_vm_get_global(vm,"calls",&v)&&v.number==float(source_calls),"Exactly one Call per request");check(!dh2_script_vm_get_global(vm,"projections",&v)&&v.number==float(projected),"All returns projected once even outside selected index");
 auto before=c.observations;check(dh2_script_vm_call_indexed_source_v3(vm,"LaterError",nullptr,0,0,Context::observe,&c)>0&&c.observations==before,"Later projection failure precedes observer");check(dh2_script_vm_call_indexed_source_v3(vm,"SourceError",nullptr,0,1,Context::observe,&c)>0&&c.observations==before,"Source status");check(dh2_script_vm_call_indexed_source_v3(vm,"UnsupportedError",nullptr,0,0,Context::observe,&c)==-4,"Unsupported error");check(dh2_script_vm_call_indexed_source_v3(vm,"CaughtRequired",nullptr,0,1,Context::observe,&c)==-5&&c.result.type==1&&c.result.boolean==1,"Caught required epoch");c.reject=DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;check(dh2_script_vm_call_indexed_source_v3(vm,"Returns",nullptr,0,1,Context::observe,&c)==-5,"Observer failure");c.reject=0;
 check(!dh2_script_vm_call_indexed_source_v3(vm,"NoReturns",nullptr,0,1,Context::observe,&c)&&!c.result.count&&!c.result.type,"Empty no stale result");
 check(dh2_script_vm_call_indexed_source_v3(vm,"Returns",nullptr,0,0,nullptr,&c)==-1,"Null observer");check(dh2_script_vm_stack_size(vm)==top,"Final stack");check(c.busy==c.observations,"Every observer guarded");dh2_script_vm_destroy(vm);
 std::printf("{\"validation\":\"PASS\",\"source_calls\":%u,\"all_return_projections\":%u,\"observers\":%u,\"busy_guards\":%u,\"checks\":%u}\n",source_calls,projected,c.observations,c.busy,checks);return 0;
}catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}}
