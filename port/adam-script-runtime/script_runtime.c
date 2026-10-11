#include "script_runtime.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "ldo.h"
#include "lstate.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
typedef char number_must_be_float32[(sizeof(lua_Number)==4)?1:-1];
typedef char native_int_must_be_32[(sizeof(int)==4)?1:-1];
typedef char pointers_must_be_64[(sizeof(void*)==8)?1:-1];
struct dh2_script_vm { lua_State* state; size_t limit,used; char error[512]; int busy; uint64_t required_failure_epoch; };
typedef struct { dh2_script_function fn; void* context; int source_values; dh2_script_vm* vm; } Binding;
typedef struct {
  dh2_script_vm* vm; const char* name; const void* bytes; size_t size;
  const dh2_script_value* args; uint32_t count;
  dh2_script_value* out; uint32_t capacity,returned;
  dh2_script_function callback; void* context;
  unsigned char* binary; size_t binary_capacity,written; int status,source_values,library;
} Operation;
static int protected_operation(dh2_script_vm* vm,lua_CFunction fn,Operation* op);
static void* allocator(void* opaque,void* ptr,size_t old_size,size_t size) {
  dh2_script_vm* vm=(dh2_script_vm*)opaque; void* next;
  if (!ptr) old_size=0;
  if (!size) { free(ptr); vm->used-=old_size; return NULL; }
  if (size>old_size && size-old_size>vm->limit-vm->used) return NULL;
  next=realloc(ptr,size); if(next) vm->used=vm->used-old_size+size;
  return next;
}
static int valid_value(const dh2_script_value* v) {
  if(v->reserved) return 0;
  if(v->type==DH2_SCRIPT_STRING) return (!v->text_bytes||v->text)&&v->text_bytes<=8388608;
  if(v->type==DH2_SCRIPT_BOOLEAN) return v->boolean<=1;
  return v->type<=DH2_SCRIPT_NUMBER;
}
static void push(lua_State* L,const dh2_script_value* v) {
  switch(v->type) {
    case 0: lua_pushnil(L); break;
    case 1: lua_pushboolean(L,v->boolean); break;
    case 2: lua_pushlightuserdata(L,(void*)v->identity); break;
    case 3: lua_pushnumber(L,v->number); break;
    case 4: lua_pushlstring(L,v->text?v->text:"",v->text_bytes); break;
    default: luaL_error(L,"unsupported source value");
  }
}
static void pull(lua_State* L,int index,dh2_script_value* v) {
  memset(v,0,sizeof(*v)); v->type=(uint32_t)lua_type(L,index);
  switch(v->type) {
    case 0: case 5: case 6: case 7: case 8: break;
    case 1: v->boolean=(uint32_t)lua_toboolean(L,index); break;
    case 2: v->identity=(uintptr_t)lua_touserdata(L,index); break;
    case 3: v->number=lua_tonumber(L,index); break;
    case 4: v->text=lua_tolstring(L,index,&v->text_bytes); break;
    default: luaL_error(L,"missing Lua value");
  }
}
static int trampoline(lua_State* L) {
  Binding* b=(Binding*)lua_touserdata(L,lua_upvalueindex(1));
  dh2_script_value args[16],out[16]; uint32_t n=0,i; char error[256]={0};
  int count=lua_gettop(L),result; if(count>16) return luaL_error(L,"service argument limit");
  memset(out,0,sizeof(out));
  for(i=0;i<(uint32_t)count;i++) {
    pull(L,(int)i+1,&args[i]);
    if(b->source_values) {
      if(args[i].type==LUA_TTABLE) {
        lua_getfield(L,(int)i+1,"_this");
        args[i].type=LUA_TUSERDATA;
        args[i].identity=(uintptr_t)lua_touserdata(L,-1); lua_pop(L,1);
      } else if(args[i].type==LUA_TSTRING) {
        args[i].text_bytes=strlen(args[i].text);
      } else if(args[i].type>=LUA_TFUNCTION) {
        memset(&args[i],0,sizeof(args[i]));
      }
    } else if(!valid_value(&args[i])) return luaL_error(L,"unsupported service argument");
  }
  result=b->fn(b->context,args,(uint32_t)count,out,16,&n,error,sizeof(error));
  if(result) {
    if(result==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE)++b->vm->required_failure_epoch;
    error[sizeof(error)-1]=0; return luaL_error(L,"%s",error[0]?error:"game service rejected");
  }
  if(n>16) return luaL_error(L,"service result limit");
  for(i=0;i<n;i++) if(!valid_value(&out[i])) return luaL_error(L,"invalid service result");
  for(i=0;i<n;i++) push(L,&out[i]);
  return (int)n;
}
static int initialize(lua_State* L) {
  lua_pushcfunction(L,luaopen_base); lua_pushstring(L,""); lua_call(L,1,0);
  lua_pushcfunction(L,luaopen_table); lua_pushstring(L,LUA_TABLIBNAME); lua_call(L,1,0);
  lua_pushcfunction(L,luaopen_string); lua_pushstring(L,LUA_STRLIBNAME); lua_call(L,1,0);
  lua_pushcfunction(L,luaopen_math); lua_pushstring(L,LUA_MATHLIBNAME); lua_call(L,1,0);
  return 0;
}
static int open_library_entry(lua_State* L) {
  Operation* op=(Operation*)lua_touserdata(L,1);
  if(!op)return luaL_error(L,"invalid library request");
  switch(op->library) {
    case DH2_SCRIPT_LIBRARY_BASE:
      lua_pushcfunction(L,luaopen_base);lua_pushstring(L,"");lua_call(L,1,0);return 0;
    case DH2_SCRIPT_LIBRARY_MATH:
      lua_pushcfunction(L,luaopen_math);lua_pushstring(L,LUA_MATHLIBNAME);lua_call(L,1,0);return 0;
    case DH2_SCRIPT_LIBRARY_TABLE:
      lua_pushcfunction(L,luaopen_table);lua_pushstring(L,LUA_TABLIBNAME);lua_call(L,1,0);return 0;
    case DH2_SCRIPT_LIBRARY_STRING:
      lua_pushcfunction(L,luaopen_string);lua_pushstring(L,LUA_STRLIBNAME);lua_call(L,1,0);return 0;
    default:return luaL_error(L,"unsupported Lua library");
  }
}
static dh2_script_vm* create_vm(size_t limit,int open_default_libraries) {
  dh2_script_vm* vm; int little=1;
  if(*(char*)&little!=1||limit<65536||limit>1073741824) return NULL;
  vm=(dh2_script_vm*)calloc(1,sizeof(*vm)); if(!vm) return NULL;
  vm->limit=limit; vm->state=lua_newstate(allocator,vm);
  if(!vm->state) {free(vm);return NULL;}
  if(open_default_libraries&&lua_cpcall(vm->state,initialize,NULL)) {lua_close(vm->state);free(vm);return NULL;}
  return vm;
}
dh2_script_vm* dh2_script_vm_create(size_t limit) {return create_vm(limit,1);}
dh2_script_vm* dh2_script_vm_create_deferred(size_t limit) {return create_vm(limit,0);}
int dh2_script_vm_open_library(dh2_script_vm* vm,dh2_script_library library) {
  Operation op={0};if(!vm||library<DH2_SCRIPT_LIBRARY_BASE||library>DH2_SCRIPT_LIBRARY_STRING)return -1;
  op.library=(int)library;return protected_operation(vm,open_library_entry,&op);
}
void dh2_script_vm_destroy(dh2_script_vm* vm) {if(vm){lua_close(vm->state);free(vm);}}
static int protected_operation(dh2_script_vm* vm,lua_CFunction fn,Operation* op) {
  int top,status; if(!vm||vm->busy) return -1;
  top=lua_gettop(vm->state); vm->busy=1; vm->error[0]=0;
  status=lua_cpcall(vm->state,fn,op);
  if(status) {
    const char* message=lua_type(vm->state,-1)==LUA_TSTRING?lua_tostring(vm->state,-1):NULL;
    snprintf(vm->error,sizeof(vm->error),"%s",message?message:"Lua error (no text)");
  }
  lua_settop(vm->state,top); vm->busy=0;
  return status?-2:op->status;
}
static int load_entry(lua_State* L) {
  Operation* op=(Operation*)lua_touserdata(L,1);
  if(luaL_loadbuffer(L,(const char*)op->bytes,op->size,op->name)) return lua_error(L);
  lua_call(L,0,0); return 0;
}
static int valid_source(dh2_script_vm* vm,const void* bytes,size_t size,const char* name) {
  return vm&&bytes&&size&&size<=8388608&&name&&strlen(name)<4096;
}
int dh2_script_vm_load(dh2_script_vm* vm,const void* bytes,size_t size,const char* name) {
  Operation op={0}; if(!valid_source(vm,bytes,size,name))return -1;
  op.vm=vm;op.bytes=bytes;op.size=size;op.name=name;
  return protected_operation(vm,load_entry,&op);
}
/* These additive source operations deliberately use Lua's existing internal
 * protected call, without adding the lua_cpcall argument/C frame used by the
 * legacy convenience APIs. One state and allocator; no replacement VM/TU. */
