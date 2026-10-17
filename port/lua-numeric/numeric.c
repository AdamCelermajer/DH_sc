#include "numeric.h"
#include <math.h>
#include <stddef.h>
#include <string.h>
static int32_t signed_bits(uint32_t bits) { int32_t out;memcpy(&out,&bits,4);return out; }
static int32_t shift8(int32_t value) {
    uint32_t bits=(uint32_t)value;
    return signed_bits((bits>>8)|((bits&0x80000000u)?0xff000000u:0));
}
static int convert(float number,int32_t *out) {
    if (!isfinite(number) || number< -2147483648.0f || number>=2147483648.0f)return 0;
    *out=(int32_t)number;return 1;
}
uint32_t dh2_lua_numeric(uint32_t operation,const float *operands,uint32_t count,
                          struct dh2_lua_numeric_result *result) {
    if (!result || operation>DH2_TRACE || count>256 || (!operands && count))return 1;
    uintptr_t begin=(uintptr_t)operands,end=begin+(size_t)count*sizeof(float);
    uintptr_t out=(uintptr_t)result,out_end=out+sizeof(*result);
    if(end<begin || out_end<out || (count && begin<out_end && out<end))return 1;
    uint32_t needed=0;
    switch(operation) {
    case DH2_TO_FIXED:case DH2_FROM_FIXED:needed=count?1:0;break;
    case DH2_MUL_FIXED:case DH2_DIV_FIXED:needed=count>=2?2:0;break;
    case DH2_BIT_NOT:needed=count==1?1:0;break;
    case DH2_BIT_XOR:needed=count==2?2:0;break;
    case DH2_BIT_AND:case DH2_BIT_OR:needed=count>=2?count:0;break;
    case DH2_TRACE:break;
    }
    struct dh2_lua_numeric_result value={0,0,0};
    if(!needed) { *result=value;return 0; }
    int32_t first=0,second=0;
    if (!convert(operands[0],&first))return 2;
    if (needed>=2 && !convert(operands[1],&second))return 2;
    value.count=1;
    switch(operation) {
    case DH2_TO_FIXED:value.integer=signed_bits((uint32_t)first<<8);break;
    case DH2_FROM_FIXED:
        value.count=2;value.integer=shift8(first);value.number=(float)first*0.00390625f;break;
    case DH2_MUL_FIXED:value.integer=shift8(signed_bits((uint32_t)first*(uint32_t)second));break;
    case DH2_DIV_FIXED:
        second=shift8(second);if(!second || (first==INT32_MIN && second==-1))return 3;
        value.integer=first/second;break;
    case DH2_BIT_NOT:value.integer=signed_bits(~(uint32_t)first);break;
    case DH2_BIT_XOR:value.integer=signed_bits((uint32_t)first^(uint32_t)second);break;
    case DH2_BIT_AND:case DH2_BIT_OR: {
        uint32_t bits=(uint32_t)first;
        for(uint32_t i=1;i<count;++i) {
            if(!convert(operands[i],&second))return 2;
            if(operation==DH2_BIT_AND)bits&=(uint32_t)second;else bits|=(uint32_t)second;
        }
        value.integer=signed_bits(bits);break;
    }
    case DH2_TRACE:break;
    }
    *result=value;return 0;
}
