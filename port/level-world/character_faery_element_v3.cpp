#include "character_faery_element_v3.hpp"
#include <cstdio>
extern "C" int dh2_character_faery_element_v3(std::int32_t* out,std::uintptr_t character,
 const dh2::character::skills::CurrentSpellServices16V1* services){
 using namespace dh2::character::skills;
 if(!out||reinterpret_cast<std::uintptr_t>(out)%alignof(std::int32_t)||!character||!services||reinterpret_cast<std::uintptr_t>(services)%alignof(CurrentSpellServices16V1)||!services->invoke)return -1;
 CurrentSpellRequest24V1 q{current_spell_selected_faery,0,-1,0,character};CurrentSpellResponse16V1 r{};
 if(services->invoke(services->context,&q,&r))return -2;
 q.operation=current_spell_faery_element_v3;q.id=static_cast<std::uint32_t>(r.value);r={};
 if(services->invoke(services->context,&q,&r))return -2;
 *out=r.value;return 1;
}
namespace dh2::character::skills {
int equipped_faery_level_v3(void* context,const dh2_script_value*,std::uint32_t,
 dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* error,std::size_t size){
 if(!returned)return -1;*returned=0;auto* b=static_cast<CurrentSpellBindingsV1*>(context);std::int32_t level{};
 auto code=b?dh2_character_faery_level_v3(&level,b->character,&b->services):-1;
 if(code!=1||!out||!capacity){if(error&&size)std::snprintf(error,size,"EquippedFaeryLevel requires actual save delivery (status %d)",code);return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
 *out={};out->type=DH2_SCRIPT_NUMBER;out->number=static_cast<float>(level);*returned=1;return 0;
}
int equipped_faery_element_v3(void* context,const dh2_script_value*,std::uint32_t,
 dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* error,std::size_t size){
 if(!returned)return -1;*returned=0;auto* b=static_cast<CurrentSpellBindingsV1*>(context);std::int32_t element{};
 auto code=b?dh2_character_faery_element_v3(&element,b->character,&b->services):-1;
 if(code!=1||!out||!capacity){if(error&&size)std::snprintf(error,size,"EquippedFaeryElement requires actual save/table delivery (status %d)",code);return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
 *out={};out->type=DH2_SCRIPT_NUMBER;out->number=static_cast<float>(element);*returned=1;return 0;
}
}
extern "C" int dh2_character_faery_level_v3(std::int32_t* out,std::uintptr_t character,
 const dh2::character::skills::CurrentSpellServices16V1* services){
 using namespace dh2::character::skills;
 if(!out||reinterpret_cast<std::uintptr_t>(out)%alignof(std::int32_t)||!character||!services||reinterpret_cast<std::uintptr_t>(services)%alignof(CurrentSpellServices16V1)||!services->invoke)return -1;
 CurrentSpellRequest24V1 q{current_spell_selected_faery,0,-1,0,character};CurrentSpellResponse16V1 r{};
 if(services->invoke(services->context,&q,&r))return -2;
 q.operation=current_spell_saved_level;q.id=static_cast<std::uint32_t>(r.value);r={};
 if(services->invoke(services->context,&q,&r))return -2;
 *out=r.value;return 1;
}
