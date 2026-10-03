#ifndef DH2_SCRIPT_RUNTIME_H
#define DH2_SCRIPT_RUNTIME_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct dh2_script_vm dh2_script_vm;
/* Lua 5.1 type numbers. Text results are borrowed until the next VM operation.
 * identity is a native opaque light-userdata value; serialized chunks contain no pointers. */
typedef struct dh2_script_value {
  uint32_t type, reserved;
  float number;
  uint32_t boolean;
  const char* text;
  size_t text_bytes;
  uintptr_t identity;
} dh2_script_value;
enum { DH2_SCRIPT_NIL=0, DH2_SCRIPT_BOOLEAN=1, DH2_SCRIPT_IDENTITY=2,
       DH2_SCRIPT_NUMBER=3, DH2_SCRIPT_STRING=4, DH2_SCRIPT_TABLE=5,
       DH2_SCRIPT_FUNCTION=6 };
/* Synchronous borrowed service. It must not throw or re-enter this VM.
 * Arguments and returned strings remain alive through this invocation.
 * Nonzero return raises a protected Lua error using error_text. */
typedef int (*dh2_script_function)(void* context,
  const dh2_script_value* arguments, uint32_t count,
  dh2_script_value* results, uint32_t capacity, uint32_t* result_count,
  char* error_text, size_t error_capacity);
/* Base/coroutine, table, string, math libraries only. Game globals are absent.
 * Each VM owns its allocator and state. No ARM32 runtime. */
dh2_script_vm* dh2_script_vm_create(size_t memory_limit);
void dh2_script_vm_destroy(dh2_script_vm* vm);
int dh2_script_vm_load(dh2_script_vm* vm,const void* bytes,size_t size,const char* name);
int dh2_script_vm_compile(dh2_script_vm* vm,const void* bytes,size_t size,const char* name,
  void* output,size_t capacity,size_t* written);
int dh2_script_vm_call(dh2_script_vm* vm,const char* function,
  const dh2_script_value* arguments,uint32_t count,
  dh2_script_value* results,uint32_t capacity,uint32_t* result_count);
/* LuaScript::Call's discarded ReturnValues path: request all returns, project
 * each through source Value conversion in order (including table._this lookup),
 * then discard them. Return arity has no fixed wrapper cap; VM budget applies. */
int dh2_script_vm_call_discard_source(dh2_script_vm*,const char* function,
  const dh2_script_value* arguments,uint32_t count);
int dh2_script_vm_bind(dh2_script_vm* vm,const char* name,
  dh2_script_function callback,void* borrowed_context);
/* Exact sfc Value::_setFromStack projection for native game callbacks:
 * table -> type7 identity from table._this (normal Lua field lookup),
 * function/thread/raw full userdata -> nil, strings stop at their first NUL.
 * This preserves source game argument coercion separately from generic bind. */
int dh2_script_vm_bind_source_values(dh2_script_vm* vm,const char* name,
  dh2_script_function callback,void* borrowed_context);
int dh2_script_vm_get_global(dh2_script_vm* vm,const char* name,dh2_script_value* value);
const char* dh2_script_vm_error(const dh2_script_vm* vm);
size_t dh2_script_vm_memory(const dh2_script_vm* vm);
/* 0 success; -1 invalid caller input; -2 protected Lua error; -3 output too small.
 * A Lua error can follow script-side mutations; there is no transactional rollback. */
#ifdef __cplusplus
}
static_assert(sizeof(void*)==8,"The native port requires 64-bit pointers");
static_assert(sizeof(dh2_script_value)==40,"script value ABI");
#endif
#endif
