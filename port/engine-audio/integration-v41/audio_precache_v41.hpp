#pragma once
#include <cstdint>
#include <memory>
#include <string>
namespace dh2::audio {
struct AudioManagerLeaseV41 {std::uintptr_t identity{};std::shared_ptr<void> lifetime;};
struct AudioPrecacheServicesV41 {
 void* context{};
 bool(*capture_nullable_manager)(void*,AudioManagerLeaseV41&,std::string&){};
 bool(*actual_get_sound)(void*,int&,std::string&){};
 bool(*actual_disabled)(void*,bool&,std::string&){};
 bool(*actual_pack_bound)(void*,std::uintptr_t,int&,std::string&){};
 bool(*actual_bank_info)(void*,std::uintptr_t,int,std::string&){};
 bool(*actual_cached_slot)(void*,std::uintptr_t,int,bool&,std::string&){};
 bool(*load_exact_pack_uid)(void*,std::uintptr_t,int,std::string&){};
};
enum class AudioPrecacheResultV41 {required,null_manager,disabled,out_of_range,cached,loaded};
// Synchronous one-producer leaf: manager capture BEFORE GetSound, original
// LoadSound disabled/range/metadata/recheck/cache prefix, then exact raw UID.
// No source ordinal lookup, Play3D/GS gate, RNG, focus, clock or command post.
inline bool audio_container_precache_v41(const AudioPrecacheServicesV41&s,
 AudioPrecacheResultV41&result,std::string&error){
 result=AudioPrecacheResultV41::required;
 auto required=[&](const char*name){error=std::string("Required actual precache authority: ")+name;return false;};
 AudioManagerLeaseV41 manager;
 if(!s.capture_nullable_manager)return required("nullable global manager snapshot");
 if(!s.capture_nullable_manager(s.context,manager,error))return false;
 if(!manager.identity){result=AudioPrecacheResultV41::null_manager;error.clear();return true;}
 if(!manager.lifetime)return required("captured manager/provider lifetime");
 int uid{};if(!s.actual_get_sound)return required("GetSound");
 if(!s.actual_get_sound(s.context,uid,error))return false;
 bool disabled{};if(!s.actual_disabled)return required("disabled global");
 if(!s.actual_disabled(s.context,disabled,error))return false;
 if(disabled){result=AudioPrecacheResultV41::disabled;error.clear();return true;}
 if(uid<0){result=AudioPrecacheResultV41::out_of_range;error.clear();return true;}
 int bound{};if(!s.actual_pack_bound)return required("selected soundpack bound");
 if(!s.actual_pack_bound(s.context,manager.identity,bound,error))return false;
 if(uid>bound){result=AudioPrecacheResultV41::out_of_range;error.clear();return true;}
 if(!s.actual_bank_info)return required("selected soundpack bank metadata");
 if(!s.actual_bank_info(s.context,manager.identity,uid,error))return false;
 if(!s.actual_pack_bound(s.context,manager.identity,bound,error))return false;
 if(uid>bound){result=AudioPrecacheResultV41::out_of_range;error.clear();return true;}
 bool cached{};if(!s.actual_cached_slot)return required("same manager cache slot");
 if(!s.actual_cached_slot(s.context,manager.identity,uid,cached,error))return false;
 if(cached){result=AudioPrecacheResultV41::cached;error.clear();return true;}
 if(!s.load_exact_pack_uid)return required("exact selected pack loader");
 if(!s.load_exact_pack_uid(s.context,manager.identity,uid,error))return false;
 result=AudioPrecacheResultV41::loaded;error.clear();return true;
}
}
