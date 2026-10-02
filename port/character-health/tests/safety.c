#include "../health.h"
#include <assert.h>
#include <math.h>
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
        for(unsigned f=0;f<224;++f) {s.base.values[f]=random_signed();s.saved.values[f]=random_signed();s.gears.values[f]=random_signed();s.final.values[f]=random_signed();}
        for(unsigned f=36;f<=43;++f)put(raw+900+4*f,random32()%64);
        uint32_t channel=i&1;int32_t delta=random_signed(),out[3]={123,456,789};float fraction;
        assert(!dh2_health_set(&t,&s,channel,delta));assert(!dh2_health_regen(&t,&s,channel,delta));
        assert(!dh2_health_validate(&t,&s));assert(!dh2_health_use_mana(&t,&s,delta,i&1,out));
        assert(!dh2_health_has_mana(&s,delta,i&1,out));assert(!dh2_health_fraction(&s,channel,&fraction));
        assert(!dh2_health_get(&s,channel,i&1,out));
        struct dh2_character_props before=s;int32_t old[3];memcpy(old,out,12);
        assert(dh2_health_set(&t,&s,2,delta) && !memcmp(&before,&s,sizeof(s)));
        assert(dh2_health_regen(&t,&s,2,delta) && !memcmp(&before,&s,sizeof(s)));
        assert(dh2_health_use_mana(&t,&s,delta,2,out) && !memcmp(&before,&s,sizeof(s)) && !memcmp(old,out,12));
        assert(dh2_health_has_mana(&s,delta,2,out) && !memcmp(old,out,12));
        assert(dh2_health_get(&s,2,0,out) && !memcmp(old,out,12));
        assert(dh2_health_fraction(&s,0,(void *)&s) && !memcmp(&before,&s,sizeof(s)));
        assert(dh2_health_script_hp(&s,(void *)&s) && !memcmp(&before,&s,sizeof(s)));
        assert(dh2_health_use_mana(&t,&s,delta,0,s.final.values) && !memcmp(&before,&s,sizeof(s)));
        assert(dh2_health_use_mana(&t,&s,delta,0,(void *)&t));
        t.offsets[0]++;assert(dh2_health_validate(&t,&s) && !memcmp(&before,&s,sizeof(s)));
        assert(dh2_health_regen(&t,&s,channel,0) && !memcmp(&before,&s,sizeof(s)));t.offsets[0]--;
        s.final.values[36]=random_signed();s.final.values[38]=1;
        assert(dh2_health_script_hp(&s,out) && !memcmp(old,out,12));
        s.final.values[38]=0;assert(!dh2_health_script_hp(&s,out));assert(!out[0] && !out[1] && !out[2]);
        assert(!dh2_health_fraction(&s,0,&fraction));assert(s.final.values[36]?isinf(fraction):isnan(fraction));
        s.final.values[36]=536870912;s.final.values[38]=-1;
        assert(dh2_health_script_hp(&s,out));
    }
    assert(dh2_health_validate(NULL,&s));assert(dh2_health_validate(&t,NULL));
    assert(dh2_health_get(NULL,0,0,NULL));assert(dh2_health_fraction(NULL,0,NULL));
    assert(dh2_health_script_hp(NULL,NULL));assert(dh2_health_has_mana(NULL,0,0,NULL));
    puts("HEALTH SAFETY PASS 12000");return 0;
}
