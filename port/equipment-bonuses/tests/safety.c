#include "../equipment.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static uint32_t seed=20261002;
static uint32_t random_word(void) { seed=seed*1664525u+1013904223u;return seed; }
static void word(unsigned char *p,uint32_t v) { for(unsigned i=0;i<4;++i)p[i]=(unsigned char)(v>>(i*8)); }
int main(void) {
    unsigned char raw[192]={0},copy[192];
    /* Eight tables, one item, two embedded-NUL strings, all scalar bits intact. */
    word(raw+12,1);word(raw+16,2);raw[20]='x';raw[21]=0;
    word(raw+83,3);raw[87]='d';raw[88]=0;raw[89]='z';
    uint32_t n=186; /* 16 + (149+2+3) + 16 empty trailing table headers. */
    struct dh2_loot_tables v,before;assert(!dh2_loot_open(&v,raw,n));
    struct dh2_loot_item item;assert(!dh2_loot_item_read(&v,0,&item));
    assert(item.name.size==2 && item.description.size==3 && item.name.bytes[1]==0 && item.description.bytes[1]==0);
    memcpy(copy,raw,sizeof(raw));memset(&before,0xa5,sizeof(before));
    for(unsigned size=0;size<n;++size) { struct dh2_loot_tables out=before;assert(dh2_loot_open(&out,raw,size));assert(!memcmp(&out,&before,sizeof(out))); }
    struct dh2_loot_tables out=before;assert(dh2_loot_open(&out,raw,n+1));assert(!memcmp(&out,&before,sizeof(out)));
    assert(dh2_loot_open((struct dh2_loot_tables *)raw,raw,n)==1);
    int32_t ids[6]={-1,0,-1,-1,0,0};struct dh2_equipment e;assert(!dh2_equipment_load(&v,ids,1,0,&e));
    struct dh2_equipment eb=e;ids[4]=1;assert(dh2_equipment_load(&v,ids,1,0,&e));assert(!memcmp(&e,&eb,sizeof(e)));ids[4]=0;
    assert(dh2_equipment_load(&v,ids,2,0,&e));assert(!memcmp(&e,&eb,sizeof(e)));
    struct dh2_character_props state;struct dh2_property_sheet temp;
    for(unsigned iteration=0;iteration<12000;++iteration) {
        for(unsigned i=0;i<224;++i) { uint32_t a=random_word(),b=random_word();memcpy(&state.final.values[i],&a,4);memcpy(&temp.values[i],&b,4); }
        for(unsigned s=0;s<2;++s)for(unsigned i=0;i<3;++i) {
            struct dh2_equipment_slot *slot=&e.slots[s][i];slot->present=random_word()&1;slot->item_id=0;slot->type=(int32_t)(random_word()%15)-1;
            slot->slotting=(int32_t)(random_word()%14)-4;slot->weapon_kind=(int32_t)(random_word()%7)-1;
        }
        e.current_set=random_word()&1;e.owner_hand_rule=random_word();eb=e;
        int32_t result=12345;for(unsigned flag=0;flag<4;++flag)assert(!dh2_equipment_flag(&e,flag,&result));
        for(unsigned bonus=0;bonus<3;++bonus)for(unsigned off=0;off<2;++off)assert(!dh2_equipment_bonus(&e,&state.final,bonus,off,&result));
        assert(!dh2_equipment_get_int_bonus(&e,&state,&temp,random_word()%224,random_word()&1,&result));
        assert(!memcmp(&e,&eb,sizeof(e)));
        result=12345;assert(dh2_equipment_bonus(&e,&state.final,3,0,&result)==2 && result==12345);
        assert(dh2_equipment_get_int_bonus(&e,&state,NULL,0,1,&result)==2 && result==12345);
        assert(dh2_equipment_bonus(&e,&state.final,0,0,&e.slots[0][0].item_id)==1);
        unsigned char mutated[192];memcpy(mutated,raw,sizeof(raw));unsigned pos=random_word()%n;mutated[pos]^=(unsigned char)random_word();
        struct dh2_loot_tables mv=before;uint32_t status=dh2_loot_open(&mv,mutated,n);
        if(status)assert(!memcmp(&mv,&before,sizeof(mv)));
        else if(mv.counts[3]) { struct dh2_loot_item mi;assert(!dh2_loot_item_read(&mv,0,&mi)); }
    }
    assert(!memcmp(raw,copy,sizeof(raw)));puts("Equipment/loot safety: 12000 iterations passed");return 0;
}
