#include "../app/src/main/cpp/native_camera_input_v1.hpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <string>

namespace {
unsigned checks = 0;
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
    ++checks;
}
bool near(float a, float b) { return std::fabs(a - b) < 2e-6f; }
void compare(float yaw, float pitch, const float input[3], const float expected[3]) {
    float actual[3]{input[0], input[1], input[2]};
    check(dh2::native::camera_input_v1::rotate_ground_input(actual, yaw, pitch) == 0,
          "source camera input rejected valid yaw");
    check(near(actual[0], expected[0]) && near(actual[1], expected[1]) &&
          near(actual[2], expected[2]), "camera-relative heading differs from source rotation");
}
}

int main() {
    constexpr float half_pi = 1.57079632679489661923f;
    constexpr float pi = 3.14159265358979323846f;
    constexpr float pitch = 0.35f;
    constexpr float forward[3]{0, 1, 0.25f};
    constexpr float strafe[3]{1, 0, 0.5f};
    constexpr float forward_default[3]{0, 1, 0.25f};
    constexpr float forward_yaw_zero[3]{-1, 0, 0.25f};
    constexpr float forward_yaw_positive[3]{0, -1, 0.25f};
    constexpr float strafe_yaw_zero[3]{0, 1, 0.5f};
    const float forward_with_pitch[3]{-std::sin(pitch), std::cos(pitch), 0.25f};
    compare(-half_pi, 0, forward, forward_default);
    compare(0, 0, forward, forward_yaw_zero);
    compare(half_pi, 0, forward, forward_yaw_positive);
    compare(0, 0, strafe, strafe_yaw_zero);
    compare(-half_pi, pitch, forward, forward_with_pitch);

    // Pin the original unsigned Point3D::angle limitation at the far orbit
    // quadrant: camera looks +X, but acos loses that sign and forward rotates
    // the same way as yaw 0. This is evidence, not a claim that yaw is fixed.
    float opposite_look[3]{};
    check(dh2::native::camera_input_v1::camera_look_at_from_orbit(pi,0,opposite_look)==0&&
          opposite_look[0]>0.999f,
          "opposite orbit quadrant must point the camera look vector toward +X");
    compare(pi, 0, forward, forward_yaw_zero);

    float look_at[3]{};
    check(dh2::native::camera_input_v1::camera_look_at_from_orbit(-half_pi,pitch,look_at)==0,
          "orbit failed to provide source camera look-at vector");
    check(near(look_at[0],0)&&near(look_at[1],std::cos(pitch))&&near(look_at[2],-std::sin(pitch)),
          "orbit look-at vector differs from center-minus-eye camera direction");

    // LevelConfig::InitPost (IDA 0x3f28ec) selects CameraTests.bdae and
    // PlayerCamera_Default for levels with an empty camera file/name. Its
    // PlayerCamera_Default-node has this CameraTests eye offset, distinct
    // from the separately preserved Crypt-gated playercamera.bdae rig.
    constexpr const float* swamp_eye_offset =
        dh2::native::camera_input_v1::kSwampDefaultCameraEyeOffset;
    float swamp_yaw=0,swamp_pitch=0;
    float swamp_distance=0;
    check(dh2::native::camera_input_v1::orbit_from_eye_offset(
              swamp_eye_offset,&swamp_yaw,&swamp_pitch,&swamp_distance)==0&&
          near(swamp_yaw,-0.785398163f)&&near(swamp_pitch,0.8981458f),
          "CameraTests authored eye offset converts to the SWAMP fallback orbit basis");
    float swamp_look_at[3]{};
    const float swamp_eye_length=std::sqrt(swamp_eye_offset[0]*swamp_eye_offset[0]+
        swamp_eye_offset[1]*swamp_eye_offset[1]+swamp_eye_offset[2]*swamp_eye_offset[2]);
    check(near(swamp_distance,swamp_eye_length),
          "SWAMP fallback camera preserves the authored CameraTests eye distance");
    check(dh2::native::camera_input_v1::camera_look_at_from_orbit(
              swamp_yaw,swamp_pitch,swamp_look_at)==0&&
          near(swamp_look_at[0],-swamp_eye_offset[0]/swamp_eye_length)&&
          near(swamp_look_at[1],-swamp_eye_offset[1]/swamp_eye_length)&&
          near(swamp_look_at[2],-swamp_eye_offset[2]/swamp_eye_length),
          "SWAMP fallback screen basis uses CameraTests target-minus-eye direction");
    float swamp_right[3]{swamp_look_at[1],-swamp_look_at[0],0.0f};
    const float swamp_right_length=std::hypot(swamp_right[0],swamp_right[1]);
    for(float& component:swamp_right)component/=swamp_right_length;
    const float swamp_renderer_up[3]{
        swamp_right[1]*swamp_look_at[2]-swamp_right[2]*swamp_look_at[1],
        swamp_right[2]*swamp_look_at[0]-swamp_right[0]*swamp_look_at[2],
        swamp_right[0]*swamp_look_at[1]-swamp_right[1]*swamp_look_at[0]};
    // BDAE camera quaternion rotates its local upvector node to this authored
    // world-up direction; the renderer's global-Z basis reproduces it.
    constexpr float swamp_authored_up[3]{-0.5530796f,0.5530797f,0.6230616f};
    check(near(swamp_renderer_up[0],swamp_authored_up[0])&&
          near(swamp_renderer_up[1],swamp_authored_up[1])&&
          near(swamp_renderer_up[2],swamp_authored_up[2]),
          "SWAMP renderer screen-up matches the CameraTests authored upvector");

    bool active=true;
    float tiny[3]{0.2f,0,0.9f};
    check(dh2::native::camera_input_v1::map_touch_ground_input(tiny,0,0,false,&active)==0&&
          !active&&near(tiny[0],0)&&near(tiny[1],0)&&near(tiny[2],0),
          "source radial stick deadzone clears input below 0.25");
    float edge[3]{0.25f,0,0};
    check(dh2::native::camera_input_v1::map_touch_ground_input(edge,0,0,false,&active)==0&&
          !active&&near(edge[0],0),"exact 0.25 source deadzone edge maps to zero");
    float half[3]{0,0.5f,9};
    check(dh2::native::camera_input_v1::map_touch_ground_input(half,0,0,false,&active)==0&&
          active&&near(half[0],0)&&near(half[1],1.0f/3.0f)&&near(half[2],0),
          "source radial curve maps half-stick magnitude to one third");
    float diagonal[3]{0.5f,0.5f,0};
    const float diagonal_magnitude=(std::sqrt(0.5f)-0.25f)/0.75f;
    check(dh2::native::camera_input_v1::map_touch_ground_input(diagonal,0,0,false,&active)==0&&
          active&&near(diagonal[0],diagonal_magnitude/std::sqrt(2.0f))&&
          near(diagonal[1],diagonal_magnitude/std::sqrt(2.0f)),
          "source radial curve preserves diagonal heading while normalizing length");
    float camera_relative[3]{0,0.5f,0};
    check(dh2::native::camera_input_v1::map_touch_ground_input(
              camera_relative,-half_pi,pitch,true,&active)==0&&active&&
          near(camera_relative[0],-std::sin(pitch)/3.0f)&&
          near(camera_relative[1],std::cos(pitch)/3.0f),
          "camera-relative stick mapping uses full pitch-aware look vector after radial curve");

    // HUDControls::OnEvent maps touchscreen stick angle through its authored
    // (1,-1) basis; this is independent of the gamepad camera-relative path.
    float hud_right[3]{1,0,0};
    check(dh2::native::camera_input_v1::map_hud_touch_input(hud_right,&active)==0&&
          active&&near(hud_right[0],std::sqrt(0.5f))&&
          near(hud_right[1],std::sqrt(0.5f))&&near(hud_right[2],0),
          "touchscreen right maps through the source HUD 45-degree basis");
    float hud_up[3]{0,1,0};
    check(dh2::native::camera_input_v1::map_hud_touch_input(hud_up,&active)==0&&
          active&&near(hud_up[0],-std::sqrt(0.5f))&&
          near(hud_up[1],std::sqrt(0.5f)),
          "touchscreen up preserves the source HUD screen-Y sign");
    float movement_control_up[3]{0,1,0};
    check(dh2::native::camera_input_v1::map_movement_control_input(
              movement_control_up,&active)==0&&active&&
          near(movement_control_up[0],-std::sqrt(0.5f))&&
          near(movement_control_up[1],std::sqrt(0.5f))&&movement_control_up[2]==0,
          "Android MovementControl follows HUDControls direct HeadTowards vector");
    float beyond_joystick_axis[3]{1.2f,0.8f,9.0f};
    const float original_angle_radius=std::hypot(1.2f,0.8f);
    const float original_direction_x=1.2f/original_angle_radius;
    const float original_direction_y=0.8f/original_angle_radius;
    check(dh2::native::camera_input_v1::map_movement_control_input(
              beyond_joystick_axis,&active)==0&&active&&
          near(beyond_joystick_axis[0],(original_direction_x-original_direction_y)*std::sqrt(0.5f))&&
          near(beyond_joystick_axis[1],(original_direction_x+original_direction_y)*std::sqrt(0.5f))&&
          near(std::hypot(beyond_joystick_axis[0],beyond_joystick_axis[1]),1.0f),
          "over-radius HUD stick retains source angle while capping magnitude");
    float gamepad_up[3]{0,1,0};
    check(dh2::native::camera_input_v1::map_touch_ground_input(
              gamepad_up,0.31f,0.89f,true,&active)==0&&active&&
          (!near(gamepad_up[0],movement_control_up[0])||
           !near(gamepad_up[1],movement_control_up[1])),
          "HUD MovementControl avoids the separate gamepad camera-relative remap");
    float hud_partial[3]{0.1f,0,8};
    check(dh2::native::camera_input_v1::map_hud_touch_input(hud_partial,&active)==0&&
          active&&near(hud_partial[0],std::sqrt(0.005f))&&
          near(hud_partial[1],std::sqrt(0.005f))&&near(hud_partial[2],0),
          "touchscreen input preserves analog magnitude below the gamepad deadzone");
    float hud_below_heading_threshold[3]{0.009f,0,0};
    check(dh2::native::camera_input_v1::map_hud_touch_input(
              hud_below_heading_threshold,&active)==0&&!active,
          "touchscreen activity follows the source heading threshold");

    // Crypt remains gated on the separately verified playercamera.bdae route.
    // CameraBase input uses target-minus-eye, then unsigned Point3D::angle and
    // rotateXY; this is not the default SWAMP CameraTests.bdae pose above.
    constexpr float rig_eye_offset[3]{1380.0f,-1180.46f,2551.55f};
    const float rig_yaw=std::atan2(rig_eye_offset[1],rig_eye_offset[0]);
    const float rig_pitch=std::atan2(rig_eye_offset[2],
        std::hypot(rig_eye_offset[0],rig_eye_offset[1]));
    float rig_look_at[3]{};
    const float rig_length=std::sqrt(rig_eye_offset[0]*rig_eye_offset[0]+
        rig_eye_offset[1]*rig_eye_offset[1]+rig_eye_offset[2]*rig_eye_offset[2]);
    check(dh2::native::camera_input_v1::camera_look_at_from_orbit(
              rig_yaw,rig_pitch,rig_look_at)==0&&
          near(rig_look_at[0],-rig_eye_offset[0]/rig_length)&&
          near(rig_look_at[1],-rig_eye_offset[1]/rig_length)&&
          near(rig_look_at[2],-rig_eye_offset[2]/rig_length),
          "shared authored eye offset reconstructs CameraBase target-minus-eye vector");
    float rig_forward[3]{0.0f,1.0f,0.0f};
    check(dh2::native::camera_input_v1::map_touch_ground_input(
              rig_forward,rig_yaw,rig_pitch,true,&active)==0&&active&&
          near(rig_forward[0],-std::sin(std::acos(rig_look_at[1])))&&
          near(rig_forward[1],std::cos(std::acos(rig_look_at[1]))),
          "player forward touch follows source Point3D angle/rotateXY for authored camera basis");

    // Match Point3D::angleCos binary32 evaluation order with a non-unit camera
    // look vector. This catches normalized/clamped cosine substitutions.
    constexpr float irregular_look_at[3]{0.31415927f,-0.27182818f,0.90625f};
    float source_order_direction[3]{0.375f,-0.625f,0.125f};
    const float look_length_squared=((irregular_look_at[0]*irregular_look_at[0])+
        (irregular_look_at[1]*irregular_look_at[1]))+
        (irregular_look_at[2]*irregular_look_at[2]);
    const float look_length=std::sqrt(look_length_squared);
    const float cosine=irregular_look_at[1]/(1.0f*look_length);
    const float source_angle=std::acos(cosine);
    const float expected_x=source_order_direction[0]*std::cos(source_angle)-
        source_order_direction[1]*std::sin(source_angle);
    const float expected_y=source_order_direction[0]*std::sin(source_angle)+
        source_order_direction[1]*std::cos(source_angle);
    check(dh2_nav_rotate_input_for_camera(source_order_direction,irregular_look_at)==0&&
          near(source_order_direction[0],expected_x)&&
          near(source_order_direction[1],expected_y)&&
          source_order_direction[2]==0.125f,
          "camera joystick matches original Point3D binary32 angle/rotateXY order");

    // HeadTowards is a heading-only source command. It must not replace the
    // caller-owned route destination while publishing the new visual angle.
    dh2::navigation::HeadingState heading{{0,0,0},0.25f,0,0};
    float movement_angle=heading.angle;
    float destination[3]{120,240,360};
    float direct[3]{0.5f,0.5f,0};
    const float destination_before[3]{destination[0],destination[1],destination[2]};
    check(dh2::native::camera_input_v1::apply_head_towards(
              &heading,&movement_angle,direct)==0&&heading.active&&
          near(heading.direction[0],direct[0])&&near(heading.direction[1],direct[1])&&
          near(movement_angle,heading.angle)&&
          std::memcmp(destination,destination_before,sizeof(destination))==0,
          "HeadTowards updates heading without taking ownership of path destination");
    const float stopped_angle=heading.angle;
    float stopped[3]{};
    check(dh2::native::camera_input_v1::apply_head_towards(
              &heading,&movement_angle,stopped)==0&&!heading.active&&
          heading.direction[0]==0&&heading.direction[1]==0&&
          movement_angle==stopped_angle&&
          std::memcmp(destination,destination_before,sizeof(destination))==0,
          "zero HeadTowards clears movement while retaining heading angle and destination");

    float unchanged[3]{0.2f, -0.7f, 0.3f};
    const float before[3]{unchanged[0], unchanged[1], unchanged[2]};
    check(dh2::native::camera_input_v1::rotate_ground_input(
              unchanged, std::numeric_limits<float>::quiet_NaN(),0) == 1,
          "nonfinite yaw accepted");
    check(std::memcmp(unchanged, before, sizeof(before)) == 0,
          "invalid yaw mutated movement input");
    check(dh2::native::camera_input_v1::rotate_ground_input(nullptr, 0,0) == 1,
          "null movement input accepted");
    float bad[3]{0,0.5f,0};
    const float bad_before[3]{bad[0],bad[1],bad[2]};
    active=false;
    check(dh2::native::camera_input_v1::map_touch_ground_input(
              bad,std::numeric_limits<float>::quiet_NaN(),0,true,&active)==1&&
          !active&&std::memcmp(bad,bad_before,sizeof(bad))==0,
          "invalid camera frame preserves touch input and activity state");
    std::printf("PASS: source gamepad-to-touch camera mapping (%u assertions)\n",checks);
    return 0;
}
