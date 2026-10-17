#include "state.h"
#include <stddef.h>
#include <string.h>
static int overlap(const void *a,size_t an,const void *b,size_t bn) {
    uintptr_t aa=(uintptr_t)a,bb=(uintptr_t)b;
    return aa+an<aa || bb+bn<bb || (an && bn && aa<bb+bn && bb<aa+an);
}
static int32_t signed_bits(uint32_t bits) { int32_t value;memcpy(&value,&bits,4);return value; }
uint32_t dh2_character_props_init(const struct dh2_property_table *t,struct dh2_character_props *out) {
    if(!t || !out || overlap(out,sizeof(*out),t,sizeof(*t)) || overlap(out,sizeof(*out),t->bytes,t->size))return 1;
    struct dh2_character_props value;uint32_t status=dh2_property_reset(t,&value.base);
    if(status)return status;
    value.saved=value.gears=value.final=value.base;*out=value;return 0;
}
uint32_t dh2_character_props_write(const struct dh2_property_table *t,struct dh2_character_props *state,const struct dh2_property_group *groups,uint32_t count,uint32_t id,int32_t delta,uint32_t op) {
    if(!t || !state || (!groups && count))return 1;
    if(id>=224 || count>32 || op>3)return 2;
    if(overlap(state,sizeof(*state),t,sizeof(*t)) || overlap(state,sizeof(*state),t->bytes,t->size) ||
       overlap(state,sizeof(*state),groups,count*sizeof(*groups)))return 1;
    uint32_t total=0;
    for(uint32_t g=0;g<count;++g) {
        if(groups[g].count>64 || total+groups[g].count>1024)return 2;
        if((!groups[g].sheets && groups[g].count) || overlap(state,sizeof(*state),groups[g].sheets,groups[g].count*sizeof(*groups[g].sheets)))return 1;
        total+=groups[g].count;
        for(uint32_t j=0;j<groups[g].count;++j)if(!groups[g].sheets[j] || overlap(state,sizeof(*state),groups[g].sheets[j],sizeof(struct dh2_property_sheet)))return 1;
    }
    int32_t type;uint32_t status=dh2_property_type(t,id,&type);if(status)return status;
    if(op>=2)delta=signed_bits((uint32_t)delta<<8);
    struct dh2_character_props value=*state;
    struct dh2_property_inputs inputs={&value.base,&value.saved,&value.gears,groups,count};
    /* Validate the complete borrowed input before committing a mutation. */
    struct dh2_property_sheet probe=value.final;status=dh2_property_recalc(t,&inputs,id,&probe);if(status)return status;
    if(type&32) {
        value.saved.values[id]=(op&1)?signed_bits((uint32_t)value.saved.values[id]+(uint32_t)delta):delta;
        status=dh2_property_recalc(t,&inputs,id,&value.final);if(status)return status;
    } else if(type&8)value.final.values[id]=(op&1)?signed_bits((uint32_t)value.final.values[id]+(uint32_t)delta):delta;
    *state=value;return 0;
}
uint32_t dh2_character_props_get_int(const struct dh2_character_props *state,uint32_t id,int32_t *out) {
    if(!state || !out || overlap(out,4,state,sizeof(*state)))return 1;
    if(id>=224)return 2;
    uint32_t bits=(uint32_t)state->final.values[id];
    uint32_t shifted=bits>>8;if(bits&0x80000000u)shifted|=0xff000000u;
    *out=signed_bits(shifted);return 0;
}
