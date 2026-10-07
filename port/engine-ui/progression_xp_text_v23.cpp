#include "progression_xp_text_v23.hpp"
#include "item_text_varargs_v5.hpp"
namespace dh2::ui {namespace {
struct Context {HudTextV1& text;const HudTextEnvironmentV1& environment;std::string borrowed;};
bool invoke(void* raw,const HudTextRequestV1& q,HudTextResponseV1& r,std::string& error){
 auto& c=*static_cast<Context*>(raw);const auto& e=c.environment;
 switch(q.operation){
 case hud_text_constant_v1:{std::uint32_t v{};
  if(!e.localization.constant||!e.localization.constant(e.localization.context,q.group,q.key,v,error))return false;
  r.value=std::int32_t(v);return true;}
 case hud_text_integer_string_v1:{bool null{};
  if(!c.text.integer_string(q.value,e.localization,c.borrowed,null,error))return false;
  r.text=null?nullptr:c.borrowed.c_str();return true;}
 case hud_text_pack_v1:r.value=c.text.pack();return true;
 case hud_text_version_v1:
  if(!e.version||!e.version(e.application,q.limit,q.flag!=0,c.borrowed,error))return false;
  r.text=c.borrowed.c_str();return true;
 case hud_text_title_v1:
  if(!e.title||!e.title(e.application,q.limit,c.borrowed,error))return false;
  r.text=c.borrowed.c_str();return true;
 default:error="Unknown source XP varargs provider operation";return false;
 }
}
}
bool progression_xp_format_v23(HudTextV1& text,const HudTextEnvironmentV1& environment,
 const char* input,std::int32_t amount,std::string& output,std::string& error){
 Context context{text,environment,{}};const data::ItemTextArgumentV5 value{0,amount,nullptr};bool changed{};
 return item_text_varargs_v5(input,&value,1,{&context,invoke},output,changed,error);
}
}
