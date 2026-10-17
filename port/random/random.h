#ifndef DH2_RANDOM_H
#define DH2_RANDOM_H
#include <stdint.h>
struct dh2_random_state { uint32_t seeds[2],counters[2]; };
struct dh2_random_result { uint32_t count;int32_t value; };
/* Owned state, independent of original globals/Character layout. Any nonzero
 * sync selects the second stream. Zero bound still advances the counter. */
int32_t dh2_random_next(struct dh2_random_state *,uint32_t bound,uint32_t sync);
/* Original numeric/tagged Rand projection, at most 32 argument slots. Online
 * draws use/update object_seed and replace ordinary global seed, as original.
 * Invalid finite conversions/pointer aliases reject without changing state. */
uint32_t dh2_random_callback(struct dh2_random_state *,uint32_t *object_seed,
    uint32_t online,const float *,const uint32_t *tags,uint32_t count,
    struct dh2_random_result *);
#endif
