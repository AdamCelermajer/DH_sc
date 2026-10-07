#include "swf_anim_tooltip_v104.hpp"
#include <cstring>
namespace dh2::ui {
bool SwfAnimTooltipV104::construct(std::string& e){
 if(construct_attempted_||!services_.owner||!services_.grab){e="Required fresh actual SWFAnimToolTip/source manager";return false;}construct_attempted_=true;
 if(!services_.grab("anim_item_info",anim8_,e))return false;
 if(!anim8_){if(!services_.missing_anim_assertion){e="Required original m_swfAnim!=0 assertion";return false;}return services_.missing_anim_assertion(e);}return true;
}
bool SwfAnimTooltipV104::destroy(std::string& e){
 if(destroyed_||!services_.drop){e="Required actual SWFAnimToolTip deleting prefix";return false;}
 if(!services_.drop(anim8_,e))return false;anim8_=0;destroyed_=true;e.clear();return true;
}
bool SwfAnimTooltipV104::play(const char* label,std::string& e){if(!services_.play){e="Required actual SWFAnim.PlayAnim";return false;}return services_.play(anim8_,label,e);}
bool SwfAnimTooltipV104::over(bool& result,std::string& e){bool playing{};if(!services_.playing||!services_.playing(anim8_,playing,e)){if(e.empty())e="Required actual SWFAnim.IsAnimPlaying";return false;}result=!playing;return true;}
bool SwfAnimTooltipV104::is_visible(bool& out,std::string& e){out=false;if(!state4_||!anim8_)return true;if(!services_.visible){e="Required source SWFAnim.IsVisible";return false;}return services_.visible(anim8_,out,e);}
bool SwfAnimTooltipV104::do_fade_out(std::string& e){state4_=3;return play("fade_out",e);}
bool SwfAnimTooltipV104::fade_out(std::int32_t delay,std::string& e){
 if(std::uint32_t(state4_)-1u>1u||delay0_>0)return true;delay0_=delay;
 return delay>=0||do_fade_out(e);
}
bool SwfAnimTooltipV104::fade_in(std::string& e){
 if(state4_==0||state4_==3){
  if(!anim8_){if(!services_.grab||!services_.grab("anim_item_info",anim8_,e))return false;}
  state4_=1;if(!play("fade_in",e)||!services_.visibility||!services_.visibility(anim8_,true,e))return false;
 }
 delay0_=0;e.clear();return true;
}
bool SwfAnimTooltipV104::update(std::string& e){
 if(destroyed_){e="Released SWFAnimToolTip Update receiver";return false;}
 if(state4_==0){if(anim8_){if(!services_.drop||!services_.drop(anim8_,e))return false;anim8_=0;}return true;}
 if(state4_==1){bool done{};if(!over(done,e))return false;if(done){if(!play("show",e))return false;state4_=2;}return true;}
 if(state4_==2){if(delay0_<=0)return true;std::uint32_t dt{};if(!services_.dt||!services_.dt(dt,e))return false;
  const auto bits=std::uint32_t(delay0_)-dt;std::memcpy(&delay0_,&bits,4);return delay0_>0||do_fade_out(e);}
 if(state4_==3){bool done{};if(!over(done,e))return false;if(done)state4_=0;}return true;
}
}
