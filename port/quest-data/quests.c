#include "quests.h"
#include <stddef.h>
#include <string.h>
#define MAX_BYTES (4u*1024u*1024u)
#define MAX_ROWS 4096u
struct cursor {const unsigned char *bytes;uint32_t size,at;};
static int overlap(const void *a,size_t an,const void *b,size_t bn) {
    uintptr_t aa=(uintptr_t)a,bb=(uintptr_t)b;
    return aa+an<aa || bb+bn<bb || (an && bn && aa<bb+bn && bb<aa+an);
}
static int take(struct cursor *c,uint32_t size,uint32_t *offset) {
    if(c->at>c->size || size>c->size-c->at)return 0;
    if(offset)*offset=c->at;
    c->at+=size;return 1;
}
static int word(struct cursor *c,uint32_t *out) {
    uint32_t at;if(!take(c,4,&at))return 0;
    const unsigned char *p=c->bytes+at;
    *out=(uint32_t)p[0]|(uint32_t)p[1]<<8|(uint32_t)p[2]<<16|(uint32_t)p[3]<<24;return 1;
}
static int integer(struct cursor *c,int32_t *out) {uint32_t bits;if(!word(c,&bits))return 0;memcpy(out,&bits,4);return 1;}
static int string(struct cursor *c,struct dh2_quest_span *out) {
    if(!word(c,&out->size) || !take(c,out->size,&out->offset))return 0;
    return 1;
}
static int objective(struct cursor *c,struct dh2_quest_objective *out) {
    for(uint32_t i=0;i<3;++i)if(!integer(c,&out->common[i]))return 0;
    for(uint32_t i=0;i<2;++i)if(!string(c,&out->strings[i]))return 0;
    for(uint32_t i=0;i<3;++i)if(!integer(c,&out->args[i]))return 0;
    return 1;
}
static int list_entry(struct cursor *c,uint32_t list) {
    if(list==1) {struct dh2_quest_objective unused;return objective(c,&unused);}
    return take(c,12,NULL);
}
static int record(struct cursor *c,struct dh2_quest_record *out) {
    memset(out,0,sizeof(*out));
    for(uint32_t i=0;i<4;++i)if(!integer(c,&out->ids[i]))return 0;
    for(uint32_t list=0;list<5;++list) {
        if(!word(c,&out->lists[list].count) || out->lists[list].count>MAX_ROWS)return 0;
        out->lists[list].offset=c->at;
        for(uint32_t i=0;i<out->lists[list].count;++i)if(!list_entry(c,list))return 0;
    }
    uint32_t at;
    if(!objective(c,&out->accept) || !objective(c,&out->end) || !integer(c,&out->target_level) || !take(c,1,&at))return 0;
    out->repeatable=c->bytes[at];if(!integer(c,&out->state))return 0;
    for(uint32_t i=0;i<14;++i)if(!string(c,&out->scripts[i]))return 0;
    return integer(c,&out->priority) && integer(c,&out->act);
}
static int validate(struct dh2_quest_table *t) {
    if(t->size<4 || t->size>MAX_BYTES)return 0;
    struct cursor c={t->bytes,t->size,0};uint32_t count;
    if(!word(&c,&count) || count>MAX_ROWS)return 0;
    for(uint32_t i=0;i<count;++i) {struct dh2_quest_record unused;if(!record(&c,&unused))return 0;}
    if(c.at!=c.size)return 0;
    t->count=count;return 1;
}
uint32_t dh2_quests_open(struct dh2_quest_table *out,const void *bytes,uint32_t size) {
    if(!out || !bytes || overlap(out,sizeof(*out),bytes,size))return 1;
    struct dh2_quest_table t={(const unsigned char *)bytes,size,0};if(!validate(&t))return 2;
    *out=t;return 0;
}
static uint32_t check(const struct dh2_quest_table *t,const void *out,size_t size) {
    if(!t || !t->bytes || !out || overlap(t,sizeof(*t),out,size) || overlap(t->bytes,t->size,out,size))return 1;
    struct dh2_quest_table next=*t;return !validate(&next) || next.count!=t->count?2:0;
}
static uint32_t read_record(const struct dh2_quest_table *t,uint32_t row,struct dh2_quest_record *out) {
    if(row>=t->count)return 2;
    struct cursor c={t->bytes,t->size,4};
    for(uint32_t i=0;i<=row;++i)if(!record(&c,out))return 2;
    return 0;
}
uint32_t dh2_quests_record(const struct dh2_quest_table *t,uint32_t row,struct dh2_quest_record *out) {
    uint32_t status=check(t,out,sizeof(*out));if(status)return status;
    struct dh2_quest_record next;status=read_record(t,row,&next);if(status)return status;*out=next;return 0;
}
uint32_t dh2_quests_list_record(const struct dh2_quest_table *t,uint32_t row,uint32_t list,uint32_t index,struct dh2_quest_span *out) {
    uint32_t status=check(t,out,sizeof(*out));if(status)return status;
    struct dh2_quest_record r;if(list>=5 || read_record(t,row,&r) || index>=r.lists[list].count)return 2;
    struct cursor c={t->bytes,t->size,r.lists[list].offset};
    for(uint32_t i=0;i<index;++i)if(!list_entry(&c,list))return 2;
    struct dh2_quest_span next={c.at,0};if(!list_entry(&c,list))return 2;next.size=c.at-next.offset;*out=next;return 0;
}
uint32_t dh2_quests_objective(const struct dh2_quest_table *t,uint32_t row,uint32_t index,struct dh2_quest_objective *out) {
    uint32_t status=check(t,out,sizeof(*out));if(status)return status;
    struct dh2_quest_span span;status=dh2_quests_list_record(t,row,1,index,&span);if(status)return status;
    struct cursor c={t->bytes,t->size,span.offset};struct dh2_quest_objective next;
    if(!objective(&c,&next))return 2;
    *out=next;return 0;
}
