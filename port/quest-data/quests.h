#ifndef DH2_QUEST_DATA_H
#define DH2_QUEST_DATA_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
struct dh2_quest_table {const unsigned char *bytes;uint32_t size,count;};
struct dh2_quest_span {uint32_t offset,size;};
struct dh2_quest_list {uint32_t offset,count;};
/* Serialized stub fields: three common ints, two strings, three type-specific
 * ints. Integers are raw; interpreting other objective types is caller work. */
struct dh2_quest_objective {
    int32_t common[3];struct dh2_quest_span strings[2];int32_t args[3];
};
struct dh2_quest_record {
    int32_t ids[4];
    /* 0 conditions, 1 objectives, 2/3/4 rewards (normal/hard/very hard). */
    struct dh2_quest_list lists[5];
    struct dh2_quest_objective accept,end;
    int32_t target_level;uint32_t repeatable;int32_t state;
    struct dh2_quest_span scripts[14];int32_t priority,act;
};
/* Borrowed immutable complete v2quests_pyarray.bin, <=4 MiB. Validate all
 * records, strings/lists and exact consumption. Max 4096 records/list entries.
 * No original pointers/vtables enter source results. Spans use file offsets.
 * 0 success, 1 bad pointer/alias, 2 data/index error. Rejection is atomic.
 * Backing bytes remain alive and immutable while the view is used. */
uint32_t dh2_quests_open(struct dh2_quest_table *,const void *,uint32_t);
uint32_t dh2_quests_record(const struct dh2_quest_table *,uint32_t,struct dh2_quest_record *);
uint32_t dh2_quests_list_record(const struct dh2_quest_table *,uint32_t,uint32_t,uint32_t,struct dh2_quest_span *);
uint32_t dh2_quests_objective(const struct dh2_quest_table *,uint32_t,uint32_t,struct dh2_quest_objective *);
#ifdef __cplusplus
}
#endif
#endif
