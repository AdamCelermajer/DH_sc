#include "quest.h"
#include <stddef.h>
#include <string.h>
static int overlap(const void *a,size_t an,const void *b,size_t bn) {
    uintptr_t aa=(uintptr_t)a,bb=(uintptr_t)b;
    return aa+an<aa || bb+bn<bb || (aa<bb+bn && bb<aa+an);
}
static int32_t signed_bits(uint32_t bits) {int32_t result;memcpy(&result,&bits,4);return result;}
uint32_t dh2_quest_kill_event(struct dh2_kill_objective *s,
    struct dh2_kill_progress_event *e,struct dh2_kill_progress_result *out) {
    if(!s || !e || !out)return 1;
    if(overlap(s,sizeof(*s),e,sizeof(*e)) || overlap(s,sizeof(*s),out,sizeof(*out)) || overlap(e,sizeof(*e),out,sizeof(*out)))return 1;
    if(s->completed>255 || e->outbound>255 || e->synchronized>255)return 2;
    struct dh2_kill_progress_result r={0,0,0,0};
    if(s->match_id!=e->match_id) {*out=r;return 0;}
    r.matched=1;struct dh2_kill_objective next=*s;struct dh2_kill_progress_event event=*e;
    if(!event.synchronized) {
        next.current=signed_bits((uint32_t)next.current+1);event.outbound=1;event.quantity=next.current;r.changed=1;
    } else if(next.current<event.quantity) {next.current=event.quantity;r.changed=1;}
    if(r.changed && next.current>=next.required) {
        r.completion_requested=1;r.newly_completed=!next.completed;
        if(!next.completed)next.completed=1;
    }
    *s=next;*e=event;*out=r;return 0;
}
