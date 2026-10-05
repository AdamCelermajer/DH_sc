#pragma once
#include "owned_hud_settings_v1.hpp"
namespace gameswf {struct fn_call;}
namespace dh2::ui {
struct SwfMenuOptionServicesV1 {
    OwnedHudSettingsV1* settings{};
    void* context{};
    bool sharp_device=false;
    bool (*string_by_id)(void*,std::int32_t,std::string&,std::string&){};
    bool (*observe_sharp_language)(void*,std::int32_t,std::string&){};
    bool (*apply_option)(void*,const char*,std::int32_t,std::string&){};
    bool (*load)(void*,std::string&){};
    bool (*save)(void*,std::string&){};
    bool (*enter)(void*,std::string&){};
    bool (*refresh_hud)(void*,std::string&){};
    bool (*input_behavior)(void*,std::int32_t,bool,std::string&){};
    bool japanese_build=false;
};
// Original NativeGetOptionParameters 0x44a298. Preserves the supplied AS
// object and actual member setters; requires real settings/string owners.
bool swf_menu_option_parameters(const gameswf::fn_call&,const SwfMenuOptionServicesV1&,std::string&);
bool swf_menu_settings_action(const char*,const gameswf::fn_call&,const SwfMenuOptionServicesV1&,std::string&);
}
