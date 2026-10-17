// Adapted AdamCelermajer/DH_sc c3ae797332a82a30a586b9156cddc25445e36a4c
// character_skill_cooldown_v3.cpp; replace its owner with borrowed services.
#include "character_skill_cooldown_services.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
namespace dh2::character_skill_cooldown_services { namespace {
struct Range {std::uintptr_t first,end;};
bool range(const void* p,std::size_t n,std::size_t alignment,Range& out){const auto at=reinterpret_cast<std::uintptr_t>(p);if(!p||at%alignment||at>UINTPTR_MAX-n)return false;out={at,at+n};return true;}
bool overlap(Range a,Range b){return a.first<b.end&&b.first<a.end;}
int failure(char* e,std::size_t n,const char* text){if(e&&n)std::snprintf(e,n,"%s",text);return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
std::uint32_t unsigned_number(float f){std::uint32_t b;std::memcpy(&b,&f,4);const auto e=(b>>23)&255,frac=b&0x7fffff;if((b>>31)||e<127||(e==255&&frac))return 0;if(e>158)return UINT32_MAX;return ((frac<<8)|0x80000000u)>>(158-e);}
std::int32_t signed_number(float f){if(std::isnan(f))return 0;if(f>=2147483648.f)return INT32_MAX;if(f< -2147483648.f)return INT32_MIN;return std::int32_t(f);}
std::int32_t signed_bits(std::uint32_t w){std::int32_t s;std::memcpy(&s,&w,4);return s;}
bool number(const Services& s,const dh2_script_value* v,float& f){switch(v->type){case 1:case 3:f=v->number;return true;case 2:case 4:case 7:return s.number&&s.number(s.context,v,&f)==0;default:f=0;return true;}}
struct Controls {
    Range args{},result{},error{},services{};bool have_args=false,have_error=false;
    bool field(const std::int32_t* p)const {Range r;if(!range(p,sizeof(*p),alignof(std::int32_t),r))return false;return !overlap(r,result)&&!overlap(r,services)&&(!have_args||!overlap(r,args))&&(!have_error||!overlap(r,error));}
};
bool initial(const dh2_script_value* a,std::uint32_t count,std::uint32_t* result,char* error,std::size_t size,Controls& c){
    if(count>65536||size>1048576||!range(result,sizeof(*result),alignof(std::uint32_t),c.result))return false;
    if(count){if(!range(a,std::size_t(count)*sizeof(*a),alignof(dh2_script_value),c.args)||overlap(c.args,c.result))return false;c.have_args=true;}
    if(size){if(!range(error,size,1,c.error)||overlap(c.error,c.result)||(count&&overlap(c.error,c.args)))return false;c.have_error=true;}
    return true;
}
bool bind(void* p,Controls& c,Services& s){if(!range(p,sizeof(Services),alignof(Services),c.services)||overlap(c.services,c.result)||(c.have_args&&overlap(c.services,c.args))||(c.have_error&&overlap(c.services,c.error)))return false;s=*static_cast<Services*>(p);return s.character!=0;}
bool safe_control(void* p,const Controls& c){if(!p)return true;Range r;if(!range(p,sizeof(Services),alignof(Services),r))return false;return !overlap(r,c.result)&&(!c.have_args||!overlap(r,c.args))&&(!c.have_error||!overlap(r,c.error));}
// 1 valid field, 0 source-null slot, -1 missing/failed provider, -2 malformed
// returned field. Keep provider failures distinct from a successful null slot.
// A malformed field may alias the diagnostic buffer, so that case must not
// copy an error string through it.
int get_slot(const Services& s,Controls& c,std::uint32_t kind,std::uint32_t index,Slot& slot){
    slot={};
    if(!s.slot)return -1;
    Slot candidate{};
    if(s.slot(s.context,s.character,kind,index,&candidate))return -1;
    if(!candidate.instance){
        if(candidate.timer_id_18)return -2;
        slot=candidate;
        return 0;
    }
    if(!c.field(candidate.timer_id_18))return -2;
    slot=candidate;
    return 1;
}
}
int skill(void* p,const dh2_script_value* a,std::uint32_t count,dh2_script_value*,std::uint32_t,std::uint32_t* result,char* error,std::size_t size){
    Controls controls;if(!initial(a,count,result,error,size,controls))return -1;
    // Numeric index skips the first list query. A rejected timer type reaches
    // no owner/provider at all, even if no native binding is installed.
    const bool skip=count<2||(a[0].type==3&&a[1].type!=0&&a[1].type!=3);
    if(!skip&&!safe_control(p,controls))return -1;
    *result=0;if(skip)return 0;Services s{};if(!bind(p,controls,s))return failure(error,size,"Skill cooldown bindings unavailable");
    try {
        float value;
        if(a[0].type!=3){
            if(!number(s,a,value))return failure(error,size,"Skill index number provider unavailable");
            std::uint32_t n;if(!s.list_count||s.list_count(s.context,s.character,0,&n))return failure(error,size,"Skill list unavailable");
            if(unsigned_number(value)>=n)return 0;
        }
        if(a[1].type!=0&&a[1].type!=3)return 0;
        if(!number(s,a,value))return failure(error,size,"Skill index second conversion failed");
        const auto index=signed_number(value);if(index<0)return failure(error,size,"Skill index assertion domain unsupported");
        Slot slot{};const auto found=get_slot(s,controls,0,std::uint32_t(index),slot);
        if(found==-2)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
        if(found<0)return failure(error,size,"Skill slot unavailable");
        if(!found)return 0;
        std::int32_t timer=-1;
        if(a[1].type!=0){if(!number(s,a+1,value))return failure(error,size,"Skill timer number provider unavailable");timer=signed_bits(unsigned_number(value));}
        *slot.timer_id_18=timer;return 0;
    } catch(...){return failure(error,size,"Skill cooldown provider failed");}
}
int spell(void* p,const dh2_script_value* a,std::uint32_t count,dh2_script_value*,std::uint32_t,std::uint32_t* result,char* error,std::size_t size){
    Controls controls;if(!initial(a,count,result,error,size,controls)||(count&&(a[0].type==0||a[0].type==3)&&!safe_control(p,controls)))return -1;
    *result=0;if(!count||(a[0].type!=0&&a[0].type!=3))return 0;Services s{};if(!bind(p,controls,s))return failure(error,size,"Spell cooldown bindings unavailable");
    try {
        float value=0;std::int32_t timer=-1;
        if(a[0].type!=0){if(!number(s,a,value))return failure(error,size,"Spell timer number provider unavailable");timer=signed_bits(unsigned_number(value));}
        std::uint32_t n;if(!s.list_count||s.list_count(s.context,s.character,1,&n))return failure(error,size,"Spell list unavailable");
        if(n>65536)return failure(error,size,"Spell list exceeds port bound");
        for(std::uint32_t i=0;i<n;++i){Slot slot{};const auto found=get_slot(s,controls,1,i,slot);if(found==-2)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;if(found<0)return failure(error,size,"Spell slot unavailable");if(found>0)*slot.timer_id_18=timer;}
        return 0;
    } catch(...){return failure(error,size,"Spell cooldown provider failed");}
}
}
