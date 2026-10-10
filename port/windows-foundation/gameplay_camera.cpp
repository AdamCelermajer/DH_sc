#include "gameplay_camera.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace dh::foundation {
namespace {
constexpr float pi = 3.14159265358979323846f;
constexpr float epsilon = 1.0e-6f;
CameraVec3 add(CameraVec3 a,CameraVec3 b){return {a.x+b.x,a.y+b.y,a.z+b.z};}
CameraVec3 sub(CameraVec3 a,CameraVec3 b){return {a.x-b.x,a.y-b.y,a.z-b.z};}
CameraVec3 mul(CameraVec3 a,float s){return {a.x*s,a.y*s,a.z*s};}
float dot(CameraVec3 a,CameraVec3 b){return a.x*b.x+a.y*b.y+a.z*b.z;}
CameraVec3 cross(CameraVec3 a,CameraVec3 b){return {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};}
bool finite(CameraVec3 a){return std::isfinite(a.x)&&std::isfinite(a.y)&&std::isfinite(a.z);}
CameraVec3 normalized(CameraVec3 a,CameraVec3 fallback){const float l=std::sqrt(dot(a,a));return l>epsilon?mul(a,1/l):fallback;}
CameraVec3 projected(CameraVec3 a,CameraVec3 up){return sub(a,mul(up,dot(a,up)));}
CameraVec3 referenceForward(CameraVec3 up){
    return normalized(projected(std::abs(up.y)<0.9f?CameraVec3{0,1,0}:CameraVec3{0,0,-1},up),{0,1,0});
}
CameraVec3 lerp(CameraVec3 a,CameraVec3 b,float t){return add(mul(a,1-t),mul(b,t));}
void validateActor(const CameraActorTarget& a){
    if(!finite(a.position)||!finite(a.facing))throw std::invalid_argument("Camera actor position/facing must be finite");
}
}

CameraMovementBasis cameraMovementBasis(const CameraPose& pose,CameraVec3 worldUp){
    if(!finite(pose.position)||!finite(pose.target)||!finite(worldUp)||dot(worldUp,worldUp)<epsilon*epsilon)
        throw std::invalid_argument("Camera movement basis requires finite vectors and a nonzero world up");
    const auto up=normalized(worldUp,{0,0,1});
    const auto forward=normalized(projected(sub(pose.target,pose.position),up),referenceForward(up));
    return {normalized(cross(forward,up),{1,0,0}),forward,up};
}

CameraFollow::CameraFollow(CameraFollowConfig config){configure(config);}
void CameraFollow::configure(CameraFollowConfig config){
    if(!std::isfinite(config.distance)||config.distance<=epsilon||
       !std::isfinite(config.pitchDegrees)||config.pitchDegrees<=-89.9f||config.pitchDegrees>=89.9f||
       !std::isfinite(config.headingDegrees)||!finite(config.targetOffset)||
       !std::isfinite(config.smoothingRate)||config.smoothingRate<0||
       !std::isfinite(config.verticalFovDegrees)||config.verticalFovDegrees<=0||config.verticalFovDegrees>=180||
       !finite(config.worldUp)||dot(config.worldUp,config.worldUp)<epsilon*epsilon)
        throw std::invalid_argument("Invalid gameplay camera follow configuration");
    config.worldUp=normalized(config.worldUp,{0,0,1});
    config.headingDegrees=std::remainder(config.headingDegrees,360.0f);
    config_=config;
    initialized_=false;
}
void CameraFollow::configureFromPose(const CameraPose& pose,CameraVec3 actorPosition,float smoothingRate){
    // Reuse the public view validator for pose/FOV validity.
    (void)cameraViewMatrix(pose);
    if(!finite(actorPosition))throw std::invalid_argument("Camera actor position must be finite");
    const auto up=normalized(pose.up,{0,0,1});
    const auto offset=sub(pose.position,pose.target);
    const float distance=std::sqrt(dot(offset,offset));
    if(distance<=epsilon)throw std::invalid_argument("Follow camera needs distinct eye and target");
    const auto basis=cameraMovementBasis(pose,up);
    const auto reference=referenceForward(up);
    const auto right=normalized(cross(reference,up),{1,0,0});
    CameraFollowConfig config;
    config.distance=distance;
    config.pitchDegrees=std::asin(std::clamp(dot(offset,up)/distance,-1.0f,1.0f))*180/pi;
    config.headingDegrees=std::atan2(dot(basis.forward,right),dot(basis.forward,reference))*180/pi;
    config.targetOffset=sub(pose.target,actorPosition);
    config.smoothingRate=smoothingRate;
    config.verticalFovDegrees=pose.verticalFovDegrees;
    config.worldUp=up;
    configure(config);
    pose_=pose;
    initialized_=true;
}
CameraPose CameraFollow::desiredPose(const CameraActorTarget& actor) const{
    validateActor(actor);
    auto forward=referenceForward(config_.worldUp);
    if(config_.followFacing)forward=normalized(projected(actor.facing,config_.worldUp),forward);
    const auto right=normalized(cross(forward,config_.worldUp),{1,0,0});
    const float heading=config_.headingDegrees*pi/180;
    forward=add(mul(forward,std::cos(heading)),mul(right,std::sin(heading)));
    const float pitch=config_.pitchDegrees*pi/180;
    const auto target=add(actor.position,config_.targetOffset);
    const auto eye=add(sub(target,mul(forward,config_.distance*std::cos(pitch))),
                       mul(config_.worldUp,config_.distance*std::sin(pitch)));
    return {eye,target,config_.worldUp,config_.verticalFovDegrees};
}
void CameraFollow::reset(const CameraActorTarget& actor){pose_=desiredPose(actor);initialized_=true;}
void CameraFollow::update(const CameraActorTarget& actor,double dt){
    if(!std::isfinite(dt)||dt<0)throw std::invalid_argument("Camera delta time must be finite and nonnegative");
    const auto desired=desiredPose(actor);
    if(!initialized_||config_.smoothingRate==0){pose_=desired;initialized_=true;return;}
    // Exact exponential response for a constant target; independent of how a
    // time interval is split into frames. A zero dt does not move the camera.
    const float response=static_cast<float>(-std::expm1(-double(config_.smoothingRate)*dt));
    pose_.position=lerp(pose_.position,desired.position,response);
    pose_.target=lerp(pose_.target,desired.target,response);
    pose_.up=desired.up;
    pose_.verticalFovDegrees=desired.verticalFovDegrees;
}
CameraMovementBasis CameraFollow::movementBasis() const{return cameraMovementBasis(pose_,config_.worldUp);}

} // namespace dh::foundation
