#ifndef DH2_CHARACTER_PROPERTIES_H
#define DH2_CHARACTER_PROPERTIES_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define DH2_PROPERTY_COUNT 224
struct dh2_property_sheet { int32_t values[DH2_PROPERTY_COUNT]; };
struct dh2_property_table {
    const unsigned char *bytes;
    uint32_t size,counts[3],offsets[3];
};
/* Immutable borrowed complete character_properties_pyarray.bin, <=4 MiB.
 * At least two character rows are required for defaults/types. All three
 * serialized tables must be valid and fully consumed. No ARM32 vtables/pointers
 * enter the source sheet. 0 succeeds, 1 invalid arguments/alias, 2 bad data/index.
 * Every rejection preserves output and caller input. */
uint32_t dh2_property_open(struct dh2_property_table *,const void *,uint32_t);
uint32_t dh2_property_load(const struct dh2_property_table *,uint32_t,struct dh2_property_sheet *);
uint32_t dh2_property_get(const struct dh2_property_sheet *,uint32_t,int32_t *);
uint32_t dh2_property_set(struct dh2_property_sheet *,uint32_t,int32_t);
uint32_t dh2_property_default(const struct dh2_property_table *,uint32_t,int32_t *);
uint32_t dh2_property_type(const struct dh2_property_table *,uint32_t,int32_t *);
uint32_t dh2_property_reset(const struct dh2_property_table *,struct dh2_property_sheet *);
uint32_t dh2_property_is_set(const struct dh2_property_table *,const struct dh2_property_sheet *,uint32_t,int32_t *);
uint32_t dh2_property_add(const struct dh2_property_table *,struct dh2_property_sheet *,uint32_t,int32_t);
#ifdef __cplusplus
}
#endif
#endif
