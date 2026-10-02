#include "../composition.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static uint32_t seed=20261002;
static uint32_t random32(void) { seed=1664525u*seed+1013904223u;return seed; }
static void put(unsigned char *p,uint32_t v) { for(unsigned i=0;i<4;++i)p[i]=(unsigned char)(v>>(8*i)); }
int main(void) {
    unsigned char raw[1804]={0};put(raw,2);
    struct dh2_property_table table;assert(!dh2_property_open(&table,raw,sizeof(raw)));
    struct dh2_property_sheet base={{0}},saved={{0}},gears={{0}},buff={{0}},out={{0}};
    const struct dh2_property_sheet *sheets[]={&buff};struct dh2_property_group group={sheets,1};
    struct dh2_property_inputs in={&base,&saved,&gears,&group,1};
    for(unsigned k=0;k<10000;++k) {
        uint32_t id=random32()%224;put(raw+900+4*id,random32()%64);
        base.values[id]=(int32_t)(random32()%100000);saved.values[id]=(int32_t)(random32()%100000);
        gears.values[id]=(int32_t)(random32()%100000);buff.values[id]=(int32_t)(random32()%100000);
        assert(!dh2_property_recalc(&table,&in,id,&out));struct dh2_property_sheet before=out;
        assert(dh2_property_recalc(&table,&in,224,&out) && !memcmp(&out,&before,sizeof(out)));
        struct dh2_property_inputs bad=in;bad.count=33;
        assert(dh2_property_recalc(&table,&bad,id,&out) && !memcmp(&out,&before,sizeof(out)));
        assert(dh2_property_recalc(&table,&in,id,&base));
        group.count=65;assert(dh2_property_recalc(&table,&in,id,&out) && !memcmp(&out,&before,sizeof(out)));group.count=1;
        sheets[0]=&out;assert(dh2_property_recalc(&table,&in,id,&out) && !memcmp(&out,&before,sizeof(out)));sheets[0]=&buff;
        table.offsets[1]++;assert(dh2_property_recalc(&table,&in,id,&out) && !memcmp(&out,&before,sizeof(out)));table.offsets[1]--;
    }
    puts("Composition: 10000 valid/damaged safety iterations passed");return 0;
}
