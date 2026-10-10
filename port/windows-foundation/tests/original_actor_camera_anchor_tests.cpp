#include "original_actor_camera_anchor.hpp"
#include "asset_catalog.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
namespace {
void check(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
bool near(float a,float b){return std::abs(a-b)<0.0001f;}
void run(const char* assetRoot){
    AssetCatalog assets(assetRoot);OriginalActorCameraAnchorConfig config;std::string error;
    const auto zeroLook=original_actor_camera_look_at(0);
    check(near(zeroLook.x,0)&&near(zeroLook.y,-1)&&near(zeroLook.z,0),"source Euler zero looks along minus Y");
    check(load_original_actor_camera_anchor_config(assets,false,config,error),error.c_str());
    check(near(config.maximumDistance,640)&&near(config.distancePerUpdate,17)&&near(config.threshold,0.5f),"original design constants differ");
    OriginalActorCameraAnchorFrame frame;frame.position={1000,2000,25};frame.lookAt={1,0,0};
    OriginalActorCameraAnchor anchor;
    check(anchor.initialize(config,frame,error),error.c_str());
    check(near(anchor.position().x,1000)&&near(anchor.position().z,25),"InitCam anchor reset copies actual GameObject XYZ");
    check(anchor.update(frame,16,error)&&near(anchor.position().x,1000),"idle before movement has no invented height or lead");
    frame.headingActive=true;frame.heading={1,0,0};frame.attacking=true;
    check(anchor.update(frame,16,error)&&near(anchor.position().x,1320)&&near(anchor.position().z,25),"stationary attacking receives original half-distance lead");
    frame.attacking=false;frame.headingActive=false;
    check(anchor.update(frame,16,error)&&near(anchor.position().x,1256),"idle lead caps at original 40 percent maximum");
    frame.position.x=1100;frame.headingActive=true;frame.heading={1,0,0};frame.moving=true;
    check(anchor.update(frame,16,error)&&near(anchor.position().x,1420),"moving begins original minimum half-distance lead");
    frame.position.x=1200;
    check(anchor.update(frame,16,error)&&near(anchor.position().x,1537),"moving increments by original 17 per update, not dt scaling");
    frame.position.x=1300;
    check(anchor.update(frame,1000,error)&&near(anchor.position().x,1654),"normal forward anchor preserves frame-dependent source algorithm");
    frame.position.x=1400;frame.heading={-1,0,0};frame.lookAt={-1,0,0};
    check(anchor.update(frame,16,error)&&near(anchor.position().x,1050.25f),"large heading turn reduces original distance by speed quarter");
    OriginalActorCameraAnchorConfig staticConfig;
    check(load_original_actor_camera_anchor_config(assets,true,staticConfig,error),"static source debug branch config");
    OriginalActorCameraAnchor staticAnchor;check(staticAnchor.initialize(staticConfig,frame,error),"static anchor initialize");
    frame.position={44,55,66};check(staticAnchor.update(frame,16,error),"static anchor update");
    check(near(staticAnchor.position().x,44)&&near(staticAnchor.position().z,66),"static anchor exactly follows physical position");
    check(anchor.reset(frame,error)&&near(anchor.position().x,44),"source reset uses physical position");
    config.threshold=2;check(!staticAnchor.initialize(config,frame,error),"invalid recovered constants rejected");
}
}
int main(int argc,char** argv){try{run(argc>1?argv[1]:"port/android-native/app/src/main/assets");std::cout<<"Source actor camera anchor PASS\n";return 0;}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
