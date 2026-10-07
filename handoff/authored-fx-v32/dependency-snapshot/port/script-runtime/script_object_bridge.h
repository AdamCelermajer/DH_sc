#ifndef DH2_SCRIPT_OBJECT_BRIDGE_H
#define DH2_SCRIPT_OBJECT_BRIDGE_H
#include "script_runtime.h"
#ifdef __cplusplus
extern "C" {
#endif
/* Source Value type7 is a UserData object, pushed as a fresh Lua table. This
 * distinct output is accepted ONLY by bind_source_objects and object methods.
 * Generic bind/source-values/call guards retain their existing contracts. */
enum {DH2_SCRIPT_SOURCE_OBJECT=7};
typedef struct dh2_script_object_method {
 const char* name;
 uint32_t source_address,reserved;
} dh2_script_object_method;
typedef struct dh2_script_object_services {
 void* context;
 /* Synchronous borrowed producers; no generic VM reentry, throw, VM/owner
  * destruction. Strings/method arrays live through producer return. Context
  * and this service record outlive the VM, including finalizers. */
 int (*type_name)(void*,uintptr_t,const char**);
 /* Called only for a newly-created sfc_vftable_TYPE registry entry. Methods
  * are installed in supplied source order; later duplicate names overwrite. */
 int (*methods)(void*,uintptr_t,const dh2_script_object_method**,uint32_t*);
 /* Receiver identity is captured from normal self._this lookup BEFORE all
  * remaining argument projections. Source self is excluded from arguments.
  * Return0 delivers raw source values; nonzero raises a protected diagnostic.
  * Scope grants dedicated discarded same-VM calls during this invocation.
  * Supported callbacks produce <=16 values; no argument-count limit. */
 int (*invoke)(void*,const dh2_script_callback_scope*,uintptr_t,uint32_t,
   const dh2_script_value*,uint32_t,dh2_script_value*,uint32_t,uint32_t*,char*,size_t);
} dh2_script_object_services;
/* One persistent borrowed provider per VM, set before first object binding.
 * Existing registry/metatable ownership is Lua's; object identities remain
 * borrowed raw source identities and require caller actor lifetimes. */
int dh2_script_vm_set_source_objects(dh2_script_vm*,const dh2_script_object_services*);
int dh2_script_vm_bind_source_objects(dh2_script_vm*,const char*,dh2_script_function,void*);
/* Dedicated source AIS argument calls: type7 is pushed through the same
 * original table/metatable producer. Existing generic/source-values calls
 * still reject it. Returns are projected in order and discarded. Outside
 * callbacks the existing 16-argument bridge limit applies; scoped calls use
 * the existing stack-budget domain. Generic same-VM reentry remains blocked.
 * The caller resolves the source alias freshly before each invocation. */
int dh2_script_vm_call_discard_source_objects(dh2_script_vm*,const char*,
 const dh2_script_value*,uint32_t);
int dh2_script_callback_call_discard_source_objects(const dh2_script_callback_scope*,
 const char*,const dh2_script_value*,uint32_t);
#ifdef __cplusplus
}
#endif
#endif
