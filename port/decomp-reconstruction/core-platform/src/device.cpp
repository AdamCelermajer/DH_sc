#include "core-platform/device.hpp"

#include "core-platform/deps/application.hpp"

// 0x381668 Device::IsFixedPipeline()
bool Device::IsFixedPipeline() { return (DeviceFeatureFlags() & 7u) != 0; }

// 0x3816a8 Device::IsProgrammablePipeline()
// Partial: the feature bits are read from the device object; the meaning of
// the 0x78 mask is not named in the original, so it is kept as-is.
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
// Partial: same body as IsProgrammablePipeline in the original (see above).
bool Device::IsHighPerformance() {
    if (!SHARP_DEVICES && !HTC_DEVICES && !Is_In_Multiplayer_Mode) {
        return (DeviceFeatureFlags() & 0x78u) != 0;
    }
    return false;
}

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
