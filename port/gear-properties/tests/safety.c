#include "../gears.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static uint32_t seed=20261002;
static uint32_t random_word(void) {seed=seed*1664525u+1013904223u;return seed;}
static void word(unsigned char *p,uint32_t v) {for(unsigned i=0;i<4;++i)p[i]=(unsigned char)(v>>(i*8));}
int main(void) {
    unsigned char props[1804]={0},loot[192]={0},power[49]={0},classes[8]={0};
    word(props,2);for(unsigned i=0;i<224;++i) {word(props+4+i*4,0xffffffffu);word(props+900+i*4,16);}
    struct dh2_property_table pt;assert(!dh2_property_open(&pt,props,sizeof(props)));
    word(classes,1);struct dh2_class_table ct;assert(!dh2_class_open(&ct,classes,sizeof(classes)));
    word(loot+12,1);word(loot+16,2);loot[20]='x';word(loot+83,3);loot[87]='d';loot[89]='z';
    struct dh2_loot_tables lt;assert(!dh2_loot_open(&lt,loot,186));
    word(power+4,1);word(power+13,1);word(power+17,9);word(power+21,0x80000000u);
    struct dh2_power_tables wt,bad;assert(!dh2_power_open(&wt,power,sizeof(power)));memset(&bad,0xa5,sizeof(bad));
    for(unsigned n=0;n<sizeof(power);++n) {struct dh2_power_tables x=bad;assert(dh2_power_open(&x,power,n));assert(!memcmp(&x,&bad,sizeof(x)));}
    assert(dh2_power_open((struct dh2_power_tables *)power,power,sizeof(power))==1);
    struct dh2_power_record record;assert(!dh2_power_read(&wt,0,&record) && record.count==1);
    struct dh2_property_sheet sheet,old;assert(!dh2_property_reset(&pt,&sheet));
    assert(!dh2_gear_add_power(&pt,&wt,&sheet,0,0) && (uint32_t)sheet.values[97]==0x80000000u);
    int32_t ids[]={0,1};struct dh2_gear_entry entries[]={{0,1,ids},{-1,0,NULL},{0,1,ids}};
    struct dh2_character_props state;state.base=state.saved=state.gears=state.final=sheet;
    assert(!dh2_character_update_base(&pt,&ct,&state,0));
    assert(!dh2_character_update_gears(&pt,&ct,&lt,&wt,&state,entries,3));
    struct dh2_character_props before=state;
    entries[2].power_count=2;assert(dh2_character_update_gears(&pt,&ct,&lt,&wt,&state,entries,3)==2);assert(!memcmp(&before,&state,sizeof(state)));entries[2].power_count=1;
    struct dh2_property_table broken=pt;broken.counts[0]++;
    assert(dh2_character_update_base(&broken,&ct,&state,2)==2 && !memcmp(&before,&state,sizeof(state)));
    assert(dh2_character_recalc(&pt,&ct,&state,2)==2 && !memcmp(&before,&state,sizeof(state)));
    assert(dh2_character_update_gears(&pt,&ct,&lt,&wt,&state,(const struct dh2_gear_entry *)&state,1)==1);
    assert(dh2_gear_add_power(&pt,&wt,(struct dh2_property_sheet *)props,0,0)==1);
    for(unsigned iteration=0;iteration<12000;++iteration) {
        for(unsigned i=0;i<224;++i) {uint32_t b=random_word();memcpy(&sheet.values[i],&b,4);}
        word(power+17,iteration%52-1);word(power+21,random_word());word(power+25,random_word());
        assert(!dh2_gear_add_power(&pt,&wt,&sheet,0,iteration&1));
        /* Every item type and signed/shift overflow on bounded records. */
        word(loot+94,(iteration%17)-1);word(loot+106,random_word());word(loot+146,random_word());word(loot+150,random_word());
        assert(!dh2_gear_add_item(&pt,&lt,&sheet,0,iteration&1));old=sheet;
        assert(dh2_gear_add_item(&pt,&lt,&sheet,1,0)==2 && !memcmp(&sheet,&old,sizeof(sheet)));
        assert(dh2_gear_add_power(&pt,&wt,&sheet,0,2)==2 && !memcmp(&sheet,&old,sizeof(sheet)));
        assert(dh2_gear_load(&pt,&lt,&wt,&sheet,entries,65)==2 && !memcmp(&sheet,&old,sizeof(sheet)));
        struct dh2_power_tables damaged=wt;damaged.offsets[1]++;
        assert(dh2_gear_add_power(&pt,&damaged,&sheet,0,0)==2 && !memcmp(&sheet,&old,sizeof(sheet)));
        unsigned char mutant[49];memcpy(mutant,power,sizeof(mutant));mutant[random_word()%49]^=(unsigned char)random_word();
        struct dh2_power_tables out=bad;uint32_t rc=dh2_power_open(&out,mutant,sizeof(mutant));
        if(rc)assert(!memcmp(&out,&bad,sizeof(out)));else assert(!dh2_power_read(&out,0,&record));
        if(iteration%100==0) {state.base=state.saved=state.gears=state.final=sheet;state.base.values[26]=-1;assert(!dh2_character_recalc(&pt,&ct,&state,1));}
    }
    puts("Gear/power/lifecycle safety: 12000 iterations passed");return 0;
}
