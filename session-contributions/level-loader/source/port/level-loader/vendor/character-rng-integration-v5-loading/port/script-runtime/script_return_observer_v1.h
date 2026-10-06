#ifndef DH2_SCRIPT_RETURN_OBSERVER_V1_H
#define DH2_SCRIPT_RETURN_OBSERVER_V1_H
#include "script_runtime.h"
#ifdef __cplusplus
extern "C" {
#endif
/* Exact source Value tag: nil0,bool1,lightuserdata2,number3,string4,table7.
 * Every return is projected in source order before this observer. Strings stop
 * at first NUL; tables synchronously query _this. Count has no fixed arity cap.
 * First text is borrowed only during observer. All other fields are snapshots.
 * Observer cannot throw/destroy/rebind VM or call generic VM APIs. */
typedef struct dh2_script_first_return_v1 {
 uint32_t count,type;
 float number;
 uint32_t boolean;
 uintptr_t identity;
 const char* text;
 size_t text_bytes;
} dh2_script_first_return_v1;
typedef int(*dh2_script_return_observer_v1)(void*,const dh2_script_first_return_v1*,char*,size_t);
/* One actual LuaScript::Call protocol, with the same nonbusy, stack, positive
 * Lua status, unsupported error -4 and caught required-failure -5 behavior as
 * frozen call_source_status_objects. Observer is called only on success. */
int dh2_script_vm_call_first_source_v1(dh2_script_vm*,const char*,
 const dh2_script_value*,uint32_t,dh2_script_return_observer_v1,void*);
#ifdef __cplusplus
}
static_assert(sizeof(dh2_script_first_return_v1)==40,"first source return ABI");
#endif
#endif
