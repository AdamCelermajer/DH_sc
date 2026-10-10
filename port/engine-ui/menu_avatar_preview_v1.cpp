#include "menu_avatar_preview_v1.hpp"
namespace dh2::ui {
bool change_menu_avatar_preview_v1(MenuAvatarPreviewStateV1& state,std::int32_t slot,bool force,
    const MenuAvatarPreviewServicesV1& services,std::string& error,bool fresh_slot_intent){
    if(slot<-1||slot>=4){error="Unsafe menu avatar campaign slot";return false;}
    if(state.slot==slot&&!force){error.clear();return true;}
    if(!services.setup_character||!services.create_avatar_camera||
        (state.slot!=-1&&!services.destroy_character)){
        error="Canonical menu avatar creation/equipment/camera services unavailable";return false;
    }
    if(state.slot!=-1&&!services.destroy_character(services.context,error))return false;
    state.slot=slot;
    state.fresh_slot_intent=fresh_slot_intent;
    const bool setup_ok=services.setup_character(services.context,slot,error);
    state.fresh_slot_intent=false; // one-shot, including provider failures
    if(!setup_ok)return false;
    if(!services.create_avatar_camera(services.context,error))return false;
    error.clear();return true;
}
}
