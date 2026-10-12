#include "script_int_bindings.hpp"
extern "C" {
#include "lua/lua.h"
#include "lua/lauxlib.h"
}
#include <map>
#include <string>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <climits>
#include <iterator>
struct dh2_script_int_map {std::map<uint32_t,int32_t> values;};
struct dh2_script_string_projection {std::string converted;};
namespace {
bool services_valid(const dh2_script_int_bindings* s){return !s||!s->reserved;}
int32_t signed_integer(float f){
 uint32_t b;std::memcpy(&b,&f,4);const uint32_t exp=(b>>23)&255;
 if(exp==255&&(b&0x7fffff))return 0;
 if(exp<127)return 0;if(exp>=158)return b>>31?INT32_MIN:INT32_MAX;
 uint32_t m=(b&0x7fffff)|0x800000;m=exp>=150?m<<(exp-150):m>>(150-exp);
 return b>>31?-static_cast<int32_t>(m):static_cast<int32_t>(m);
}
int identity(const dh2_script_int_bindings* s,uintptr_t original,uint32_t* out){
 if(!original){*out=0;return 0;}
 return !s||!s->identity?-3:s->identity(s->context,original,out)?-3:0;
}
struct NumericText {const char* text;float result;};
int number_text(lua_State* L){auto* p=static_cast<NumericText*>(lua_touserdata(L,1));lua_pushstring(L,p->text);p->result=lua_tonumber(L,-1);return 0;}
int number_value(const dh2_script_int_bindings* s,const dh2_script_value* v,float* out){
 uint32_t source_identity=0;if(!v||v->reserved||!services_valid(s))return -1;
 switch(v->type){
 case 1:if(v->boolean>1)return -1;*out=static_cast<float>(v->boolean);return 0;
 case 3:*out=v->number;return 0;
 case 2:case 7:{int status=identity(s,v->identity,&source_identity);if(status)return status;*out=static_cast<float>(source_identity);return 0;}
 case 4:{if(!v->text)return -1;lua_State* L=luaL_newstate();if(!L)return -2;NumericText p{v->text,0};int status=lua_cpcall(L,number_text,&p);lua_close(L);if(status)return -2;*out=p.result;return 0;}
 default:*out=0;return 0;
 }
}
int callback_failure(char* e,size_t n,int status){if(e&&n)std::snprintf(e,n,"source private integer operation failed (%d)",status);return 1;}
}
extern "C" dh2_script_int_map* dh2_script_int_create(void){try{return new dh2_script_int_map;}catch(...){return nullptr;}}
extern "C" void dh2_script_int_destroy(dh2_script_int_map* m){delete m;}
extern "C" int dh2_script_int_clear_contents(dh2_script_int_map* m){if(!m)return -1;m->values.clear();return 0;}
extern "C" uint32_t dh2_script_int_hash(const char* s){
 if(!s)return 0;uint32_t h=0;for(auto* p=reinterpret_cast<const unsigned char*>(s);*p;++p){uint32_t c=static_cast<uint32_t>(*p<128?int(*p):int(*p)-256);h^=c+0x9e3779b9u+(h<<6)+(h>>2);}return h;
}
extern "C" int dh2_script_int_set(dh2_script_int_map* m,const char* s,int32_t v){if(!m||!s)return -1;try{m->values[dh2_script_int_hash(s)]=v;return 0;}catch(...){return -2;}}
extern "C" int dh2_script_int_get(dh2_script_int_map* m,const char* s,int32_t* v){if(!m||!s||!v)return -1;try{*v=m->values[dh2_script_int_hash(s)];return 0;}catch(...){return -2;}}
extern "C" size_t dh2_script_int_size(const dh2_script_int_map* m){return m?m->values.size():0;}
extern "C" int dh2_script_int_entry(const dh2_script_int_map* m,size_t index,uint32_t* key,int32_t* value){if(!m||!key||!value||index>=m->values.size())return -1;auto i=m->values.begin();std::advance(i,index);*key=i->first;*value=i->second;return 0;}
extern "C" dh2_script_string_projection* dh2_script_string_projection_create(void){try{return new dh2_script_string_projection;}catch(...){return nullptr;}}
extern "C" void dh2_script_string_projection_destroy(dh2_script_string_projection* p){delete p;}
extern "C" int dh2_script_value_get_string(dh2_script_string_projection* p,const dh2_script_int_bindings* s,const dh2_script_value* v,const char** out){
 if(!p||!v||!out||v->reserved||!services_valid(s))return -1;*out=nullptr;
 try{
  switch(v->type){
  case 0:*out="nil";return 0;
  case 1:if(v->boolean>1)return -1;*out=v->boolean?"true":"false";return 0;
  case 4:if(!v->text)return -1;*out=v->text;return 0;
  case 3:{
   char buffer[32]={};
   if(std::floor(v->number)==v->number)std::snprintf(buffer,sizeof buffer,"%d",signed_integer(v->number));
   else{
    size_t bytes=0;if(!s||!s->format_fraction)return -3;
    if(s->format_fraction(s->context,v->number,buffer,sizeof buffer,&bytes))return -3;
    if(bytes>=sizeof buffer||buffer[bytes]||std::strlen(buffer)!=bytes)return -1;
   }
   p->converted=buffer;*out=p->converted.c_str();return 0;
  }
  case 2:case 7:{
   uint32_t source_identity=0;int status=identity(s,v->identity,&source_identity);if(status)return status;
   char buffer[32]={};std::snprintf(buffer,sizeof buffer,"0x%0*x",8,source_identity);p->converted=buffer;*out=p->converted.c_str();return 0;
  }
  default:return 0;
  }
 }catch(...){return -2;}
}
extern "C" int dh2_script_value_get_integer(const dh2_script_int_bindings* s,const dh2_script_value* v,int32_t* out){if(!out)return -1;float n=0;int status=number_value(s,v,&n);if(status)return status;*out=signed_integer(n);return 0;}
extern "C" int dh2_script_int_set_callback(void* opaque,const dh2_script_value* a,uint32_t count,dh2_script_value*,uint32_t,uint32_t* returned,char* error,size_t capacity){
 if(!returned||(!a&&count))return callback_failure(error,capacity,-1);*returned=0;if(count<2)return 0;
 auto* s=static_cast<const dh2_script_int_bindings*>(opaque);if(!s||!s->map||s->reserved)return callback_failure(error,capacity,-1);
 dh2_script_string_projection p;const char* name=nullptr;int32_t value=0;
 int status=dh2_script_value_get_string(&p,s,a,&name);if(!status&&!name)status=-4;
 if(!status)status=dh2_script_value_get_integer(s,a+1,&value);
 if(!status)status=dh2_script_int_set(s->map,name,value);
 return status?callback_failure(error,capacity,status):0;
}
extern "C" int dh2_script_int_get_callback(void* opaque,const dh2_script_value* a,uint32_t count,dh2_script_value* out,uint32_t capacity,uint32_t* returned,char* error,size_t ec){
 if(!returned||(!a&&count))return callback_failure(error,ec,-1);*returned=0;if(!count)return 0;
 auto* s=static_cast<const dh2_script_int_bindings*>(opaque);if(!s||!s->map||s->reserved||!out||capacity<1)return callback_failure(error,ec,-1);
 dh2_script_string_projection p;const char* name=nullptr;int32_t value=0;
 int status=dh2_script_value_get_string(&p,s,a,&name);if(!status&&!name)status=-4;
 if(!status)status=dh2_script_int_get(s->map,name,&value);
 if(status)return callback_failure(error,ec,status);
 std::memset(out,0,sizeof(*out));out->type=3;out->number=static_cast<float>(value);*returned=1;return 0;
}
extern "C" int dh2_script_int_bind(dh2_script_vm* vm,const dh2_script_int_bindings* s){
 if(!vm||!s||!s->map||s->reserved)return -1;
 int status=dh2_script_vm_bind_source_values(vm,"SetInt",dh2_script_int_set_callback,const_cast<dh2_script_int_bindings*>(s));if(status)return status;
 return dh2_script_vm_bind_source_values(vm,"GetInt",dh2_script_int_get_callback,const_cast<dh2_script_int_bindings*>(s));
}
