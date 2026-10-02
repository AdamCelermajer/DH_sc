#include "../damage.h"
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
        struct dh2_hit_policy p={i&1,(i>>1)&1,(i>>2)&1,(i>>3)&1,(i>>4)&1,
            random_signed(),(i>>5)&1,(i>>6)&1,(i>>7)&1,(i>>8)&1};
        struct dh2_hit_state h={random_signed()};struct dh2_hit_result out={123,456,789};
        struct dh2_character_props before=s;struct dh2_hit_state oldh=h;
        assert(!dh2_hit_nonplayer(&t,&s,&h,&p,random32(),&out));
        if(p.target_dead)assert(!memcmp(&s,&before,sizeof(s)) && h.death_reason==oldh.death_reason && !out.processed && !out.death_requested);
        before=s;oldh=h;struct dh2_hit_result oldout=out;
        p.target_dead=2;assert(dh2_hit_nonplayer(&t,&s,&h,&p,1,&out));p.target_dead=0;
        assert(!memcmp(&s,&before,sizeof(s)) && !memcmp(&h,&oldh,sizeof(h)) && !memcmp(&out,&oldout,sizeof(out)));
        assert(dh2_hit_nonplayer(&t,&s,&h,&p,1,(void *)&s));
        assert(dh2_hit_nonplayer(&t,&s,(void *)&s,&p,1,&out));
        assert(dh2_hit_nonplayer(&t,&s,&h,(void *)&s,1,&out));
        assert(dh2_hit_nonplayer(&t,&s,&h,&p,1,(void *)&t));
        t.offsets[0]++;assert(dh2_hit_nonplayer(&t,&s,&h,&p,1,&out));t.offsets[0]--;
        assert(!memcmp(&s,&before,sizeof(s)) && !memcmp(&h,&oldh,sizeof(h)) && !memcmp(&out,&oldout,sizeof(out)));
    }
    assert(dh2_hit_nonplayer(NULL,&s,NULL,NULL,0,NULL));
    puts("DAMAGE SAFETY PASS 12000");return 0;
}
