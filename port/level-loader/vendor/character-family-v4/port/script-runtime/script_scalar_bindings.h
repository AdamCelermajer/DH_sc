#ifndef DH2_SCRIPT_SCALAR_BINDINGS_H
#define DH2_SCRIPT_SCALAR_BINDINGS_H
#include "script_runtime.h"
#ifdef __cplusplus
extern "C" {
#endif
/* These imports are explicit source dependencies. Native identities must be
 * mapped to their source uint32 numeric identity; no pointer truncation. The
 * division-zero handler supplies the imported __aeabi_idiv result, not a
 * guessed game policy. Return0 delivers the value; nonzero rejects the call.
 * Borrowed services/context remain alive while bound VM callbacks exist. */
typedef int (*dh2_script_scalar_identity)(void*,uintptr_t,uint32_t*);
typedef int (*dh2_script_scalar_divide_zero)(void*,int32_t,int32_t*);
typedef struct dh2_script_scalar_bindings {
  void* context;
  dh2_script_scalar_identity identity;
  dh2_script_scalar_divide_zero divide_zero;
  uint64_t reserved;
} dh2_script_scalar_bindings;
#define DH2_SCRIPT_SCALAR_CALLBACK(name) int dh2_script_scalar_##name(void*,const dh2_script_value*,uint32_t,dh2_script_value*,uint32_t,uint32_t*,char*,size_t)
DH2_SCRIPT_SCALAR_CALLBACK(to_fixed);
DH2_SCRIPT_SCALAR_CALLBACK(from_fixed);
DH2_SCRIPT_SCALAR_CALLBACK(mul_fixed);
DH2_SCRIPT_SCALAR_CALLBACK(div_fixed);
DH2_SCRIPT_SCALAR_CALLBACK(bit_not);
DH2_SCRIPT_SCALAR_CALLBACK(bit_and);
DH2_SCRIPT_SCALAR_CALLBACK(bit_or);
DH2_SCRIPT_SCALAR_CALLBACK(bit_xor);
#undef DH2_SCRIPT_SCALAR_CALLBACK
/* Source callback kernels accept full uint32 arity. Existing VM source-values
 * bridge permits at most16 arguments; calls above that explicit VM boundary
 * fail, rather than silently truncating BitAnd/BitOr. A NULL service structure
 * installs arithmetic callbacks; identity/division-zero calls then reject. */
int dh2_script_scalar_bind(dh2_script_vm*,const dh2_script_scalar_bindings*);
#ifdef __cplusplus
}
static_assert(sizeof(dh2_script_scalar_bindings)==32,"source scalar services ABI");
#endif
#endif
