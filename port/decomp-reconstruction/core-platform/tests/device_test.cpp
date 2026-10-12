#include "core-platform/device.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

// Test stubs for the Application-owned device object and platform flags.
static unsigned g_flags = 0;
unsigned DeviceFeatureFlags() { return g_flags; }
bool SHARP_DEVICES = false;
bool HTC_DEVICES = false;
bool Is_In_Multiplayer_Mode = false;

int main() {
    g_flags = 0x1;
    CHECK(Device::IsFixedPipeline());
    CHECK(!Device::IsProgrammablePipeline());
    g_flags = 0x8;
    CHECK(!Device::IsFixedPipeline());
    CHECK(Device::IsProgrammablePipeline());
    CHECK(Device::IsHighPerformance());
    SHARP_DEVICES = true;
    CHECK(!Device::IsProgrammablePipeline());
    CHECK(!Device::IsHighPerformance());
    SHARP_DEVICES = false;
    CHECK(Device::GetModel() == -1);
    CHECK(Device::IsIphoneHighRez());
    CHECK(!Device::IsGameCenterSupported());
    Device::Resolution r = Device::GetScreenResolution();
    CHECK(r.width == 960 && r.height == 640);
    std::printf("%s\n", failures ? "device: FAILED" : "device: ok");
    return failures != 0;
}
