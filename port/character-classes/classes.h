#ifndef DH2_CHARACTER_CLASSES_H
#define DH2_CHARACTER_CLASSES_H
#include "../character-state/state.h"
#ifdef __cplusplus
extern "C" {
#endif
struct dh2_class_table { const unsigned char *bytes;uint32_t size,count; };
/* Borrowed complete character_classes_pyarray.bin <=4 MiB, <=4096 classes,
 * <=65536 rules total. View and output must not alias source bytes. */
uint32_t dh2_class_open(struct dh2_class_table *,const void *,uint32_t);
/* Apply ordered class rules with empty buffs. target: 0 base,1 saved,2 gears,
 * 3 final,4 temporary. Temporary storage is required only for target 4.
 * from_final: 0 or 1. Negative/out-of-range class IDs are original no-ops.
 * Guards limit recursion to 64 levels and 100000 rule/group visits; reject
 * invalid property IDs, malformed views and aliases without changing outputs.
 * Original class application is not transactional; source guards are authored.
 * 0 success,1 invalid arguments/aliases,2 malformed/index/limits. */
uint32_t dh2_class_apply(const struct dh2_class_table *,const struct dh2_property_table *,
                         struct dh2_character_props *,uint32_t,struct dh2_property_sheet *,int32_t,uint32_t);
#ifdef __cplusplus
}
#endif
#endif
