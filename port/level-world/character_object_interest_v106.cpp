#include "character_object_interest_v106.hpp"
#include <cstring>
namespace dh2::character {
bool source_character_update_interest_v106(CharacterInterestFieldsV106& f,
 std::uint32_t dt,const CharacterInterestServicesV106& s,std::string& e){
 if(!f.identity||!f.object14a4||!f.type14a8||!f.delay14aa||!f.blocked14ac||!f.changed14ad||!s.owner){e="Required SAME Character OOI source fields/services";return false;}
 *f.changed14ad=0;
 if(*f.object14a4){
  if(!s.interactive){e="Required selected OOI virtual8c";return false;}
  bool interactive{};const auto captured=*f.object14a4;
  if(!s.interactive(captured,f.identity,interactive,e))return false;
  if(!interactive)*f.object14a4=0;
  else{
   // Source reloads14a4 after virtual8c, rather than reusing captured target.
   if(!*f.object14a4){e="Source OOI virtual8c removed receiver before disabled81 read";return false;}
   const std::uint8_t* disabled{};std::shared_ptr<void> pin;
   if(!s.disabled||!s.disabled(*f.object14a4,disabled,pin,e)||!disabled||!pin){if(e.empty())e="Required actual OOI disabled81";return false;}
   if(*disabled)*f.object14a4=0;
  }
 }
 const auto bits=static_cast<std::uint16_t>(static_cast<std::uint16_t>(*f.delay14aa)-dt);
 std::int16_t delay;std::memcpy(&delay,&bits,sizeof delay);*f.delay14aa=delay;
 if(delay>0)return true;
 *f.type14a8=-1;*f.delay14aa=500;*f.blocked14ac=0;
 const auto old=*f.object14a4;*f.object14a4=0;
 if(!s.search){e="Required original OOI TargetList Search/characterFlags89/objectType1";return false;}
 std::vector<CharacterInterestCandidateV106> candidates;
 if(!s.search(f.identity,candidates,e))return false;
 for(const auto& candidate:candidates){
  if(candidate.identity==f.identity)continue;
  if(!candidate.identity){e="Required actual OOI TargetInfo receiver";return false;}
  if(!s.interaction_type){e="Required selected OOI virtual90";return false;}
  if(*f.object14a4&&!(candidate.flags&1u)){
   std::int32_t kind{};
   if(!s.interaction_type(candidate.identity,f.identity,kind,e))return false;
   if(kind!=1)continue;
  }
  *f.object14a4=candidate.identity;
  std::int32_t raw{};if(!s.interaction_type(candidate.identity,f.identity,raw,e))return false;
  // Source uxtb store followed by sxtb use, independent of host signed-char.
  const auto byte=static_cast<std::uint8_t>(raw);
  *f.type14a8=byte<128?byte:static_cast<std::int32_t>(byte)-256;
  *f.blocked14ac=static_cast<std::uint8_t>(candidate.flags&1u);
  if(*f.blocked14ac||*f.type14a8==0||*f.type14a8==2)break;
  if(*f.type14a8==1){
   std::uintptr_t owner{};if(!s.item_owner3bc||!s.item_owner3bc(*f.object14a4,owner,e)){if(e.empty())e="Required SAME Item owner3bc";return false;}
   if(owner==f.identity)break;
  }
  // Rejected candidate remains14a4 while the next queue head is evaluated.
 }
 if(*f.object14a4&&*f.object14a4!=old)*f.changed14ad=1;
 return true;
}
}
