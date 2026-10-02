#include "loot.h"
#include <stddef.h>
#include <string.h>
static int overlap(const void *a,size_t an,const void *b,size_t bn) {
    uintptr_t aa=(uintptr_t)a,bb=(uintptr_t)b;
    return aa+an<aa || bb+bn<bb || (an && bn && aa<bb+bn && bb<aa+an);
}
static uint32_t u32(const unsigned char *p) {
    return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
}
static int32_t i32(const unsigned char *p) { uint32_t b=u32(p);int32_t v;memcpy(&v,&b,4);return v; }
struct reader { const unsigned char *bytes;uint32_t at,size; };
static int skip(struct reader *r,uint32_t count) {
    if(r->at>r->size || count>r->size-r->at)return 0;
    r->at+=count;return 1;
}
static int list(struct reader *r,uint32_t width) {
    if(r->at>r->size || r->size-r->at<4)return 0;
    uint32_t n=u32(r->bytes+r->at);r->at+=4;
    if(n>(r->size-r->at)/width)return 0;
    return skip(r,n*width);
}
static int record(struct reader *r,uint32_t t) {
    switch(t) {
    case DH2_DROP_TILE:case DH2_NUM_PROBS:return list(r,4);
    case DH2_INVENTORY:return skip(r,2);
    case DH2_ITEM_LIST:return list(r,7);
    case DH2_ITEMS:return list(r,1) && skip(r,61) && list(r,1) && skip(r,80);
    case DH2_ITEM_TYPES:return list(r,1);
    case DH2_LOOT:return skip(r,8) && list(r,32) && list(r,32) && list(r,4);
    case DH2_MERCHANTS:return skip(r,8) && list(r,8);
    default:return 0;
    }
}
static int walk(struct dh2_loot_tables *v) {
    if(!v->bytes || v->size<32 || v->size>4*1024*1024)return 0;
    struct reader r={v->bytes,0,v->size};
    for(uint32_t t=0;t<8;++t) {
        if(r.size-r.at<4)return 0;
        uint32_t n=u32(r.bytes+r.at);r.at+=4;
        if(n>65536)return 0;
        v->counts[t]=n;v->offsets[t]=r.at;
        for(uint32_t i=0;i<n;++i)if(!record(&r,t))return 0;
    }
    return r.at==r.size;
}
static int valid(const struct dh2_loot_tables *v) {
    if(!v)return 0;
    struct dh2_loot_tables c=*v;
    return walk(&c) && !memcmp(c.counts,v->counts,sizeof(c.counts)) && !memcmp(c.offsets,v->offsets,sizeof(c.offsets));
}
static int output(const struct dh2_loot_tables *v,void *p,size_t n) {
    return v && p && !overlap(p,n,v,sizeof(*v)) && !overlap(p,n,v->bytes,v->size);
}
uint32_t dh2_loot_open(struct dh2_loot_tables *out,const void *bytes,uint32_t size) {
    if(!out || !bytes || overlap(out,sizeof(*out),bytes,size))return 1;
    struct dh2_loot_tables v={bytes,size,{0},{0}};
    if(!walk(&v))return 2;
    *out=v;return 0;
}
uint32_t dh2_loot_record(const struct dh2_loot_tables *v,uint32_t t,uint32_t index,struct dh2_loot_span *out) {
    if(!output(v,out,sizeof(*out)))return 1;
    if(!valid(v) || t>=8 || index>=v->counts[t])return 2;
    struct reader r={v->bytes,v->offsets[t],v->size};
    for(uint32_t i=0;i<index;++i)if(!record(&r,t))return 2;
    uint32_t start=r.at;
    if(!record(&r,t))return 2;
    struct dh2_loot_span span={v->bytes+start,r.at-start};*out=span;return 0;
}
uint32_t dh2_loot_item_read(const struct dh2_loot_tables *v,uint32_t index,struct dh2_loot_item *out) {
    if(!output(v,out,sizeof(*out)))return 1;
    struct dh2_loot_span span;uint32_t status=dh2_loot_record(v,DH2_ITEMS,index,&span);
    if(status)return status;
    struct dh2_loot_item item;memset(&item,0,sizeof(item));
    const unsigned char *p=span.bytes;
    item.name.size=u32(p);item.name.bytes=p+4;p+=4+item.name.size;
    for(unsigned i=0;i<4;++i)item.base_ints[i]=i32(p+4*i);
    item.base_bool=p[16];p+=17;
    for(unsigned i=0;i<9;++i)item.base_float_bits[i]=u32(p+4*i);
    p+=36;
    for(unsigned i=0;i<2;++i)item.item_ints[i]=i32(p+4*i);
    p+=8;item.description.size=u32(p);item.description.bytes=p+4;p+=4+item.description.size;
    for(unsigned i=0;i<20;++i)item.tail_ints[i]=i32(p+4*i);
    *out=item;return 0;
}
