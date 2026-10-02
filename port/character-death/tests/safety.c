#include "../death.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static uint32_t seed=20261002;
static uint32_t random32(void) {seed=1664525u*seed+1013904223u;return seed;}
static int32_t random_signed(void) {uint32_t u=random32();int32_t i;memcpy(&i,&u,4);return i;}
static void put(unsigned char *p,uint32_t v) {for(unsigned i=0;i<4;++i)p[i]=(unsigned char)(v>>(8*i));}
int main(void) {
    unsigned char raw[1804]={0};put(raw,2);struct dh2_property_table t;
    assert(!dh2_property_open(&t,raw,sizeof(raw)));struct dh2_character_props s;
    assert(!dh2_character_props_init(&t,&s));
    for(unsigned i=0;i<12000;++i) {
        struct dh2_property_sheet *sheets[]={&s.base,&s.saved,&s.gears,&s.final};
        for(unsigned n=0;n<4;++n)for(unsigned f=0;f<224;++f)sheets[n]->values[f]=random_signed();
        put(raw+900+4*36,random32()%64);
        struct dh2_death_actor a={i&1,(i>>1)&1,(i>>2)&1,random32(),
            (int32_t)(random32()%65536)-32768,(int32_t)(random32()%65536)-32768};
        struct dh2_death_policy p={(i>>3)&1,(i>>4)&1,{random_signed(),random_signed(),random_signed(),random_signed()}};
        struct dh2_death_result out;memset(&out,0xa5,sizeof(out));
        struct dh2_character_props before=s;struct dh2_death_actor olda=a;
        assert(!dh2_death_nonplayer(&t,&s,&a,&p,&out));
        if(olda.dead)assert(!memcmp(&s,&before,sizeof(s)) && !memcmp(&a,&olda,sizeof(a)) && !out.processed && !out.event_count && !out.drop_loot_requested);
        before=s;olda=a;struct dh2_death_result oldout=out;
        a.dead=2;assert(dh2_death_nonplayer(&t,&s,&a,&p,&out));a=olda;
        a.property_id=32768;assert(dh2_death_nonplayer(&t,&s,&a,&p,&out));a=olda;
        p.forced=2;assert(dh2_death_nonplayer(&t,&s,&a,&p,&out));p.forced=(i>>3)&1;
        assert(dh2_death_nonplayer(&t,&s,&a,&p,(void *)&s));
        assert(dh2_death_nonplayer(&t,&s,(void *)&s,&p,&out));
        assert(dh2_death_nonplayer(&t,&s,&a,(void *)&s,&out));
        assert(dh2_death_nonplayer(&t,&s,&a,&p,(void *)&t));
        t.offsets[0]++;assert(dh2_death_nonplayer(&t,&s,&a,&p,&out));t.offsets[0]--;
        assert(!memcmp(&s,&before,sizeof(s)) && !memcmp(&a,&olda,sizeof(a)) && !memcmp(&out,&oldout,sizeof(out)));
    }
    assert(dh2_death_nonplayer(NULL,&s,NULL,NULL,NULL));
    puts("DEATH SAFETY PASS 12000");return 0;
}
