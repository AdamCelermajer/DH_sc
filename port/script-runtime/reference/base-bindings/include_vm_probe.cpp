// A source-operation feasibility probe, not a ScriptOwner or public binding.
// Cached bytes/loaded-set are explicit fixtures; all parsing, nested execution,
// coercion/metamethods and Lua stack frames run the genuine float32 Lua DSO.
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>
struct Budget {std::size_t used=0,limit=16u*1024u*1024u;};
static void* allocate(void* p,void* old,std::size_t old_size,std::size_t size){
 auto& b=*static_cast<Budget*>(p);
 if(!size){if(old)b.used-=old_size;std::free(old);return nullptr;}
 const auto previous=old?old_size:0;
 if(size>b.limit-b.used+previous)return nullptr;
 void* out=std::realloc(old,size);if(out)b.used=b.used-previous+size;return out;
}
struct Manager {std::map<std::string,std::string> files,cache;std::vector<std::string> trace;unsigned requests=0,resets=0;};
struct Session {
 Budget budget;lua_State* L=nullptr;Manager& manager;std::set<std::string> loaded;
 unsigned includes=0,errors=0,stack_checks=0,projection_checks=0;
 explicit Session(Manager& m):manager(m){
  L=lua_newstate(allocate,&budget);assert(L);
  luaopen_base(L);luaopen_math(L);luaopen_table(L);luaopen_string(L);
  assert(lua_gettop(L)==5);
  lua_pushlightuserdata(L,this);lua_pushcclosure(L,include,1);lua_setglobal(L,"Include");
 }
 ~Session(){lua_close(L);assert(budget.used==0);}
 static int include(lua_State* L){
  auto& s=*static_cast<Session*>(lua_touserdata(L,lua_upvalueindex(1)));assert(s.L==L);
  ++s.includes;const int count=lua_gettop(L);
  // Source Arguments::_setFromStack projects every argument before _Include.
  // A table's _this field uses genuine normal field lookup even when unused.
  for(int i=1;i<=count;++i)if(lua_type(L,i)==LUA_TTABLE){
   lua_getfield(L,i,"_this");lua_pop(L,1);++s.projection_checks;
  }
  if(count&&lua_type(L,1)==LUA_TSTRING)s.load(lua_tostring(L,1));
  assert(lua_gettop(L)==count);++s.stack_checks;return 0;
 }
 bool load(const char* name){
  if(!name||!*name)return false;
  std::string filename="data/scripts/ai/";filename+=name;
  const char* suffix=std::strstr(name,".lua");
  if(!suffix)filename+=".luac";else if(std::strncmp(suffix,".luac",5))filename+='c';
  manager.trace.push_back("lookup:"+filename);
  if(loaded.count(filename)){manager.trace.push_back("loaded:"+filename);return true;}
  auto cached=manager.cache.find(filename);
  if(cached==manager.cache.end()){
   ++manager.requests;auto source=manager.files.find(filename);
   if(source==manager.files.end()){manager.trace.push_back("missing:"+filename);return false;}
   cached=manager.cache.emplace(filename,source->second).first;
   manager.trace.push_back("cache:"+filename);
  }else{++manager.resets;manager.trace.push_back("reset:"+filename);}
  const int top=lua_gettop(L);
  // Equivalent source lua_load reader outcome for this immutable byte fixture.
  int status=luaL_loadbuffer(L,cached->second.data(),cached->second.size(),"loadFile()");
  if(!status)status=lua_pcall(L,0,0,0);
  if(status){
   const char* error=lua_tostring(L,-1);assert(error); // actual source string-error domain
   manager.trace.push_back("error:"+filename+":"+std::to_string(status));
   lua_pop(L,1);++errors;
  }
  assert(lua_gettop(L)==top);++stack_checks;
  if(status)return false;
  loaded.insert(filename);manager.trace.push_back("insert:"+filename);return true;
 }
 float number(const char* name){lua_getglobal(L,name);assert(lua_isnumber(L,-1));auto n=lua_tonumber(L,-1);lua_pop(L,1);return n;}
};
static std::size_t index(const std::vector<std::string>& trace,const std::string& value){
 for(std::size_t i=0;i<trace.size();++i)if(trace[i]==value)return i;
 assert(false);return 0;
}
int main(){
 static_assert(sizeof(lua_Number)==4);static_assert(sizeof(void*)==8);
 Manager m;
 m.files["data/scripts/ai/leaf.luac"]="leaf=(leaf or 0)+1; return 1,2,3,setmetatable({}, {__index=function() return_error=1; error('returned projection forbidden') end})";
 m.files["data/scripts/ai/child.luac"]="Include('leaf',7,{},nil); child=(child or 0)+1";
 m.files["data/scripts/ai/root.luac"]="Include('child',1,2); Include('leaf'); root=(root or 0)+1";
 m.files["data/scripts/ai/broken.luac"]="broken=(broken or 0)+1; error('nested failure')";
 m.files["data/scripts/ai/syntax.luac"]="this is not valid Lua ???";
 m.files["data/scripts/ai/errors.luac"]="Include('absent'); Include('broken'); Include('syntax'); continued=1; Include('broken'); continued=continued+1";
 m.files["data/scripts/ai/cycle.luac"]="cycle=(cycle or 0)+1; if cycle<3 then Include('cycle') end";
 m.files["data/scripts/ai/types.luac"]="projection=0; local v=setmetatable({}, {__index=function() projection=projection+1; return nil end}); Include(); Include(19); Include(v); Include('leaf',v); Include('leaf\\000ignored')";
 m.files["data/scripts/ai/child.lua.extrac"]="substring=1";
 m.files["data/scripts/ai/projection-error.luac"]="Include('leaf',setmetatable({}, {__index=function() error('argument projection failure') end})); forbidden=1";
 unsigned checks=0,errors=0,includes=0,projections=0;
 {
  Session first(m);assert(first.load("root"));assert(first.number("leaf")==1&&first.number("child")==1&&first.number("root")==1);
  assert(index(m.trace,"insert:data/scripts/ai/leaf.luac")<index(m.trace,"insert:data/scripts/ai/child.luac"));
  assert(index(m.trace,"insert:data/scripts/ai/child.luac")<index(m.trace,"insert:data/scripts/ai/root.luac"));
  assert(first.load("errors"));assert(first.number("broken")==2&&first.number("continued")==2);
  assert(!first.loaded.count("data/scripts/ai/broken.luac")&&m.cache.count("data/scripts/ai/broken.luac"));
  assert(!first.loaded.count("data/scripts/ai/syntax.luac")&&m.cache.count("data/scripts/ai/syntax.luac"));
  assert(first.load("cycle"));assert(first.number("cycle")==3);
  assert(first.load("types"));assert(first.number("projection")==2);
  assert(first.load("child.lua.extra"));assert(first.number("substring")==1);
  assert(!first.load("projection-error"));lua_getglobal(first.L,"forbidden");assert(lua_isnil(first.L,-1));lua_pop(first.L,1);
  lua_getglobal(first.L,"return_error");assert(lua_isnil(first.L,-1));lua_pop(first.L,1);
  assert(lua_gettop(first.L)==5);
  // Shared immutable file cache, distinct owned VM globals and loaded set.
  Session second(m);assert(second.loaded.empty());assert(second.load("root"));assert(second.number("leaf")==1&&second.number("root")==1);
  assert(first.number("root")==1);assert(lua_gettop(second.L)==5);
  checks=first.stack_checks+second.stack_checks;errors=first.errors+second.errors;
  includes=first.includes+second.includes;projections=first.projection_checks+second.projection_checks;
 }
 std::cout<<"{\"validation\":\"PASS\",\"mismatches\":0,\"real_float32_lua\":true,\"owned_vms\":2,\"nested_include_callbacks\":"<<includes<<",\"stack_preservation_checks\":"<<checks<<",\"nested_errors\":"<<errors<<",\"argument_table_projections\":"<<projections<<",\"cache_resource_requests\":"<<m.requests<<",\"cache_resets\":"<<m.resets<<",\"source_operation_probe_only\":true}\n";
}
