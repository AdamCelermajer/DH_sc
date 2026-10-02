#include "names.h"
#include <stddef.h>
#include <string.h>
static int overlap(const void *a,size_t an,const void *b,size_t bn) {
    uintptr_t aa=(uintptr_t)a,bb=(uintptr_t)b;
    return aa+an<aa || bb+bn<bb || (an && bn && aa<bb+bn && bb<aa+an);
}
static int word(const unsigned char *raw,uint32_t size,uint32_t *at,uint32_t *out) {
    if(*at>size || size-*at<4)return 0;
    const unsigned char *p=raw+*at;*at+=4;
    *out=(uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);return 1;
}
static int walk(const struct dh2_pynames_view *view,struct dh2_pyname *entries,
                const void *needle,uint32_t length,int32_t *index,uint32_t *total) {
    if(!view || !view->bytes || view->size>16*1024*1024)return 0;
    uint32_t at=0,count;
    if(!word(view->bytes,view->size,&at,&count) || count>(view->size-at)/4)return 0;
    for(uint32_t i=0;i<count;++i) {
        uint32_t size;if(!word(view->bytes,view->size,&at,&size) || size>255 || size>view->size-at)return 0;
        if(memchr(view->bytes+at,0,size))return 0;
        if(entries) { entries[i].offset=at;entries[i].length=size; }
        if(index && *index<0 && size==length && (!size || memcmp(view->bytes+at,needle,size)==0))*index=(int32_t)i;
        at+=size;
    }
    if(at!=view->size)return 0;
    if(total)*total=count;
    return 1;
}
static int valid(const struct dh2_pynames_view *view) {
    uint32_t count;
    return walk(view,NULL,NULL,0,NULL,&count) && count==view->count;
}
uint32_t dh2_pynames_open(struct dh2_pynames_view *out,const void *bytes,uint32_t size) {
    if(!out || !bytes || overlap(out,sizeof(*out),bytes,size))return 1;
    struct dh2_pynames_view value={bytes,size,0};
    if(!walk(&value,NULL,NULL,0,NULL,&value.count))return 2;
    *out=value;return 0;
}
uint32_t dh2_pynames_copy(const struct dh2_pynames_view *view,struct dh2_pyname *out,uint32_t capacity) {
    if(!view || (!out && capacity) || capacity>UINT32_MAX/sizeof(*out))return 1;
    size_t size=(size_t)capacity*sizeof(*out);
    if(overlap(out,size,view,sizeof(*view)) || overlap(out,size,view->bytes,view->size))return 1;
    if(!valid(view) || capacity<view->count)return 2;
    if(!walk(view,out,NULL,0,NULL,NULL))return 2;
    return 0;
}
uint32_t dh2_pynames_get(const struct dh2_pynames_view *view,const void *name,uint32_t length,int32_t *out) {
    if(!view || !out || (!name && length) || length>255 ||
       overlap(out,sizeof(*out),view,sizeof(*view)) || overlap(out,sizeof(*out),view->bytes,view->size) ||
       overlap(out,sizeof(*out),name,length) || (length && memchr(name,0,length)))return 1;
    if(!valid(view))return 2;
    int32_t index=-1;
    if(!walk(view,NULL,name,length,&index,NULL))return 2;
    *out=index;return 0;
}
