#include "random.h"
#include <stddef.h>
#include <math.h>
#include <string.h>
static int32_t signed_bits(uint32_t x) {int32_t y;memcpy(&y,&x,4);return y;}
int32_t dh2_random_next(struct dh2_random_state *s,uint32_t bound,uint32_t sync) {
    uint32_t i=sync!=0,value=0;
    if(bound) {
        s->seeds[i]=(s->seeds[i]*UINT32_C(0xe6ab)+UINT32_C(0x2b3fd))%UINT32_C(0xdaf26b);
        value=s->seeds[i]%bound;
        /* The native absolute-value operation is redundant after reduction:
         * the new seed is below 0xdaf26b and its remainder is nonnegative. */
    }
    ++s->counters[i];return signed_bits(value);
}
static int overlap(const void *a,size_t an,const void *b,size_t bn) {
    uintptr_t aa=(uintptr_t)a,bb=(uintptr_t)b;
    return aa+an<aa || bb+bn<bb || (an && bn && aa<bb+bn && bb<aa+an);
}
static int uint_number(float x,uint32_t *out) {
    if(!isfinite(x) || (double)x>=4294967296.0)return 0;
    *out=x<=0?0:(uint32_t)x;return 1;
}
uint32_t dh2_random_callback(struct dh2_random_state *s,uint32_t *seed,uint32_t online,
    const float *args,const uint32_t *tags,uint32_t count,struct dh2_random_result *out) {
    if(!s || !seed || !out || count>32 || (!args && count) || (!tags && count))return 1;
    size_t n=count*4;
    const void *writes[]={s,seed,out};size_t sizes[]={sizeof(*s),4,sizeof(*out)};
    for(unsigned i=0;i<3;++i) {
        if(overlap(writes[i],sizes[i],args,n) || overlap(writes[i],sizes[i],tags,n))return 1;
        for(unsigned j=0;j<i;++j)if(overlap(writes[i],sizes[i],writes[j],sizes[j]))return 1;
    }
    uint32_t base=0,bound=100;struct dh2_random_result result={0,0};
    if(count==1 || count==2) {
        if(tags[0]!=3 || (count==2 && tags[1]!=3)) {*out=result;return 0;}
        if(!uint_number(args[0],&bound))return 2;
        if(count==2) {
            base=bound;if(!uint_number(args[1],&bound))return 2;bound-=base;
        }
    }
    struct dh2_random_state next=*s;uint32_t next_seed=*seed;
    if(online)next.seeds[0]=next_seed;
    result.value=signed_bits((uint32_t)dh2_random_next(&next,bound,0)+base);result.count=1;
    if(online)next_seed=next.seeds[0];
    *s=next;*seed=next_seed;*out=result;return 0;
}
