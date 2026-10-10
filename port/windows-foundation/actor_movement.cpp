#include "actor_movement.hpp"
#include "collision_scene.hpp"
#include "gameplay_camera.hpp"
#include "../level-world/navigation_heading.hpp"
#include "../level-world/move_state.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>

namespace dh::foundation {
namespace {
constexpr float pi = 3.14159265358979323846f;
bool finite(Vec3 v) { return std::isfinite(v.x)&&std::isfinite(v.y)&&std::isfinite(v.z); }
Vec3 subtract(Vec3 a,Vec3 b) { return {a.x-b.x,a.y-b.y,a.z-b.z}; }
Vec3 cross(Vec3 a,Vec3 b) { return {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x}; }
float dot(Vec3 a,Vec3 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }
bool intersects(Vec3 from,Vec3 to,const CollisionTriangle& t) {
    const auto d=subtract(to,from),e1=subtract(t.b,t.a),e2=subtract(t.c,t.a);
    const auto p=cross(d,e2); const float det=dot(e1,p);
    if(std::abs(det)<1e-8f) return false;
    const float inv=1/det;const auto s=subtract(from,t.a);
    const float u=dot(s,p)*inv;if(u<0||u>1)return false;
    const auto q=cross(s,e1);const float v=dot(d,q)*inv;
    if(v<0||u+v>1)return false;
    const float fraction=dot(e2,q)*inv;
    return fraction>=0 && fraction<=1;
}
bool blocked(Vec3 from,Vec3 to,const CollisionScene& scene,const ActorMovementConfig& c) {
    // Sweep an octagonal approximation of the upright body's perimeter at
    // lower, middle and upper heights against the original collision helpers.
    const float lower=std::min(c.bodyHeight*.9f,c.maxStepUp+1e-4f);
    for(int ring=0;ring<8;++ring) {
        const float angle=ring*pi/4;
        const float ox=std::cos(angle)*c.bodyRadius,oy=std::sin(angle)*c.bodyRadius;
        for(float height : {lower,std::max(lower,c.bodyHeight*.5f),c.bodyHeight}) {
            Vec3 a{from.x+ox,from.y+oy,from.z+height};
            Vec3 b{to.x+ox,to.y+oy,to.z+height};
            for(const auto& triangle:scene.triangles)
                if(!triangle.floor&&intersects(a,b,triangle))return true;
        }
    }
    return false;
}
bool supported(Vec3& candidate,Vec3 from,const CollisionScene& scene,const ActorMovementConfig& c) {
    FloorHit hit;
    if(!scene.floor(candidate.x,candidate.y,from.z,hit,c.maxStepUp,c.maxStepDown))return false;
    const float normalLength=std::sqrt(dot(hit.normal,hit.normal));
    if(normalLength<=0||std::abs(hit.normal.z)/normalLength+1e-5f<
        std::cos(c.maxSlopeDegrees*pi/180))return false;
    candidate.z=hit.point.z;
    // Original floor admission samples the actor center. Physical body radius
    // belongs to obstacle handling, not an invented floor-footprint requirement.
    return finite(candidate)&&!blocked(from,candidate,scene,c);
}
}

ActorMovement::ActorMovement(ActorMovementConfig config):config_(std::move(config)) {
    for(float v:{config_.walkSpeed,config_.runSpeed,config_.turnSpeedRadians,config_.bodyRadius,
                 config_.bodyHeight,config_.maxStepUp,config_.maxStepDown,config_.maxSlopeDegrees})
        if(!std::isfinite(v)||v<0)throw std::invalid_argument("Invalid actor movement parameter");
    if(config_.bodyRadius<=0||config_.bodyHeight<=0||config_.maxSlopeDegrees>=90)
        throw std::invalid_argument("Actor movement requires body dimensions and a slope below 90 degrees");
    if(!finite(config_.forwardAxis)||std::abs(config_.forwardAxis.z)>1e-6f||
       std::hypot(config_.forwardAxis.x,config_.forwardAxis.y)<1e-6f)
        throw std::invalid_argument("Actor forward axis must be finite, nonzero and horizontal");
    state_.animationName=config_.idleAnimation;
}
bool ActorMovement::set_body_radius(float radius,std::string& error) {
    if(!std::isfinite(radius)||radius<=0){error="Source body radius must be finite and positive";return false;}
    config_.bodyRadius=radius;error.clear();return true;
}
void ActorMovement::setPosition(Vec3 feet,float facing) {
    if(!finite(feet)||!std::isfinite(facing))throw std::invalid_argument("Invalid actor placement");
    state_.position=feet;state_.facingRadians=std::remainder(facing,2*pi);
    state_.grounded=false;
    state_.action=ActorAction::Idle;state_.animationName=config_.idleAnimation;motionScale_=0;
    state_.desiredHeading={};
}
void ActorMovement::sync_source_position(Vec3 feet,float facing) {
    if(!finite(feet)||!std::isfinite(facing))throw std::invalid_argument("Invalid source actor placement");
    state_.position=feet;state_.facingRadians=facing;
}
void ActorMovement::setFacingRadians(float facing) {
    if(!std::isfinite(facing))throw std::invalid_argument("Invalid actor facing");
    state_.facingRadians=std::remainder(facing,2*pi);
}
bool ActorMovement::setDesiredHeading(Vec3 direction,bool rotateHeadingAngle) {
    dh2::navigation::HeadingState heading{{state_.desiredHeading.direction.x,
        state_.desiredHeading.direction.y,state_.desiredHeading.direction.z},
        state_.desiredHeading.sourceAngleRadians,state_.desiredHeading.active?1u:0u,0};
    const float desired[3]{direction.x,direction.y,direction.z};
    if(dh2_nav_set_heading(&heading,desired,rotateHeadingAngle?1u:0u))return false;
    state_.desiredHeading={{heading.direction[0],heading.direction[1],heading.direction[2]},heading.angle,heading.active!=0};
    return true;
}
void ActorMovement::stopDesiredHeading() noexcept {
    state_.desiredHeading.active=false;state_.desiredHeading.direction={};
}
bool ActorMovement::step(const InputActions& input,const CameraMovementBasis& basis,
                         const CollisionScene& collision,double dt) {
    if(!std::isfinite(dt)||dt<=0||!std::isfinite(input.move2D.x)||!std::isfinite(input.move2D.y))return false;
    const Vec3 direction{basis.right.x*input.move2D.x+basis.forward.x*input.move2D.y,
                         basis.right.y*input.move2D.x+basis.forward.y*input.move2D.y,0};
    if(!finite(direction))return false;
    const float length=std::hypot(direction.x,direction.y);
    if(!std::isfinite(length))return false;
    if(length>0) { if(!setDesiredHeading(direction))return false; }
    else if(!input.attack)stopDesiredHeading();
    state_.action=ActorAction::Idle;state_.animationName=config_.idleAnimation;
    Vec3 ground=state_.position;
    state_.grounded=supported(ground,state_.position,collision,config_);
    if(state_.grounded)state_.position=ground;
    if(input.attack) {
        state_.action=ActorAction::Attack;state_.animationName=config_.attackAnimation;return false;
    }
    if(length<1e-6f||!state_.grounded)return false;
    const bool running=input.run&&config_.runSpeed>0;
    const float speed=running?config_.runSpeed:config_.walkSpeed;
    const double distance=speed*dt*std::min(length,1.0f);
    if(!std::isfinite(distance)||distance>4096*config_.bodyRadius*.5)return false;
    const Vec3 delta{static_cast<float>(distance)*direction.x/length,
                     static_cast<float>(distance)*direction.y/length,0};
    const bool moved=advance(delta,collision);
    if(moved) {
        auto effective=input;effective.run=running;
        steer(effective,basis,dt);
    }
    return moved;
}

bool ActorMovement::steer(const InputActions& input,const CameraMovementBasis& basis,double dt) {
    return steer_internal(input,basis,dt,true);
}
bool ActorMovement::steer_source_intent(const InputActions& input,const CameraMovementBasis& basis,double dt) {
    return steer_internal(input,basis,dt,false);
}
bool ActorMovement::steer_internal(const InputActions& input,const CameraMovementBasis& basis,double dt,bool preview_turn) {
    if(!std::isfinite(dt)||dt<=0||!std::isfinite(input.move2D.x)||!std::isfinite(input.move2D.y))return false;
    const Vec3 direction{basis.right.x*input.move2D.x+basis.forward.x*input.move2D.y,
                         basis.right.y*input.move2D.x+basis.forward.y*input.move2D.y,0};
    const float length=std::hypot(direction.x,direction.y);
    if(!finite(direction)||!std::isfinite(length))return false;
    if(length>0) { if(!setDesiredHeading(direction))return false; }
    else if(!input.attack)stopDesiredHeading();
    state_.action=ActorAction::Idle;state_.animationName=config_.idleAnimation;motionScale_=0;
    if(length>=1e-6f) {
        if(preview_turn){
            const float authoredHeading=std::atan2(config_.forwardAxis.y,config_.forwardAxis.x);
            const float target=std::atan2(direction.y,direction.x)-authoredHeading;
            const float difference=std::remainder(target-state_.facingRadians,2*pi);
            const float limit=static_cast<float>(std::min(double(pi),config_.turnSpeedRadians*dt));
            state_.facingRadians=std::remainder(state_.facingRadians+std::clamp(difference,-limit,limit),2*pi);
        }
        const bool running=input.run&&!config_.runAnimation.empty();
        state_.action=running?ActorAction::Run:ActorAction::Walk;
        state_.animationName=running?config_.runAnimation:config_.walkAnimation;
        motionScale_=std::min(length,1.0f);
    }
    if(input.attack) {
        state_.action=ActorAction::Attack;state_.animationName=config_.attackAnimation;motionScale_=1;
    }
    return true;
}
bool ActorMovement::apply_source_rotation_late(SourceRotationBorrow actual,const dh2::actor::RotationPolicy& policy,
    const SourceRotationSync& sync_service,std::string& error){
    error.clear();if(!actual.euler_xyz||!actual.heading_angle||!actual.turn_positive){error="Source actor Euler/heading/turn fields are required";return false;}
    auto overlap=[](const void*a,size_t an,const void*b,size_t bn){const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<an:x-y<bn;};
    if(overlap(actual.euler_xyz,12,actual.heading_angle,4)||overlap(actual.euler_xyz,12,actual.turn_positive,4)||overlap(actual.heading_angle,4,actual.turn_positive,4)){error="Source rotation actor fields overlap";return false;}
    dh2::actor::RotationState source{{actual.euler_xyz[0],actual.euler_xyz[1],actual.euler_xyz[2]},*actual.heading_angle,*actual.turn_positive,0};std::uint32_t synchronize=0;
    if(dh2_actor_update_rotation(&source,&policy,&synchronize)){error="Original source rotation kernel rejected actor/policy";return false;}
    if(synchronize&&!sync_service){error="Original visual SyncRotation provider is required";return false;}
    std::copy(source.rotation,source.rotation+3,actual.euler_xyz);*actual.turn_positive=source.turn_positive;
    state_.facingRadians=source.rotation[2];
    if(synchronize){try{if(!sync_service(actual.euler_xyz,error)){if(error.empty())error="Original visual SyncRotation failed";return false;}}catch(const std::exception& failure){error=failure.what();return false;}}
    error.clear();return true;
}
bool ActorMovement::apply_source_character_rotation_late(SourceRotationBorrow actual,const std::uint32_t* flags,
    const std::int32_t* sheet,std::uint32_t dt,bool visual_present,const SourceRotationSync& sync,std::string& error){
    error.clear();dh2::move::Policy source_policy{};float speed=0;
    if(!flags||!sheet||dh2_move_policy(&source_policy,flags)||dh2_move_rotation_speed(&speed,flags,sheet)){error="Actual Character flags520/resolved rotation property47 are required";return false;}
    const dh2::actor::RotationPolicy policy{speed,dt,visual_present?1u:0u,source_policy.visual_with_rotation};
    return apply_source_rotation_late(actual,policy,sync,error);
}

bool ActorMovement::apply_root_motion(Vec3 localDelta,const CollisionScene& collision) {
    Vec3 delta;if(!root_world_delta(localDelta,delta))return false;
    return advance(delta,collision);
}
bool ActorMovement::root_world_delta(Vec3 localDelta,Vec3& delta)const {
    if(!finite(localDelta))return false;
    if(motionScale_<=0){delta={};return true;}
    const float c=std::cos(state_.facingRadians),s=std::sin(state_.facingRadians);
    delta=Vec3{(localDelta.x*c-localDelta.y*s)*motionScale_,
                     (localDelta.x*s+localDelta.y*c)*motionScale_,0};
    return finite(delta);
}
bool ActorMovement::apply_root_motion(Vec3 localDelta,const SourceMotionAdmission& admission,
                                     bool& moved,std::string& error){
    moved=false;error.clear();Vec3 delta;
    if(!root_world_delta(localDelta,delta)){error="Invalid source root-motion displacement";return false;}
    if(!admission){error="Source motion-admission provider is required";return false;}
    SourceMotionAdmissionResult admitted{state_.position,state_.grounded};
    try{if(!admission(state_.position,delta,admitted,error)){if(error.empty())error="Source motion admission failed";return false;}}
    catch(const std::exception& failure){error=failure.what();return false;}
    if(!finite(admitted.position)){error="Source motion admission produced nonfinite position";return false;}
    moved=admitted.position.x!=state_.position.x||admitted.position.y!=state_.position.y||admitted.position.z!=state_.position.z;
    state_.position=admitted.position;state_.grounded=admitted.grounded;error.clear();return true;
}
bool ActorMovement::apply_authored_root_motion(Vec3 delta,const CollisionScene& collision){
    auto candidate=*this;candidate.motionScale_=1;
    const bool moved=candidate.apply_root_motion(delta,collision);
    state_=std::move(candidate.state_);return moved;
}
bool ActorMovement::apply_authored_root_motion(Vec3 delta,const SourceMotionAdmission& admission,bool& moved,std::string& error){
    auto candidate=*this;candidate.motionScale_=1;
    if(!candidate.apply_root_motion(delta,admission,moved,error))return false;
    state_=std::move(candidate.state_);return true;
}

bool ActorMovement::step_root_motion(const InputActions& input,const CameraMovementBasis& basis,
                                     const CollisionScene& collision,Vec3 delta,double dt) {
    if(!finite(delta))return false;
    if(!steer(input,basis,dt))return false;
    return apply_root_motion(delta,collision);
}
bool ActorMovement::step_root_motion(const InputActions& input,const CameraMovementBasis& basis,
    const SourceMotionAdmission& admission,Vec3 delta,double dt,bool& moved,std::string& error){
    moved=false;error.clear();if(!finite(delta)){error="Invalid source root-motion displacement";return false;}
    // The combined convenience call is transactional even when the source rejects
    // admission after valid steering. Individual steer/apply calls remain separate.
    auto candidate=*this;
    if(!candidate.steer(input,basis,dt)){error="Invalid source root-motion steering";return false;}
    if(!candidate.apply_root_motion(delta,admission,moved,error))return false;
    state_=std::move(candidate.state_);motionScale_=candidate.motionScale_;return true;
}

bool ActorMovement::advance(Vec3 delta,const CollisionScene& collision) {
    if(!finite(delta))return false;
    const double distance=std::hypot(double(delta.x),double(delta.y));
    const double steps=std::ceil(distance/(config_.bodyRadius*.5));
    // Reject extreme deltas rather than creating an unbounded collision workload.
    if(!std::isfinite(steps)||steps>4096)return false;
    const int count=std::max(1,static_cast<int>(steps));
    Vec3 ground=state_.position;
    state_.grounded=supported(ground,state_.position,collision,config_);
    if(!state_.grounded)return false;
    state_.position=ground;
    const float dx=delta.x/count,dy=delta.y/count;
    const Vec3 initial=state_.position;
    for(int i=0;i<count;++i) {
        const Vec3 from=state_.position;
        Vec3 full{from.x+dx,from.y+dy,from.z};
        if(supported(full,from,collision,config_)){state_.position=full;continue;}
        Vec3 slideX{from.x+dx,from.y,from.z};
        if(supported(slideX,from,collision,config_))state_.position=slideX;
        const auto current=state_.position;
        Vec3 slideY{current.x,current.y+dy,current.z};
        if(supported(slideY,current,collision,config_))state_.position=slideY;
    }
    return state_.position.x!=initial.x||state_.position.y!=initial.y;
}
}
