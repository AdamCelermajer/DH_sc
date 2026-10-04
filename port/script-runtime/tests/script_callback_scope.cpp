#ifdef NDEBUG
#undef NDEBUG
#endif
#include "../script_runtime.h"
#include <cassert>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
struct Context {
  dh2_script_vm* vm=nullptr;dh2_script_vm* other=nullptr;
  std::vector<dh2_script_callback_scope> active,copies;
  std::vector<dh2_script_include_scope> includes;
  unsigned callbacks=0,calls=0,guards=0,include_calls=0,projections=0;
  unsigned native_storage_checks=0;
  bool reject=false;std::vector<int> statuses;
};
static int dummy(void*,const dh2_script_value*,uint32_t,dh2_script_value*,uint32_t,uint32_t*,char*,size_t){return 0;}
static int scoped_dummy(void*,const dh2_script_callback_scope*,const dh2_script_value*,uint32_t,char*,size_t){return 0;}
static int include_dummy(void*,const dh2_script_include_scope*,const char*,char*,size_t){return 0;}
static void guarded(Context& c,const dh2_script_callback_scope* scope){
  assert(dh2_script_callback_scope_valid(scope));++c.guards;
  for(size_t i=0;i+1<c.active.size();++i){assert(!dh2_script_callback_scope_valid(&c.active[i]));++c.guards;}
  for(auto& include:c.includes){assert(!dh2_script_include_scope_valid(&include));++c.guards;}
  auto bad=*scope;bad.generation++;assert(!dh2_script_callback_scope_valid(&bad));bad=*scope;bad.vm=c.other;assert(!dh2_script_callback_scope_valid(&bad));c.guards+=2;
  dh2_script_value v={};uint32_t n=0;size_t written=0;char output[4];
  assert(dh2_script_vm_load(c.vm,"x=1",3,"forbidden")==-1);
  assert(dh2_script_vm_load_source_file(c.vm,"x=1",3)==-1);
  assert(dh2_script_vm_call(c.vm,"Leaf",nullptr,0,&v,1,&n)==-1);
  assert(dh2_script_vm_call_discard_source(c.vm,"Leaf",nullptr,0)==-1);
  assert(dh2_script_vm_bind(c.vm,"bad",dummy,nullptr)==-1);
  assert(dh2_script_vm_bind_source_values(c.vm,"bad",dummy,nullptr)==-1);
  assert(dh2_script_vm_bind_source_scoped(c.vm,"bad",scoped_dummy,nullptr)==-1);
  assert(dh2_script_vm_bind_source_include(c.vm,include_dummy,nullptr)==-1);
  assert(dh2_script_vm_get_global(c.vm,"x",&v)==-1);
  assert(dh2_script_vm_compile(c.vm,"x=1",3,"forbidden",output,sizeof output,&written)==-1);
  assert(dh2_script_vm_stack_size(c.vm)==-1);c.guards+=11;
  assert(dh2_script_callback_call_discard_source(&bad,"Leaf",nullptr,0)==-1);++c.guards;
  assert(dh2_script_callback_call_discard_source(scope,nullptr,nullptr,0)==-1);
  assert(dh2_script_callback_call_discard_source(scope,"Leaf",nullptr,1)==-1);
  dh2_script_value malformed={};malformed.reserved=1;assert(dh2_script_callback_call_discard_source(scope,"Leaf",&malformed,1)==-1);
  malformed.reserved=0;malformed.type=7;assert(dh2_script_callback_call_discard_source(scope,"Leaf",&malformed,1)==-1);
  malformed.type=3;assert(dh2_script_callback_call_discard_source(scope,"Leaf",&malformed,UINT32_MAX)==-1);c.guards+=5;
}
static int call(Context& c,const dh2_script_callback_scope* scope,const char* name,const dh2_script_value* args=nullptr,uint32_t count=0){
  int status=dh2_script_callback_call_discard_source(scope,name,args,count);++c.calls;assert(dh2_script_callback_scope_valid(scope));++c.guards;return status;
}
static int provider(void* opaque,const dh2_script_callback_scope* scope,const dh2_script_value* args,uint32_t count,char* error,size_t capacity){
  auto& c=*static_cast<Context*>(opaque);++c.callbacks;c.active.push_back(*scope);c.copies.push_back(*scope);guarded(c,scope);
  for(size_t i=0;i+1<c.copies.size();++i){assert(!dh2_script_callback_scope_valid(&c.copies[i]));++c.guards;}
  assert(count>=1&&args[0].type==4);std::string mode(args[0].text,args[0].text_bytes);
  if(mode=="outer"){
    assert(call(c,scope,"Post")==0);assert(call(c,scope,"Init")==0);
  }else if(mode=="inner"){
    assert(call(c,scope,"Inner")==0);
  }else if(mode=="nested_include"){
    assert(call(c,scope,"UseInclude")==0);
  }else if(mode=="from_include"){
    assert(call(c,scope,"Leaf")==0);
  }else if(mode=="many"){
    assert(call(c,scope,"Many")==0);std::vector<dh2_script_value> a(65);for(auto& v:a){v={};v.type=3;v.number=1;}
    assert(call(c,scope,"Args",a.data(),static_cast<uint32_t>(a.size()))==0);
  }else if(mode=="projection"){
    assert(count==34);for(uint32_t i=1;i<count;++i)assert(args[i].type==7&&!args[i].identity);c.projections+=33;
  }else if(mode=="errors"){
    for(auto name:{"StringError","NumberError","ObjectError","ProjectionError","Absent"}){
      int status=call(c,scope,name);c.statuses.push_back(status);
      std::string message=dh2_script_callback_error(scope);assert(!message.empty());
      if(std::strcmp(name,"ObjectError")==0)assert(status==-4&&message=="unsupported source callback error object");
      else assert(status==2);
      if(std::strcmp(name,"NumberError")==0)assert(message=="17");
    }
    assert(call(c,scope,"Leaf")==0);assert(!*dh2_script_callback_error(scope));
  }else if(mode=="reject"){
    std::snprintf(error,capacity,"native state provider rejected");c.active.pop_back();return 1;
  }else if(mode=="storage"){
    assert(count==2&&args[1].type==3);
    assert(dh2_script_vm_memory(c.vm)==static_cast<size_t>(args[1].number));++c.native_storage_checks;
  }else if(mode=="projfail"){
    for(unsigned i=0;i<64;++i){assert(call(c,scope,"ArgumentError")==2);assert(std::strstr(dh2_script_callback_error(scope),"argument projection error"));}
    assert(call(c,scope,"Leaf")==0);
  }else if(mode=="oom"){
    std::string huge(1024*1024,'x');dh2_script_value v={};v.type=4;v.text=huge.data();v.text_bytes=huge.size();
    int status=call(c,scope,"Args",&v,1);c.statuses.push_back(status);assert(status==4);assert(!std::strcmp(dh2_script_callback_error(scope),"not enough memory"));
    assert(call(c,scope,"Leaf")==0);
  }else assert(mode=="leaf");
  c.active.pop_back();return 0;
}
static int include(void* opaque,const dh2_script_include_scope* scope,const char* name,char*,size_t){
  auto& c=*static_cast<Context*>(opaque);++c.include_calls;assert(!std::strcmp(name,"nested"));
  assert(dh2_script_include_scope_valid(scope));++c.guards;
  for(auto& active:c.active){assert(!dh2_script_callback_scope_valid(&active));assert(dh2_script_callback_call_discard_source(&active,"Leaf",nullptr,0)==-1);c.guards+=2;}
  c.includes.push_back(*scope);const char* text="included=(included or 0)+1; Bridge('from_include')";
  assert(dh2_script_include_load(scope,text,std::strlen(text))==0);assert(dh2_script_include_scope_valid(scope));++c.guards;c.includes.pop_back();return 0;
}
static const char script[]=R"LUA(
log=''; projected=0; included=0
function Leaf() log=log..'L'; return 1 end
function Inner() log=log..'I'; return setmetatable({}, {__index=function(t,k) projected=projected+1; Bridge('leaf'); return nil end}) end
function Post() log=log..'P'; Bridge('inner'); log=log..'p' end
function Init() log=log..'N' end
function UseInclude() Include('nested') end
function Many() local a={} for i=1,80 do a[i]=setmetatable({}, {__index=function() projected=projected+1; return nil end}) end return unpack(a) end
function Args(...) arg_count=select('#',...) end
function StringError() before_error=1; error('source failure') end
function NumberError() error(17,0) end
function ObjectError() error({}) end
function ProjectionError() return setmetatable({}, {__index=function() error('projection failure') end}) end
function ArgumentError() Bridge('leaf',setmetatable({}, {__index=function() error('argument projection error') end})) end
Bridge('outer'); Bridge('nested_include'); Bridge('many'); Bridge('errors')
local many={'projection'} for i=1,33 do many[i+1]=setmetatable({}, {__index=function() projected=projected+1; return nil end}) end Bridge(unpack(many))
provider_ok,provider_error=pcall(function() Bridge('reject') end)
co=coroutine.create(function() Bridge('leaf') end); co_ok,co_error=coroutine.resume(co)
)LUA";
static dh2_script_value get(dh2_script_vm* vm,const char* name){dh2_script_value v={};assert(dh2_script_vm_get_global(vm,name,&v)==0);return v;}
int main(){
  Context c;c.vm=dh2_script_vm_create_empty(2*1024*1024);c.other=dh2_script_vm_create(65536);assert(c.vm&&c.other);assert(dh2_script_vm_open_source_libraries(c.vm)==0);assert(dh2_script_vm_stack_size(c.vm)==5);
  assert(dh2_script_vm_bind_source_scoped(c.vm,"Bridge",provider,&c)==0);assert(dh2_script_vm_bind_source_include(c.vm,include,&c)==0);
  assert(dh2_script_vm_load_source_file(c.vm,script,std::strlen(script))==0);assert(dh2_script_vm_stack_size(c.vm)==5);
  assert(c.active.empty()&&c.includes.empty());for(auto& scope:c.copies){assert(!dh2_script_callback_scope_valid(&scope));assert(dh2_script_callback_call_discard_source(&scope,"Leaf",nullptr,0)==-1);c.guards+=2;}
  assert(get(c.vm,"log").type==4);auto log=get(c.vm,"log");assert(std::string(log.text,log.text_bytes)=="PIpNLL");
  assert(get(c.vm,"arg_count").number==65&&get(c.vm,"projected").number==114&&get(c.vm,"included").number==1&&get(c.vm,"before_error").number==1);
  assert(!get(c.vm,"provider_ok").boolean&&!get(c.vm,"co_ok").boolean);auto err=get(c.vm,"co_error");assert(err.type==4&&std::string(err.text,err.text_bytes).find("unsupported source callback execution context")!=std::string::npos);
  const char* repeated="Bridge('outer'); Bridge('many')";assert(dh2_script_vm_load_source_file(c.vm,repeated,std::strlen(repeated))==0);assert(dh2_script_vm_stack_size(c.vm)==5);
  const char* storage="collectgarbage('collect'); collectgarbage('stop'); gc_count=0; do local p=newproxy(true); getmetatable(p).__gc=function() gc_count=gc_count+1 end end; Bridge('storage',collectgarbage('count')*1024); before_gc=gc_count; collectgarbage('restart'); collectgarbage('collect')";
  assert(dh2_script_vm_load_source_file(c.vm,storage,std::strlen(storage))==0);assert(get(c.vm,"before_gc").number==0&&get(c.vm,"gc_count").number==1);
  const char* projection_fail="Bridge('projfail')";assert(dh2_script_vm_load_source_file(c.vm,projection_fail,std::strlen(projection_fail))==0);assert(dh2_script_vm_stack_size(c.vm)==5);
  dh2_script_callback_scope malformed={};assert(!dh2_script_callback_scope_valid(nullptr)&&!dh2_script_callback_scope_valid(&malformed));assert(dh2_script_callback_call_discard_source(nullptr,"Leaf",nullptr,0)==-1);c.guards+=3;
  unsigned return_projections=static_cast<unsigned>(get(c.vm,"projected").number)-c.projections;assert(return_projections==162);
  unsigned positive_errors=0,unsupported_errors=0;for(int status:c.statuses){positive_errors+=status>0;unsupported_errors+=status==-4;}
  unsigned callbacks=c.callbacks,calls=c.calls,guards=c.guards,includes=c.include_calls,projections=c.projections,storage_checks=c.native_storage_checks;dh2_script_vm_destroy(c.vm);dh2_script_vm_destroy(c.other);
  Context oom;oom.vm=dh2_script_vm_create(65536);oom.other=dh2_script_vm_create(65536);assert(oom.vm&&oom.other);assert(dh2_script_vm_bind_source_scoped(oom.vm,"Bridge",provider,&oom)==0);
  const char* oomscript="function Args(...) end; function Leaf() recovered=1 end; Bridge('oom')";assert(dh2_script_vm_load_source_file(oom.vm,oomscript,std::strlen(oomscript))==0);assert(get(oom.vm,"recovered").number==1);assert(dh2_script_vm_stack_size(oom.vm)==0);
  callbacks+=oom.callbacks;calls+=oom.calls;guards+=oom.guards;for(int status:oom.statuses){positive_errors+=status>0;unsupported_errors+=status==-4;}dh2_script_vm_destroy(oom.vm);dh2_script_vm_destroy(oom.other);
  std::printf("{\"validation\":\"PASS\",\"actual_scoped_callbacks\":%u,\"actual_discarded_Lua_calls\":%u,\"scope_busy_guards\":%u,\"cross_Include_calls\":%u,\"source_argument_table_projections\":%u,\"source_return_table_projections\":%u,\"positive_Lua_error_cases\":%u,\"unsupported_error_object_cases\":%u,\"native_argument_storage_checks\":%u,\"argument_projection_error_cleanup_cases\":64,\"mismatches\":0}\n",callbacks,calls,guards,includes,projections,return_projections,positive_errors,unsupported_errors,storage_checks);
}
