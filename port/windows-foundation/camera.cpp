#include "camera.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>

namespace dh::foundation {
namespace {
constexpr float pi = 3.14159265358979323846f;
constexpr float epsilon = 1.0e-6f;
CameraVec3 add(CameraVec3 a, CameraVec3 b) { return {a.x+b.x, a.y+b.y, a.z+b.z}; }
CameraVec3 sub(CameraVec3 a, CameraVec3 b) { return {a.x-b.x, a.y-b.y, a.z-b.z}; }
CameraVec3 mul(CameraVec3 a, float s) { return {a.x*s, a.y*s, a.z*s}; }
float dot(CameraVec3 a, CameraVec3 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }
CameraVec3 cross(CameraVec3 a, CameraVec3 b) {
    return {a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x};
}
CameraVec3 normalized(CameraVec3 v, CameraVec3 fallback) {
    const float length = std::sqrt(dot(v,v));
    return length > epsilon ? mul(v,1.0f/length) : fallback;
}
bool finite(CameraVec3 v) { return std::isfinite(v.x)&&std::isfinite(v.y)&&std::isfinite(v.z); }
void validate(const CameraPose& p) {
    if (!finite(p.position)||!finite(p.target)||!finite(p.up)||
        !std::isfinite(p.verticalFovDegrees)||p.verticalFovDegrees <= 0.0f||
        p.verticalFovDegrees >= 180.0f) {
        throw std::invalid_argument("Camera pose must be finite with FOV between 0 and 180 degrees");
    }
}
struct Basis { CameraVec3 forward, right, up; };
Basis basis(const CameraPose& p) {
    const auto forward = normalized(sub(p.target,p.position), {0,0,-1});
    auto up = normalized(p.up,{0,1,0});
    if (std::abs(dot(forward,up)) > 0.999f)
        up = std::abs(forward.y) < 0.9f ? CameraVec3{0,1,0} : CameraVec3{0,0,1};
    const auto right = normalized(cross(forward,up), {1,0,0});
    return {forward,right,cross(right,forward)};
}
CameraVec3 lerp(CameraVec3 a, CameraVec3 b, float t) { return add(mul(a,1-t),mul(b,t)); }
CameraVec3 rotateAxis(CameraVec3 v, CameraVec3 axis, float angle) {
    const float c = std::cos(angle), s = std::sin(angle);
    return add(add(mul(v,c),mul(cross(axis,v),s)),mul(axis,dot(axis,v)*(1-c)));
}
}

std::array<float,16> cameraViewMatrix(const CameraPose& pose) {
    validate(pose);
    const auto b = basis(pose);
    return {b.right.x,b.up.x,-b.forward.x,0,
            b.right.y,b.up.y,-b.forward.y,0,
            b.right.z,b.up.z,-b.forward.z,0,
            -dot(b.right,pose.position),-dot(b.up,pose.position),dot(b.forward,pose.position),1};
}

std::array<float,16> cameraProjectionMatrix(float fov, float aspect, float nearPlane, float farPlane) {
    if (!std::isfinite(fov)||!std::isfinite(aspect)||!std::isfinite(nearPlane)||
        !std::isfinite(farPlane)||fov <= 0||fov >= 180||aspect <= 0||
        nearPlane <= 0||farPlane <= nearPlane)
        throw std::invalid_argument("Invalid camera perspective parameters");
    const float f = 1.0f/std::tan(fov*pi/360.0f);
    return {f/aspect,0,0,0, 0,f,0,0, 0,0,(farPlane+nearPlane)/(nearPlane-farPlane),-1,
            0,0,(2*farPlane*nearPlane)/(nearPlane-farPlane),0};
}

FreeCamera::FreeCamera(CameraPose pose) { setPose(pose); }
void FreeCamera::setPose(CameraPose pose) {
    validate(pose);
    if (dot(sub(pose.target,pose.position),sub(pose.target,pose.position)) < epsilon*epsilon)
        pose.target = add(pose.position,{0,0,-1});
    pose.up = normalized(pose.up,{0,1,0});
    pose_ = pose;
}
void FreeCamera::move(float right, float up, float forward) {
    if (!std::isfinite(right)||!std::isfinite(up)||!std::isfinite(forward))
        throw std::invalid_argument("Camera movement must be finite");
    const auto b = basis(pose_);
    const auto offset = add(add(mul(b.right,right),mul(b.up,up)),mul(b.forward,forward));
    pose_.position = add(pose_.position,offset);
    pose_.target = add(pose_.target,offset);
}
void FreeCamera::rotate(float yaw, float pitch) {
    if (!std::isfinite(yaw)||!std::isfinite(pitch))
        throw std::invalid_argument("Camera rotation must be finite");
    const auto worldUp = normalized(pose_.up,{0,1,0});
    const auto offset = sub(pose_.target,pose_.position);
    const float distance = std::sqrt(dot(offset,offset));
    auto direction = normalized(offset,{0,0,-1});
    direction = rotateAxis(direction,worldUp,-std::remainder(yaw,360.0f)*pi/180.0f);
    const auto right = normalized(cross(direction,worldUp),basis(pose_).right);
    const float elevation = std::asin(std::clamp(dot(direction,worldUp),-1.0f,1.0f));
    const float limit = 89.0f*pi/180.0f;
    const float wanted = std::clamp(elevation+pitch*pi/180.0f,-limit,limit);
    direction = rotateAxis(direction,right,wanted-elevation);
    pose_.target = add(pose_.position,mul(direction,distance > epsilon ? distance : 1.0f));
}

CameraTimeline::CameraTimeline(std::vector<CameraKeyframe> keys) { setKeyframes(std::move(keys)); }
void CameraTimeline::setKeyframes(std::vector<CameraKeyframe> keys) {
    for (const auto& key : keys) {
        if (!std::isfinite(key.timeSeconds)||key.timeSeconds < 0)
            throw std::invalid_argument("Camera keyframe time must be finite and nonnegative");
        validate(key.pose);
    }
    std::sort(keys.begin(),keys.end(),[](const auto& a,const auto& b){return a.timeSeconds<b.timeSeconds;});
    for (std::size_t i=1;i<keys.size();++i)
        if (keys[i-1].timeSeconds == keys[i].timeSeconds)
            throw std::invalid_argument("Camera keyframe times must be unique");
    keyframes_ = std::move(keys);
    reset();
}
void CameraTimeline::play() noexcept { playing_ = !finished(); }
void CameraTimeline::reset() noexcept { elapsed_ = 0.0; playing_ = false; }
void CameraTimeline::update(double dt) {
    if (!std::isfinite(dt)||dt < 0)
        throw std::invalid_argument("Camera delta time must be finite and nonnegative");
    if (!playing_) return;
    const double remaining = duration()-elapsed_;
    elapsed_ = dt >= remaining ? duration() : elapsed_+dt;
    if (finished()) playing_ = false;
}
CameraPose CameraTimeline::sample(double time) const {
    if (!std::isfinite(time)) throw std::invalid_argument("Camera sample time must be finite");
    if (keyframes_.empty()) return {};
    if (time <= keyframes_.front().timeSeconds) return keyframes_.front().pose;
    if (time >= duration()) return keyframes_.back().pose;
    const auto next = std::upper_bound(keyframes_.begin(),keyframes_.end(),time,
        [](double t,const CameraKeyframe& key){return t<key.timeSeconds;});
    const auto& before = *(next-1);
    if (next->cut) return before.pose;
    const float t = static_cast<float>((time-before.timeSeconds)/(next->timeSeconds-before.timeSeconds));
    return {lerp(before.pose.position,next->pose.position,t),
            lerp(before.pose.target,next->pose.target,t),
            normalized(lerp(before.pose.up,next->pose.up,t),before.pose.up),
            before.pose.verticalFovDegrees+(next->pose.verticalFovDegrees-before.pose.verticalFovDegrees)*t};
}
bool CameraTimeline::finished() const noexcept { return keyframes_.empty()||elapsed_ >= duration(); }
double CameraTimeline::duration() const noexcept { return keyframes_.empty() ? 0.0 : keyframes_.back().timeSeconds; }

} // namespace dh::foundation
