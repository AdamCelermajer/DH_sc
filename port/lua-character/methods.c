#include "methods.h"
#include <stddef.h>
#include <math.h>
static int overlap(const void *a,size_t an,const void *b,size_t bn) {
    uintptr_t aa=(uintptr_t)a,bb=(uintptr_t)b;
    return aa+an<aa || bb+bn<bb || (an && bn && aa<bb+bn && bb<aa+an);
}
static int number(float x,int32_t *out) {
    if(!isfinite(x) || (double)x<-2147483648.0 || (double)x>=2147483648.0)return 0;
    *out=(int32_t)x;return 1;
}
uint32_t dh2_character_property_method(const struct dh2_property_table *t,struct dh2_character_props *s,uint32_t op,const float *args,const uint32_t *tags,uint32_t count,struct dh2_property_result *out) {
    if(!t || !s || !out || (!args && count) || (!tags && count) || op>1 || count>32)return 1;
    size_t an=count*sizeof(*args),tn=count*sizeof(*tags);
    if(overlap(out,sizeof(*out),s,sizeof(*s)) || overlap(out,sizeof(*out),t,sizeof(*t)) ||
       overlap(out,sizeof(*out),t->bytes,t->size) || overlap(out,sizeof(*out),args,an) || overlap(out,sizeof(*out),tags,tn) ||
       overlap(s,sizeof(*s),args,an) || overlap(s,sizeof(*s),tags,tn) || overlap(s,sizeof(*s),t,sizeof(*t)) || overlap(s,sizeof(*s),t->bytes,t->size))return 1;
    struct dh2_property_result result={0,0};
    if(!count || tags[0]!=3 || (op==1 && (count<2 || tags[1]!=3))) { *out=result;return 0; }
    /* Original property IDs use getUInteger and its actual unsigned float
     * conversion: finite negative values clamp to zero. Value writes use the
     * distinct signed conversion above. */
    if(!isfinite(args[0]) || (double)args[0]>=4294967296.0)return 2;
    uint32_t id=args[0]<=0?0:(uint32_t)args[0];
    if(id>=224) { *out=result;return 0; }
    if(op==0) {
        if(count>1 && tags[1]==2)return 2;
        uint32_t status;
        if(count>1 && tags[1]==1 && args[1]!=0)status=dh2_property_default(t,(uint32_t)id,&result.value);
        else status=dh2_property_get(&s->final,(uint32_t)id,&result.value);
        if(status)return status;
        result.count=1;
    } else {
        if(count>2 && tags[2]==2)return 2;
        int32_t value;if(!number(args[1],&value))return 2;
        uint32_t status=dh2_character_props_write(t,s,NULL,0,(uint32_t)id,value,DH2_PROPS_SET);
        if(status)return status;
    }
    *out=result;return 0;
}
