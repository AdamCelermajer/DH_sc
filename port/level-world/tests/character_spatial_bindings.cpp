#ifdef NDEBUG
#undef NDEBUG
#endif
#include "../character_spatial_bindings.hpp"
#include <cassert>
#include <array>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <dlfcn.h>
using dh2::character::SpatialBindings40;
using Event=std::array<uint32_t,3>;
constexpr uintptr_t identity_base=UINT64_C(0xfedcba9800000000);
static uint32_t bits(float f){uint32_t b;std::memcpy(&b,&f,4);return b;}
static float number(uint32_t b){float f;std::memcpy(&f,&b,4);return f;}
static bool nan(uint32_t b){return (b&0x7f800000)==0x7f800000&&(b&0x7fffff);}
static uint32_t read(std::ifstream& f){uint32_t v=0;f.read(reinterpret_cast<char*>(&v),4);assert(f);return v;}
static std::string blob(std::ifstream& f){std::string s(read(f),'\0');if(!s.empty())f.read(&s[0],s.size());assert(f);return s;}
struct Fixture {
 float positions[3][3]{};float replacement[3][3]{};unsigned plan[2]{},mutation=0,lookups=0;
 std::string names[2];std::vector<Event> trace;bool gold=true,reject=false;
 void mutate(unsigned bit){if(mutation&bit)std::memcpy(positions,replacement,sizeof positions);}
 static int named(void* pointer,const char* name,const float** out){
  auto& s=*static_cast<Fixture*>(pointer);unsigned index=s.lookups++,code=0;
  if(s.gold){assert(index<2&&!std::strcmp(name,s.names[index].c_str()));code=s.plan[index];s.trace.push_back({0,index,code});s.trace.push_back({1,index,code});s.mutate(1<<index);}
  else{
   s.names[index%2]=name;
   if(!std::strcmp(name,"first"))code=1;else if(!std::strcmp(name,"second"))code=2;else if(!std::strcmp(name,"self"))code=3;
   else if(!std::strcmp(name,"mutate")){code=2;s.positions[0][0]=30;s.positions[0][1]=40;}
   else if(!std::strcmp(name,"owner_mutate")){code=1;s.positions[2][0]=1;s.positions[2][1]=2;s.positions[2][2]=3;}
   s.trace.push_back({0,index,code});s.trace.push_back({1,index,code});
  }
  if(s.reject)return 1;*out=code?s.positions[code-1]:nullptr;return 0;
 }
 static int userdata(void* pointer,uintptr_t identity,const float** out){
  auto& s=*static_cast<Fixture*>(pointer);unsigned code=unsigned((identity-identity_base)/0x1000);assert(code>=1&&code<=3&&identity==identity_base+code*0x1000);
  s.trace.push_back({2,0,code});s.mutate(4);if(s.reject)return 1;*out=s.positions[code-1];return 0;
 }
 void reset(){const float source[3][3]={{3,4,12},{1,2,3},{-1,-2,-3}};std::memcpy(positions,source,sizeof positions);trace.clear();lookups=mutation=0;}
};
static void load(dh2_script_vm* vm,const char* text){assert(dh2_script_vm_load_source_file(vm,text,std::strlen(text))==0);}
static dh2_script_value global(dh2_script_vm* vm,const char* name){dh2_script_value v={};assert(!dh2_script_vm_get_global(vm,name,&v));return v;}
static int fixture_identity(void*,const dh2_script_value*,uint32_t,dh2_script_value* out,uint32_t cap,uint32_t* count,char*,size_t){assert(cap);*out={};out->type=2;out->identity=identity_base+0x1000;*count=1;return 0;}
int main(int argc,char** argv){
 assert(argc==2);std::ifstream file(argv[1],std::ios::binary);assert(file&&read(file)==0x31425053);uint32_t cases=read(file),services=0,VMchecks=0,guards=0;Fixture f;
 SpatialBindings40 source{f.positions[2],&f,&Fixture::named,&Fixture::userdata,0};
 const dh2_script_function callbacks[]={dh2_character_get_position,dh2_character_get_distance_from,dh2_character_get_distance_between};
 for(unsigned i=0;i<cases;++i){
  unsigned op=read(file),count=read(file);assert(op<3&&count<=3);unsigned kinds[3],ids[3];for(auto& v:kinds)v=read(file);for(auto& v:ids)v=read(file);for(auto& v:f.plan)v=read(file);f.mutation=read(file);f.lookups=0;f.trace.clear();
  for(auto& p:f.positions)for(auto& v:p)v=number(read(file));for(auto& p:f.replacement)for(auto& v:p)v=number(read(file));for(auto& name:f.names)name=blob(file);
  dh2_script_value a[3]{};for(unsigned j=0;j<count;++j){a[j].type=kinds[j];a[j].number=99.5f;a[j].identity=ids[j]?identity_base+ids[j]*0x1000:0;a[j].text=f.names[j?1:0].c_str();a[j].text_bytes=f.names[j?1:0].size();}
  dh2_script_value out[3]{};uint32_t returned=0xdeadbeef;char error[256]{};assert(!callbacks[op](&source,a,count,out,3,&returned,error,sizeof error)&&returned==read(file));
  for(unsigned j=0;j<returned;++j){auto expected=read(file),actual=bits(out[j].number);assert(out[j].type==3&&(actual==expected||(op&&nan(actual)&&nan(expected))));}
  for(auto& p:f.positions)for(auto v:p)assert(bits(v)==read(file));unsigned n=read(file);std::vector<Event> expected;for(unsigned j=0;j<n;++j){Event e{};for(auto& v:e)v=read(file);expected.push_back(e);}assert(expected==f.trace);services+=n;
 }
 char extra;assert(!file.read(&extra,1));f.gold=false;f.reset();auto* vm=dh2_script_vm_create(8*1024*1024);assert(vm&&!dh2_character_spatial_bind(vm,&source)&&!dh2_script_vm_bind(vm,"FixtureIdentity",&fixture_identity,nullptr));
 load(vm,"x,y,z=GetPosition(); d=GetDistanceFrom('first'); pair=GetDistanceBetween('first','second'); miss=GetDistanceFrom('missing'); function arity(...)return select('#',...)end; pcount=arity(GetPosition('ignored')); dcount=arity(GetDistanceFrom()); bcount=arity(GetDistanceBetween('first')); wrong=arity(GetDistanceFrom(3)); light=arity(GetDistanceFrom(FixtureIdentity())); table_distance=GetDistanceFrom({_this=FixtureIdentity()}); nulltable=GetDistanceFrom({})");
 const char* names[]={"x","y","z","d","pair","miss","pcount","dcount","bcount","wrong","light","table_distance","nulltable"};const float values[]={-1,-2,-3,277,89,-1,3,0,0,0,0,277,-1};for(unsigned i=0;i<13;++i){auto v=global(vm,names[i]);assert(v.type==3&&v.number==values[i]);++VMchecks;}
 f.trace.clear();f.lookups=0;load(vm,"first_missing=GetDistanceBetween('missing','second')");assert(global(vm,"first_missing").number==-1&&f.trace.size()==4&&f.trace[0][2]==0&&f.trace[2][2]==2);++VMchecks;
 f.reset();load(vm,"mutated=GetDistanceBetween('first','mutate')");assert(global(vm,"mutated").number==2366&&f.positions[0][0]==30&&f.positions[0][1]==40);++VMchecks;
 f.reset();load(vm,"owner_mutated=GetDistanceFrom('owner_mutate')");assert(global(vm,"owner_mutated").number==89&&f.positions[2][0]==1);++VMchecks;
 f.reset();load(vm,"equal=GetDistanceBetween('first','first'); projected=0;t=setmetatable({}, {__index=function(_,key) if key=='_this' then projected=projected+1 end return nil end});ignored=arity(GetDistanceFrom(2,t));x,y,z=GetPosition(t);nul=GetDistanceFrom('first'..string.char(0)..'tail'); extra_distance=GetDistanceBetween('first','second',t)");
 assert(global(vm,"equal").number==0&&global(vm,"projected").number==3&&global(vm,"ignored").number==0&&global(vm,"nul").number==277&&global(vm,"extra_distance").number==89);VMchecks+=5;
 f.reject=true;load(vm,"provider_ok,provider_error=pcall(function()return GetDistanceFrom('first')end)");assert(!global(vm,"provider_ok").boolean);auto err=global(vm,"provider_error");assert(err.type==4&&std::string(err.text,err.text_bytes).find("resolver failed")!=std::string::npos);VMchecks+=2;f.reject=false;
 load(vm,"limit_ok,limit_error=pcall(function()return GetPosition(unpack({1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17}))end)");assert(!global(vm,"limit_ok").boolean);err=global(vm,"limit_error");assert(err.type==4&&std::string(err.text,err.text_bytes).find("argument limit")!=std::string::npos);VMchecks+=2;
 dh2_script_vm_destroy(vm);
 // Genuine callback-capacity/malformed/provider boundaries, before resolution.
 dh2_script_value a[2]{};a[0].type=a[1].type=4;a[0].text="first";a[1].text="second";dh2_script_value out[3]{};uint32_t returned=123;char error[256];
 for(unsigned op=0;op<3;++op){f.trace.clear();assert(callbacks[op](&source,a,2,out,op?0:2,&returned,error,sizeof error)!=0&&returned==0&&f.trace.empty());++guards;}
 source.reserved=1;for(unsigned op=0;op<3;++op){assert(callbacks[op](&source,a,2,out,3,&returned,error,sizeof error)!=0&&returned==0);++guards;}source.reserved=0;
 source.named_position=nullptr;assert(dh2_character_get_distance_between(&source,a,2,out,3,&returned,error,sizeof error)!=0&&returned==0);++guards;
 source.userdata_position=nullptr;a[0].type=7;a[0].identity=identity_base+0x1000;assert(dh2_character_get_distance_from(&source,a,1,out,3,&returned,error,sizeof error)!=0&&returned==0);++guards;
 a[0].identity=0;assert(!dh2_character_get_distance_from(&source,a,1,out,3,&returned,error,sizeof error)&&returned==1&&out[0].number==-1);++guards;
 a[0].reserved=1;assert(dh2_character_get_distance_from(&source,a,1,out,3,&returned,error,sizeof error)!=0&&returned==0);++guards;
 for(unsigned op=0;op<3;++op){assert(callbacks[op](&source,nullptr,1,out,3,&returned,error,sizeof error)!=0);++guards;}
 source.owner_position=nullptr;assert(dh2_character_get_position(&source,a,0,out,3,&returned,error,sizeof error)!=0&&returned==0);++guards;
 source.named_position=&Fixture::named;a[0].reserved=0;a[0].type=4;f.reset();assert(!dh2_character_get_distance_between(&source,a,2,out,3,&returned,error,sizeof error)&&returned==1&&out[0].number==89);++guards;
 assert(!dh2_character_get_distance_from(nullptr,nullptr,0,out,3,&returned,error,sizeof error)&&returned==0);++guards;
 Dl_info kernel{},runtime{};assert(dladdr(reinterpret_cast<void*>(&dh2_character_get_position),&kernel)&&dladdr(reinterpret_cast<void*>(&dh2_script_vm_create),&runtime));
 std::printf("{\"validation\":\"PASS\",\"original_gold_cases\":%u,\"ordered_logical_lookup_cast_services\":%u,\"actual_VM_checks\":%u,\"native_atomic_and_unsupported_guards\":%u,\"mismatches\":0,\"kernel_library\":\"%s\",\"runtime_library\":\"%s\"}\n",cases,services,VMchecks,guards,kernel.dli_fname,runtime.dli_fname);
}
