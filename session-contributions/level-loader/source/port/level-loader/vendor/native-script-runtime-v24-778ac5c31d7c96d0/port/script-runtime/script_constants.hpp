#ifndef DH2_SCRIPT_CONSTANTS_HPP
#define DH2_SCRIPT_CONSTANTS_HPP
#include <cstdint>
struct dh2_script_constants;
struct dh2_script_constants_reload {
  uint32_t consumed, assignments, groups_complete, source_name_stop;
};
extern "C" {
dh2_script_constants* dh2_script_constants_create();
void dh2_script_constants_destroy(dh2_script_constants*);
void dh2_script_constants_clear(dh2_script_constants*);
/* Source PyDataConstants reloadData merges named entries, assigning later
 * duplicates and retaining old entries. Empty groups are not inserted. Source
 * readString consumes at most255 bytes and stops reload when length>=256.
 * 0=count-driven completion, 1=source name stop, -1=bounded input/API failure,
 * -2=allocation failure. Partial assignments survive every stop. Short stream
 * reads are outside original parity: native rejects instead of reading stale
 * stack data. The actual application file load order is caller-owned. */
int dh2_script_constants_load(dh2_script_constants*,const uint8_t*,uint32_t,
                             dh2_script_constants_reload*);
int dh2_script_constants_get(const dh2_script_constants*,const char*,const char*,int32_t*);
uint32_t dh2_script_constants_size(const dh2_script_constants*);
/* Persistent design lookup context can use this for kind0. Array/OID kind1
 * remains a different backend and returns delivery failure here. */
int dh2_script_constants_lookup(void*,uint32_t,const char*,const char*,int32_t*);
}
static_assert(sizeof(dh2_script_constants_reload)==16,"constant reload projection");
#endif