typedef struct {
  const unsigned char* bytes;size_t size,offset;int status;char error[512];
} SourceFile;
static int source_aliases_vm(const dh2_script_vm* vm,const void* pointer,size_t size) {
  uintptr_t start=(uintptr_t)pointer,owner=(uintptr_t)vm;
  return size&&(start>UINTPTR_MAX-size || (start<owner+sizeof(*vm)&&owner<start+size));
}
static const char* source_file_reader(lua_State* L,void* context,size_t* size) {
  SourceFile* op=(SourceFile*)context;size_t n;(void)L;
  if(op->offset==op->size){*size=0;return NULL;}
  n=op->size-op->offset;if(n>1024)n=1024;
  *size=n;op->offset+=n;return (const char*)op->bytes+op->offset-n;
}
static void source_file_direct(lua_State* L,void* context) {
  SourceFile* op=(SourceFile*)context;const char* text;
  op->status=lua_load(L,source_file_reader,op,"loadFile()");
  if(!op->status)op->status=lua_pcall(L,0,0,0);
  if(op->status) {
    text=lua_tolstring(L,-1,NULL);
    if(text)snprintf(op->error,sizeof(op->error),"%s",text);
    else {op->status=-4;snprintf(op->error,sizeof(op->error),"%s","unsupported source file error object");}
    lua_pop(L,1);
  }
}
int dh2_script_vm_load_source_file(dh2_script_vm* vm,const void* bytes,size_t size) {
  SourceFile op;lua_State* L;int top,status;
  if(!vm||vm->busy||(!bytes&&size)||size>8388608||source_aliases_vm(vm,bytes,size))return -1;
  memset(&op,0,sizeof(op));op.bytes=(const unsigned char*)bytes;op.size=size;
  L=vm->state;top=lua_gettop(L);vm->busy=1;vm->error[0]=0;
  status=luaD_pcall(L,source_file_direct,&op,savestack(L,L->top),0);
  if(status) {
    const char* text=lua_type(L,-1)==LUA_TSTRING?lua_tostring(L,-1):NULL;
    snprintf(op.error,sizeof(op.error),"%s",text?text:"Lua error (no text)");
  } else status=op.status;
  snprintf(vm->error,sizeof(vm->error),"%s",op.error);
  lua_settop(L,top);vm->busy=0;return status;
}
static int writer(lua_State* L,const void* bytes,size_t size,void* opaque) {
  Operation* op=(Operation*)opaque; (void)L;
  if(size>SIZE_MAX-op->written) return 1;
  if(op->written<=op->binary_capacity&&size<=op->binary_capacity-op->written)
    memcpy(op->binary+op->written,bytes,size);
  else op->status=-3;
  op->written+=size; return 0;
}
static int compile_entry(lua_State* L) {
  Operation* op=(Operation*)lua_touserdata(L,1);
  if(luaL_loadbuffer(L,(const char*)op->bytes,op->size,op->name)) return lua_error(L);
  if(lua_dump(L,writer,op))return luaL_error(L,"chunk dump failed");
  return 0;
}
int dh2_script_vm_compile(dh2_script_vm* vm,const void* bytes,size_t size,const char* name,
  void* output,size_t capacity,size_t* written) {
  Operation op={0};int result;
  if(!valid_source(vm,bytes,size,name)||(!output&&capacity)||!written)return -1;
  op.bytes=bytes;op.size=size;op.name=name;op.binary=(unsigned char*)output;op.binary_capacity=capacity;
  result=protected_operation(vm,compile_entry,&op);*written=op.written;return result;
}
static int call_entry(lua_State* L) {
  Operation* op=(Operation*)lua_touserdata(L,1); uint32_t i; int top=lua_gettop(L);
  lua_getglobal(L,op->name);
  if(!lua_isfunction(L,-1)) return luaL_error(L,"missing function: %s",op->name);
  for(i=0;i<op->count;i++)push(L,&op->args[i]);
  lua_call(L,(int)op->count,LUA_MULTRET);op->returned=(uint32_t)(lua_gettop(L)-top);
  if(op->returned>op->capacity){op->status=-3;return 0;}
  for(i=0;i<op->returned;i++)pull(L,top+1+(int)i,&op->out[i]);return 0;
}
int dh2_script_vm_call(dh2_script_vm* vm,const char* name,const dh2_script_value* args,
  uint32_t count,dh2_script_value* out,uint32_t capacity,uint32_t* returned) {
  Operation op={0};uint32_t i;int status;
  if(!vm||!name||count>16||capacity>16||(!args&&count)||(!out&&capacity)||!returned)return -1;
  for(i=0;i<count;i++)if(!valid_value(&args[i]))return -1;
  op.name=name;op.args=args;op.count=count;op.out=out;op.capacity=capacity;
  status=protected_operation(vm,call_entry,&op);*returned=op.returned;return status;
}
static int discard_source_entry(lua_State* L) {
  Operation* op=(Operation*)lua_touserdata(L,1);uint32_t i;
  int first=lua_gettop(L)+1,last,index;
  lua_getglobal(L,op->name);
  if(!lua_isfunction(L,-1))return luaL_error(L,"missing function: %s",op->name);
  for(i=0;i<op->count;i++)push(L,&op->args[i]);
  lua_call(L,(int)op->count,LUA_MULTRET);last=lua_gettop(L);
  if(!lua_checkstack(L,1))return luaL_error(L,"source return projection stack exhausted");
  for(index=first;index<=last;index++) {
    switch(lua_type(L,index)) {
      case LUA_TTABLE:
        /* Source _setFromStack uses a normal getfield, not rawget: __index
         * remains synchronous even though the native pointer is discarded. */
        lua_getfield(L,index,"_this");(void)lua_touserdata(L,-1);lua_pop(L,1);break;
      case LUA_TSTRING: {
        const char* text=lua_tolstring(L,index,NULL);
        volatile size_t copied_length=strlen(text);(void)copied_length;
        /* Original std::string copy/destruction has no Lua callback effects. */
        break;
      }
      case LUA_TBOOLEAN:(void)lua_toboolean(L,index);break;
      case LUA_TNUMBER:(void)lua_tonumber(L,index);break;
      case LUA_TLIGHTUSERDATA:(void)lua_touserdata(L,index);break;
      default:break; /* Source maps function/thread/raw full userdata to nil. */
    }
  }
  return 0;
}
int dh2_script_vm_call_discard_source(dh2_script_vm* vm,const char* name,
  const dh2_script_value* args,uint32_t count) {
  Operation op={0};uint32_t i;
  if(!vm||!name||count>16||(!args&&count))return -1;
  for(i=0;i<count;i++)if(!valid_value(&args[i]))return -1;
  op.name=name;op.args=args;op.count=count;
  return protected_operation(vm,discard_source_entry,&op);
}
typedef struct {
  Operation call;uint32_t index;
  dh2_script_return_observer_v1 observer;
  dh2_script_returns_observer_v1 all_observer;void* context;char error[512];
} SourceReturn;
static void source_project_return(lua_State* L,int index,uint32_t count,dh2_script_first_return_v1* value) {
  memset(value,0,sizeof(*value));value->count=count;
  value->type=(uint32_t)lua_type(L,index);
  switch(value->type) {
    case LUA_TTABLE:
      lua_getfield(L,index,"_this");value->identity=(uintptr_t)lua_touserdata(L,-1);lua_pop(L,1);value->type=7;break;
    case LUA_TSTRING:
      value->text=lua_tolstring(L,index,NULL);value->text_bytes=strlen(value->text);break;
    case LUA_TBOOLEAN:
      value->boolean=(uint32_t)lua_toboolean(L,index);value->number=(float)value->boolean;break;
    case LUA_TNUMBER:value->number=lua_tonumber(L,index);break;
    case LUA_TLIGHTUSERDATA:value->identity=(uintptr_t)lua_touserdata(L,index);break;
    default:value->type=0;break;
  }
}
static void source_return_direct(lua_State* L,void* context) {
  SourceReturn* op=(SourceReturn*)context;dh2_script_first_return_v1 selected;
  dh2_script_first_return_v1* all=NULL;
  uint32_t i;int first,last,index,result;char error[256]={0};
  memset(&selected,0,sizeof(selected));
  if(!lua_checkstack(L,(int)op->call.count+20))luaL_error(L,"source call stack exhausted");
  first=lua_gettop(L)+1;lua_getglobal(L,op->call.name);
  if(!lua_isfunction(L,-1))luaL_error(L,"missing function: %s",op->call.name);
  for(i=0;i<op->call.count;++i)push(L,op->call.args+i);
  lua_call(L,(int)op->call.count,LUA_MULTRET);last=lua_gettop(L);
  if(!lua_checkstack(L,2))luaL_error(L,"source return projection stack exhausted");
  selected.count=(uint32_t)(last-first+1);
  if(op->all_observer&&selected.count) {
    if((size_t)selected.count>SIZE_MAX/sizeof(*all))luaL_error(L,"source return storage overflow");
    all=(dh2_script_first_return_v1*)lua_newuserdata(L,(size_t)selected.count*sizeof(*all));
  }
  for(index=first;index<=last;++index) {
    dh2_script_first_return_v1 value;source_project_return(L,index,selected.count,&value);
    if(all)all[index-first]=value;
    if((uint32_t)(index-first)==op->index)selected=value;
  }
  result=op->all_observer?op->all_observer(op->context,all,selected.count,error,sizeof(error)):
    op->observer(op->context,&selected,error,sizeof(error));
  if(result) {
    if(result==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE)++op->call.vm->required_failure_epoch;
    error[sizeof(error)-1]=0;luaL_error(L,"%s",error[0]?error:"source return observer rejected");
  }
}
static void source_error_direct(lua_State* L,void* context) {
  SourceReturn* op=(SourceReturn*)context;const char* text=lua_tolstring(L,-1,NULL);
  snprintf(op->error,sizeof(op->error),"%s",text?text:"unsupported source callback error object");
}
static int call_source(dh2_script_vm* vm,const char* name,
  const dh2_script_value* args,uint32_t count,uint32_t index,dh2_script_return_observer_v1 observer,
  dh2_script_returns_observer_v1 all_observer,void* context) {
  SourceReturn op;lua_State* L;uint32_t i;int top,status,error_status;uint64_t epoch;
  if(!vm||vm->busy||!name||(!observer&&!all_observer)||(!args&&count)||(uint64_t)count+20>INT_MAX)return -1;
  if(source_aliases_vm(vm,name,1))return -1;
  if(count&&((uintptr_t)args%sizeof(void*) || source_aliases_vm(vm,args,(size_t)count*sizeof(*args))))return -1;
  for(i=0;i<count;++i) {
    if(!valid_value(args+i))return -1;
    if(args[i].type==DH2_SCRIPT_STRING && source_aliases_vm(vm,args[i].text,args[i].text_bytes))return -1;
  }
  memset(&op,0,sizeof(op));op.call.vm=vm;op.call.name=name;op.call.args=args;op.call.count=count;
  op.index=index;op.observer=observer;op.all_observer=all_observer;op.context=context;L=vm->state;top=lua_gettop(L);
  epoch=vm->required_failure_epoch;vm->busy=1;vm->error[0]=0;
  status=luaD_pcall(L,source_return_direct,&op,savestack(L,L->top),0);
  if(status) {
    int type=lua_type(L,-1);
    error_status=luaD_pcall(L,source_error_direct,&op,savestack(L,L->top),0);
    if(error_status) {
      const char* text=lua_type(L,-1)==LUA_TSTRING?lua_tostring(L,-1):NULL;
      snprintf(op.error,sizeof(op.error),"%s",text?text:"Lua error (no text)");status=error_status;
    }else if(type!=LUA_TSTRING&&type!=LUA_TNUMBER)status=-4;
  }
  snprintf(vm->error,sizeof(vm->error),"%s",op.error);lua_settop(L,top);vm->busy=0;
  return vm->required_failure_epoch!=epoch?DH2_SCRIPT_REQUIRED_FAILURE_STATUS:status;
}
int dh2_script_vm_call_indexed_source_v3(dh2_script_vm* vm,const char* name,
  const dh2_script_value* args,uint32_t count,uint32_t index,dh2_script_return_observer_v1 observer,void* context) {
  return call_source(vm,name,args,count,index,observer,NULL,context);
}
int dh2_script_vm_call_all_source_v1(dh2_script_vm* vm,const char* name,
  const dh2_script_value* args,uint32_t count,dh2_script_returns_observer_v1 observer,void* context) {
  return call_source(vm,name,args,count,0,NULL,observer,context);
}
int dh2_script_vm_call_first_source_v1(dh2_script_vm* vm,const char* name,
  const dh2_script_value* args,uint32_t count,dh2_script_return_observer_v1 observer,void* context) {
  return dh2_script_vm_call_indexed_source_v3(vm,name,args,count,0,observer,context);
}
uint64_t dh2_script_vm_required_failure_epoch(const dh2_script_vm* vm) {
  return vm?vm->required_failure_epoch:0;
}
static int bind_entry(lua_State* L) {
  Operation* op=(Operation*)lua_touserdata(L,1);Binding* binding;
  binding=(Binding*)lua_newuserdata(L,sizeof(*binding)); binding->fn=op->callback;binding->context=op->context;
  binding->source_values=op->source_values;
  binding->vm=op->vm;
  lua_pushcclosure(L,trampoline,1);lua_setglobal(L,op->name);return 0;
}
int dh2_script_vm_bind(dh2_script_vm* vm,const char* name,dh2_script_function fn,void* context) {
  Operation op={0};if(!vm||!name||!fn)return -1;op.vm=vm;op.name=name;op.callback=fn;op.context=context;
  return protected_operation(vm,bind_entry,&op);
}
int dh2_script_vm_bind_source_values(dh2_script_vm* vm,const char* name,dh2_script_function fn,void* context) {
  Operation op={0};if(!vm||!name||!fn)return -1;op.vm=vm;op.name=name;op.callback=fn;op.context=context;op.source_values=1;
  return protected_operation(vm,bind_entry,&op);
}
static int get_entry(lua_State* L) {
  Operation* op=(Operation*)lua_touserdata(L,1);lua_getglobal(L,op->name);pull(L,-1,op->out);return 0;
}
int dh2_script_vm_get_global(dh2_script_vm* vm,const char* name,dh2_script_value* value) {
  Operation op={0};if(!vm||!name||!value)return -1;op.name=name;op.out=value;
  return protected_operation(vm,get_entry,&op);
}
const char* dh2_script_vm_error(const dh2_script_vm* vm){return vm?vm->error:"invalid VM";}
size_t dh2_script_vm_memory(const dh2_script_vm* vm){return vm?vm->used:0;}
