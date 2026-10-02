#include "gears.h"
#include <stddef.h>
#include <string.h>
static int overlap(const void *a,size_t an,const void *b,size_t bn) {
    uintptr_t x=(uintptr_t)a,y=(uintptr_t)b;
    return x+an<x || y+bn<y || (an && bn && x<y+bn && y<x+an);
}
static uint32_t u32(const unsigned char *p) {return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static int32_t bits(uint32_t b) {int32_t s;memcpy(&s,&b,4);return s;}
static int walk(struct dh2_power_tables *v) {
    if(!v->bytes || v->size>4*1024*1024 || v->size<8)return 0;
    uint32_t at=0;
    for(uint32_t t=0;t<2;++t) {
        if(v->size-at<4)return 0;
        uint32_t n=u32(v->bytes+at);at+=4;
        if(n>65536)return 0;
        v->counts[t]=n;v->offsets[t]=at;
        for(uint32_t i=0;i<n;++i) {
            uint32_t head=t?9:4,width=t?12:5;
            if(v->size-at<head)return 0;
            uint32_t count=u32(v->bytes+at+(t?5:0));at+=head;
            if(count>65536 || count>(v->size-at)/width)return 0;
            at+=count*width;
            if(t) {if(v->size-at<20)return 0;at+=20;}
        }
    }
    return at==v->size;
}
static int valid(const struct dh2_power_tables *v) {
    if(!v)return 0;
    struct dh2_power_tables x=*v;
    return walk(&x) && !memcmp(x.counts,v->counts,sizeof(x.counts)) && !memcmp(x.offsets,v->offsets,sizeof(x.offsets));
}
static int output_ok(const void *o,size_t n,const void *v,size_t vn,const void *b,size_t bn) {return o && v && !overlap(o,n,v,vn) && !overlap(o,n,b,bn);}
uint32_t dh2_power_open(struct dh2_power_tables *v,const void *b,uint32_t n) {
    if(!b || !output_ok(v,sizeof(*v),b,n,NULL,0))return 1;
    struct dh2_power_tables x={b,n,{0,0},{0,0}};
    if(!walk(&x))return 2;
    *v=x;return 0;
}
uint32_t dh2_power_read(const struct dh2_power_tables *v,uint32_t id,struct dh2_power_record *out) {
    if(!v || !output_ok(out,sizeof(*out),v,sizeof(*v),v->bytes,v->size))return 1;
    if(!valid(v) || id>=v->counts[1])return 2;
    uint32_t at=v->offsets[1];
    for(uint32_t i=0;i<id;++i)at+=29+u32(v->bytes+at+5)*12;
    const unsigned char *p=v->bytes+at;
    struct dh2_power_record x;memset(&x,0,sizeof(x));
    x.type=p[0]>=128?(int32_t)p[0]-256:(int32_t)p[0];x.value=bits(u32(p+1));x.count=u32(p+5);x.stats=p+9;
    for(uint32_t i=0;i<5;++i)x.tail[i]=bits(u32(p+9+x.count*12+i*4));
    *out=x;return 0;
}
static int property_output(const struct dh2_property_table *t,void *out,size_t n) {return t && output_ok(out,n,t,sizeof(*t),t->bytes,t->size);}
uint32_t dh2_gear_add_item(const struct dh2_property_table *t,const struct dh2_loot_tables *loot,struct dh2_property_sheet *s,uint32_t id,uint32_t left) {
    if(!loot || !property_output(t,s,sizeof(*s)) || !output_ok(s,sizeof(*s),loot,sizeof(*loot),loot->bytes,loot->size))return 1;
    if(left>1)return 2;
    struct dh2_loot_item item;uint32_t rc=dh2_loot_item_read(loot,id,&item);if(rc)return rc;
    struct dh2_property_sheet x=*s,verify;rc=dh2_property_reset(t,&verify);if(rc)return rc;
    int32_t type=item.tail_ints[1];uint32_t ids[3],n=0;int32_t values[3];
    if(type>=0 && type<=5) {
        uint32_t off=type==1 || ((type==0 || type==3) && left)?81:79;
        ids[n]=off;values[n++]=item.tail_ints[14];ids[n]=off+1;values[n++]=item.tail_ints[15];
        if(type==4 || type==5) {ids[n]=97;values[n++]=bits((uint32_t)item.tail_ints[4]<<8);}
    } else if(type==6) {ids[n]=71;values[n++]=item.tail_ints[14];ids[n]=61;values[n++]=item.tail_ints[15];}
    else if((type>=7 && type<=10) || type==12) {ids[n]=71;values[n++]=item.tail_ints[14];}
    for(uint32_t i=0;i<n;++i) {rc=dh2_property_add(t,&x,ids[i],values[i]);if(rc)return rc;}
    *s=x;return 0;
}
uint32_t dh2_gear_add_power(const struct dh2_property_table *t,const struct dh2_power_tables *powers,struct dh2_property_sheet *s,uint32_t id,uint32_t left) {
    if(!powers || !property_output(t,s,sizeof(*s)) || !output_ok(s,sizeof(*s),powers,sizeof(*powers),powers->bytes,powers->size))return 1;
    if(left>1)return 2;
    struct dh2_power_record p;uint32_t rc=dh2_power_read(powers,id,&p);if(rc)return rc;
    struct dh2_property_sheet x=*s,verify;rc=dh2_property_reset(t,&verify);if(rc)return rc;
    static const uint32_t fields[]={149,150,151,152,38,43,0,0,0,0,0,0,0,0,0,0,0,0,0,39,44,40,45,63,60,61,71,0,50,59,74,77,75,78,76,0,132,133,165,158,0,170,167,168,169,166,195,196};
    for(uint32_t i=0;i<p.count;++i) {
        const unsigned char *q=p.stats+i*12;int32_t op=bits(u32(q)),v=bits(u32(q+4));uint32_t ids[5],n=0;
        if(op==0) {for(uint32_t j=149;j<=152;++j)ids[n++]=j;}
        else if(op>=1 && op<=6)ids[n++]=fields[op-1];
        else if(op>=7 && op<=9)ids[n++]=95+(uint32_t)(op-7)+(left?3:0);
        else if(op>=10 && op<=19) {uint32_t k=(uint32_t)(op-10);ids[n++]=101+(k/2)*4+(k%2)+(left?2:0);}
        else if(op==28) {ids[n++]=left?81:79;ids[n++]=left?82:80;}
        else if(op==36) {uint32_t f[]={74,77,75,78,76};memcpy(ids,f,sizeof(f));n=5;}
        else if(op==41) {uint32_t f[]={166,169,167,170,168};memcpy(ids,f,sizeof(f));n=5;}
        else if(op>=20 && op<=48)ids[n++]=fields[op-1];
        for(uint32_t j=0;j<n;++j) {
            rc=op==9?dh2_property_set(&x,ids[j],v):dh2_property_add(t,&x,ids[j],v);
            if(rc)return rc;
        }
    }
    *s=x;return 0;
}
static uint32_t entries_ok(const struct dh2_gear_entry *e,uint32_t n,const void *out,size_t bytes) {
    if(n>64)return 2;
    if((!e && n) || overlap(out,bytes,e,n*sizeof(*e)))return 1;
    uint32_t total=0;
    for(uint32_t i=0;i<n;++i) {
        if(e[i].power_count>4096 || total+e[i].power_count>4096 || e[i].item_id<-1)return 2;
        if((!e[i].power_ids && e[i].power_count) || overlap(out,bytes,e[i].power_ids,e[i].power_count*4))return 1;
        total+=e[i].power_count;
    }
    return 0;
}
uint32_t dh2_gear_load(const struct dh2_property_table *t,const struct dh2_loot_tables *loot,const struct dh2_power_tables *powers,struct dh2_property_sheet *s,const struct dh2_gear_entry *e,uint32_t n) {
    if(!loot || !powers || !property_output(t,s,sizeof(*s)) || !output_ok(s,sizeof(*s),loot,sizeof(*loot),loot->bytes,loot->size) || !output_ok(s,sizeof(*s),powers,sizeof(*powers),powers->bytes,powers->size))return 1;
    uint32_t rc=entries_ok(e,n,s,sizeof(*s));if(rc)return rc;
    struct dh2_property_sheet x=*s,verify;rc=dh2_property_reset(t,&verify);if(rc)return rc;
    if(!valid(powers))return 2;
    /* Validate loot even for an empty inventory. */
    struct dh2_loot_tables lv;rc=dh2_loot_open(&lv,loot->bytes,loot->size);if(rc)return rc;
    if(memcmp(lv.counts,loot->counts,sizeof(lv.counts)) || memcmp(lv.offsets,loot->offsets,sizeof(lv.offsets)))return 2;
    for(uint32_t i=0;i<n;++i)if(e[i].item_id!=-1) {
        rc=dh2_gear_add_item(t,loot,&x,(uint32_t)e[i].item_id,i==2);if(rc)return rc;
        for(uint32_t j=0;j<e[i].power_count;++j) {rc=dh2_gear_add_power(t,powers,&x,(uint32_t)e[i].power_ids[j],i==2);if(rc)return rc;}
    }
    *s=x;return 0;
}
static int state_output(const struct dh2_property_table *t,const struct dh2_class_table *ct,struct dh2_character_props *s) {return ct && property_output(t,s,sizeof(*s)) && output_ok(s,sizeof(*s),ct,sizeof(*ct),ct->bytes,ct->size);}
uint32_t dh2_character_recalc(const struct dh2_property_table *t,const struct dh2_class_table *ct,struct dh2_character_props *s,uint32_t apply_class) {
    if(!state_output(t,ct,s))return 1;
    if(apply_class>1)return 2;
    struct dh2_character_props x=*s;uint32_t rc;
    if(apply_class) {rc=dh2_class_apply(ct,t,&x,0,NULL,x.base.values[26],0);if(rc)return rc;}
    struct dh2_property_inputs in={&x.base,&x.saved,&x.gears,NULL,0};
    for(uint32_t i=0;i<224;++i) {rc=dh2_property_recalc(t,&in,i,&x.final);if(rc)return rc;}
    *s=x;return 0;
}
uint32_t dh2_character_update_base(const struct dh2_property_table *t,const struct dh2_class_table *ct,struct dh2_character_props *s,int32_t row) {
    if(!state_output(t,ct,s))return 1;
    struct dh2_character_props x=*s;uint32_t rc=dh2_property_reset(t,&x.base);if(rc)return rc;
    if(row>=0 && (uint32_t)row<t->counts[0]) {rc=dh2_property_load(t,(uint32_t)row,&x.base);if(rc)return rc;}
    rc=dh2_character_recalc(t,ct,&x,1);if(rc)return rc;
    *s=x;return 0;
}
uint32_t dh2_character_update_gears(const struct dh2_property_table *t,const struct dh2_class_table *ct,const struct dh2_loot_tables *loot,const struct dh2_power_tables *powers,struct dh2_character_props *s,const struct dh2_gear_entry *e,uint32_t n) {
    if(!state_output(t,ct,s) || !loot || !powers || !output_ok(s,sizeof(*s),loot,sizeof(*loot),loot->bytes,loot->size) || !output_ok(s,sizeof(*s),powers,sizeof(*powers),powers->bytes,powers->size))return 1;
    uint32_t rc=entries_ok(e,n,s,sizeof(*s));if(rc)return rc;
    struct dh2_character_props x=*s;rc=dh2_property_reset(t,&x.gears);if(rc)return rc;
    rc=dh2_gear_load(t,loot,powers,&x.gears,e,n);if(rc)return rc;
    rc=dh2_character_recalc(t,ct,&x,1);if(rc)return rc;
    *s=x;return 0;
}
