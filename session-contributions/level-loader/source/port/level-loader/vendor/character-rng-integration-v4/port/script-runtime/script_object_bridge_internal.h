#ifndef DH2_SCRIPT_OBJECT_BRIDGE_INTERNAL_H
#define DH2_SCRIPT_OBJECT_BRIDGE_INTERNAL_H
#include "script_object_bridge.h"
#include "lua.h"
/* Private cross-file runtime dispatch. No raw state exposed in public API. */
void dh2_script_object_push(lua_State*,dh2_script_vm*,const dh2_script_object_services*,uintptr_t);
int dh2_script_object_method_call(lua_State*,dh2_script_vm*,const dh2_script_object_services*,uintptr_t,uint32_t);
#endif
