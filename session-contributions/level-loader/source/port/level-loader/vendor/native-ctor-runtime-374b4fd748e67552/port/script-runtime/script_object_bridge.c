#include "script_object_bridge_internal.h"
#include "lauxlib.h"
#include <string.h>
typedef struct {
 dh2_script_vm* vm;
 const dh2_script_object_services* services;
 uint32_t address;
} ObjectMethod;
static int method_callback(lua_State* L){
 ObjectMethod* method=(ObjectMethod*)lua_touserdata(L,lua_upvalueindex(1));
 uintptr_t identity;
 /* Original normal lookup permits user __index and captures _this before
  * argument projection. Lua handles invalid receiver indexing as an error. */
 lua_getfield(L,1,"_this");identity=(uintptr_t)lua_touserdata(L,-1);lua_pop(L,1);
 return dh2_script_object_method_call(L,method->vm,method->services,identity,method->address);
}
void dh2_script_object_push(lua_State* L,dh2_script_vm* vm,const dh2_script_object_services* services,uintptr_t identity){
 const char* type=NULL;const dh2_script_object_method* methods=NULL;uint32_t count=0,i;
 size_t length;char* key;
 if(!identity){lua_pushnil(L);return;}
 if(!services||!services->type_name||!services->methods||!services->invoke)
  luaL_error(L,"missing source object provider");
 lua_createtable(L,0,0);lua_pushstring(L,"_this");lua_pushlightuserdata(L,(void*)identity);lua_settable(L,-3);
 if(services->type_name(services->context,identity,&type)||!type)
  luaL_error(L,"source object type provider failure");
 length=strlen(type);
 /* Original string construction uses the same prefix then appends virtual
  * type name. Lua temporary storage roots bytes across protected allocation. */
 if(length>8388608)luaL_error(L,"source object type exceeds domain");
 key=(char*)lua_newuserdata(L,length+13);memcpy(key,"sfc_vftable_",12);memcpy(key+12,type,length+1);
 if(luaL_newmetatable(L,key)){
  lua_pushstring(L,"__index");lua_createtable(L,0,0);
  if(services->methods(services->context,identity,&methods,&count)||count>65536||(count&&!methods))
   luaL_error(L,"source object method provider failure");
  for(i=0;i<count;++i){
   ObjectMethod* method;
   if(!methods[i].name||!methods[i].source_address||methods[i].reserved)
    luaL_error(L,"malformed source object method");
   lua_pushstring(L,methods[i].name);method=(ObjectMethod*)lua_newuserdata(L,sizeof(*method));
   method->vm=vm;method->services=services;method->address=methods[i].source_address;
   lua_pushcclosure(L,method_callback,1);lua_settable(L,-3);
  }
  lua_settable(L,-3);
 }
 /* Remove temporary key userdata before source setmetatable(-2). */
 lua_remove(L,-2);lua_setmetatable(L,-2);
}
