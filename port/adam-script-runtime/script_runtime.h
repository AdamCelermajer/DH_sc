#ifndef DH2_SCRIPT_RUNTIME_H
#define DH2_SCRIPT_RUNTIME_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct dh2_script_vm dh2_script_vm;
typedef enum dh2_script_library {
  DH2_SCRIPT_LIBRARY_BASE=0, DH2_SCRIPT_LIBRARY_MATH=1,
  DH2_SCRIPT_LIBRARY_TABLE=2, DH2_SCRIPT_LIBRARY_STRING=3
} dh2_script_library;
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
/* Explicit missing required native implementation. Other nonzero callback
 * results remain ordinary protected Lua errors. Legacy API statuses are
 * unchanged; epoch also records a marker caught by Lua pcall. */
enum { DH2_SCRIPT_REQUIRED_SERVICE_FAILURE=-1001, DH2_SCRIPT_REQUIRED_FAILURE_STATUS=-5 };
uint64_t dh2_script_vm_required_failure_epoch(const dh2_script_vm*);
/* Base/coroutine, table, string, math libraries only. Game globals are absent.
 * Each VM owns its allocator and state. No ARM32 runtime. */
dh2_script_vm* dh2_script_vm_create(size_t memory_limit);
/* Source AIS path: create the same bounded Lua state without implicitly
 * opening libraries. The owning LuaScript BindFunction path opens them in its
 * recovered base/math/table/string order via dh2_script_vm_open_library. */
dh2_script_vm* dh2_script_vm_create_deferred(size_t memory_limit);
int dh2_script_vm_open_library(dh2_script_vm* vm, dh2_script_library library);
void dh2_script_vm_destroy(dh2_script_vm* vm);
int dh2_script_vm_load(dh2_script_vm* vm,const void* bytes,size_t size,const char* name);
/* Source loadFile byte-stream protocol on this SAME state: empty input allowed,
 * 1024-byte reader and loadFile() chunk name. 0 success, positive Lua status,
 * -1 malformed/busy/beyond8MiB bound or input alias of VM storage,
 * -4 unsupported nonstring/nonnumber error.
 * Source status is retained even for required failure; query epoch separately.
 * Numeric errors convert under protection; first-NUL diagnostic is vm_error.
 * Completed script mutations survive errors; caller retains bytes through return. */
int dh2_script_vm_load_source_file(dh2_script_vm*,const void*,size_t);
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
/* Source Value tag: nil0,bool1,lightuserdata2,number3,string4,table7. All returns
 * project in order (table._this uses normal lookup) before one selected result
 * is observed. Pointer identities are native-width, numbers remain float32.
 * Absent index reports nil with the actual count. First-NUL string is borrowed
 * ONLY during the synchronous observer while the full return stack is rooted.
 * Observer/context/inputs stay live and cannot throw/destroy/reenter/rebind VM.
 * There is one Lua call, no fixed return/legacy16-argument cap; Lua stack/memory
 * bounds apply. Source-object input/Include/scoped provider support is separate
 * and currently unsupported tags are rejected, never converted to numbers. */
typedef struct dh2_script_first_return_v1 {
  uint32_t count,type;
  float number;
  uint32_t boolean;
  uintptr_t identity;
  const char* text;
  size_t text_bytes;
} dh2_script_first_return_v1;
typedef int (*dh2_script_return_observer_v1)(void*,const dh2_script_first_return_v1*,char*,size_t);
/* One synchronous observation of ALL projected ReturnValues, in original
 * order. Values/text are borrowed only during this observer; each count field
 * holds the complete arity. Zero returns passes NULL/count0. All projection
 * finishes before observation, so a later table._this error observes nothing.
 * No replay, extra VM, fixed return cap, or generic Lua truthiness conversion.
 * Memory is charged to this VM and the full Lua result stack remains rooted. */
typedef int (*dh2_script_returns_observer_v1)(void*,const dh2_script_first_return_v1*,uint32_t,char*,size_t);
int dh2_script_vm_call_all_source_v1(dh2_script_vm*,const char*,
  const dh2_script_value*,uint32_t,dh2_script_returns_observer_v1,void*);
/* 0 success; positive Lua status; -1 invalid/busy/input alias of VM storage;
 * -4 unsupported error object;
 * -5 required native service failure, including one caught by Lua pcall.
 * Observer runs only after the call and ALL return projection succeed. */
int dh2_script_vm_call_indexed_source_v3(dh2_script_vm*,const char*,
  const dh2_script_value*,uint32_t,uint32_t index,dh2_script_return_observer_v1,void*);
int dh2_script_vm_call_first_source_v1(dh2_script_vm*,const char*,
  const dh2_script_value*,uint32_t,dh2_script_return_observer_v1,void*);
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
static_assert(sizeof(dh2_script_first_return_v1)==40,"source return observer ABI");
#endif
#endif
