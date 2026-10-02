#include "../methods.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
static uint32_t seed=20261002;
static uint32_t random32(void) { seed=1664525u*seed+1013904223u;return seed; }
static void put(unsigned char *p,uint32_t v) { for(unsigned i=0;i<4;++i)p[i]=(unsigned char)(v>>(8*i)); }
int main(void) {
    unsigned char raw[1804]={0};put(raw,2);struct dh2_property_table t;assert(!dh2_property_open(&t,raw,sizeof(raw)));
    struct dh2_character_props s;assert(!dh2_character_props_init(&t,&s));
    for(unsigned i=0;i<12000;++i) {
        float args[3]={(float)(random32()%224),257.9f,1};uint32_t tags[3]={3,3,0};
        put(raw+900+4*(uint32_t)args[0],random32()%64);
        struct dh2_property_result r={123,456};assert(!dh2_character_property_method(&t,&s,1,args,tags,3,&r) && !r.count);
        struct dh2_character_props before=s;r.count=123;r.value=456;struct dh2_property_result prior=r;
        args[1]=i%2?INFINITY:NAN;
        assert(dh2_character_property_method(&t,&s,1,args,tags,2,&r)==2 && !memcmp(&r,&prior,sizeof(r)) && !memcmp(&s,&before,sizeof(s)));
        args[1]=2147483648.f;assert(dh2_character_property_method(&t,&s,1,args,tags,2,&r)==2 && !memcmp(&s,&before,sizeof(s)));
        args[1]=1;tags[2]=2;assert(dh2_character_property_method(&t,&s,1,args,tags,3,&r)==2 && !memcmp(&s,&before,sizeof(s)));
        assert(dh2_character_property_method(&t,&s,0,args,tags,3,(void *)&s) && !memcmp(&s,&before,sizeof(s)));
        assert(dh2_character_property_method(&t,&s,0,args,tags,33,&r) && !memcmp(&s,&before,sizeof(s)));
        tags[0]=0;assert(!dh2_character_property_method(&t,&s,0,args,tags,1,&r) && !r.count);
    }
    puts("Property methods: 12000 valid/damaged safety iterations passed");return 0;
}
