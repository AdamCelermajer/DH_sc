#include "methods.h"
#include "lua.h"
#include "lauxlib.h"
#include <math.h>
#include <string.h>
static char data_key;
#define STATE_TYPE "dh2.source.property-state"
struct dataset { struct dh2_property_table table;unsigned char bytes[]; };
struct object { struct dh2_character_props state; };
static struct dataset *data_for(lua_State *L,int object) {
    lua_getfenv(L,object);lua_rawgeti(L,-1,1);
    struct dataset *data=(struct dataset *)lua_touserdata(L,-1);
    if(!data)luaL_error(L,"property dataset unavailable");
    return data;
}
static int method(lua_State *L,uint32_t op) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);
    int count=lua_gettop(L)-1;if(count>32)return luaL_error(L,"property argument limit exceeded");
    float args[32]={0};uint32_t tags[32]={0};
    for(int i=0;i<count;++i) {
        int t=lua_type(L,i+2);
        if(t==LUA_TNUMBER) { tags[i]=3;args[i]=(float)lua_tonumber(L,i+2); }
        else if(t==LUA_TBOOLEAN) { tags[i]=1;args[i]=(float)lua_toboolean(L,i+2); }
        else if(t==LUA_TUSERDATA || t==LUA_TLIGHTUSERDATA)tags[i]=2;
    }
    struct dataset *data=data_for(L,1);struct dh2_property_result result;
    uint32_t status=dh2_character_property_method(&data->table,&obj->state,op,args,tags,(uint32_t)count,&result);
    if(status)return luaL_error(L,"unsupported or invalid property arguments");
    if(result.count) { lua_pushinteger(L,result.value);return 1; }
    return 0;
}
static int get_prop(lua_State *L) { return method(L,0); }
static int set_prop(lua_State *L) { return method(L,1); }
static int create(lua_State *L) {
    if(lua_type(L,1)!=LUA_TNUMBER)return luaL_error(L,"property row must be a number");
    lua_Number row=lua_tonumber(L,1);
    lua_pushlightuserdata(L,&data_key);lua_rawget(L,LUA_REGISTRYINDEX);
    struct dataset *data=(struct dataset *)lua_touserdata(L,-1);
    if(!data)return luaL_error(L,"character property data is not loaded");
    if(!isfinite(row) || row<0 || row>=data->table.counts[0] || (lua_Number)(uint32_t)row!=row)
        return luaL_error(L,"invalid property row");
    int dataset_index=lua_gettop(L);
    struct object *obj=lua_newuserdata(L,sizeof(*obj));
    if(dh2_character_props_init(&data->table,&obj->state) || dh2_property_load(&data->table,(uint32_t)row,&obj->state.base))
        return luaL_error(L,"invalid character property data");
    struct dh2_property_inputs inputs={&obj->state.base,&obj->state.saved,&obj->state.gears,NULL,0};
    for(uint32_t id=0;id<224;++id)if(dh2_property_recalc(&data->table,&inputs,id,&obj->state.final))
        return luaL_error(L,"invalid character property data");
    luaL_getmetatable(L,STATE_TYPE);lua_setmetatable(L,-2);
    /* Each state retains its exact dataset generation after replacement. */
    lua_newtable(L);lua_pushvalue(L,dataset_index);lua_rawseti(L,-2,1);lua_setfenv(L,-2);
    return 1;
}
void dh2_lua_register_characters(lua_State *L) {
    luaL_newmetatable(L,STATE_TYPE);lua_newtable(L);
    lua_pushcfunction(L,get_prop);lua_setfield(L,-2,"GetProp");
    lua_pushcfunction(L,set_prop);lua_setfield(L,-2,"SetProp");
    lua_setfield(L,-2,"__index");lua_pushboolean(L,0);lua_setfield(L,-2,"__metatable");lua_pop(L,1);
    lua_pushcfunction(L,create);lua_setglobal(L,"DH2CreatePropertyState");
}
static int import(lua_State *L) {
    const struct dh2_property_table *input=lua_touserdata(L,1);
    struct dataset *data=lua_newuserdata(L,sizeof(*data)+input->size);
    memcpy(data->bytes,input->bytes,input->size);
    if(dh2_property_open(&data->table,data->bytes,input->size))return luaL_error(L,"invalid property dataset");
    lua_pushlightuserdata(L,&data_key);lua_pushvalue(L,-2);lua_rawset(L,LUA_REGISTRYINDEX);
    return 0;
}
int dh2_lua_characters_load(lua_State *L,const struct dh2_property_table *view) {
    return lua_cpcall(L,import,(void *)view);
}
