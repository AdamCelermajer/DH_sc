/* Versioned replacement translation unit. Build this INSTEAD OF the frozen
 * script_runtime.c, not alongside it. Inclusion preserves every existing core
 * byte and API; the additive operation uses its private protected primitives.
 */
#include "script_runtime.c"
#include "script_return_observer_v1.h"

typedef struct {
 Operation call;
 dh2_script_return_observer_v1 observer;
 void* context;
} ReturnOperationV1;

static void first_source_direct_v1(lua_State* L,void* opaque) {
 ReturnOperationV1* q=(ReturnOperationV1*)opaque;Operation* op=&q->call;
 dh2_script_first_return_v1 first;uint32_t i;int start,last,index,result;
 char error[256]={0};memset(&first,0,sizeof(first));
 if(!lua_checkstack(L,(int)op->count+20))luaL_error(L,"source call stack exhausted");
 start=lua_gettop(L)+1;lua_getglobal(L,op->name);
 if(!lua_isfunction(L,-1))luaL_error(L,"missing function: %s",op->name);
 for(i=0;i<op->count;++i) {
  if(op->args[i].type==DH2_SCRIPT_SOURCE_OBJECT)
   dh2_script_object_push(L,op->vm,op->vm->objects,op->args[i].identity);
  else push(L,op->args+i);
 }
 lua_call(L,(int)op->count,LUA_MULTRET);last=lua_gettop(L);
 if(!lua_checkstack(L,1))luaL_error(L,"source return projection stack exhausted");
 first.count=(uint32_t)(last-start+1);
 for(index=start;index<=last;++index) {
  dh2_script_first_return_v1 value;memset(&value,0,sizeof(value));
  value.type=(uint32_t)lua_type(L,index);
  switch(value.type) {
   case LUA_TTABLE:
    lua_getfield(L,index,"_this");value.identity=(uintptr_t)lua_touserdata(L,-1);lua_pop(L,1);value.type=7;break;
   case LUA_TSTRING:
    value.text=lua_tolstring(L,index,NULL);value.text_bytes=strlen(value.text);break;
   case LUA_TBOOLEAN:value.boolean=(uint32_t)lua_toboolean(L,index);value.number=(float)value.boolean;break;
   case LUA_TNUMBER:value.number=lua_tonumber(L,index);break;
   case LUA_TLIGHTUSERDATA:value.identity=(uintptr_t)lua_touserdata(L,index);break;
   default:value.type=0;break;
  }
  if(index==start){value.count=first.count;first=value;}
 }
 result=q->observer(q->context,&first,error,sizeof(error));
 if(result){if(result==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE)++op->vm->required_failure_epoch;error[sizeof(error)-1]=0;luaL_error(L,"%s",error[0]?error:"source return observer rejected");}
}

int dh2_script_vm_call_first_source_v1(dh2_script_vm* vm,const char* name,
 const dh2_script_value* args,uint32_t count,dh2_script_return_observer_v1 observer,void* context) {
 ReturnOperationV1 q;CallbackFrame diagnostic;lua_State* L;uint32_t i;
 int top,status,error_status;uint64_t epoch;
 if(!vm||vm->busy||!name||!observer||(!args&&count)||(uint64_t)count+20>2147483647)return -1;
 for(i=0;i<count;++i)if(!valid_value(args+i)&&
  !(args[i].type==DH2_SCRIPT_SOURCE_OBJECT&&!args[i].reserved&&vm->objects))return -1;
 memset(&q,0,sizeof(q));memset(&diagnostic,0,sizeof(diagnostic));
 q.call.vm=vm;q.call.name=name;q.call.args=args;q.call.count=count;
 q.observer=observer;q.context=context;L=vm->state;top=lua_gettop(L);
 epoch=vm->required_failure_epoch;vm->busy=1;vm->error[0]=0;
 status=luaD_pcall(L,first_source_direct_v1,&q,savestack(L,L->top),0);
 if(status){int type=lua_type(L,-1);error_status=luaD_pcall(L,callback_error_direct,&diagnostic,savestack(L,L->top),0);
  if(error_status){const char* text=lua_type(L,-1)==LUA_TSTRING?lua_tostring(L,-1):NULL;snprintf(diagnostic.error,sizeof(diagnostic.error),"%s",text?text:"Lua error (no text)");status=error_status;}
  else if(type!=LUA_TSTRING&&type!=LUA_TNUMBER)status=-4;
 }
 snprintf(vm->error,sizeof(vm->error),"%s",diagnostic.error);lua_settop(L,top);vm->busy=0;
 return vm->required_failure_epoch!=epoch?DH2_SCRIPT_REQUIRED_FAILURE_STATUS:status;
}
