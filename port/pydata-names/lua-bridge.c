#include "names.h"
#include "lua.h"
#include "lauxlib.h"
#include <string.h>
#include "struct-names.h"
int dh2_lua_names_load(lua_State *,const struct dh2_pynames_view *,const char *,size_t);
static char registry_key;
static void existing(lua_State *state) {
    lua_pushlightuserdata(state,&registry_key);lua_rawget(state,LUA_REGISTRYINDEX);
}
static void argument_string(lua_State *state,int index) {
    size_t length;const char *text=lua_tolstring(state,index,&length);
    const char *end=memchr(text,0,length);if(end)length=(size_t)(end-text);
    lua_pushlstring(state,text,length);
}
static int get_oid(lua_State *state) {
    if(lua_gettop(state)<2 || lua_type(state,1)!=LUA_TSTRING || lua_type(state,2)!=LUA_TSTRING)return 0;
    existing(state);argument_string(state,1);lua_rawget(state,-2);
    if(lua_istable(state,-1)) {
        argument_string(state,2);lua_rawget(state,-2);
        if(lua_isnumber(state,-1))return 1;
    }
    lua_pushinteger(state,-1);return 1;
}
void dh2_lua_register_names(lua_State *state) {
    lua_pushlightuserdata(state,&registry_key);lua_newtable(state);lua_rawset(state,LUA_REGISTRYINDEX);
    lua_pushcfunction(state,get_oid);lua_setglobal(state,"GetPyOID");
    /* Both original callbacks call the same PyDataArrays::GetOID manager. */
    lua_pushcfunction(state,get_oid);lua_setglobal(state,"GetPyStruct");
    for(size_t i=0;i<sizeof(dh2_struct_name_tables)/sizeof(dh2_struct_name_tables[0]);++i) {
        const struct dh2_builtin_name_table *table=dh2_struct_name_tables+i;
        struct dh2_pynames_view view;
        if(dh2_pynames_open(&view,table->bytes,table->size))luaL_error(state,"invalid built-in field names");
        if(dh2_lua_names_load(state,&view,table->name,strlen(table->name)))lua_error(state);
    }
}
static uint32_t word(const unsigned char *raw,uint32_t *offset) {
    const unsigned char *p=raw+*offset;*offset+=4;
    return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
}
struct request { const struct dh2_pynames_view *view;const char *name;size_t length; };
/* Validated immutable source, one owner/thread. Previous class maps are immutable. */
static int import_names(lua_State *state) {
    const struct request *input=lua_touserdata(state,1);
    existing(state);lua_newtable(state); /* 2: old classes, 3: staging classes */
    lua_pushnil(state);
    while(lua_next(state,2)) {
        lua_pushvalue(state,-2);lua_pushvalue(state,-2);lua_rawset(state,3);lua_pop(state,1);
    }
    lua_pushlstring(state,input->name,input->length);lua_newtable(state); /* 4: class name, 5: new members */
    uint32_t offset=4;
    for(uint32_t i=0;i<input->view->count;++i) {
        uint32_t length=word(input->view->bytes,&offset);
        lua_pushlstring(state,(const char *)input->view->bytes+offset,length);offset+=length;
        lua_pushvalue(state,-1);lua_rawget(state,5);
        int missing=lua_isnil(state,-1);lua_pop(state,1);
        if(missing) { lua_pushinteger(state,(lua_Integer)i);lua_rawset(state,5); }
        else lua_pop(state,1); /* Original linear lookup returns the first duplicate. */
    }
    lua_rawset(state,3);
    lua_pushlightuserdata(state,&registry_key);lua_pushvalue(state,3);lua_rawset(state,LUA_REGISTRYINDEX);
    return 0;
}
int dh2_lua_names_load(lua_State *state,const struct dh2_pynames_view *view,const char *name,size_t length) {
    struct request input={view,name,length};return lua_cpcall(state,import_names,&input);
}
