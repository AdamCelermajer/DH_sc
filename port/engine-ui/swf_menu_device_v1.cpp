#include "swf_menu_device_v1.hpp"
#include "gameswf/gameswf_function.h"
namespace dh2::ui {
namespace {
struct RequestOwner {gameswf::as_value* result;HudDevicePipeline16 pipeline;};
int invoke(void* context,HudStartupState48*,const HudStartupRequest40* request,HudStartupResponse16* response){
    auto& owner=*static_cast<RequestOwner*>(context);
    if(request->operation==HudStartupOperation::is_high_performance){
        response->value=dh2_hud_device_pipeline(&owner.pipeline);return response->value<0?-1:0;
    }
    if(request->operation==HudStartupOperation::set_result_bool&&request->subject==reinterpret_cast<std::uintptr_t>(owner.result)){
        owner.result->set_bool(request->argument!=0);return 0;
    }
    return -1;
}
}
bool swf_menu_multiplayer_enabled_v1(const gameswf::fn_call& fn,const MenuDeviceFactsV1& facts,std::string& error){
    if(!fn.result){error="Device query requires its actual AS result";return false;}
    HudStartupState48 state{};
    state.result=reinterpret_cast<std::uintptr_t>(fn.result);
    state.sharp_devices=facts.sharp;state.htc_devices=facts.htc;state.no_igp=facts.no_igp;
    RequestOwner owner{fn.result,{{facts.sharp,facts.htc,facts.multiplayer_mode},facts.driver_type}};
    const HudStartupServices16 services{&owner,invoke};
    if(dh2_hud_is_multiplayer_enabled(&state,&services)){error="Original graphics capability query failed";return false;}
    error.clear();return true;
}
}
