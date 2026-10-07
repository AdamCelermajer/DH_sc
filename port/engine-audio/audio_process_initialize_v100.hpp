#pragma once
#include <functional>
#include <cstdint>
#include <memory>
#include <string>
namespace dh2::audio {
// Whole original VoxSoundManager.Initialize36c2e4 services on the process App.
// GSInit phase9 calls this before its first StringManager.switchPack(language,
// false); phase10 switches again. Neither string call belongs to this owner.
struct AudioInitializeServicesV100 {
 std::shared_ptr<void> actual_application;
 std::function<bool(const char*,std::int32_t&,std::string&)> saved_option;
 std::function<bool(bool&,std::string&)> high_performance,htc_devices,sharp_devices,multiplayer_mode;
 // Original load/GetSwitch("isTracingPreload_SFX") occurs after each selected UID.
 // Its boolean result is not a preload gate; preserve the actual Debug calls.
 std::function<bool(const char*,bool&,std::string&)> debug;
 // Reached only for the source process platform flag, before native gate.
 std::function<bool(int selector,float percentage,std::string&)> platform_volume;
};
}
