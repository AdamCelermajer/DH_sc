#include "character_property_bindings.hpp"
#include <cstring>
#include <cstdio>
#include <cstddef>
namespace {
using namespace dh2::character;
bool aligned(const void* p,std::size_t n){return p&&reinterpret_cast<std::uintptr_t>(p)%n==0;}
bool valid(const PropertySheet16* s){return aligned(s,alignof(PropertySheet16))&&
 !s->reserved&&s->count>=224&&s->count<=65536&&aligned(s->words,alignof(std::int32_t));}
bool overlap(const void* a,std::size_t n,const void* b,std::size_t m){
 auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
 return n&&m&&(x<=y?y-x<n:x-y<m);
}
std::uint32_t unsigned_number(float number){
 std::uint32_t bits;std::memcpy(&bits,&number,4);
 auto exponent=(bits>>23)&255,fraction=bits&0x7fffff;
 if((bits>>31)||exponent<127||(exponent==255&&fraction))return 0;
 if(exponent>158)return UINT32_MAX;
 return ((fraction<<8)|0x80000000u)>>(158-exponent);
}
int reject(char* text,std::size_t size){if(text&&size)std::snprintf(text,size,"invalid Character.GetProp property backend");return 1;}
}
extern "C" int dh2_character_property_word(std::int32_t* out,const PropertySheet16* sheet,std::int32_t property){
 if(!aligned(out,alignof(std::int32_t)))return 1;
 if(sheet&&(!valid(sheet)||overlap(out,4,sheet,sizeof(*sheet))||overlap(out,4,sheet->words,224*4)))return 1;
 if(property<0||property>=224||!sheet){*out=-1;return 0;}
 *out=sheet->words[property];return 0;
}
extern "C" int dh2_character_get_prop(void* opaque,const dh2_script_value* a,std::uint32_t n,
 dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* error,std::size_t error_size){
 if(!aligned(returned,alignof(std::uint32_t))||(n&&!aligned(a,alignof(dh2_script_value)))||n>1048576)return reject(error,error_size);
 if((n&&overlap(returned,4,a,std::size_t(n)*sizeof(*a)))||
  (capacity&&out&&overlap(returned,4,out,std::size_t(capacity)*sizeof(*out))))return reject(error,error_size);
 auto* bindings=static_cast<const PropertyBindings48*>(opaque);
 if(aligned(bindings,alignof(PropertyBindings48))&&
  (overlap(returned,4,bindings,sizeof(*bindings))||
   (valid(&bindings->resolved)&&overlap(returned,4,bindings->resolved.words,224*4))||
   (valid(&bindings->temporary)&&overlap(returned,4,bindings->temporary.words,224*4))))return reject(error,error_size);
 *returned=0;
 if(!n||a[0].type!=DH2_SCRIPT_NUMBER)return 0;
 auto property=unsigned_number(a[0].number);
 if(property>=224)return 0;
 if(n>1&&a[1].type==DH2_SCRIPT_IDENTITY&&!a[1].identity)return 0;
 if(!aligned(bindings,alignof(PropertyBindings48))||!aligned(out,alignof(dh2_script_value))||!capacity||capacity>1048576||
  overlap(out,sizeof(*out),bindings,sizeof(*bindings))||overlap(out,sizeof(*out),a,std::size_t(n)*sizeof(*a)))return reject(error,error_size);
 PropertySheet16 external{};const PropertySheet16* selected=&bindings->resolved;
 if(n>1&&a[1].type==DH2_SCRIPT_IDENTITY){
  if(!bindings->sheet||bindings->sheet(bindings->context,a[1].identity,&external))return reject(error,error_size);
  selected=&external;
 }else if(n>1&&a[1].type==DH2_SCRIPT_BOOLEAN&&a[1].boolean)selected=&bindings->temporary;
 if(!valid(selected)||overlap(out,sizeof(*out),selected->words,224*4)||overlap(returned,4,selected->words,224*4))return reject(error,error_size);
 std::int32_t word;if(dh2_character_property_word(&word,selected,static_cast<std::int32_t>(property)))return reject(error,error_size);
 dh2_script_value value{};value.type=DH2_SCRIPT_NUMBER;value.number=static_cast<float>(word);
 *out=value;*returned=1;return 0;
}
extern "C" int dh2_character_property_bind(dh2_script_vm* vm,const PropertyBindings48* bindings){
 if(!vm||!aligned(bindings,alignof(PropertyBindings48))||!valid(&bindings->resolved)||!valid(&bindings->temporary))return -1;
 return dh2_script_vm_bind_source_values(vm,"GetProp",dh2_character_get_prop,const_cast<PropertyBindings48*>(bindings));
}
