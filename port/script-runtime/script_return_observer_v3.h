#ifndef DH2_SCRIPT_RETURN_OBSERVER_V3_H
#define DH2_SCRIPT_RETURN_OBSERVER_V3_H
#include "script_return_observer_v1.h"
#ifdef __cplusplus
extern "C" {
#endif
/* One source Call, complete ordered projection, then one selected return.
 * index is zero based. An absent selected return has type0 and still reports
 * the actual total count. String bytes are borrowed during the observer only.
 * All V1 observer/busy/ownership restrictions remain in force. */
int dh2_script_vm_call_indexed_source_v3(dh2_script_vm*,const char*,
 const dh2_script_value*,uint32_t,uint32_t index,
 dh2_script_return_observer_v1,void*);
/* Original synchronous native callback reentry on its SAME protected VM.
 * The capability must still be the active source provider. Busy stays set;
 * nested providers shadow and restore it through the existing protocol.
 * Full return projection/selected observer/error/required-epoch semantics
 * match V3; no unscoped or retained capability admission is added. */
int dh2_script_callback_call_indexed_source_v112(const dh2_script_callback_scope*,const char*,
 const dh2_script_value*,uint32_t,uint32_t index,dh2_script_return_observer_v1,void*);
#ifdef __cplusplus
}
#endif
#endif
