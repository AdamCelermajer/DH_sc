#include "../properties.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static uint32_t seed=20261002;
static uint32_t random32(void) { seed=1664525u*seed+1013904223u;return seed; }
static void put(unsigned char *p,uint32_t v) { for(unsigned i=0;i<4;++i)p[i]=(unsigned char)(v>>(8*i)); }
int main(void) {
    unsigned char raw[1820],saved[1820];
    for(unsigned i=0;i<12000;++i) {
        memset(raw,0,sizeof(raw));put(raw,2);put(raw+4,0xffffffffu);put(raw+900,0xffffffffu);
        uint32_t size=1804;
        if(i%4==0)size=random32()%1804;
        if(i%4==1)raw[random32()%size]=(unsigned char)random32();
        if(i%4==2) { size=random32()%sizeof(raw);for(unsigned j=0;j<size;++j)raw[j]=(unsigned char)random32(); }
        memcpy(saved,raw,sizeof(raw));
        struct dh2_property_table v;memset(&v,0xa5,sizeof(v));struct dh2_property_table before=v;
        uint32_t status=dh2_property_open(&v,raw,size);
        assert(!memcmp(raw,saved,sizeof(raw)));
        if(status) { assert(!memcmp(&v,&before,sizeof(v)));continue; }
        struct dh2_property_sheet s;memset(&s,0xa5,sizeof(s));
        assert(!dh2_property_load(&v,0,&s));
        int32_t value=123;assert(!dh2_property_get(&s,0,&value));
        if(i%4==3) {
            assert(value==-1);assert(!dh2_property_type(&v,0,&value) && value==16);
            assert(!dh2_property_add(&v,&s,0,INT32_MAX) && s.values[0]==INT32_MAX);
            assert(!dh2_property_add(&v,&s,0,1) && s.values[0]==INT32_MIN);
            assert(!dh2_property_reset(&v,&s) && s.values[0]==-1);
        }
        struct dh2_property_sheet old=s;int32_t prior=value;
        assert(dh2_property_get(&s,224,&value) && value==prior);
        assert(dh2_property_set(&s,UINT32_MAX,77) && !memcmp(&s,&old,sizeof(s)));
        assert(dh2_property_load(&v,v.counts[0],&s) && !memcmp(&s,&old,sizeof(s)));
        assert(dh2_property_load(&v,0,(void *)raw) && !memcmp(raw,saved,sizeof(raw)));
        assert(dh2_property_default(&v,0,(void *)raw) && !memcmp(raw,saved,sizeof(raw)));
        assert(dh2_property_get(&s,0,&s.values[1]) && !memcmp(&s,&old,sizeof(s)));
        assert(dh2_property_add(&v,(void *)raw,0,1) && !memcmp(raw,saved,sizeof(raw)));
        v.offsets[0]++;assert(dh2_property_load(&v,0,&s) && !memcmp(&s,&old,sizeof(s)));
    }
    puts("Properties: 12000 valid/damaged safety iterations passed");return 0;
}
