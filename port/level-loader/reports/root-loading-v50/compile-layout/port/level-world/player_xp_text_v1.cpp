#include "player_xp_text_v1.hpp"
namespace dh2::character::skills {
int player_xp_text_v1(std::uintptr_t victim,std::int32_t value,std::int32_t color,const PlayerXPTextServicesV1& services){
 const auto& s=services.common;
 if(!victim||!s.position||!s.height||!s.constant||!s.localized||!s.enqueue||!services.format)return -1;
 CombatTextRequestV1 request{};request.style="anim_sct_xp";request.color=color;request.numeric=false;
 float height{};if(s.position(s.context,victim,request.position)||s.height(s.context,victim,&height))return -2;
 volatile float raised=request.position[2]+height;request.position[2]=raised;
 std::int32_t id{};const char* localized{};
 if(s.constant(s.context,"StrID","GAMEPLAYMENUS_REWARD_XP",&id)||s.localized(s.context,id,&localized)||!localized)return -2;
 std::string text;if(services.format(s.context,localized,value,&text))return -2;
 request.text=text.c_str();return s.enqueue(s.context,&request)?-2:1;
}
}
