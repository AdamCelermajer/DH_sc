#include "character_current_skill_v2.hpp"
#include <cmath>
#include <cstdio>
namespace {
bool aligned(const void* p,std::size_t n){return p&&reinterpret_cast<std::uintptr_t>(p)%n==0;}
std::int32_t integer(float f){if(std::isnan(f))return 0;if(f>=2147483648.0f)return INT32_MAX;if(f< -2147483648.0f)return INT32_MIN;return static_cast<std::int32_t>(f);}
int failure(char* e,std::size_t n,const char* s){if(e&&n)std::snprintf(e,n,"%s",s);return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
}
extern "C" int dh2_character_current_skill_level_v2(std::int32_t* out,const dh2::character::skills::CurrentSkillView32V2* v,float number,std::uint32_t present){
 if(!aligned(out,alignof(std::int32_t))||!aligned(v,alignof(dh2::character::skills::CurrentSkillView32V2))||v->reserved||present>1||v->list_count>65536||v->skill_count>65536)return -1;
 if(!present)return 0;
 if(!aligned(v->lists,alignof(dh2::character::skills::List16))||v->list_count<=3)return -1;
 const auto selected=v->selected_list>=0&&static_cast<std::uint32_t>(v->selected_list)<v->list_count?static_cast<std::uint32_t>(v->selected_list):3u;
 const auto& list=v->lists[selected];if(list.reserved||list.count>65536||(list.count&&!aligned(list.ids,alignof(std::int32_t))))return -1;
 const auto index=integer(number);if(index<0||static_cast<std::uint32_t>(index)>=list.count)return -2;
 const auto id=list.ids[index];if(id<0||static_cast<std::uint32_t>(id)>=v->skill_count)return -2;
 if(!v->saved){*out=-1;return 1;}
 const auto* saved=v->saved;
 if(!aligned(saved,alignof(dh2::data::SavedSkillsView16V1))||saved->reserved||saved->count>65536||(saved->count&&!aligned(saved->rows,alignof(dh2::data::SavedSkill8V1))))return -1;
 if(static_cast<std::uint32_t>(index)>=saved->count)return -2;
 // Same source PlayerSavegame::SG_GetSkillLevel unsigned16 read. Zero pointer
 // with count0 is outside the safe GetCharSkill/index domain, never invented0.
 *out=saved->rows[index].level;return 1;
}
namespace dh2::character::skills {
int current_skill_info_v2(void* opaque,const dh2_script_value* args,std::uint32_t count,dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* error,std::size_t size){
 if(!returned||(count&&!args))return -1;*returned=0;if(!count)return 0;
 auto* b=static_cast<CurrentSkillBindingsV2*>(opaque);
 if(!b||!b->properties||dh2_property_validate(b->properties)||!b->tables||!*b->tables)return failure(error,size,"CurrentSkill genuine tables/properties unavailable");
 float number{};
 if(args[0].type==DH2_SCRIPT_NUMBER)number=args[0].number;
 else if(args[0].type==DH2_SCRIPT_BOOLEAN)number=args[0].boolean?1.f:0.f;
 else if(args[0].type!=DH2_SCRIPT_NIL){if(!b->number||b->number(b->context,args,&number))return failure(error,size,"CurrentSkill source numeric conversion unavailable");}
 try{
  std::vector<List16> lists;for(const auto& row:b->tables->lists())lists.push_back({row.data(),static_cast<std::uint32_t>(row.size()),0});
  CurrentSkillView32V2 view{lists.data(),static_cast<std::uint32_t>(lists.size()),static_cast<std::uint32_t>(b->tables->skills().size()),b->properties->resolved[28],0,nullptr};
  // Source nonnumeric first-argument precheck is unsigned f2u before the later
  // signed conversion/GetCharSkill. Only numeric tag3 skips that precheck.
  if(args[0].type!=DH2_SCRIPT_NUMBER){const auto selected=view.selected_list>=0&&static_cast<std::uint32_t>(view.selected_list)<view.list_count?static_cast<std::uint32_t>(view.selected_list):3u;if(selected>=view.list_count)return failure(error,size,"CurrentSkill fallback table unavailable");const auto u=std::isnan(number)||number<=0?0u:number>=4294967296.f?UINT32_MAX:static_cast<std::uint32_t>(number);if(u>=lists[selected].count)return 0;
   if(args[0].type!=DH2_SCRIPT_BOOLEAN&&args[0].type!=DH2_SCRIPT_NIL){if(b->number(b->context,args,&number))return failure(error,size,"CurrentSkill repeated numeric conversion unavailable");}}
  view.selected_list=b->properties->resolved[28]; // GetCharSkill re-reads live list.
  data::SavedSkillsView16V1 saved{};
  if(b->saved){const auto& rows=b->saved->skills();saved={const_cast<data::SavedSkill8V1*>(rows.data()),static_cast<std::uint32_t>(rows.size()),0};view.saved=&saved;}
  std::int32_t value{};const auto result=dh2_character_current_skill_level_v2(&value,&view,number,1);
  if(result!=1)return failure(error,size,"CurrentSkill original assertion/index boundary unsupported");
  if(!out||!capacity)return failure(error,size,"CurrentSkill return storage unavailable");*out={};out->type=DH2_SCRIPT_NUMBER;out->number=static_cast<float>(value);*returned=1;return 0;
 }catch(...){return failure(error,size,"CurrentSkill table projection failed");}
}
}
