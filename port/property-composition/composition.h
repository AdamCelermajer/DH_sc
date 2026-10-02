#ifndef DH2_PROPERTY_COMPOSITION_H
#define DH2_PROPERTY_COMPOSITION_H
#include "../character-properties/properties.h"
#ifdef __cplusplus
extern "C" {
#endif
/* Buff groups are already ordered by ascending original map order. Each group's
 * sheets retain original deque order. Borrowed input; no original map ownership. */
struct dh2_property_group { const struct dh2_property_sheet *const *sheets; uint32_t count; };
struct dh2_property_inputs {
    const struct dh2_property_sheet *base,*saved,*gears;
    const struct dh2_property_group *groups;
    uint32_t count;
};
/* Recalculate one field of final. Limits: <=32 groups, <=64 sheets/group,
 * <=1024 sheets total. Reject input/output aliases and preserve on rejection.
 * 0 success, 1 bad arguments/alias, 2 invalid index/table/count. */
uint32_t dh2_property_recalc(const struct dh2_property_table *,const struct dh2_property_inputs *,uint32_t,struct dh2_property_sheet *final);
#ifdef __cplusplus
}
#endif
#endif
