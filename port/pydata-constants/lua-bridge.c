#include "constants.h"
#include "lua.h"
#include "lauxlib.h"
#include <string.h>
static const char registry_key=0;
static void existing(lua_State *state) {
    lua_pushlightuserdata(state,(void *)&registry_key);lua_rawget(state,LUA_REGISTRYINDEX);
}
static int get_constant(lua_State *state) {
    if(lua_gettop(state)<2 || lua_type(state,1)!=LUA_TSTRING || lua_type(state,2)!=LUA_TSTRING)return 0;
    existing(state);
    size_t length;const char *text=lua_tolstring(state,1,&length);
    const char *end=memchr(text,0,length);if(end)length=(size_t)(end-text);
    lua_pushlstring(state,text,length);lua_rawget(state,-2);
    if(lua_istable(state,-1)) {
        text=lua_tolstring(state,2,&length);end=memchr(text,0,length);if(end)length=(size_t)(end-text);
        lua_pushlstring(state,text,length);lua_rawget(state,-2);
        if(lua_isnumber(state,-1))return 1;
    }
    lua_pushinteger(state,0);return 1;
}
void dh2_lua_register_constants(lua_State *state) {
    lua_pushlightuserdata(state,(void *)&registry_key);lua_newtable(state);lua_rawset(state,LUA_REGISTRYINDEX);
    lua_pushcfunction(state,get_constant);lua_setglobal(state,"GetPyCst");
}
static uint32_t word(const unsigned char *raw,uint32_t *offset) {
    const unsigned char *p=raw+*offset;*offset+=4;
    return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
}
/* Called only after full source validation. The immutable buffer is borrowed
 * for this non-reentrant call. Lua owns every installed name/value. */
static int import_constants(lua_State *state) {
    const struct dh2_pycst_view *view=lua_touserdata(state,1);
    existing(state); /* 2: old table */
    lua_newtable(state); /* 3: staging table */
    lua_pushnil(state);
    while(lua_next(state,2)) { /* 4: group key, 5: old group */
        lua_newtable(state); /* 6: cloned group */
        lua_pushnil(state);
        while(lua_next(state,5)) { /* 7: key, 8: value */
            lua_pushvalue(state,7);lua_pushvalue(state,8);lua_rawset(state,6);lua_pop(state,1);
        }
        lua_pushvalue(state,4);lua_insert(state,-2);lua_rawset(state,3);lua_pop(state,1);
    }
    uint32_t offset=4;
    for(uint32_t g=0;g<view->groups;++g) {
        uint32_t length=word(view->bytes,&offset);
        lua_pushlstring(state,(const char *)view->bytes+offset,length);offset+=length;
        lua_pushvalue(state,-1);lua_rawget(state,3);
        if(!lua_istable(state,-1)) { lua_pop(state,1);lua_newtable(state); }
        /* 4: group name, 5: staging group */
        uint32_t count=word(view->bytes,&offset);
        for(uint32_t k=0;k<count;++k) {
            length=word(view->bytes,&offset);
            lua_pushlstring(state,(const char *)view->bytes+offset,length);offset+=length;
            uint32_t bits=word(view->bytes,&offset);int32_t value;
            memcpy(&value,&bits,4);lua_pushinteger(state,value);lua_rawset(state,5);
        }
        lua_rawset(state,3);
    }
    /* Commit only after every allocation succeeds. Old mappings survive OOM. */
    lua_pushlightuserdata(state,(void *)&registry_key);lua_pushvalue(state,3);lua_rawset(state,LUA_REGISTRYINDEX);
    return 0;
}
int dh2_lua_constants_load(lua_State *state,const struct dh2_pycst_view *view) {
    return lua_cpcall(state,import_constants,(void *)view);
}
