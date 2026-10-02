#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "lua.h"
#include "lauxlib.h"
#include "numeric-vectors.h"
int dh2_lua_numeric_tests(void) {
    if (sizeof(lua_Number)!=4 || sizeof(lua_Integer)!=4)return 2;
    const char *operations[]={"local a,b=...;return a+b","local a,b=...;return a-b",
        "local a,b=...;return a*b","local a,b=...;return a/b","local a,b=...;return a%b",
        "local a,b=...;return a^b","local a,b=...;return -a"};
    lua_State *state=luaL_newstate();if (!state)return 2;
    size_t count=sizeof(numeric_vectors)/sizeof(numeric_vectors[0]);
    for (size_t i=0;i<count;++i) {
        unsigned op=numeric_vectors[i].op;if(op<5 || op>11)return 2;
        const char *source=operations[op-5];
        if(luaL_loadbuffer(state,source,strlen(source),"@numeric-probe"))return 2;
        float a,b;memcpy(&a,&numeric_vectors[i].a,4);memcpy(&b,&numeric_vectors[i].b,4);
        lua_pushnumber(state,a);lua_pushnumber(state,b);
        if(lua_pcall(state,2,1,0) || lua_type(state,-1)!=LUA_TNUMBER)return 2;
        float value=lua_tonumber(state,-1);uint32_t bits;memcpy(&bits,&value,4);
        uint32_t expected=numeric_vectors[i].result;
        if(bits!=expected && !((bits&0x7fffffffu)==0 && (expected&0x7fffffffu)==0)) {
            fprintf(stderr,"numeric mismatch %zu: %08x != %08x\n",i,bits,expected);return 2;
        }
        lua_settop(state,0);
    }
    lua_close(state);printf("NUMERIC PASS %zu\n",count);return 0;
}
