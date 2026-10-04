#ifdef NDEBUG
#undef NDEBUG
#endif
#include "../script_int_bindings.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
using Event=std::pair<uint32_t,uint32_t>;
struct Services {std::vector<Event> calls;};
static uint32_t bits(float f){uint32_t b;std::memcpy(&b,&f,4);return b;}
static float number(uint32_t b){float f;std::memcpy(&f,&b,4);return f;}
static uint32_t read(std::ifstream& f){uint32_t v=0;f.read(reinterpret_cast<char*>(&v),4);assert(f);return v;}
static std::string blob(std::ifstream& f){std::string s(read(f),'\0');if(!s.empty())f.read(&s[0],s.size());assert(f);return s;}
static int identity(void* p,uintptr_t id,uint32_t* out){assert(id==UINT64_C(0xfedcba9876543210));*out=0xfedcba98;static_cast<Services*>(p)->calls.emplace_back(0,*out);return 0;}
static int fraction(void* p,float f,char* out,size_t capacity,size_t* bytes){static_cast<Services*>(p)->calls.emplace_back(1,bits(f));int n=std::snprintf(out,capacity,"%f",static_cast<double>(f));assert(n>=0&&static_cast<size_t>(n)<capacity);*bytes=static_cast<size_t>(n);return 0;}
static dh2_script_value global(dh2_script_vm* vm,const char* name){dh2_script_value v={};assert(dh2_script_vm_get_global(vm,name,&v)==0);return v;}
static void load(dh2_script_vm* vm,const char* text){assert(dh2_script_vm_load(vm,text,std::strlen(text),"int-host")==0);}
static int fixture_identity(void*,const dh2_script_value*,uint32_t,dh2_script_value* out,uint32_t capacity,uint32_t* count,char*,size_t){assert(capacity);*out={};out->type=2;out->identity=UINT64_C(0xfedcba9876543210);*count=1;return 0;}
int main(int argc,char** argv){
 assert(argc==2);std::ifstream file(argv[1],std::ios::binary);assert(file&&read(file)==0x31544e49);uint32_t cases=read(file),checks=0,guards=0,services=0;
 dh2_script_int_map* maps[]={dh2_script_int_create(),dh2_script_int_create()};assert(maps[0]&&maps[1]);unsigned active=0;Services calls;dh2_script_int_bindings source{maps[0],&calls,identity,fraction,0};auto* workspace=dh2_script_string_projection_create();assert(workspace);
 for(uint32_t i=0;i<cases;++i){
  uint32_t op=read(file),value=read(file),count=read(file);std::string key=blob(file);std::vector<dh2_script_value> args(count);calls.calls.clear();
  for(unsigned j=0;j<count;++j){auto& a=args[j];a={};a.type=read(file);a.number=number(read(file));a.boolean=read(file);if(read(file))a.identity=UINT64_C(0xfedcba9876543210);a.text=j?"1024.5":key.c_str();a.text_bytes=j?6:key.size();}
  source.map=maps[active];int32_t v=0;uint32_t scalar=0,returned=0;dh2_script_value out[2]={};char error[256]={};const char* text=nullptr;
  switch(op){
  case 0:scalar=dh2_script_int_hash(key.c_str());assert(scalar==read(file));break;
  case 1:assert(dh2_script_int_set(source.map,key.c_str(),static_cast<int32_t>(value))==0);break;
  case 2:assert(dh2_script_int_get(source.map,key.c_str(),&v)==0&&static_cast<uint32_t>(v)==read(file));break;
  case 3:case 4:{auto callback=op==3?dh2_script_int_set_callback:dh2_script_int_get_callback;assert(callback(&source,args.data(),count,out,2,&returned,error,sizeof error)==0);assert(returned==read(file));for(unsigned j=0;j<returned;++j)assert(out[j].type==3&&bits(out[j].number)==read(file));break;}
  case 5:{assert(dh2_script_value_get_string(workspace,&source,args.data(),&text)==0);bool present=read(file),same=read(file);std::string expected=blob(file);assert(present==(text!=nullptr)&&same==(text==key.c_str()));if(present)assert(expected==text);break;}
  case 6:assert(dh2_script_value_get_integer(&source,args.data(),&v)==0&&static_cast<uint32_t>(v)==read(file));break;
  case 7:assert(dh2_script_int_clear_contents(source.map)==0);break;
  case 8:active=value;assert(active<2);break;
  default:assert(false);
  }
  for(auto* m:maps){uint32_t n=read(file);assert(dh2_script_int_size(m)==n);for(unsigned j=0;j<n;++j){uint32_t k=0;assert(dh2_script_int_entry(m,j,&k,&v)==0&&k==read(file)&&static_cast<uint32_t>(v)==read(file));}}
  uint32_t n=read(file);std::vector<Event> expected;for(unsigned j=0;j<n;++j){auto kind=read(file),word=read(file);expected.emplace_back(kind,word);}assert(expected==calls.calls);services+=n;
 }
 int32_t v=0;char extra;assert(!file.read(&extra,1));dh2_script_string_projection_destroy(workspace);
 for(auto* m:maps)assert(dh2_script_int_clear_contents(m)==0);
 auto* vm=dh2_script_vm_create(8*1024*1024);auto* other=dh2_script_vm_create(8*1024*1024);assert(vm&&other);source.map=maps[0];dh2_script_int_bindings other_source{maps[1],&calls,identity,fraction,0};assert(dh2_script_int_bind(vm,&source)==0&&dh2_script_int_bind(other,&other_source)==0);
 // Lua5.1's same-number constant reuse retains the earlier negative0 here.
 load(vm,"SetInt('ordinary',513.75); n=GetInt('ordinary'); missing=GetInt('missing'); SetInt('Alias16038',17); collision=GetInt('Alias16275'); SetInt(nil,21); nilkey=GetInt('nil'); SetInt(true,22); boolkey=GetInt('true'); SetInt(1.5,23); fractional=GetInt('1.500000'); SetInt(-0,24); zero=GetInt('0'); SetInt(1/0,25); infinity=GetInt('-2147483648')");
 const char* names[]={"n","missing","collision","nilkey","boolkey","fractional","zero","infinity"};const float values[]={513,0,17,21,22,23,24,25};for(unsigned i=0;i<8;++i){auto actual=global(vm,names[i]);if(actual.number!=values[i])std::fprintf(stderr,"VM %s actual %g expected %g\n",names[i],actual.number,values[i]);assert(actual.type==3&&actual.number==values[i]);++checks;}
 load(other,"n=GetInt('ordinary'); SetInt('ordinary',999); local0=GetInt('ordinary')");assert(global(other,"n").number==0&&global(other,"local0").number==999&&global(vm,"n").number==513);checks+=3;
 load(vm,"SetInt('parse',' 17 '); a=GetInt('parse'); SetInt('parse','0x100'); b=GetInt('parse'); SetInt('parse','garbage'); c=GetInt('parse'); SetInt('parse',true); d=GetInt('parse'); SetInt('parse',nil); e=GetInt('parse'); SetInt('parse',function() end); f=GetInt('parse'); SetInt('parse','17'..string.char(0)..'999'); nul=GetInt('parse')");
 const char* parsed[]={"a","b","c","d","e","f","nul"};const float pv[]={17,256,0,1,0,0,17};for(unsigned i=0;i<7;++i){assert(global(vm,parsed[i]).number==pv[i]);++checks;}
 load(vm,"function arity(...) return select('#',...) end; none=arity(SetInt('x',1,3)); absent=arity(SetInt('x')); empty=arity(GetInt()); one=arity(GetInt('x','extra')); projections=0; t=setmetatable({}, {__index=function(_,k) if k=='_this' then projections=projections+1 end return nil end}); SetInt(t,73); tablekey=GetInt('0x00000000'); SetInt('x',77,t); x=GetInt('x',t)");
 assert(global(vm,"none").number==0&&global(vm,"absent").number==0&&global(vm,"empty").number==0&&global(vm,"one").number==1&&global(vm,"projections").number==3&&global(vm,"tablekey").number==73&&global(vm,"x").number==77);checks+=7;
 assert(dh2_script_vm_bind(vm,"FixtureIdentity",fixture_identity,nullptr)==0);
 load(vm,"id=FixtureIdentity(); SetInt({_this=id},91); idkey=GetInt('0xfedcba98'); SetInt('identity_number',{_this=id}); identity_number=GetInt('identity_number'); SetInt('before'..string.char(0)..'after',93); nulkey=GetInt('before'); ok_function,err_function=pcall(function() SetInt(function() end,1) end)");
 assert(global(vm,"idkey").number==91&&global(vm,"identity_number").number==2147483648.0f&&global(vm,"nulkey").number==93&&global(vm,"ok_function").boolean);assert(global(vm,"err_function").type==0);int32_t nil_value=0;assert(dh2_script_int_get(maps[0],"nil",&nil_value)==0&&nil_value==1);checks+=6;
 auto* missing_vm=dh2_script_vm_create(1024*1024);assert(missing_vm);dh2_script_int_bindings missing_source{maps[1],nullptr,nullptr,nullptr,0};assert(dh2_script_int_bind(missing_vm,&missing_source)==0&&dh2_script_vm_bind(missing_vm,"FixtureIdentity",fixture_identity,nullptr)==0);
 size_t unchanged=dh2_script_int_size(maps[1]);load(missing_vm,"ok_fraction=pcall(function() SetInt(1.5,1) end); ok_identity=pcall(function() GetInt(FixtureIdentity()) end)");assert(!global(missing_vm,"ok_fraction").boolean&&!global(missing_vm,"ok_identity").boolean&&dh2_script_int_size(maps[1])==unchanged);checks+=3;dh2_script_vm_destroy(missing_vm);
 assert(dh2_script_int_clear_contents(maps[0])==0);load(vm,"finalizer=newproxy(true); getmetatable(finalizer).__gc=function() SetInt('gc',81); assert(GetInt('gc')==81) end");dh2_script_vm_destroy(vm);assert(dh2_script_int_get(maps[0],"gc",&v)==0&&v==81);++checks;dh2_script_vm_destroy(other);
 workspace=dh2_script_string_projection_create();dh2_script_value a={};a.type=3;a.number=1.5f;const char* projected=nullptr;assert(dh2_script_value_get_string(workspace,nullptr,&a,&projected)==-3);++guards;
 a.type=7;a.identity=UINT64_C(0xfedcba9876543210);assert(dh2_script_value_get_string(workspace,nullptr,&a,&projected)==-3&&dh2_script_value_get_integer(nullptr,&a,&v)==-3);guards+=2;
 for(unsigned kind:{5u,6u,8u}){a={};a.type=kind;assert(dh2_script_value_get_string(workspace,&source,&a,&projected)==0&&!projected);++guards;dh2_script_value args[]={a,{}};args[1].type=3;args[1].number=5;uint32_t returned=123;char error[256];size_t previous=dh2_script_int_size(maps[0]);assert(dh2_script_int_set_callback(&source,args,2,nullptr,0,&returned,error,sizeof error)!=0&&returned==0&&dh2_script_int_size(maps[0])==previous);++guards;}
 source.reserved=1;a={};assert(dh2_script_value_get_string(workspace,&source,&a,&projected)==-1);++guards;source.reserved=0;
 a.type=4;a.text="atomic_miss";uint32_t returned=123;char error[256];size_t previous=dh2_script_int_size(maps[0]);assert(dh2_script_int_get_callback(&source,&a,1,nullptr,0,&returned,error,sizeof error)!=0&&returned==0&&dh2_script_int_size(maps[0])==previous);++guards;
 assert(dh2_script_int_get(nullptr,"x",&v)==-1&&dh2_script_int_set(maps[0],nullptr,1)==-1&&dh2_script_int_entry(maps[0],SIZE_MAX,nullptr,&v)==-1);guards+=3;
 const char* embedded="first\0tail";a.text=embedded;a.text_bytes=10;assert(dh2_script_value_get_string(workspace,&source,&a,&projected)==0&&projected==embedded);++guards;
 dh2_script_string_projection_destroy(workspace);for(auto* m:maps)dh2_script_int_destroy(m);
 std::printf("{\"validation\":\"PASS\",\"original_gold_cases\":%u,\"complete_private_map_snapshots\":%u,\"ordered_identity_format_services\":%u,\"actual_VM_checks\":%u,\"atomic_unsupported_guards\":%u,\"mismatches\":0}\n",cases,cases*2,services,checks,guards);
}
