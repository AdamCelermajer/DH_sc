#include "../classes.h"
#include <stdio.h>
#include <string.h>
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"class safety line %d: %s\n",__LINE__,#x);return 1; } }while(0)
static unsigned rng=20261002;
static unsigned next(void) { rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng; }
static void put(unsigned char *p,unsigned n) { for(unsigned i=0;i<4;++i)p[i]=(unsigned char)(n>>(8*i)); }
int main(void) {
    unsigned char props[1804]={0};put(props,2);struct dh2_property_table pt;CHECK(!dh2_property_open(&pt,props,sizeof(props)));
    unsigned char raw[328];struct dh2_character_props s,before;struct dh2_property_sheet temp,temp_before;
    for(unsigned n=0;n<12000;++n) {
        memset(raw,0,sizeof(raw));put(raw,1);put(raw+4,16);
        for(unsigned i=0;i<sizeof(s)/4;++i) { unsigned v=next();memcpy((unsigned char *)&s+4*i,&v,4); }
        for(unsigned i=0;i<224;++i) { unsigned v=next();memcpy(temp.values+i,&v,4);put(props+900+i*4,next()%64); }
        for(unsigned i=0;i<16;++i) {
            unsigned char *p=raw+8+i*20;unsigned op=next()%10;
            put(p,next()%224);put(p+4,op);put(p+8,(op==0)?4096:((op==4||op==5||op==6)?next()%224:next()));
            put(p+12,op==1?next()%224:next());put(p+16,next());
        }
        struct dh2_class_table ct;CHECK(!dh2_class_open(&ct,raw,sizeof(raw)));before=s;temp_before=temp;
        CHECK(!dh2_class_apply(&ct,&pt,&s,next()%5,&temp,0,next()%2));
        before=s;temp_before=temp;
        CHECK(dh2_class_apply(&ct,&pt,&s,5,&temp,0,0)==1);CHECK(!memcmp(&s,&before,sizeof(s)));
        struct dh2_class_table bad=ct;bad.count=2;CHECK(dh2_class_apply(&bad,&pt,&s,4,&temp,0,0)==2);
        CHECK(!memcmp(&s,&before,sizeof(s)) && !memcmp(&temp,&temp_before,sizeof(temp)));
        struct dh2_class_table sentinel;memset(&sentinel,0xa5,sizeof(sentinel));struct dh2_class_table saved=sentinel;
        CHECK(dh2_class_open(&sentinel,raw,sizeof(raw)-1)==2);CHECK(!memcmp(&sentinel,&saved,sizeof(saved)));
        CHECK(dh2_class_apply(&ct,&pt,&s,4,&s.base,0,0)==1);CHECK(!memcmp(&s,&before,sizeof(s)));
        CHECK(dh2_class_open((struct dh2_class_table *)raw,raw,sizeof(raw))==1);
        put(raw+4,1);put(raw+8,0);put(raw+12,0);put(raw+16,0);put(raw+20,-1u);put(raw+24,-1u);
        CHECK(!dh2_class_open(&ct,raw,28));CHECK(dh2_class_apply(&ct,&pt,&s,4,&temp,0,0)==2);
        CHECK(!memcmp(&s,&before,sizeof(s)) && !memcmp(&temp,&temp_before,sizeof(temp)));
        put(raw+8,224);put(raw+12,9);CHECK(dh2_class_apply(&ct,&pt,&s,4,&temp,0,0)==2);
        CHECK(!memcmp(&s,&before,sizeof(s)) && !memcmp(&temp,&temp_before,sizeof(temp)));
        CHECK(!dh2_class_apply(&ct,&pt,&s,4,&temp,-1,0));CHECK(!dh2_class_apply(&ct,&pt,&s,4,&temp,1,0));
        CHECK(!memcmp(&s,&before,sizeof(s)) && !memcmp(&temp,&temp_before,sizeof(temp)));
    }
    puts("CLASS SAFETY PASS 12000");return 0;
}
