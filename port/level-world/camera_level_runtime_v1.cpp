#include "camera_level_runtime_v1.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

namespace dh2::camera_level_runtime_v1 {
namespace {
constexpr float kSqrtHalf=0.7071067811865475244f; // IDA literal 0x3f3504f3
Vec3 add(Vec3 a,Vec3 b) noexcept { return {a.x+b.x,a.y+b.y,a.z+b.z}; }
Vec3 sub(Vec3 a,Vec3 b) noexcept { return {a.x-b.x,a.y-b.y,a.z-b.z}; }
Vec3 scale(Vec3 a,float f) noexcept { return {a.x*f,a.y*f,a.z*f}; }
Vec3 lerp(Vec3 a,Vec3 b,float t) noexcept { return add(a,scale(sub(b,a),t)); }
float clamp(float x,float lo,float hi) noexcept { return x<lo?lo:(x>hi?hi:x); }
bool finite(Vec3 v) noexcept { return std::isfinite(v.x)&&std::isfinite(v.y)&&std::isfinite(v.z); }
}

bool resolve_camera_anchor(const GameObjectSample& object,Vec3* out) noexcept {
    if(!out||!object.present||!object.identity)return false;
    *out=object.custom_anchor_present?object.custom_anchor_plus_12:object.object_position;
    return finite(*out);
}

bool camera_base_get_center_offset(const CameraBaseOffsetInput& in,Vec3 camera_position,Vec3* out) noexcept {
    if(!out||!finite(in.camera_basis_column)||!finite(camera_position)||
       !std::isfinite(in.vertical_fov_radians)||!std::isfinite(in.target_z))return false;
    *out={in.camera_basis_column.x,in.camera_basis_column.y,0.0f};
    const float direction_length_sq=out->x*out->x+out->y*out->y;
    if(direction_length_sq<0.0001f)return true;

    const float basis_length=std::sqrt(in.camera_basis_column.x*in.camera_basis_column.x+
        in.camera_basis_column.y*in.camera_basis_column.y+
        in.camera_basis_column.z*in.camera_basis_column.z);
    if(basis_length==0.0f)return false;
    // Point3D::angle(camera basis, -Vec3f_K), with Vec3f_K=(0,0,1).
    const float cosine=-in.camera_basis_column.z/basis_length;
    const float theta=std::acos(clamp(cosine,-1.0f,1.0f));
    const float half_fov=in.vertical_fov_radians*0.5f;
    const float tan_theta=std::tan(theta);
    const float height_delta=camera_position.z-in.target_z;
    const float low_edge=(std::tan(theta-half_fov)-tan_theta)*height_delta;
    const float high_edge=(std::tan(theta+half_fov)-tan_theta)*height_delta;
    const float magnitude=((low_edge+high_edge)*0.5f)-low_edge;
    const float xy_length=std::sqrt(direction_length_sq);
    out->x=(out->x/xy_length)*magnitude;
    out->y=(out->y/xy_length)*magnitude;
    out->z=0.0f;
    return finite(*out);
}

bool Owner::set_target(const GameObjectSample* next,std::int32_t duration_ms,
                       const GameObjectSample* old_target_sample) noexcept {
    if(!next)return true; // source ignores null targets
    if(!next->present||!next->identity)return false;
    if(duration_ms<=0){
        transition_start_={};transition_duration_ms_=0;transition_remaining_ms_=0;
    }else{
        Vec3 old{};
        if(target_identity_){
            if(!old_target_sample||old_target_sample->identity!=target_identity_||
               !resolve_camera_anchor(*old_target_sample,&old))return false;
        }
        transition_start_=old;transition_duration_ms_=duration_ms;transition_remaining_ms_=duration_ms;
    }
    target_identity_=next->identity;
    ghost_camera_offset_={};
    return true;
}

void Owner::clear_target() noexcept { target_identity_=0; }
void Owner::enable_center_offset(bool enabled) noexcept { center_offset_enabled_=enabled; }
void Owner::enable_damping(bool enabled) noexcept { damping_enabled_=enabled;damping_velocity_={}; }
void Owner::set_damping_ratio(float ratio) noexcept { damping_ratio_=ratio; }
void Owner::set_ghost_camera_offset(Vec3 offset) noexcept { ghost_camera_offset_=offset; }
void Owner::play_animation_started(bool preserve_zoom) noexcept {
    zoom_animation_active_=true;
    preserve_zoom_=preserve_zoom;
    if(!preserve_zoom){current_zoom_=0.0f;target_zoom_=1.0f;}
}
void Owner::end_animation() noexcept { zoom_animation_active_=false; }
void Owner::set_zoom(float current,float target) noexcept { current_zoom_=current;target_zoom_=target; }
void Owner::set_zoom_animation_state(bool active,bool preserve_zoom) noexcept {
    zoom_animation_active_=active;preserve_zoom_=preserve_zoom;
}
void Owner::set_default_target_distance(float distance) noexcept { default_target_distance_=distance; }

bool ZoomInput::set_camera(Owner* owner,DrawableTargetSize target) noexcept {
    const auto count=std::max(target.width,target.height);
    if(!owner||count<=0)return false;
    owner_=owner;
    sensitivity_=1.0f/static_cast<float>(count);
    mouse_pan_enabled_=false;
    mouse_pan_active_=false;
    mouse_start_x_=mouse_start_y_=0;
    mouse_pan_baseline_={};
    return std::isfinite(sensitivity_)&&sensitivity_>0.0f;
}

bool ZoomInput::update_drawable_target(DrawableTargetSize target) noexcept {
    const auto count=std::max(target.width,target.height);
    if(!owner_||count<=0)return false;
    const float next=1.0f/static_cast<float>(count);
    if(!std::isfinite(next)||next<=0.0f)return false;
    sensitivity_=next;
    return true;
}

void ZoomInput::clear_camera() noexcept {
    owner_=nullptr;sensitivity_=0.0f;mouse_pan_enabled_=false;mouse_pan_active_=false;
    mouse_start_x_=mouse_start_y_=0;mouse_pan_baseline_={};
}

bool ZoomInput::mouse_wheel(float delta) noexcept {
    if(!owner_||!std::isfinite(delta)||!std::isfinite(sensitivity_))return false;
    const float current=owner_->current_zoom();
    const float next=current+(sensitivity_*5.0f)*delta;
    if(!std::isfinite(current)||!std::isfinite(next))return false;
    owner_->set_zoom(next,owner_->target_zoom());
    return true;
}

bool ZoomInput::begin_mouse_pan(std::int32_t x,std::int32_t y,Vec3 camera_offset,
                                bool menu_camera,bool inside_map_render_zone) noexcept {
    if(!owner_||!finite(camera_offset))return false;
    mouse_start_x_=x;mouse_start_y_=y;mouse_pan_baseline_=camera_offset;
    mouse_pan_active_=mouse_pan_enabled_&&(!menu_camera||inside_map_render_zone);
    return mouse_pan_active_;
}

bool ZoomInput::move_mouse_pan(std::int32_t x,std::int32_t y,Vec3* camera_offset) const noexcept {
    if(!owner_||!camera_offset||!mouse_pan_active_)return false;
    const float next_x=mouse_pan_baseline_.x+
        (static_cast<float>(mouse_start_x_)-static_cast<float>(x))*50.0f;
    const float next_y=mouse_pan_baseline_.y+
        (static_cast<float>(y)-static_cast<float>(mouse_start_y_))*50.0f;
    const Vec3 next{next_x,next_y,mouse_pan_baseline_.z};
    if(!finite(next))return false;
    *camera_offset=next;
    return true;
}

bool ZoomInput::end_mouse_pan() noexcept {
    if(!owner_)return false;
    mouse_pan_active_=false;
    return true;
}

bool ZoomInput::touch_pan(std::int32_t delta_x,std::int32_t delta_y,Vec3* camera_offset,
                          bool touch_pan_enabled) const noexcept {
    if(!owner_||!owner_->use_object_position()||!camera_offset||!touch_pan_enabled||
       !finite(*camera_offset))return false;
    const Vec3 next{camera_offset->x-static_cast<float>(delta_x)*50.0f,
                    camera_offset->y+static_cast<float>(delta_y)*50.0f,
                    camera_offset->z};
    if(!finite(next))return false;
    *camera_offset=next;
    return true;
}

bool ZoomInput::pinch_zoom(float previous_distance,float current_distance) noexcept {
    if(!owner_||!owner_->use_object_position()||!std::isfinite(previous_distance)||
       !std::isfinite(current_distance)||
       previous_distance<0.0f||current_distance<0.0f)return false;
    const float zoom=owner_->current_zoom();
    const float next=zoom+(current_distance-previous_distance)*sensitivity_;
    if(!std::isfinite(zoom)||!std::isfinite(next))return false;
    owner_->set_zoom(next,owner_->target_zoom());
    return true;
}

bool ZoomInput::native_pinch_zoom(float input_delta,bool dpad_enabled) noexcept {
    if(!owner_||!dpad_enabled||owner_->zoom_locked()||!std::isfinite(input_delta)||
       !std::isfinite(sensitivity_)||sensitivity_<=0.0f||
       static_cast<double>(input_delta)<std::numeric_limits<std::int32_t>::min()||
       static_cast<double>(input_delta)>std::numeric_limits<std::int32_t>::max())return false;
    // NativePinchZoom calls as_value::to_number, converts to int, then applies
    // `int * ZoomHandler(+40)` to CameraLevel(+136).
    const auto delta=static_cast<std::int32_t>(input_delta);
    const float current=owner_->current_zoom();
    const float next=current+static_cast<float>(delta)*sensitivity_;
    if(!std::isfinite(current)||!std::isfinite(next))return false;
    owner_->set_zoom(next,owner_->target_zoom());
    return true;
}

bool MapCameraSession::open(const GameObjectSample& player,DrawableTargetSize drawable,
                           float default_target_distance) noexcept {
    if(active_||!player.present||!player.identity||
       !std::isfinite(default_target_distance)||default_target_distance<0.0f)return false;
    camera_=Owner{};
    if(!camera_.set_target(&player,0,nullptr))return false;
    // The original map menu owns a distinct CameraLevel, starts at zoom 0/1,
    // disables damping, and sets +0x85 before binding ZoomHandler.
    camera_.set_use_object_position(true);
    camera_.enable_damping(false);
    camera_.set_default_target_distance(default_target_distance);
    camera_.set_zoom(0.0f,1.0f);
    if(!zoom_input_.set_camera(&camera_,drawable)){
        camera_.clear_target();
        camera_=Owner{};
        return false;
    }
    target_identity_=player.identity;
    pan_offset_={};
    active_=true;
    return true;
}

bool MapCameraSession::resize(DrawableTargetSize drawable) noexcept {
    return active_&&zoom_input_.update_drawable_target(drawable);
}

bool MapCameraSession::update(FrameInput input,FrameOutput* output) noexcept {
    if(!active_||!output||!input.target.present||
       input.target.identity!=target_identity_)return false;
    input.use_object_position=true;
    input.animated_target_cam_offset=pan_offset_;
    const auto status=camera_.update(input,output);
    return status==Status::transition_position_written||
           status==Status::follow_position_written;
}

bool MapCameraSession::pan(std::int32_t delta_x,std::int32_t delta_y,
                           bool source_touch_pan_enabled) noexcept {
    return active_&&zoom_input_.touch_pan(delta_x,delta_y,&pan_offset_,
                                          source_touch_pan_enabled);
}

bool MapCameraSession::pinch(float previous_distance,float current_distance) noexcept {
    return active_&&zoom_input_.pinch_zoom(previous_distance,current_distance);
}

bool MapCameraSession::reset_zoom() noexcept {
    if (!active_) return false;
    camera_.set_zoom(0.0f, 1.0f);
    return true;
}

void MapCameraSession::close() noexcept {
    // Source Hide unbinds ZoomHandler before destroying the map CameraLevel.
    zoom_input_.clear_camera();
    camera_.clear_target();
    camera_=Owner{};
    target_identity_=0;
    pan_offset_={};
    active_=false;
}

Status Owner::update(const FrameInput& in,FrameOutput* out) noexcept {
    if(!out||in.dt_ms<0||!std::isfinite(in.design_zoom_bounds.normal_min)||
       !std::isfinite(in.design_zoom_bounds.normal_max)||
       !std::isfinite(in.design_zoom_bounds.alternate_min)||
       !std::isfinite(in.design_zoom_bounds.alternate_max)||
       in.design_zoom_bounds.normal_min>in.design_zoom_bounds.normal_max||
       in.design_zoom_bounds.alternate_min>in.design_zoom_bounds.alternate_max||
       !std::isfinite(in.camera_world_position.x)||!std::isfinite(in.camera_world_position.y)||
       !std::isfinite(in.camera_world_position.z))return Status::invalid_input;
    use_object_position_=in.use_object_position;
    *out={};
    if(!in.active_camera||!in.camera_node_present||!in.target_cam_node_present||
       !in.target_game_object_present||!target_identity_||!in.target.present||
       in.target.identity!=target_identity_)return Status::no_camera_update;

    Vec3 anchor{};
    if(!resolve_camera_anchor(in.target,&anchor))return Status::invalid_input;

    // CameraTarget::HandleTransition runs before any CameraLevel offset, zoom,
    // ghost or damping work and returns early for the whole CameraLevel update.
    // Preserve its 0 ms equality edge: SetTarget(duration<=0) stores zero, so
    // repeated frames with dt==0 keep taking the source snap/early-return path.
    if(transition_remaining_ms_>=0){
        const auto remaining=static_cast<std::int64_t>(transition_remaining_ms_)-in.dt_ms;
        transition_remaining_ms_=remaining<std::numeric_limits<std::int32_t>::min()
            ?std::numeric_limits<std::int32_t>::min():static_cast<std::int32_t>(remaining);
        out->target_cam_local_position=target_cam_local_position_;
        if(remaining<=0){out->camera_world_position=anchor;return Status::transition_position_written;}
        const float t=1.0f-static_cast<float>(transition_remaining_ms_)/static_cast<float>(transition_duration_ms_);
        out->camera_world_position=lerp(transition_start_,anchor,t);
        return Status::transition_position_written;
    }

    Vec3 target=in.use_object_position?in.target.object_position:anchor;
    if(center_offset_enabled_){
        auto offset_input=in.center_offset_source;
        offset_input.target_z=target.z;
        Vec3 offset{};
        if(!camera_base_get_center_offset(offset_input,in.camera_world_position,&offset))return Status::invalid_input;
        target=add(target,offset);
    }
    target=add(target,in.multiplayer_centering_delta);
    Vec3 rig=in.animated_target_cam_offset;
    if(in.use_object_position){
        const float x=rig.x,y=rig.y;
        rig.x=kSqrtHalf*x-kSqrtHalf*y;
        rig.y=kSqrtHalf*y+kSqrtHalf*x;
    }
    target=add(target,rig);

    float signed_zoom=0.0f;
    if(!zoom_animation_active_||preserve_zoom_){
        if(in.infinite_zoom)effective_zoom_=current_zoom_;
        else if(!in.zoom_clamp_disabled&&!zoom_locked_){
            const float min_zoom=in.use_object_position?in.design_zoom_bounds.alternate_min:
                                                        in.design_zoom_bounds.normal_min;
            const float max_zoom=in.use_object_position?in.design_zoom_bounds.alternate_max:
                                                        in.design_zoom_bounds.normal_max;
            current_zoom_=clamp(current_zoom_,min_zoom,max_zoom);
            target_zoom_=clamp(target_zoom_,min_zoom,max_zoom);
            effective_zoom_=std::fmin(current_zoom_,target_zoom_);
        }
        signed_zoom=-effective_zoom_;
    }else if(std::fabs(current_zoom_)>=0.1f){
        current_zoom_*=0.75f;signed_zoom=-current_zoom_;
    }else{
        current_zoom_=0.0f;signed_zoom=-0.0f;
    }
    rig={0.0f,0.0f,signed_zoom*default_target_distance_};
    target_cam_local_position_=rig;
    out->target_cam_local_position=target_cam_local_position_;
    target=add(target,ghost_camera_offset_);
    out->target_before_damping=target;

    if(damping_enabled_){
        damping_velocity_=scale(sub(add(target,damping_velocity_),in.camera_world_position),damping_ratio_);
        target=add(in.camera_world_position,scale(damping_velocity_,static_cast<float>(in.dt_ms)*0.001f));
    }
    out->camera_world_position=target;
    out->target_was_retired=in.target.clear_camera_target_after_update;
    if(out->target_was_retired)clear_target();
    return Status::follow_position_written;
}

} // namespace dh2::camera_level_runtime_v1
