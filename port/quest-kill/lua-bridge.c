#include "quest.h"
#include "lua.h"
#include "lauxlib.h"
#include <math.h>
#define QUEST_TYPE "dh2.source.kill-objective"
struct objective {struct dh2_kill_objective state;uint32_t kind;};
static int32_t integer(lua_State *L,int table,const char *key,int optional,int32_t fallback) {
    lua_pushstring(L,key);lua_rawget(L,table);
    if(optional && lua_isnil(L,-1)) {lua_pop(L,1);return fallback;}
    if(lua_type(L,-1)!=LUA_TNUMBER)luaL_error(L,"quest requires integer fields");
    lua_Number n=lua_tonumber(L,-1);
    if(!isfinite(n) || n< -2147483648.0 || n>=2147483648.0 || (lua_Number)(int32_t)n!=n)luaL_error(L,"invalid quest integer");
    lua_pop(L,1);return (int32_t)n;
}
static uint32_t boolean(lua_State *L,int table,const char *key,int optional) {
    lua_pushstring(L,key);lua_rawget(L,table);
    if(optional && lua_isnil(L,-1)) {lua_pop(L,1);return 0;}
    if(lua_type(L,-1)!=LUA_TBOOLEAN)luaL_error(L,"quest requires boolean fields");
    uint32_t result=(uint32_t)lua_toboolean(L,-1);lua_pop(L,1);return result;
}
static void number(lua_State *L,const char *key,int32_t n) {lua_pushinteger(L,n);lua_setfield(L,-2,key);}
static void flag(lua_State *L,const char *key,uint32_t n) {lua_pushboolean(L,n);lua_setfield(L,-2,key);}
static void snapshot(lua_State *L,const struct objective *o) {
    lua_newtable(L);number(L,"kind",(int32_t)o->kind);number(L,"match_id",o->state.match_id);
    number(L,"current",o->state.current);number(L,"required",o->state.required);flag(L,"completed",o->state.completed);
}
static int progress(lua_State *L) {
    struct objective *o=luaL_checkudata(L,1,QUEST_TYPE);
    if(lua_gettop(L)!=1)return luaL_error(L,"quest progress takes no arguments");
    snapshot(L,o);return 1;
}
static int consume(lua_State *L) {
    struct objective *o=luaL_checkudata(L,1,QUEST_TYPE);
    if(lua_gettop(L)!=2 || lua_type(L,2)!=LUA_TTABLE)return luaL_error(L,"quest event table required");
    int32_t kind=integer(L,2,"kind",0,0);if(kind<0 || kind>3)return luaL_error(L,"invalid quest event kind");
    struct dh2_kill_progress_event event={integer(L,2,"match_id",0,0),integer(L,2,"quantity",1,-1),boolean(L,2,"outbound",1),boolean(L,2,"synchronized",1)};
    struct objective next=*o;struct dh2_kill_progress_result r={0,0,0,0};
    if((uint32_t)kind==o->kind && dh2_quest_kill_event(&next.state,&event,&r))return luaL_error(L,"invalid quest event data");
    lua_newtable(L);flag(L,"matched",r.matched);flag(L,"changed",r.changed);
    flag(L,"completion_requested",r.completion_requested);flag(L,"newly_completed",r.newly_completed);
    snapshot(L,&next);lua_setfield(L,-2,"progress");
    lua_newtable(L);number(L,"kind",kind);number(L,"match_id",event.match_id);number(L,"quantity",event.quantity);
    flag(L,"outbound",event.outbound);flag(L,"synchronized",event.synchronized);lua_setfield(L,-2,"event");
    /* State commits after allocating the complete result. Caller input is read
     * through raw access and never mutated. Dispatch by source kind is authored. */
    *o=next;return 1;
}
static int create(lua_State *L) {
    if(lua_gettop(L)!=1 || lua_type(L,1)!=LUA_TTABLE)return luaL_error(L,"quest context table required");
    struct objective value;int32_t kind=integer(L,1,"kind",0,0);
    if(kind<0 || kind>3)return luaL_error(L,"invalid quest objective kind");
    value.kind=(uint32_t)kind;value.state.match_id=integer(L,1,"match_id",0,0);
    value.state.current=integer(L,1,"current",0,0);value.state.required=integer(L,1,"required",0,0);
    value.state.completed=boolean(L,1,"completed",0);
    struct objective *o=lua_newuserdata(L,sizeof(*o));*o=value;luaL_getmetatable(L,QUEST_TYPE);lua_setmetatable(L,-2);return 1;
}
void dh2_lua_register_kill_objectives(lua_State *L) {
    luaL_newmetatable(L,QUEST_TYPE);lua_pushvalue(L,-1);lua_setfield(L,-2,"__index");
    lua_pushcfunction(L,progress);lua_setfield(L,-2,"GetProgress");lua_pushcfunction(L,consume);lua_setfield(L,-2,"ConsumeKillEvent");
    lua_pop(L,1);lua_pushcfunction(L,create);lua_setglobal(L,"DH2CreateKillObjective");
}
