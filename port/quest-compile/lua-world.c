#include "compile.h"
#include "lua.h"
#include "lauxlib.h"
#include <math.h>
#define WORLD_TYPE "dh2.source.quest-world"
struct world {int32_t level;uint32_t count,cached;struct dh2_quest_character characters[];};
static int32_t integer(lua_State *L,int index) {
    if(lua_type(L,index)!=LUA_TNUMBER)luaL_error(L,"quest world requires integers");
    lua_Number n=lua_tonumber(L,index);
    if(!isfinite(n) || n< -2147483648.0 || n>=2147483648.0 || (lua_Number)(int32_t)n!=n)luaL_error(L,"invalid quest world integer");
    return (int32_t)n;
}
static int32_t field(lua_State *L,int table,const char *key) {
    lua_pushstring(L,key);lua_rawget(L,table);int32_t value=integer(L,-1);lua_pop(L,1);return value;
}
static struct dh2_quest_cache_entry *cache(struct world *w) {return (void *)(w->characters+w->count);}
static int32_t population(lua_State *L,struct world *w,uint32_t kind,int32_t id) {
    int32_t result;
    if(dh2_quest_population(kind,id,w->characters,w->count,cache(w),w->cached,&result))luaL_error(L,"invalid quest world data");
    return result;
}
void dh2_lua_quest_world_context(lua_State *L,int index,uint32_t kind,int32_t id,int32_t *level,int32_t *count) {
    struct world *w=luaL_checkudata(L,index,WORLD_TYPE);*level=w->level;*count=population(L,w,kind/2,id);
}
static int get_population(lua_State *L) {
    struct world *w=luaL_checkudata(L,1,WORLD_TYPE);
    if(lua_gettop(L)!=3)return luaL_error(L,"quest world population requires kind and match ID");
    int32_t kind=integer(L,2),id=integer(L,3);if(kind<0 || kind>1)return luaL_error(L,"invalid population kind");
    lua_pushinteger(L,population(L,w,(uint32_t)kind,id));return 1;
}
static int get_level(lua_State *L) {
    struct world *w=luaL_checkudata(L,1,WORLD_TYPE);
    if(lua_gettop(L)!=1)return luaL_error(L,"quest world level takes no arguments");
    lua_pushinteger(L,w->level);return 1;
}
static int create(lua_State *L) {
    if(lua_gettop(L)!=3 || lua_type(L,2)!=LUA_TTABLE || lua_type(L,3)!=LUA_TTABLE)return luaL_error(L,"quest world requires level, character list and cache list");
    int32_t level=integer(L,1);size_t count=lua_objlen(L,2),cached=lua_objlen(L,3);
    if(count>4096 || cached>4096)return luaL_error(L,"quest world list too large");
    struct world *w=lua_newuserdata(L,sizeof(*w)+count*sizeof(*w->characters)+cached*sizeof(struct dh2_quest_cache_entry));
    w->level=level;w->count=(uint32_t)count;w->cached=(uint32_t)cached;
    for(uint32_t i=0;i<w->count;++i) {
        lua_rawgeti(L,2,(int)i+1);int table=lua_gettop(L);
        if(lua_type(L,table)!=LUA_TTABLE)return luaL_error(L,"invalid quest world character");
        lua_pushliteral(L,"present");lua_rawget(L,table);
        if(!lua_isnil(L,-1) && lua_type(L,-1)!=LUA_TBOOLEAN)return luaL_error(L,"quest world present must be boolean");
        uint32_t present=lua_isnil(L,-1)?1:(uint32_t)lua_toboolean(L,-1);lua_pop(L,1);
        w->characters[i].present=present;
        w->characters[i].property_id=field(L,table,"property_id");w->characters[i].template_id=field(L,table,"template_id");lua_pop(L,1);
    }
    for(uint32_t i=0;i<w->cached;++i) {
        lua_rawgeti(L,3,(int)i+1);int table=lua_gettop(L);
        if(lua_type(L,table)!=LUA_TTABLE)return luaL_error(L,"invalid quest world cache entry");
        cache(w)[i].property_id=field(L,table,"property_id");cache(w)[i].quantity=field(L,table,"quantity");lua_pop(L,1);
    }
    (void)population(L,w,0,0);luaL_getmetatable(L,WORLD_TYPE);lua_setmetatable(L,-2);return 1;
}
void dh2_lua_register_quest_world(lua_State *L) {
    luaL_newmetatable(L,WORLD_TYPE);lua_pushvalue(L,-1);lua_setfield(L,-2,"__index");
    lua_pushcfunction(L,get_population);lua_setfield(L,-2,"GetPopulation");lua_pushcfunction(L,get_level);lua_setfield(L,-2,"GetLevel");
    lua_pop(L,1);lua_pushcfunction(L,create);lua_setglobal(L,"DH2CreateQuestWorld");
}
