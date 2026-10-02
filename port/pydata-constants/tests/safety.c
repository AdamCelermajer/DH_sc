#include "../constants.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static uint32_t seed=20261002;
static uint32_t random32(void) { seed=1664525u*seed+1013904223u;return seed; }
int main(void) {
    const unsigned char fixture[]={1,0,0,0,1,0,0,0,'G',2,0,0,0,
        1,0,0,0,'K',1,0,0,0,1,0,0,0,'K',0xff,0xff,0xff,0xff};
    unsigned char raw[512],saved[512];
    for(unsigned i=0;i<12000;++i) {
        memset(raw,0,sizeof(raw));memcpy(raw,fixture,sizeof(fixture));
        uint32_t size=sizeof(fixture);
        if(i%4==0)size=random32()%sizeof(fixture);
        if(i%4==1)raw[random32()%sizeof(fixture)]=(unsigned char)random32();
        if(i%4==2) { size=random32()%512;for(unsigned j=0;j<size;++j)raw[j]=(unsigned char)random32(); }
        memcpy(saved,raw,sizeof(raw));
        struct dh2_pycst_view view;memset(&view,0xa5,sizeof(view));
        struct dh2_pycst_view before=view;
        uint32_t status=dh2_pycst_open(&view,raw,size);
        assert(memcmp(raw,saved,sizeof(raw))==0);
        if(status) { assert(memcmp(&view,&before,sizeof(view))==0);continue; }
        struct dh2_pycst_entry entries[64];memset(entries,0xa5,sizeof(entries));
        struct dh2_pycst_entry entry_before=entries[0];
        assert(dh2_pycst_entry_at(&view,view.entries,entries)!=0);
        assert(memcmp(entries,&entry_before,sizeof(entry_before))==0);
        assert(dh2_pycst_copy_entries(&view,entries,64)==0);
        struct dh2_pycst_result value={7,9};
        assert(dh2_pycst_get(&view,"G",1,"K",1,&value)==0);
        if(i%4==3) { assert(value.found && value.value==-1);assert(view.entries==2); }
        struct dh2_pycst_result old=value;
        assert(dh2_pycst_get(&view,NULL,1,"K",1,&value)!=0);
        assert(memcmp(&value,&old,sizeof(value))==0);
        assert(dh2_pycst_copy_entries(&view,(void *)raw,1)!=0);
        assert(memcmp(raw,saved,sizeof(raw))==0);
        view.entries++;
        assert(dh2_pycst_get(&view,"G",1,"K",1,&value)!=0);
        assert(memcmp(&value,&old,sizeof(value))==0);
    }
    puts("Constants safety: 12000 valid/damaged input iterations passed");return 0;
}
