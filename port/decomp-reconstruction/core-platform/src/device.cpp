#include "core-platform/device.hpp"

#include "core-platform/deps/application.hpp"

// 0x381668 Device::IsFixedPipeline()
bool Device::IsFixedPipeline() { return (DeviceFeatureFlags() & 7u) != 0; }

// 0x3816a8 Device::IsProgrammablePipeline()
// Partial: the listing calls virtual slot +0x5c of the feature object (global at
// +0x10) and tests its result with mask 0x78; DeviceFeatureFlags() stands in for
// that virtual call. The mask's meaning is not named in the original.
bool Device::IsProgrammablePipeline() {
    if (!SHARP_DEVICES && !HTC_DEVICES && !Is_In_Multiplayer_Mode) {
        return (DeviceFeatureFlags() & 0x78u) != 0;
    }
    return false;
}

// 0x38173c Device::GetModel()
int Device::GetModel() { return -1; }

// 0x381744 Device::IsiOSVersionSupported(char const*)
bool Device::IsiOSVersionSupported(const char*) { return true; }

// 0x38174c Device::IsHighPerformance()
// A thunk: the listing returns Device::IsProgrammablePipeline(this).
bool Device::IsHighPerformance() { return IsProgrammablePipeline(); }

// 0x381750 Device::IsIphoneHighRez()
bool Device::IsIphoneHighRez() { return true; }

// 0x381758 Device::GetScreenResolution()
Device::Resolution Device::GetScreenResolution() { return Resolution{960, 640}; }

// 0x38176c Device::IsMultitaskSupported()
bool Device::IsMultitaskSupported() { return true; }

// 0x381774 Device::IsGameCenterSupported()
bool Device::IsGameCenterSupported() { return false; }

// 0x38177c Device::IsBluetoothSupported()
bool Device::IsBluetoothSupported() { return true; }
