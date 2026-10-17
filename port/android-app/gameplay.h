#ifndef DH2_SOURCE_GAMEPLAY_H
#define DH2_SOURCE_GAMEPLAY_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct dh2_gameplay dh2_gameplay;
struct dh2_gameplay_bytes { const void *data; size_t size; };
/* Authored development encounter. Visual map, original property/class/loot/
 * power/quest data and recovered combat/health/death/quest kernels are used.
 * Placement, balancing, movement, enemy steering and event routing are authored;
 * this is not the original game world's lifecycle or a complete game port.
 * Assets order: properties, classes, loot, powers, quests, merged constants,
 * original combat-formulas source. Input is borrowed only during creation. */
dh2_gameplay *dh2_gameplay_create(const struct dh2_gameplay_bytes assets[7],
                                 char *error,size_t capacity);
void dh2_gameplay_destroy(dh2_gameplay *);
int dh2_gameplay_reset(dh2_gameplay *);
/* Single owner/thread. Invalid movement/time inputs reject without mutation.
 * Snapshot layout: x,y,heading,moving,attackPulse,enemyCount, then per enemy
 * x,y,heading,hpRatio,hitPulse. At most 21 floats; dead enemies retain entries
 * with hpRatio=0. Returns snapshot length or zero on failure. */
size_t dh2_gameplay_step(dh2_gameplay *,float move_x,float move_y,float seconds,
                         int attack,float snapshot[21]);
void dh2_gameplay_status(const dh2_gameplay *,char *text,size_t capacity);
/* Filled by the source world renderer; no movement is allowed off its floor. */
int dh2_gameplay_player_hp(const dh2_gameplay *);
/* Authored DH2S v1 checkpoints, unrelated to original .savegame files. Caller
 * supplies SHA-256 of the scene/definition/input generation once after create.
 * Save is <=8192 bytes. Restore and fresh reset create a candidate and replace
 * *session only on complete success; failures preserve the preceding session. */
int dh2_gameplay_set_generation(dh2_gameplay *,const unsigned char generation[32]);
size_t dh2_gameplay_save(dh2_gameplay *,void *output,size_t capacity);
int dh2_gameplay_restore(dh2_gameplay **session,const void *bytes,size_t size,char *error,size_t capacity);
int dh2_gameplay_reset_transactional(dh2_gameplay **session,char *error,size_t capacity);
#ifdef __cplusplus
}
#endif
#endif
