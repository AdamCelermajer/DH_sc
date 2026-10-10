#include "gameplay_camera.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
using namespace dh::foundation;
namespace {
void check(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
bool near(float a,float b){return std::abs(a-b)<0.0002f;}
void same(CameraVec3 a,CameraVec3 b,const char* m){check(near(a.x,b.x)&&near(a.y,b.y)&&near(a.z,b.z),m);}
float dot(CameraVec3 a,CameraVec3 b){return a.x*b.x+a.y*b.y+a.z*b.z;}
template<class F> void rejects(F f){try{f();}catch(const std::invalid_argument&){return;}throw std::runtime_error("invalid camera input accepted");}
void run(){
    const CameraPose authored{{14,-22,18},{10,2,3},{0,0,1},72};
    const CameraActorTarget actor{{8,1,0},{1,0,0}};
    CameraFollow follow; follow.configureFromPose(authored,actor.position);
    same(follow.pose().position,authored.position,"configure preserves eye");
    same(follow.pose().target,authored.target,"configure preserves target");
    follow.update(actor,0.1);
    same(follow.pose().position,authored.position,"derived follow preserves authored framing");
    check(near(follow.pose().verticalFovDegrees,72),"derived follow preserves FOV");
    CameraActorTarget moved{{13,-2,4},{0,-1,0}};
    follow.update(moved,0.1);
    same(follow.pose().position,{19,-25,22},"eye follows actor translation");
    same(follow.pose().target,{15,-1,7},"target follows actor translation");
    same(follow.pose().up,{0,0,1},"follow remains Z up");
    // All settings here are test fixtures, not recovered original tuning.
    CameraFollowConfig config;config.distance=20;config.pitchDegrees=30;config.smoothingRate=3;
    CameraFollow one(config),split(config);one.reset(actor);split.reset(actor);
    one.update(moved,1);for(int i=0;i<10;++i)split.update(moved,0.1);
    same(one.pose().position,split.pose().position,"damping is independent of constant-target dt partition");
    same(one.pose().target,split.pose().target,"target damping is dt invariant");
    const auto held=one.pose();one.update(actor,0);
    same(one.pose().position,held.position,"zero dt holds initialized smoothed camera");
    one.update(moved,1e100);same(one.pose().target,moved.position,"large dt converges without overshoot");
    rejects([&]{one.update(moved,-1);});
    rejects([&]{one.update(moved,std::numeric_limits<double>::quiet_NaN());});
    config.smoothingRate=0;config.followFacing=true;CameraFollow facing(config);
    facing.reset({{0,0,0},{1,0,0}});
    check(facing.pose().position.x<0&&near(facing.pose().position.y,0),"opt-in follows actor heading");
    for(CameraPose pose : {authored,CameraPose{{0,0,10},{0,0,0},{0,0,1},60},
                           CameraPose{{0,0,0},{0,0,0},{0,0,1},60}}){
        const auto basis=cameraMovementBasis(pose);
        check(near(dot(basis.right,basis.right),1)&&near(dot(basis.forward,basis.forward),1),"movement axes unit length");
        check(near(dot(basis.right,basis.forward),0)&&near(basis.right.z,0)&&near(basis.forward.z,0),"movement axes orthogonal in ground plane");
        same(basis.up,{0,0,1},"movement plane Z up");
    }
    const auto straight=cameraMovementBasis({{0,-10,10},{0,0,0},{0,0,1},60});
    same(straight.forward,{0,1,0},"camera forward projects onto XY");
    same(straight.right,{1,0,0},"camera right retains handedness");
    rejects([&]{cameraMovementBasis(authored,{0,0,0});});
    config.distance=0;rejects([&]{CameraFollow invalid(config);});
}
}
int main(){try{run();std::cout<<"gameplay camera tests passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
