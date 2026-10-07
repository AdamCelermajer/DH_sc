#include "character_script_init_vitals.hpp"
#include "character_script_session.hpp"
extern "C" int dh2_character_script_session_init_service(void* context,
 dh2::character::CharacterScriptSession& session,const dh2::character::ScriptLifecycleRequest32& request){
 using namespace dh2::character;
 if(!context||reinterpret_cast<std::uintptr_t>(context)%alignof(ScriptSessionInit32))return -1;
 const auto& b=*static_cast<ScriptSessionInit32*>(context);
 if(!b.owner||b.reserved||request.reserved||b.owner!=session.timers().owner)return -1;
 if(request.service==script_refresh_vitals){
  if(request.subject!=b.owner||request.argument0||request.argument1||request.payload||!b.debug)return -1;
  const ScriptInitVitals32 source{b.owner,&session.property_view(),*b.debug};ScriptInitVitals24 out{};
  return dh2_character_script_init_vitals(&out,&source)==1?0:-2;
 }
 if(request.service!=script_configure_skills&&request.service!=script_update_skills)return -1;
 if(request.subject||request.payload||request.argument0||request.argument1||!b.skills||!b.skills->invoke)return -1;
 return b.skills->invoke(b.skills->context,session,request);
}
