#include "swf_menu_navigation.hpp"
#include "gameswf/gameswf_function.h"
namespace dh2::ui {
bool swf_menu_push(const gameswf::fn_call& fn,const SwfMenuNavigationServicesV1& services,std::string& error){
    // Source has no zero-argument guard. Reject malformed host calls instead
    // of accessing outside the AS stack; do not invent an empty menu name.
    if(fn.nargs<1||!fn.env){error="Malformed menu-push AS call";return false;}
    // Retain the conversion through synchronous delivery. Object conversion
    // uses the actual core to_xstring, not a flattened/JSON object label.
    const std::string name=fn.arg(0).to_xstring();
    if(!services.push_named){error="Required MenuManager push owner unavailable";return false;}
    return services.push_named(services.context,name.c_str(),error);
}
bool swf_menu_pop(const gameswf::fn_call& fn,const SwfMenuNavigationServicesV1& services,std::string& error){
    if(fn.nargs<0){error="Malformed menu-pop AS call";return false;}
    if(fn.nargs==0){
        if(!services.pop_top){error="Required MultiMenuManager top-pop owner unavailable";return false;}
        return services.pop_top(services.context,error);
    }
    if(!fn.env){error="Malformed menu-pop AS call";return false;}
    const std::string name=fn.arg(0).to_xstring();
    if(!services.pop_named){error="Required MenuManager named-pop owner unavailable";return false;}
    return services.pop_named(services.context,name.c_str(),error);
}
}
