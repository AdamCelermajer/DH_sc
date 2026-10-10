#include "../platform_win32.hpp"
#include "../renderer.hpp"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <GL/gl.h>
#include <thread>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
static void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
int main(){try{
    std::string error;bool selected=false;Renderer renderer;
    check(!renderer.game_object_visual_quality(selected,error),"Uninitialized renderer admitted source quality");
    Window window;check(window.open("Native visual loading policy",320,240),window.error());
    check(renderer.initialize(window.width(),window.height()),"Renderer initialize failed");
    check(renderer.game_object_visual_quality(selected,error)&&selected,"Default original-content Full policy unavailable");
    renderer.setVisualAssetQuality(VisualAssetQuality::ReducedOptional);
    check(renderer.game_object_visual_quality(selected,error)&&!selected,"Actual reduced-optional policy not observed");
    bool otherThreadAccepted=true;
    std::thread otherThread([&]{std::string failure;bool value=true;otherThreadAccepted=renderer.game_object_visual_quality(value,failure);check(value,"Rejected thread changed output");});
    otherThread.join();check(!otherThreadAccepted,"Foreign thread admitted renderer policy");
    const auto originalContext=wglGetCurrentContext();const auto originalDevice=wglGetCurrentDC();
    {
        Window replacement;check(replacement.open("Native replacement context",320,240),replacement.error());
        selected=true;
        check(!renderer.game_object_visual_quality(selected,error)&&selected,"Foreign context admitted old renderer policy");
    }
    check(!renderer.game_object_visual_quality(selected,error),"Absent context admitted old renderer policy");
    check(wglMakeCurrent(originalDevice,originalContext)!=FALSE,"Original context restore failed");
    check(renderer.game_object_visual_quality(selected,error)&&!selected,"Same live restored context lost selected quality");
    renderer.setVisualAssetQuality(static_cast<VisualAssetQuality>(255));selected=true;
    check(!renderer.game_object_visual_quality(selected,error)&&selected,"Unknown policy admitted or changed output");
    renderer.setVisualAssetQuality(VisualAssetQuality::Full);
    check(renderer.game_object_visual_quality(selected,error)&&selected,"Full policy could not be selected again");
    std::cout<<"PASS actual WGL Renderer full/reduced asset policy, context/thread identity and missing/invalid rejection\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
