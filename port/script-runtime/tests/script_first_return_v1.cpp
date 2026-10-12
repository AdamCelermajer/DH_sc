#include "../script_return_observer_v1.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <stdexcept>
struct Context {dh2_script_vm* vm{};dh2_script_first_return_v1 first{};unsigned calls{},busy_checks{};char text[64]{};int reject{};};
static int observe(void* p,const dh2_script_first_return_v1* r,char*,std::size_t){auto& c=*static_cast<Context*>(p);c.first=*r;if(r->text){if(r->text_bytes>=sizeof c.text)return -1;std::memcpy(c.text,r->text,r->text_bytes);c.text[r->text_bytes]=0;}++c.calls;std::uint32_t n=0;dh2_script_value out{};if(dh2_script_vm_stack_size(c.vm)==-1&&dh2_script_vm_call(c.vm,"Plain",nullptr,0,&out,1,&n)==-1)++c.busy_checks;return c.reject;}
static int identity(void*,const dh2_script_value*,std::uint32_t,dh2_script_value* out,std::uint32_t,std::uint32_t* n,char*,std::size_t){*out={};out->type=DH2_SCRIPT_IDENTITY;out->identity=0x1234;*n=1;return 0;}
static int missing(void*,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
static void check(bool b,const char* text){if(!b)throw std::runtime_error(text);}
int main(){try{auto* vm=dh2_script_vm_create(4u*1024u*1024u);check(vm,"VM allocation");Context c;c.vm=vm;check(!dh2_script_vm_bind(vm,"Identity",identity,nullptr)&&!dh2_script_vm_bind(vm,"Missing",missing,nullptr),"Binding");const char* code=R"(
counter=0
function Plain() counter=counter+1;return 2.5 end
function None() counter=counter+1 end
function Mixed() counter=counter+1;return 17.75,setmetatable({}, {__index=function(t,k) counter=counter+10;return Identity() end}),"a\000b",true,function()end end
function Table() return setmetatable({}, {__index=function(t,k) counter=counter+1;return Identity() end}) end
function Text() return "a\000b",nil,false end
function Many(n) local t={};for i=1,(n or 256) do t[i]=setmetatable({}, {__index=function() counter=counter+1;return Identity() end}) end;return unpack(t) end
function LaterProjectionFail() counter=counter+1;return 7,setmetatable({}, {__index=function() error("projection failed") end}) end
function Fail() counter=counter+1;error("source diagnostic") end
function CaughtMissing() pcall(Missing);return 8 end
function MissingCall() Missing() end
function Unsupported() error({}) end
function ReturnsNil() return function()end end
)";check(!dh2_script_vm_load(vm,code,std::strlen(code),"source-return-protocol"),"Load");int top=dh2_script_vm_stack_size(vm);unsigned checks=0;
auto call=[&](const char* name){auto n=c.calls;int status=dh2_script_vm_call_first_source_v1(vm,name,nullptr,0,observe,&c);check(dh2_script_vm_stack_size(vm)==top,"Stack preservation");if(!status)check(c.calls==n+1,"One observer");++checks;return status;};
check(!call("Plain")&&c.first.count==1&&c.first.type==3&&c.first.number==2.5f,"Number tag3");check(!call("None")&&!c.first.count&&!c.first.type,"No returns");check(!call("Mixed")&&c.first.count==5&&c.first.type==3&&c.first.number==17.75f,"All projections");dh2_script_value count{};check(!dh2_script_vm_get_global(vm,"counter",&count)&&count.number==13,"Actual one call plus later __index");
check(!call("Table")&&c.first.count==1&&c.first.type==7&&c.first.identity==0x1234,"Table identity");check(!call("Text")&&c.first.count==3&&c.first.type==4&&c.first.text_bytes==1&&!std::strcmp(c.text,"a"),"First NUL");check(!call("Many")&&c.first.count==256&&c.first.type==7,"Unlimited projections");check(!call("ReturnsNil")&&c.first.count==1&&!c.first.type,"Function to nil");
for(unsigned n:{1u,2u,17u,48u}){dh2_script_value argument{};argument.type=DH2_SCRIPT_NUMBER;argument.number=static_cast<float>(n);check(!dh2_script_vm_call_first_source_v1(vm,"Many",&argument,1,observe,&c)&&c.first.count==n&&c.first.type==7&&c.first.identity==0x1234,"Original arity/table projection protocol");++checks;}
unsigned before=c.calls;check(call("Fail")>0&&c.calls==before&&std::strstr(dh2_script_vm_error(vm),"source diagnostic"),"Lua error");check(call("Unsupported")==-4&&c.calls==before,"Unsupported source error");check(call("MissingCall")==-5&&c.calls==before,"Required error");check(call("CaughtMissing")==-5&&c.calls==before+1,"Caught required prefix");
before=c.calls;check(call("LaterProjectionFail")>0&&c.calls==before&&std::strstr(dh2_script_vm_error(vm),"projection failed"),"Later projection prevents first observer");
c.reject=DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;check(call("Plain")==-5,"Required observer");c.reject=0;check(dh2_script_vm_call_first_source_v1(vm,"Plain",nullptr,0,nullptr,&c)==-1,"Observer guard");check(c.busy_checks==c.calls,"Busy guards");dh2_script_vm_destroy(vm);std::printf("{\"validation\":\"PASS\",\"calls\":%u,\"observers\":%u,\"busy_guards\":%u,\"maximum_arity\":256,\"checks\":%u}\n",checks,c.calls,c.busy_checks,checks+13);return 0;}catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}}
