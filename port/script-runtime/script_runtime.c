#include "script_runtime.h"
#include "script_object_bridge_internal.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "ldo.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
typedef char number_must_be_float32[(sizeof(lua_Number)==4)?1:-1];
typedef char native_int_must_be_32[(sizeof(int)==4)?1:-1];
typedef char pointers_must_be_64[(sizeof(void*)==8)?1:-1];
typedef struct IncludeFrame IncludeFrame;
typedef struct CallbackFrame CallbackFrame;
struct dh2_script_vm { lua_State* state; size_t limit,used; char error[512]; int busy;
  IncludeFrame* include; uint64_t scope_generation;
  CallbackFrame* callback; void* active_provider;
  const dh2_script_object_services* objects; uint64_t required_failure_epoch; };
struct IncludeFrame { dh2_script_include_scope scope; IncludeFrame* previous;
  char error[512]; };
typedef struct {dh2_script_vm* vm;dh2_script_include_provider fn;void* context;} IncludeBinding;
struct CallbackFrame {dh2_script_callback_scope scope;CallbackFrame* previous;char error[512];};
typedef struct {dh2_script_vm* vm;dh2_script_scoped_function fn;void* context;} ScopedBinding;
typedef struct { dh2_script_function fn; void* context; int source_values; dh2_script_vm* vm; } Binding;
typedef struct {
  dh2_script_vm* vm; const char* name; const void* bytes; size_t size;
  const dh2_script_value* args; uint32_t count;
  dh2_script_value* out; uint32_t capacity,returned;
  dh2_script_function callback; void* context;
  unsigned char* binary; size_t binary_capacity,written; int status,source_values;
  dh2_script_include_provider include_callback;
  dh2_script_scoped_function scoped_callback;
} Operation;
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
  int result,count=lua_gettop(L); if(count>16) return luaL_error(L,"service argument limit");
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
  for(i=0;i<n;i++) if(!valid_value(&out[i])&&!(b->source_values==2&&out[i].type==DH2_SCRIPT_SOURCE_OBJECT&&!out[i].reserved)) return luaL_error(L,"invalid service result");
  for(i=0;i<n;i++) {
    if(b->source_values==2&&out[i].type==DH2_SCRIPT_SOURCE_OBJECT)
      dh2_script_object_push(L,b->vm,b->vm->objects,out[i].identity);
    else push(L,&out[i]);
  }
  return (int)n;
}
static int initialize(lua_State* L) {
  lua_pushcfunction(L,luaopen_base); lua_pushstring(L,""); lua_call(L,1,0);
  lua_pushcfunction(L,luaopen_table); lua_pushstring(L,LUA_TABLIBNAME); lua_call(L,1,0);
  lua_pushcfunction(L,luaopen_string); lua_pushstring(L,LUA_STRLIBNAME); lua_call(L,1,0);
  lua_pushcfunction(L,luaopen_math); lua_pushstring(L,LUA_MATHLIBNAME); lua_call(L,1,0);
  return 0;
}
dh2_script_vm* dh2_script_vm_create_empty(size_t limit) {
  dh2_script_vm* vm; int little=1;
  if(*(char*)&little!=1||limit<65536||limit>1073741824) return NULL;
  vm=(dh2_script_vm*)calloc(1,sizeof(*vm)); if(!vm) return NULL;
  vm->limit=limit; vm->state=lua_newstate(allocator,vm);
  if(!vm->state) {free(vm);return NULL;}
  return vm;
}
dh2_script_vm* dh2_script_vm_create(size_t limit) {
  dh2_script_vm* vm=dh2_script_vm_create_empty(limit);
  if(!vm)return NULL;
  if(lua_cpcall(vm->state,initialize,NULL)) {lua_close(vm->state);free(vm);return NULL;}
  return vm;
}
static void direct_source_library(lua_State* L,void* opaque) {
  uint32_t library=*(uint32_t*)opaque;
  /* Growth is inside the protected region. Direct opens need their own C API
   * stack reserve since there is no enclosing lua_call frame to supply it. */
  if(!lua_checkstack(L,20))luaL_error(L,"source library stack exhausted");
  switch(library) {
    case DH2_SCRIPT_LIBRARY_BASE:(void)luaopen_base(L);break;
    case DH2_SCRIPT_LIBRARY_MATH:(void)luaopen_math(L);break;
    case DH2_SCRIPT_LIBRARY_TABLE:(void)luaopen_table(L);break;
    case DH2_SCRIPT_LIBRARY_STRING:(void)luaopen_string(L);break;
  }
}
int dh2_script_vm_open_source_library(dh2_script_vm* vm,uint32_t library) {
  int top,status;
  if(!vm||vm->busy||library>DH2_SCRIPT_LIBRARY_STRING)return -1;
  top=lua_gettop(vm->state);vm->busy=1;vm->error[0]=0;
  /* Lua's pinned internal protected-call primitive preserves successful
   * direct-call stack results. It also protects the very first allocation;
   * pushing a new C closure before lua_pcall would be outside protection. */
  status=luaD_pcall(vm->state,direct_source_library,&library,
                    savestack(vm->state,vm->state->top),0);
  if(status) {
    const char* message=lua_type(vm->state,-1)==LUA_TSTRING?lua_tostring(vm->state,-1):NULL;
    snprintf(vm->error,sizeof(vm->error),"%s",message?message:"Lua error (no text)");
    lua_settop(vm->state,top);
  }
  vm->busy=0;return status?-2:0;
}
int dh2_script_vm_open_source_libraries(dh2_script_vm* vm) {
  uint32_t library;int result;
  if(!vm||vm->busy)return -1;
  for(library=DH2_SCRIPT_LIBRARY_BASE;library<=DH2_SCRIPT_LIBRARY_STRING;library++) {
    result=dh2_script_vm_open_source_library(vm,library);if(result)return result;
  }
  return 0;
}
int dh2_script_vm_stack_size(const dh2_script_vm* vm) {
  return !vm||vm->busy?-1:lua_gettop(vm->state);
}
void dh2_script_vm_destroy(dh2_script_vm* vm) {
  if(vm) {
    /* lua_close runs source __gc methods. Their scoped object/target callbacks
     * are executing Lua just like callbacks under protected_operation; retain
     * the busy guard so dedicated scopes are valid and generic reentry fails.
     * Every bound owner/provider must remain alive until close returns. */
    vm->busy=1;
    lua_close(vm->state);
    free(vm);
  }
}
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
static void discard_source_values(lua_State* L,Operation* op) {
  uint32_t i;
  int first=lua_gettop(L)+1,last,index;
  lua_getglobal(L,op->name);
  if(!lua_isfunction(L,-1))luaL_error(L,"missing function: %s",op->name);
  for(i=0;i<op->count;i++) {
    if(op->source_values==2&&op->args[i].type==DH2_SCRIPT_SOURCE_OBJECT)
      dh2_script_object_push(L,op->vm,op->vm->objects,op->args[i].identity);
    else push(L,&op->args[i]);
  }
  lua_call(L,(int)op->count,LUA_MULTRET);last=lua_gettop(L);
  if(!lua_checkstack(L,1))luaL_error(L,"source return projection stack exhausted");
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
}
static int discard_source_entry(lua_State* L) {
  discard_source_values(L,(Operation*)lua_touserdata(L,1));return 0;
}
int dh2_script_vm_call_discard_source(dh2_script_vm* vm,const char* name,
  const dh2_script_value* args,uint32_t count) {
  Operation op={0};uint32_t i;
  if(!vm||!name||count>16||(!args&&count))return -1;
  for(i=0;i<count;i++)if(!valid_value(&args[i]))return -1;
  op.name=name;op.args=args;op.count=count;
  return protected_operation(vm,discard_source_entry,&op);
}
int dh2_script_vm_call_discard_source_objects(dh2_script_vm* vm,const char* name,
  const dh2_script_value* args,uint32_t count) {
  Operation op={0};uint32_t i;
  if(!vm||!name||count>16||(!args&&count))return -1;
  for(i=0;i<count;i++)if(!valid_value(args+i)&&
    !(args[i].type==DH2_SCRIPT_SOURCE_OBJECT&&!args[i].reserved))return -1;
  op.vm=vm;op.name=name;op.args=args;op.count=count;op.source_values=2;
  return protected_operation(vm,discard_source_entry,&op);
}
static int bind_entry(lua_State* L) {
  Operation* op=(Operation*)lua_touserdata(L,1);Binding* binding;
  binding=(Binding*)lua_newuserdata(L,sizeof(*binding)); binding->fn=op->callback;binding->context=op->context;
  binding->vm=op->vm;
  binding->source_values=op->source_values;
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
int dh2_script_vm_set_source_objects(dh2_script_vm* vm,const dh2_script_object_services* objects){
  if(!vm||vm->busy||vm->objects||!objects||!objects->type_name||!objects->methods||!objects->invoke)return -1;
  vm->objects=objects;return 0;
}
int dh2_script_vm_bind_source_objects(dh2_script_vm* vm,const char* name,dh2_script_function fn,void* context){
  Operation op={0};if(!vm||!vm->objects||!name||!fn)return -1;
  op.vm=vm;op.name=name;op.callback=fn;op.context=context;op.source_values=2;
  return protected_operation(vm,bind_entry,&op);
}
static IncludeFrame* include_frame(const dh2_script_include_scope* scope) {
  dh2_script_vm* vm;
  if(!scope||!(vm=scope->vm)||!vm->busy||!vm->include||vm->active_provider!=vm->include||
     scope->generation!=vm->include->scope.generation)return NULL;
  return vm->include;
}
typedef struct {const unsigned char* bytes;size_t size,offset;IncludeFrame* frame;int status;} IncludeLoad;
static const char* include_reader(lua_State* L,void* opaque,size_t* size) {
  IncludeLoad* op=(IncludeLoad*)opaque;size_t n; (void)L;
  if(op->offset==op->size){*size=0;return NULL;}
  n=op->size-op->offset;if(n>1024)n=1024;
  *size=n;op->offset+=n;return (const char*)op->bytes+op->offset-n;
}
static void include_direct(lua_State* L,void* opaque) {
  IncludeLoad* op=(IncludeLoad*)opaque;const char* message;
  op->status=lua_load(L,include_reader,op,"loadFile()");
  if(!op->status)op->status=lua_pcall(L,0,0,0);
  if(op->status) {
    /* Error::setError uses lua_tolstring and copies through first NUL.
     * Preserve numeric conversion under protection; the source null-string
     * error-object crash domain is an explicit unsupported native result. */
    message=lua_tolstring(L,-1,NULL);
    if(message)snprintf(op->frame->error,sizeof(op->frame->error),"%s",message);
    else {op->status=-4;snprintf(op->frame->error,sizeof(op->frame->error),
      "%s","unsupported source Include error object");}
    lua_pop(L,1);
  }
}
static int source_file_load(dh2_script_vm* vm,IncludeFrame* frame,const void* bytes,size_t size) {
  IncludeLoad op;lua_State* L;int top,status;
  memset(&op,0,sizeof(op));op.bytes=(const unsigned char*)bytes;op.size=size;op.frame=frame;
  frame->error[0]=0;L=vm->state;top=lua_gettop(L);
  /* Direct protected operation adds no C frame/arguments and preserves the
   * caller frame's stack. It protects error-number conversion/OOM too. */
  status=luaD_pcall(L,include_direct,&op,savestack(L,L->top),0);
  if(status){
    const char* message=lua_type(L,-1)==LUA_TSTRING?lua_tostring(L,-1):NULL;
    snprintf(frame->error,sizeof(frame->error),"%s",message?message:"Lua error (no text)");
    lua_settop(L,top);return status;
  }
  return op.status;
}
int dh2_script_include_load(const dh2_script_include_scope* scope,const void* bytes,size_t size) {
  IncludeFrame* frame=include_frame(scope);
  if(!frame||(!bytes&&size))return -1;
  return source_file_load(scope->vm,frame,bytes,size);
}
int dh2_script_vm_load_source_file(dh2_script_vm* vm,const void* bytes,size_t size) {
  IncludeFrame frame;int status;
  if(!vm||vm->busy||(!bytes&&size))return -1;
  memset(&frame,0,sizeof(frame));vm->busy=1;vm->error[0]=0;
  status=source_file_load(vm,&frame,bytes,size);
  snprintf(vm->error,sizeof(vm->error),"%s",frame.error);
  vm->busy=0;return status;
}
const char* dh2_script_include_error(const dh2_script_include_scope* scope) {
  IncludeFrame* frame=include_frame(scope);return frame?frame->error:"invalid Include scope";
}
int dh2_script_include_scope_valid(const dh2_script_include_scope* scope) {
  return include_frame(scope)!=NULL;
}
static int include_trampoline(lua_State* L) {
  IncludeBinding* binding=(IncludeBinding*)lua_touserdata(L,lua_upvalueindex(1));
  IncludeFrame frame;void* previous_provider;const char* name=NULL;char error[256]={0};int count=lua_gettop(L),i,result;
  if(!binding->vm->busy||L!=binding->vm->state)
    return luaL_error(L,"unsupported source Include execution context");
  if(!lua_checkstack(L,1))return luaL_error(L,"source Include projection stack exhausted");
  for(i=1;i<=count;++i) {
    int type=lua_type(L,i);
    if(type==LUA_TTABLE){lua_getfield(L,i,"_this");(void)lua_touserdata(L,-1);lua_pop(L,1);}
    else if(type==LUA_TSTRING){
      const char* text=lua_tostring(L,i);volatile size_t copied_length=strlen(text);(void)copied_length;
      if(i==1)name=text;
    }
  }
  if(!name)return 0;
  if(binding->vm->scope_generation==UINT64_MAX)return luaL_error(L,"source Include generation exhausted");
  memset(&frame,0,sizeof(frame));frame.scope.vm=binding->vm;
  frame.scope.generation=++binding->vm->scope_generation;
  frame.previous=binding->vm->include;binding->vm->include=&frame;
  previous_provider=binding->vm->active_provider;binding->vm->active_provider=&frame;
  result=binding->fn(binding->context,&frame.scope,name,error,sizeof(error));
  if(result==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE)++binding->vm->required_failure_epoch;
  binding->vm->active_provider=previous_provider;
  binding->vm->include=frame.previous;
  if(result){error[sizeof(error)-1]=0;return luaL_error(L,"%s",error[0]?error:"Include provider rejected");}
  return 0;
}
static int bind_include_entry(lua_State* L) {
  Operation* op=(Operation*)lua_touserdata(L,1);IncludeBinding* binding;
  binding=(IncludeBinding*)lua_newuserdata(L,sizeof(*binding));binding->vm=op->vm;
  binding->fn=op->include_callback;binding->context=op->context;
  lua_pushcclosure(L,include_trampoline,1);lua_setglobal(L,"Include");return 0;
}
int dh2_script_vm_bind_source_include(dh2_script_vm* vm,dh2_script_include_provider fn,void* context) {
  Operation op={0};if(!vm||!fn)return -1;op.vm=vm;op.include_callback=fn;op.context=context;
  return protected_operation(vm,bind_include_entry,&op);
}
static CallbackFrame* callback_frame(const dh2_script_callback_scope* scope) {
  dh2_script_vm* vm;
  if(!scope||!(vm=scope->vm)||!vm->busy||!vm->callback||
     vm->active_provider!=vm->callback||scope->generation!=vm->callback->scope.generation)return NULL;
  return vm->callback;
}
int dh2_script_callback_scope_valid(const dh2_script_callback_scope* scope) {
  return callback_frame(scope)!=NULL;
}
const char* dh2_script_callback_error(const dh2_script_callback_scope* scope) {
  CallbackFrame* frame=callback_frame(scope);return frame?frame->error:"invalid source callback scope";
}
static void callback_discard_direct(lua_State* L,void* opaque) {
  Operation* op=(Operation*)opaque;
  if(!lua_checkstack(L,(int)op->count+20))luaL_error(L,"source call stack exhausted");
  discard_source_values(L,op);
}
static void callback_error_direct(lua_State* L,void* opaque) {
  CallbackFrame* frame=(CallbackFrame*)opaque;const char* text=lua_tolstring(L,-1,NULL);
  if(text)snprintf(frame->error,sizeof(frame->error),"%s",text);
  else snprintf(frame->error,sizeof(frame->error),"%s","unsupported source callback error object");
}
uint64_t dh2_script_vm_required_failure_epoch(const dh2_script_vm* vm){return vm?vm->required_failure_epoch:0;}
int dh2_script_vm_call_source_status_objects(dh2_script_vm* vm,const char* name,
 const dh2_script_value* args,uint32_t count){
 Operation op={0};CallbackFrame diagnostic;lua_State* L;uint32_t i;int top,status,error_status;
 uint64_t epoch;
 if(!vm||vm->busy||!name||(!args&&count)||(uint64_t)count+20>2147483647)return -1;
 for(i=0;i<count;++i)if(!valid_value(args+i)&&
   !(args[i].type==DH2_SCRIPT_SOURCE_OBJECT&&!args[i].reserved&&vm->objects))return -1;
 memset(&diagnostic,0,sizeof(diagnostic));op.vm=vm;op.source_values=2;
 op.name=name;op.args=args;op.count=count;L=vm->state;top=lua_gettop(L);
 epoch=vm->required_failure_epoch;vm->busy=1;vm->error[0]=0;
 status=luaD_pcall(L,callback_discard_direct,&op,savestack(L,L->top),0);
 if(status){
  int type=lua_type(L,-1);
  error_status=luaD_pcall(L,callback_error_direct,&diagnostic,savestack(L,L->top),0);
  if(error_status){
   const char* text=lua_type(L,-1)==LUA_TSTRING?lua_tostring(L,-1):NULL;
   snprintf(diagnostic.error,sizeof(diagnostic.error),"%s",text?text:"Lua error (no text)");status=error_status;
  }else if(type!=LUA_TSTRING&&type!=LUA_TNUMBER)status=-4;
 }
 snprintf(vm->error,sizeof(vm->error),"%s",diagnostic.error);
 lua_settop(L,top);vm->busy=0;
 return vm->required_failure_epoch!=epoch?DH2_SCRIPT_REQUIRED_FAILURE_STATUS:status;
}
static int callback_call_discard(const dh2_script_callback_scope* scope,
  const char* name,const dh2_script_value* args,uint32_t count,int source_objects) {
  CallbackFrame* frame=callback_frame(scope);Operation op={0};lua_State* L;uint32_t i;int top,status,error_status;
  if(!frame||!name||(!args&&count)||(uint64_t)count+20>2147483647)return -1;
  for(i=0;i<count;++i)if(!valid_value(args+i)&&
    !(source_objects&&args[i].type==DH2_SCRIPT_SOURCE_OBJECT&&!args[i].reserved))return -1;
  op.vm=scope->vm;op.source_values=source_objects?2:0;
  op.name=name;op.args=args;op.count=count;L=scope->vm->state;top=lua_gettop(L);frame->error[0]=0;
  status=luaD_pcall(L,callback_discard_direct,&op,savestack(L,L->top),0);
  if(status) {
    int type=lua_type(L,-1);
    /* Source Error conversion can allocate for numeric errors. Protect that
     * projection too, without creating a new C callback argument frame. */
    error_status=luaD_pcall(L,callback_error_direct,frame,savestack(L,L->top),0);
    if(error_status) {
      const char* text=lua_type(L,-1)==LUA_TSTRING?lua_tostring(L,-1):NULL;
      snprintf(frame->error,sizeof(frame->error),"%s",text?text:"Lua error (no text)");status=error_status;
    } else if(type!=LUA_TSTRING&&type!=LUA_TNUMBER)status=-4;
  }
  lua_settop(L,top);return status;
}
int dh2_script_callback_call_discard_source(const dh2_script_callback_scope* scope,
  const char* name,const dh2_script_value* args,uint32_t count) {
  return callback_call_discard(scope,name,args,count,0);
}
int dh2_script_callback_call_discard_source_objects(const dh2_script_callback_scope* scope,
  const char* name,const dh2_script_value* args,uint32_t count) {
  return callback_call_discard(scope,name,args,count,1);
}
typedef struct {dh2_script_value* args;int count,first;} ScopedArguments;
static void scoped_project_arguments(lua_State* L,void* opaque) {
  ScopedArguments* projection=(ScopedArguments*)opaque;int i;
  if(!lua_checkstack(L,1))luaL_error(L,"source callback projection stack exhausted");
  for(i=0;i<projection->count;++i) {
    dh2_script_value* value=projection->args+i;
    pull(L,i+projection->first,value);
    if(value->type==LUA_TTABLE) {
      lua_getfield(L,i+projection->first,"_this");value->type=LUA_TUSERDATA;
      value->identity=(uintptr_t)lua_touserdata(L,-1);lua_pop(L,1);
    } else if(value->type==LUA_TSTRING)value->text_bytes=strlen(value->text);
    else if(value->type>=LUA_TFUNCTION)memset(value,0,sizeof(*value));
  }
}
static int scoped_trampoline(lua_State* L) {
  ScopedBinding* b=(ScopedBinding*)lua_touserdata(L,lua_upvalueindex(1));
  CallbackFrame frame;ScopedArguments projection;dh2_script_value* args;void* previous_provider;
  int count=lua_gettop(L),result,status;char error[256]={0};
  if(!b->vm->busy||L!=b->vm->state)return luaL_error(L,"unsupported source callback execution context");
  /* Source Binder creates a native STL Arguments vector. A Lua userdata here
   * would add VM-budget/GC effects before the real callback. Keep the borrowed
   * source records in native storage; the actual Lua argument stack roots all
   * strings while projection and reentrant calls execute. Protect projection
   * without another C frame, and release storage before rethrowing Lua errors. */
  args=count?(dh2_script_value*)malloc((size_t)count*sizeof(*args)):NULL;
  if(count&&!args)return luaL_error(L,"source native argument allocation failed");
  projection.args=args;projection.count=count;projection.first=1;
  status=luaD_pcall(L,scoped_project_arguments,&projection,savestack(L,L->top),0);
  if(status){free(args);return lua_error(L);}
  if(b->vm->scope_generation==UINT64_MAX){free(args);return luaL_error(L,"source callback generation exhausted");}
  memset(&frame,0,sizeof(frame));frame.scope.vm=b->vm;frame.scope.generation=++b->vm->scope_generation;
  frame.previous=b->vm->callback;b->vm->callback=&frame;
  previous_provider=b->vm->active_provider;b->vm->active_provider=&frame;
  result=b->fn(b->context,&frame.scope,args,(uint32_t)count,error,sizeof(error));
  if(result==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE)++b->vm->required_failure_epoch;
  b->vm->active_provider=previous_provider;b->vm->callback=frame.previous;
  free(args);
  if(result){error[sizeof(error)-1]=0;return luaL_error(L,"%s",error[0]?error:"source callback provider rejected");}
  return 0;
}
static int bind_scoped_entry(lua_State* L) {
  Operation* op=(Operation*)lua_touserdata(L,1);ScopedBinding* b;
  b=(ScopedBinding*)lua_newuserdata(L,sizeof(*b));b->vm=op->vm;b->fn=op->scoped_callback;b->context=op->context;
  lua_pushcclosure(L,scoped_trampoline,1);lua_setglobal(L,op->name);return 0;
}
int dh2_script_object_method_call(lua_State* L,dh2_script_vm* vm,const dh2_script_object_services* services,uintptr_t identity,uint32_t address){
  CallbackFrame frame;ScopedArguments projection;dh2_script_value* args;void* previous_provider;
  dh2_script_value out[16];uint32_t returned=0,i;char error[256]={0};
  int count=lua_gettop(L)-1,result,status;
  if(!vm||!vm->busy||L!=vm->state||!services||count<0)return luaL_error(L,"unsupported source object execution context");
  args=count?(dh2_script_value*)malloc((size_t)count*sizeof(*args)):NULL;
  if(count&&!args)return luaL_error(L,"source native argument allocation failed");
  projection.args=args;projection.count=count;projection.first=2;
  status=luaD_pcall(L,scoped_project_arguments,&projection,savestack(L,L->top),0);
  if(status){free(args);return lua_error(L);}
  if(vm->scope_generation==UINT64_MAX){free(args);return luaL_error(L,"source callback generation exhausted");}
  memset(&frame,0,sizeof(frame));memset(out,0,sizeof(out));
  frame.scope.vm=vm;frame.scope.generation=++vm->scope_generation;
  frame.previous=vm->callback;vm->callback=&frame;previous_provider=vm->active_provider;vm->active_provider=&frame;
  result=services->invoke(services->context,&frame.scope,identity,address,args,(uint32_t)count,out,16,&returned,error,sizeof(error));
  if(result==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE)++vm->required_failure_epoch;
  vm->active_provider=previous_provider;vm->callback=frame.previous;free(args);
  if(result){error[sizeof(error)-1]=0;return luaL_error(L,"%s",error[0]?error:"source object method provider rejected");}
  if(returned>16)return luaL_error(L,"source object result limit");
  for(i=0;i<returned;++i)if(!valid_value(out+i)&&!(out[i].type==DH2_SCRIPT_SOURCE_OBJECT&&!out[i].reserved))return luaL_error(L,"invalid source object result");
  for(i=0;i<returned;++i){
    if(out[i].type==DH2_SCRIPT_SOURCE_OBJECT)dh2_script_object_push(L,vm,services,out[i].identity);
    else push(L,out+i);
  }
  return (int)returned;
}
int dh2_script_vm_bind_source_scoped(dh2_script_vm* vm,const char* name,
  dh2_script_scoped_function fn,void* context) {
  Operation op={0};if(!vm||!name||!fn)return -1;op.vm=vm;op.name=name;op.scoped_callback=fn;op.context=context;
  return protected_operation(vm,bind_scoped_entry,&op);
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
