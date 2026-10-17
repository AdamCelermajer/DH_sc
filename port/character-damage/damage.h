#ifndef DH2_CHARACTER_DAMAGE_H
#define DH2_CHARACTER_DAMAGE_H
#include "../character-state/state.h"
#ifdef __cplusplus
extern "C" {
#endif
/* Numeric projection of Character::HitFor for a non-player target and an
 * unresolved attacker handle. Policies are explicit borrowed boundary values;
 * this is not the original Character, player manager or death state machine. */
struct dh2_hit_policy {
    uint32_t target_dead, target_monster, local_player_alive;
    uint32_t online, manager_present;
    int32_t manager_mode;
    uint32_t monster_invincible, force_kill_config, force_kill_switch;
    uint32_t target_network;
};
struct dh2_hit_state { int32_t death_reason; };
struct dh2_hit_result {
    uint32_t processed, death_requested;
    int32_t damage_whole;
};
/* Damage has the original unsigned raw fixed-point bit pattern. No lower/upper
 * saturation before subtraction. Dead targets are untouched. A death request
 * does not set target_dead: the original delegates that to a separate owner.
 * Empty buffs. Rejects invalid flags, table, pointers or aliases atomically.
 * 0 success, 1 invalid pointer/alias, 2 invalid data. */
uint32_t dh2_hit_nonplayer(const struct dh2_property_table *,
    struct dh2_character_props *,struct dh2_hit_state *,
    const struct dh2_hit_policy *,uint32_t,struct dh2_hit_result *);
#ifdef __cplusplus
}
#endif
#endif
