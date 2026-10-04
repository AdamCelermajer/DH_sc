#include "character_hud_skill_text_v1.hpp"
#include <cstring>
namespace dh2::character {
CharacterHudSkillTextV1::CharacterHudSkillTextV1(ui::HudTextV1& t,const ui::HudTextEnvironmentV1& e,void* p,decltype(actor_) resolve):text_(t),environment_(e),context_(p),actor_(resolve){}
int CharacterHudSkillTextV1::query(const ui::HudInitRequest64& q,ui::HudInitResponse32& r,std::string& e){
 using Op=ui::HudInitOperation;e.clear();
 switch(q.operation){case Op::constant:case Op::string_symbol:case Op::arguments_create:case Op::arguments_append:case Op::skill_info:case Op::property:case Op::parse_text:break;default:return -1;}
 if(q.reserved){e="HUD skill text malformed request";return 0;}
 try {
 if(q.operation==Op::constant){auto& s=environment_.localization;std::uint32_t value;if(!q.text||!q.name||!s.constant||!s.constant(s.context,q.text,q.name,value,e)){if(e.empty())e="HUD skill constant provider unavailable";return 0;}std::memcpy(&r.value,&value,4);return 1;}
 if(q.operation==Op::string_symbol){std::string text;bool null;if(!text_.integer_string(q.value,environment_.localization,text,null,e))return 0;if(null){r.text=nullptr;return 1;}strings_.push_back(std::move(text));r.text=strings_.back().c_str();return 1;}
 if(q.operation==Op::arguments_create){auto a=std::make_unique<Arguments>();r.identity=reinterpret_cast<std::uintptr_t>(a.get());arguments_.push_back(std::move(a));return 1;}
 if(q.operation==Op::arguments_append||q.operation==Op::parse_text){
  Arguments* args=nullptr;for(auto& a:arguments_)if(reinterpret_cast<std::uintptr_t>(a.get())==q.subject){args=a.get();break;}
  if(!args){e="HUD VarArgs foreign or expired handle";return 0;}
  if(q.operation==Op::arguments_append){if(args->values.size()>=65536){e="HUD VarArgs capacity exceeded";return 0;}args->values.push_back({static_cast<float>(q.number),q.value,nullptr});return 1;}
  std::string formatted;bool changed;if(!text_.parse_ex(q.text,args->values.data(),args->values.size(),environment_,formatted,changed,e))return 0;strings_.push_back(std::move(formatted));r.text=strings_.back().c_str();return 1;
 }
 skills::State40* state=nullptr;const skills::SkillInfoServicesV1* services=nullptr;PropertySheet16 temporary{};
 if(!actor_||!actor_(context_,q.subject,state,services,temporary)){e="HUD skill actor owner unavailable";return 0;}
 if(q.operation==Op::property){if(dh2_character_property_word(&r.value,&temporary,static_cast<std::int32_t>(q.index))){e="HUD skill temporary property getter malformed";return 0;}return 1;}
 if(skills::character_skill_info_v1(state,q.index,static_cast<std::uint32_t>(q.value),&r.fraction,services)){e="HUD skill OnSkillInfo provider failed";return 0;}return 1;
 }catch(const std::exception& ex){e=ex.what();return 0;}
}
}
