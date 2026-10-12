#pragma once
// Authored HUD owner adapted from AdamCelermajer/DH_sc commit
// 11fa5242de525e0fd132d8019920baa862ef70d7.
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
 // MenuMessageManager<StatusMsg,4> dispatch: _root.onStatusMessage(0).
 // The SWF pulls the already queued localized text through its existing
 // NativeGetNextStatusMessage callback; this does not own or synthesize a queue.
 bool notify_status_message(std::string&);
 // MenuManager.Update: signed Character+14a8 raw OOI type, NOT AI target.
 bool update_action_icon(std::int8_t actual_object_of_interest_type,std::string&);
 bool refresh_action_icon(std::string&); // source menu-return/equipment refresh
 // PlayerManager::RemoveCharacter calls RenderFX::InvokeASCallback on the
 // retained menu_HUD_0 with completeRefresh and no arguments.
 bool complete_refresh(std::string&);
 // Script_ShowFlash 0x45f8c8 / Script_HideFlash 0x45f78c "HUD" branch: the HUD
 // root's DisplayOfCurrentHud(show) sets the saved-style menu_HUD_N visibility.
 bool display_of_current_hud(bool show,std::string&);
 static std::int32_t action_icon(std::int8_t actual_object_of_interest_type) noexcept;
 std::int32_t cached_action_icon()const noexcept{return action_icon_;}
 bool update(const std::array<std::int32_t,17>& actual_infos,
             const std::array<std::int32_t,3>& actual_skill_slots,
             std::int32_t actual_class,bool actual_dpad,std::string&);
 bool display(std::string&);
 // Synchronous read/write access to the authored enemy HUD clips. These
 // operations borrow only the existing gameplay SWF and retain no actors.
 bool enemy_visible(bool,std::string&);
 bool enemy_label(const char*,std::string&);
 bool enemy_text(std::uint32_t,const char*,std::string&);
 bool enemy_frame(std::int32_t,bool,std::string&);
 bool enemy_play(std::string&);
 bool enemy_bar_is_sprite(bool&,std::string&);
 // DELIBERATE USER-APPROVED DEVIATION: the source HealthBars.enemy group sits at
 // a fixed HUD position. Here the group follows the displayed target: its
 // bounds centre is placed on the projected head point (world +350 Z, same
 // offset InfoHUDManager uses for allies) using the gameplay view-projection.
 bool enemy_follow(const float head_world[3],const float view_projection[16],
                   int viewport_width,int viewport_height,std::string&);
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
