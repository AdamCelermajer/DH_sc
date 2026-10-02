#include "numeric.h"
#include "lua.h"
#include "lauxlib.h"
static int numeric_bridge(lua_State *state) {
    unsigned operation=(unsigned)lua_tointeger(state,lua_upvalueindex(1));
    int count=lua_gettop(state);if(count>256)return luaL_error(state,"too many numeric operands");
    unsigned needed=0;
    switch(operation) {
    case DH2_TO_FIXED:case DH2_FROM_FIXED:needed=count?1:0;break;
    case DH2_MUL_FIXED:case DH2_DIV_FIXED:needed=count>=2?2:0;break;
    case DH2_BIT_NOT:needed=count==1?1:0;break;
    case DH2_BIT_XOR:needed=count==2?2:0;break;
    case DH2_BIT_AND:case DH2_BIT_OR:needed=count>=2?(unsigned)count:0;break;
    case DH2_TRACE:break;
    }
    if(!needed)return 0;
    float operands[256];
    for(unsigned i=0;i<needed;++i) {
        if(lua_type(state,(int)i+1)!=LUA_TNUMBER) {
            if(operation==DH2_BIT_NOT || operation==DH2_BIT_XOR)return 0;
            return luaL_error(state,"numeric-only bridge operand required");
        }
        operands[i]=lua_tonumber(state,(int)i+1);
    }
    /* Only consumed operands are needed by the projection; unused trailing
     * argument slots are filled without coercion and ignored. */
    for(unsigned i=needed;i<(unsigned)count;++i)operands[i]=0;
    struct dh2_lua_numeric_result result;
    uint32_t error=dh2_lua_numeric(operation,operands,(uint32_t)count,&result);
    if(error)return luaL_error(state,"numeric bridge rejected operation (%d)",(int)error);
    if(result.count)lua_pushinteger(state,result.integer);
    if(result.count==2)lua_pushnumber(state,result.number);
    return (int)result.count;
}
void dh2_lua_register_numeric(lua_State *state) {
    const char *names[]={"ToFixed","FromFixed","MulFixed","DivFixed","BitNot",
                         "BitXOr","BitAnd","BitOr","Trace"};
    for(unsigned i=0;i<sizeof(names)/sizeof(names[0]);++i) {
        lua_pushinteger(state,(lua_Integer)i);lua_pushcclosure(state,numeric_bridge,1);
        lua_setglobal(state,names[i]);
    }
}
