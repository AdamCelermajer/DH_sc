#ifndef DH2_LUA_CONFIG_H
#define DH2_LUA_CONFIG_H
/* Included via Lua 5.1's supported LUA_USER_H hook after luaconf.h and before
 * lua_Number/lua_Integer typedefs. Keep exact upstream files unchanged.
 * Original lua_pushnumber stores one 32-bit value plus its 32-bit type tag;
 * original lua_tointeger converts float to signed 32-bit integer.
 * Other configuration choices retain upstream defaults, not proven engine parity. */
#include <stdint.h>
#undef LUA_NUMBER_DOUBLE
#undef LUA_NUMBER
#define LUA_NUMBER float
#undef LUA_INTEGER
#define LUA_INTEGER int32_t
#undef LUA_NUMBER_SCAN
#define LUA_NUMBER_SCAN "%f"
/* Default strtod then narrowing matches the observed original parser flow.
 * Default modulo rounds the division as float, then computes floor/product
 * and subtraction as double; power also uses double before float narrowing. */
#endif
