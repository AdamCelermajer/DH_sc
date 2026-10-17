#include "death.h"
#include <stddef.h>
#include <string.h>
static int overlap(const void *a,size_t an,const void *b,size_t bn) {
    uintptr_t aa=(uintptr_t)a,bb=(uintptr_t)b;
    return aa+an<aa || bb+bn<bb || (an && bn && aa<bb+bn && bb<aa+an);
}
uint32_t dh2_death_nonplayer(const struct dh2_property_table *t,
    struct dh2_character_props *s,struct dh2_death_actor *a,
    const struct dh2_death_policy *p,struct dh2_death_result *out) {
    if(!t || !s || !a || !p || !out)return 1;
    const void *inputs[]={t,t->bytes,p};const size_t input_sizes[]={sizeof(*t),t->size,sizeof(*p)};
    const void *outputs[]={s,a,out};const size_t output_sizes[]={sizeof(*s),sizeof(*a),sizeof(*out)};
    for(uint32_t i=0;i<3;++i) {
        for(uint32_t j=0;j<3;++j)if(overlap(outputs[i],output_sizes[i],inputs[j],input_sizes[j]))return 1;
        for(uint32_t j=i+1;j<3;++j)if(overlap(outputs[i],output_sizes[i],outputs[j],output_sizes[j]))return 1;
    }
    if(a->dead>1 || a->network>1 || a->suppress_events>1 || p->forced>1 || p->loot_manager_present>1 ||
       a->property_id< -32768 || a->property_id>32767 || a->template_id< -32768 || a->template_id>32767)return 2;
    int32_t type;uint32_t status=dh2_property_type(t,36,&type);if(status)return status;
    struct dh2_death_result result;memset(&result,0,sizeof(result));
    if(a->dead) {*out=result;return 0;}
    struct dh2_character_props next=*s;struct dh2_death_actor actor=*a;
    actor.dead=1;result.processed=1;
    status=dh2_character_props_write(t,&next,NULL,0,36,0,DH2_PROPS_SET);if(status)return status;
    result.drop_loot_requested=!p->loot_manager_present;
    if(result.drop_loot_requested)result.drop_loot_id=next.final.values[9];
    if(!p->forced && !a->network && !a->suppress_events) {
        uint32_t count=a->template_id==-1?2:4;
        for(uint32_t kind=0;kind<count;++kind) {
            struct dh2_death_event *event=&result.events[kind];
            event->kind=kind;event->target_id=a->target_id;event->objective_id=p->objective_ids[kind];
            event->match_id=kind<2?a->property_id:a->template_id;
        }
        result.event_count=count;
    }
    *s=next;*a=actor;*out=result;return 0;
}
