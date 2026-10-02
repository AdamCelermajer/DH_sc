#ifndef DH2_CHARACTER_HEALTH_H
#define DH2_CHARACTER_HEALTH_H
#include "../character-state/state.h"
#ifdef __cplusplus
extern "C" {
#endif
/* Owned four-sheet projection, with empty buffs. Channel 0 HP, 1 MP.
 * Set/get use whole integers; regeneration and mana costs use raw fixed values.
 * Mutations preserve the state on rejected arguments. Output aliases reject.
 * Mana exemption is an explicit boundary input; this module owns no network,
 * config, debug-switch or Character virtual-method implementation. */
uint32_t dh2_health_set(const struct dh2_property_table *,struct dh2_character_props *,uint32_t,int32_t);
uint32_t dh2_health_get(const struct dh2_character_props *,uint32_t,uint32_t,int32_t *);
uint32_t dh2_health_validate(const struct dh2_property_table *,struct dh2_character_props *);
uint32_t dh2_health_regen(const struct dh2_property_table *,struct dh2_character_props *,uint32_t,int32_t);
uint32_t dh2_health_fraction(const struct dh2_character_props *,uint32_t,float *);
/* Original script GetHP returns current, maximum, percentage as three integers.
 * Undefined signed division (zero shifted maximum, INT_MIN/-1) rejects. */
uint32_t dh2_health_script_hp(const struct dh2_character_props *,int32_t [3]);
uint32_t dh2_health_has_mana(const struct dh2_character_props *,int32_t,uint32_t,int32_t *);
uint32_t dh2_health_use_mana(const struct dh2_property_table *,struct dh2_character_props *,int32_t,uint32_t,int32_t *);
#ifdef __cplusplus
}
#endif
#endif
