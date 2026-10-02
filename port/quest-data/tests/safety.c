#include "../quests.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static uint32_t seed=20261002;
static uint32_t random32(void) {seed=seed*1664525u+1013904223u;return seed;}
int main(void) {
    unsigned char data[178]={0};data[0]=1;
    struct dh2_quest_table t;assert(!dh2_quests_open(&t,data,177));assert(t.count==1);
    struct dh2_quest_record r;assert(!dh2_quests_record(&t,0,&r));
    for(uint32_t i=0;i<12000;++i) {
        unsigned char copy[178];memcpy(copy,data,sizeof(copy));
        uint32_t index=random32()%177;copy[index]^=(unsigned char)(1u<<(random32()%8));
        struct dh2_quest_table out;memset(&out,0xa5,sizeof(out));struct dh2_quest_table before=out;
        uint32_t status=dh2_quests_open(&out,copy,177);
        if(status)assert(!memcmp(&out,&before,sizeof(out)));
        else {assert(out.count<=4096);for(uint32_t j=0;j<out.count;++j)assert(!dh2_quests_record(&out,j,&r));}
        memset(&r,0xa5,sizeof(r));struct dh2_quest_record old=r;
        assert(dh2_quests_record(&t,1,&r));assert(!memcmp(&r,&old,sizeof(r)));
        struct dh2_quest_span span={0xa5a5a5a5,0xa5a5a5a5},oldspan=span;
        assert(dh2_quests_list_record(&t,0,0,0,&span));assert(!memcmp(&span,&oldspan,sizeof(span)));
        assert(dh2_quests_record(&t,0,(void *)&t));
        assert(dh2_quests_open((void *)copy,copy,177));
        assert(dh2_quests_open(&out,copy,random32()%177));
    }
    for(uint32_t size=0;size<177;++size)assert(dh2_quests_open(&t,data,size));
    assert(dh2_quests_open(&t,data,178));assert(dh2_quests_open(NULL,NULL,0));
    puts("QUEST DATA SAFETY PASS 12000");return 0;
}
