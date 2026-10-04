#pragma once
#include <string>
namespace gameswf {struct fn_call;}
namespace dh2::ui {
// Synchronous original MenuManager entry points. Providers must implement
// real stack/lifecycle ownership; collecting requests is not delivery.
struct SwfMenuNavigationServicesV1 {
    void* context{};
    bool (*push_named)(void*,const char*,std::string&){};
    bool (*pop_named)(void*,const char*,std::string&){};
    bool (*pop_top)(void*,std::string&){};
};
// Original 0x43b1b4 and 0x43b158: arg(0).to_xstring(), ignoring extra
// arguments and leaving the AS result untouched. Pop with zero arguments
// invokes the MultiMenuManager virtual top-pop directly.
bool swf_menu_push(const gameswf::fn_call&,const SwfMenuNavigationServicesV1&,std::string&);
bool swf_menu_pop(const gameswf::fn_call&,const SwfMenuNavigationServicesV1&,std::string&);
}
