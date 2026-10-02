#include "compile.h"
#include <stddef.h>
static int overlap(const void *a,size_t an,const void *b,size_t bn) {
    uintptr_t aa=(uintptr_t)a,bb=(uintptr_t)b;
    return aa+an<aa || bb+bn<bb || (aa<bb+bn && bb<aa+an);
}
uint32_t dh2_quest_population(uint32_t kind,int32_t id,const struct dh2_quest_character *chars,uint32_t count,
    const struct dh2_quest_cache_entry *cache,uint32_t cached,int32_t *out) {
    if(!out || (count && !chars) || (cached && !cache))return 1;
    if(kind>1 || count>4096 || cached>4096)return 2;
    if((count && overlap(chars,count*sizeof(*chars),out,sizeof(*out))) ||
       (cached && overlap(cache,cached*sizeof(*cache),out,sizeof(*out))))return 1;
    for(uint32_t i=0;i<count;++i) {
        if(chars[i].present>255)return 2;
        if(chars[i].present && (chars[i].property_id< -32768 || chars[i].property_id>32767 ||
            chars[i].template_id< -32768 || chars[i].template_id>32767))return 2;
    }
    for(uint32_t i=0;i<cached;++i)for(uint32_t j=0;j<i;++j)
        if(cache[i].property_id==cache[j].property_id)return 2;
    int32_t quantity=0;
    for(uint32_t i=0;i<count;++i)if(chars[i].present &&
        (kind?chars[i].template_id:chars[i].property_id)==id)++quantity;
    if(!quantity && !kind)for(uint32_t i=0;i<cached;++i)
        if(cache[i].property_id==id) {quantity=cache[i].quantity;break;}
    *out=quantity;return 0;
}
uint32_t dh2_quest_compile(struct dh2_quest_compiled *s,const struct dh2_quest_compile_context *context,
    struct dh2_quest_compile_result *out) {
    if(!s || !context || !out)return 1;
    if(overlap(s,sizeof(*s),context,sizeof(*context)) || overlap(s,sizeof(*s),out,sizeof(*out)) ||
       overlap(context,sizeof(*context),out,sizeof(*out)))return 1;
    if(context->kind>3 || s->active>255 || s->progress.completed>255)return 2;
    struct dh2_quest_compiled next=*s;struct dh2_quest_compile_result r={0,0,0,0};
    next.progress.match_id=context->match_id;
    int counted=!(context->kind&1);
    int level=context->record_level==-1 || context->record_level==context->current_level;
    if(counted) {next.progress.required=context->record_required;r.required_updated=1;}
    if(level) {
        if(!counted) {next.progress.required=context->population;r.required_updated=1;}
        if(context->population>0 && (!counted || next.progress.required>0)) {
            next.active=1;r.eligible=1;
            if(next.progress.current>=next.progress.required) {
                r.completion_requested=1;r.newly_completed=!next.progress.completed;
                if(!next.progress.completed)next.progress.completed=1;
            }
        }
    }
    if(counted && !r.eligible)next.active=0;
    *s=next;*out=r;return 0;
}
