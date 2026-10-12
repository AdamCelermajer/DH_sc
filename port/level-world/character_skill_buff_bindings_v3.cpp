#include "character_skill_buff_bindings_v3.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
namespace dh2::character::skills {namespace {
bool aligned(const void* p,std::size_t a){return p&&reinterpret_cast<std::uintptr_t>(p)%a==0;}
int fail(char* e,std::size_t n,const char* text){if(e&&n)std::snprintf(e,n,"%s",text);return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
std::uint32_t u32(float f){std::uint32_t b;std::memcpy(&b,&f,4);auto e=(b>>23)&255,frac=b&0x7fffff;if((b>>31)||e<127||(e==255&&frac))return 0;if(e>158)return UINT32_MAX;return ((frac<<8)|0x80000000u)>>(158-e);}
std::int32_t i32(float f){if(std::isnan(f))return 0;if(f>=2147483648.f)return INT32_MAX;if(f< -2147483648.f)return INT32_MIN;return static_cast<std::int32_t>(f);}
bool number(SkillBuffBindingsV3& b,const dh2_script_value& v,float& f){if(v.type==0)f=0;else if(v.type==1)f=float(v.boolean!=0);else if(v.type==3)f=v.number;else return b.number&&b.number(b.conversion_context,&v,&f)==0;return true;}
}
int skill_create_buff_v3(void* opaque,const dh2_script_value* a,std::uint32_t count,dh2_script_value* out,std::uint32_t capacity,std::uint32_t* result,char* error,std::size_t size){
 if(!aligned(result,alignof(std::uint32_t))||(count&&!aligned(a,alignof(dh2_script_value)))||count>1048576)return -1;*result=0;
 if(!count||a[0].type!=3)return 0;auto* b=static_cast<SkillBuffBindingsV3*>(opaque);
 if(!aligned(b,alignof(SkillBuffBindingsV3))||!b->owner||b->class_count>10000)return fail(error,size,"Skill Buff owner unavailable");
 if(u32(a[0].number)>=b->class_count)return 0;
 const auto id=i32(a[0].number);std::uint32_t duration=0,strength=0;std::int32_t cap=1,fx=-1;const char* name="";float converted;
 if(count>1&&a[1].type!=0){if(a[1].type==3)duration=u32(a[1].number);}
 if(count>2&&a[2].type!=0){if(a[2].type==1)cap=a[2].boolean?0:1;else {if(!number(*b,a[2],converted))return fail(error,size,"Skill Buff capacity conversion unavailable");cap=i32(converted);}}
 if(count>3&&a[3].type==3)strength=u32(a[3].number);
 if(count>4&&a[4].type!=0){
  if(a[4].type==3)fx=i32(a[4].number);
  else {if(!number(*b,a[4],converted))return fail(error,size,"Skill Buff FX conversion unavailable");if(b->fx_count==UINT32_MAX)return fail(error,size,"Skill Buff actual FX catalog unavailable");if(u32(converted)<b->fx_count)fx=i32(converted);}
 }
 if(count>5&&a[5].type==4){if(!a[5].text)return fail(error,size,"Skill Buff name storage unavailable");name=a[5].text;}
 if(!aligned(out,alignof(dh2_script_value))||!capacity)return fail(error,size,"Skill Buff return storage unavailable");
 BuffResult24 r;const auto code=dh2_character_buff_add(&r,b->owner,id,duration,cap,strength,fx,name);
 if(code!=1)return fail(error,size,"Skill Buff creation required provider failed");
 if(r.instance){*out={};out->type=DH2_SCRIPT_IDENTITY;out->identity=r.instance;*result=1;}return 0;
}
int skill_remove_buff_v3(void* opaque,const dh2_script_value* a,std::uint32_t count,dh2_script_value*,std::uint32_t,std::uint32_t* result,char* error,std::size_t size){
 if(!aligned(result,alignof(std::uint32_t))||(count&&!aligned(a,alignof(dh2_script_value)))||count>1048576)return -1;*result=0;if(!count||a[0].type!=3)return 0;
 auto* b=static_cast<SkillBuffBindingsV3*>(opaque);if(!aligned(b,alignof(SkillBuffBindingsV3))||!b->owner)return fail(error,size,"Skill Buff owner unavailable");auto id=u32(a[0].number);if(id>=b->class_count)return 0;
 if(count>1&&a[1].type!=2)return 0;BuffResult24 r;if(dh2_character_buff_delete(&r,b->owner,static_cast<std::int32_t>(id),count>1?a[1].identity:0)!=1)return fail(error,size,"Skill Buff removal required provider failed");return 0;
}
int skill_buff_sheet_v3(BuffOwner* owner,std::uintptr_t identity,std::int32_t** out){
 if(!owner||!identity||!aligned(out,alignof(std::int32_t*)))return -1;
 for(unsigned i=0;i<dh2_character_buffs_count(owner);++i){BuffSnapshot48 s;if(dh2_character_buff_snapshot(&s,owner,i)!=1)return -1;if(s.instance==identity){*out=const_cast<std::int32_t*>(s.sheet);return 0;}}
 return -1;
}
}
