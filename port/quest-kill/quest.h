#ifndef DH2_QUEST_KILL_H
#define DH2_QUEST_KILL_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
/* Owned projection of four already-dispatched ObjectiveTemplate_KillCharacter
 * event handlers. Caller dispatches the correct event kind. Compile/world
 * population, quest persistence, markers and completion observers are external. */
struct dh2_kill_objective {int32_t match_id,current,required;uint32_t completed;};
struct dh2_kill_progress_event {int32_t match_id,quantity;uint32_t outbound,synchronized;};
struct dh2_kill_progress_result {uint32_t matched,changed,completion_requested,newly_completed;};
/* Native flags are bytes (0..255); synchronized is true for any nonzero byte.
 * Counter increment wraps as ARM32. Completion callback may repeat even when
 * completed was already set. Return 0 success, 1 pointer/alias, 2 invalid data.
 * Validation failures preserve all state and result bytes. */
uint32_t dh2_quest_kill_event(struct dh2_kill_objective *,
    struct dh2_kill_progress_event *,struct dh2_kill_progress_result *);
#ifdef __cplusplus
}
#endif
#endif
