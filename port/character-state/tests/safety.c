#include "../state.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static uint32_t seed=20261002;
static uint32_t random32(void) { seed=1664525u*seed+1013904223u;return seed; }
static void put(unsigned char *p,uint32_t v) { for(unsigned i=0;i<4;++i)p[i]=(unsigned char)(v>>(8*i)); }
int main(void) {
    unsigned char raw[1804]={0};put(raw,2);struct dh2_property_table t;assert(!dh2_property_open(&t,raw,sizeof(raw)));
    struct dh2_character_props state;assert(!dh2_character_props_init(&t,&state));
    for(unsigned i=0;i<12000;++i) {
        uint32_t id=random32()%224;put(raw+900+id*4,random32()%64);
        int32_t delta;uint32_t bits=random32();memcpy(&delta,&bits,4);
        assert(!dh2_character_props_write(&t,&state,NULL,0,id,delta,random32()%4));
        struct dh2_character_props before=state;int32_t result=123;
        assert(!dh2_character_props_get_int(&state,id,&result));
        assert(dh2_character_props_write(&t,&state,NULL,0,224,delta,0) && !memcmp(&before,&state,sizeof(state)));
        assert(dh2_character_props_write(&t,&state,NULL,0,id,delta,4) && !memcmp(&before,&state,sizeof(state)));
        const struct dh2_property_sheet *sheets[]={&state.base};struct dh2_property_group group={sheets,1};
        assert(dh2_character_props_write(&t,&state,&group,1,id,delta,0) && !memcmp(&before,&state,sizeof(state)));
        assert(dh2_character_props_write(&t,&state,&group,33,id,delta,0) && !memcmp(&before,&state,sizeof(state)));
        t.offsets[0]++;assert(dh2_character_props_write(&t,&state,NULL,0,id,delta,0) && !memcmp(&before,&state,sizeof(state)));t.offsets[0]--;
        assert(dh2_character_props_get_int(&state,id,&state.final.values[id]) && !memcmp(&before,&state,sizeof(state)));
    }
    puts("Character writes: 12000 valid/damaged safety iterations passed");return 0;
}
