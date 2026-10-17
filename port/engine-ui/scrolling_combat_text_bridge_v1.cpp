#include "scrolling_combat_text_bridge_v1.hpp"
#include "scrolling_combat_text_projection_v1.hpp"

#include <cmath>
#include <limits>

namespace dh2::ui {

void decode_scrolling_combat_text_color_rgb_v1(std::int32_t source_color,
                                                std::uint8_t rgba[4]) noexcept {
    if(!rgba)return;
    const auto rgb=static_cast<std::uint32_t>(source_color)&0x00ffffffu;
    rgba[0]=static_cast<std::uint8_t>(rgb>>16);
    rgba[1]=static_cast<std::uint8_t>(rgb>>8);
    rgba[2]=static_cast<std::uint8_t>(rgb);
    rgba[3]=255;
}

bool ScrollingCombatTextBridgeV1::bind(const ScrollingCombatTextBridgeProvidersV1& p,
 const float matrix[16],int width,int height,std::string& error) noexcept {
    error.clear();
    if(!matrix||width<=0||height<=0||!p.is_follower||!p.position||
       !p.source_property||!p.is_dual_wielding||!p.is_local_player||
       !p.constant||!p.localized_string||!p.localized_formatted_string||
       !p.style_id||!p.decode_color_rgba||!p.play_authored_screen){
        error="Source SCT bridge is missing a required live owner";return false;
    }
    for(int i=0;i<16;++i)if(!std::isfinite(matrix[i])){
        error="Source SCT bridge camera matrix is non-finite";return false;
    }
    providers_=p;for(int i=0;i<16;++i)view_projection_[i]=matrix[i];
    width_=width;height_=height;bound_=true;return true;
}

ScrollingCombatTextServicesV1 ScrollingCombatTextBridgeV1::services() noexcept {
    if(!bound_)return {};
    ScrollingCombatTextServicesV1 out{};out.context=this;
    out.is_follower=follower;out.position=position;out.source_property=property;
    out.is_dual_wielding=dual;out.is_local_player=local;out.constant=constant;
    out.localized_string=localized;out.localized_formatted_string=formatted;
    out.style_id=style;out.play_authored_text=play_text;out.play_authored_value=play_value;
    return out;
}

bool ScrollingCombatTextBridgeV1::follower(void* raw,std::uintptr_t id,bool& out,std::string& e){
 auto& s=*static_cast<ScrollingCombatTextBridgeV1*>(raw);return s.bound_&&s.providers_.is_follower(s.providers_.source_context,id,out,e);
}
bool ScrollingCombatTextBridgeV1::position(void* raw,std::uintptr_t id,float out[3],std::string& e){
 auto& s=*static_cast<ScrollingCombatTextBridgeV1*>(raw);return s.bound_&&s.providers_.position(s.providers_.source_context,id,out,e);
}
bool ScrollingCombatTextBridgeV1::property(void* raw,std::uintptr_t id,std::int32_t prop,std::int32_t& out,std::string& e){
 auto& s=*static_cast<ScrollingCombatTextBridgeV1*>(raw);return s.bound_&&s.providers_.source_property(s.providers_.source_context,id,prop,out,e);
}
bool ScrollingCombatTextBridgeV1::dual(void* raw,std::uintptr_t id,bool& out,std::string& e){
 auto& s=*static_cast<ScrollingCombatTextBridgeV1*>(raw);return s.bound_&&s.providers_.is_dual_wielding(s.providers_.source_context,id,out,e);
}
bool ScrollingCombatTextBridgeV1::local(void* raw,std::uintptr_t id,bool& out,std::string& e){
 auto& s=*static_cast<ScrollingCombatTextBridgeV1*>(raw);return s.bound_&&s.providers_.is_local_player(s.providers_.source_context,id,out,e);
}
bool ScrollingCombatTextBridgeV1::constant(void* raw,const char* group,const char* key,std::int32_t& out,std::string& e){
 auto& s=*static_cast<ScrollingCombatTextBridgeV1*>(raw);return s.bound_&&s.providers_.constant(s.providers_.ui_context,group,key,out,e);
}
bool ScrollingCombatTextBridgeV1::localized(void* raw,std::int32_t id,std::string& out,std::string& e){
 auto& s=*static_cast<ScrollingCombatTextBridgeV1*>(raw);return s.bound_&&s.providers_.localized_string(s.providers_.ui_context,id,out,e);
}
bool ScrollingCombatTextBridgeV1::formatted(void* raw,std::int32_t id,std::int32_t argument,std::string& out,std::string& e){
 auto& s=*static_cast<ScrollingCombatTextBridgeV1*>(raw);return s.bound_&&s.providers_.localized_formatted_string(s.providers_.ui_context,id,argument,out,e);
}
bool ScrollingCombatTextBridgeV1::style(void* raw,const char* name,std::int32_t& out,std::string& e){
 auto& s=*static_cast<ScrollingCombatTextBridgeV1*>(raw);return s.bound_&&s.providers_.style_id(s.providers_.ui_context,name,out,e);
}
bool ScrollingCombatTextBridgeV1::play_text(void* raw,const char* name,const float xyz[3],const char* text,std::int32_t color,std::string& e){
 auto& s=*static_cast<ScrollingCombatTextBridgeV1*>(raw);if(!s.bound_){e="Source SCT bridge is not bound";return false;}
 float px[2]{};bool inside{};std::uint8_t rgba[4]{};
 if(!project_world_to_screen_pixels_v1(s.view_projection_.data(),s.width_,s.height_,xyz,px,&inside,e)||
    !s.providers_.decode_color_rgba(s.providers_.ui_context,color,rgba,e))return false;
 return s.providers_.play_authored_screen(s.providers_.ui_context,name,
    std::numeric_limits<std::uint32_t>::max(),px[0],px[1],text,rgba,e);
}
bool ScrollingCombatTextBridgeV1::play_value(void* raw,const char* name,const float xyz[3],std::int32_t value,std::int32_t color,std::string& e){
 const auto text=std::to_string(value);return play_text(raw,name,xyz,text.c_str(),color,e);
}

} // namespace dh2::ui
