#include "script_design_bindings.h"
#include <stdio.h>
#include <string.h>

static int fail(char* text,size_t capacity,const char* message) {
  if(text&&capacity)snprintf(text,capacity,"%s",message);
  return 1;
}

static int query(uint32_t kind,void* context,const dh2_script_value* values,
    uint32_t count,dh2_script_value* output,uint32_t capacity,
    uint32_t* returned,char* error,size_t error_capacity) {
  const dh2_script_design_bindings* source=(const dh2_script_design_bindings*)context;
  int32_t value;
  if(!returned||(!values&&count))
    return fail(error,error_capacity,"invalid design callback arguments");
  *returned=0;
  /* Actual wrappers require at least two exact string Values. They do not
   * coerce numeric names, validate unused arguments or return a nil on a guard. */
  if(count<2||values[0].type!=DH2_SCRIPT_STRING||values[1].type!=DH2_SCRIPT_STRING)
    return 0;
  if(!source||!source->lookup||source->reserved||!values[0].text||
      !values[1].text||!output||capacity<1)
    return fail(error,error_capacity,"invalid design lookup services/results");
  if(source->lookup(source->context,kind,values[0].text,values[1].text,&value))
    return fail(error,error_capacity,"source design lookup delivery failed");
  memset(output,0,sizeof(*output));
  output->type=DH2_SCRIPT_NUMBER;
  output->number=(float)value; /* ReturnValues.pushInteger signed32 -> float32. */
  *returned=1;
  return 0;
}

#define CALLBACK(name,kind) \
int dh2_script_design_get_##name(void* context,const dh2_script_value* values, \
    uint32_t count,dh2_script_value* output,uint32_t capacity,uint32_t* returned, \
    char* error,size_t error_capacity) { \
  return query(kind,context,values,count,output,capacity,returned,error,error_capacity); \
}
CALLBACK(constant,0)
CALLBACK(struct,1)
CALLBACK(oid,1)
#undef CALLBACK

int dh2_script_design_bind(dh2_script_vm* vm,const dh2_script_design_bindings* source) {
  int status;
  if(!vm||!source||!source->lookup||source->reserved)return -1;
  status=dh2_script_vm_bind_source_values(vm,"GetPyCst",dh2_script_design_get_constant,(void*)source);
  if(status)return status;
  status=dh2_script_vm_bind_source_values(vm,"GetPyStruct",dh2_script_design_get_struct,(void*)source);
  if(status)return status;
  return dh2_script_vm_bind_source_values(vm,"GetPyOID",dh2_script_design_get_oid,(void*)source);
}
