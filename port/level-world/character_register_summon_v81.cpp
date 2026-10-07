#include "character_register_summon_v81.hpp"
#include <cstdio>
#include <cstring>
#include <exception>
#include <limits>
namespace {
int fail(char* out,std::size_t n,const char* message){if(out&&n)std::snprintf(out,n,"%s",message);return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
//Actual ARM __fixunssfsi8be2a0, including sign/NaN/overflow branches.
std::uint32_t unsigned_number(float f)noexcept{
 std::uint32_t bits;std::memcpy(&bits,&f,4);const auto exponent=(bits>>23)&255u;
 if(bits>>31||exponent<127||(exponent==255&&(bits&0x7fffffu)))return 0;
 if(exponent>158)return UINT32_MAX;
 return ((bits<<8)|0x80000000u)>>(158-exponent);
}
//Imported source __aeabi_f2iz contract used by existing native scalar owner.
std::int32_t signed_number(float f)noexcept{
 std::uint32_t bits;std::memcpy(&bits,&f,4);const auto exponent=(bits>>23)&255u;
 if((exponent==255&&(bits&0x7fffffu))||exponent<127)return 0;
 if(exponent>=158)return bits>>31?INT32_MIN:INT32_MAX;
 const auto mantissa=(bits&0x7fffffu)|0x800000u;
 const auto value=exponent>=150?mantissa<<(exponent-150):mantissa>>(150-exponent);
 return bits>>31?-static_cast<std::int32_t>(value):static_cast<std::int32_t>(value);
}
}
extern "C" int dh2_character_register_summon_v81(void* p,const dh2_script_value* a,std::uint32_t n,
 dh2_script_value*,std::uint32_t,std::uint32_t* r,char* e,std::size_t z){
 if(!p||!r||(n&&!a))return fail(e,z,"Malformed source RegisterSummon arguments");
 *r=0;auto& self=*static_cast<dh2::character::RegisterSummonBindingsV81*>(p);
 if(!n||a[0].type!=DH2_SCRIPT_NUMBER)return 0;
 if(!self.characters)return fail(e,z,"Required actual CharacterProperties size");
 if(unsigned_number(a[0].number)>=self.characters->rows.size())return 0;
 const auto id=signed_number(a[0].number);
 const auto quantity=n>1&&a[1].type==DH2_SCRIPT_NUMBER?unsigned_number(a[1].number):1u;
 if(!self.cache||!self.add)return fail(e,z,"Required actual process RegisterSummon cachedCharOID owner");
 try{std::string error;if(!self.add(id,quantity,error))return fail(e,z,error.c_str());return 0;}
 catch(const std::exception& failure){return fail(e,z,failure.what());}
 catch(...){return fail(e,z,"Source RegisterSummon provider threw");}
}
