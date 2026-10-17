#include "properties.h"
#include <stddef.h>
#include <string.h>
static int overlap(const void *a,size_t an,const void *b,size_t bn) {
    uintptr_t aa=(uintptr_t)a,bb=(uintptr_t)b;
    return aa+an<aa || bb+bn<bb || (an && bn && aa<bb+bn && bb<aa+an);
}
static uint32_t u32(const unsigned char *p) {
    return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
}
static int32_t signed_bits(uint32_t bits) { int32_t value;memcpy(&value,&bits,4);return value; }
static int walk(struct dh2_property_table *v) {
    if(!v->bytes || v->size>4*1024*1024 || v->size<4)return 0;
    uint32_t at=0;
    for(unsigned t=0;t<3;++t) {
        if(at>v->size || v->size-at<4)return 0;
        uint32_t count=u32(v->bytes+at);at+=4;v->counts[t]=count;v->offsets[t]=at;
        if(t==0) {
            if(count<2 || count>(v->size-at)/896)return 0;
            at+=count*896;
        } else {
            if(count>(v->size-at)/4)return 0;
            for(uint32_t i=0;i<count;++i) {
                if(v->size-at<4)return 0;
                uint32_t n=u32(v->bytes+at),width=t==1?12:4;at+=4;
                if(n>(v->size-at)/width)return 0;
                at+=n*width;
            }
        }
    }
    return at==v->size;
}
static int valid(const struct dh2_property_table *v) {
    if(!v)return 0;
    struct dh2_property_table copy=*v;
    return walk(&copy) && !memcmp(copy.counts,v->counts,sizeof(v->counts)) &&
           !memcmp(copy.offsets,v->offsets,sizeof(v->offsets));
}
static int table_output(const struct dh2_property_table *v,void *out,size_t size) {
    return v && out && !overlap(out,size,v,sizeof(*v)) && !overlap(out,size,v->bytes,v->size);
}
uint32_t dh2_property_open(struct dh2_property_table *out,const void *bytes,uint32_t size) {
    if(!out || !bytes || overlap(out,sizeof(*out),bytes,size))return 1;
    struct dh2_property_table value={bytes,size,{0,0,0},{0,0,0}};
    if(!walk(&value))return 2;
    *out=value;return 0;
}
uint32_t dh2_property_load(const struct dh2_property_table *v,uint32_t row,struct dh2_property_sheet *out) {
    if(!table_output(v,out,sizeof(*out)))return 1;
    if(!valid(v) || row>=v->counts[0])return 2;
    struct dh2_property_sheet value;
    const unsigned char *p=v->bytes+v->offsets[0]+row*896;
    for(unsigned i=0;i<DH2_PROPERTY_COUNT;++i)value.values[i]=signed_bits(u32(p+4*i));
    *out=value;return 0;
}
uint32_t dh2_property_get(const struct dh2_property_sheet *s,uint32_t id,int32_t *out) {
    if(!s || !out || overlap(out,4,s,sizeof(*s)))return 1;
    if(id>=DH2_PROPERTY_COUNT)return 2;
    *out=s->values[id];return 0;
}
uint32_t dh2_property_set(struct dh2_property_sheet *s,uint32_t id,int32_t value) {
    if(!s)return 1;
    if(id>=DH2_PROPERTY_COUNT)return 2;
    s->values[id]=value;return 0;
}
static uint32_t table_value(const struct dh2_property_table *v,uint32_t row,uint32_t id,int32_t *out) {
    if(!table_output(v,out,4))return 1;
    if(!valid(v) || id>=DH2_PROPERTY_COUNT)return 2;
    *out=signed_bits(u32(v->bytes+v->offsets[0]+row*896+id*4));return 0;
}
uint32_t dh2_property_default(const struct dh2_property_table *v,uint32_t id,int32_t *out) {
    return table_value(v,0,id,out);
}
uint32_t dh2_property_type(const struct dh2_property_table *v,uint32_t id,int32_t *out) {
    int32_t value;
    if(!table_output(v,out,4))return 1;
    uint32_t status=table_value(v,1,id,&value);
    if(status)return status;
    *out=value==-1?16:value;return 0;
}
uint32_t dh2_property_reset(const struct dh2_property_table *v,struct dh2_property_sheet *s) {
    return dh2_property_load(v,0,s);
}
uint32_t dh2_property_is_set(const struct dh2_property_table *v,const struct dh2_property_sheet *s,uint32_t id,int32_t *out) {
    if(!s || !out || overlap(out,4,s,sizeof(*s)) || !table_output(v,out,4))return 1;
    int32_t base;uint32_t status=dh2_property_default(v,id,&base);
    if(status)return status;
    *out=s->values[id]!=base;return 0;
}
uint32_t dh2_property_add(const struct dh2_property_table *v,struct dh2_property_sheet *s,uint32_t id,int32_t delta) {
    if(!table_output(v,s,sizeof(*s)))return 1;
    int32_t base;uint32_t status=dh2_property_default(v,id,&base);
    if(status)return status;
    int32_t current=s->values[id];
    s->values[id]=current==base?delta:signed_bits((uint32_t)current+(uint32_t)delta);
    return 0;
}
