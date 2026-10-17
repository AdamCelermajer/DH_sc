#include "constants.h"
#include <stddef.h>
#include <string.h>
static int overlap(const void *a,size_t an,const void *b,size_t bn) {
    uintptr_t aa=(uintptr_t)a,bb=(uintptr_t)b;
    return aa+an<aa || bb+bn<bb || (an && bn && aa<bb+bn && bb<aa+an);
}
static int word(const unsigned char *raw,uint32_t size,uint32_t *at,uint32_t *out) {
    if(*at>size || size-*at<4)return 0;
    const unsigned char *b=raw+*at;
    *out=(uint32_t)b[0]|((uint32_t)b[1]<<8)|((uint32_t)b[2]<<16)|((uint32_t)b[3]<<24);
    *at+=4;return 1;
}
static int name(const unsigned char *raw,uint32_t size,uint32_t *at,uint32_t *begin,uint32_t *length) {
    if(!word(raw,size,at,length) || *length>255 || *at>size || *length>size-*at)return 0;
    if(memchr(raw+*at,0,*length))return 0;
    *begin=*at;*at+=*length;return 1;
}
/* Shared bounded walk: validation, ordinal enumeration and last-write lookup. */
static int walk(const struct dh2_pycst_view *view,uint32_t ordinal,
                struct dh2_pycst_entry *entry,const void *group,uint32_t glen,
                const void *key,uint32_t klen,struct dh2_pycst_result *result,
                uint32_t *total_groups,uint32_t *total_entries) {
    uint32_t at=0,groups=0,entries=0;
    if(!view || !view->bytes || view->size>16*1024*1024 || !word(view->bytes,view->size,&at,&groups))return 0;
    if(groups>(view->size-at)/8)return 0;
    for(uint32_t g=0;g<groups;++g) {
        uint32_t go,gn,count;
        if(!name(view->bytes,view->size,&at,&go,&gn) || !word(view->bytes,view->size,&at,&count))return 0;
        if(count>(view->size-at)/8)return 0;
        for(uint32_t k=0;k<count;++k) {
            struct dh2_pycst_entry row={go,gn,0,0,0};uint32_t value;
            if(!name(view->bytes,view->size,&at,&row.name_offset,&row.name_length) ||
               !word(view->bytes,view->size,&at,&value))return 0;
            memcpy(&row.value,&value,4);
            if(entry && ordinal==UINT32_MAX)entry[entries]=row;
            else if(entry && entries==ordinal)*entry=row;
            if(result && gn==glen && row.name_length==klen &&
               (!gn || memcmp(view->bytes+go,group,gn)==0) &&
               (!klen || memcmp(view->bytes+row.name_offset,key,klen)==0)) {
                result->found=1;result->value=row.value;
            }
            ++entries;
        }
    }
    if(at!=view->size)return 0;
    if(total_groups)*total_groups=groups;
    if(total_entries)*total_entries=entries;
    return 1;
}
uint32_t dh2_pycst_open(struct dh2_pycst_view *out,const void *bytes,uint32_t size) {
    if(!out || !bytes || overlap(out,sizeof(*out),bytes,size))return 1;
    struct dh2_pycst_view value={bytes,size,0,0};
    if(!walk(&value,0,NULL,NULL,0,NULL,0,NULL,&value.groups,&value.entries))return 2;
    *out=value;return 0;
}
static int valid(const struct dh2_pycst_view *view) {
    uint32_t groups,entries;
    return walk(view,0,NULL,NULL,0,NULL,0,NULL,&groups,&entries) &&
           groups==view->groups && entries==view->entries;
}
uint32_t dh2_pycst_copy_entries(const struct dh2_pycst_view *view,struct dh2_pycst_entry *out,uint32_t capacity) {
    if(!view || capacity>UINT32_MAX/sizeof(*out) || (!out && capacity))return 1;
    size_t bytes=(size_t)capacity*sizeof(*out);
    if(overlap(out,bytes,view,sizeof(*view)) || overlap(out,bytes,view->bytes,view->size))return 1;
    if(!valid(view) || capacity<view->entries)return 2;
    if(!walk(view,UINT32_MAX,out,NULL,0,NULL,0,NULL,NULL,NULL))return 2;
    return 0;
}
uint32_t dh2_pycst_entry_at(const struct dh2_pycst_view *view,uint32_t ordinal,struct dh2_pycst_entry *out) {
    if(!view || !out || overlap(out,sizeof(*out),view,sizeof(*view)) ||
       overlap(out,sizeof(*out),view->bytes,view->size))return 1;
    if(!valid(view) || ordinal>=view->entries)return 2;
    struct dh2_pycst_entry row;
    if(!walk(view,ordinal,&row,NULL,0,NULL,0,NULL,NULL,NULL))return 2;
    *out=row;return 0;
}
uint32_t dh2_pycst_get(const struct dh2_pycst_view *view,const void *group,uint32_t glen,
                     const void *key,uint32_t klen,struct dh2_pycst_result *out) {
    if(!view || !out || (!group && glen) || (!key && klen) || glen>255 || klen>255 ||
       overlap(out,sizeof(*out),view,sizeof(*view)) || overlap(out,sizeof(*out),view->bytes,view->size) ||
       overlap(out,sizeof(*out),group,glen) || overlap(out,sizeof(*out),key,klen))return 1;
    if((glen && memchr(group,0,glen)) || (klen && memchr(key,0,klen)))return 1;
    if(!valid(view))return 2;
    struct dh2_pycst_result result={0,0};
    if(!walk(view,0,NULL,group,glen,key,klen,&result,NULL,NULL))return 2;
    *out=result;return 0;
}
