#ifndef DH2_LOOT_TABLES_H
#define DH2_LOOT_TABLES_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
enum dh2_loot_table_id { DH2_DROP_TILE=0,DH2_INVENTORY=1,DH2_ITEM_LIST=2,DH2_ITEMS=3,
                        DH2_ITEM_TYPES=4,DH2_LOOT=5,DH2_MERCHANTS=6,DH2_NUM_PROBS=7 };
struct dh2_loot_tables { const unsigned char *bytes;uint32_t size,counts[8],offsets[8]; };
struct dh2_loot_span { const unsigned char *bytes;uint32_t size; };
struct dh2_loot_item {
    struct dh2_loot_span name;
    int32_t base_ints[4];uint32_t base_bool,base_float_bits[9];
    int32_t item_ints[2];
    struct dh2_loot_span description;
    int32_t tail_ints[20];
};
/* Borrowed immutable whole loot_table_pyarray.bin, <=4 MiB. Validate all eight
 * table schemas, bounded counts, and exact consumption. No ARM32 pointers or
 * vtables enter source objects. Strings remain byte spans, not C strings.
 * 0 succeeds; 1 bad pointers/alias; 2 bad data/index. Rejection preserves output.
 * The caller keeps backing bytes alive and immutable for the view and spans. */
uint32_t dh2_loot_open(struct dh2_loot_tables *,const void *,uint32_t);
uint32_t dh2_loot_record(const struct dh2_loot_tables *,uint32_t,uint32_t,struct dh2_loot_span *);
uint32_t dh2_loot_item_read(const struct dh2_loot_tables *,uint32_t,struct dh2_loot_item *);
#ifdef __cplusplus
}
#endif
#endif
