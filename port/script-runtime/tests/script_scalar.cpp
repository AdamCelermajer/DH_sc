#ifdef NDEBUG
#undef NDEBUG
#endif
#include "../script_scalar_bindings.h"
#include <cassert>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
using Event=std::pair<uint32_t,uint32_t>;
struct Services {uint32_t identity;std::vector<Event> calls;};
static uint32_t bits(float f){uint32_t b;std::memcpy(&b,&f,4);return b;}
static float number(uint32_t b){float f;std::memcpy(&f,&b,4);return f;}
static uint32_t read(std::ifstream& f){uint32_t v=0;f.read(reinterpret_cast<char*>(&v),4);assert(f);return v;}
static int identity(void* p,uintptr_t id,uint32_t* out){auto& s=*static_cast<Services*>(p);assert(id==UINT64_C(0xfedcba9876543210));s.calls.emplace_back(0,s.identity);*out=s.identity;return 0;}
static int zero(void* p,int32_t numerator,int32_t* out){auto& s=*static_cast<Services*>(p);s.calls.emplace_back(1,static_cast<uint32_t>(numerator));*out=0x13579bdf;return 0;}
static int identity_reloading(void* p,uintptr_t id,uint32_t* out){auto& s=*static_cast<Services*>(p);assert(id==UINT64_C(0xfedcba9876543210));*out=s.calls.empty()?256:512;s.calls.emplace_back(0,*out);return 0;}
static dh2_script_value arg(float f){dh2_script_value v={};v.type=3;v.number=f;return v;}
static dh2_script_value global(dh2_script_vm* vm,const char* name){dh2_script_value v={};assert(dh2_script_vm_get_global(vm,name,&v)==0);return v;}
static void load(dh2_script_vm* vm,const char* text){assert(dh2_script_vm_load(vm,text,std::strlen(text),"scalar-host")==0);}
int main(int argc,char** argv){
  assert(argc==2);std::ifstream file(argv[1],std::ios::binary);assert(file);assert(read(file)==0x314c4353);uint32_t cases=read(file),services=0,vm_checks=0,guards=0;
  const dh2_script_function callback[]={dh2_script_scalar_to_fixed,dh2_script_scalar_from_fixed,dh2_script_scalar_mul_fixed,dh2_script_scalar_div_fixed,dh2_script_scalar_bit_not,dh2_script_scalar_bit_and,dh2_script_scalar_bit_or,dh2_script_scalar_bit_xor};
  Services state{};dh2_script_scalar_bindings source={&state,identity,zero,0};
  for(uint32_t i=0;i<cases;++i){
    uint32_t op=read(file),count=read(file);state.identity=read(file);uint32_t outputs=read(file),calls=read(file);assert(op<8&&count<=65&&outputs<=2);state.calls.clear();
    std::vector<dh2_script_value> args(count);
    for(auto& a:args){a={};a.type=read(file);a.number=number(read(file));a.boolean=read(file);if(a.type==4){a.text="1024.5";a.text_bytes=6;}if((a.type==2||a.type==7)&&state.identity)a.identity=UINT64_C(0xfedcba9876543210);}
    std::vector<uint32_t> expected(outputs);for(auto& v:expected)v=read(file);
    std::vector<Event> expected_calls;for(uint32_t j=0;j<calls;++j){uint32_t kind=read(file),value=read(file);if(kind!=2)expected_calls.emplace_back(kind,value);}
    dh2_script_value out[2]={};uint32_t returned=0xdeadbeef;char error[256]={};assert(callback[op](&source,args.data(),count,out,2,&returned,error,sizeof error)==0);assert(returned==outputs);
    for(uint32_t j=0;j<outputs;++j){assert(out[j].type==3&&bits(out[j].number)==expected[j]);}
    assert(state.calls==expected_calls);services+=state.calls.size();
  }
  char extra;assert(!file.read(&extra,1));
  auto* vm=dh2_script_vm_create(8*1024*1024);assert(vm);assert(dh2_script_scalar_bind(vm,&source)==0);
  load(vm,"a,b=FromFixed(513.75); t=ToFixed(1.99); m=MulFixed(513.75,256.75); d=DivFixed(1024,512); n=BitNot(0); x=BitXOr(9,3); aa=BitAnd(255,31,7); oo=BitOr(1,2,8)");
  const char* names[]={"a","b","t","m","d","n","x","aa","oo"};const float expected[]={2,513.0f/256,256,513,512,-1,10,7,11};
  for(unsigned i=0;i<9;++i){auto v=global(vm,names[i]);assert(v.type==3&&v.number==expected[i]);++vm_checks;}
  load(vm,"function arity(...) return select('#',...) end; ar0=arity(BitNot(true)); ar1=arity(ToFixed()); ar2=arity(FromFixed(1)); ar3=arity(BitXOr(1,2,3)); ar4=arity(BitAnd(1,2,'3')); ar5=arity(MulFixed(true,'256'))");
  const float ar[]={0,0,2,0,0,1};for(unsigned i=0;i<6;++i){std::string name="ar"+std::to_string(i);assert(global(vm,name.c_str()).number==ar[i]);++vm_checks;}
  load(vm,"s1,s2=FromFixed('1024.5'); h=ToFixed('0x100'); neg1,neg2=FromFixed('-256.5'); bad=ToFixed('garbage'); z=ToFixed(''); ws=ToFixed('  17  '); nul=ToFixed('17'..string.char(0)..'999')");
  const char* strings[]={"s1","s2","h","neg1","neg2","bad","z","ws","nul"};const float sv[]={4,4,65536,-1,-1,0,0,4352,4352};
  for(unsigned i=0;i<9;++i){assert(global(vm,strings[i]).number==sv[i]);++vm_checks;}
  load(vm,"proj=0; table_arg=setmetatable({}, {__index=function(t,k) if k=='_this' then proj=proj+1 end return nil end}); q1,q2=FromFixed(table_arg); discarded=ToFixed(3,table_arg); boolean=ToFixed(true); nilvalue=ToFixed(nil); functionvalue=ToFixed(function() end)");
  assert(global(vm,"proj").number==2);assert(global(vm,"q1").number==0&&global(vm,"q2").number==0);assert(global(vm,"discarded").number==768);assert(global(vm,"boolean").number==256);assert(global(vm,"nilvalue").number==0&&global(vm,"functionvalue").number==0);vm_checks+=7;
  load(vm,"ok_limit,err_limit=pcall(function() return BitOr(unpack({1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17})) end)");
  assert(global(vm,"ok_limit").type==1&&!global(vm,"ok_limit").boolean);auto limit=global(vm,"err_limit");assert(limit.type==4&&std::string(limit.text,limit.text_bytes).find("service argument limit")!=std::string::npos);vm_checks+=2;
  state.calls.clear();load(vm,"zero=DivFixed(123,255)");assert(global(vm,"zero").number==static_cast<float>(0x13579bdf));assert(state.calls==std::vector<Event>({{1,123}}));vm_checks+=2;
  dh2_script_vm_destroy(vm);
  auto* missing=dh2_script_vm_create(1024*1024);assert(missing);assert(dh2_script_scalar_bind(missing,nullptr)==0);load(missing,"ok,err=pcall(function() return DivFixed(1,255) end)");assert(!global(missing,"ok").boolean);auto err=global(missing,"err");assert(err.type==4&&std::string(err.text,err.text_bytes).find("zero-divisor service unavailable")!=std::string::npos);vm_checks+=2;dh2_script_vm_destroy(missing);
  char error[256]={};dh2_script_value out[2],a=arg(256);uint32_t returned;
  for(unsigned op=0;op<8;++op){assert(callback[op](nullptr,nullptr,1,out,2,&returned,error,sizeof error)!=0);++guards;}
  assert(dh2_script_scalar_from_fixed(nullptr,&a,1,out,1,&returned,error,sizeof error)!=0&&returned==0);++guards;
  a.type=7;a.identity=UINT64_C(0xfedcba9876543210);assert(dh2_script_scalar_to_fixed(nullptr,&a,1,out,2,&returned,error,sizeof error)!=0);++guards;
  state.calls.clear();source.identity=identity_reloading;assert(dh2_script_scalar_from_fixed(&source,&a,1,out,2,&returned,error,sizeof error)==0&&returned==2&&out[0].number==1&&out[1].number==2&&state.calls.size()==2);++guards;
  source.reserved=1;a=arg(1);assert(dh2_script_scalar_to_fixed(&source,&a,1,out,2,&returned,error,sizeof error)!=0);assert(dh2_script_scalar_bind(nullptr,&source)!=0);guards+=2;
  std::printf("{\"validation\":\"PASS\",\"original_gold_cases\":%u,\"ordered_identity_division_services\":%u,\"actual_VM_checks\":%u,\"guard_reload_checks\":%u,\"mismatches\":0}\n",cases,services,vm_checks,guards);
}
