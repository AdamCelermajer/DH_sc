#include "../numeric.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static uint32_t next(uint32_t *state) { *state^=*state<<13;*state^=*state>>17;*state^=*state<<5;return *state; }
int main(void) {
    uint32_t seed=20261002;float values[8],before[8];
    for(unsigned trial=0;trial<6000;++trial) {
        for(unsigned i=0;i<8;++i) { uint32_t raw=next(&seed);memcpy(values+i,&raw,4); }
        memcpy(before,values,sizeof(values));
        for(unsigned operation=0;operation<=DH2_TRACE;++operation) {
            struct { uint32_t left;struct dh2_lua_numeric_result result;uint32_t right; } output;
            memset(&output,0xa5,sizeof(output));struct dh2_lua_numeric_result old=output.result;
            uint32_t error=dh2_lua_numeric(operation,values,trial%9,&output.result);
            assert(output.left==0xa5a5a5a5u && output.right==0xa5a5a5a5u);
            assert(!error || memcmp(&old,&output.result,sizeof(old))==0);
            assert(memcmp(before,values,sizeof(values))==0);
        }
    }
    struct dh2_lua_numeric_result result;memset(&result,0xa5,sizeof(result));
    struct dh2_lua_numeric_result old=result;
    assert(dh2_lua_numeric(DH2_TO_FIXED,(float *)&result,1,&result));assert(!memcmp(&old,&result,sizeof(old)));
    assert(dh2_lua_numeric(DH2_TRACE,NULL,257,&result));assert(!memcmp(&old,&result,sizeof(old)));
    float divide[]={-2147483648.0f,-256.0f};
    assert(dh2_lua_numeric(DH2_DIV_FIXED,divide,2,&result)==3);assert(!memcmp(&old,&result,sizeof(old)));
    puts("Numeric safety: 6000 lists / 54000 operations passed");return 0;
}
