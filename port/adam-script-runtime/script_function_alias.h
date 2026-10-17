#ifndef DH2_SCRIPT_FUNCTION_ALIAS_H
#define DH2_SCRIPT_FUNCTION_ALIAS_H
#include "script_runtime.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct dh2_script_aliases dh2_script_aliases;
/* Source VFTable keys are unsigned hash values, not string-name pairs.
 * The map owns copied values. A miss returns the exact requested pointer;
 * a hit is borrowed until that entry is modified or erased. No recursive alias. */
dh2_script_aliases* dh2_script_alias_create(void);
void dh2_script_alias_destroy(dh2_script_aliases*);
uint32_t dh2_script_alias_hash(const char* name);
const char* dh2_script_alias_resolve(const dh2_script_aliases*,const char* requested);
int dh2_script_alias_contains(const dh2_script_aliases*,const char* name);
int dh2_script_alias_add(dh2_script_aliases*,const char* name,const char* replacement);
/* Original AddToVFTable producer guard, after source Value projection.
 * Fewer than two or non-string arguments silently produce no mutation. */
int dh2_script_alias_add_values(dh2_script_aliases*,const dh2_script_value*,uint32_t count);
/* Push clears a SINGLE backup and enables recording. Pop restores in unsigned
 * hash order: nonempty previous values are restored; empty values are erased. */
int dh2_script_alias_push(dh2_script_aliases*);
int dh2_script_alias_pop(dh2_script_aliases*);
/* Borrowed map must outlive this VM's bindings. These install genuine source
 * PushVFTable, AddToVFTable and PopVFTable, not timer or manager ownership. */
int dh2_script_alias_bind(dh2_script_vm*,dh2_script_aliases*);
int dh2_script_alias_call_discard_source(dh2_script_vm*,const dh2_script_aliases*,
  const char* requested,const dh2_script_value* arguments,uint32_t count);
/* Native contract: 0 success, -1 malformed caller, -2 allocation/protected
 * error. Allocation failure has no invented source rollback guarantee.
 * Alias allocation is owned separately from the VM allocator. */
#ifdef __cplusplus
}
#endif
#endif
