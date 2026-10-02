#ifndef DH2_CHARACTER_STATE_H
#define DH2_CHARACTER_STATE_H
#include "../property-composition/composition.h"
#ifdef __cplusplus
extern "C" {
#endif
struct dh2_character_props { struct dh2_property_sheet base,saved,gears,final; };
enum dh2_property_write { DH2_PROPS_SET=0,DH2_PROPS_ADD=1,DH2_PROPS_SET_INT=2,DH2_PROPS_ADD_INT=3 };
/* Explicit source ownership of four sheets, not an original Character object.
 * init resets all sheets to defaults. Writes project original PROPS_Set/Add and
 * shifted Int wrappers; groups are borrowed ordered buffs from composition.
 * 0 succeeds, 1 bad arguments/alias, 2 invalid table/index/count/operation.
 * Reject aliases between mutable state and any input; preserve on rejection. */
uint32_t dh2_character_props_init(const struct dh2_property_table *,struct dh2_character_props *);
uint32_t dh2_character_props_write(const struct dh2_property_table *,struct dh2_character_props *,const struct dh2_property_group *,uint32_t,uint32_t,int32_t,uint32_t);
uint32_t dh2_character_props_get_int(const struct dh2_character_props *,uint32_t,int32_t *);
#ifdef __cplusplus
}
#endif
#endif
