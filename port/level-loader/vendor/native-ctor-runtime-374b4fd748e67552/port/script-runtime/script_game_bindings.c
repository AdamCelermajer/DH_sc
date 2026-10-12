#include "script_game_bindings.h"
#include <string.h>
#include <stdio.h>
static int valid(const dh2_script_game_bindings* s) {
  return s&&s->character&&s->start&&s->stop&&!s->reserved;
}
/* Actual original __aeabi_f2uiz8be2a0, including saturation/NaN policy.
 * A native C cast would be undefined outside the uint32 range. */
static uint32_t unsigned_number(float n) {
  uint32_t bits,exponent,fraction;memcpy(&bits,&n,4);
  exponent=(bits>>23)&255;fraction=bits&0x7fffff;
  if((bits>>31)||exponent<127)return 0;
  if(exponent==255&&fraction)return 0;
  if(exponent>158)return UINT32_MAX;
  return ((fraction<<8)|0x80000000u)>>(158-exponent);
}
static int source_bool(const dh2_script_value* v) {
  switch(v->type) {
    case 1:return v->boolean!=0;
    case 3:return v->number!=0.0f;
    case 2:case 7:return v->identity!=0;
    case 4:return 1;
    default:return 0;
  }
}
static int reject(char* text,size_t capacity) {
  if(text&&capacity)snprintf(text,capacity,"invalid character timer services");return 1;
}
int dh2_script_game_start_timer(void* opaque,const dh2_script_value* a,uint32_t n,
  dh2_script_value* out,uint32_t capacity,uint32_t* returned,char* error,size_t error_capacity) {
  const dh2_script_game_bindings* s=(const dh2_script_game_bindings*)opaque;int32_t id;
  if(!returned||(!a&&n))return reject(error,error_capacity);
  *returned=0;
  if(!n||a[0].type!=3)return 0;
  if(!valid(s)||!out||capacity<1)return reject(error,error_capacity);
  id=s->start(s->context,s->character,unsigned_number(a[0].number),
    n>1&&source_bool(&a[1])?-1:0,0x35,0);
  if(id!=-1) {
    memset(out,0,sizeof(*out));out->type=3;out->number=(float)id;*returned=1;
  }
  return 0;
}
int dh2_script_game_stop_timer(void* opaque,const dh2_script_value* a,uint32_t n,
  dh2_script_value* out,uint32_t capacity,uint32_t* returned,char* error,size_t error_capacity) {
  const dh2_script_game_bindings* s=(const dh2_script_game_bindings*)opaque;(void)out;(void)capacity;
  if(!returned||(!a&&n))return reject(error,error_capacity);
  *returned=0;
  if(!n||a[0].type!=3)return 0;
  if(!valid(s))return reject(error,error_capacity);
  s->stop(s->context,s->character,unsigned_number(a[0].number));return 0;
}
int dh2_script_game_trace(void* context,const dh2_script_value* a,uint32_t n,
  dh2_script_value* out,uint32_t capacity,uint32_t* returned,char* error,size_t error_capacity) {
  (void)context;(void)a;(void)n;(void)out;(void)capacity;(void)error;(void)error_capacity;
  if(!returned)return 1;*returned=0;return 0;
}
int dh2_script_game_bind(dh2_script_vm* vm,const dh2_script_game_bindings* s) {
  int status;if(!vm||!valid(s))return -1;
  status=dh2_script_vm_bind_source_values(vm,"StartTimer",dh2_script_game_start_timer,(void*)s);
  if(status)return status;
  status=dh2_script_vm_bind_source_values(vm,"StopTimer",dh2_script_game_stop_timer,(void*)s);
  if(status)return status;
  return dh2_script_vm_bind_source_values(vm,"Trace",dh2_script_game_trace,NULL);
}
int dh2_script_game_on_timer(dh2_script_vm* vm,uint32_t id) {
  int32_t signed_id;dh2_script_value arg={0};
  memcpy(&signed_id,&id,4);arg.type=3;arg.number=(float)signed_id;
  return dh2_script_vm_call_discard_source(vm,"OnTimer",&arg,1);
}
