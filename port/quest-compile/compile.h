#ifndef DH2_QUEST_COMPILE_H
#define DH2_QUEST_COMPILE_H
#include "../quest-kill/quest.h"
#ifdef __cplusplus
extern "C" {
#endif
/* Resolved IDs are signed native shorts. A nonzero present byte represents a
 * non-null loaded-list entry. Resolution/loading of original Character is external. */
struct dh2_quest_character {uint32_t present;int32_t property_id,template_id;};
struct dh2_quest_cache_entry {int32_t property_id,quantity;};
/* kind 0 property, 1 template. Loaded entries are counted regardless of dead
 * status. If no property matches, return its cached quantity. The original
 * template comparator differs from the property comparator, so no cache fallback.
 * <=4096 entries each; unique cache IDs; failures preserve output. */
uint32_t dh2_quest_population(uint32_t,int32_t,const struct dh2_quest_character *,uint32_t,
    const struct dh2_quest_cache_entry *,uint32_t,int32_t *);
struct dh2_quest_compiled {struct dh2_kill_objective progress;uint32_t active;};
struct dh2_quest_compile_context {
    /* source kinds 0 kill property, 1 clear property, 2 kill template, 3 clear template */
    uint32_t kind;int32_t match_id,record_level,current_level,record_required,population;
};
struct dh2_quest_compile_result {uint32_t eligible,required_updated,completion_requested,newly_completed;};
/* Already resolved world population feeds the original four Compile paths.
 * Counted objectives set required and clear active on failure. Clear objectives
 * preserve active (and preserve required on level mismatch). Completion observers
 * and persistent save actions remain external. Native flags accept byte 0..255.
 * 0 success, 1 null/alias, 2 invalid data. Rejections preserve state/output. */
uint32_t dh2_quest_compile(struct dh2_quest_compiled *,const struct dh2_quest_compile_context *,
    struct dh2_quest_compile_result *);
#ifdef __cplusplus
}
#endif
#endif
