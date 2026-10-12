#include "hud_startup_callbacks.hpp"
namespace {
using namespace dh2::ui;
bool valid(const HudStartupState48* s,const HudStartupServices16* services) {
    return s&&services&&services->invoke&&!s->reserved&&s->sharp_devices<=255&&s->htc_devices<=255&&s->no_igp<=255;
}
int call(HudStartupState48* s,const HudStartupServices16* svc,HudStartupOperation op,
    std::uintptr_t subject=0,std::int32_t argument=0,const char* name=nullptr,
    HudStartupResponse16* result=nullptr,float a=0,float b=0,float c=0) {
    HudStartupRequest40 request{op,argument,subject,name,{a,b,c},0};HudStartupResponse16 response{};
    if(svc->invoke(svc->context,s,&request,&response)!=0||response.reserved)return -2;
    if(result)*result=response;return 0;
}
}
extern "C" int dh2_hud_load_settings(dh2::ui::HudStartupState48* s,const dh2::ui::HudStartupServices16* svc) noexcept {
    using namespace dh2::ui;
    if(!valid(s,svc)||!s->application||!s->savegame)return -1;
    const auto application=s->application,initial_manager=s->savegame;
    if(call(s,svc,HudStartupOperation::load_settings,initial_manager,0)||call(s,svc,HudStartupOperation::update_saved_values))return -2;
    const auto sound=s->sound;
    if(sound){
        HudStartupResponse16 music{},fx{};
        if(call(s,svc,HudStartupOperation::get_saved_option,application,0,"VolumeMusic",&music)||
           call(s,svc,HudStartupOperation::get_saved_option,application,0,"VolumeFX",&fx))return -2;
        if(call(s,svc,HudStartupOperation::set_initial_volume,sound,0,nullptr,nullptr,
            static_cast<float>(music.value),static_cast<float>(fx.value),0.0f))return -2;
    }
    HudStartupResponse16 language{};
    if(!s->savegame||call(s,svc,HudStartupOperation::get_language,s->savegame,0,nullptr,&language))return -2;
    if(!s->savegame||call(s,svc,HudStartupOperation::set_language,s->savegame,language.value))return -2;
    return 0;
}
extern "C" int dh2_hud_is_multiplayer_enabled(dh2::ui::HudStartupState48* s,const dh2::ui::HudStartupServices16* svc) noexcept {
    using namespace dh2::ui;
    if(!valid(s,svc)||!s->result)return -1;
    const auto result=s->result;std::int32_t value{};
    if(s->sharp_devices||s->htc_devices)value=1;
    else if(!s->no_igp){HudStartupResponse16 performance{};
        if(call(s,svc,HudStartupOperation::is_high_performance,0,0,nullptr,&performance)||performance.value<0||performance.value>1)return -2;
        value=performance.value;
    }
    return call(s,svc,HudStartupOperation::set_result_bool,result,value);
}
extern "C" int dh2_hud_device_pipeline(const dh2::ui::HudDevicePipeline16* s) noexcept {
    if(!s||s->exclusions[0]>255||s->exclusions[1]>255||s->exclusions[2]>255)return -1;
    return s->exclusions[0]||s->exclusions[1]||s->exclusions[2]?0:int((s->capabilities&0x78)!=0);
}
