#include "quest.h"
#include "../quest-compile/compile.h"
#include "../persistence/binary.h"
#include "lua.h"
#include "lauxlib.h"
#include <math.h>
#include <string.h>
#define QUEST_TYPE "dh2.source.kill-objective"
struct objective {struct dh2_kill_objective state;uint32_t kind,active,has_record;int32_t record_level,record_required;};
void dh2_lua_quest_world_context(lua_State *,int,uint32_t,int32_t,int32_t *,int32_t *);
void dh2_lua_push_kill_objective(lua_State *L,const struct dh2_kill_objective *state,uint32_t kind) {
    struct objective *o=lua_newuserdata(L,sizeof(*o));o->state=*state;o->kind=kind;
    o->active=1;o->has_record=0;o->record_level=-1;o->record_required=state->required;
    luaL_getmetatable(L,QUEST_TYPE);lua_setmetatable(L,-2);
}
void dh2_lua_push_compiled_kill_objective(lua_State *L,const struct dh2_quest_compiled *s,const struct dh2_quest_compile_context *c) {
    dh2_lua_push_kill_objective(L,&s->progress,c->kind);struct objective *o=lua_touserdata(L,-1);
    o->active=s->active;o->has_record=1;o->record_level=c->record_level;o->record_required=c->record_required;
}
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
    flag(L,"active",o->active);flag(L,"compiled_record",o->has_record);
}
void dh2_lua_push_quest_compile_result(lua_State *L,const struct dh2_quest_compiled *s,uint32_t kind,const struct dh2_quest_compile_result *r) {
    struct objective o={s->progress,kind,s->active,1,-1,0};
    lua_newtable(L);flag(L,"eligible",r->eligible);flag(L,"required_updated",r->required_updated);
    flag(L,"completion_requested",r->completion_requested);flag(L,"newly_completed",r->newly_completed);
    snapshot(L,&o);lua_setfield(L,-2,"progress");
}
static int compile_against(lua_State *L) {
    struct objective *o=luaL_checkudata(L,1,QUEST_TYPE);
    if(lua_gettop(L)!=2 || !o->has_record)return luaL_error(L,"compiled quest record and world required");
    struct dh2_quest_compile_context c={o->kind,o->state.match_id,o->record_level,0,o->record_required,0};
    dh2_lua_quest_world_context(L,2,o->kind,o->state.match_id,&c.current_level,&c.population);
    struct dh2_quest_compiled next={o->state,o->active};struct dh2_quest_compile_result r;
    if(dh2_quest_compile(&next,&c,&r))return luaL_error(L,"invalid quest compile context");
    dh2_lua_push_quest_compile_result(L,&next,o->kind,&r);o->state=next.progress;o->active=next.active;return 1;
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
    dh2_lua_push_kill_objective(L,&value.state,value.kind);return 1;
}
static int export_progress(lua_State *L) {
    struct objective *o=luaL_checkudata(L,1,QUEST_TYPE);
    if(lua_gettop(L)!=1)return luaL_error(L,"progress export takes no arguments");
    unsigned char bytes[DH2_QUEST_SAVE_BYTES];memcpy(bytes,"DHQ1",4);
    const uint32_t fields[]={o->kind,(uint32_t)o->state.match_id,(uint32_t)o->state.current,
        (uint32_t)o->state.required,o->state.completed,o->active,o->has_record,
        (uint32_t)o->record_level,(uint32_t)o->record_required};
    for(unsigned i=0;i<9;++i)dh2_save_write32(bytes+4+i*4,fields[i]);
    lua_pushlstring(L,(const char *)bytes,sizeof(bytes));return 1;
}
static int import_progress(lua_State *L) {
    struct objective *o=luaL_checkudata(L,1,QUEST_TYPE);size_t size=0;
    if(lua_gettop(L)!=2 || lua_type(L,2)!=LUA_TSTRING)return luaL_error(L,"binary progress required");
    const unsigned char *bytes=(const unsigned char *)lua_tolstring(L,2,&size);
    if(size!=DH2_QUEST_SAVE_BYTES || memcmp(bytes,"DHQ1",4))return luaL_error(L,"unsupported quest checkpoint");
    struct objective next=*o;next.state.current=dh2_save_read_i32(bytes+12);
    next.state.completed=dh2_save_read32(bytes+20);next.active=dh2_save_read32(bytes+24);
    if(dh2_save_read32(bytes+4)!=o->kind || dh2_save_read_i32(bytes+8)!=o->state.match_id ||
       dh2_save_read_i32(bytes+16)!=o->state.required || dh2_save_read32(bytes+28)!=o->has_record ||
       dh2_save_read_i32(bytes+32)!=o->record_level || dh2_save_read_i32(bytes+36)!=o->record_required ||
       next.state.current<0 || next.state.current>next.state.required || next.state.completed>1 ||
       next.active!=o->active || next.state.completed!=(uint32_t)(next.state.current>=next.state.required))
        return luaL_error(L,"quest checkpoint differs from encounter definition");
    *o=next;return 0;
}
void dh2_lua_register_kill_objectives(lua_State *L) {
    luaL_newmetatable(L,QUEST_TYPE);lua_pushvalue(L,-1);lua_setfield(L,-2,"__index");
    lua_pushcfunction(L,progress);lua_setfield(L,-2,"GetProgress");lua_pushcfunction(L,consume);lua_setfield(L,-2,"ConsumeKillEvent");
    lua_pushcfunction(L,compile_against);lua_setfield(L,-2,"CompileAgainst");
    lua_pushcfunction(L,export_progress);lua_setfield(L,-2,"ExportEncounterProgress");
    lua_pushcfunction(L,import_progress);lua_setfield(L,-2,"ImportEncounterProgress");
    lua_pop(L,1);lua_pushcfunction(L,create);lua_setglobal(L,"DH2CreateKillObjective");
}
