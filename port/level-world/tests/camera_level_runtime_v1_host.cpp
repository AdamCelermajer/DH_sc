#include "../camera_level_runtime_v1.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <limits>

using namespace dh2::camera_level_runtime_v1;

namespace {
int assertions=0;
void check(bool condition,const char* label){++assertions;if(!condition){std::fprintf(stderr,"FAIL: %s\n",label);std::exit(1);}}
bool near(float a,float b){return std::fabs(a-b)<0.0002f;}
bool near(Vec3 a,Vec3 b){return near(a.x,b.x)&&near(a.y,b.y)&&near(a.z,b.z);}
FrameInput frame(const GameObjectSample& target){
    FrameInput in{};in.active_camera=true;in.camera_node_present=true;in.target_cam_node_present=true;
    in.target_game_object_present=true;in.target=target;
    in.design_zoom_bounds={0,0,2,0,2};return in;
}
}

int main(){
    check(can_play_shake_anim({false,true,4,false}),
          "source shake gate keeps release return true for a null requester");
    check(can_play_shake_anim({true,false,4,false}),
          "source shake gate allows non-Player Character requesters without local-player checks");
    check(can_play_shake_anim({true,true,1,true})&&
          !can_play_shake_anim({true,true,1,false})&&
          !can_play_shake_anim({true,true,2,true}),
          "source shake gate permits only the sole local Player character");

    CameraOverviewOwner overview;
    check(overview.construct({1280,720})&&overview.live()&&!overview.active(),
          "Level camera construction retains a distinct inactive overview owner");
    const auto overview_identity=overview.projection_identity();
    const auto* overview_projection=overview.projection();
    check(overview_identity!=0&&overview_projection&&
          reinterpret_cast<std::uintptr_t>(overview_projection)==overview_identity&&
          overview_projection->scene_node_id==1601003875&&
          overview_projection->drawable.width==1280&&overview_projection->drawable.height==720&&
          near(overview_projection->vertical_fov_radians,0.42963001132011414f)&&
          near(overview_projection->authored_aspect_ratio,1.7778f)&&
          near(overview_projection->near_clip,15000.0f)&&
          near(overview_projection->far_clip,25000.0f)&&
          near(overview_projection->position,{0,0,20000})&&
          near(overview_projection->up,{0,1,0}),
          "CameraOverview owns its source constructor camera node/projection parameters");
    check(overview.resize({2400,1080})&&
          overview.projection()->drawable.width==2400&&
          near(overview.projection()->authored_aspect_ratio,1.7778f),
          "overview retains the live renderer viewport without rewriting source SetData aspect");
    const GameObjectSample overview_player{77,true,{12,34,56},false,{},false};
    check(overview.activate(overview_player,400.0f)&&overview.active()&&
          near(overview.projection()->target,{12,34,400})&&
          near(overview.projection()->position,{12,34,20400})&&
          near(overview.projection()->near_clip,15400)&&
          near(overview.projection()->far_clip,25400),
          "CameraOverview activation centers on the local Player and PFWorld midpoint");
    check(!overview.destroy(overview_identity)&&overview.deactivate()&&
          !overview.active()&&!overview.destroy(overview_identity+1)&&
          overview.destroy(overview_identity)&&!overview.live()&&
          overview.projection_identity()==0,
          "overview must deactivate and match its actual retained projection before Level unload destroys it");

    GameObjectSample a{1,true,{2,3,4},true,{10,20,30},false},b{2,true,{50,60,70},false,{},false};
    Owner initialized_owner;
    check(near(initialized_owner.current_zoom(),0.0f)&&near(initialized_owner.target_zoom(),1.0f),
          "ZoomHandler constructor preserves exact current=0 and target=1 initialization");
    Vec3 resolved{};
    check(resolve_camera_anchor(a,&resolved)&&near(resolved,{10,20,30}),"custom +736 anchor resolves to pointer +12");
    a.custom_anchor_present=false;
    check(resolve_camera_anchor(a,&resolved)&&near(resolved,{2,3,4}),"missing custom anchor falls back to +352 world position");
    check(!resolve_camera_anchor({},&resolved),"missing GameObject cannot fabricate an anchor");

    Owner owner;owner.enable_damping(false);
    check(owner.set_target(&a,0,nullptr),"SetTarget accepts actual target");
    auto in=frame(a);in.dt_ms=16;FrameOutput out{};
    check(owner.update(in,&out)==Status::transition_position_written&&near(out.camera_world_position,{2,3,4}),
          "zero-duration SetTarget still consumes the source transition snap frame");
    check(near(out.target_cam_local_position,{}),
          "initial transition retains the target-camera node's startup zero zoom");
    in.dt_ms=16;
    check(owner.update(in,&out)==Status::follow_position_written,"expired transition returns to normal follow update");

    Owner zero_dt_owner;zero_dt_owner.enable_damping(false);
    check(zero_dt_owner.set_target(&a,0,nullptr),"zero-delta fixture target set");
    auto zero_dt=frame(a);FrameOutput zero_dt_out{};
    check(zero_dt_owner.update(zero_dt,&zero_dt_out)==Status::transition_position_written&&
          zero_dt_owner.transition_remaining_ms()==0,"source equality branch snaps at dt=0");
    check(zero_dt_owner.update(zero_dt,&zero_dt_out)==Status::transition_position_written&&
          zero_dt_owner.transition_remaining_ms()==0,"source repeats zero-duration snap while dt remains 0");
    zero_dt.dt_ms=16;
    check(zero_dt_owner.update(zero_dt,&zero_dt_out)==Status::transition_position_written&&
          zero_dt_owner.transition_remaining_ms()<0,"first positive dt exits immediate-target snap state");
    zero_dt.dt_ms=16;
    check(zero_dt_owner.update(zero_dt,&zero_dt_out)==Status::follow_position_written,
          "regular follow starts on the frame after the source snap");

    GameObjectSample old{1,true,{2,3,4},true,{10,20,30},false};
    check(owner.set_target(&b,100,&old),"positive SetTarget captures old source anchor");
    in=frame(b);in.dt_ms=25;
    check(owner.update(in,&out)==Status::transition_position_written&&near(out.camera_world_position,{20,30,40}),
          "transition interpolates captured anchor to current new anchor at 25 percent");
    in.dt_ms=75;
    check(owner.update(in,&out)==Status::transition_position_written&&near(out.camera_world_position,{50,60,70}),
          "transition endpoint snaps to current target and short-circuits follow");
    in=frame(b);in.dt_ms=16;
    check(owner.update(in,&out)==Status::transition_position_written&&owner.transition_remaining_ms()<0,
          "zero remaining transition expires before the regular camera frame");

    check(owner.set_target(nullptr,500,nullptr)&&owner.target_identity()==2,"null SetTarget is source no-op");
    owner.enable_center_offset(true);owner.set_ghost_camera_offset({1,2,3});
    in=frame(b);in.dt_ms=16;in.camera_world_position={0,0,71};
    in.center_offset_source.camera_basis_column={0.8f,0,-0.6f};
    in.center_offset_source.vertical_fov_radians=1.0f;
    in.multiplayer_centering_delta={100,200,300};in.animated_target_cam_offset={4,8,12};
    check(owner.update(in,&out)==Status::follow_position_written&&near(out.target_before_damping,{158.23267f,270,385}),
          "follow runs source CameraBase FOV offset, multiplayer centering, animated rig, then ghost");

    Vec3 center_offset{};
    check(camera_base_get_center_offset(
              CameraBaseOffsetInput{{0.009f,-0.004f,0.999f},0.0f,123.0f},
              {0.0f,0.0f,9999.0f},&center_offset)&&
          near(center_offset,{0.009f,-0.004f,0.0f}),
          "near-vertical CameraBase basis takes source low-XY branch before FOV/height geometry");

    owner.set_ghost_camera_offset({});in=frame(b);in.dt_ms=16;in.use_object_position=true;
    in.animated_target_cam_offset={1,0,0};
    check(owner.update(in,&out)==Status::follow_position_written&&
          near(out.target_before_damping,{50.7071067f,60.7071067f,70}),
          "direct-position camera mode rotates authored XY offset by source 45-degree constants");

    owner.enable_center_offset(false);owner.set_ghost_camera_offset({});owner.set_default_target_distance(10);
    owner.set_zoom(0.8f,0.6f);owner.end_animation();in=frame(b);in.dt_ms=16;
    check(owner.update(in,&out)==Status::follow_position_written&&near(owner.current_zoom(),0.8f)&&
          near(owner.effective_zoom(),0.6f)&&near(out.target_cam_local_position,{0,0,-6}),
          "design zoom provider clamps current/target then uses their minimum times loaded camera distance");
    const Vec3 prior_target_cam_position=out.target_cam_local_position;
    const GameObjectSample previous_target=b;
    check(owner.set_target(&a,100,&previous_target),"positive retarget starts source camera transition");
    in=frame(a);in.dt_ms=25;
    check(owner.update(in,&out)==Status::transition_position_written&&
          near(out.target_cam_local_position,prior_target_cam_position),
          "transition frame preserves previously written target-camera zoom");
    in.dt_ms=75;
    check(owner.update(in,&out)==Status::transition_position_written&&
          near(out.target_cam_local_position,prior_target_cam_position),
          "transition endpoint preserves target-camera zoom until follow writes it again");
    in=frame(a);in.dt_ms=16;
    check(owner.update(in,&out)==Status::transition_position_written&&
          near(out.target_cam_local_position,prior_target_cam_position),
          "first frame after transition endpoint preserves target-camera zoom and snaps to source target");
    in.dt_ms=16;
    check(owner.update(in,&out)==Status::follow_position_written&&
          near(out.target_cam_local_position,prior_target_cam_position),
          "follow resumes after the source transition snap frame");
    check(owner.set_target(&b,0,nullptr),"zero-duration retarget restores the original actor");
    in=frame(b);in.dt_ms=16;
    check(owner.update(in,&out)==Status::transition_position_written&&
          near(out.target_cam_local_position,prior_target_cam_position),
          "immediate retarget also retains the last target-camera local position");
    in=frame(b);in.dt_ms=16;
    check(owner.update(in,&out)==Status::follow_position_written,
          "follow resumes after zoom-retaining immediate transition");
    in.infinite_zoom=true;
    check(owner.update(in,&out)==Status::follow_position_written&&near(owner.effective_zoom(),0.8f)&&
          near(out.target_cam_local_position,{0,0,-8}),"InfiniteZoom selects unclamped current zoom");
    in.infinite_zoom=false;in.zoom_clamp_disabled=true;owner.set_zoom(0.4f,0.2f);
    check(owner.update(in,&out)==Status::follow_position_written&&near(owner.effective_zoom(),0.8f),
          "source zoom-clamp-disabled path preserves previous effective zoom");
    in.zoom_clamp_disabled=false;in.design_zoom_bounds.normal_max=0.35f;
    owner.set_zoom(0.8f,0.2f);owner.set_zoom_locked(true);
    check(owner.update(in,&out)==Status::follow_position_written&&near(owner.effective_zoom(),0.8f),
          "NativeLockZoom +134 persists on CameraLevel and skips HandleZoom clamping");
    owner.set_zoom_locked(false);
    check(owner.update(in,&out)==Status::follow_position_written&&near(owner.current_zoom(),0.35f)&&
          near(owner.effective_zoom(),0.2f),
          "NativeUnlockZoom clears +134 and resumes the configured zoom clamp");

    owner.set_zoom(0.8f,0.8f);owner.set_zoom_animation_state(true,false);in.zoom_clamp_disabled=false;
    check(owner.update(in,&out)==Status::follow_position_written&&near(owner.current_zoom(),0.6f)&&
          near(out.target_cam_local_position.z,-6),"active non-preserving animation decays zoom by 0.75");
    owner.set_zoom(0.05f,0.8f);
    check(owner.update(in,&out)==Status::follow_position_written&&near(owner.current_zoom(),0)&&
          near(out.target_cam_local_position.z,0),"animation zoom tail snaps below source 0.1 threshold");

    owner.set_zoom_animation_state(false,false);owner.set_ghost_camera_offset({});owner.enable_damping(true);owner.set_damping_ratio(0.7f);
    in=frame(b);in.dt_ms=100;in.camera_world_position={10,10,10};
    check(owner.update(in,&out)==Status::follow_position_written&&near(owner.damping_velocity(),{28,35,42})&&
          near(out.camera_world_position,{12.8f,13.5f,14.2f}),
          "damping uses (target + stored velocity - camera position) * ratio then dt seconds");
    owner.set_ghost_camera_offset({9,9,9});
    check(owner.set_target(&b,0,nullptr),"retarget to current actor resets ghost camera offset");
    check(near(owner.damping_velocity(),{28,35,42}),"SetTarget preserves damping velocity");
    owner.enable_damping(false);check(near(owner.damping_velocity(),{}),"EnableDamping resets all velocity components");

    Owner cache_bounds_owner;cache_bounds_owner.enable_damping(false);
    check(cache_bounds_owner.set_target(&b,0,nullptr),"cache bounds owner target set");
    auto cache_bounds_frame=frame(b);cache_bounds_frame.dt_ms=16;
    cache_bounds_frame.design_zoom_bounds={0,0.0f,0.35f,-1.5f,0.5f};
    cache_bounds_owner.set_zoom(0.8f,0.6f);
    check(cache_bounds_owner.update(cache_bounds_frame,&out)==Status::transition_position_written,
          "cache bounds owner consumes source transition before zoom");
    cache_bounds_frame.dt_ms=16;
    check(cache_bounds_owner.update(cache_bounds_frame,&out)==Status::follow_position_written&&
          near(cache_bounds_owner.current_zoom(),0.35f)&&near(cache_bounds_owner.target_zoom(),0.35f),
          "normal gameplay zoom clamps from projected DesignSettings bounds");
    cache_bounds_frame.use_object_position=true;
    cache_bounds_owner.set_zoom(0.8f,0.6f);
    check(cache_bounds_owner.update(cache_bounds_frame,&out)==Status::follow_position_written&&
          near(cache_bounds_owner.current_zoom(),0.5f)&&near(cache_bounds_owner.target_zoom(),0.5f),
          "alternate camera mode uses the same row's minimap zoom bounds");

    in=frame(b);in.dt_ms=16;
    check(owner.update(in,&out)==Status::transition_position_written,
          "immediate retarget consumes its source snap frame");
    in=frame(b);in.dt_ms=16;in.target.clear_camera_target_after_update=true;
    check(owner.update(in,&out)==Status::follow_position_written&&out.target_was_retired&&owner.target_identity()==0,
          "GameObject +129 retires target after completing follow update");
    in.target.identity=999;
    check(owner.update(in,&out)==Status::no_camera_update,"missing canonical target owner cannot update camera");

    Owner input_owner;input_owner.set_zoom(0.25f,0.8f);
    check(input_owner.set_target(&b,0,nullptr),"gesture fixture binds the canonical camera target");
    auto crypt_camera_frame=frame(b);crypt_camera_frame.dt_ms=16;
    check(!crypt_camera_frame.use_object_position&&
          input_owner.update(crypt_camera_frame,&out)==Status::transition_position_written&&
          !input_owner.use_object_position(),
          "active Crypt camera frame preserves the source constructor mode byte at false");
    ZoomInput input;
    check(input.set_camera(&input_owner,{4,8})&&near(input.sensitivity(),0.125f),
          "ZoomHandler setCamera uses reciprocal of larger difficulty count");
    check(input.mouse_wheel(2.0f)&&near(input_owner.current_zoom(),1.5f)&&
          near(input_owner.target_zoom(),0.8f),
          "mouse wheel changes current zoom by delta times five and sensitivity only");
    Vec3 touch_offset{100,200,300};
    const float crypt_zoom_before_gesture=input_owner.current_zoom();
    check(!input.touch_pan(2,-3,&touch_offset,true)&&near(touch_offset,{100,200,300}),
          "source CameraLevel mode false rejects touch pan without changing camera offset");
    check(!input.pinch_zoom(100.0f,116.0f)&&
          near(input_owner.current_zoom(),crypt_zoom_before_gesture),
          "source CameraLevel mode false rejects pinch without changing zoom");
    check(!input.native_pinch_zoom(16.0f,false)&&
          near(input_owner.current_zoom(),crypt_zoom_before_gesture),
          "authored NativePinchZoom is inert unless the saved DPad option is enabled");
    check(input.native_pinch_zoom(16.9f,true)&&
          near(input_owner.current_zoom(),crypt_zoom_before_gesture+2.0f)&&
          near(input_owner.target_zoom(),0.8f),
          "authored NativePinchZoom truncates its numeric input and applies drawable sensitivity to CameraLevel +136");
    const float native_zoom_before_lock=input_owner.current_zoom();
    input_owner.set_zoom_locked(true);
    check(input_owner.zoom_locked()&&!input.native_pinch_zoom(16.0f,true)&&
          near(input_owner.current_zoom(),native_zoom_before_lock),
          "NativeSetLockZoom +134 rejects authored zoom input without changing current zoom");
    input_owner.set_zoom_locked(false);
    check(!input_owner.zoom_locked()&&input.native_pinch_zoom(16.0f,true),
          "NativeUnlockZoom clears +134 and re-enables the authored zoom input");
    input_owner.set_zoom(crypt_zoom_before_gesture,0.8f);
    crypt_camera_frame.use_object_position=true;
    check(input_owner.update(crypt_camera_frame,&out)==Status::follow_position_written&&
          input_owner.use_object_position(),
          "gesture gate follows only the explicit CameraLevel source frame mode");
    check(!input.begin_mouse_pan(10,20,{100,200,300},false,false),
          "mouse drag remains disabled until the source enable flag is provided");
    input.set_mouse_pan_enabled(true);
    check(!input.begin_mouse_pan(10,20,{100,200,300},true,false)&&
          !input.mouse_pan_active(),"menu camera rejects drag start outside map render zone");
    check(input.begin_mouse_pan(10,20,{100,200,300},true,true),
          "menu camera accepts drag start inside map render zone");
    Vec3 mouse_offset{};
    check(input.move_mouse_pan(12,17,&mouse_offset)&&near(mouse_offset,{0,50,300}),
          "mouse drag uses captured start offset and source 50-units-per-pixel signs");
    check(input.end_mouse_pan()&&!input.mouse_pan_active(),"mouse-up clears pan gesture state");
    check(input.touch_pan(2,-3,&touch_offset,true)&&near(touch_offset,{0,50,300}),
          "single-touch pan applies source incremental x/y signs at 50 units per pixel");
    check(!input.touch_pan(1,1,&touch_offset,false),
          "single-touch pan requires the source camera touch-pan flag");
    check(input.pinch_zoom(100.0f,116.0f)&&near(input_owner.current_zoom(),3.5f)&&
          near(input_owner.target_zoom(),0.8f),
          "two-pointer pinch applies drawable-pixel distance delta times sensitivity to current zoom");
    const float before_resize=input_owner.current_zoom();
    check(input.update_drawable_target({1080,2400})&&near(input.sensitivity(),1.0f/2400.0f)&&
          near(input_owner.current_zoom(),before_resize),
          "surface resize refreshes inverse drawable target size without changing camera zoom");
    check(input.pinch_zoom(100.0f,116.0f)&&near(input_owner.current_zoom(),before_resize+16.0f/2400.0f),
          "pinch after resize uses the active drawable target while SWF stage remains independent");
    const float resized_sensitivity=input.sensitivity();
    check(!input.update_drawable_target({0,0})&&near(input.sensitivity(),resized_sensitivity),
          "invalid surface size leaves current ZoomHandler sensitivity unchanged");
    const float input_zoom=input_owner.current_zoom();
    check(!input.pinch_zoom(std::numeric_limits<float>::quiet_NaN(),120.0f)&&
          near(input_owner.current_zoom(),input_zoom),
          "invalid pinch distances leave camera zoom unchanged");
    check(!input.set_camera(&input_owner,{0,0})&&input_owner.current_zoom()==input_zoom,
          "invalid difficulty counts are rejected without mutating zoom");

    Owner gameplay_camera;gameplay_camera.enable_damping(false);
    gameplay_camera.set_zoom(0.3f,0.4f);
    check(gameplay_camera.set_target(&b,0,nullptr),"map session fixture retains live Player identity");
    const auto gameplay_zoom_before=gameplay_camera.current_zoom();
    MapCameraSession map;
    check(map.open(b,{1000,1600})&&map.active()&&map.target_identity()==b.identity&&
          map.camera_owner().use_object_position()&&near(map.camera_owner().current_zoom(),0.0f)&&
          near(map.camera_owner().target_zoom(),1.0f),
          "interactive map opens a distinct object-position CameraLevel with source zoom defaults");
    check(map.pan(2,-3,true)&&near(map.pan_offset(),{-100,-150,0}),
          "map menu pan mutates only its bound CameraLevel offsets");
    check(map.pinch(100.0f,260.0f)&&near(map.camera_owner().current_zoom(),0.1f),
          "map menu pinch binds ZoomHandler to the separate map camera and drawable sensitivity");
    const auto map_pan_before_reset=map.pan_offset();
    map.camera_owner().set_zoom(0.4f,0.6f);
    check(map.reset_zoom()&&near(map.camera_owner().current_zoom(),0.0f)&&
          near(map.camera_owner().target_zoom(),1.0f)&&
          near(map.pan_offset(),map_pan_before_reset)&&
          near(gameplay_camera.current_zoom(),gameplay_zoom_before),
          "authored ResetMapZoom restores only the active map CameraLevel current/target zoom");
    auto map_frame=frame(b);map_frame.dt_ms=16;
    FrameOutput map_out{};
    check(map.update(map_frame,&map_out)&&map.update(map_frame,&map_out)&&
          near(gameplay_camera.current_zoom(),gameplay_zoom_before),
          "map frame updates require the exact Player target while preserving gameplay camera zoom");
    auto wrong_map_frame=frame(a);
    check(!map.update(wrong_map_frame,&map_out),
          "map camera rejects a frame from any target other than its retained Player");
    map.close();
    check(!map.active()&&map.target_identity()==0&&!map.reset_zoom()&&!map.pinch(0,100)&&
          !map.pan(1,1,true)&&near(gameplay_camera.current_zoom(),gameplay_zoom_before),
          "map teardown unbinds gestures and retires only its own camera owner");
    std::printf("PASS: CameraLevel source-backed host checks (%d assertions)\n",assertions);
}
