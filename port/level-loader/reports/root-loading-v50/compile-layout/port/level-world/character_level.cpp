#include "character_level.hpp"
#include <cstring>
#include <cstdio>
#include <limits>
namespace {
using namespace dh2::character;
bool aligned(const void* p,std::size_t a){return p&&reinterpret_cast<std::uintptr_t>(p)%a==0;}
bool properties(const dh2::data::PropertyView* v){
 if(!aligned(v,alignof(dh2::data::PropertyView)))return false;
 const std::int32_t* sheets[]={v->defaults,v->types,v->base,v->saved,v->gear,v->resolved};
 for(auto p:sheets)if(!aligned(p,alignof(std::int32_t)))return false;
 if(v->group_count>10000||(v->group_count&&!aligned(v->groups,alignof(dh2::data::PropertyBuffGroup))))return false;
 unsigned total=0;
 for(unsigned i=0;i<v->group_count;++i){auto& g=v->groups[i];if(g.count>10000||total>100000-g.count||(g.count&&!aligned(g.sheets,alignof(const std::int32_t*))))return false;total+=g.count;for(unsigned j=0;j<g.count;++j)if(!aligned(g.sheets[j],alignof(std::int32_t)))return false;}
 return !dh2_property_validate(v);
}
std::int32_t signed_word(std::uint32_t v){std::int32_t out;std::memcpy(&out,&v,4);return out;}
std::int32_t integer(float f){
 std::uint32_t b;std::memcpy(&b,&f,4);auto e=(b>>23)&255;
 if(e==255&&(b&0x7fffff))return 0;
 if(e<127)return 0;
 if(e>=158)return b>>31?INT32_MIN:INT32_MAX;
 auto magnitude=(b&0x7fffff)|0x800000u;
 magnitude=e>=150?magnitude<<(e-150):magnitude>>(150-e);
 return b>>31?-static_cast<std::int32_t>(magnitude):static_cast<std::int32_t>(magnitude);
}
bool valid(const LevelModel32* m){
 if(!(aligned(m,alignof(LevelModel32))&&!m->reserved&&
 properties(m->properties)&&
 aligned(m->classes,alignof(dh2::data::ClassRow))&&m->class_count&&m->class_count<=10000&&
 aligned(m->design,alignof(dh2_script_design_bindings))&&!m->design->reserved&&m->design->lookup))return false;
 for(unsigned i=0;i<m->class_count;++i){auto& row=m->classes[i];if(row.count>10000||(row.count&&!aligned(row.data,alignof(dh2::data::ClassFormula))))return false;}
 return true;
}
int regen(LevelModel32* m,unsigned property,unsigned maximum,const LevelServices16* services){
 auto* view=m->properties;auto current=view->resolved[property],limit=view->resolved[maximum];auto delta=limit;
 if(signed_word(std::uint32_t(current)+std::uint32_t(delta))>limit)delta=signed_word(std::uint32_t(limit)-std::uint32_t(current));
 if(delta<=0)return 0;
 if(!aligned(services,alignof(LevelServices16))||!services->invoke)return 2;
 LevelRequest24 request{level_debug_load,property,delta,0,nullptr};
 if(services->invoke(services->context,m,&request))return 2;
 request.service=level_debug_query;request.name="isTracingChar_Stats";
 if(services->invoke(services->context,m,&request)||!valid(m)||m->properties!=view)return 2;
 // Source retains delta sampled before DebugSwitch effects, while Add rereads
 // live property rules/sheets. Recomputing regen here would change its order.
 return dh2_property_add(view,static_cast<std::int32_t>(property),delta)?2:0;
}
}
extern "C" int dh2_character_set_level(LevelModel32* m,float raw,const LevelServices16* services){
 if(!valid(m)||(services&&!aligned(services,alignof(LevelServices16))))return 1;
 auto* view=m->properties;auto* design=m->design;std::int32_t maximum;
 if(design->lookup(design->context,0,"CharacterDesign","MaxLevelDVeryHard",&maximum))return 2;
 auto cap=signed_word(std::uint32_t(maximum)<<8);auto requested=integer(raw);
 if(cap<requested){
  if(design->lookup(design->context,0,"CharacterDesign","MaxLevelDVeryHard",&maximum))return 2;
  requested=signed_word(std::uint32_t(maximum)<<8);
 }
 if(!valid(m)||m->properties!=view)return 2;
 auto* base=const_cast<std::int32_t*>(view->base);base[19]=requested;
 if(dh2_class_recalc_base(m->classes,m->class_count,base,view))return 2;
 auto status=regen(m,36,38,services);return status?status:regen(m,41,43,services);
}
extern "C" int dh2_character_get_level(std::int32_t* out,const dh2::data::PropertyView* v){
 if(!aligned(out,alignof(std::int32_t))||!properties(v))return 1;
 auto p=reinterpret_cast<std::uintptr_t>(out),vp=reinterpret_cast<std::uintptr_t>(v);
 if(p<=vp?vp-p<4:p-vp<sizeof(*v))return 1;
 const std::int32_t* sheets[]={v->defaults,v->types,v->base,v->saved,v->gear,v->resolved};
 for(auto s:sheets){auto sheet=reinterpret_cast<std::uintptr_t>(s);if(p<=sheet?sheet-p<4:p-sheet<224*4)return 1;}
 auto b=std::uint32_t(v->resolved[19]);*out=signed_word((b>>8)|((b&0x80000000u)?0xff000000u:0));return 0;
}
extern "C" int dh2_character_set_level_lua(void* opaque,const dh2_script_value* a,std::uint32_t n,
 dh2_script_value*,std::uint32_t,std::uint32_t* returned,char* error,std::size_t size){
 if(!aligned(returned,alignof(std::uint32_t))||(n&&!aligned(a,alignof(dh2_script_value)))||n>1048576)return 1;
 *returned=0;if(!n||a[0].type!=3)return 0;
 auto* bindings=static_cast<const LevelBindings48*>(opaque);
 if(!aligned(bindings,alignof(LevelBindings48))||bindings->reserved[0]||bindings->reserved[1]||bindings->reserved[2]||dh2_character_set_level(bindings->model,a[0].number,&bindings->services)){
  if(error&&size)std::snprintf(error,size,"Character.SetLevel property/class provider failure");
  return 1;
 }return 0;
}
extern "C" int dh2_character_level_bind(dh2_script_vm* vm,const LevelBindings48* b){
 if(!vm||!aligned(b,alignof(LevelBindings48))||b->reserved[0]||b->reserved[1]||b->reserved[2]||!valid(b->model))return -1;
 return dh2_script_vm_bind_source_values(vm,"SetLevel",dh2_character_set_level_lua,const_cast<LevelBindings48*>(b));
}
