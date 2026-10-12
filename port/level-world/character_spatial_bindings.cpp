#include "character_spatial_bindings.hpp"
#include <cstdio>
#include <cstring>
namespace {
using dh2::character::SpatialBindings40;
int fail(char* error,std::size_t capacity,const char* text){if(error&&capacity)std::snprintf(error,capacity,"%s",text);return 1;}
void push(dh2_script_value* out,float value){std::memset(out,0,sizeof(*out));out->type=DH2_SCRIPT_NUMBER;out->number=value;}
float squared(const float* first,const float* second){
 const float x=first[0]-second[0],y=first[1]-second[1],z=first[2]-second[2];
 const float xx=x*x,yy=y*y,zz=z*z;
 return (xx+yy)+zz;
}
bool valid(const SpatialBindings40* source){return source&&!source->reserved;}
}
extern "C" int dh2_character_get_position(void* pointer,const dh2_script_value* a,std::uint32_t count,
 dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* error,std::size_t ec){
 if(!returned)return fail(error,ec,"Malformed spatial result count");
 *returned=0;
 const auto* source=static_cast<const SpatialBindings40*>(pointer);
 if((count&&!a)||!valid(source)||!source->owner_position||!out||capacity<3)return fail(error,ec,"Malformed GetPosition receiver/output");
 for(unsigned i=0;i<3;++i)push(out+i,source->owner_position[i]);
 *returned=3;return 0;
}
extern "C" int dh2_character_get_distance_from(void* pointer,const dh2_script_value* a,std::uint32_t count,
 dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* error,std::size_t ec){
 if(!returned||(count&&!a))return fail(error,ec,"Malformed spatial arguments");
 *returned=0;
 if(!count||(a[0].type!=DH2_SCRIPT_STRING&&a[0].type!=7))return 0;
 const auto* source=static_cast<const SpatialBindings40*>(pointer);
 if(!valid(source)||!source->owner_position||a[0].reserved||!out||!capacity)return fail(error,ec,"Malformed GetDistanceFrom receiver/output");
 const float* target=nullptr;
 if(a[0].type==DH2_SCRIPT_STRING){
  if(!a[0].text)return fail(error,ec,"Malformed spatial string");
  if(!source->named_position)return fail(error,ec,"Named spatial resolver unavailable");
  if(source->named_position(source->context,a[0].text,&target))return fail(error,ec,"Named spatial resolver failed");
 }else if(a[0].identity){
  if(!source->userdata_position)return fail(error,ec,"Userdata spatial resolver unavailable");
  if(source->userdata_position(source->context,a[0].identity,&target))return fail(error,ec,"Userdata spatial resolver failed");
 }
 push(out,target?squared(target,source->owner_position):-1.0f);*returned=1;return 0;
}
extern "C" int dh2_character_get_distance_between(void* pointer,const dh2_script_value* a,std::uint32_t count,
 dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* error,std::size_t ec){
 if(!returned||(count&&!a))return fail(error,ec,"Malformed spatial arguments");
 *returned=0;
 if(count<2||a[0].type!=DH2_SCRIPT_STRING||a[1].type!=DH2_SCRIPT_STRING)return 0;
 const auto* source=static_cast<const SpatialBindings40*>(pointer);
 if(!valid(source)||a[0].reserved||a[1].reserved||!a[0].text||!a[1].text||!out||!capacity)return fail(error,ec,"Malformed GetDistanceBetween receiver/output");
 if(!source->named_position)return fail(error,ec,"Named spatial resolver unavailable");
 const float *first=nullptr,*second=nullptr;
 if(source->named_position(source->context,a[0].text,&first))return fail(error,ec,"Named spatial resolver failed");
 // Original resolves second even when first is missing; do not read/copy first
 // coordinates until this synchronous resolution and its side effects finish.
 if(source->named_position(source->context,a[1].text,&second))return fail(error,ec,"Named spatial resolver failed");
 push(out,first&&second?squared(first,second):-1.0f);*returned=1;return 0;
}
extern "C" int dh2_character_spatial_bind(dh2_script_vm* vm,const SpatialBindings40* source){
 if(!vm||!valid(source)||!source->owner_position)return -1;
 const char* names[]={"GetPosition","GetDistanceFrom","GetDistanceBetween"};
 const dh2_script_function callbacks[]={dh2_character_get_position,dh2_character_get_distance_from,dh2_character_get_distance_between};
 for(unsigned i=0;i<3;++i){int status=dh2_script_vm_bind_source_values(vm,names[i],callbacks[i],const_cast<SpatialBindings40*>(source));if(status)return status;}return 0;
}
