#include "character_skill_properties_v1.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
namespace dh2::character::skills {namespace {
int failure(char* e,std::size_t n,const char* s){if(e&&n)std::snprintf(e,n,"%s",s);return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
std::int32_t integer(float f){if(std::isnan(f))return 0;if(f>=2147483648.0f)return INT32_MAX;if(f< -2147483648.0f)return INT32_MIN;return static_cast<std::int32_t>(f);}
bool aligned(const void* p,std::size_t a){return p&&reinterpret_cast<std::uintptr_t>(p)%a==0;}
bool valid(const SkillPropertyBindingsV1* b){return aligned(b,alignof(SkillPropertyBindingsV1))&&aligned(b->owner,alignof(data::PropertyView))&&aligned(b->temporary,alignof(data::PropertySheet))&&aligned(b->classes,alignof(data::ClassTables))&&!dh2_property_validate(b->owner)&&b->classes->rows.size()<=10000;}
int external(SkillPropertyBindingsV1& b,std::uintptr_t identity,std::int32_t*& p){p=nullptr;return !b.external||b.external(b.context,identity,&p)||!aligned(p,alignof(std::int32_t))?-1:0;}
int recalculate(SkillPropertyBindingsV1& b){return !b.recalculate||b.recalculate(b.context,b.owner)?-1:0;}
unsigned apply(SkillPropertyBindingsV1& b,std::int32_t id,std::int32_t* sheet){
 if(id<0||static_cast<std::size_t>(id)>=b.classes->rows.size())return 0;
 try {
 std::vector<data::ClassRow> rows;rows.reserve(b.classes->rows.size());
 for(const auto& r:b.classes->rows){if(r.size()>10000)return 4;rows.push_back({r.data(),static_cast<std::uint32_t>(r.size())});}
 // Buff-mode getters re-read the live owner resolved sheet, including an
 // explicit external sheet that aliases it. Do not snapshot the buff source.
 return dh2_class_apply(rows.data(),static_cast<std::uint32_t>(rows.size()),id,sheet,b.owner->resolved);
 }catch(...){return 4;}
}
}
int skill_clear_properties_v1(void* p,const dh2_script_value* a,std::uint32_t count,dh2_script_value*,std::uint32_t,std::uint32_t* n,char* e,std::size_t cap){
 if(!n||(!a&&count))return -1;*n=0;auto* b=static_cast<SkillPropertyBindingsV1*>(p);if(!valid(b))return failure(e,cap,"Skill property owner unavailable");
 if(!count)return 0;
 if(a[0].type==DH2_SCRIPT_BOOLEAN){if(a[0].boolean)std::memcpy(b->temporary->data(),b->owner->defaults,896);return 0;}
 if(a[0].type!=DH2_SCRIPT_IDENTITY||!a[0].identity)return 0;
 std::int32_t* sheet;if(external(*b,a[0].identity,sheet))return failure(e,cap,"Skill external sheet unavailable");
 std::memcpy(sheet,b->owner->defaults,896);if(recalculate(*b))return failure(e,cap,"Skill external reset recalculation unavailable");return 0;
}
int skill_set_property_v1(void* p,const dh2_script_value* a,std::uint32_t count,dh2_script_value*,std::uint32_t,std::uint32_t* n,char* e,std::size_t cap){
 if(!n||(!a&&count))return -1;*n=0;auto* b=static_cast<SkillPropertyBindingsV1*>(p);if(!valid(b))return failure(e,cap,"Skill property owner unavailable");
 if(count<2||a[0].type!=DH2_SCRIPT_NUMBER||a[1].type!=DH2_SCRIPT_NUMBER)return 0;auto id=integer(a[0].number);if(static_cast<std::uint32_t>(id)>223)return 0;
 auto value=integer(a[1].number);
 if(count>2&&a[2].type==DH2_SCRIPT_IDENTITY){if(!a[2].identity)return 0;std::int32_t* sheet;if(external(*b,a[2].identity,sheet)||dh2_property_set_to_sheet(b->owner,id,value,sheet))return failure(e,cap,"Skill external property setter unavailable");}
 else if(dh2_property_set(b->owner,id,value))return failure(e,cap,"Skill property setter failed");
 return 0;
}
int skill_apply_class_v1(void* p,const dh2_script_value* a,std::uint32_t count,dh2_script_value*,std::uint32_t,std::uint32_t* n,char* e,std::size_t cap){
 if(!n||(!a&&count))return -1;*n=0;auto* b=static_cast<SkillPropertyBindingsV1*>(p);if(!valid(b))return failure(e,cap,"Skill property owner unavailable");
 if(!count||a[0].type!=DH2_SCRIPT_NUMBER)return 0;auto id=integer(a[0].number);if(static_cast<std::uint32_t>(id)>b->classes->rows.size())return 0;
 if(count>1&&a[1].type==DH2_SCRIPT_IDENTITY){
  if(!a[1].identity)return 0;std::int32_t* sheet;if(external(*b,a[1].identity,sheet))return failure(e,cap,"Skill external class sheet unavailable");
  if(apply(*b,id,sheet))return failure(e,cap,"Skill external class evaluation failed");
  if(recalculate(*b))return failure(e,cap,"Skill external class recalculation unavailable");return 0;
 }
 if(count>1&&a[1].type==DH2_SCRIPT_BOOLEAN&&a[1].boolean){
  if(apply(*b,id,b->temporary->data()))return failure(e,cap,"Skill temporary class evaluation failed");return 0;
 }
 if(!b->normal_class||b->normal_class(b->context,b->owner,id))return failure(e,cap,"Skill uncached class provider unavailable");return 0;
}
int skill_property_bind_v1(dh2_script_vm* vm,SkillPropertyBindingsV1* b){if(!valid(b))return -1;
 return dh2_script_vm_bind_source_values(vm,"ClearProps",skill_clear_properties_v1,b)||dh2_script_vm_bind_source_values(vm,"SetProp",skill_set_property_v1,b)||dh2_script_vm_bind_source_values(vm,"ApplyPropClass",skill_apply_class_v1,b)?-1:0;
}
}
