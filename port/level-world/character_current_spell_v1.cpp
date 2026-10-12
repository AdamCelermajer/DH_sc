#include "character_current_spell_v1.hpp"
#include <cstdio>
namespace {
bool aligned(const void* p,std::size_t n){return p&&reinterpret_cast<std::uintptr_t>(p)%n==0;}
}
extern "C" int dh2_character_current_spell_level_v1(std::int32_t* out,std::uintptr_t character,
 const dh2::character::skills::CurrentSpellServices16V1* services){
 using namespace dh2::character::skills;
 if(!aligned(out,alignof(std::int32_t))||!character||!aligned(services,alignof(CurrentSpellServices16V1))||!services->invoke)return -1;
 CurrentSpellResponse16V1 response{};
 auto call=[&](std::uint32_t operation,std::uint32_t id){
  CurrentSpellRequest24V1 request{operation,id,-1,0,character};response={};
  return !services->invoke(services->context,&request,&response);
 };
 if(!call(current_spell_selected_faery,0))return -2;
 const auto first=static_cast<std::uint32_t>(response.value);
 if(!call(current_spell_validate_faery,first))return -2;
 if(!call(current_spell_selected_faery,0))return -2;
 const auto second=static_cast<std::uint32_t>(response.value);
 if(!call(current_spell_saved_level,second))return -2;
 *out=response.value;return 1;
}
namespace dh2::character::skills {
int current_spell_info_v1(void* context,const dh2_script_value*,std::uint32_t,
 dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* error,std::size_t size){
 if(!returned)return -1;
 *returned=0;
 auto* bindings=static_cast<CurrentSpellBindingsV1*>(context);std::int32_t level{};
 const auto code=bindings?dh2_character_current_spell_level_v1(&level,bindings->character,&bindings->services):-1;
 if(code!=1||!out||!capacity){
  if(error&&size)std::snprintf(error,size,"CurrentSpell genuine selected-faery/table/saved-level delivery required (status %d)",code);
  return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
 }
 *out={};out->type=DH2_SCRIPT_NUMBER;out->number=static_cast<float>(level);*returned=1;return 0;
}
}
