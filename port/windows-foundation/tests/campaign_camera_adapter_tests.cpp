#include "campaign_camera_adapter.hpp"
#include <cmath>
#include <iostream>
#include <map>
#include <stdexcept>
using namespace dh::foundation;
namespace {
void check(bool b,const char* message){if(!b)throw std::runtime_error(message);}
bool near(float a,float b){return std::abs(a-b)<0.0001f;}
void run(){
    std::map<std::uint64_t,CameraVec3> anchors{{1,{100,200,300}},{2,{1100,1200,1300}}};
    CampaignCameraProviders providers;
    providers.named_target=[](const std::string& name,std::uint64_t& id,bool& found,std::string&){
        found=name=="Waypoint"||name=="LocalPlayer";id=name=="Waypoint"?2:1;return true;};
    providers.local_player=[](std::uint64_t& id,std::string&){id=1;return true;};
    providers.anchor=[&](std::uint64_t id,CameraVec3& value,std::string&){value=anchors.at(id);return true;};
    CampaignCameraAdapter camera(providers);std::string error;bool handled=false,blocking=false;CampaignCameraFrame frame;
    check(camera.seed_target(1,error),"seed local target");
    check(camera.tick(16,frame,error)&&!frame.applyDamping&&near(frame.anchor.x,100),"immediate source transition bypasses damping");
    check(camera.tick(16,frame,error)&&frame.applyDamping,"normal follow resumes after transition");
    OriginalCampaignCommand target;target.kind=8;target.strings[16]="Waypoint";target.scalars[8]=1000;target.scalars[20]=0;
    check(camera.command(CampaignCommandPhase::execute,target,false,handled,blocking,error)&&handled,"execute source target");
    check(camera.command(CampaignCommandPhase::is_blocking,target,false,handled,blocking,error)&&!blocking,"authored wait false does not block");
    check(camera.tick(250,frame,error)&&!frame.applyDamping&&near(frame.anchor.x,350),"quarter transition uses previous target anchor");
    anchors[2].x=1500;
    check(camera.tick(250,frame,error)&&near(frame.anchor.x,800),"transition destination queries current target each frame");
    target.scalars[20]=1;
    check(camera.command(CampaignCommandPhase::is_blocking,target,false,handled,blocking,error)&&blocking,"wait true blocks while remaining positive");
    check(camera.tick(500,frame,error)&&near(frame.anchor.x,1500)&&!frame.applyDamping,"exact endpoint bypasses damping");
    check(camera.command(CampaignCommandPhase::is_blocking,target,false,handled,blocking,error)&&!blocking,"endpoint clears blocking");
    check(camera.tick(16,frame,error)&&!frame.applyDamping,"source zero remaining completes before following next frame");
    check(camera.tick(16,frame,error)&&frame.applyDamping,"follow resumes with preserved host damping velocity");
    target.strings[16]="LocalPlayer";target.scalars[20]=0;
    check(camera.command(CampaignCommandPhase::execute,target,false,handled,blocking,error),"return through registered source alias");
    check(camera.tick(500,frame,error)&&near(frame.anchor.x,800),"return starts from previous target current anchor");
    target.strings[16]="";
    check(camera.command(CampaignCommandPhase::execute,target,true,handled,blocking,error)&&camera.transition_remaining()==0,"skip selects real local player immediately");
    check(camera.tick(16,frame,error)&&near(frame.anchor.x,100),"empty name resolves local player0");
    target.strings[16]="absent";
    check(camera.command(CampaignCommandPhase::execute,target,false,handled,blocking,error)&&camera.target()==1,"missing target is original no-op");
    OriginalCampaignCommand noOp;noOp.kind=4;
    check(camera.command(CampaignCommandPhase::execute,noOp,false,handled,blocking,error)&&handled,"SetCamera literal no-op");
    noOp.kind=5;check(camera.command(CampaignCommandPhase::execute,noOp,false,handled,blocking,error)&&!handled,"unimplemented camera animation is explicit other-provider boundary");
    CampaignCameraAdapter empty(providers);check(empty.tick(16,frame,error)&&!frame.hasTarget,"no target has no update");
    target.strings[16]="Waypoint";check(empty.command(CampaignCommandPhase::execute,target,false,handled,blocking,error),"first timed target");
    check(empty.tick(250,frame,error)&&near(frame.anchor.x,375),"first timed target starts at actual source origin");
    check(camera.seed_target(0,error)&&camera.target()==1,"null source target preserves prior target");
}
}
int main(){try{run();std::cout<<"Original camera command adapter PASS\n";return 0;}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
