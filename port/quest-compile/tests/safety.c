#include "../compile.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
static uint32_t seed=20261002;
static uint32_t random32(void) {seed=seed*1664525u+1013904223u;return seed;}
static int32_t random_signed(void) {uint32_t x=random32();int32_t y;memcpy(&y,&x,4);return y;}
int main(void) {
    for(uint32_t i=0;i<12000;++i) {
        struct dh2_quest_compiled s={{random_signed(),random_signed(),random_signed(),i&255},(i>>2)&255};
        struct dh2_quest_compile_context context={i&3,random_signed(),i&1?-1:7,i&2?7:9,random_signed(),random_signed()};
        struct dh2_quest_compile_result result;
        assert(!dh2_quest_compile(&s,&context,&result));
        struct dh2_quest_compiled before=s;memset(&result,0xa5,sizeof(result));struct dh2_quest_compile_result old=result;
        context.kind=4;assert(dh2_quest_compile(&s,&context,&result)==2);context.kind=i&3;
        s.active=256;assert(dh2_quest_compile(&s,&context,&result)==2);s=before;
        s.progress.completed=256;assert(dh2_quest_compile(&s,&context,&result)==2);s=before;
        assert(dh2_quest_compile(&s,(void *)&s,&result)==1);
        assert(dh2_quest_compile(&s,&context,(void *)&s)==1);
        assert(dh2_quest_compile(&s,&context,(void *)&context)==1);
        assert(!memcmp(&s,&before,sizeof(s)) && !memcmp(&result,&old,sizeof(result)));
        struct dh2_quest_character chars[2]={{1,7,9},{0,0,0}};
        struct dh2_quest_cache_entry cache[2]={{7,random_signed()},{9,random_signed()}};
        int32_t out=123;
        assert(!dh2_quest_population(0,7,chars,2,cache,2,&out) && out==1);
        chars[0].present=0;assert(!dh2_quest_population(0,7,chars,2,cache,2,&out) && out==cache[0].quantity);
        assert(!dh2_quest_population(1,7,chars,2,cache,2,&out) && out==0);out=123;
        chars[0].present=256;assert(dh2_quest_population(0,7,chars,2,cache,2,&out)==2 && out==123);chars[0].present=1;
        chars[0].property_id=32768;assert(dh2_quest_population(0,7,chars,2,cache,2,&out)==2 && out==123);chars[0].property_id=7;
        cache[1].property_id=7;assert(dh2_quest_population(0,7,chars,2,cache,2,&out)==2 && out==123);cache[1].property_id=9;
        assert(dh2_quest_population(0,7,chars,2,cache,2,(void *)chars)==1);
        assert(dh2_quest_population(0,7,chars,2,cache,2,(void *)cache)==1);
        assert(dh2_quest_population(0,7,chars,4097,cache,2,&out)==2 && out==123);
    }
    assert(dh2_quest_compile(NULL,NULL,NULL)==1);
    assert(dh2_quest_population(0,0,NULL,1,NULL,0,NULL)==1);
    puts("QUEST COMPILE SAFETY PASS 12000");return 0;
}
