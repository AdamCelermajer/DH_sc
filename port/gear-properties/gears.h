#ifndef DH2_GEAR_PROPERTIES_H
#define DH2_GEAR_PROPERTIES_H
#include "../character-classes/classes.h"
#include "../loot-tables/loot.h"
#ifdef __cplusplus
extern "C" {
#endif
struct dh2_power_tables { const unsigned char *bytes;uint32_t size,counts[2],offsets[2]; };
struct dh2_power_record { int32_t type,value;uint32_t count;const unsigned char *stats;int32_t tail[5]; };
struct dh2_gear_entry { int32_t item_id;uint32_t power_count;const int32_t *power_ids; };
/* Immutable borrowed complete item_powers_pyarray.bin, <=4 MiB, <=65536 rows
 * per table. Nested stats retain serialized little-endian triples, not native
 * vtables. No random generation, equip requirements or ItemInstance ownership.
 * Caller supplies equipment in original slot order; only slot 2 is left hand.
 * -1 is an empty slot. <=64 slots, <=4096 total powers are authored limits.
 * All mutable output/input aliases reject; every rejection preserves output.
 * 0 success,1 invalid pointers/aliases,2 malformed data/index/count. */
uint32_t dh2_power_open(struct dh2_power_tables *,const void *,uint32_t);
uint32_t dh2_power_read(const struct dh2_power_tables *,uint32_t,struct dh2_power_record *);
uint32_t dh2_gear_add_item(const struct dh2_property_table *,const struct dh2_loot_tables *,struct dh2_property_sheet *,uint32_t,uint32_t);
uint32_t dh2_gear_add_power(const struct dh2_property_table *,const struct dh2_power_tables *,struct dh2_property_sheet *,uint32_t,uint32_t);
uint32_t dh2_gear_load(const struct dh2_property_table *,const struct dh2_loot_tables *,const struct dh2_power_tables *,struct dh2_property_sheet *,const struct dh2_gear_entry *,uint32_t);
/* Reconstructed empty-buff lifecycle. Class application uses base field 26
 * and from_final=false, followed by ascending recalculation of all 224 fields.
 * Repeated recalculation can compound class rules, as in the original.
 * Does not construct an original Character or own inventory/buff containers. */
uint32_t dh2_character_recalc(const struct dh2_property_table *,const struct dh2_class_table *,struct dh2_character_props *,uint32_t);
/* Negative/out-of-range row IDs leave the reset base at defaults, then apply
 * its class and recalculate, matching the original guarded row loader. */
uint32_t dh2_character_update_base(const struct dh2_property_table *,const struct dh2_class_table *,struct dh2_character_props *,int32_t);
uint32_t dh2_character_update_gears(const struct dh2_property_table *,const struct dh2_class_table *,const struct dh2_loot_tables *,const struct dh2_power_tables *,struct dh2_character_props *,const struct dh2_gear_entry *,uint32_t);
#ifdef __cplusplus
}
#endif
#endif
