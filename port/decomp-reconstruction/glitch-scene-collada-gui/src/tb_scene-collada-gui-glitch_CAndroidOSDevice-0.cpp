// Track B batch e1-glitch-scene-collada-gui-glitch_CAndroidOSDevice-0: glitch::CAndroidOSDevice::createDriver.
// Declarations are in glitch-scene-collada-gui/include/glitch-scene-collada-gui/platform_stubs.hpp.

#include "glitch-scene-collada-gui/platform_stubs.hpp"

namespace glitch {

class IDevice;

namespace video {

// glitch-video/null_driver.hpp: CNullDriver* createNullDriver(IDevice*). Declared here because
// glitch-video is not on this category's include path.
class CNullDriver;
CNullDriver* createNullDriver(IDevice* device);

// _ZN6glitch5video21createOpenGLES2DriverEPNS_7IDeviceE. The symbol gives the IDevice* argument.
// The return type is not mapped: the callee is skipped in glitch-video's ledger (its
// COpenGLES2Driver class is not in the port). The result is taken as an opaque driver pointer.
void* createOpenGLES2Driver(IDevice* device);

}  // namespace video

// 0x6a0454 _ZN6glitch16CAndroidOSDevice12createDriverEv
// Partial. The control flow and side effects follow the assembly (IDA's pseudocode shows an
// unused second argument, but the symbol and the assembly take none):
//   - the type word at +0x5c selects the driver: 1 OpenGL ES 2, 0 null, 128 and 256 OSX-only;
//     any other value (including negative ones) logs an error;
//   - the driver is stored at +0x10 before it is tested, and returned on success;
//   - every failure path is a tail call to Printer::log at ELL_ERROR (level 3).
// Open points, which keep this row partial:
//   - createOpenGLES2Driver is not in the port (see above), so the ES2 branch depends on an
//     unmapped callee;
//   - the callees receive the device as IDevice*. The assembly passes this unadjusted, so the
//     IDevice base is at offset 0. CAndroidOSDevice does not declare its IDevice base yet, so
//     the conversion is a reinterpret_cast;
//   - the failure paths tail-call a void function, so the original returns whatever r0 holds
//     after Printer::log. That value is not defined. This body returns nullptr instead.
void* CAndroidOSDevice::createDriver() {
    const std::int32_t type = unk_005c;  // +0x5c
    if (type == 1) {
        void* driver = video::createOpenGLES2Driver(reinterpret_cast<IDevice*>(this));
        unk_0010 = driver;  // +0x10
        if (driver == nullptr) {
            os::Printer::log("No OpenGL support compiled in.", ELL_ERROR);
            return nullptr;
        }
        return driver;
    }
    if (type <= 1) {
        if (type != 0) {
            os::Printer::log("Unable to create video driver of unknown type.", ELL_ERROR);
            return nullptr;
        }
        video::CNullDriver* driver = video::createNullDriver(reinterpret_cast<IDevice*>(this));
        unk_0010 = driver;  // +0x10
        return driver;
    }
    if (type != 0x80 && type != 0x100) {
        os::Printer::log("Unable to create video driver of unknown type.", ELL_ERROR);
        return nullptr;
    }
    os::Printer::log("This driver is not available in OSX. Try OpenGL or Software renderer.", ELL_ERROR);
    return nullptr;
}

}  // namespace glitch
