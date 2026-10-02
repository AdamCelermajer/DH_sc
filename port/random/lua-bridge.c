#include "random.h"
#include "lua.h"
#include "lauxlib.h"
#include <math.h>
#include <string.h>
static int draw(lua_State *L) {
    struct dh2_random_state *state=lua_touserdata(L,lua_upvalueindex(1));
    int count=lua_gettop(L);if(count>32)return luaL_error(L,"random argument limit exceeded");
    float args[32]={0};uint32_t tags[32]={0};
    for(int i=0;i<count;++i)if(lua_type(L,i+1)==LUA_TNUMBER) {tags[i]=3;args[i]=(float)lua_tonumber(L,i+1);}
    uint32_t seed=0;struct dh2_random_result result;
    if(dh2_random_callback(state,&seed,0,args,tags,(uint32_t)count,&result))return luaL_error(L,"unsupported random numeric argument");
    if(result.count)lua_pushinteger(L,result.value);
    return (int)result.count;
}
static int seed(lua_State *L) {
    struct dh2_random_state *state=lua_touserdata(L,lua_upvalueindex(1));int count=lua_gettop(L);
    if(count<1 || count>2)return luaL_error(L,"seed requires one or two unsigned integers");
    uint32_t seeds[2]={0,0};
    for(int i=0;i<count;++i) {
        if(lua_type(L,i+1)!=LUA_TNUMBER)return luaL_error(L,"invalid random seed");
        lua_Number v=lua_tonumber(L,i+1);
        if(!isfinite(v) || v<0 || (double)v>=4294967296.0 || (lua_Number)(uint32_t)v!=v)return luaL_error(L,"invalid random seed");
        seeds[i]=(uint32_t)v;
    }
    memcpy(state->seeds,seeds,sizeof(seeds));memset(state->counters,0,sizeof(state->counters));return 0;
}
void dh2_lua_register_random(lua_State *L) {
    struct dh2_random_state *state=lua_newuserdata(L,sizeof(*state));memset(state,0,sizeof(*state));
    lua_pushvalue(L,-1);lua_pushcclosure(L,draw,1);lua_setglobal(L,"Rand");
    lua_pushcclosure(L,seed,1);lua_setglobal(L,"DH2SeedRandom");
}
