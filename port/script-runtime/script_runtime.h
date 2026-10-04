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
/* Explicit missing native implementation marker. Other nonzero callback
 * results remain ordinary protected Lua diagnostics, including source argument
 * errors. Legacy call APIs preserve their status conventions. */
enum { DH2_SCRIPT_REQUIRED_SERVICE_FAILURE=-1001, DH2_SCRIPT_REQUIRED_FAILURE_STATUS=-5 };
uint64_t dh2_script_vm_required_failure_epoch(const dh2_script_vm*);
/* Base/coroutine, table, string, math libraries only. Game globals are absent.
 * Each VM owns its allocator and state. No ARM32 runtime. */
dh2_script_vm* dh2_script_vm_create(size_t memory_limit);
/* Source Instance constructor: owned empty VM, no eagerly opened libraries.
 * Existing create() retains its convenience policy and zero initial stack. */
dh2_script_vm* dh2_script_vm_create_empty(size_t memory_limit);
enum { DH2_SCRIPT_LIBRARY_BASE=0, DH2_SCRIPT_LIBRARY_MATH=1,
       DH2_SCRIPT_LIBRARY_TABLE=2, DH2_SCRIPT_LIBRARY_STRING=3 };
/* Direct source library calls retain their stack results: Base adds two,
 * Math/Table/String add one each. Repeated opens are permitted. Allocation
 * errors are protected; earlier global mutations are not rolled back. */
int dh2_script_vm_open_source_library(dh2_script_vm*,uint32_t library);
int dh2_script_vm_open_source_libraries(dh2_script_vm*);
int dh2_script_vm_stack_size(const dh2_script_vm*); /* -1 invalid/busy VM */
void dh2_script_vm_destroy(dh2_script_vm* vm);
int dh2_script_vm_load(dh2_script_vm* vm,const void* bytes,size_t size,const char* name);
/* Source root Instance.loadFile operation. Requires a nonbusy VM and retains
 * its initial stack (including source library-open results). Shares Include's
 * 1024 reader/loadFile()/pcall0/one-error-pop primitive, enables only properly
 * bound nested Include, and leaves generic vm_load policy unchanged. Returns
 * 0 success, positive source Lua status, -1 malformed/busy, -4 unsupported
 * source error object. Diagnostic is vm_error(), not a filename substitution. */
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
/* Native source delivery: all returns are projected then discarded. 0 success,
 * positive Lua status, -1 malformed/busy, -4 unsupported error object, -5 explicit
 * required provider failure even when Lua pcall caught it. No fixed arity cap;
 * source objects use this VM's retained object provider. */
int dh2_script_vm_call_source_status_objects(dh2_script_vm*,const char*,
  const dh2_script_value*,uint32_t count);
int dh2_script_vm_bind(dh2_script_vm* vm,const char* name,
  dh2_script_function callback,void* borrowed_context);
/* Exact sfc Value::_setFromStack projection for native game callbacks:
 * table -> type7 identity from table._this (normal Lua field lookup),
 * function/thread/raw full userdata -> nil, strings stop at their first NUL.
 * This preserves source game argument coercion separately from generic bind. */
int dh2_script_vm_bind_source_values(dh2_script_vm* vm,const char* name,
  dh2_script_function callback,void* borrowed_context);
/* Additive original UserData table output/method bridge is declared in
 * script_object_bridge.h. Existing value/call APIs still reject object output. */
/* Dedicated LuaScript::_Include capability. The provider must not throw,
 * destroy this VM or use generic VM operations. It may synchronously call
 * include_load for this same scope and genuine owner/cache services. Return0
 * means delivery (source missing/load-false is ignored); nonzero raises a
 * protected native-provider diagnostic using error_text. Context outlives VM.
 * Arguments are fully source-projected before the first-string guard; no fixed
 * arity cap. A copied scope expires at provider return; checking it afterwards
 * is permitted only while its VM remains alive. Coroutine/VM-close Include is
 * explicitly unsupported. This grants no general VM reentry. */
typedef struct dh2_script_include_scope {
  dh2_script_vm* vm;
  uint64_t generation;
} dh2_script_include_scope;
typedef int (*dh2_script_include_provider)(void* context,
  const dh2_script_include_scope*,const char* requested_name,
  char* error_text,size_t error_capacity);
int dh2_script_vm_bind_source_include(dh2_script_vm*,
  dh2_script_include_provider,void* borrowed_context);
/* Source Instance.loadFile: 1024-byte reader, chunk name loadFile(),
 * lua_load then pcall(0,0,0) on success; one error object popped on failure.
 * 0 success, positive Lua status (source AddFile false), -1 malformed/expired
 * capability, -4 unsupported original non-string/non-number error object.
 * Empty input is valid; genuine VM allocator budget remains active. */
int dh2_script_include_load(const dh2_script_include_scope*,const void*,size_t);
/* Side-effect-free current-frame validation before owner/cache operations. */
int dh2_script_include_scope_valid(const dh2_script_include_scope*);
const char* dh2_script_include_error(const dh2_script_include_scope*);
/* Dedicated source native callback with zero Lua returns. Arguments are fully
 * source-projected without the generic bridge's fixed arity limit. Provider
 * may only use the scoped discarded source call below while this VM is busy;
 * it must not throw, destroy VM/receiver, or use generic VM operations.
 * Borrowed context outlives callbacks, including VM-close finalizers. */
typedef struct dh2_script_callback_scope {
  dh2_script_vm* vm;
  uint64_t generation;
} dh2_script_callback_scope;
typedef int (*dh2_script_scoped_function)(void*,const dh2_script_callback_scope*,
  const dh2_script_value*,uint32_t,char*,size_t);
int dh2_script_vm_bind_source_scoped(dh2_script_vm*,const char* name,
  dh2_script_scoped_function,void* borrowed_context);
int dh2_script_callback_scope_valid(const dh2_script_callback_scope*);
/* Caller resolves alias freshly before each call; VM looks up global freshly.
 * Executes every returned Value projection (including table._this __index),
 * then discards all results. No fixed count cap; Lua stack/VM budget applies.
 * Stack is restored, earlier Lua effects persist, and generic busy gates stay
 * enabled. Returns0, positive Lua error status, -1 malformed/expired/shadowed,
 * or -4 unsupported source nonstring/nonnumeric error object. Nested scoped
 * callbacks/Include shadow the outer capability and restore it afterwards. */
int dh2_script_callback_call_discard_source(const dh2_script_callback_scope*,
  const char* resolved_name,const dh2_script_value*,uint32_t count);
const char* dh2_script_callback_error(const dh2_script_callback_scope*);
int dh2_script_vm_get_global(dh2_script_vm* vm,const char* name,dh2_script_value* value);
const char* dh2_script_vm_error(const dh2_script_vm* vm);
size_t dh2_script_vm_memory(const dh2_script_vm* vm);
/* 0 success; -1 invalid caller input; -2 protected Lua error; -3 output too small.
 * A Lua error can follow script-side mutations; there is no transactional rollback. */
#ifdef __cplusplus
}
static_assert(sizeof(void*)==8,"The native port requires 64-bit pointers");
static_assert(sizeof(dh2_script_value)==40,"script value ABI");
static_assert(sizeof(dh2_script_include_scope)==16,"Include capability ABI");
static_assert(sizeof(dh2_script_callback_scope)==16,"source callback capability ABI");
#endif
#endif
