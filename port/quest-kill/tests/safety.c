#include "../quest.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
static uint32_t seed=20261002;
static uint32_t random32(void) {seed=seed*1664525u+1013904223u;return seed;}
static int32_t random_signed(void) {uint32_t x=random32();int32_t y;memcpy(&y,&x,4);return y;}
int main(void) {
    for(uint32_t i=0;i<12000;++i) {
        struct dh2_kill_objective s={random_signed(),random_signed(),random_signed(),i&255};
        struct dh2_kill_progress_event e={i&1?s.match_id:random_signed(),random_signed(),(i>>1)&255,(i>>2)&255};
        struct dh2_kill_progress_result r;
        assert(!dh2_quest_kill_event(&s,&e,&r));
        struct dh2_kill_objective before=s;struct dh2_kill_progress_event old=e;
        memset(&r,0xa5,sizeof(r));struct dh2_kill_progress_result previous=r;
        s.completed=256;assert(dh2_quest_kill_event(&s,&e,&r)==2);s=before;
        e.synchronized=256;assert(dh2_quest_kill_event(&s,&e,&r)==2);e=old;
        assert(dh2_quest_kill_event(&s,(void *)&s,&r)==1);
        assert(dh2_quest_kill_event(&s,&e,(void *)&e)==1);
        assert(dh2_quest_kill_event(&s,&e,(void *)&s)==1);
        assert(!memcmp(&s,&before,sizeof(s)) && !memcmp(&e,&old,sizeof(e)) && !memcmp(&r,&previous,sizeof(r)));
    }
    assert(dh2_quest_kill_event(NULL,NULL,NULL)==1);
    puts("QUEST KILL SAFETY PASS 12000");return 0;
}
