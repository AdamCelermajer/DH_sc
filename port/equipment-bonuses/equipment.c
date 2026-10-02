#include "equipment.h"
#include <stddef.h>
#include <string.h>
static int overlap(const void *a,size_t an,const void *b,size_t bn) {
    uintptr_t aa=(uintptr_t)a,bb=(uintptr_t)b;
    return aa+an<aa || bb+bn<bb || (an && bn && aa<bb+bn && bb<aa+an);
}
static int32_t bits(uint32_t b) { int32_t v;memcpy(&v,&b,4);return v; }
static int32_t add(int32_t a,int32_t b) { return bits((uint32_t)a+(uint32_t)b); }
static int32_t sar8(int32_t v) { uint32_t b=(uint32_t)v;return bits((b>>8)|(v<0?0xff000000u:0)); }
static int valid(const struct dh2_equipment *e) {
    if(!e || e->current_set>1)return 0;
    for(unsigned s=0;s<2;++s)for(unsigned i=0;i<3;++i) {
        const struct dh2_equipment_slot *slot=&e->slots[s][i];
        if(slot->present>1 || (slot->present && (slot->item_id<0 || slot->weapon_kind< -1 || slot->weapon_kind>6)))return 0;
    }
    return 1;
}
static int output(const struct dh2_equipment *e,int32_t *out) { return e && out && !overlap(out,4,e,sizeof(*e)); }
uint32_t dh2_equipment_load(const struct dh2_loot_tables *v,const int32_t ids[6],uint32_t set,uint32_t owner,struct dh2_equipment *out) {
    if(!v || !ids || !out || overlap(out,sizeof(*out),ids,24) || overlap(out,sizeof(*out),v,sizeof(*v)) || overlap(out,sizeof(*out),v->bytes,v->size))return 1;
    if(set>1)return 2;
    struct dh2_equipment e;memset(&e,0,sizeof(e));e.current_set=set;e.owner_hand_rule=owner;
    for(unsigned s=0;s<2;++s)for(unsigned i=0;i<3;++i) {
        int32_t id=ids[3*s+i];struct dh2_equipment_slot *slot=&e.slots[s][i];slot->item_id=id;
        if(id== -1)continue;
        if(id<0)return 2;
        struct dh2_loot_item item;
        if(dh2_loot_item_read(v,(uint32_t)id,&item))return 2;
        slot->present=1;slot->type=item.tail_ints[1];slot->slotting=item.tail_ints[5];slot->weapon_kind=item.tail_ints[16];
    }
    /* Validate even an all-empty snapshot's borrowed input. */
    struct dh2_loot_tables check;
    if(dh2_loot_open(&check,v->bytes,v->size) || memcmp(check.counts,v->counts,sizeof(check.counts)) || memcmp(check.offsets,v->offsets,sizeof(check.offsets)) || !valid(&e))return 2;
    *out=e;return 0;
}
uint32_t dh2_equipment_get_set(const struct dh2_equipment *e,int32_t slot,int32_t *out) {
    if(!output(e,out))return 1;
    if(!valid(e))return 2;
    *out=(slot<0 || slot==1 || slot==2)?(int32_t)e->current_set:0;return 0;
}
uint32_t dh2_equipment_get_item(const struct dh2_equipment *e,uint32_t id,int32_t *out) {
    if(!output(e,out))return 1;
    if(!valid(e))return 2;
    if(id>2) { *out= -1;return 0; }
    const struct dh2_equipment_slot *s=&e->slots[id==0?0:e->current_set][id];
    *out=s->present?s->item_id:-1;return 0;
}
static int flag(const struct dh2_equipment *e,uint32_t op) {
    const struct dh2_equipment_slot *off=&e->slots[e->current_set][2],*main=&e->slots[e->current_set][1];
    if(op==DH2_HAS_SHIELD)return off->present && off->type==6;
    if(op==DH2_HAS_OFF_HAND)return off->present && off->type!=6;
    if(!main->present || main->slotting!= -4)return 0;
    if(op==DH2_HAS_TWO_HANDER_IGNORE_RULE || main->type==4 || main->type==5)return 1;
    return e->owner_hand_rule==0;
}
uint32_t dh2_equipment_flag(const struct dh2_equipment *e,uint32_t op,int32_t *out) {
    if(!output(e,out))return 1;
    if(!valid(e) || op>3)return 2;
    *out=flag(e,op);return 0;
}
static int32_t bonus(const struct dh2_equipment *e,const struct dh2_property_sheet *sheet,uint32_t op,uint32_t off) {
    const struct dh2_equipment_slot *s=&e->slots[e->current_set][off?2:1];
    if(!s->present || s->weapon_kind== -1)return 0;
    uint32_t base=op==DH2_BONUS_CRIT?0x40:op==DH2_BONUS_ATTACK?0x33:0x53;
    int32_t v=sheet->values[base+(uint32_t)s->weapon_kind];
    if(op==DH2_BONUS_DAMAGE && flag(e,DH2_HAS_TWO_HANDER_IGNORE_RULE))v=add(v,sheet->values[0x5b]);
    if(op!=DH2_BONUS_CRIT && flag(e,DH2_HAS_OFF_HAND))v=add(v,sheet->values[op==DH2_BONUS_ATTACK?0x3a:0x5a]);
    return v;
}
uint32_t dh2_equipment_bonus(const struct dh2_equipment *e,const struct dh2_property_sheet *sheet,uint32_t op,uint32_t off,int32_t *out) {
    if(!output(e,out) || !sheet || overlap(out,4,sheet,sizeof(*sheet)))return 1;
    if(!valid(e) || op>2 || off>1)return 2;
    *out=bonus(e,sheet,op,off);return 0;
}
uint32_t dh2_equipment_get_int_bonus(const struct dh2_equipment *e,const struct dh2_character_props *state,const struct dh2_property_sheet *temp,uint32_t id,uint32_t use_temp,int32_t *out) {
    if(!output(e,out) || !state || overlap(out,4,state,sizeof(*state)) || (temp && overlap(out,4,temp,sizeof(*temp))))return 1;
    if(!valid(e) || id>=224 || use_temp>1 || (use_temp && !temp))return 2;
    int32_t v=sar8((use_temp?temp:&state->final)->values[id]);
    if(id==0x32)v=add(v,sar8(bonus(e,&state->final,DH2_BONUS_ATTACK,0)));
    else if(id==0x4f || id==0x50)v=add(v,sar8(bonus(e,&state->final,DH2_BONUS_DAMAGE,0)));
    else if(id==0x51 || id==0x52)v=add(v,sar8(bonus(e,&state->final,DH2_BONUS_DAMAGE,1)));
    *out=v;return 0;
}
