#include "../app/src/main/cpp/native_camera_crypt_frame_v1.hpp"

#include <cmath>
#include <cstdio>
#include <limits>

using namespace dh2::native::crypt_camera_frame_v1;

namespace {
bool near(float a, float b, float epsilon = 0.02f) {
    return std::fabs(a - b) <= epsilon;
}
void transform(const Matrix& m, const Vec3& p, float out[4]) {
    const float in[4]{p[0], p[1], p[2], 1.0f};
    for (int row = 0; row < 4; ++row) {
        out[row] = 0.0f;
        for (int column = 0; column < 4; ++column)
            out[row] += m[column * 4 + row] * in[column];
    }
}
}

int main() {
    int checks = 0;
    auto expect = [&](bool ok, const char* message) {
        if (!ok) { std::fprintf(stderr, "FAIL: %s\n", message); return false; }
        ++checks; return true;
    };
    if (!expect(verified_crypt_route("GOTHICUS_CRYPT_01", "007_crypt_01.rule.xml"), "Crypt gate")) return 1;
    if (!expect(!verified_crypt_route("SWAMP", "001_swamp.mlx"), "SWAMP excluded")) return 2;
    if (!expect(!verified_crypt_route("GOTHICUS_CRYPT_01", "x07_crypt_backup.mlx"), "wrong route file excluded")) return 3;
    if (!expect(source_player_camera_route_ready("GOTHICUS_CRYPT_01", "007_crypt_01.rule.xml", true, true),
                "verified Crypt route enables the authored player camera only when both providers are ready")) return 48;
    if (!expect(source_player_camera_route_ready("SWAMP", "001_swamp.mlx", true, true),
                "verified SWAMP LevelConfig, CameraTests rig and selected CamAnimSet enable the shared source camera owner")) return 49;
    if (!expect(!source_player_camera_route_ready("SWAMP", "001_swamp.mlx", false, true) &&
                !source_player_camera_route_ready("SWAMP", "001_swamp.mlx", true, false) &&
                !source_player_camera_route_ready("SWAMP", "001_swamp_backup.mlx", true, true),
                "SWAMP source camera still requires both providers and the exact level route")) return 55;
    if (!expect(verified_swamp_default_camera_route("SWAMP", "001_swamp.mlx") &&
                !verified_swamp_default_camera_route("SWAMP", "001_swamp_backup.mlx"),
                "SWAMP CameraTests rig is selected only for the exact source LevelConfig")) return 51;
    if (!expect(source_camera_rig_route_ready("SWAMP", "001_swamp.mlx", true, true) &&
                !source_camera_rig_route_ready("SWAMP", "001_swamp.mlx", false, true) &&
                !source_camera_rig_route_ready("SWAMP", "001_swamp.mlx", true, false),
                "SWAMP authored static rig requires both source LevelConfig and BDAE providers")) return 52;
    if (!expect(source_swamp_static_camera_fallback_ready("SWAMP", "001_swamp.mlx", true) &&
                !source_swamp_static_camera_fallback_ready("SWAMP", "001_swamp.mlx", false) &&
                !source_swamp_static_camera_fallback_ready("GOTHICUS_CRYPT_01", "007_crypt_01.rule.xml", true),
                "missing optional SWAMP CameraTests rig uses only its verified static camera fallback")) return 54;
    if (!expect(source_camera_rig_route_ready("GOTHICUS_CRYPT_01", "007_crypt_01.rule.xml", true, true) &&
                !source_camera_rig_route_ready("GOTHICUS_CRYPT_01", "other.rule.xml", true, true),
                "generic authored-rig route preserves Crypt and exact-level gating")) return 53;
    if (!expect(!source_player_camera_route_ready("GOTHICUS_CRYPT_01", "007_crypt_01.rule.xml", false, true) &&
                !source_player_camera_route_ready("GOTHICUS_CRYPT_01", "007_crypt_01.rule.xml", true, false),
                "Crypt route remains disabled when either camera provider is unavailable")) return 50;
    if (!expect(near(kVerticalFovRadians, 0.42963001132011414f, 1e-8f) &&
                kNearPlane == 900.0f && kFarPlane == 5000.0f,
                "Crypt clip planes and shared Crypt/SWAMP CameraBase::SetData FOV match source")) return 45;

    const Vec3 receipt_target{-2227.77f, 1220.93f, 842.364f};
    Frame frame{};
    if (!expect(build("GOTHICUS_CRYPT_01", "007_crypt_01.rule.xml", receipt_target, 2400, 1080, &frame), "build Crypt frame")) return 4;
    if (!expect(near(frame.eye[0], -847.77f) && near(frame.eye[1], -159.07f) &&
                near(frame.eye[2], 3292.364f), "reproduces Adam v20 observed eye from target-relative source frame")) return 5;
    if (!expect(near(frame.aspect, 2.2222223f, 1e-5f), "modern surface aspect")) return 6;
    if (!expect(near(frame.input_yaw, -0.78539816f, 1e-6f), "movement yaw follows source eye azimuth")) return 7;
    if (!expect(near(frame.input_pitch, input_pitch_radians(), 1e-6f) && frame.input_pitch > 0.89f,
                "movement pitch follows source eye elevation")) return 8;
    const Vec3 authored_eye_offset{1380.0f,-1180.46f,2551.55f};
    // The BDAE upvector node is authored as a point beside the camera node.
    // Its vector therefore starts at the eye, not at the look-at target.
    const Vec3 authored_up{-404.008f,345.604f,378.41f};
    const Vec3 authored_target_offset{2.0f,-3.0f,4.0f};
    Frame authored_frame{};
    if (!expect(build("GOTHICUS_CRYPT_01", "007_crypt_01.rule.xml", receipt_target,
                      2400,1080,&authored_frame,authored_eye_offset,authored_up,
                      authored_target_offset),
                "build frame from authored camera rig vectors")) return 41;
    if (!expect(near(authored_frame.target[0],receipt_target[0]+authored_target_offset[0]) &&
                near(authored_frame.target[1],receipt_target[1]+authored_target_offset[1]) &&
                near(authored_frame.target[2],receipt_target[2]+authored_target_offset[2]),
                "camera rig target transform is applied to the player anchor")) return 44;
    if (!expect(near(authored_frame.eye[0],authored_frame.target[0]+authored_eye_offset[0]) &&
                near(authored_frame.eye[1],authored_frame.target[1]+authored_eye_offset[1]) &&
                near(authored_frame.eye[2],authored_frame.target[2]+authored_eye_offset[2]),
                "authored rig eye offset positions Crypt camera")) return 42;
    if (!expect(near(authored_frame.input_yaw,std::atan2(authored_eye_offset[1],authored_eye_offset[0])) &&
                near(authored_frame.input_pitch,std::atan2(authored_eye_offset[2],
                    std::hypot(authored_eye_offset[0],authored_eye_offset[1]))),
                "movement basis follows authored rig eye offset")) return 43;
    Vec3 composed_target{};
    if (!expect(apply_target_camera_local_z({3.0f,4.0f,0.0f},{0.0f,0.0f,1.0f},-1.0f,
                                             &composed_target) &&
                near(composed_target[0],3.0f) && near(composed_target[1],4.0f) &&
                near(composed_target[2],-1.0f),
                "negative CameraLevel local-Z zoom moves the target through its authored node basis")) return 54;
    Frame zoomed_frame{};
    if (!expect(build_player_frame(receipt_target,2400,1080,&zoomed_frame,
                    kVerticalFovRadians,kNearPlane,kFarPlane,authored_eye_offset,
                    authored_up,authored_target_offset,-100.0f,{0.0f,0.0f,1.0f}),
                "build Crypt frame with the CameraLevel local-Z offset")) return 55;
    if (!expect(near(zoomed_frame.target[0],authored_frame.target[0]) &&
                near(zoomed_frame.target[1],authored_frame.target[1]) &&
                near(zoomed_frame.target[2],authored_frame.target[2]-100.0f) &&
                near(zoomed_frame.eye[0],authored_frame.eye[0]) &&
                near(zoomed_frame.eye[1],authored_frame.eye[1]) &&
                near(zoomed_frame.eye[2],authored_frame.eye[2]),
                "rendered camera applies CameraLevel local-Z to the target node while preserving the authored eye")) return 56;
    float clip[4]{};
    const Vec3 authored_up_point{authored_frame.target[0]+authored_up[0],
                                 authored_frame.target[1]+authored_up[1],
                                 authored_frame.target[2]+authored_up[2]};
    transform(authored_frame.view_projection,authored_up_point,clip);
    if (!expect(clip[1] / clip[3] > 0.0f,
                "authored camera up direction projects to positive GLES screen Y")) return 47;

    ForwardAnchorState anchor{};
    ForwardAnchorState owned_anchor{};
    if (!expect(initialize_character_camera_anchor(&owned_anchor,0x1234,
                    {4.0f,5.0f,6.0f},false,800.0f,20.0f,25.0f) &&
                owned_anchor.source_initialized && owned_anchor.character_identity==0x1234 &&
                near(owned_anchor.source_threshold,0.25f) &&
                near(owned_anchor.target[0],4.0f),
                "Character InitCam retains its identity, source inputs and AnchorBase reset position")) return 57;
    if (!expect(update_character_camera_anchor(&owned_anchor,0x1234,
                    {4.0f,5.0f,6.0f},{0.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},
                    true,true,false) && near(owned_anchor.distance,400.0f),
                "owned AnchorForward uses CharacterDesign max distance and per-frame increment")) return 58;
    if (!expect(!retire_character_camera_anchor(&owned_anchor,0x9999) &&
                owned_anchor.source_initialized &&
                retire_character_camera_anchor(&owned_anchor,0x1234) &&
                !owned_anchor.source_initialized,
                "SetCameraAnchorObject lifetime retires only for the matching Character identity")) return 59;
    ForwardAnchorState static_anchor{};
    if (!expect(initialize_character_camera_anchor(&static_anchor,0x4321,
                    {1.0f,2.0f,3.0f},true,0.0f,0.0f,0.0f) &&
                update_character_camera_anchor(&static_anchor,0x4321,
                    {7.0f,8.0f,9.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},
                    false,false,false) && near(static_anchor.target[0],7.0f) &&
                near(static_anchor.target[1],8.0f) && near(static_anchor.target[2],9.0f),
                "UseStaticCamera follows the Character position through its AnchorBase owner")) return 60;
    if (!expect(update_forward_anchor(&anchor, 0.0f, false, false) &&
                near(anchor.distance, 0.0f), "idle camera anchor starts at the character")) return 21;
    if (!expect(update_forward_anchor(&anchor, 0.0f, true, true) &&
                near(anchor.distance, 320.0f), "AnchorForward reaches its half-distance startup ramp")) return 22;
    if (!expect(update_forward_anchor(&anchor, 0.0f, true, true) &&
                near(anchor.distance, 337.0f), "AnchorForward advances by its source frame increment")) return 23;
    if (!expect(update_forward_anchor(&anchor, 1.570796327f, true, true) &&
                near(anchor.distance, 332.75f), "sharp facing turn retreats the source anchor")) return 24;
    if (!expect(update_forward_anchor(&anchor, 1.570796327f, false, false) &&
                near(anchor.distance, kForwardAnchorIdleCap), "idle anchor retains only 40 percent of maximum")) return 25;
    const Vec3 forward_anchor = player_camera_anchor({1.0f, 2.0f, 3.0f}, 0.0f, 256.0f);
    if (!expect(near(forward_anchor[0], 1.0f) && near(forward_anchor[1], -254.0f) &&
                near(forward_anchor[2], 3.0f), "camera anchor uses GameObject::GetLookAtVec orientation")) return 26;
    if (!expect(!player_displaced_enough_for_forward_anchor({0.2f, 0.0f, 0.0f}) &&
                player_displaced_enough_for_forward_anchor({0.0f, 0.0f, 0.23f}),
                "AnchorForward displacement threshold includes vertical movement")) return 27;
    ForwardAnchorState threshold_turn{320.0f, 0.0f, true};
    if (!expect(update_forward_anchor(&threshold_turn, 1.0f, true, true) &&
                near(threshold_turn.distance, 337.0f),
                "one-radian source turn threshold does not trigger retreat")) return 28;
    if (!expect(!player_displaced_enough_for_forward_anchor(
                    {0.0f, 0.0f, std::numeric_limits<float>::infinity()}),
                "nonfinite displacement cannot expand camera anchor")) return 29;
    ForwardAnchorState look_at_gate{};
    if (!expect(update_forward_anchor(&look_at_gate,{10.0f,20.0f,30.0f},
                                     {1.0f,0.0f,0.0f},{0.0f,1.0f,0.0f},
                                     true,true,false) && near(look_at_gate.distance,320.0f),
                "source zero-initialized look-at keeps first active frame finite and ramps to half distance")) return 34;
    ForwardAnchorState source_heading_gate{};
    if (!expect(update_forward_anchor(&source_heading_gate,{0.0f,0.0f,0.0f},
                                     {0.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},
                                     false,true,false) &&
                near(source_heading_gate.distance,320.0f) &&
                update_forward_anchor(&source_heading_gate,{0.0f,0.0f,0.0f},
                                     {0.0f,0.0f,0.0f},{0.0f,1.0f,0.0f},
                                     true,true,false) &&
                near(source_heading_gate.distance,kForwardAnchorIdleCap),
                "AnchorForward uses GameObject +0x1b5 heading availability instead of controller heading-active")) return 44;
    if (!expect(update_forward_anchor(&look_at_gate,{10.0f,21.0f,30.0f},
                                     {0.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},
                                     true,true,false) && near(look_at_gate.distance,337.0f) &&
                near(look_at_gate.target[0],10.0f) && near(look_at_gate.target[1],358.0f),
                "turn gate uses prior actor look-at and advances the active camera target")) return 35;
    ForwardAnchorState slow_walk{};
    if (!expect(update_forward_anchor(&slow_walk,{0.0f,0.0f,0.0f},
                                     {0.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},
                                     false,false,false),
                "idle AnchorForward captures the source displacement baseline")) return 41;
    if (!expect(update_forward_anchor(&slow_walk,{0.005f,0.0f,0.0f},
                                     {0.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},
                                     true,true,false) && near(slow_walk.distance,320.0f),
                "slow movement begins inside the camera expansion threshold")) return 42;
    bool slow_walk_ok = true;
    for (unsigned step = 2; step <= 50; ++step) {
        const float x = static_cast<float>(step) * 0.005f;
        slow_walk_ok = slow_walk_ok && update_forward_anchor(
            &slow_walk,{x,0.0f,0.0f},{0.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},
            true,true,false);
    }
    if (!expect(slow_walk_ok && near(slow_walk.distance,320.0f) &&
                near(slow_walk.target[1],320.0f) &&
                near(slow_walk.previous_actor_position[0],0.25f),
                "AnchorForward compares each movement frame with the immediately preceding position")) return 43;
    ForwardAnchorState look_at_turn{};
    look_at_turn.distance=320.0f;look_at_turn.previous_actor_look_at={1.0f,0.0f,0.0f};
    look_at_turn.previous_actor_position={0.0f,0.0f,0.0f};
    look_at_turn.target={0.0f,320.0f,0.0f};look_at_turn.has_target=true;
    if (!expect(update_forward_anchor(&look_at_turn,{0.0f,1.0f,0.0f},
                                     {0.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},
                                     true,true,false) && near(look_at_turn.distance,315.75f) &&
                near(look_at_turn.target[1],316.75f),
                "source turn retreats against stored look-at while preserving actor offset")) return 36;
    ForwardAnchorState look_at_threshold{};
    look_at_threshold.distance=320.0f;look_at_threshold.previous_actor_look_at={1.0f,0.0f,0.0f};
    look_at_threshold.target={0.0f,320.0f,0.0f};look_at_threshold.has_target=true;
    const Vec3 exactly_one_radian{std::cos(1.0f),std::sin(1.0f),0.0f};
    if (!expect(update_forward_anchor(&look_at_threshold,{0.0f,1.0f,0.0f},
                                     exactly_one_radian,{0.0f,1.0f,0.0f},
                                     true,true,false) && near(look_at_threshold.distance,337.0f),
                "source strict one-radian angle threshold does not retreat")) return 37;
    ForwardAnchorState stopped{};
    stopped.distance=320.0f;stopped.previous_actor_look_at={0.0f,1.0f,0.0f};
    stopped.previous_actor_position={0.0f,0.0f,0.0f};
    if (!expect(update_forward_anchor(&stopped,{2.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},
                                     {0.0f,1.0f,0.0f},false,false,false) &&
                near(stopped.distance,kForwardAnchorIdleCap) &&
                near(stopped.target[0],2.0f) && near(stopped.target[1],kForwardAnchorIdleCap),
                "Stop with zero source heading idles on the retained facing vector")) return 38;
    ForwardAnchorState attacking{};
    if (!expect(update_forward_anchor(&attacking,{1.0f,0.0f,0.0f},{1.0f,0.0f,0.0f},
                                     {0.0f,1.0f,0.0f},true,false,true) &&
                near(attacking.distance,kForwardAnchorMaxDistance*0.5f),
                "attack root motion never takes the moving-only full-distance branch")) return 39;
    if (!expect(near(forward_anchor_target(&stopped,{2.0f,0.0f,0.0f})[1],
                     kForwardAnchorIdleCap),
                "rendered camera consumes the retained source anchor target")) return 40;

    transform(frame.view_projection, frame.target, clip);
    if (!expect(std::fabs(clip[0] / clip[3]) < 1e-5f && std::fabs(clip[1] / clip[3]) < 1e-5f,
                "camera target projects to screen center")) return 9;
    if (!expect(clip[3] > 0.0f, "target lies in front of camera")) return 10;

    Vec3 forward = subtract(frame.target, frame.eye);
    if (!expect(normalize(forward), "source forward basis normalizes")) return 11;
    const Vec3 movement_forward{
        -std::cos(frame.input_yaw) * std::cos(frame.input_pitch),
        -std::sin(frame.input_yaw) * std::cos(frame.input_pitch),
        -std::sin(frame.input_pitch)};
    if (!expect(near(movement_forward[0], forward[0], 1e-6f) &&
                near(movement_forward[1], forward[1], 1e-6f) &&
                near(movement_forward[2], forward[2], 1e-6f),
                "controller look vector matches recovered source camera forward")) return 12;
    Vec3 side = cross(forward, {0.0f, 0.0f, 1.0f});
    if (!expect(normalize(side), "source side basis normalizes")) return 13;
    Vec3 vertical = cross(side, forward);
    // For the default eye offset (+X,-Y,+Z) and world up (+Z), the GLES-facing
    // up basis must point toward positive world Z after adapting the source
    // positive-forward matrix convention.
    const Vec3 right_point{frame.target[0] + 100.0f,
                           frame.target[1] + 100.0f,
                           frame.target[2]};
    transform(frame.view_projection, right_point, clip);
    if (!expect(clip[0] / clip[3] > 0.0f, "source camera side maps right on screen")) return 14;
    const Vec3 up_point{frame.target[0] + vertical[0] * 100.0f,
                        frame.target[1] + vertical[1] * 100.0f,
                        frame.target[2] + vertical[2] * 100.0f};
    transform(frame.view_projection, up_point, clip);
    if (!expect(clip[1] / clip[3] > 0.0f, "source camera vertical maps up on screen")) return 15;
    const Vec3 world_up_point{frame.target[0],frame.target[1],frame.target[2]+100.0f};
    transform(frame.view_projection,world_up_point,clip);
    if (!expect(clip[1] / clip[3] > 0.0f,
                "authored world up must project to positive GLES screen Y")) return 46;
    const Vec3 near_point{frame.eye[0] + forward[0] * kNearPlane,
                          frame.eye[1] + forward[1] * kNearPlane,
                          frame.eye[2] + forward[2] * kNearPlane};
    transform(frame.view_projection, near_point, clip);
    if (!expect(near(clip[2] / clip[3], -1.0f, 1e-5f), "source near plane maps to GLES -1")) return 16;
    const Vec3 far_point{frame.eye[0] + forward[0] * kFarPlane,
                         frame.eye[1] + forward[1] * kFarPlane,
                         frame.eye[2] + forward[2] * kFarPlane};
    transform(frame.view_projection, far_point, clip);
    if (!expect(near(clip[2] / clip[3], 1.0f, 1e-5f), "source far plane maps to GLES +1")) return 17;

    Frame swamp{};
    if (!expect(!build("SWAMP", "001_swamp.mlx", receipt_target, 2400, 1080, &swamp),
                "Crypt frame builder refuses SWAMP")) return 18;
    Frame portrait{};
    if (!expect(build("GOTHICUS_CRYPT_01", "007_crypt_01.rule.xml", receipt_target, 1080, 2400, &portrait),
                "build portrait Crypt frame")) return 19;
    if (!expect(portrait.aspect < frame.aspect, "portrait aspect changes projection")) return 20;

    std::printf("native_camera_crypt_frame_v1: %d assertions passed\n", checks);
}
