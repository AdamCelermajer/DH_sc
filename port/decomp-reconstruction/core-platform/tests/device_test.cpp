#include "core-platform/device.hpp"

#include "dh2/Application.hpp"
#include "glitch-core-io/os/device.hpp"
#include "glitch-video/null_driver.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

// Test stubs for the platform flags and the process singleton. IsProgrammablePipeline reads
// ApplicationGetInstance().m_device->videoDriver and calls the virtual at IVideoDriver vtable
// +0x5c (getDriverType); only those words are used, so the Application storage is
// zero-initialised and no Application constructor runs (the same pattern as the game-app tests).
bool SHARP_DEVICES = false;
bool HTC_DEVICES = false;
bool Is_In_Multiplayer_Mode = false;

// Still read by IsFixedPipeline, whose row is not in this batch.
static unsigned g_flags = 0;
unsigned DeviceFeatureFlags() { return g_flags; }

alignas(Application) static unsigned char g_applicationStorage[sizeof(Application)];
Application& ApplicationGetInstance() { return *reinterpret_cast<Application*>(g_applicationStorage); }

// The driver type 0x78 is CProgrammableGLDriver::getDriverType (0x5af04c); CNullDriver
// returns 0, the fixed-pipeline case.
class ProgrammableDriver : public glitch::video::CNullDriver {
public:
    using CNullDriver::CNullDriver;
    glitch::video::E_DRIVER_TYPE getDriverType() const override { return 0x78; }
};

int main() {
    glitch::SCreationParameters parameters{};
    glitch::IDevice device(parameters);
    glitch::video::CNullDriver fixedPipelineDriver(&device);
    ProgrammableDriver programmableDriver(&device);
    ApplicationGetInstance().m_device = &device;

    // IsProgrammablePipeline: the driver type (mask 0x78) decides, and the three gates veto it.
    device.videoDriver = &fixedPipelineDriver;
    CHECK(!Device::IsProgrammablePipeline());
    device.videoDriver = &programmableDriver;
    CHECK(Device::IsProgrammablePipeline());
    CHECK(Device::IsHighPerformance());  // thunk to IsProgrammablePipeline
    SHARP_DEVICES = true;
    CHECK(!Device::IsProgrammablePipeline());
    CHECK(!Device::IsHighPerformance());
    SHARP_DEVICES = false;
    HTC_DEVICES = true;
    CHECK(!Device::IsProgrammablePipeline());
    HTC_DEVICES = false;
    Is_In_Multiplayer_Mode = true;
    CHECK(!Device::IsProgrammablePipeline());
    Is_In_Multiplayer_Mode = false;

    // IsFixedPipeline still reads the DeviceFeatureFlags() stand-in (its row is not this batch).
    g_flags = 0x1;
    CHECK(Device::IsFixedPipeline());
    g_flags = 0x8;
    CHECK(!Device::IsFixedPipeline());

    CHECK(Device::GetModel() == -1);
    CHECK(Device::IsIphoneHighRez());
    CHECK(!Device::IsGameCenterSupported());
    Device::Resolution r = Device::GetScreenResolution();
    CHECK(r.width == 960 && r.height == 640);
    std::printf("%s\n", failures ? "device: FAILED" : "device: ok");
    return failures != 0;
}
