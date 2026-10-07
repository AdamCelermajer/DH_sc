#include "character_menu_potions_v4.hpp"
#include "item_text_varargs_v5.hpp"
namespace dh2::ui {
bool character_menu_potion_localized_format_v4(HudTextV1& text,const HudTextEnvironmentV1& env,std::string& format,bool& null,std::string& error){
 const auto& services=env.localization;if(!services.constant){error="Required actual StrID constant provider";return false;}
 std::uint32_t id=0;if(!services.constant(services.context,"StrID","GAMEPLAYMENUS_POTIONS",id,error))return false;
 return text.integer_string(static_cast<std::int32_t>(id),services,format,null,error);
}
bool character_menu_potion_integer_text_v4(HudTextV1& text,const HudTextEnvironmentV1& env,const char* format,std::int32_t count,std::string& out,std::string& error){
 struct Context {HudTextV1& text;const HudTextEnvironmentV1& env;std::string scratch;} context{text,env,{}};
 HudTextServicesV1 services{&context,[](void* raw,const HudTextRequestV1& q,HudTextResponseV1& r,std::string& error){
  auto& c=*static_cast<Context*>(raw);const auto& env=c.env;const auto& localized=env.localization;
  if(q.operation==hud_text_constant_v1){if(!localized.constant){error="Required actual numeric-format constant producer";return false;}std::uint32_t value=0;if(!localized.constant(localized.context,q.group,q.key,value,error))return false;r.value=static_cast<std::int32_t>(value);return true;}
  if(q.operation==hud_text_integer_string_v1){bool null=false;if(!c.text.integer_string(q.value,localized,c.scratch,null,error))return false;r.text=null?nullptr:c.scratch.c_str();return true;}
  if(q.operation==hud_text_pack_v1){r.value=c.text.pack();return true;}
  if(q.operation==hud_text_version_v1){if(!env.version){error="Required actual Application version producer";return false;}if(!env.version(env.application,q.limit,q.flag!=0,c.scratch,error))return false;r.text=c.scratch.c_str();return true;}
  if(q.operation==hud_text_title_v1){if(!env.title){error="Required actual Application title producer";return false;}if(!env.title(env.application,q.limit,c.scratch,error))return false;r.text=c.scratch.c_str();return true;}
  error="Unknown source potion varargs service";return false;
 }};
 const data::ItemTextArgumentV5 argument{static_cast<float>(count),count,nullptr};bool changed=false;
 return item_text_varargs_v5(format,&argument,1,services,out,changed,error);
}
}
