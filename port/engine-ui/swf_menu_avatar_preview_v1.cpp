#include "menu_avatar_preview_v1.hpp"
#include "gameswf/gameswf_function.h"
#include <cmath>
namespace dh2::ui {
bool swf_menu_avatar_preview_v1(const gameswf::fn_call& fn,MenuAvatarPreviewStateV1& state,
    const MenuAvatarPreviewServicesV1& services,std::string& error){
    if(fn.nargs<1||!fn.env){error="Malformed menu avatar AS call";return false;}
    const double value=fn.arg(0).to_number();
    if(!std::isfinite(value)||value<-2147483648.0||value>=2147483648.0){error="Unsafe menu avatar slot number";return false;}
    const auto slot=static_cast<std::int32_t>(value);
    const bool force=fn.nargs>1?fn.arg(1).to_bool():false;
    return change_menu_avatar_preview_v1(state,slot,force,services,error);
}
}
