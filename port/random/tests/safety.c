#include "../random.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
int main(void) {
    struct dh2_random_state s={{0xffffffff,0x80000000},{0xffffffff,0xfffffffe}};
    uint32_t seed=17,tags[32];float args[32];struct dh2_random_result out,before;
    for(unsigned i=0;i<32;++i) {tags[i]=3;args[i]=(float)(i*31);}
    for(unsigned i=0;i<12000;++i) {
        struct dh2_random_state saved=s;uint32_t original_seed=seed;
        memset(&out,0xa5,sizeof(out));before=out;args[0]=i&1?INFINITY:NAN;
        assert(dh2_random_callback(&s,&seed,i&1,args,tags,1,&out)==2);
        assert(!memcmp(&s,&saved,sizeof(s)) && seed==original_seed && !memcmp(&out,&before,sizeof(out)));
        assert(dh2_random_callback(&s,&seed,0,args,tags,33,&out)==1);
        assert(dh2_random_callback(&s,s.seeds,0,args,tags,0,&out)==1);
        assert(dh2_random_callback(&s,&seed,0,args,tags,0,(void *)&s)==1);
        assert(dh2_random_callback(&s,&seed,0,(void *)&s,tags,1,&out)==1);
        args[0]=i&1? -1000000.5f:4294967040.0f;args[1]=(float)(i%997);
        assert(!dh2_random_callback(&s,&seed,i&1,args,tags,2,&out) && out.count==1);
        saved=s;assert(dh2_random_next(&s,0,i&1)==0);
        assert(s.seeds[0]==saved.seeds[0] && s.seeds[1]==saved.seeds[1]);
        assert(s.counters[i&1]==saved.counters[i&1]+1u);
        uint32_t value=(uint32_t)dh2_random_next(&s,i%997+1,i&1);assert(value<i%997+1);
        tags[0]=1;saved=s;original_seed=seed;
        assert(!dh2_random_callback(&s,&seed,1,args,tags,1,&out) && !out.count);
        assert(!memcmp(&s,&saved,sizeof(s)) && seed==original_seed);tags[0]=3;
        assert(!dh2_random_callback(&s,&seed,0,args,tags,32,&out) && out.value>=0 && out.value<100);
    }
    puts("RANDOM SAFETY PASS 12000");return 0;
}
