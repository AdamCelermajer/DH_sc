#include "swf_menu_sound.hpp"
#include "gameswf/gameswf_function.h"
namespace dh2::ui {
bool swf_menu_sound_argument(const gameswf::fn_call& fn,std::string& name){
    if(fn.nargs!=1||!fn.arg(0).is_string())return false;
    name=fn.arg(0).to_xstring();return true;
}
}
