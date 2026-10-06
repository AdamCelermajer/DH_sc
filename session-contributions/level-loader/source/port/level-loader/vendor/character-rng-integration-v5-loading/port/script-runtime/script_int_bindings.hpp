#ifndef DH2_SCRIPT_INT_BINDINGS_HPP
#define DH2_SCRIPT_INT_BINDINGS_HPP
#include "script_runtime.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct dh2_script_int_map dh2_script_int_map;
typedef struct dh2_script_string_projection dh2_script_string_projection;
typedef int (*dh2_script_int_identity)(void*,uintptr_t,uint32_t*);
/* Original imported sprintf("%f", promoted float). Provider supplies exact
 * platform/locale spelling, including NaNs; no native printf equivalence is
 * assumed. Output must be NUL terminated, bytes excludes NUL and <=31. */
typedef int (*dh2_script_int_format_fraction)(void*,float,char*,size_t,size_t*);
typedef struct dh2_script_int_bindings {
  dh2_script_int_map* map;
  void* context;
  dh2_script_int_identity identity;
  dh2_script_int_format_fraction format_fraction;
  uint64_t reserved;
} dh2_script_int_bindings;
dh2_script_int_map* dh2_script_int_create(void);
void dh2_script_int_destroy(dh2_script_int_map*);
/* Source dtor clears this map after aliases, before VM close. Keep wrapper
 * alive through Lua __gc; callbacks may insert anew. Destroy after VM close. */
int dh2_script_int_clear_contents(dh2_script_int_map*);
uint32_t dh2_script_int_hash(const char* source_string);
int dh2_script_int_set(dh2_script_int_map*,const char*,int32_t);
/* A missing hash inserts0. Keys are unsigned hashes only, not name pairs. */
int dh2_script_int_get(dh2_script_int_map*,const char*,int32_t*);
size_t dh2_script_int_size(const dh2_script_int_map*);
int dh2_script_int_entry(const dh2_script_int_map*,size_t,uint32_t*,int32_t*);
dh2_script_string_projection* dh2_script_string_projection_create(void);
void dh2_script_string_projection_destroy(dh2_script_string_projection*);
/* Source strings return the EXACT input pointer; nil/bool return shared
 * constants. Converted numbers/identities use workspace-owned storage until
 * its next conversion/destruction. Unknown source kinds return NULL/status0.
 * Input strings must be genuine NUL-terminated source strings; text_bytes may
 * include bytes after firstNUL. Identity mappings are never pointer truncation.
 * Integral number formatting includes signed saturation/Infinity/negative0;
 * fractional/NaN formatting requires the explicit imported printf service. */
int dh2_script_value_get_string(dh2_script_string_projection*,
  const dh2_script_int_bindings*,const dh2_script_value*,const char**);
/* Source getNumber→imported signed32 conversion. Genuine fresh empty Lua VM
 * parses source strings. Imported signed conversion contract: trunc0,
 * saturation and NaN0. Allocation failure is explicit, not original panic. */
int dh2_script_value_get_integer(const dh2_script_int_bindings*,
  const dh2_script_value*,int32_t*);
int dh2_script_int_set_callback(void*,const dh2_script_value*,uint32_t,
  dh2_script_value*,uint32_t,uint32_t*,char*,size_t);
int dh2_script_int_get_callback(void*,const dh2_script_value*,uint32_t,
  dh2_script_value*,uint32_t,uint32_t*,char*,size_t);
/* Borrowed receiver/services outlive bound callbacks and VM close. Existing
 * source-values VM bridge's >16 argument rejection remains explicit. No
 * global/default map is created and no missing identity/printf service faked. */
int dh2_script_int_bind(dh2_script_vm*,const dh2_script_int_bindings*);
#ifdef __cplusplus
}
static_assert(sizeof(dh2_script_int_bindings)==40,"source integer map receiver ABI");
#endif
#endif
