#include "../names.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static uint32_t seed=20261002;
static uint32_t random32(void) { seed=1664525u*seed+1013904223u;return seed; }
int main(void) {
    const unsigned char fixture[]={3,0,0,0,3,0,0,0,'d','u','p',5,0,0,0,'o','t','h','e','r',3,0,0,0,'d','u','p'};
    unsigned char raw[512],saved[512];
    for(unsigned i=0;i<10000;++i) {
        memset(raw,0,sizeof(raw));memcpy(raw,fixture,sizeof(fixture));uint32_t size=sizeof(fixture);
        if(i%4==0)size=random32()%sizeof(fixture);
        if(i%4==1)raw[random32()%sizeof(fixture)]=(unsigned char)random32();
        if(i%4==2) { size=random32()%512;for(unsigned j=0;j<size;++j)raw[j]=(unsigned char)random32(); }
        memcpy(saved,raw,sizeof(raw));
        struct dh2_pynames_view view;memset(&view,0xa5,sizeof(view));struct dh2_pynames_view old=view;
        uint32_t status=dh2_pynames_open(&view,raw,size);
        assert(memcmp(raw,saved,sizeof(raw))==0);
        if(status) { assert(memcmp(&view,&old,sizeof(view))==0);continue; }
        struct dh2_pyname entries[128];memset(entries,0xa5,sizeof(entries));
        assert(dh2_pynames_copy(&view,entries,128)==0);
        int32_t index=123;assert(dh2_pynames_get(&view,"dup",3,&index)==0);
        if(i%4==3) { assert(index==0);assert(view.count==3); }
        int32_t before=index;
        assert(dh2_pynames_get(&view,NULL,1,&index)!=0);assert(index==before);
        assert(dh2_pynames_get(&view,"other",5,(void *)raw)!=0);
        assert(dh2_pynames_copy(&view,(void *)raw,1)!=0);assert(memcmp(raw,saved,sizeof(raw))==0);
        view.count++;assert(dh2_pynames_get(&view,"dup",3,&index)!=0);assert(index==before);
    }
    puts("Name safety: 10000 valid/damaged input iterations passed");return 0;
}
