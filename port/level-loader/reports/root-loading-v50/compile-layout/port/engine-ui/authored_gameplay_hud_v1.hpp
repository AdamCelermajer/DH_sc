#pragma once
#include "swf_movie.hpp"
#include <array>
namespace dh2::ui {
enum class AuthoredHudControlV1 : unsigned {pause,character,potion,faery,skill1,skill2,skill3,attack,joystick};
struct AuthoredHudGeometryV1 {
 float bounds[4]{}; // world twips: xmin,xmax,ymin,ymax, authored shape bounds
 float local[2]{};  // exact pointer in the control's own coordinate system
 float local_matrix[6]{}; // original receiver matrix, same row-major ordering
 bool hit{};
 std::int32_t character_id{};
};
// Borrow ONE existing movie; no player, texture, skill or settings owner is created.
// Native callbacks must already be installed before this movie loads.
class AuthoredGameplayHudV1 {
public:
 explicit AuthoredGameplayHudV1(SwfMovie& movie):movie_(movie){}
 bool bind(std::int32_t actual_saved_style,std::string&);
 bool activate(std::string&); // whole root DisplayRightHud selects CurrentHud
 bool refresh_skills(std::string&); // actual setSkillsButtons native queries
 // MenuManager.Update: signed Character+14a8 raw OOI type, NOT AI target.
 bool update_action_icon(std::int8_t actual_object_of_interest_type,std::string&);
 bool refresh_action_icon(std::string&); // source menu-return/equipment refresh
 static std::int32_t action_icon(std::int8_t actual_object_of_interest_type) noexcept;
 std::int32_t cached_action_icon()const noexcept{return action_icon_;}
 bool update(const std::array<std::int32_t,17>& actual_infos,
             std::int32_t actual_class,bool actual_dpad,std::string&);
 bool display(std::string&);
 // Screen coordinate conversion uses the movie's retained source viewport.
 // Hit is GameSWF shape traversal, never a manufactured circle/rectangle.
 bool geometry(AuthoredHudControlV1,float screen_x,float screen_y,AuthoredHudGeometryV1&,std::string&);
 bool release(AuthoredHudControlV1,std::string&); // actual authored onRelease
 // Visual transport only: caller supplies its actual controller displacement
 // in joystick-local twips. No radius/deadzone/axis policy is invented here.
 bool joystick_stick_offset(float x_twips,float y_twips,std::string&);
 bool joystick_stick_reset(std::string&);
 bool joystick_stick_position(std::int32_t source_x_pixels,std::int32_t source_y_pixels,std::string&);
 bool joystick_background_width(float& actual_width_twips,std::string&);
 bool joystick_receiver_geometry(float screen_x,float screen_y,AuthoredHudGeometryV1&,std::string&);
 const std::string& menu_path()const noexcept{return menu_;}
 const std::string& elements_path()const noexcept{return elements_;}
 const std::string& control_path(AuthoredHudControlV1 c)const{return controls_.at(static_cast<unsigned>(c));}
 std::int32_t style()const noexcept{return style_;}
private:
 bool invoke(const std::string&,const char*,const std::vector<SwfAsValue>&,std::string&);
 bool frame(const std::string&,std::int32_t source_zero_based,std::string&);
 SwfMovie& movie_;std::int32_t style_{-1};std::string menu_,elements_;
 std::array<std::string,9> controls_{};
 float stick_rest_[2]{};
 std::int32_t action_icon_{-1}; // MenuManager C1 431d08..431d0c
};
}
