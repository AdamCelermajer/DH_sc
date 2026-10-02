#include "runtime.h"
#include "../quest-data/quests.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "../pydata-constants/constants.h"
#include "../pydata-names/names.h"
#include "../character-properties/properties.h"
#include "../character-classes/classes.h"
#include "../loot-tables/loot.h"
#include "../gear-properties/gears.h"
#include <stdlib.h>
#include <string.h>

struct dh2_lua { lua_State *state; size_t used,limit; uint32_t blocks; };
void dh2_lua_register_numeric(lua_State *state);
void dh2_lua_register_random(lua_State *state);
void dh2_lua_register_constants(lua_State *state);
int dh2_lua_constants_load(lua_State *state,const struct dh2_pycst_view *view);
void dh2_lua_register_names(lua_State *state);
int dh2_lua_names_load(lua_State *state,const struct dh2_pynames_view *,const char *,size_t);
void dh2_lua_register_characters(lua_State *state);
int dh2_lua_characters_load(lua_State *state,const struct dh2_property_table *);
int dh2_lua_classes_load(lua_State *state,const struct dh2_class_table *);
int dh2_lua_loot_load(lua_State *state,const struct dh2_loot_tables *);
int dh2_lua_powers_load(lua_State *state,const struct dh2_power_tables *);
void dh2_lua_register_kill_objectives(lua_State *state);
void dh2_lua_register_quest_data(lua_State *state);
int dh2_lua_quests_load(lua_State *,const struct dh2_quest_table *);
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
    dh2_lua_register_numeric(state);
    dh2_lua_register_random(state);
    dh2_lua_register_constants(state);
    dh2_lua_register_names(state);
    dh2_lua_register_characters(state);
    dh2_lua_register_kill_objectives(state);
    dh2_lua_register_quest_data(state);
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
    if (!message)message="non-string Lua error";
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
int dh2_lua_import_constants(dh2_lua *runtime,const void *bytes,size_t size,char *error,size_t capacity) {
    if(error && capacity)error[0]='\0';
    if(!runtime || !bytes || size>16*1024*1024) {
        diagnostic(error,capacity,"invalid constant source arguments");return -1;
    }
    struct dh2_pycst_view view;
    if(dh2_pycst_open(&view,bytes,(uint32_t)size)) {
        diagnostic(error,capacity,"malformed or unsupported integer constant file");return -1;
    }
    lua_State *state=runtime->state;lua_settop(state,0);
    int status=dh2_lua_constants_load(state,&view);
    if(status)diagnostic(error,capacity,lua_tostring(state,-1));
    lua_settop(state,0);return status;
}
int dh2_lua_import_names(dh2_lua *runtime,const char *name,size_t length,
                         const void *bytes,size_t size,char *error,size_t capacity) {
    if(error && capacity)error[0]='\0';
    if(!runtime || !name || !length || length>255 || memchr(name,0,length) || !bytes || size>16*1024*1024) {
        diagnostic(error,capacity,"invalid name table arguments");return -1;
    }
    struct dh2_pynames_view view;
    if(dh2_pynames_open(&view,bytes,(uint32_t)size)) {
        diagnostic(error,capacity,"malformed or unsupported name table");return -1;
    }
    lua_State *state=runtime->state;lua_settop(state,0);
    int status=dh2_lua_names_load(state,&view,name,length);
    if(status)diagnostic(error,capacity,lua_tostring(state,-1));
    lua_settop(state,0);return status;
}
int dh2_lua_import_character_properties(dh2_lua *runtime,const void *bytes,size_t size,char *error,size_t capacity) {
    if(error && capacity)error[0]='\0';
    if(!runtime || !bytes || size>4*1024*1024) {
        diagnostic(error,capacity,"invalid character property arguments");return -1;
    }
    struct dh2_property_table view;
    if(dh2_property_open(&view,bytes,(uint32_t)size)) {
        diagnostic(error,capacity,"malformed character property file");return -1;
    }
    lua_State *state=runtime->state;lua_settop(state,0);
    int status=dh2_lua_characters_load(state,&view);
    if(status)diagnostic(error,capacity,lua_tostring(state,-1));
    lua_settop(state,0);return status;
}
int dh2_lua_import_character_classes(dh2_lua *runtime,const void *bytes,size_t size,char *error,size_t capacity) {
    if(error && capacity)error[0]='\0';
    if(!runtime || !bytes || size>4*1024*1024) {
        diagnostic(error,capacity,"invalid character class arguments");return -1;
    }
    struct dh2_class_table view;
    if(dh2_class_open(&view,bytes,(uint32_t)size)) {
        diagnostic(error,capacity,"malformed character class file");return -1;
    }
    lua_State *state=runtime->state;lua_settop(state,0);
    int status=dh2_lua_classes_load(state,&view);
    if(status)diagnostic(error,capacity,lua_tostring(state,-1));
    lua_settop(state,0);return status;
}
int dh2_lua_import_loot_tables(dh2_lua *runtime,const void *bytes,size_t size,char *error,size_t capacity) {
    if(error && capacity)error[0]='\0';
    if(!runtime || !bytes || size>4*1024*1024) {
        diagnostic(error,capacity,"invalid item data arguments");return -1;
    }
    struct dh2_loot_tables view;
    if(dh2_loot_open(&view,bytes,(uint32_t)size)) {
        diagnostic(error,capacity,"malformed item data file");return -1;
    }
    lua_State *state=runtime->state;lua_settop(state,0);int status=dh2_lua_loot_load(state,&view);
    if(status)diagnostic(error,capacity,lua_tostring(state,-1));lua_settop(state,0);return status;
}
int dh2_lua_import_item_powers(dh2_lua *runtime,const void *bytes,size_t size,char *error,size_t capacity) {
    if(error && capacity)error[0]='\0';
    if(!runtime || !bytes || size>4*1024*1024) {
        diagnostic(error,capacity,"invalid item power arguments");return -1;
    }
    struct dh2_power_tables view;
    if(dh2_power_open(&view,bytes,(uint32_t)size)) {
        diagnostic(error,capacity,"malformed item power file");return -1;
    }
    lua_State *state=runtime->state;lua_settop(state,0);
    int status=dh2_lua_powers_load(state,&view);
    if(status)diagnostic(error,capacity,lua_tostring(state,-1));
    lua_settop(state,0);return status;
}
int dh2_lua_import_quests(dh2_lua *runtime,const void *bytes,size_t size,char *error,size_t capacity) {
    if(error && capacity)error[0]='\0';
    if(!runtime || !bytes || size>4*1024*1024) {
        diagnostic(error,capacity,"invalid quest data arguments");return -1;
    }
    struct dh2_quest_table view;
    if(dh2_quests_open(&view,bytes,(uint32_t)size)) {
        diagnostic(error,capacity,"malformed quest data file");return -1;
    }
    lua_State *state=runtime->state;lua_settop(state,0);
    int status=dh2_lua_quests_load(state,&view);
    if(status)diagnostic(error,capacity,lua_tostring(state,-1));
    lua_settop(state,0);return status;
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
