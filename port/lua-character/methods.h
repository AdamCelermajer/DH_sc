#ifndef DH2_LUA_CHARACTER_METHODS_H
#define DH2_LUA_CHARACTER_METHODS_H
#include "../character-state/state.h"
#ifdef __cplusplus
extern "C" {
#endif
struct dh2_property_result { uint32_t count;int32_t value; };
/* Numeric/boolean projection of Character GetProp (operation 0) and SetProp (1).
 * Tags mirror observed original values: 0 nil, 1 bool, 2 pointer, 3 number.
 * Booleans use float 0/1 in this portable input. Pointer overloads are rejected.
 * <=32 arguments; finite/in-range numeric conversions only. Property IDs use
 * unsigned conversion with negative clamp to zero; writes truncate signed.
 * Invalid argument
 * types/missing required values return no results; unsafe casts return status 2.
 * Original ReturnValues allocation/coercion and Lua object binding are absent.
 * 0 success, 1 bad arguments/alias, 2 unsupported or unsafe. Preserve on error. */
uint32_t dh2_character_property_method(const struct dh2_property_table *,struct dh2_character_props *,uint32_t,const float *,const uint32_t *,uint32_t,struct dh2_property_result *);
#ifdef __cplusplus
}
#endif
#endif
