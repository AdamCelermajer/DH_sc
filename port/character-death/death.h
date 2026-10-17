#ifndef DH2_CHARACTER_DEATH_H
#define DH2_CHARACTER_DEATH_H
#include "../character-state/state.h"
#ifdef __cplusplus
extern "C" {
#endif
/* Non-player Character::Kill projection with a null killer. Metadata ownership
 * is authored; no original pointers, Character constructor or event queue. */
struct dh2_death_actor {
    uint32_t dead,network,suppress_events,target_id;
    int32_t property_id,template_id;
};
struct dh2_death_policy {
    uint32_t forced,loot_manager_present;
    int32_t objective_ids[4];
};
/* Stable source kinds: 0 KillXEnemies, 1 ClearEnemies, 2 KillEnemyTemplate,
 * 3 ClearEnemyTemplate. Native event vtable identities are represented by kind.
 * Original null killer, two zero flags and value -1 are implicit. */
struct dh2_death_event {uint32_t kind,target_id;int32_t objective_id,match_id;};
struct dh2_death_result {
    uint32_t processed,drop_loot_requested,event_count;
    int32_t drop_loot_id;
    struct dh2_death_event events[4];
};
/* Property and template IDs are original signed short fields, checked before mutation.
 * Results describe external requests; they do not grant loot/quest progress.
 * Empty buffs; 0 success, 1 bad pointer/alias, 2 invalid data. Atomic rejection. */
uint32_t dh2_death_nonplayer(const struct dh2_property_table *,
    struct dh2_character_props *,struct dh2_death_actor *,
    const struct dh2_death_policy *,struct dh2_death_result *);
#ifdef __cplusplus
}
#endif
#endif
