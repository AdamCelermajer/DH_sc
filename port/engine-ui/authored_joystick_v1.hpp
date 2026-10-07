#pragma once
#include <cstdint>
#include <string>
namespace dh2::ui {
struct AuthoredJoystickStateV1 {
 std::int32_t radius_x{},radius_y{},center_x{},center_y{}; // original C/10/14/18
 float direction[3]{},magnitude{}; // original 65c/660/664/668
 std::uint8_t active{}; // original byte A; SAME HUDControls owner
};
struct AuthoredJoystickServicesV1 {
 void* context{};
 bool (*position_stick)(void*,std::int32_t x,std::int32_t y,std::string&){};
 bool (*controller_allowed)(void*,bool&,std::string&){};
 // Actual Point3D.normalize + rotateXYBy source producer. Kernel supplies
 // original base(1,-1,0), angle degrees and origin(0,0,0).
 bool (*rotate_direction)(void*,const float base[3],float degrees,float out[3],std::string&){};
 bool (*head_towards)(void*,const float scaled_direction[3],std::string&){};
 bool (*stop)(void*,std::string&){};
};
// initCachedChars41a048..084 reads actual Joystick.bg virtual width (twips),
// radius=trunc(width/20 * .5), SAME radius in x/y. No deadzone introduced.
bool authored_joystick_initialize_v1(AuthoredJoystickStateV1&,float actual_bg_width_twips,std::string&);
// Original event4 stores event x/20,y/20 into center14/18.
bool authored_joystick_press_v1(AuthoredJoystickStateV1&,float source_event_x,float source_event_y,std::string&);
// Whole joystick event5 numeric/state/callback branch419764..419978. Input
// receiver LOCAL tx/ty must be the original event character matrix+8/+14.
bool authored_joystick_drag_v1(AuthoredJoystickStateV1&,float source_event_x,float source_event_y,
 float receiver_local_tx,float receiver_local_ty,const AuthoredJoystickServicesV1&,std::string&);
// Original event6/7 publishes center/stick reset and active0 BEFORE Cmd_Stop.
bool authored_joystick_release_v1(AuthoredJoystickStateV1&,bool actual_player_present,const AuthoredJoystickServicesV1&,std::string&);
// Joystick-only Update41a840/978..9d0; caller supplies genuine outer level/
// ready/local-player gates. No threshold: original sends direction*magnitude.
bool authored_joystick_update_v1(const AuthoredJoystickStateV1&,const AuthoredJoystickServicesV1&,std::string&);
}
