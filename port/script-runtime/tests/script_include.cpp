#include "script_runtime.h"
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cstdint>
#include <cassert>
#include <cstring>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>
// Explicit immutable file-cache and receiver-owned loaded-set service fixture.
// The production capability, parser, protected execution and projection run.
struct Cache {std::map<std::string,std::string> files,bytes;unsigned requests=0,resets=0;std::vector<std::string> insertions;};
struct Session {
 Cache& cache;dh2_script_vm* vm;std::set<std::string> loaded;
 dh2_script_include_scope saved{},active{};unsigned callbacks=0,loads=0,errors=0,unsupported=0,guards=0;
 int last_status=0;std::string last_error;bool reject=false,deliver_empty=false;
 explicit Session(Cache& c):cache(c),vm(dh2_script_vm_create_empty(16u*1024u*1024u)){
  assert(vm);assert(dh2_script_vm_open_source_libraries(vm)==0);
  assert(dh2_script_vm_stack_size(vm)==5);
  assert(dh2_script_vm_bind_source_include(vm,include,this)==0);
 }
 ~Session(){dh2_script_vm_destroy(vm);}
 static int include(void* context,const dh2_script_include_scope* scope,const char* name,char* error,size_t capacity){
  auto& s=*static_cast<Session*>(context);++s.callbacks;s.saved=*scope;s.last_status=0;
  assert(dh2_script_include_scope_valid(scope)==1);++s.guards;
  dh2_script_value value{};std::uint32_t returned=0;
  assert(dh2_script_vm_stack_size(s.vm)==-1);
  assert(dh2_script_vm_get_global(s.vm,"x",&value)==-1);
  assert(dh2_script_vm_load(s.vm,"x=1",3,"generic")==-1);
  assert(dh2_script_vm_load_source_file(s.vm,nullptr,0)==-1);++s.guards;
  assert(dh2_script_vm_call_discard_source(s.vm,"OnTimer",nullptr,0)==-1);
  assert(dh2_script_vm_call(s.vm,"x",nullptr,0,nullptr,0,&returned)==-1);s.guards+=5;
  assert(dh2_script_include_load(scope,nullptr,1)==-1);++s.guards;
  if(s.active.vm){assert(dh2_script_include_scope_valid(&s.active)==0);assert(dh2_script_include_load(&s.active,nullptr,0)==-1);s.guards+=2;}
  const auto previous=s.active;s.active=*scope;
  if(s.reject){std::snprintf(error,capacity,"genuine provider unavailable");s.active=previous;return 1;}
  if(s.deliver_empty){assert(dh2_script_include_load(scope,nullptr,0)==0);++s.loads;}
  else s.load(*scope,name);
  if(s.last_status<0){std::snprintf(error,capacity,"%s",s.last_error.c_str());s.active=previous;return 1;}
  s.active=previous;
  // After nested callbacks the outer capability is restored and still valid.
  assert(dh2_script_include_scope_valid(scope)==1);++s.guards;
  assert(dh2_script_include_load(scope,nullptr,0)==0);++s.loads;
  return 0;
 }
 void load(const dh2_script_include_scope& scope,const char* name){
  if(!*name)return;
  std::string path="data/scripts/ai/";path+=name;const char* suffix=std::strstr(name,".lua");
  if(!suffix)path+=".luac";else if(std::strncmp(suffix,".luac",5))path+='c';
  if(loaded.count(path))return;
  auto found=cache.bytes.find(path);
  if(found==cache.bytes.end()){
   ++cache.requests;auto file=cache.files.find(path);if(file==cache.files.end())return;
   found=cache.bytes.emplace(path,file->second).first;
  }else ++cache.resets;
  ++loads;last_status=dh2_script_include_load(&scope,found->second.data(),found->second.size());
  last_error=dh2_script_include_error(&scope);
  if(last_status){if(last_status>0)++errors;else ++unsupported;assert(!last_error.empty());return;}
  loaded.insert(path);cache.insertions.push_back(path);
 }
 int source(const std::string& text){int result=dh2_script_vm_load(vm,text.data(),text.size(),"outer");assert(dh2_script_vm_stack_size(vm)==5);return result;}
 float number(const char* name){dh2_script_value v{};assert(dh2_script_vm_get_global(vm,name,&v)==0&&v.type==DH2_SCRIPT_NUMBER);return v.number;}
 bool absent(const char* name){dh2_script_value v{};assert(dh2_script_vm_get_global(vm,name,&v)==0);return v.type==DH2_SCRIPT_NIL;}
 void expired(){assert(dh2_script_include_scope_valid(&saved)==0);assert(dh2_script_include_load(&saved,nullptr,0)==-1);assert(std::strcmp(dh2_script_include_error(&saved),"invalid Include scope")==0);guards+=2;}
};
static std::size_t at(const std::vector<std::string>& v,const char* wanted){for(std::size_t i=0;i<v.size();++i)if(v[i]==wanted)return i;assert(false);return 0;}
static int identity(void*,const dh2_script_value*,uint32_t,dh2_script_value* out,uint32_t capacity,uint32_t* count,char*,size_t){assert(capacity);out[0]={};out[0].type=DH2_SCRIPT_IDENTITY;out[0].identity=0xabcdef0123456789ull;*count=1;return 0;}
int main(){
 Cache cache;
 cache.files["data/scripts/ai/leaf.luac"]="leaf=(leaf or 0)+1; return 1,2,setmetatable({}, {__index=function() forbidden_return_projection=1; error('not projected') end})";
 cache.files["data/scripts/ai/child.luac"]="Include('leaf',{},7); child=1";
 cache.files["data/scripts/ai/root.luac"]="Include('child',1,2); Include('leaf'); root=1";
 cache.files["data/scripts/ai/broken.luac"]="broken=(broken or 0)+1; error('nested runtime failure')";
 cache.files["data/scripts/ai/syntax.luac"]="not valid Lua ???";
 cache.files["data/scripts/ai/cycle.luac"]="cycle=(cycle or 0)+1; if cycle<3 then Include('cycle') end";
 cache.files["data/scripts/ai/numeric.luac"]="error(123,0)";
 cache.files["data/scripts/ai/object.luac"]="error({})";
 cache.files["data/scripts/ai/nul-error.luac"]="error('prefix\\000suffix')";
 cache.files["data/scripts/ai/child.lua.extrac"]="substring=1";
 unsigned guards=0,callbacks=0,loads=0,errors=0,unsupported=0,original_guards=0;
 {
  Session first(cache);assert(dh2_script_vm_bind(first.vm,"Identity",identity,nullptr)==0);
  const char* values[]={"nil","false","Identity()","3.5","'missing'","function() end","coroutine.create(function() end)","{}"};
  for(int count:{0,1,2,16,33})for(int kind=0;kind<8;++kind)for(int delivered=0;delivered<2;++delivered){
   first.deliver_empty=delivered!=0;unsigned before=first.callbacks;
   std::string source="Include(";if(count){source+=values[kind];for(int i=1;i<count;++i)source+=",0";}source+=")";
   assert(first.source(source)==0);assert(first.callbacks-before==unsigned(count&&kind==4));++original_guards;
  }
  first.deliver_empty=false;
  assert(first.source("assert(select('#',Include('root',7,{},nil))==0)")==0);
  assert(first.number("leaf")==1&&first.number("child")==1&&first.number("root")==1);
  assert(at(cache.insertions,"data/scripts/ai/leaf.luac")<at(cache.insertions,"data/scripts/ai/child.luac"));
  assert(at(cache.insertions,"data/scripts/ai/child.luac")<at(cache.insertions,"data/scripts/ai/root.luac"));
  assert(first.absent("forbidden_return_projection"));first.expired();
  assert(first.source("Include('broken'); continued=1; Include('syntax'); Include('broken'); continued=continued+1")==0);
  assert(first.number("broken")==2&&first.number("continued")==2);
  assert(!first.loaded.count("data/scripts/ai/broken.luac")&&cache.bytes.count("data/scripts/ai/broken.luac"));
  assert(first.last_error.find("loadFile()")!=std::string::npos);
  assert(first.source("Include('cycle')")==0&&first.number("cycle")==3);
  assert(first.source("projections=0; local t=setmetatable({}, {__index=function() projections=projections+1 end}); local a={'leaf'};for i=2,40 do a[i]=t end;Include(unpack(a));Include(t)")==0);
  assert(first.number("projections")==40); // 39 unused args plus rejected first table
  assert(first.source("Include('leaf',setmetatable({}, {__index=function() error('argument projection failure') end})); forbidden=1")==-2);
  assert(first.absent("forbidden"));first.expired();
  assert(first.source("Include('numeric'); resumed_numeric=1")==0&&first.last_status==2&&first.last_error=="123");
  assert(first.source("Include('object'); resumed_object=1")==-2&&first.last_status==-4&&first.absent("resumed_object"));
  assert(first.source("Include('nul-error'); resumed_nul=1")==0&&first.last_error.find("suffix")==std::string::npos);
  assert(first.source("Include('child.lua.extra');Include('leaf\\000ignored')")==0&&first.number("substring")==1);
  first.reject=true;assert(first.source("Include('leaf'); forbidden_provider=1")==-2);assert(first.absent("forbidden_provider"));first.reject=false;first.expired();
  assert(first.source("local c=coroutine.create(function() Include('leaf') end); local ok,msg=coroutine.resume(c); coroutine_rejected=(not ok) and string.find(msg,'unsupported source Include execution context') and 1 or 0")==0);
  assert(first.number("coroutine_rejected")==1);
  Session second(cache);assert(second.source("Include('root')")==0&&second.number("root")==1&&second.number("leaf")==1);assert(first.number("leaf")==1);
  second.expired();
  assert(dh2_script_include_load(nullptr,nullptr,0)==-1);assert(dh2_script_vm_bind_source_include(nullptr,Session::include,nullptr)==-1);
  assert(dh2_script_vm_bind_source_include(first.vm,nullptr,nullptr)==-1);
  guards=first.guards+second.guards+3;callbacks=first.callbacks+second.callbacks;loads=first.loads+second.loads;errors=first.errors+second.errors;unsupported=first.unsupported+second.unsupported;
 }
 // Exact source root loader uses the same primitive and retains library stack5.
 unsigned root_checks=0;
 {
  Session s(cache);const char* root="Include('root');root_loaded=1;return setmetatable({}, {__index=function() error('return projection forbidden') end})";
  assert(dh2_script_vm_load_source_file(s.vm,root,std::strlen(root))==0);
  assert(s.number("root_loaded")==1&&s.number("leaf")==1&&dh2_script_vm_stack_size(s.vm)==5);root_checks+=4;
  assert(dh2_script_vm_load_source_file(s.vm,nullptr,0)==0&&dh2_script_vm_stack_size(s.vm)==5);root_checks+=2;
  const char* syntax="not valid Lua ???";
  assert(dh2_script_vm_load_source_file(s.vm,syntax,std::strlen(syntax))==3);
  assert(std::strstr(dh2_script_vm_error(s.vm),"loadFile()")&&dh2_script_vm_stack_size(s.vm)==5);root_checks+=3;
  const char* failure="partial=7;error('root failure')";
  assert(dh2_script_vm_load_source_file(s.vm,failure,std::strlen(failure))==2);
  assert(std::strstr(dh2_script_vm_error(s.vm),"loadFile()")&&dh2_script_vm_stack_size(s.vm)==5);
  assert(s.number("partial")==7);root_checks+=4;
  const char* object="error({})";
  assert(dh2_script_vm_load_source_file(s.vm,object,std::strlen(object))==-4&&dh2_script_vm_stack_size(s.vm)==5);root_checks+=2;
  assert(dh2_script_vm_load_source_file(nullptr,nullptr,0)==-1&&dh2_script_vm_load_source_file(s.vm,nullptr,1)==-1);root_checks+=2;
  s.expired();guards+=s.guards;callbacks+=s.callbacks;loads+=s.loads;errors+=s.errors;
 }
 // Real allocator exhaustion is caught inside Include and caller continues.
 {
  Cache small;small.files["data/scripts/ai/memory.luac"]="local t={};for i=1,100000 do t[i]=string.rep('x',100) end";
  Session s(small); // replace before callback registration with bounded source VM
  dh2_script_vm_destroy(s.vm);s.vm=dh2_script_vm_create(65536);assert(s.vm);
  assert(dh2_script_vm_bind_source_include(s.vm,Session::include,&s)==0);
  const char* code="Include('memory'); after_memory=1";
  assert(dh2_script_vm_load(s.vm,code,std::strlen(code),"outer")==0);
  assert(s.last_status==4&&s.number("after_memory")==1);assert(dh2_script_vm_memory(s.vm)<=65536);
  s.expired();guards+=s.guards;callbacks+=s.callbacks;loads+=s.loads;errors+=s.errors;
 }
 std::cout<<"{\"validation\":\"PASS\",\"mismatches\":0,\"original_Include_guard_cases\":"<<original_guards<<",\"actual_include_callbacks\":"<<callbacks<<",\"actual_source_load_operations\":"<<loads<<",\"source_root_file_checks\":"<<root_checks<<",\"handled_source_errors\":"<<errors<<",\"unsupported_error_objects\":"<<unsupported<<",\"atomic_scope_and_generic_busy_checks\":"<<guards<<",\"unused_argument_projections\":40,\"private_vms_verified\":true,\"allocation_failure_protected\":true,\"generic_busy_guards_preserved\":true}\n";
}
