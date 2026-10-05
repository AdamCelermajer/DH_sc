#pragma once
#include "swf_event_dispatch.hpp"
#include <array>
#include <string>
#include <vector>
namespace dh2::ui {
// MenuBase constructor initializes DragAndDrop=null and an empty dead-zone
// vector. Its rollover flag is a separate source BSS boolean (initially 0).
// This owns that native event stage, not MenuManager/HUDControls forwarding.
struct MenuNativeEventServicesV1 {
    void* context{};
    bool (*drag_event)(void*,SwfEvent48&,std::string&){};
    bool (*can_mouse)(void*,std::uintptr_t,bool&,std::string&){};
    bool (*focus)(void*,std::uintptr_t,std::string&){};
    bool (*raw_position)(void*,int&,int&,std::string&){};
    bool (*consume)(void*,std::string&){};
    bool (*browser)(void*,const char*,std::string&){};
    bool (*language)(void*,int&,std::string&){};
    bool (*online)(void*,bool live,int language,std::string&){};
};
class MenuNativeEventV1 {
public:
    bool rollover_enabled=false,drag_bound=false,render_bound=false;
    std::vector<std::array<float,4>> dead_zones; // xmin,xmax,ymin,ymax
    bool base(SwfEvent48&,const MenuNativeEventServicesV1&,std::string&);
    bool main(SwfEvent48&,const MenuNativeEventServicesV1&,std::string&);
private:
    bool post(const MenuNativeEventServicesV1&,std::string&);
};
}
