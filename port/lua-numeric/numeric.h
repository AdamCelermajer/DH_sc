#ifndef DH2_LUA_NUMERIC_H
#define DH2_LUA_NUMERIC_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
enum dh2_lua_operation { DH2_TO_FIXED,DH2_FROM_FIXED,DH2_MUL_FIXED,DH2_DIV_FIXED,
    DH2_BIT_NOT,DH2_BIT_XOR,DH2_BIT_AND,DH2_BIT_OR,DH2_TRACE };
struct dh2_lua_numeric_result { uint32_t count; int32_t integer; float number; };
/* Numeric-only projection; operands are original float32 Value numbers.
 * Max 256 operands. Zero return = success. Errors preserve the entire result.
 * Result must be disjoint from the borrowed inputs. Unused operands are ignored.
 * Rejects nonfinite/out-of-range consumed casts, zero or overflowing division.
 * Does not implement string/bool/userdata Value coercion or the engine ABI. */
uint32_t dh2_lua_numeric(uint32_t operation,const float *operands,uint32_t count,
                          struct dh2_lua_numeric_result *result);
#ifdef __cplusplus
}
#endif
#endif
