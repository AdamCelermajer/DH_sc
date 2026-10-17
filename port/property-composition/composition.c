#include "composition.h"
#include <stddef.h>
#include <string.h>
static int overlap(const void *a,size_t an,const void *b,size_t bn) {
    uintptr_t aa=(uintptr_t)a,bb=(uintptr_t)b;
    return aa+an<aa || bb+bn<bb || (an && bn && aa<bb+bn && bb<aa+an);
}
static int32_t add(int32_t current,int32_t delta,int32_t base) {
    if(current==base)return delta;
    uint32_t bits=(uint32_t)current+(uint32_t)delta;int32_t value;memcpy(&value,&bits,4);return value;
}
static int aliases(struct dh2_property_sheet *out,const void *p,size_t n) { return overlap(out,sizeof(*out),p,n); }
uint32_t dh2_property_recalc(const struct dh2_property_table *t,const struct dh2_property_inputs *in,uint32_t id,struct dh2_property_sheet *out) {
    if(!t || !in || !out || !in->base || !in->saved || !in->gears || (!in->groups && in->count))return 1;
    if(id>=224 || in->count>32)return 2;
    if(aliases(out,t,sizeof(*t)) || aliases(out,t->bytes,t->size) || aliases(out,in,sizeof(*in)) ||
       aliases(out,in->base,sizeof(*out)) || aliases(out,in->saved,sizeof(*out)) || aliases(out,in->gears,sizeof(*out)) ||
       aliases(out,in->groups,in->count*sizeof(*in->groups)))return 1;
    uint32_t total=0;
    for(uint32_t g=0;g<in->count;++g) {
        const struct dh2_property_group *group=in->groups+g;
        if(group->count>64 || total+group->count>1024)return 2;
        if((!group->sheets && group->count) || aliases(out,group->sheets,group->count*sizeof(*group->sheets)))return 1;
        total+=group->count;
        for(uint32_t j=0;j<group->count;++j)
            if(!group->sheets[j] || aliases(out,group->sheets[j],sizeof(*out)))return 1;
    }
    int32_t base,type;uint32_t status=dh2_property_default(t,id,&base);
    if(status)return status;
    status=dh2_property_type(t,id,&type);if(status)return status;
    int32_t value=out->values[id];
    const struct dh2_property_sheet *plain[]={in->base,in->saved,in->gears};
    if(type&4) {
        value=base;
        for(unsigned p=0;p<3;++p)if(plain[p]->values[id]!=base)value=add(value,plain[p]->values[id],base);
        for(uint32_t g=0;g<in->count;++g)for(uint32_t j=0;j<in->groups[g].count;++j) {
            int32_t v=in->groups[g].sheets[j]->values[id];if(v!=base)value=add(value,v,base);
        }
    } else if(type&2) {
        int found=0;
        for(unsigned p=0;p<3;++p)if(plain[p]->values[id]!=base) { value=plain[p]->values[id];found=1;break; }
        if(!found)for(uint32_t g=0;g<in->count;++g) {
            for(uint32_t j=0;j<in->groups[g].count;++j) {
                int32_t v=in->groups[g].sheets[j]->values[id];if(v!=base) { value=v;found=1; }
            }
            if(found)break;
        }
        if(!found)value=base;
    } else if(type&1) {
        int found=0;
        for(uint32_t g=in->count;g>0;--g) {
            const struct dh2_property_group *group=in->groups+g-1;
            for(uint32_t j=0;j<group->count;++j) {
                int32_t v=group->sheets[j]->values[id];if(v!=base) { value=v;found=1; }
            }
            if(found)break;
        }
        if(!found)for(unsigned p=3;p>0;--p)if(plain[p-1]->values[id]!=base) { value=plain[p-1]->values[id];found=1;break; }
        if(!found)value=base;
    } else if(type&32)value=add(in->base->values[id],in->saved->values[id],base);
    else if(type&16)value=in->base->values[id];
    out->values[id]=value;return 0;
}
