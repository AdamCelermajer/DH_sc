#pragma once
#include <string>
#include <cstdint>
namespace gameswf {struct fn_call;}
namespace dh2::ui {
// Synchronous original MenuManager entry points. Providers must implement
// real stack/lifecycle ownership; collecting requests is not delivery.
struct SwfMenuNavigationServicesV1 {
    void* context{};
    bool (*push_named)(void*,const char*,std::string&){};
    bool (*pop_named)(void*,const char*,std::string&){};
    bool (*pop_top)(void*,std::string&){};
    bool (*pop_above)(void*,const char*,std::string&){};
};
// Original 0x43b1b4 and 0x43b158: arg(0).to_xstring(), ignoring extra
// arguments and leaving the AS result untouched. Pop with zero arguments
// invokes the MultiMenuManager virtual top-pop directly.
bool swf_menu_push(const gameswf::fn_call&,const SwfMenuNavigationServicesV1&,std::string&);
bool swf_menu_pop(const gameswf::fn_call&,const SwfMenuNavigationServicesV1&,std::string&);
// 0x43ac28: exactly one STRING/OBJECT argument, to_xstring, then
// MultiMenuManager::PopMenu(name,true); leaves the AS result untouched.
bool swf_menu_pop_above(const gameswf::fn_call&,const SwfMenuNavigationServicesV1&,std::string&);
// Source 0x43ccec, actual dt and HTC device fact; result uses the real AS tag.
bool swf_menu_credit_movement(const gameswf::fn_call&,std::uint32_t milliseconds,bool htc,std::string&);
}
