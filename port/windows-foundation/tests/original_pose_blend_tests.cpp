#include "../original_pose_blend.hpp"
#include "../../engine-animation/animation.hpp"
#include "../../engine-animation/animation_blend.hpp"
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <cstring>

using namespace dh::foundation;
namespace {
void check(bool value,const std::string& message) { if(!value) throw std::runtime_error(message); }
SkeletalPose fixture(float x,float angle) {
    LocalNodePose p; p.id="joint"; p.translation={x,0,0};
    p.quaternion={0,0,std::sin(angle/2),std::cos(angle/2)}; return {p};
}
bool same(const SkeletalPose& a,const SkeletalPose& b) {
    if(a.size()!=b.size()) return false;
    for(std::size_t i=0;i<a.size();++i)
        if(a[i].id!=b[i].id || a[i].translation!=b[i].translation
           || a[i].quaternion!=b[i].quaternion || a[i].scale!=b[i].scale) return false;
    return true;
}
std::vector<std::uint8_t> read(const std::string& path) {
    std::ifstream f(path,std::ios::binary); check(bool(f),"Missing "+path);
    return {std::istreambuf_iterator<char>(f),std::istreambuf_iterator<char>()};
}
void metadata_and_atomicity() {
    OriginalPoseBlend blend; std::string error; SkeletalPose pose;
    const std::array<SkeletalPose,2> poses{fixture(10,1.5f),fixture(0,0)};
    std::array<int,2> calls{};
    auto sample=[&](std::uint32_t slot,SkeletalPose& out,std::string&) { ++calls[slot];out=poses[slot];return true; };
    check(blend.select(100,error) && blend.current_slot()==1,error);
    check(blend.evaluate(0,sample,pose,error) && same(pose,poses[1]),"Initial incoming pose differs");
    check(calls[0]==0 && calls[1]==1,"Initial zero-weight slot sampled");
    check(blend.select(0,error) && blend.current_slot()==0,error);
    check(blend.state().remaining==100 && blend.state().duration==0,"Incoming fade ignored outgoing BlendOut");
    check(blend.evaluate(50,sample,pose,error),error);
    check(std::abs(pose[0].translation[0]-5)<1e-6f && calls[0]==1 && calls[1]==2,"50ms fade sample weights differ");
    check(std::abs(pose[0].quaternion[2]-std::sin(.375f))<1e-5f,"Quaternion source slerp differs");
    const auto before=pose; const auto state=blend.state();
    auto malformed=[&](std::uint32_t slot,SkeletalPose& out,std::string&) { out=poses[slot];out[0].id="other";return slot==0; };
    check(!blend.evaluate(75,malformed,pose,error) && same(before,pose)
          && std::memcmp(&state,&blend.state(),sizeof(state))==0,"Failed pose sampler changed blend/output");
    check(blend.evaluate(100,sample,pose,error) && same(pose,poses[0]),"Fade endpoint differs");
    check(calls[1]==2,"Completed fade sampled outgoing slot");
    check(blend.select(999,error) && blend.state().remaining==0,"BlendOut0 did not cause hard transition");
    check(blend.evaluate(100,sample,pose,error) && same(pose,poses[1]),"Zero-duration transition differs");
    blend.clear(); check(blend.state().current==0 && blend.state().duration==0,"Clear did not reset source metadata");
}
void real_poses(const std::string& root) {
    std::string error;
    const auto model=read(root+"/models/prince_modular.bdae");
    dh2::resources::BresView bres{};
    check(dh2_bres_open(&bres,model.data(),model.size())==dh2::resources::BresError::ok,"Real model BRES rejected");
    dh2::scene::Scene rest;
    check(dh2::scene::load(bres,rest,error),error);
    std::array<dh2::animation::Player,2> players;
    // Slots are physical AnimatorBlender order: incoming walk0, outgoing idle1.
    const std::array<std::string,2> paths{"prince_walk_1hand.bdae","prince_idle_shield.bdae"};
    for(std::uint32_t slot=0;slot<2;++slot) {
        const auto bytes=read(root+"/animations/"+paths[slot]);
        check(players[slot].load(bytes.data(),bytes.size(),rest,error,dh2::animation::MissingTargets::ignore),error);
    }
    OriginalPoseBlend blend; SkeletalPose pose;
    std::uint32_t now=0; std::array<int,2> calls{};
    auto sample=[&](std::uint32_t slot,SkeletalPose& output,std::string& e) {
        ++calls[slot]; auto scene=rest;
        const auto time=players[slot].start+static_cast<std::int32_t>(now)+(slot==1?150:0);
        return players[slot].sample(scene,time,e) && capture_scene_pose(scene,output,e);
    };
    check(blend.select(100,error) && blend.evaluate(0,sample,pose,error),error);
    check(blend.select(0,error),error);
    now=50; check(blend.evaluate(now,sample,pose,error),error);
    SkeletalPose walk,idle; check(sample(0,walk,error) && sample(1,idle,error),error);
    std::size_t moving=0;
    for(std::size_t i=0;i<pose.size();++i) {
        const float weights[]={.5f,.5f};
        float values[8],expected[4];
        std::copy(walk[i].quaternion.begin(),walk[i].quaternion.end(),values);
        std::copy(idle[i].quaternion.begin(),idle[i].quaternion.end(),values+4);
        check(!dh2_animation_blend_quaternion(expected,values,weights,2),"Real quaternion kernel rejected");
        for(unsigned axis=0;axis<4;++axis) check(pose[i].quaternion[axis]==expected[axis],"Real mixed quaternion differs");
        for(unsigned axis=0;axis<3;++axis) {
            const float expected_position=(0.0f+.5f*walk[i].translation[axis])+.5f*idle[i].translation[axis];
            check(pose[i].translation[axis]==expected_position,"Real mixed position differs");
        }
        moving+=walk[i].translation!=idle[i].translation || walk[i].quaternion!=idle[i].quaternion;
    }
    check(moving>0,"Real poses were identical");
    auto applied=rest;
    const auto owner=applied.graph[0].source_owner_v91();
    check(apply_scene_pose(pose,applied,error),error);
    check(applied.graph[0].source_owner_v91()==owner,"Applying pose replaced native node storage");
    SkeletalPose captured; check(capture_scene_pose(applied,captured,error) && same(pose,captured),"Applied real local poses differ");
    for(const auto& node:applied.graph) for(float value:node.world) check(std::isfinite(value),"Mixed hierarchy became nonfinite");
    auto malformed=pose; malformed[0].id="wrong-node";
    check(!apply_scene_pose(malformed,applied,error),"Mismatched real scene domain accepted");
    check(capture_scene_pose(applied,captured,error) && same(pose,captured),"Invalid apply changed scene");
    malformed=pose;malformed[0].translation[0]=std::numeric_limits<float>::quiet_NaN();
    check(!apply_scene_pose(malformed,applied,error),"Nonfinite real pose accepted");
    now=100; check(blend.evaluate(now,sample,pose,error),error);
    check(sample(0,walk,error) && same(pose,walk),"Real fade endpoint differs from incoming sampled pose");
    std::cout<<"Real Prince local poses: "<<pose.size()<<" nodes, "<<moving<<" differing nodes\n";
}
}
int main(int argc,char**argv) {
    try {
        metadata_and_atomicity();
        check(argc==2,"Expected original asset root for real-pose verification");
        real_poses(argv[1]);
        std::cout<<"original pose blend tests passed\n";
    } catch(const std::exception& e) { std::cerr<<e.what()<<'\n'; return 1; }
}
