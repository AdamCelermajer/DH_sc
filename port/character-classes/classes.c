#include "classes.h"
#include <stddef.h>
#include <string.h>
static int overlap(const void *a,size_t an,const void *b,size_t bn) {
    uintptr_t aa=(uintptr_t)a,bb=(uintptr_t)b;
    return aa+an<aa || bb+bn<bb || (an && bn && aa<bb+bn && bb<aa+an);
}
static uint32_t u32(const unsigned char *p) { return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24); }
static int32_t bits(uint32_t n) { int32_t v;memcpy(&v,&n,4);return v; }
static int32_t sar8(uint32_t n) { return bits((n>>8)|((n&0x80000000)?0xff000000:0)); }
static int valid(const struct dh2_class_table *t) {
    if(!t || !t->bytes || t->size<4 || t->size>4*1024*1024 || t->count!=u32(t->bytes) || t->count>4096)return 0;
    uint32_t at=4,total=0;
    for(uint32_t i=0;i<t->count;++i) {
        if(t->size-at<4)return 0;
        uint32_t n=u32(t->bytes+at);at+=4;
        if(n>65536-total || n>(t->size-at)/20)return 0;
        total+=n;at+=n*20;
    }
    return at==t->size;
}
uint32_t dh2_class_open(struct dh2_class_table *out,const void *bytes,uint32_t size) {
    if(!out || !bytes || overlap(out,sizeof(*out),bytes,size))return 1;
    if(size<4)return 2;
    struct dh2_class_table t={bytes,size,u32(bytes)};
    if(!valid(&t))return 2;
    *out=t;return 0;
}
struct context {
    const struct dh2_class_table *classes;
    const struct dh2_property_table *props;
    struct dh2_character_props *state;
    struct dh2_property_sheet *target;
    uint32_t temporary,from_final,budget;
};
static uint32_t apply(struct context *c,int32_t id,uint32_t depth) {
    if(id<0 || (uint32_t)id>=c->classes->count)return 0;
    if(depth>=64 || !c->budget--)return 2;
    uint32_t at=4;
    for(int32_t i=0;i<id;++i)at+=4+u32(c->classes->bytes+at)*20;
    uint32_t count=u32(c->classes->bytes+at);const unsigned char *p=c->classes->bytes+at+4;
    for(uint32_t i=0;i<count;++i,p+=20) {
        if(!c->budget--)return 2;
        int32_t field=bits(u32(p)),op=bits(u32(p+4)),a=bits(u32(p+8)),b=bits(u32(p+12)),d=bits(u32(p+16));
        if(op==0) {
            int32_t ids[]={a,b,d};
            for(unsigned j=0;j<3;++j)if(ids[j]!=-1) { uint32_t status=apply(c,ids[j],depth+1);if(status)return status; }
            continue;
        }
        if(op==8)return 0; /* Original helper is empty; dispatch returns early. */
        if(op==3 || op<0 || op>9)continue;
        if(field<0 || field>=224)return 2;
        int32_t v=c->target->values[field];
        if(op==1) {
            if(b<0 || b>=224)return 2;
            int32_t source=c->target->values[b];
            if(a==-666)a=v;
            if(c->from_final)source=c->state->final.values[b];
            else if(!c->temporary) {
                struct dh2_property_inputs in={&c->state->base,&c->state->saved,&c->state->gears,NULL,0};
                uint32_t status=dh2_property_recalc(c->props,&in,(uint32_t)b,&c->state->final);if(status)return status;
                source=c->state->final.values[b];
            }
            v=bits((uint32_t)d*(uint32_t)sar8((uint32_t)source)+(uint32_t)a);
        } else if(op==2) v=v<a?a:(v>=b?b:v);
        else if(op==4 || op==5 || op==6) {
            if(a<0 || a>=224)return 2;
            int32_t factor=c->target->values[a];
            if(op==4)v=bits((uint32_t)v+(uint32_t)factor);
            else {
                if(op==6 && c->from_final)v=c->state->final.values[field];
                v=sar8((uint32_t)factor*(uint32_t)v);
                if(op==6)v=bits((uint32_t)v+(uint32_t)a); /* Real dispatch falls through AddValue. */
            }
        } else if(op==7)v=bits((uint32_t)v+(uint32_t)a);
        else if(op==9)v=a;
        c->target->values[field]=v;
    }
    return 0;
}
uint32_t dh2_class_apply(const struct dh2_class_table *ct,const struct dh2_property_table *pt,struct dh2_character_props *s,uint32_t target,struct dh2_property_sheet *temp,int32_t id,uint32_t from_final) {
    if(!ct || !pt || !s || target>4 || from_final>1 || (target==4 && !temp))return 1;
    const void *inputs[]={ct,ct->bytes,pt,pt->bytes};size_t sizes[]={sizeof(*ct),ct->size,sizeof(*pt),pt->size};
    for(unsigned i=0;i<4;++i)if(overlap(s,sizeof(*s),inputs[i],sizes[i]) || (target==4 && overlap(temp,sizeof(*temp),inputs[i],sizes[i])))return 1;
    if(target==4 && overlap(s,sizeof(*s),temp,sizeof(*temp)))return 1;
    if(!valid(ct))return 2;
    struct dh2_property_sheet verify;if(dh2_property_reset(pt,&verify))return 2;
    struct dh2_character_props next=*s;struct dh2_property_sheet tmp;
    if(target==4)tmp=*temp;
    struct dh2_property_sheet *sheets[]={&next.base,&next.saved,&next.gears,&next.final,&tmp};
    struct context c={ct,pt,&next,sheets[target],target==4,from_final,100000};
    uint32_t status=apply(&c,id,0);if(status)return status;
    *s=next;if(target==4)*temp=tmp;return 0;
}
