#include "runtime.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include <stdlib.h>
#include <string.h>

struct dh2_lua { lua_State *state; size_t used,limit; uint32_t blocks; };
static void *allocate(void *opaque, void *pointer, size_t old_size, size_t new_size) {
    dh2_lua *runtime=(dh2_lua *)opaque;
    if (!pointer) old_size=0;
    if (old_size>runtime->used) abort(); /* Lua allocator contract violation. */
    if (!new_size) { free(pointer); runtime->used-=old_size; return NULL; }
    if (new_size>runtime->limit-(runtime->used-old_size)) return NULL;
    void *result=realloc(pointer,new_size);
    if (result) runtime->used=runtime->used-old_size+new_size;
    return result;
}
static int libraries(lua_State *state) {
    const luaL_Reg selected[]={{"",luaopen_base},{LUA_MATHLIBNAME,luaopen_math},
        {LUA_TABLIBNAME,luaopen_table},{LUA_STRLIBNAME,luaopen_string},{NULL,NULL}};
    for (const luaL_Reg *entry=selected;entry->func;++entry) {
        lua_pushcfunction(state,entry->func);lua_pushstring(state,entry->name);
        lua_call(state,1,0);
    }
    const char *removed[]={"dofile","loadfile","print",NULL};
    for (const char **name=removed;*name;++name) { lua_pushnil(state);lua_setglobal(state,*name); }
    return 0;
}
dh2_lua *dh2_lua_create(size_t memory_limit) {
    if (memory_limit<256*1024 || memory_limit>64*1024*1024) return NULL;
    dh2_lua *runtime=(dh2_lua *)calloc(1,sizeof(*runtime));if (!runtime)return NULL;
    runtime->limit=memory_limit;runtime->state=lua_newstate(allocate,runtime);
    if (!runtime->state) { free(runtime);return NULL; }
    if (lua_cpcall(runtime->state,libraries,NULL)) { dh2_lua_destroy(runtime);return NULL; }
    lua_settop(runtime->state,0);return runtime;
}
void dh2_lua_destroy(dh2_lua *runtime) {
    if (!runtime)return;
    if (runtime->state)lua_close(runtime->state);
    if (runtime->used)abort();
    free(runtime);
}
static void diagnostic(char *out,size_t capacity,const char *message) {
    if (!out || !capacity)return;
    size_t length=strlen(message);if (length>=capacity)length=capacity-1;
    memcpy(out,message,length);out[length]='\0';
}
static int load(dh2_lua *runtime,const void *source,size_t bytes,char *error,size_t capacity) {
    if (error && capacity)error[0]='\0';
    if (!runtime || (!source && bytes) || bytes>1024*1024) {
        diagnostic(error,capacity,"invalid source arguments");return -1;
    }
    if (bytes && ((const unsigned char *)source)[0]==0x1b) {
        diagnostic(error,capacity,"bytecode input is unsupported");return -1;
    }
    lua_State *state=runtime->state;lua_settop(state,0);
    int status=luaL_loadbuffer(state,source?(const char *)source:"",bytes,"@dh2-source");
    if (status) { diagnostic(error,capacity,lua_tostring(state,-1));lua_settop(state,0); }
    return status;
}
int dh2_lua_compile(dh2_lua *runtime,const void *source,size_t bytes,char *error,size_t capacity) {
    int status=load(runtime,source,bytes,error,capacity);
    if (runtime)lua_settop(runtime->state,0);
    return status;
}
static void instruction_limit(lua_State *state,lua_Debug *debug) {
    (void)debug;void *opaque=NULL;lua_getallocf(state,&opaque);dh2_lua *runtime=(dh2_lua *)opaque;
    if (!runtime->blocks || !--runtime->blocks)luaL_error(state,"instruction budget exhausted");
}
int dh2_lua_execute(dh2_lua *runtime,const void *source,size_t bytes,uint32_t blocks,char *error,size_t capacity) {
    if (!blocks) { diagnostic(error,capacity,"invalid instruction budget");return -1; }
    int status=load(runtime,source,bytes,error,capacity);if (status)return status;
    lua_State *state=runtime->state;runtime->blocks=blocks;
    lua_sethook(state,instruction_limit,LUA_MASKCOUNT,1000);status=lua_pcall(state,0,0,0);
    lua_sethook(state,NULL,0,0);runtime->blocks=0;
    if (status)diagnostic(error,capacity,lua_tostring(state,-1));
    lua_settop(state,0);return status;
}
size_t dh2_lua_memory_used(const dh2_lua *runtime) { return runtime?runtime->used:0; }
