#ifndef DH2_SCRIPT_GAME_BINDINGS_H
#define DH2_SCRIPT_GAME_BINDINGS_H
#include "script_runtime.h"
#ifdef __cplusplus
extern "C" {
#endif
/* Borrowed source timer services; owner is the Character identity, not AIS/VM.
 * start receives source event0x35 and user_ref0. It returns original timer ID,
 * or exactly -1 when the original service cannot allocate a timer.
 * stop is void: out-of-range IDs have no script result.
 * The structure/context/owner remain alive while bound VM callbacks exist. */
typedef int32_t (*dh2_script_timer_start)(void*,uintptr_t,uint32_t,int32_t,int32_t,uintptr_t);
typedef void (*dh2_script_timer_stop)(void*,uintptr_t,uint32_t);
typedef struct dh2_script_game_bindings {
  void* context;
  uintptr_t character;
  dh2_script_timer_start start;
  dh2_script_timer_stop stop;
  uint64_t reserved;
} dh2_script_game_bindings;
/* Callback ABI used by VM source-value binding and standalone original oracle.
 * Argument types are projected sfc Values, including type7 object identities.
 * Shipping Trace is empty and accepts all projected arguments without logging. */
int dh2_script_game_start_timer(void*,const dh2_script_value*,uint32_t,
  dh2_script_value*,uint32_t,uint32_t*,char*,size_t);
int dh2_script_game_stop_timer(void*,const dh2_script_value*,uint32_t,
  dh2_script_value*,uint32_t,uint32_t*,char*,size_t);
int dh2_script_game_trace(void*,const dh2_script_value*,uint32_t,
  dh2_script_value*,uint32_t,uint32_t*,char*,size_t);
/* Requires genuine timer ownership before any of these names are installed.
 * Missing services reject installation, leaving timer names absent. */
int dh2_script_game_bind(dh2_script_vm*,const dh2_script_game_bindings*);
/* Direct AISDefault::OnScriptTimer projection after caller resolves active AIS.
 * Converts timer ID through signed32 pushInteger, calls real OnTimer closure.
 * It does not invent a generic CharAI/FSM event route. */
int dh2_script_game_on_timer(dh2_script_vm*,uint32_t timer_id);
#ifdef __cplusplus
}
static_assert(sizeof(dh2_script_game_bindings)==40,"native borrowed timer ABI");
#endif
#endif
