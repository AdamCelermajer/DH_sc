#pragma once
#include "audio_application_manager_v42.hpp"
namespace dh2::audio {
struct AudioContainerPrecacheServicesV42 {
 void* context{};
 bool(*borrow_actual_global)(void*,AudioApplicationBorrowV42&,std::string&){};
 bool(*actual_get_sound)(void*,int&,std::string&){};
};
inline bool audio_container_precache_v42(const AudioContainerPrecacheServicesV42&s,
 std::string&error){
 AudioApplicationBorrowV42 captured;
 if(!s.borrow_actual_global){error="Required actual nullable Application SoundManager";return false;}
 if(!s.borrow_actual_global(s.context,captured,error))return false;
 if(!captured.manager){error.clear();return true;}
 if(!s.actual_get_sound){error="Required actual Container GetSound";return false;}
 int raw_uid{};if(!s.actual_get_sound(s.context,raw_uid,error))return false;
 return captured.precache_raw_uid(raw_uid,error);
}
}
