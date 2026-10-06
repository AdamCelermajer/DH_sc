#ifndef DH2_SCRIPT_DESIGN_BINDINGS_H
#define DH2_SCRIPT_DESIGN_BINDINGS_H
#include "script_runtime.h"
#ifdef __cplusplus
extern "C" {
#endif
/* Source Application design services: kind0 PyDataConstants.getConstant;
 * kind1 PyDataArrays.GetOID (shared by GetPyStruct and GetPyOID). Lookup returns
 * an actual signed source value, including constant-miss0 or OID-miss-1.
 * Delivery failure is distinct from an authored lookup miss. The persistent
 * services/context outlive bound VM callbacks. Strings are source-projected,
 * borrowed NUL-terminated C strings; native arrays/registration lifetime and
 * case-sensitive manager lookup belong to this service, not invented globals. */
typedef int (*dh2_script_design_lookup)(void*,uint32_t,const char*,const char*,int32_t*);
typedef struct dh2_script_design_bindings {
  void* context;
  dh2_script_design_lookup lookup;
  uint64_t reserved;
} dh2_script_design_bindings;
int dh2_script_design_get_constant(void*,const dh2_script_value*,uint32_t,
    dh2_script_value*,uint32_t,uint32_t*,char*,size_t);
int dh2_script_design_get_struct(void*,const dh2_script_value*,uint32_t,
    dh2_script_value*,uint32_t,uint32_t*,char*,size_t);
int dh2_script_design_get_oid(void*,const dh2_script_value*,uint32_t,
    dh2_script_value*,uint32_t,uint32_t*,char*,size_t);
/* Existing source-values VM binder projects at most16 arguments. Direct
 * kernels accept the original full uint32 arity and ignore trailing values. */
int dh2_script_design_bind(dh2_script_vm*,const dh2_script_design_bindings*);
#ifdef __cplusplus
}
static_assert(sizeof(dh2_script_design_bindings)==24,"design lookup services ABI");
#endif
#endif
