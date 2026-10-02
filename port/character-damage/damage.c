#include "damage.h"
#include <stddef.h>
#include <string.h>
static int overlap(const void *a,size_t an,const void *b,size_t bn) {
    uintptr_t aa=(uintptr_t)a,bb=(uintptr_t)b;
    return aa+an<aa || bb+bn<bb || (an && bn && aa<bb+bn && bb<aa+an);
}
static int32_t signed_bits(uint32_t u) {int32_t v;memcpy(&v,&u,4);return v;}
uint32_t dh2_hit_nonplayer(const struct dh2_property_table *t,
    struct dh2_character_props *s,struct dh2_hit_state *h,
    const struct dh2_hit_policy *p,uint32_t damage,struct dh2_hit_result *out) {
    if(!t || !s || !h || !p || !out)return 1;
    const void *inputs[]={t,t->bytes,p};
    const size_t input_sizes[]={sizeof(*t),t->size,sizeof(*p)};
    const void *outputs[]={s,h,out};
    const size_t output_sizes[]={sizeof(*s),sizeof(*h),sizeof(*out)};
    for(uint32_t i=0;i<3;++i) {
        for(uint32_t j=0;j<3;++j)
            if(overlap(outputs[i],output_sizes[i],inputs[j],input_sizes[j]))return 1;
        for(uint32_t j=i+1;j<3;++j)
            if(overlap(outputs[i],output_sizes[i],outputs[j],output_sizes[j]))return 1;
    }
    if(p->target_dead>1 || p->target_monster>1 || p->local_player_alive>1 ||
       p->online>1 || p->manager_present>1 || p->monster_invincible>1 ||
       p->force_kill_config>1 || p->force_kill_switch>1 || p->target_network>1)return 2;
    int32_t type;uint32_t status=dh2_property_type(t,36,&type);if(status)return status;
    struct dh2_hit_result result={0,0,0};
    if(p->target_dead) {*out=result;return 0;}
    struct dh2_character_props next=*s;struct dh2_hit_state hit=*h;
    uint32_t enabled=!(p->monster_invincible && p->target_monster) &&
        (p->online?(!p->manager_present || p->manager_mode==0 || p->manager_mode==5):p->local_player_alive);
    uint32_t delta=enabled?0u-damage:0u;
    result.processed=1;
    if(enabled)result.damage_whole=signed_bits((damage>>8)|((damage&0x80000000u)?0xff000000u:0));
    /* Even a zero delta runs the original property routing and recalculation. */
    status=dh2_character_props_write(t,&next,NULL,0,36,signed_bits(delta),DH2_PROPS_ADD);
    if(status)return status;
    if(p->force_kill_config || p->force_kill_switch) {
        status=dh2_character_props_write(t,&next,NULL,0,36,0,DH2_PROPS_SET);
        if(status)return status;
    }
    if(next.final.values[36]<=0) {
        status=dh2_character_props_write(t,&next,NULL,0,36,0,DH2_PROPS_SET);
        if(status)return status;
        result.death_requested=1;
        if(!p->target_network)hit.death_reason=3;
    }
    *s=next;*h=hit;*out=result;return 0;
}
