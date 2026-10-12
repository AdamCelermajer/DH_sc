#pragma once
#include "owned_hud_settings_v1.hpp"
namespace dh2::ui {
// Modern native private-directory provider for the original logical filename.
// Uses actual stdio/errno and copies exact file bytes; it does not synthesize a
// profile or create/write a missing file. Caller owns this through all loads.
class SettingsNativeFilesV1 {
 std::string directory_;
 static bool open(void*,const char*,bool&,std::vector<std::uint8_t>&,std::uintptr_t&,std::string&);
 static bool close(void*,std::uintptr_t,std::string&);
public:
 explicit SettingsNativeFilesV1(std::string directory);
 SettingsFileServicesV1 services(){return {this,open,close};}
};
}
