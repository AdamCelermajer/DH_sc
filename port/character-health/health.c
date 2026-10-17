#include "health.h"
#include <stddef.h>
#include <string.h>
static int overlap(const void *a,size_t an,const void *b,size_t bn) {
    uintptr_t aa=(uintptr_t)a,bb=(uintptr_t)b;
    return aa+an<aa || bb+bn<bb || (an && bn && aa<bb+bn && bb<aa+an);
}
static int32_t bits(uint32_t u) {int32_t i;memcpy(&i,&u,4);return i;}
static int32_t sar8(int32_t i) {uint32_t u=(uint32_t)i;return bits((u>>8)|((u&0x80000000u)?0xff000000u:0));}
static uint32_t current(uint32_t channel) {return channel?41:36;}
static uint32_t maximum(uint32_t channel) {return channel?43:38;}
static uint32_t valid(const struct dh2_property_table *t,struct dh2_character_props *s) {
    if(!t || !s || overlap(s,sizeof(*s),t,sizeof(*t)) || overlap(s,sizeof(*s),t->bytes,t->size))return 1;
    int32_t dummy;return dh2_property_type(t,36,&dummy);
}
uint32_t dh2_health_set(const struct dh2_property_table *t,struct dh2_character_props *s,uint32_t channel,int32_t value) {
    if(channel>1)return 2;
    return dh2_character_props_write(t,s,NULL,0,current(channel),value,DH2_PROPS_SET_INT);
}
uint32_t dh2_health_get(const struct dh2_character_props *s,uint32_t channel,uint32_t total,int32_t *out) {
    if(channel>1 || total>1)return 2;
    return dh2_character_props_get_int(s,total?maximum(channel):current(channel),out);
}
uint32_t dh2_health_validate(const struct dh2_property_table *t,struct dh2_character_props *s) {
    uint32_t status=valid(t,s);if(status)return status;
    struct dh2_character_props next=*s;
    for(uint32_t channel=0;channel<2;++channel) {
        int32_t hp=next.final.values[current(channel)],max=next.final.values[maximum(channel)];
        status=dh2_character_props_write(t,&next,NULL,0,current(channel),hp<max?hp:max,DH2_PROPS_SET);
        if(status)return status;
    }
    *s=next;return 0;
}
uint32_t dh2_health_regen(const struct dh2_property_table *t,struct dh2_character_props *s,uint32_t channel,int32_t delta) {
    if(channel>1)return 2;
    uint32_t status=valid(t,s);if(status)return status;
    int32_t hp=s->final.values[current(channel)],max=s->final.values[maximum(channel)];
    if(delta<0)delta=max;
    if(bits((uint32_t)delta+(uint32_t)hp)>max)delta=bits((uint32_t)max-(uint32_t)hp);
    if(delta>0)return dh2_character_props_write(t,s,NULL,0,current(channel),delta,DH2_PROPS_ADD);
    return 0;
}
uint32_t dh2_health_fraction(const struct dh2_character_props *s,uint32_t channel,float *out) {
    if(!s || !out || overlap(out,4,s,sizeof(*s)))return 1;
    if(channel>1)return 2;
    float numerator=(float)s->final.values[current(channel)],denominator=(float)s->final.values[maximum(channel)];
    if(denominator==0) {
        /* Explicit IEEE zero-divisor values avoid C implementation-dependent
         * floating division traps. NaN payload is not an equivalence claim. */
        uint32_t value=numerator==0?0x7fc00000u:numerator<0?0xff800000u:0x7f800000u;
        memcpy(out,&value,4);
    } else *out=numerator/denominator;
    return 0;
}
uint32_t dh2_health_script_hp(const struct dh2_character_props *s,int32_t out[3]) {
    if(!s || !out || overlap(out,12,s,sizeof(*s)))return 1;
    int32_t result[3]={0,0,0},hp=s->final.values[36],max=s->final.values[38];
    if(max) {
        int32_t whole=sar8(max),product=bits((uint32_t)hp*100u);
        if(!whole || (product==INT32_MIN && whole==-1))return 2;
        result[0]=sar8(hp);result[1]=whole;result[2]=sar8(product/whole);
    }
    memcpy(out,result,12);return 0;
}
uint32_t dh2_health_has_mana(const struct dh2_character_props *s,int32_t cost,uint32_t exempt,int32_t *out) {
    if(!s || !out || overlap(out,4,s,sizeof(*s)))return 1;
    if(exempt>1)return 2;
    *out=exempt || cost<=s->final.values[41];return 0;
}
uint32_t dh2_health_use_mana(const struct dh2_property_table *t,struct dh2_character_props *s,int32_t cost,uint32_t exempt,int32_t *out) {
    uint32_t status=valid(t,s);if(status)return status;
    if(!out || overlap(out,4,s,sizeof(*s)) || overlap(out,4,t,sizeof(*t)) || overlap(out,4,t->bytes,t->size))return 1;
    if(exempt>1)return 2;
    int32_t result=exempt || cost<=s->final.values[41];
    if(result && !exempt) {
        status=dh2_character_props_write(t,s,NULL,0,41,bits(0u-(uint32_t)cost),DH2_PROPS_ADD);
        if(status)return status;
    }
    *out=result;return 0;
}
