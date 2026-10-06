#include "script_scalar_bindings.h"
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include <string.h>
#include <stdio.h>
#include <limits.h>
enum { TO_FIXED,FROM_FIXED,MUL_FIXED,DIV_FIXED,BIT_NOT,BIT_AND,BIT_OR,BIT_XOR };
static int fail(char* e,size_t n,const char* message) {
  if(e&&n)snprintf(e,n,"%s",message);return 1;
}
static int32_t signed_word(uint32_t u) {int32_t i;memcpy(&i,&u,4);return i;}
static uint32_t arithmetic_shift8(uint32_t v) {
  return (v>>8)|((v&0x80000000u)?0xff000000u:0);
}
/* Explicit imported __aeabi_f2iz contract used by original instruction oracle:
 * truncate toward zero, saturate finite/Infinity, NaN0. No undefined C casts. */
static int32_t integer(float f) {
  uint32_t bits,exp,magnitude;memcpy(&bits,&f,4);exp=(bits>>23)&255;
  if(exp==255&&(bits&0x7fffff))return 0;
  if(exp<127)return 0;
  if(exp>=158)return bits>>31?INT32_MIN:INT32_MAX;
  magnitude=((bits&0x7fffff)|0x800000u);
  magnitude=exp>=150?magnitude<<(exp-150):magnitude>>(150-exp);
  return bits>>31?-(int32_t)magnitude:(int32_t)magnitude;
}
typedef struct {const char* text;size_t size;float number;} NumberText;
static int parse_number(lua_State* L) {
  NumberText* p=(NumberText*)lua_touserdata(L,1);
  lua_pushlstring(L,p->text,p->size);p->number=lua_tonumber(L,-1);return 0;
}
static int number(const dh2_script_scalar_bindings* s,const dh2_script_value* v,
  float* out,char* e,size_t ec) {
  uint32_t identity;NumberText p;const char* zero;lua_State* L;int status;
  switch(v->type) {
    case DH2_SCRIPT_BOOLEAN:
      if(v->boolean>1)return fail(e,ec,"invalid source boolean");
      *out=(float)v->boolean;return 0;
    case DH2_SCRIPT_NUMBER:*out=v->number;return 0;
    case DH2_SCRIPT_IDENTITY:case 7:
      if(!v->identity){*out=0;return 0;}
      if(!s||!s->identity||s->identity(s->context,v->identity,&identity))
        return fail(e,ec,"source numeric identity service unavailable");
      *out=(float)identity;return 0;
    case DH2_SCRIPT_STRING:
      if(!v->text)return fail(e,ec,"invalid source string");
      zero=(const char*)memchr(v->text,0,v->text_bytes);
      p.text=v->text;p.size=zero?(size_t)(zero-v->text):v->text_bytes;p.number=0;
      L=luaL_newstate();if(!L)return fail(e,ec,"source temporary Lua allocation failed");
      status=lua_cpcall(L,parse_number,&p);lua_close(L);
      if(status)return fail(e,ec,"source temporary Lua number conversion failed");
      *out=p.number;return 0;
    default:*out=0;return 0;
  }
}
static void result(dh2_script_value* out,float f) {
  memset(out,0,sizeof(*out));out->type=DH2_SCRIPT_NUMBER;out->number=f;
}
static int invoke(unsigned op,void* context,const dh2_script_value* a,uint32_t n,
  dh2_script_value* out,uint32_t cap,uint32_t* returned,char* e,size_t ec) {
  const dh2_script_scalar_bindings* s=(const dh2_script_scalar_bindings*)context;
  uint32_t i,needed=op==FROM_FIXED?2:1,word=0;int32_t x,y;float f,g;
  if(!returned||(!a&&n))return fail(e,ec,"invalid source scalar arguments");
  *returned=0;
  if((op<=FROM_FIXED&&n<1)||(op>=MUL_FIXED&&op<=DIV_FIXED&&n<2))return 0;
  if(op==BIT_NOT&&(n!=1||a[0].type!=3))return 0;
  if(op==BIT_XOR&&(n!=2||a[0].type!=3||a[1].type!=3))return 0;
  if(op==BIT_AND||op==BIT_OR) {
    if(n<2)return 0;
    for(i=0;i<n;++i)if(a[i].type!=3)return 0;
  }
  if((s&&s->reserved)||!out||cap<needed)return fail(e,ec,"invalid source scalar result/services");
  if(number(s,a,&f,e,ec))return 1;x=integer(f);
  if(op==FROM_FIXED) {
    /* Source calls Value.getNumber twice, before each ordered return push. */
    result(out,(float)signed_word(arithmetic_shift8((uint32_t)x)));
    if(number(s,a,&g,e,ec))return 1;
    result(out+1,(float)integer(g)*0x1p-8f);*returned=2;return 0;
  }
  if(op==TO_FIXED)word=(uint32_t)x<<8;
  else if(op==BIT_NOT)word=~(uint32_t)x;
  else if(op==BIT_AND||op==BIT_OR) {
    word=(uint32_t)x;
    for(i=1;i<n;++i) {
      if(number(s,a+i,&g,e,ec))return 1;y=integer(g);
      word=op==BIT_AND?word&(uint32_t)y:word|(uint32_t)y;
    }
  } else {
    if(number(s,a+1,&g,e,ec))return 1;y=integer(g);
    if(op==MUL_FIXED)word=arithmetic_shift8((uint32_t)x*(uint32_t)y);
    else if(op==BIT_XOR)word=(uint32_t)x^(uint32_t)y;
    else {
      y=signed_word(arithmetic_shift8((uint32_t)y));
      if(!y) {
        if(!s||!s->divide_zero||s->divide_zero(s->context,x,&x))
          return fail(e,ec,"source __aeabi_idiv zero-divisor service unavailable");
      } else if(x==INT32_MIN&&y==-1)x=INT32_MIN;
      else x/=y;
      word=(uint32_t)x;
    }
  }
  result(out,(float)signed_word(word));*returned=1;return 0;
}
#define CALLBACK(name,op) int dh2_script_scalar_##name(void* c,const dh2_script_value* a,uint32_t n,dh2_script_value* o,uint32_t cap,uint32_t* r,char* e,size_t ec){return invoke(op,c,a,n,o,cap,r,e,ec);}
CALLBACK(to_fixed,TO_FIXED)
CALLBACK(from_fixed,FROM_FIXED)
CALLBACK(mul_fixed,MUL_FIXED)
CALLBACK(div_fixed,DIV_FIXED)
CALLBACK(bit_not,BIT_NOT)
CALLBACK(bit_and,BIT_AND)
CALLBACK(bit_or,BIT_OR)
CALLBACK(bit_xor,BIT_XOR)
#undef CALLBACK
int dh2_script_scalar_bind(dh2_script_vm* vm,const dh2_script_scalar_bindings* s) {
  static const char* names[]={"ToFixed","FromFixed","MulFixed","DivFixed","BitNot","BitAnd","BitOr","BitXOr"};
  static const dh2_script_function callbacks[]={dh2_script_scalar_to_fixed,dh2_script_scalar_from_fixed,dh2_script_scalar_mul_fixed,dh2_script_scalar_div_fixed,dh2_script_scalar_bit_not,dh2_script_scalar_bit_and,dh2_script_scalar_bit_or,dh2_script_scalar_bit_xor};
  unsigned i;int status;if(!vm||(s&&s->reserved))return -1;
  for(i=0;i<8;++i) {
    status=dh2_script_vm_bind_source_values(vm,names[i],callbacks[i],(void*)s);
    if(status)return status;
  }
  return 0;
}
