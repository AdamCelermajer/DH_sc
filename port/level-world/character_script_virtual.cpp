#include "character_script_virtual.hpp"
extern "C" int dh2_character_script_initial_virtual(
 const dh2::character::ScriptTimerCall16* session,std::uint32_t kind,std::uint32_t slot){
 using namespace dh2::character;
 if(!session||kind>script_player||slot<8||slot>20||(slot&3))return -1;
 if(kind!=script_external)return 0;
 if(!session->vm||!session->aliases)return -1;
 const char* names[]={"OnInit","OnInitPost","OnInitFinal","OnTerminate"};
 return dh2_script_alias_call_discard_source(session->vm,session->aliases,names[(slot-8)/4],nullptr,0);
}
extern "C" int dh2_character_script_init_vcb(
 std::uint32_t* flags,const dh2_script_aliases* aliases,std::uint32_t external){
 if(!flags||!aliases||external>1)return -1;
 *flags=0;
 std::uint32_t result=dh2_script_alias_contains(aliases,"OnTargetHit")?0x800u:0u;
 *flags=result;
 result|=dh2_script_alias_contains(aliases,"OnTargetMissed")?0x1000u:0u;
 *flags=result;
 if(external){
  const char* names[]={"OnUpdate","OnFriendSpotted","OnTargetOutOfRange","OnTargetInRangedRange",
   "OnTargetInCloseRange","OnTargetInMeleeRange","OnMasterOutOfRange","OnMasterInRangedRange",
   "OnMasterInCloseRange","OnMasterInMeleeRange"};
  for(std::uint32_t i=0;i<10;++i){result|=dh2_script_alias_contains(aliases,names[i])?(1u<<i):0u;*flags=result;}
 }
 return 0;
}
